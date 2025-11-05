/**
 ******************************************************************************
 * @file    st7796.h
 * @author  MCD Application Team
 * @brief   This file contains all the configuration options for the st7796
 *          driver.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef ST7796_H
#define ST7796_H

/* Orientation
 - 0: 240x320 portrait (plug in top)
 - 1: 320x240 landscape (plug in left)
 - 2: 240x320 portrait (plug in bottom)
 - 3: 320x240 landscape (plug in right) */
#define  ST7796_ORIENTATION             0

/* To clear the screen before display turning on ?
 - 0: does not clear
 - 1: clear */
#define  ST7796_INITCLEAR               1

/* Color order (0 = RGB, 1 = BGR) */
#define  ST7796_COLORMODE               0

/* Draw and read bitdeph (16: RGB565, 24: RGB888)
 note: my SPI ST7796 LCD only readable if ST7796_READBITDEPTH 24 */
#define  ST7796_WRITEBITDEPTH           16
#define  ST7796_READBITDEPTH            24

// ILI9341 physic resolution (in 0 orientation)
#define  ST7796_LCD_PIXEL_WIDTH         320U
#define  ST7796_LCD_PIXEL_HEIGHT        480U

#define ST7796_SETWINDOW(x1, x2, y1, y2) \
  { transdata.d16[0] = __REVSH(x1); transdata.d16[1] = __REVSH(x2); LCD_IO_WriteCmd8MultipleData8(ST7796_CASET, &transdata, 4); \
    transdata.d16[0] = __REVSH(y1); transdata.d16[1] = __REVSH(y2); LCD_IO_WriteCmd8MultipleData8(ST7796_PASET, &transdata, 4); }

#define ST7796_SETCURSOR(x, y)            ST7796_SETWINDOW(x, x, y, y)

//-----------------------------------------------------------------------------
#define ST7796_LCD_INITIALIZED    0x01
#define ST7796_IO_INITIALIZED     0x02
static uint8_t Is_ST7796_Initialized = 0;

const uint8_t EntryRightThenUp = ST7796_MAD_DATA_RIGHT_THEN_UP;
const uint8_t EntryRightThenDown = ST7796_MAD_DATA_RIGHT_THEN_DOWN;

/* the last set drawing direction is stored here */
uint8_t LastEntry = ST7796_MAD_DATA_RIGHT_THEN_DOWN;

static uint16_t yStart, yEnd;

//-----------------------------------------------------------------------------
/* Pixel draw and read functions */

void st7796_Init(void);
uint32_t st7796_ReadID(void);
void st7796_DisplayOn(void);
void st7796_DisplayOff(void);
void st7796_SetCursor(uint16_t Xpos, uint16_t Ypos);
void st7796_WritePixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGB_Code);
uint16_t st7796_ReadPixel(uint16_t Xpos, uint16_t Ypos);
void st7796_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint16_t Width,uint16_t Height);
void st7796_DrawHLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void st7796_DrawVLine(uint16_t RGBCode, uint16_t Xpos, uint16_t Ypos, uint16_t Length);
void st7796_FillRect(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t RGBCode);
uint16_t st7796_GetLcdPixelWidth(void);
uint16_t st7796_GetLcdPixelHeight(void);
void st7796_DrawBitmap(uint16_t Xpos, uint16_t Ypos, uint8_t *pbmp);
void st7796_DrawRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t *pData);
void st7796_ReadRGBImage(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t *pData);
void st7796_Scroll(int16_t Scroll, uint16_t TopFix, uint16_t BottonFix);
void st7796_UserCommand(uint16_t Command, uint8_t *pData, uint32_t Size, uint8_t Mode);

#if ST7796_WRITEBITDEPTH == ST7796_READBITDEPTH
/* 16/16 and 24/24 bit, no need to change bitdepth data */
#define SetWriteDir()
#define SetReadDir()
#else /* #if ST7796_WRITEBITDEPTH == ST7796_READBITDEPTH */
uint8_t lastdir = 0;
#if ST7796_WRITEBITDEPTH == 16
/* 16/24 bit */
#define SetWriteDir() {                                      \
  if(lastdir != 0)                                           \
  {                                                          \
    LCD_IO_WriteCmd8MultipleData8(ST7796_COLORMODE, "\55", 1);  \
    lastdir = 0;                                             \
  }                                                          }
#define SetReadDir() {                                       \
  if(lastdir == 0)                                           \
  {                                                          \
    LCD_IO_WriteCmd8MultipleData8(ST7796_COLORMODE, "\66", 1);  \
    lastdir = 1;                                             \
  }                                                          }
#elif ST7796_WRITEBITDEPTH == 24
/* 24/16 bit */
#define SetWriteDir() {                                      \
  if(lastdir != 0)                                           \
  {                                                          \
    LCD_IO_WriteCmd8MultipleData8(ST7796_COLORMODE, "\66", 1);  \
    lastdir = 0;                                             \
  }                                                          }
#define SetReadDir() {                                       \
  if(lastdir == 0)                                           \
  {                                                          \
    LCD_IO_WriteCmd8MultipleData8(ST7796_COLORMODE, "\55", 1);  \
    lastdir = 1;                                             \
  }                                                          }
#endif /* #elif ILI9488_WRITEBITDEPTH == 24 */
#endif /* #else ILI9488_WRITEBITDEPTH == ILI9488_READBITDEPTH */

#if ST7796_WRITEBITDEPTH == 16
#define  LCD_IO_DrawFill(Color, Size) { \
  SetWriteDir(); \
  LCD_IO_WriteCmd8DataFill16(ST7796_WRITE_RAM, Color, Size); }            /* Fill 16 bit pixel(s) */
#define  LCD_IO_DrawBitmap(pData, Size) { \
  SetWriteDir(); \
  LCD_IO_WriteCmd8MultipleData16(ST7796_WRITE_RAM, pData, Size); }        /* Draw 16 bit bitmap */
#elif ST7796_WRITEBITDEPTH == 24
#define  LCD_IO_DrawFill(Color, Size) { \
  SetWriteDir(); \
  LCD_IO_WriteCmd8DataFill16to24(ST7796_WRITE_RAM, Color, Size); }        /* Fill 24 bit pixel(s) from 16 bit color code */
#define  LCD_IO_DrawBitmap(pData, Size) { \
  SetWriteDir(); \
  LCD_IO_WriteCmd8MultipleData16to24(ST7796_WRITE_RAM, pData, Size); }    /* Draw 24 bit Lcd bitmap from 16 bit bitmap data */
#endif /* #elif ST7796_WRITEBITDEPTH == 24 */

#if ST7796_READBITDEPTH == 16
#define  LCD_IO_ReadBitmap(pData, Size) { \
  SetReadDir(); \
  LCD_IO_ReadCmd8MultipleData16(ST7796_READ_RAM, pData, Size, 1); }      /* Read 16 bit LCD */
#elif ST7796_READBITDEPTH == 24
#define  LCD_IO_ReadBitmap(pData, Size) { \
  SetReadDir(); \
  LCD_IO_ReadCmd8MultipleData24to16(ST7796_READ_RAM, pData, Size, 1); }  /* Read 24 bit Lcd and convert to 16 bit bitmap */
#endif /* #elif ST7796_READBITDEPTH == 24 */

#endif /* ST7796_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
