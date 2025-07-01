/**
  ******************************************************************************
  * @file    raspberrypi.c
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
  * The 7" touchscreen consists of a DPI LCD panel, a Toshiba
  * TC358762XBG DSI-DPI bridge, and an I2C-connected Atmel ATTINY88-MUR
  * controlling power management, the LCD PWM, and initial register
  * setup of the Tohsiba.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "raspberrypi.h"
#include <stddef.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup RASPBERRYPI RASPBERRYPI
  * @brief     This file provides a set of functions needed to drive the
  *            RASPBERRYPI IC display driver.
  * @{
  */

/** @defgroup RASPBERRYPI_Exported_Types RASPBERRYPI Exported Types
  * @{
  */
RASPBERRYPI_LCD_Drv_t   RASPBERRYPI_LCD_Driver = 
{
  RASPBERRYPI_Init,
  RASPBERRYPI_DeInit,  
  RASPBERRYPI_ReadID,
  RASPBERRYPI_DisplayOn,
  RASPBERRYPI_DisplayOff,
  RASPBERRYPI_SetBrightness,
  RASPBERRYPI_GetBrightness,  
  RASPBERRYPI_SetOrientation,
  RASPBERRYPI_GetOrientation,
  RASPBERRYPI_SetCursor,
  RASPBERRYPI_DrawBitmap,
  RASPBERRYPI_FillRGBRect,
  RASPBERRYPI_DrawHLine,
  RASPBERRYPI_DrawVLine,
  RASPBERRYPI_FillRect,
  RASPBERRYPI_GetPixel,
  RASPBERRYPI_SetPixel,
  RASPBERRYPI_GetXSize,
  RASPBERRYPI_GetYSize,
};
  
static RASPBERRYPI_LCD_Ctx_t RASPBERRYPICtx;
/**
  * @}
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int32_t RASPBERRYPI_ReadI2CRegWrap(void *Handle, uint16_t Reg, uint8_t* Data, uint16_t Length);
static int32_t RASPBERRYPI_WriteI2CRegWrap(void *Handle, uint16_t Reg, uint8_t *pData, uint16_t Length);
static int32_t RASPBERRYPI_ReadDSIRegWrap(void *Handle, uint16_t Reg, uint8_t* Data, uint16_t Length);
static int32_t RASPBERRYPI_WriteDSIRegWrap(void *Handle, uint16_t Reg, uint8_t *pData, uint16_t Length);
static int32_t RASPBERRYPI_IO_Delay(RASPBERRYPI_Object_t *pObj, uint32_t Delay);
static void RASPBERRYPI_SetI2C_Address(RASPBERRYPI_Object_t *pObj, uint8_t Address);
/**
  * @}
  */

