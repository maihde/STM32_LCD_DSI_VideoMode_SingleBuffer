/**
  ******************************************************************************
  * @file    stm32H747i_discovery_conf.h
  * @author  MCD Application Team
  * @brief   STM32H747I_Discovery board configuration file.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef STM32H747I_DISCO_CONF_H
#define STM32H747I_DISCO_CONF_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* COM define */
#define USE_COM_LOG                         1U
#define USE_BSP_COM_FEATURE                 1U
/* LCD controllers defines */
#define USE_LCD_CTRL_OTM8009A               0U
#define USE_LCD_CTRL_NT35510                0U
#define USE_LCD_CTRL_WAVESHARE_2P8          0U
#define USE_LCD_CTRL_RASPBERRYPI            1U
#define USE_LCD_CTRL_ADV7533                0U

#define USE_LCD_TEST_VERTICAL               0U
#define USE_LCD_TEST_HORIZONTAL             0U

#define LCD_LAYER_0_ADDRESS                 0xD0000000U
#define LCD_LAYER_1_ADDRESS                 0xD0200000U
/* Camera sensors defines */
#define USE_CAMERA_SENSOR_OV5640            1U
#define USE_CAMERA_SENSOR_OV9655            1U
/* Audio codecs defines */
#define USE_AUDIO_CODEC_WM8994              1U


/* Default Audio IN internal buffer size */
#define DEFAULT_AUDIO_IN_BUFFER_SIZE        64U
/* TS supported features defines */
#define USE_TS_GESTURE                      1U
#define USE_TS_MULTI_TOUCH                  1U

/* Default TS touch number */
#define TS_TOUCH_NBR                        2U
#define CAMERA_FRAME_BUFFER       ((uint32_t)0xD0600000)

/* IRQ priorities */
#define BSP_SDRAM_IT_PRIORITY               15U
#define BSP_CAMERA_IT_PRIORITY              15U
#define BSP_BUTTON_WAKEUP_IT_PRIORITY         15U
#define BSP_AUDIO_OUT_IT_PRIORITY           14U
#define BSP_AUDIO_IN_IT_PRIORITY            15U
#define BSP_SD_IT_PRIORITY                  14U
#define BSP_SD_RX_IT_PRIORITY               14U
#define BSP_SD_TX_IT_PRIORITY               15U
#define BSP_TS_IT_PRIORITY                  15U
#define BSP_JOY1_SEL_IT_PRIORITY            15U
#define BSP_JOY1_DOWN_IT_PRIORITY           15U
#define BSP_JOY1_LEFT_IT_PRIORITY           15U
#define BSP_JOY1_RIGHT_IT_PRIORITY          15U
#define BSP_JOY1_UP_IT_PRIORITY             15U

#ifdef __cplusplus
}
#endif

#endif /* STM32H747I_DISCO_CONF_H */

