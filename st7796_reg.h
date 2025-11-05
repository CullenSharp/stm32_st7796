/**
  ******************************************************************************
  * @file    st7796_reg.h
  * @author  MCD Application Team
  * @brief   This file contains all the functions prototypes for the st7796_regs.c
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
#ifndef ST7796_REG_H
#define ST7796_REG_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** 
  * @brief  ST7796 Registers
  */
#define ST7796_NOP                          0x00U  /* No Operation: NOP                           */
#define ST7796_SW_RESET                     0x01U  /* Software reset: SWRESET                     */
#define ST7796_READ_ID                      0x04U  /* Read Display ID: RDDID                      */
#define ST7796_READ_STATUS                  0x09U  /* Read Display Status: RDDST                  */
#define ST7796_READ_POWER_MODE              0x0AU  /* Read Display Power: RDDPM                   */
#define ST7796_READ_MADCTL                  0x0BU  /* Read Display: RDDMADCTL                     */
#define ST7796_READ_PIXEL_FORMAT            0x0CU  /* Read Display Pixel: RDPIXFMT               */
#define ST7796_READ_IMAGE_MODE              0x0DU  /* Read Display Image: RDDIM                   */
#define ST7796_READ_SIGNAL_MODE             0x0EU  /* Read Display Signal: RDDSM                  */
#define ST7796_SLEEP_IN                     0x10U  /* Sleep in & booster off: SLPIN               */
#define ST7796_SLEEP_OUT                    0x11U  /* Sleep out & booster on: SLPOUT              */
#define ST7796_PARTIAL_DISPLAY_ON           0x12U  /* Partial mode on: PTLON                      */
#define ST7796_NORMAL_DISPLAY_OFF           0x13U  /* Partial off (Normal): NORON                 */
#define ST7796_DISPLAY_INVERSION_OFF        0x20U  /* Display inversion off: INVOFF               */
#define ST7796_DISPLAY_INVERSION_ON         0x21U  /* Display inversion on: INVON                 */
#define ST7796_DISPLAY_OFF					0x28U  /* Display off: DISPOFF						  */
#define ST7796_DISPLAY_ON					0x29U  /* Display on: DISPON						  */
#define ST7796_CASET                        0x2AU  /* Column address set: CASET                   */
#define ST7796_RASET                        0x2BU  /* Row address set: RASET                      */
#define ST7796_WRITE_RAM                    0x2CU  /* Memory write: RAMWR                         */
#define ST7796_READ_RAM                     0x2EU  /* Memory read: RAMRD                          */
#define ST7796_PTLAR                        0x30U  /* Partial start/end address set: PTLAR        */
#define ST7796_VERT_SCROLLING_DEF			0x33U  /* Vertical Scrolling Definition: VSCRDEF	  */
#define ST7796_VERT_SCROLLING_ADDR			0x37U  /* Vertical Scrolling Start Address: VSCRSADD  */
#define ST7796_TE_LINE_OFF                  0x34U  /* Tearing effect line off: TEOFF              */
#define ST7796_TE_LINE_ON                   0x35U  /* Tearing effect mode set & on: TEON          */
#define ST7796_MADCTL                       0x36U  /* Memory data access control: MADCTL          */
#define ST7796_IDLE_MODE_OFF                0x38U  /* Idle mode off: IDMOFF                       */
#define ST7796_IDLE_MODE_ON                 0x39U  /* Idle mode on: IDMON                         */
#define ST7796_COLOR_MODE                   0x3AU  /* Interface pixel format: COLMOD              */
#define ST7796_FRAME_RATE_CTRL1             0xB1U  /* In normal mode (Full colors): FRMCTR1       */
#define ST7796_FRAME_RATE_CTRL2             0xB2U  /* In Idle mode (8-colors): FRMCTR2            */
#define ST7796_FRAME_RATE_CTRL3             0xB3U  /* In partial mode + Full colors: FRMCTR3      */
#define ST7796_INV_CTRL						0xB4U  /* Display Inversion Control: INVTR			  */
#define ST7796_DISPLAY_SETTING              0xB6U  /* Display function control: DFC               */
#define ST7796_PWR_CTRL1                    0xC0U  /* Power control setting: PWR1                 */
#define ST7796_PWR_CTRL2                    0xC1U  /* Power control setting: PWR2                 */
#define ST7796_PWR_CTRL3                    0xC2U  /* In normal mode (Full colors): PWR3          */
#define ST7796_VCOMH_VCOML_CTRL1            0xC5U  /* VCOM control 1: VMCTR1                      */
#define ST7796_VMOF_CTRL                    0xC6U  /* Set VCOM offset control: VCM Offset         */
#define ST7796_NVM_BYTE_PROGRAM             0xD1U  /* Set LCM version code: NVMBPROG              */
#define ST7796_NVM_STATUS_READ              0xD2U  /* NVM status read: NVMSTRD                    */
#define ST7796_NV_CTRL1                     0xD9U  /* NVM control status: NVCTR1                  */
#define ST7796_READ_ID1                     0xDAU  /* Read ID1: RDID1                             */
#define ST7796_READ_ID2                     0xDBU  /* Read ID2: RDID2                             */
#define ST7796_READ_ID3                     0xDCU  /* Read ID3: RDID3                             */
#define ST7796_NV_CTRL2                     0xDEU  /* NVM Read Command: NVCTR2                    */
#define ST7796_NV_CTRL3                     0xDFU  /* NVM Write Command: NVCTR3                   */
#define ST7796_PV_GAMMA_CTRL                0xE0U  /* Set Gamma adjustment (+ polarity): PGC 	  */
#define ST7796_NV_GAMMA_CTRL                0xE1U  /* Set Gamma adjustment (- polarity): NGC 	  */
#define ST7796_DISP_CTRL_ADJ				0xE8U  /* Display Output Ctrl Adjust: DOCA			  */
#define ST7796_COM_SET_CTRL                 0xF0U  /* Command set control: CSCON                  */