/** @defgroup RASPBERRYPI_Exported_Variables
  * @{
  */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup RASPBERRYPI_Exported_Functions RASPBERRYPI Exported Functions
  * @{
  */

/**
  * @brief  Register component IO bus
  * @param  Component object pointer
  * @retval Component status
  */
int32_t RASPBERRYPI_RegisterBusIO (RASPBERRYPI_Object_t *pObj, RASPBERRYPI_IO_t *pIO)
{
  int32_t ret = RASPBERRYPI_OK;

  if(pObj == NULL)
  {
    ret = RASPBERRYPI_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteI2CReg  = pIO->WriteI2CReg;
    pObj->IO.ReadI2CReg   = pIO->ReadI2CReg;
    pObj->IO.WriteDSIReg  = pIO->WriteDSIReg;
    pObj->IO.ReadDSIReg   = pIO->ReadDSIReg;
    pObj->IO.GetTick   = pIO->GetTick;
    
    pObj->Ctx.ReadI2CReg  = RASPBERRYPI_ReadI2CRegWrap;
    pObj->Ctx.WriteI2CReg = RASPBERRYPI_WriteI2CRegWrap;
    pObj->Ctx.ReadDSIReg  = RASPBERRYPI_ReadDSIRegWrap;
    pObj->Ctx.WriteDSIReg = RASPBERRYPI_WriteDSIRegWrap;
    pObj->Ctx.handle   = pObj;

    if(pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = RASPBERRYPI_ERROR;
    }
  }
  
  return ret;
}

/**
  * @brief  Initializes the LCD
  * @param  pObj Component object
  * @param  ColorCoding   Color Code
  * @param  Orientation   Display orientation
  * @retval Component status
  */
int32_t RASPBERRYPI_PreInit(RASPBERRYPI_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation)
{
  int32_t ret = RASPBERRYPI_OK;
  uint8_t val;
  uint32_t tmp;
  uint32_t lcdctrl;

  if(pObj->IsInitialized == 0U)
  {
    pObj->IO.Init();

    RASPBERRYPI_SetI2C_Address(pObj, RASPBERRYPI_I2C_ADDR);

    // Turn off so we can cleanly sequence powering on
    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_POWERON, &val, 1);
    HAL_Delay (25);

    // Backlight Off
    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PWM, &val, 1);
    HAL_Delay (10);

    // LCD Power Down
    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTA, &val, 1);
    HAL_Delay (10);

    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTB, &val, 1);
    HAL_Delay (10);

    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1);
    HAL_Delay (30);

    // LCD Power Up
    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1); // Ensure bridge, and tp stay in reset
    HAL_Delay (10);

    val = RASPBERRYPI_PA_LCD_LR;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTA, &val, 1); // Set orientation
    HAL_Delay (10);

    val = RASPBERRYPI_PB_LCD_MAIN;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTB, &val, 1); // Main regulator on and power to the panel
    HAL_Delay (10);

    val = RASPBERRYPI_PC_LED_EN;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1); // Bring controllers out of reset
    HAL_Delay (80);

    // GPIO RST_BRIDGE_N = 0
    ret += raspberrypi_read_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1);
    val &= ~(RASPBERRYPI_PC_RST_BRIDGE_N | RASPBERRYPI_PC_RST_LCD_N);
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1);

    HAL_Delay(100);

    // GPIO RST_BRIDGE_N = 1
    ret += raspberrypi_read_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1);
    val |= (RASPBERRYPI_PC_RST_BRIDGE_N | RASPBERRYPI_PC_RST_LCD_N);
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1);
    HAL_Delay(8);

    // Setup Display
    val = 0x04;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_ADDRH, &val, 1); // 0x047C register set to 0x0000 is DSI input + DPI output
    HAL_Delay (8);

    val = 0x7C;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_ADDRL, &val, 1); // 0x047C register set to 0x0000 is DSI input + DPI output
    HAL_Delay (8);

    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_WRITEH, &val, 1);
    HAL_Delay (8);

    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_WRITEL, &val, 1);
    HAL_Delay (8);

    /*
     * Power up the Toshiba bridge. The Atmel device can misbehave
     * over I2C for a few ms after writes to REG_POWERON (including the
     * write in rpi_touchscreen_disable()), so sleep before and after.
     * Also to ensure that the bridge has been off for at least 100ms.
     */
    val = 0x01;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_POWERON, &val, 1);
    HAL_Delay (25);

    /* Wait for nPWRDWN to go low to indicate poweron is done. */
    for (int ii = 0; ii < 100; ii++) {
      val = 0x00;
      ret += raspberrypi_read_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTB, &val, 1);
      if (val >= 0 && (val & 1)) {
        break;
      }
      HAL_Delay (1);
    }

    if(ret != RASPBERRYPI_OK)
    {
      ret = RASPBERRYPI_ERROR;
    } else {
      pObj->IsInitialized = 1;
    }
  }

  return ret;
}

/**
  * @brief  Initializes the LCD
  * @param  pObj Component object
  * @param  ColorCoding   Color Code
  * @param  Orientation   Display orientation
  * @retval Component status
  */
