/**
  ******************************************************************************
  * @file    waveshare_reg.c
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 Michael Ihde.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "waveshare_reg.h"

/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup Components
  * @{
  */ 

/** @addtogroup WAVESHARE
  * @{
  */
    
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup WAVESHARE_Private_Constants WAVESHARE Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/************** Generic Function  *******************/
/*******************************************************************************
* Function Name : waveshare_read_reg
* Description   : Generic Reading function. It must be fullfilled with either
*                 I2C or SPI reading functions
* Input         : Register Address, length of buffer
* Output        : Data Read
*******************************************************************************/
int32_t waveshare_read_reg(waveshare_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length)
{
  return ctx->ReadReg(ctx->handle, reg, pdata, length);
}

/*******************************************************************************
* Function Name : waveshare_write_reg
* Description   : Generic Writing function. It must be fullfilled with either
*                 I2C or SPI writing function
* Input         : Register Address, Data to be written, length of buffer
* Output        : None
*******************************************************************************/
int32_t waveshare_write_reg(waveshare_ctx_t *ctx, uint16_t reg, const uint8_t *pdata, uint16_t length)
{
  return ctx->WriteReg(ctx->handle, reg, (uint8_t *)pdata, length);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
