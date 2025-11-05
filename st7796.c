/**
 ******************************************************************************
 * @file    st7796.c
 * @author  MCD Application Team
 * @brief   This file includes the driver for ST7796 LCD mounted on the Adafruit
 *          1.8" TFT LCD shield (reference ID 802).
 ******************************************************************************

 /* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "lcd.h"
#include "lcd_io.h"
#include "bmp.h"
#include "st7796.h"
#include "st7796_reg.h"

void st7796_Init(void);
uint32_t st7796_ReadID(void);
void st7796_DisplayOn(void);
void st7796_DisplayOff(void);
void st7796_SetCursor(uint16_t Xpos, uint16_t Ypos);
void st7796_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);
uint16_t st7796_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void st7796_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width,
		uint16_t Height);
void st7796_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos,
		uint16_t Length);
void st7796_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos,
		uint16_t Length);
void st7796_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize,
		uint16_t Ysize, uint16_t RGBCode);
uint16_t st7796_GetLcdPixelWidth(void);
uint16_t st7796_GetLcdPixelHeight(void);
void st7796_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
void st7796_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize,
		uint16_t Ysize, uint16_t *pData);
void st7796_ReadRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize,
		uint16_t Ysize, uint16_t *pData);
void st7796_Scroll(int16_t Scroll, uint16_t TopFix, uint16_t BottonFix);
void st7796_UserCommand(uint16_t Command, uint8_t *pData, uint32_t Size,
		uint8_t Mode);

LCD_DrvTypeDef st7796_drv = { st7796_Init, st7796_ReadID, st7796_DisplayOn,
		st7796_DisplayOff, st7796_SetCursor, st7796_WritePixel,
		st7796_ReadPixel, st7796_SetDisplayWindow, st7796_DrawHLine,
		st7796_DrawVLine, st7796_GetLcdPixelWidth, st7796_GetLcdPixelHeight,
		st7796_DrawBitmap, st7796_DrawRGBImage, st7796_FillRect,
		st7796_ReadRGBImage, st7796_Scroll, st7796_UserCommand };

#define TRANSDATAMAXSIZE  4
union {
	char c[TRANSDATAMAXSIZE];
	uint8_t d8[TRANSDATAMAXSIZE];
	uint16_t d16[TRANSDATAMAXSIZE / 2];
} transdata;

int32_t ST7796_Init(void) {
	if ((Is_st7796_Initialized & st7796_LCD_INITIALIZED) == 0) {
		Is_st7796_Initialized |= st7796_LCD_INITIALIZED;
		if ((Is_st7796_Initialized & st7796_IO_INITIALIZED) == 0)
			LCD_IO_Init();
		Is_st7796_Initialized |= st7796_IO_INITIALIZED;
	}

	LCD_Delay(120);

	/* software reset, 0 arguments, no delay */
	LCD_IO_WriteCmd8MultipleData8(ST7796_SW_RESET, NULL, 0);
	LCD_Delay(120);

	/* color mode (16 or 24 bit) */
#if ST7796_WRITEBITDEPTH == 16
	LCD_IO_WriteCmd8MultipleData8(ST7796_COLOR_MODE, (uint8_t*) "\x55", 1);