int32_t RASPBERRYPI_Init(RASPBERRYPI_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation)
{
  /*
   * 		.clock = 25979400 / 1000,
		.hdisplay = 800,
		.hsync_start = 800 + 1, // HFP=1
		.hsync_end = 800 + 1 + 2, //HSYNC=2
		.htotal = 800 + 1 + 2 + 46, // HBP=46
		.vdisplay = 480,
		.vsync_start = 480 + 7, // VFP=7
		.vsync_end = 480 + 7 + 2, // VSYNC=2
		.vtotal = 480 + 7 + 2 + 21, // VBP=21
   */
  int32_t ret = RASPBERRYPI_OK;
  uint8_t val;
  uint32_t tmp;
  uint32_t lcdctrl;
  uint16_t hdisplay = RASPBERRYPI_800X480_WIDTH;
  uint16_t hsync_start = RASPBERRYPI_800X480_WIDTH + RASPBERRYPI_800X480_HFP;     // HFP=131
  uint16_t hsync_end = RASPBERRYPI_800X480_WIDTH + RASPBERRYPI_800X480_HFP + RASPBERRYPI_800X480_HSYNC;   // HSYNC=2
  uint16_t htotal = RASPBERRYPI_800X480_WIDTH + RASPBERRYPI_800X480_HFP + RASPBERRYPI_800X480_HSYNC + RASPBERRYPI_800X480_HBP; // HBP=45
  uint16_t vdisplay = RASPBERRYPI_800X480_HEIGHT;              // VACT
  uint16_t vsync_start = RASPBERRYPI_800X480_HEIGHT + RASPBERRYPI_800X480_VFP;       // VFP=7
  uint16_t vsync_end = RASPBERRYPI_800X480_HEIGHT + RASPBERRYPI_800X480_VFP + RASPBERRYPI_800X480_VSYNC;     // SW=2
  uint16_t vtotal = RASPBERRYPI_800X480_HEIGHT + RASPBERRYPI_800X480_VFP + RASPBERRYPI_800X480_VSYNC + RASPBERRYPI_800X480_VBP;   // VBP=22
  uint16_t flags = RASPBERRYPI_DRM_MODE_FLAG_NVSYNC | RASPBERRYPI_DRM_MODE_FLAG_NHSYNC;;

#if I2C_IN_INIT
  if(pObj->IsInitialized == 0U)
  {
    pObj->IO.Init();

    RASPBERRYPI_SetI2C_Address(pObj, RASPBERRYPI_I2C_ADDR);

    // Turn off so we can cleanly sequence powering on
    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_POWERON, &val, 1);
    HAL_Delay (25);

    // Backlight Off
    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PWM, &val, 1);
    HAL_Delay (10);

    // LCD Power Down
    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTA, &val, 1);
    HAL_Delay (10);

    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTB, &val, 1);
    HAL_Delay (10);

    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1);
    HAL_Delay (30);

    // LCD Power Up
    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1); // Ensure bridge, and tp stay in reset
    HAL_Delay (10);

    val = RASPBERRYPI_PA_LCD_LR;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTA, &val, 1); // Set orientation
    HAL_Delay (10);

    val = RASPBERRYPI_PB_LCD_MAIN;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTB, &val, 1); // Main regulator on and power to the panel
    HAL_Delay (10);

    val = RASPBERRYPI_PC_LED_EN;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1); // Bring controllers out of reset
    HAL_Delay (80);

    // GPIO RST_BRIDGE_N = 0
    ret += raspberrypi_read_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1);
    val &= ~(RASPBERRYPI_PC_RST_BRIDGE_N | RASPBERRYPI_PC_RST_LCD_N);
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1);

    HAL_Delay(100);

    // GPIO RST_BRIDGE_N = 1
    ret += raspberrypi_read_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1);
    val |= (RASPBERRYPI_PC_RST_BRIDGE_N | RASPBERRYPI_PC_RST_LCD_N);
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTC, &val, 1);
    HAL_Delay(8);

    // Setup Display
    val = 0x04;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_ADDRH, &val, 1); // 0x047C register set to 0x0000 is DSI input + DPI output
    HAL_Delay (8);

    val = 0x7C;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_ADDRL, &val, 1); // 0x047C register set to 0x0000 is DSI input + DPI output
    HAL_Delay (8);

    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_WRITEH, &val, 1);
    HAL_Delay (8);

    val = 0x00;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_WRITEL, &val, 1);
    HAL_Delay (8);

    /*
     * Power up the Toshiba bridge. The Atmel device can misbehave
     * over I2C for a few ms after writes to REG_POWERON (including the
     * write in rpi_touchscreen_disable()), so sleep before and after.
     * Also to ensure that the bridge has been off for at least 100ms.
     */
    val = 0x01;
    ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_POWERON, &val, 1);
    HAL_Delay (25);

    /* Wait for nPWRDWN to go low to indicate poweron is done. */
    for (int ii = 0; ii < 100; ii++) {
      val = 0x00;
      ret += raspberrypi_read_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTB, &val, 1);
      if (val >= 0 && (val & 1)) {
        break;
      }
      HAL_Delay (1);
    }
