/**
  ******************************************************************************
  * @file    raspberrypi_reg.c
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
#include "raspberrypi_reg.h"

/** @addtogroup BSP
  * @{
  */
  
/** @addtogroup Components
  * @{
  */ 

/** @addtogroup RASPBERRYPI
  * @{
  */
    
/* Private types -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private constants ---------------------------------------------------------*/
/** @defgroup RASPBERRYPI_Private_Constants RASPBERRYPI Private Constants
  * @{
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/************** Generic Function  *******************/
/*******************************************************************************
* Function Name : raspberrypi_read_reg
* Description   : Generic Reading function. It must be fullfilled with either
*                 I2C or SPI reading functions
* Input         : Register Address, length of buffer
* Output        : Data Read
*******************************************************************************/
int32_t raspberrypi_read_i2c_reg(raspberrypi_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length)
{
  return ctx->ReadI2CReg(ctx->handle, reg, pdata, length);
}

/*******************************************************************************
* Function Name : raspberrypi_write_reg
* Description   : Generic Writing function. It must be fullfilled with either
*                 I2C or SPI writing function
* Input         : Register Address, Data to be written, length of buffer
* Output        : None
*******************************************************************************/
int32_t raspberrypi_write_i2c_reg(raspberrypi_ctx_t *ctx, uint16_t reg, const uint8_t *pdata, uint16_t length)
{
  return ctx->WriteI2CReg(ctx->handle, reg, (uint8_t *)pdata, length);
}

/*******************************************************************************
* Function Name : raspberrypi_read_reg
* Description   : Generic Reading function. It must be fullfilled with either
*                 I2C or SPI reading functions
* Input         : Register Address, length of buffer
* Output        : Data Read
*******************************************************************************/
int32_t raspberrypi_read_dsi_reg(raspberrypi_ctx_t *ctx, uint16_t reg, uint32_t data)
{
  // TODO
  return ctx->ReadDSIReg(ctx->handle, reg, &data, 4);
}

/*******************************************************************************
* Function Name : raspberrypi_write_reg
* Description   : Generic Writing function. It must be fullfilled with either
*                 I2C or SPI writing function
* Input         : Register Address, Data to be written, length of buffer
* Output        : None
*******************************************************************************/
int32_t raspberrypi_write_dsi_reg(raspberrypi_ctx_t *ctx, uint16_t reg, const uint32_t data)
{
  uint8_t msg[] =
    { reg, reg >> 8, data, data >> 8, data >> 16, data >> 24, };
  return ctx->WriteDSIReg(ctx->handle, msg[0], (uint8_t *)(&msg[1]), 5);
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