#elif ST7796_WRITEBITDEPTH == 24
		LCD_IO_WriteCmd8MultipleData8(ST7796_COLOR_MODE, (uint8_t *)"\x66", 1);
	#endif
	LCD_Delay(50);

	LCD_IO_WriteCmd8MultipleData8(ST7796_VERT_SCROLLING_ADDR, (uint8_t*) "\x00",
			1);
	LCD_IO_WriteCmd8MultipleData8(ST7796_MADCTL, &EntryRightThenDown, 1);

	/* Out of sleep mode, 0 arguments, no delay */
	LCD_IO_WriteCmd8MultipleData8(ST7796_SLEEP_OUT, NULL, 0);
	LCD_Delay(120);

	/* Enable extension command 2, 2 arguments, no delay*/
	LCD_IO_WriteCmd8MultipleData8(ST7796_COM_SET_CTRL, (uint8_t*) "\xC3\x96",
			2);

	/* Memory Data Access Control, 1 arguments, no delay  */
	/*	MX = 0, MY = 0: (0,0) is top left,  (319, 479) is bottom right
	 * RGB = 0 data is sent in RGB order
	 **/
	LCD_IO_WriteCmd8MultipleData8(ST7796_MADCTL, &EntryRightThenDown, 1);

	/* Display Inversion Control, 1 arguments, no delay */
	/* set 1-dot inversion reduces flicker */
	LCD_IO_WriteCmd8MultipleData8(ST7796_INV_CTRL, (uint8_t*) "\x01", 1);

	/* Display Function Control, 3 arguments, no delay */
	// Bypass
	//Source Output Scan from S1 to S960, Gate Output scan from G1 to G480, scan cycle=2
	// LCD Drive Line = 8*(59+1)
	LCD_IO_WriteCmd8MultipleData8(ST7796_DISPLAY_SETTING,
			(uint8_t*) "\x80\x02\x3B", 3);

	/* Display Output Control Adjust, 7 arguments, no delay*/
	LCD_IO_WriteCmd8MultipleData8(ST7796_DISP_CTRL_ADJ,
			(uint8_t*) "\x40\x8A\x00\x29\x19\xA5\x33", 8)

	/* Power Control 2, 1 arguments, no delay */
	//VAP(GVDD)=3.85+( vcom+vcom offset), VAN(GVCL)=-3.85+( vcom+vcom offset)
	LCD_IO_WriteCmd8MultipleData8(ST7796_PWR_CTRL2, (uint8_t*) "\x06", 1);

	/* Power Control 3, 1 arguments, no delay */
	// Source driving current level=low, Gamma driving current level=High
	LCD_IO_WriteCmd8MultipleData8(ST7796_PWR_CTRL3, (uint8_t*) "\xA7", 1);

	/* VCOM Control 1, 1 arguments, no delay */
	// VCOM=0.9
	LCD_IO_WriteCmd8MultipleData8(ST7796_VCOMH_VCOML_CTRL1, (uint8_t*) "\x18",
			1);

	LCD_Delay(120);

	/* Magical unicorn dust, 14 args, no delay */
	// ST7796 Gamma Sequence
	LCD_IO_WriteCmd8MultipleData8(ST7796_PV_GAMMA_CTRL,
			(uint8_t*) "\xF0\x09\x0B\x06\x04\x15\x2F\x54\x42\x3C\x17\x14\x18\x1B",
			14);
	LCD_IO_WriteCmd8MultipleData8(ST7796_NV_GAMMA_CTRL,
			(uint8_t*) "\xE0\x09\x0B\x06\x04\x03\x2B\x43\x42\x3B\x16\x14\x17\x1B",
			14);
	LCD_Delay(120);

	/* Command Set Control, 1 arguments, no delay */
	LCD_IO_WriteCmd8MultipleData8(ST7796_COM_SET_CTRL, (uint8_t*) "\x3C", 1);
	LCD_IO_WriteCmd8MultipleData8(ST7796_COM_SET_CTRL, (uint8_t*) "\x69", 1);

	LCD_Delay(pObj, 120);

	/* Display On, 1 arguments, no delay */
	/* Normal display on, no args, no delay */
	LCD_IO_WriteCmd8MultipleData8(ST7796_NORMAL_DISPLAY_OFF, NULL, 0);

	/* Main screen turn on, no delay */
	LCD_IO_WriteCmd8MultipleData8(ST7796_DISPLAY_ON, NULL, 0);

#if st7796_INITCLEAR == 1
		st7796_FillRect(0, 0, ST7796_SIZE_X, ST7796_SIZE_Y, 0x0000);
		LCD_Delay(10);
	#endif
}

//-----------------------------------------------------------------------------
/**
 * @brief  Enables the Display.
 * @param  None
 * @retval None
 */
void ST7796_DisplayOn(void) {
	LCD_IO_Bl_OnOff(1);
	LCD_IO_WriteCmd8MultipleData8(ST7796_SLEEP_OUT, NULL, 0); /* Exit Sleep */
}

//-----------------------------------------------------------------------------
/**
 * @brief  Disables the Display.
 * @param  None
 * @retval None
 */
