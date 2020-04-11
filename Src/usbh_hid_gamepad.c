/**
  ******************************************************************************
  * @file    usbh_hid_keybd.c
  * @author  MCD Application Team
  * @brief   This file is the application layer for USB Host HID Keyboard handling
  *          QWERTY and AZERTY Keyboard are supported as per the selection in
  *          usbh_hid_keybd.h
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* BSPDependencies
- "stm32xxxxx_{eval}{discovery}{nucleo_144}.c"
- "stm32xxxxx_{eval}{discovery}_io.c"
- "stm32xxxxx_{eval}{discovery}{adafruit}_lcd.c"
- "stm32xxxxx_{eval}{discovery}_sdram.c"
EndBSPDependencies */

/* Includes ------------------------------------------------------------------*/
#include "usbh_hid_gamepad.h"
#include "usbh_hid_parser.h"
#include <stdio.h>
/** @addtogroup USBH_LIB
* @{
*/

/** @addtogroup USBH_CLASS
* @{
*/

/** @addtogroup USBH_HID_CLASS
* @{
*/

/** @defgroup USBH_HID_KEYBD
* @brief    This file includes HID Layer Handlers for USB Host HID class.
* @{
*/

/** @defgroup USBH_HID_KEYBD_Private_TypesDefinitions
* @{
*/
/**
* @}
*/


/** @defgroup USBH_HID_KEYBD_Private_Defines
* @{
*/
/**
* @}
*/
#ifndef AZERTY_KEYBOARD
#define QWERTY_KEYBOARD
#endif
#define  KBD_LEFT_CTRL                                  0x01
#define  KBD_LEFT_SHIFT                                 0x02
#define  KBD_LEFT_ALT                                   0x04
#define  KBD_LEFT_GUI                                   0x08
#define  KBD_RIGHT_CTRL                                 0x10
#define  KBD_RIGHT_SHIFT                                0x20
#define  KBD_RIGHT_ALT                                  0x40
#define  KBD_RIGHT_GUI                                  0x80
#define  KBR_MAX_NBR_PRESSED                            6

/** @defgroup USBH_HID_KEYBD_Private_Macros
* @{
*/
/**
* @}
*/

/** @defgroup USBH_HID_KEYBD_Private_FunctionPrototypes
* @{
*/
static USBH_StatusTypeDef USBH_HID_PadDecode(USBH_HandleTypeDef *phost);
/**
* @}
*/

/** @defgroup USBH_HID_KEYBD_Private_Variables
* @{
*/

HID_PAD_Info_TypeDef     pad_info;
uint32_t                   pad_rx_report_buf[2];
uint32_t                   pad_report_data[8];



static const HID_Report_ItemTypedef gp_buttons_key_array =
{
  (uint8_t *)(void *)pad_report_data + 0, /*data*/
  8,     /*size*/
  0,     /*shift*/
  10,     /*count (only for array items)*/
  0,     /*signed?*/
  0,     /*min value read can return*/
  255,   /*max value read can return*/
  0,     /*min vale device can report*/
  255,   /*max value device can report*/
  1      /*resolution*/
};

/**
  * @brief  USBH_HID_KeybdInit
  *         The function init the HID keyboard.
  * @param  phost: Host handle
  * @retval USBH Status
  */
USBH_StatusTypeDef USBH_HID_PadInit(USBH_HandleTypeDef *phost)
{
  uint32_t x;
  HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;

    for (x = 0U; x < (sizeof(pad_report_data) / sizeof(uint32_t)); x++)
  {
    pad_report_data[x] = 0U;
    pad_rx_report_buf[x] = 0U;
  }

  if (HID_Handle->length > (sizeof(pad_report_data)))
  {
    HID_Handle->length = (sizeof(pad_report_data));
  }
  HID_Handle->pData = (uint8_t *)(void *)pad_rx_report_buf;
  USBH_HID_FifoInit(&HID_Handle->fifo, phost->device.Data, HID_QUEUE_SIZE * sizeof(pad_report_data));

  return USBH_OK;
}

/**
  * @brief  USBH_HID_GetKeybdInfo
  *         The function return keyboard information.
  * @param  phost: Host handle
  * @retval keyboard information
  */
HID_PAD_Info_TypeDef *USBH_HID_GetPadInfo(USBH_HandleTypeDef *phost)
{
  if (USBH_HID_PadDecode(phost) == USBH_OK)
  {
    return &pad_info;
  }
  else
  {
    return NULL;
  }
}

/**
  * @brief  USBH_HID_KeybdDecode
  *         The function decode keyboard data.
  * @param  phost: Host handle
  * @retval USBH Status
  */
static USBH_StatusTypeDef USBH_HID_PadDecode(USBH_HandleTypeDef *phost)
{
  uint8_t x;

  HID_HandleTypeDef *HID_Handle = (HID_HandleTypeDef *) phost->pActiveClass->pData;
  if (HID_Handle->length == 0U)
  {
    return USBH_FAIL;
  }
  /*Fill report */
  if (USBH_HID_FifoRead(&HID_Handle->fifo, &pad_report_data, HID_Handle->length) ==  HID_Handle->length)
  {
    for (x = 0U; x < sizeof(pad_info.keys); x++)
    {
      pad_info.keys[x] = (uint8_t)HID_ReadItem((HID_Report_ItemTypedef *) &gp_buttons_key_array, x);
    }
    return USBH_OK;
  }
  return   USBH_FAIL;
}

/**
  * @brief  USBH_HID_GetASCIICode
  *         The function decode keyboard data into ASCII characters.
  * @param  phost: Host handle
  * @param  info: Keyboard information
  * @retval ASCII code
  */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