//-----------------------------------------------------------------------------
#define ST7796_MAD_RGB        0x00
#define ST7796_MAD_BGR        0x08

#define ST7796_MAD_VERTICAL   0x20
#define ST7796_MAD_HORIZONTAL 0x00
#define ST7796_MAD_X_LEFT     0x00
#define ST7796_MAD_X_RIGHT    0x40
#define ST7796_MAD_Y_UP       0x00
#define ST7796_MAD_Y_DOWN     0x80

#if ST7796_COLORMODE == 0
#define ST7796_MAD_COLORMODE  ST7796_MAD_RGB
#else
#define ST7796_MAD_COLORMODE  ST7796_MAD_BGR
#endif

#if (ST7796_ORIENTATION == 0)
#define ST7796_SIZE_X                     ST7796_LCD_PIXEL_WIDTH
#define ST7796_SIZE_Y                     ST7796_LCD_PIXEL_HEIGHT
#define ST7796_MAD_DATA_RIGHT_THEN_UP     (ST7796_MAD_COLORMODE | ST7796_MAD_X_RIGHT | ST7796_MAD_Y_UP | ST7796_MAD_HORIZONTAL)
#define ST7796_MAD_DATA_RIGHT_THEN_DOWN   (ST7796_MAD_COLORMODE | ST7796_MAD_X_RIGHT | ST7796_MAD_Y_DOWN | ST7796_MAD_HORIZONTAL)
#define XSIZE                              Xsize
#define YSIZE                              Ysize
#elif (ST7796_ORIENTATION == 1)
#define ST7796_SIZE_X                     ST7796_LCD_PIXEL_HEIGHT
#define ST7796_SIZE_Y                     ST7796_LCD_PIXEL_WIDTH
#define ST7796_MAD_DATA_RIGHT_THEN_UP     (ST7796_MAD_COLORMODE | ST7796_MAD_X_RIGHT | ST7796_MAD_Y_DOWN | ST7796_MAD_VERTICAL)
#define ST7796_MAD_DATA_RIGHT_THEN_DOWN   (ST7796_MAD_COLORMODE | ST7796_MAD_X_LEFT  | ST7796_MAD_Y_DOWN | ST7796_MAD_VERTICAL)
#define XSIZE                              Ysize
#define YSIZE                              Xsize
#elif (ST7796_ORIENTATION == 2)
#define ST7796_SIZE_X                     ST7796_LCD_PIXEL_WIDTH
#define ST7796_SIZE_Y                     ST7796_LCD_PIXEL_HEIGHT
#define ST7796_MAD_DATA_RIGHT_THEN_UP     (ST7796_MAD_COLORMODE | ST7796_MAD_X_LEFT  | ST7796_MAD_Y_DOWN | ST7796_MAD_HORIZONTAL)
#define ST7796_MAD_DATA_RIGHT_THEN_DOWN   (ST7796_MAD_COLORMODE | ST7796_MAD_X_LEFT  | ST7796_MAD_Y_UP | ST7796_MAD_HORIZONTAL)
#define XSIZE                              Xsize
#define YSIZE                              Ysize
#elif (ST7796_ORIENTATION == 3)
#define ST7796_SIZE_X                     ST7796_LCD_PIXEL_HEIGHT
#define ST7796_SIZE_Y                     ST7796_LCD_PIXEL_WIDTH
#define ST7796_MAD_DATA_RIGHT_THEN_UP     (ST7796_MAD_COLORMODE | ST7796_MAD_X_LEFT  | ST7796_MAD_Y_UP | ST7796_MAD_VERTICAL)
#define ST7796_MAD_DATA_RIGHT_THEN_DOWN   (ST7796_MAD_COLORMODE | ST7796_MAD_X_RIGHT | ST7796_MAD_Y_UP | ST7796_MAD_VERTICAL)
#define XSIZE                              Ysize
#define YSIZE                              Xsize
#endif

#endif /* ST7796_REG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