#else
  {
#endif
    // Start DSI Configuration
    // index name refresh (Hz) hdisp hss hse htot vdisp vss vse vtot
    // #0 800x480 60.03 800 931 933 978 480 487 489 511 30000 flags: nhsync, nvsync; type: preferred, driver
    if(ret == RASPBERRYPI_OK)
    {
      // rpi_touchscreen_prepare
      ret = raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_DSI_LANEENABLE, RASPBERRYPI_LANEENABLE_L0EN | RASPBERRYPI_LANEENABLE_CLEN);

      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_PPI_D0S_CLRSIPOCOUNT, 5);
      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_PPI_D1S_CLRSIPOCOUNT, 5);

      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_PPI_D0S_ATMR, 0);
      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_PPI_D1S_ATMR, 0);

      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_PPI_D1S_ATMR, RASPBERRYPI_LPX_PERIOD);

      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_SPICMR, 0);

      lcdctrl = RASPBERRYPI_LCDCTRL_VSDELAY(1) | RASPBERRYPI_LCDCTRL_RGB888 | RASPBERRYPI_LCDCTRL_UNK6 | RASPBERRYPI_LCDCTRL_VTGEN;

      if (flags & RASPBERRYPI_DRM_MODE_FLAG_NHSYNC)
	lcdctrl |= RASPBERRYPI_LCDCTRL_HSPOL;

      if (flags & RASPBERRYPI_DRM_MODE_FLAG_NVSYNC)
	lcdctrl |= RASPBERRYPI_LCDCTRL_VSPOL;

      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_LCDCTRL, lcdctrl); // 0x00100150

      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_SYSCTRL, 0x040f);

      // Manually set timings - this code is found in tc358762.c but when included with
      // it causes the raspberry pi display to not work
      /*
      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_LCD_HS_HBP,
		  (hsync_end - hsync_start) | ((htotal - hsync_end) << 16));
      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_LCD_HDISP_HFP,hdisplay | ((hsync_start - hdisplay) << 16));
      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_LCD_VS_VBP,
		  (vsync_end - vsync_start) | ((vtotal - vsync_end) << 16));
      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_LCD_VDISP_VFP, vdisplay | ((vsync_start - vdisplay) << 16));
      */
      HAL_Delay (100);

      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_PPI_STARTPPI, RASPBERRYPI_PPI_START_FUNCTION);
      ret += raspberrypi_write_dsi_reg(&pObj->Ctx, RASPBERRYPI_DSI_STARTDSI, RASPBERRYPI_DSI_RX_START);

      HAL_Delay (100);
    }

    // rpi_touchscreen_enable
    if(ret == RASPBERRYPI_OK)
    {
      // Turn on the backlight
      val = 0xFF;
      ret = raspberrypi_write_i2c_reg(&pObj->Ctx, RASPBERRYPI_REG_PWM, &val, 1);

      /* Default to the same orientation as the closed source
       * firmware used for the panel.  Runtime rotation
       * configuration will be supported using VC4's plane
       * orientation bits.
       */
      val = RASPBERRYPI_PA_LCD_LR;
      ret += raspberrypi_write_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_PORTA, &val, 1); // Set orientation
      HAL_Delay (10);
    }

    if(ret != RASPBERRYPI_OK)
    {
      ret = RASPBERRYPI_ERROR;
    } else {
      pObj->IsInitialized = 1;
    }
  }

  return ret;
}

/**
  * @brief  De-Initializes the component
  * @param  pObj Component object
  * @retval Component status
  */
int32_t RASPBERRYPI_DeInit(RASPBERRYPI_Object_t *pObj)
{
  if(pObj->IsInitialized == 1U)
  {
    pObj->IO.DeInit();

    /* Set IsInitialized to 0 */
    pObj->IsInitialized = 0;
  }
  return RASPBERRYPI_OK;
}

/**
  * @brief  Read the component ID.
  * @param  pObj Component object
  * @param  Id   Component ID
  * @retval Component status
  */
int32_t RASPBERRYPI_ReadID(RASPBERRYPI_Object_t *pObj, uint32_t *Id)
{ 
  int32_t ret;
  uint8_t tmp;

  // Read version
  ret += raspberrypi_read_i2c_reg (&pObj->Ctx, RASPBERRYPI_REG_ID, &tmp, 1); // Ensure bridge, and tp stay in reset
  if (ret == RASPBERRYPI_OK)
  {
    *Id = tmp;
  }
  else
  {
    ret = RASPBERRYPI_ERROR;
  }



  return ret;  
}  

/**
  * @brief  Set the display brightness.
  * @param  pObj Component object
  * @param  Brightness   display brightness to be set
  * @retval Component status
  */