void ST7796_DisplayOff(void) {
	LCD_IO_WriteCmd8MultipleData8(ST7796_SLEEP_IN, NULL, 0); /* Sleep */
	LCD_IO_Bl_OnOff(0);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Get the LCD pixel Width.
 * @param  None
 * @retval The Lcd Pixel Width
 */
uint16_t ST7796_GetLcdPixelWidth(void) {
	return ST7796_SIZE_X;
}

//-----------------------------------------------------------------------------
/**
 * @brief  Get the LCD pixel Height.
 * @param  None
 * @retval The Lcd Pixel Height
 */
uint16_t ST7796_GetLcdPixelHeight(void) {
	return ST7796_SIZE_Y;
}

//-----------------------------------------------------------------------------
/**
 * @brief  Get the ST7796 ID.
 * @param  None
 * @retval The ST7796 ID
 */
uint32_t ST7796_ReadID(void) {
	uint32_t dt = 0;
	LCD_IO_ReadCmd8MultipleData8(ST7796_READ_ID, (uint8_t*) &dt, 3, 1);
	return dt;
}
//-----------------------------------------------------------------------------
/**
 * @brief  Set Cursor position.
 * @param  Xpos: specifies the X position.
 * @param  Ypos: specifies the Y position.
 * @retval None
 */
void ST7796_SetCursor(uint16_t Xpos, uint16_t Ypos) {
	ST7796_SETCURSOR(Xpos, Ypos);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Write pixel.
 * @param  Xpos: specifies the X position.
 * @param  Ypos: specifies the Y position.
 * @param  RGBCode: the RGB pixel color
 * @retval None
 */
void ST7796_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode) {
	if (LastEntry != ST7796_MAD_DATA_RIGHT_THEN_DOWN) {
		LastEntry = ST7796_MAD_DATA_RIGHT_THEN_DOWN;
		LCD_IO_WriteCmd8MultipleData8(ST7796_MADCTL, &EntryRightThenDown, 1);
	}
	ST7796_SETCURSOR(Xpos, Ypos);
	LCD_IO_DrawFill(RGBCode, 1);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Read pixel.
 * @param  None
 * @retval the RGB pixel color
 */
uint16_t ST7796_ReadPixel(uint16_t Xpos, uint16_t Ypos) {
	uint16_t ret;
	if (LastEntry != ST7796_MAD_DATA_RIGHT_THEN_DOWN) {
		LastEntry = ST7796_MAD_DATA_RIGHT_THEN_DOWN;
		LCD_IO_WriteCmd8MultipleData8(ST7796_MADCTL, &EntryRightThenDown, 1);
	}
	ST7796_SETCURSOR(Xpos, Ypos);
	LCD_IO_ReadBitmap(&ret, 1);
	return (ret);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Sets a display window
 * @param  Xpos:   specifies the X bottom left position.
 * @param  Ypos:   specifies the Y bottom left position.
 * @param  Height: display window height.
 * @param  Width:  display window width.
 * @retval None
 */
void ST7796_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width,
		uint16_t Height) {
	yStart = Ypos;
	yEnd = Ypos + Height - 1;
	ST7796_SETWINDOW(Xpos, Xpos + Width - 1, Ypos, Ypos + Height - 1);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Draw vertical line.
 * @param  RGBCode: Specifies the RGB color
 * @param  Xpos:     specifies the X position.
 * @param  Ypos:     specifies the Y position.
 * @param  Length:   specifies the Line length.
 * @retval None
 */
void ST7796_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos,
		uint16_t Length) {
	if (LastEntry != ST7796_MAD_DATA_RIGHT_THEN_DOWN) {
		LastEntry = ST7796_MAD_DATA_RIGHT_THEN_DOWN;
		LCD_IO_WriteCmd8MultipleData8(ST7796_MADCTL, &EntryRightThenDown, 1);
	}
	ST7796_SETWINDOW(Xpos, Xpos + Length - 1, Ypos, Ypos);
	LCD_IO_DrawFill(RGBCode, Length);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Draw vertical line.
 * @param  RGBCode: Specifies the RGB color
 * @param  Xpos:     specifies the X position.
 * @param  Ypos:     specifies the Y position.
 * @param  Length:   specifies the Line length.
 * @retval None
 */
void ST7796_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos,
		uint16_t Length) {
	if (LastEntry != ST7796_MAD_DATA_RIGHT_THEN_DOWN) {
		LastEntry = ST7796_MAD_DATA_RIGHT_THEN_DOWN;
		LCD_IO_WriteCmd8MultipleData8(ST7796_MADCTL, &EntryRightThenDown, 1);
	}
	ST7796_SETWINDOW(Xpos, Xpos, Ypos, Ypos + Length - 1);
	LCD_IO_DrawFill(RGBCode, Length);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Draw Filled rectangle
 * @param  Xpos:     specifies the X position.
 * @param  Ypos:     specifies the Y position.
 * @param  Xsize:    specifies the X size
 * @param  Ysize:    specifies the Y size
 * @param  RGBCode:  specifies the RGB color
 * @retval None
 */
void ST7796_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize,
		uint16_t Ysize, uint16_t RGBCode) {
	if (LastEntry != ST7796_MAD_DATA_RIGHT_THEN_DOWN) {
		LastEntry = ST7796_MAD_DATA_RIGHT_THEN_DOWN;
		LCD_IO_WriteCmd8MultipleData8(ST7796_MADCTL, &EntryRightThenDown, 1);
	}
	ST7796_SETWINDOW(Xpos, Xpos + Xsize - 1, Ypos, Ypos + Ysize - 1);
	LCD_IO_DrawFill(RGBCode, Xsize * Ysize);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Displays a 16bit bitmap picture..
 * @param  BmpAddress: Bmp picture address.
 * @param  Xpos:  Bmp X position in the LCD
 * @param  Ypos:  Bmp Y position in the LCD
 * @retval None
 * @brief  Draw direction: right then up
 */
void ST7796_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp) {
	uint32_t index, size;
	/* Read bitmap size */
	size = ((BITMAPSTRUCT*) pbmp)->fileHeader.bfSize;
	/* Get bitmap data address offset */
	index = ((BITMAPSTRUCT*) pbmp)->fileHeader.bfOffBits;
	size = (size - index) / 2;
	pbmp += index;

	if (LastEntry != ST7796_MAD_DATA_RIGHT_THEN_UP) {
		LastEntry = ST7796_MAD_DATA_RIGHT_THEN_UP;
		LCD_IO_WriteCmd8MultipleData8(ST7796_MADCTL, &EntryRightThenUp, 1);
	}
	transdata.d16[0] = __REVSH(ST7796_SIZE_Y - 1 - yEnd);
	transdata.d16[1] = __REVSH(ST7796_SIZE_Y - 1 - yStart);
	LCD_IO_WriteCmd8MultipleData8(ST7796_RASET, &transdata, 4);
	LCD_IO_DrawBitmap(pbmp, size);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Displays 16bit/pixel picture..
 * @param  pdata: picture address.
 * @param  Xpos:  Image X position in the LCD
 * @param  Ypos:  Image Y position in the LCD
 * @param  Xsize: Image X size in the LCD
 * @param  Ysize: Image Y size in the LCD
 * @retval None
 * @brief  Draw direction: right then down
 */
void ST7796_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize,
		uint16_t Ysize, uint16_t *pData) {
	if (LastEntry != ST7796_MAD_DATA_RIGHT_THEN_DOWN) {
		LastEntry = ST7796_MAD_DATA_RIGHT_THEN_DOWN;
		LCD_IO_WriteCmd8MultipleData8(ST7796_MADCTL, &EntryRightThenDown, 1);
	}
	ST7796_SetDisplayWindow(Xpos, Ypos, Xsize, Ysize);
	LCD_IO_DrawBitmap(pData, Xsize * Ysize);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Read 16bit/pixel picture from Lcd and store to RAM
 * @param  pdata: picture address.
 * @param  Xpos:  Image X position in the LCD
 * @param  Ypos:  Image Y position in the LCD
 * @param  Xsize: Image X size in the LCD
 * @param  Ysize: Image Y size in the LCD
 * @retval None
 * @brief  Draw direction: right then down
 */
void ST7796_ReadRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize,
		uint16_t Ysize, uint16_t *pData) {
	if (LastEntry != ST7796_MAD_DATA_RIGHT_THEN_DOWN) {
		LastEntry = ST7796_MAD_DATA_RIGHT_THEN_DOWN;
		LCD_IO_WriteCmd8MultipleData8(ST7796_MADCTL, &EntryRightThenDown, 1);
	}
	ST7796_SetDisplayWindow(Xpos, Ypos, Xsize, Ysize);
	LCD_IO_ReadBitmap(pData, Xsize * Ysize);
}

//-----------------------------------------------------------------------------
/**
 * @brief  Set display scroll parameters
 * @param  Scroll    : Scroll size [pixel]
 * @param  TopFix    : Top fix size [pixel]
 * @param  BottonFix : Botton fix size [pixel]
 * @retval None
 */
void ST7796_Scroll(int16_t Scroll, uint16_t TopFix, uint16_t BottonFix) {
	static uint16_t scrparam[4] = { 0, 0, 0, 0 };
#if (ST7796_ORIENTATION == 0)
	if ((TopFix != __REVSH(scrparam[1]))
			|| (BottonFix != __REVSH(scrparam[3]))) {
		scrparam[3] = __REVSH(TopFix);
		scrparam[1] = __REVSH(BottonFix);
		scrparam[2] = __REVSH(ST7796_LCD_PIXEL_HEIGHT - TopFix - BottonFix);
		LCD_IO_WriteCmd8MultipleData8(ST7796_VERT_SCROLLING_DEF, &scrparam[1],
				6);
	}
	Scroll = (0 - Scroll) % __REVSH(scrparam[2]);
	if (Scroll < 0)
		Scroll = __REVSH(scrparam[2]) + Scroll + __REVSH(scrparam[1]);
	else
		Scroll = Scroll + __REVSH(scrparam[1]);
#elif (ST7796_ORIENTATION == 1)
  if((TopFix != __REVSH(scrparam[3])) || (BottonFix != __REVSH(scrparam[1])))
  {
    scrparam[3] = __REVSH(TopFix);
    scrparam[1] = __REVSH(BottonFix);
    scrparam[2] = __REVSH(ST7796_LCD_PIXEL_HEIGHT - TopFix - BottonFix);
    LCD_IO_WriteCmd8MultipleData8(ST7796_VERT_SCROLLING_DEF, &scrparam[1], 6);
  }
  Scroll %= __REVSH(scrparam[2]);
  if(Scroll < 0)
    Scroll = __REVSH(scrparam[2]) + Scroll + __REVSH(scrparam[1]);
  else
    Scroll = Scroll + __REVSH(scrparam[1]);
  #elif (ST7796_ORIENTATION == 2)
  if((TopFix != __REVSH(scrparam[1])) || (BottonFix != __REVSH(scrparam[3])))
  {
    scrparam[1] = __REVSH(TopFix);
    scrparam[3] = __REVSH(BottonFix);
    scrparam[2] = __REVSH(ST7796_LCD_PIXEL_HEIGHT - TopFix - BottonFix);
    LCD_IO_WriteCmd8MultipleData8(ST7796_VERT_SCROLLING_DEF, &scrparam[1], 6);
  }
  Scroll %= __REVSH(scrparam[2]);
  if(Scroll < 0)
    Scroll = __REVSH(scrparam[2]) + Scroll + __REVSH(scrparam[1]);
  else
    Scroll = Scroll + __REVSH(scrparam[1]);
  #elif (ST7796_ORIENTATION == 3)
  if((TopFix != __REVSH(scrparam[1])) || (BottonFix != __REVSH(scrparam[3])))
  {
    scrparam[1] = __REVSH(TopFix);
    scrparam[3] = __REVSH(BottonFix);
    scrparam[2] = __REVSH(ST7796_LCD_PIXEL_HEIGHT - TopFix - BottonFix);
    LCD_IO_WriteCmd8MultipleData8(ST7796_VERT_SCROLLING_DEF, &scrparam[1], 6);
  }
  Scroll = (0 - Scroll) % __REVSH(scrparam[2]);
  if(Scroll < 0)
    Scroll = __REVSH(scrparam[2]) + Scroll + __REVSH(scrparam[1]);
  else
    Scroll = Scroll + __REVSH(scrparam[1]);
  #endif
	if (Scroll != __REVSH(scrparam[0])) {
		scrparam[0] = __REVSH(Scroll);
		LCD_IO_WriteCmd8MultipleData8(ST7796_VERT_SCROLLING_ADDR, &scrparam[0],
				2);
	}
}

//-----------------------------------------------------------------------------
/**
 * @brief  User command
 * @param  Command   : Lcd command
 * @param  pData     : data pointer
 * @param  Size      : data number
 * @param  Mode      : 0=write 8bits datas, 1=0=write 16bits datas, 2=read 8bits datas, 3=read 16bits datas
 * @retval None
 */
void ST7796_UserCommand(uint16_t Command, uint8_t *pData, uint32_t Size,
		uint8_t Mode) {
	if (Mode == 0)
		LCD_IO_WriteCmd8MultipleData8((uint8_t) Command, pData, Size);
	else if (Mode == 1)
		LCD_IO_WriteCmd8MultipleData16((uint8_t) Command, pData, Size);
	else if (Mode == 2)
		LCD_IO_ReadCmd8MultipleData8((uint8_t) Command, pData, Size, 1);
	else if (Mode == 3)
		LCD_IO_ReadCmd8MultipleData16((uint8_t) Command, pData, Size, 1);
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

