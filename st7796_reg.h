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

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>

/** 
  * @brief  ST7735 Registers  
  */
#define ST7796_NOP                          0x00U  /* No Operation: NOP                           */
#define ST7796_SW_RESET                     0x01U  /* Software reset: SWRESET                     */
#define ST7796_READ_ID                      0x04U  /* Read Display ID: RDDID                      */
#define ST7796_READ_STATUS                  0x09U  /* Read Display Status: RDDST                  */
#define ST7796_READ_POWER_MODE              0x0AU  /* Read Display Power: RDDPM                   */
#define ST7796_READ_MADCTL                  0x0BU  /* Read Display: RDDMADCTL                     */
#define ST7796_READ_PIXEL_FORMAT            0x0CU  /* Read Display Pixel: RDDCOLMOD               */
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
#define ST7796_TE_LINE_OFF                  0x34U  /* Tearing effect line off: TEOFF              */
#define ST7796_TE_LINE_ON                   0x35U  /* Tearing effect mode set & on: TEON          */
#define ST7796_MADCTL                       0x36U  /* Memory data access control: MADCTL          */
#define ST7796_IDLE_MODE_OFF                0x38U  /* Idle mode off: IDMOFF                       */
#define ST7796_IDLE_MODE_ON                 0x39U  /* Idle mode on: IDMON                         */
#define ST7796_COLOR_MODE                   0x3AU  /* Interface pixel format: COLMOD              */
#define ST7796_FRAME_RATE_CTRL1             0xB1U  /* In normal mode (Full colors): FRMCTR1       */
#define ST7796_FRAME_RATE_CTRL2             0xB2U  /* In Idle mode (8-colors): FRMCTR2            */
#define ST7796_FRAME_RATE_CTRL3             0xB3U  /* In partial mode + Full colors: FRMCTR3      */
#define ST7796_DISPLAY_SETTING              0xB6U  /* Display function setting                    */
#define ST7796_PWR_CTRL1                    0xC0U  /* Power control setting: PWR1                 */
#define ST7796_PWR_CTRL2                    0xC1U  /* Power control setting: PWR2                 */
#define ST7796_PWR_CTRL3                    0xC2U  /* In normal mode (Full colors): PWR3          */
#define ST7796_VCOMH_VCOML_CTRL1            0xC5U  /* VCOM control 1: VMCTR1                      */
#define ST7796_VMOF_CTRL                    0xC6U  /* Set VCOM offset control: VMOFCTR            */
#define ST7796_NVM_BYTE_PROGRAM             0xD1U  /* Set LCM version code: NVMBPROG              */
#define ST7796_NVM_STATUS_READ              0xD2U  /* NVM status read: NVMSTRD                    */
#define ST7796_NV_CTRL1                     0xD9U  /* NVM control status: NVCTR1                  */
#define ST7796_READ_ID1                     0xDAU  /* Read ID1: RDID1                             */
#define ST7796_READ_ID2                     0xDBU  /* Read ID2: RDID2                             */
#define ST7796_READ_ID3                     0xDCU  /* Read ID3: RDID3                             */
#define ST7735_NV_CTRL2                     0xDEU  /* NVM Read Command: NVCTR2                    */ 
#define ST7735_NV_CTRL3                     0xDFU  /* NVM Write Command: NVCTR3                   */
#define ST7796_PV_GAMMA_CTRL                0xE0U  /* Set Gamma adjustment (+ polarity): PGC 	  */
#define ST7796_NV_GAMMA_CTRL                0xE1U  /* Set Gamma adjustment (- polarity): NGC 	  */
#define ST7796_COM_SET_CTRL                 0xF0U  /* Command set control: CSCON                  */

/**
  * @}
  */
  
/** @defgroup ST7735_REG_Exported_Types Exported Types
  * @{
  */ 
typedef int32_t (*ST7796_Write_Func)(void *, uint8_t, uint8_t*, uint32_t);
typedef int32_t (*ST7796_Read_Func) (void *, uint8_t, uint8_t*);
typedef int32_t (*ST7796_Send_Func) (void *, uint8_t*, uint32_t);
typedef int32_t (*ST7796_Recv_Func) (void *, uint8_t*, uint32_t);

typedef struct
{
  ST7796_Write_Func   WriteReg;
  ST7796_Read_Func    ReadReg;
  ST7796_Send_Func    SendData;
  ST7796_Recv_Func    RecvData;
  void                *handle;
} st7796_ctx_t;

/**
  * @}
  */

/** @defgroup ST7735_REG_Exported_Functions Exported Functions
  * @{
  */ 
int32_t st7796_write_reg(st7735_ctx_t *ctx, uint8_t reg, uint8_t *pdata, uint32_t length);
int32_t st7796_read_reg(st7735_ctx_t *ctx, uint8_t reg, uint8_t *pdata);
int32_t st7796_send_data(st7735_ctx_t *ctx, uint8_t *pdata, uint32_t length);
int32_t st7796_recv_data(st7735_ctx_t *ctx, uint8_t *pdata, uint32_t length);

/**
  * @}
  */ 
      
#ifdef __cplusplus
}
#endif

#endif /* ST7796_REG_H */

/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