int32_t RASPBERRYPI_SetBrightness(RASPBERRYPI_Object_t *pObj, uint32_t Brightness)
{
  int32_t ret;
  uint8_t brightness = (uint8_t)((Brightness * 255U)/100U);
  
  /* Send Display on DCS command to display */
  ret = raspberrypi_write_i2c_reg(&pObj->Ctx, RASPBERRYPI_REG_PWM, &brightness, 1);
  if(ret != RASPBERRYPI_OK)
  {
    ret = RASPBERRYPI_ERROR;
  }
  else
  {
    RASPBERRYPICtx.Brightness = Brightness;
    ret = RASPBERRYPI_OK;
  }
  
  return ret;  
}

/**
  * @brief  Get the display brightness.
  * @param  pObj Component object
  * @param  Brightness   display brightness to be returned
  * @retval Component status
  */
int32_t RASPBERRYPI_GetBrightness(RASPBERRYPI_Object_t *pObj, uint32_t *Brightness)
{
  int32_t ret;
  uint8_t tmp;
  ret = raspberrypi_read_i2c_reg(&pObj->Ctx, RASPBERRYPI_REG_PWM, &tmp, 1);

  if(ret != RASPBERRYPI_OK)
  {
    ret = RASPBERRYPI_ERROR;
    *Brightness = RASPBERRYPICtx.Brightness; // Use the stored brightness
  }
  else
  {
    RASPBERRYPICtx.Brightness = ((uint32_t)tmp * 100U)/255U;
    *Brightness = RASPBERRYPICtx.Brightness;
  }

  return RASPBERRYPI_OK;  
}

/**
  * @brief  Set the display On.
  * @param  pObj Component object
  * @retval Component status
  */
int32_t RASPBERRYPI_DisplayOn(RASPBERRYPI_Object_t *pObj)
{
  int32_t ret;
  uint8_t display = 0x00;
  
  // TODO
  ret = RASPBERRYPI_OK;
  
  return ret;
}

/**
  * @brief  Set the display Off.
  * @param  pObj Component object
  * @retval Component status
  */
int32_t RASPBERRYPI_DisplayOff(RASPBERRYPI_Object_t *pObj)
{
  int32_t ret;
  uint8_t display = 0x00;
  
  // TODO
  ret = RASPBERRYPI_OK;
  
  return ret;
}

/**
  * @brief  Set the display Orientation.
  * @param  pObj Component object
  * @param  Orientation   display Orientation to be set
  * @retval Component status
  */
int32_t RASPBERRYPI_SetOrientation(RASPBERRYPI_Object_t *pObj, uint32_t Orientation)
{
  int32_t ret;
  if((Orientation != RASPBERRYPI_ORIENTATION_LANDSCAPE) && (Orientation != RASPBERRYPI_ORIENTATION_PORTRAIT))
  {
    ret = RASPBERRYPI_ERROR;
  }/* Send command to configure display orientation mode  */
  else if(Orientation == RASPBERRYPI_ORIENTATION_LANDSCAPE)
  {
    ret = RASPBERRYPI_ERROR; // TODO Not yet supported
    //RASPBERRYPICtx.Orientation = RASPBERRYPI_ORIENTATION_LANDSCAPE;
  }
  else
  {
    RASPBERRYPICtx.Orientation = RASPBERRYPI_ORIENTATION_PORTRAIT;
  }
  
  if(ret != RASPBERRYPI_OK)
  {
    ret = RASPBERRYPI_ERROR;
  }
  
  return ret;
}

/**
  * @brief  Set the display Orientation.
  * @param  pObj Component object
  * @param  Orientation   display Orientation to be returned
  * @retval Component status
  */
int32_t RASPBERRYPI_GetOrientation(RASPBERRYPI_Object_t *pObj, uint32_t *Orientation)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  *Orientation = RASPBERRYPICtx.Orientation;
  
  return RASPBERRYPI_OK;
}

/**
  * @brief  Set the display Width.
  * @param  pObj Component object
  * @param  Xsize   display Width to be set
  * @retval Component status
  */
int32_t RASPBERRYPI_GetXSize(RASPBERRYPI_Object_t *pObj, uint32_t *Xsize)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  switch(RASPBERRYPICtx.Orientation)
  {
  case RASPBERRYPI_ORIENTATION_PORTRAIT:
    *Xsize = RASPBERRYPI_480X800_WIDTH;
    break;
  case RASPBERRYPI_ORIENTATION_LANDSCAPE:
    *Xsize = RASPBERRYPI_800X480_WIDTH;
    break;
  default:
    *Xsize = RASPBERRYPI_800X480_WIDTH;
    break;
  }
  
  return RASPBERRYPI_OK;
}

/**
  * @brief  Set the display Height.
  * @param  pObj Component object
  * @param  Ysize   display Height to be set
  * @retval Component status
  */
