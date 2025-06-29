
/**
  ******************************************************************************
  * @file    waveshare_reg.h
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Michael Ihde
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef WAVESHARE_REG_H
#define WAVESHARE_REG_H

/* Includes ------------------------------------------------------------------*/
#include <cmsis_compiler.h>

#ifdef __cplusplus
 extern "C" {
#endif 

/** @addtogroup BSP
  * @{
  */
 
/** @addtogroup Components
  * @{
  */

/** @addtogroup WAVESHARE
  * @{
  */


/** @defgroup WAVESHARE_Exported_Constants
  * @{
  */
    
/* Derived from Linux Device Driver */

#define  WAVESHARE_CMD_APPLY_BRIGHTNESS     0xAA
#define  WAVESHARE_CMD_BRIGHTNESS           0xAB
#define  WAVESHARE_CMD_INIT_3               0xAC
#define  WAVESHARE_CMD_ENABLE               0xAD
#define  WAVESHARE_CMD_INIT_1               0xC0
#define  WAVESHARE_CMD_INIT_2               0xC2

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
   
/** @defgroup WAVESHARE_Exported_Macros WAVESHARE Exported Macros
  * @{
  */ 

/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/
  
/** @addtogroup WAVESHARE_Exported_Functions
  * @{
  */
/************** Generic Function  *******************/

typedef int32_t (*WAVESHARE_Write_Func)(void *, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*WAVESHARE_Read_Func) (void *, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  WAVESHARE_Write_Func   WriteReg;
  WAVESHARE_Read_Func    ReadReg;
  void                  *handle;
} waveshare_ctx_t;

/*******************************************************************************
* Register      : Generic - All
* Address       : Generic - All
* Bit Group Name: None
* Permission    : W
*******************************************************************************/
int32_t waveshare_read_reg(waveshare_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
int32_t waveshare_write_reg(waveshare_ctx_t *ctx, uint16_t reg, const uint8_t *pdata, uint16_t length);
/**
  * @}
  */ 
#ifdef __cplusplus
}
#endif

#endif /* WAVESHARE_REG_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