int32_t RASPBERRYPI_GetYSize(RASPBERRYPI_Object_t *pObj, uint32_t *Ysize)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  switch(RASPBERRYPICtx.Orientation)
  {
  case RASPBERRYPI_ORIENTATION_PORTRAIT:
    *Ysize = RASPBERRYPI_480X800_HEIGHT;
    break;
  case RASPBERRYPI_ORIENTATION_LANDSCAPE:
    *Ysize = RASPBERRYPI_800X480_HEIGHT;
    break;
  default:
    *Ysize = RASPBERRYPI_800X480_HEIGHT;
    break;
  }
  
  return RASPBERRYPI_OK;
}

/**
  * @brief  Set the display cursor.
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @retval Component status
  */
int32_t RASPBERRYPI_SetCursor(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return RASPBERRYPI_ERROR;
}

/**
  * @brief  Draw Bitmap image
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  pBmp pointer to bmp data
  * @retval Component status
  */
int32_t RASPBERRYPI_DrawBitmap(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return RASPBERRYPI_ERROR;
}

int32_t RASPBERRYPI_FillRGBRect(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return RASPBERRYPI_ERROR;
}

/**
  * @brief  Draw Horizontal Line
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Length Line length
  * @param  Color Draw color
  * @retval Component status
  */
int32_t RASPBERRYPI_DrawHLine(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return RASPBERRYPI_ERROR;
}

/**
  * @brief  Draw Vertical line
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Length Line length
  * @param  Color Draw color
  * @retval Component status
  */
int32_t RASPBERRYPI_DrawVLine(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return RASPBERRYPI_ERROR;
}

/**
  * @brief  Fill rectangle
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Width Rectangle width
  * @param  Height Rectangle height
  * @param  Color Draw color
  * @retval Component status
  */
int32_t RASPBERRYPI_FillRect(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return RASPBERRYPI_ERROR;
}

/**
  * @brief  Get pixel color
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Color Color to be returned
  * @retval Component status
  */
int32_t RASPBERRYPI_GetPixel(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t *Color)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return RASPBERRYPI_ERROR;
}

/**
  * @brief  Set pixel color
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Color Color to be returned
  * @retval Component status
  */
int32_t RASPBERRYPI_SetPixel(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Color)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return RASPBERRYPI_ERROR;
}

/**
  * @}
  */

/** @addtogroup RASPBERRYPI_Private_FunctionsPrototyes
  * @{
  */

static void RASPBERRYPI_SetI2C_Address(RASPBERRYPI_Object_t *pObj, uint8_t Address)
{
  pObj->IO.Address = Address;
}

/**
  * @brief  Wrap component ReadReg to Bus Read function
  * @param  Handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t RASPBERRYPI_ReadI2CRegWrap(void *Handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  RASPBERRYPI_Object_t *pObj = (RASPBERRYPI_Object_t *)Handle;

  return pObj->IO.ReadI2CReg(pObj->IO.Address << 1, Reg, pData, Length);
}

/**
  * @brief  Wrap component WriteReg to Bus Write function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t RASPBERRYPI_WriteI2CRegWrap(void *Handle, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  RASPBERRYPI_Object_t *pObj = (RASPBERRYPI_Object_t *)Handle;

  return pObj->IO.WriteI2CReg(pObj->IO.Address << 1, Reg, pData, Length);
}

/**
  * @brief  Wrap component ReadReg to Bus Read function
  * @param  Handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t RASPBERRYPI_ReadDSIRegWrap(void *Handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  RASPBERRYPI_Object_t *pObj = (RASPBERRYPI_Object_t *)Handle;

  return pObj->IO.ReadDSIReg(0, Reg, pData, Length);
}

/**
  * @brief  Wrap component WriteReg to Bus Write function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t RASPBERRYPI_WriteDSIRegWrap(void *Handle, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  RASPBERRYPI_Object_t *pObj = (RASPBERRYPI_Object_t *)Handle;

  return pObj->IO.WriteDSIReg(0, Reg, pData, Length);
}

/**
  * @brief  RASPBERRYPI delay
  * @param  Delay  Delay in ms
  */
static int32_t RASPBERRYPI_IO_Delay(RASPBERRYPI_Object_t *pObj, uint32_t Delay)
{
  uint32_t tickstart;
  tickstart = pObj->IO.GetTick();
  while((pObj->IO.GetTick() - tickstart) < Delay)
  {
  }
  return RASPBERRYPI_OK;
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

/**
  * @}
  */
