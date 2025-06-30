/**
  ******************************************************************************
  * @file    waveshare.c
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

/* Includes ------------------------------------------------------------------*/
#include "waveshare.h"
#include <stddef.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @defgroup WAVESHARE WAVESHARE
  * @brief     This file provides a set of functions needed to drive the
  *            WAVESHARE IC display driver.
  * @{
  */

/** @defgroup WAVESHARE_Exported_Types WAVESHARE Exported Types
  * @{
  */
WAVESHARE_LCD_Drv_t   WAVESHARE_LCD_Driver = 
{
  WAVESHARE_Init,
  WAVESHARE_DeInit,  
  WAVESHARE_ReadID,
  WAVESHARE_DisplayOn,
  WAVESHARE_DisplayOff,
  WAVESHARE_SetBrightness,
  WAVESHARE_GetBrightness,  
  WAVESHARE_SetOrientation,
  WAVESHARE_GetOrientation,
  WAVESHARE_SetCursor,
  WAVESHARE_DrawBitmap,
  WAVESHARE_FillRGBRect,
  WAVESHARE_DrawHLine,
  WAVESHARE_DrawVLine,
  WAVESHARE_FillRect,
  WAVESHARE_GetPixel,
  WAVESHARE_SetPixel,
  WAVESHARE_GetXSize,
  WAVESHARE_GetYSize,
};
  
static WAVESHARE_LCD_Ctx_t WAVESHARECtx;
/**
  * @}
  */

/**
  * @}
  */

/* Private macros ------------------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
static int32_t WAVESHARE_ReadRegWrap(void *Handle, uint16_t Reg, uint8_t* Data, uint16_t Length);
static int32_t WAVESHARE_WriteRegWrap(void *Handle, uint16_t Reg, uint8_t *pData, uint16_t Length);
static int32_t WAVESHARE_IO_Delay(WAVESHARE_Object_t *pObj, uint32_t Delay);
static void WAVESHARE_SetI2C_Address(WAVESHARE_Object_t *pObj, uint8_t Address);
/**
  * @}
  */

/** @defgroup WAVESHARE_Exported_Variables
  * @{
  */

/**
  * @}
  */

/* Exported functions ---------------------------------------------------------*/
/** @defgroup WAVESHARE_Exported_Functions WAVESHARE Exported Functions
  * @{
  */

/**
  * @brief  Register component IO bus
  * @param  Component object pointer
  * @retval Component status
  */
int32_t WAVESHARE_RegisterBusIO (WAVESHARE_Object_t *pObj, WAVESHARE_IO_t *pIO)
{
  int32_t ret = WAVESHARE_OK;

  if(pObj == NULL)
  {
    ret = WAVESHARE_ERROR;
  }
  else
  {
    pObj->IO.Init      = pIO->Init;
    pObj->IO.DeInit    = pIO->DeInit;
    pObj->IO.Address   = pIO->Address;
    pObj->IO.WriteReg  = pIO->WriteReg;
    pObj->IO.ReadReg   = pIO->ReadReg;
    pObj->IO.GetTick   = pIO->GetTick;
    
    pObj->Ctx.ReadReg  = WAVESHARE_ReadRegWrap;
    pObj->Ctx.WriteReg = WAVESHARE_WriteRegWrap;
    pObj->Ctx.handle   = pObj;

    if(pObj->IO.Init != NULL)
    {
      ret = pObj->IO.Init();
    }
    else
    {
      ret = WAVESHARE_ERROR;
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
int32_t WAVESHARE_Init(WAVESHARE_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation)
{
  int32_t ret = WAVESHARE_OK;
  uint8_t tmp;

  if(pObj->IsInitialized == 0U)
  {
    pObj->IO.Init();

    WAVESHARE_SetI2C_Address(pObj, WAVESHARE_I2C_ADDR);

    tmp = 0x01;
    ret = waveshare_write_reg(&pObj->Ctx, WAVESHARE_CMD_INIT_1, &tmp, 1);
    ret += waveshare_write_reg(&pObj->Ctx, WAVESHARE_CMD_INIT_2, &tmp, 1);
    ret += waveshare_write_reg(&pObj->Ctx, WAVESHARE_CMD_INIT_3, &tmp, 1);
    // Default to full brightness
    tmp = 0x00;
    ret += waveshare_write_reg(&pObj->Ctx, WAVESHARE_CMD_BRIGHTNESS, &tmp, 1);
    tmp = 0x01;
    ret += waveshare_write_reg(&pObj->Ctx, WAVESHARE_CMD_APPLY_BRIGHTNESS, &tmp, 1);
    // Enable display
    tmp = 0x01;
    ret += waveshare_write_reg(&pObj->Ctx, WAVESHARE_CMD_ENABLE, &tmp, 1);

    if(ret != WAVESHARE_OK)
    {
      ret = WAVESHARE_ERROR;
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
int32_t WAVESHARE_DeInit(WAVESHARE_Object_t *pObj)
{
  if(pObj->IsInitialized == 1U)
  {
    pObj->IO.DeInit();

    /* Set IsInitialized to 0 */
    pObj->IsInitialized = 0;
  }
  return WAVESHARE_OK;
}

/**
  * @brief  Read the component ID.
  * @param  pObj Component object
  * @param  Id   Component ID
  * @retval Component status
  */
int32_t WAVESHARE_ReadID(WAVESHARE_Object_t *pObj, uint32_t *Id)
{ 
  int32_t ret = WAVESHARE_OK;
  uint8_t tmp;
  *Id = 0x28;

  // Try reading register 0x00 and 0x01

  if(waveshare_read_reg(&pObj->Ctx, 0x00, &tmp, 1) != WAVESHARE_OK)
  {
    ret = WAVESHARE_ERROR;
  }
  if(waveshare_read_reg(&pObj->Ctx, WAVESHARE_CMD_ENABLE, &tmp, 1) != WAVESHARE_OK)
  {
    ret = WAVESHARE_ERROR;
  }
  // the rasperry Pi device is also on address 0x45 so if reading it's ID
  // returns C3 or DE then this is not a waveshare device
  if(waveshare_read_reg(&pObj->Ctx, 0x80, &tmp, 1) != WAVESHARE_OK)
  {
    ret = WAVESHARE_ERROR;
  }

  if ((tmp == 0xCE) || (tmp == 0xDE)) {
    ret = WAVESHARE_ERROR;
  }

  if (ret == WAVESHARE_OK) {
    *Id = 0x28; // this is made up, since we don't know a register to read
  }

  return ret;  
}  

/**
  * @brief  Set the display brightness.
  * @param  pObj Component object
  * @param  Brightness   display brightness to be set
  * @retval Component status
  */
int32_t WAVESHARE_SetBrightness(WAVESHARE_Object_t *pObj, uint32_t Brightness)
{
  int32_t ret;
  uint8_t brightness = (uint8_t)((Brightness * 255U)/100U);
  
  /* Send Display on DCS command to display */
  brightness = (0xFF - (uint8_t)brightness);
  ret = waveshare_write_reg(&pObj->Ctx, WAVESHARE_CMD_BRIGHTNESS, &brightness, 1);

  brightness = 0x01;
  ret += waveshare_write_reg(&pObj->Ctx, WAVESHARE_CMD_APPLY_BRIGHTNESS, &brightness, 1);

  if(ret != WAVESHARE_OK)
  {
    ret = WAVESHARE_ERROR;
  }
  else
  {
    WAVESHARECtx.Brightness = Brightness;
    ret = WAVESHARE_OK;
  }
  
  return ret;  
}

/**
  * @brief  Get the display brightness.
  * @param  pObj Component object
  * @param  Brightness   display brightness to be returned
  * @retval Component status
  */
int32_t WAVESHARE_GetBrightness(WAVESHARE_Object_t *pObj, uint32_t *Brightness)
{
  *Brightness = WAVESHARECtx.Brightness;
  return WAVESHARE_OK;  
}

/**
  * @brief  Set the display On.
  * @param  pObj Component object
  * @retval Component status
  */
int32_t WAVESHARE_DisplayOn(WAVESHARE_Object_t *pObj)
{
  int32_t ret;
  uint8_t display = 0x01; // 0x2 for ws_panel_13_3_2lane_mode
  
  /* Send Display on DCS command to display */
  if(waveshare_write_reg(&pObj->Ctx, WAVESHARE_CMD_ENABLE, &display, 1) != WAVESHARE_OK)
  {
    ret = WAVESHARE_ERROR;
  }
  else
  {
    ret = WAVESHARE_OK;
  }
  
  return ret;
}

/**
  * @brief  Set the display Off.
  * @param  pObj Component object
  * @retval Component status
  */
int32_t WAVESHARE_DisplayOff(WAVESHARE_Object_t *pObj)
{
  int32_t ret;
  uint8_t display = 0x00;
  
  if(waveshare_write_reg(&pObj->Ctx, WAVESHARE_CMD_ENABLE, &display, 1) != WAVESHARE_OK)
  {
    ret = WAVESHARE_ERROR;
  }
  else
  {
    ret = WAVESHARE_OK;
  }
  
  return ret;
}

/**
  * @brief  Set the display Orientation.
  * @param  pObj Component object
  * @param  Orientation   display Orientation to be set
  * @retval Component status
  */
int32_t WAVESHARE_SetOrientation(WAVESHARE_Object_t *pObj, uint32_t Orientation)
{
  int32_t ret;
  if((Orientation != WAVESHARE_ORIENTATION_LANDSCAPE) && (Orientation != WAVESHARE_ORIENTATION_PORTRAIT))
  {
    ret = WAVESHARE_ERROR;
  }/* Send command to configure display orientation mode  */
  else if(Orientation == WAVESHARE_ORIENTATION_LANDSCAPE)
  {
    ret = WAVESHARE_ERROR; // TODO Not yet supported
    //WAVESHARECtx.Orientation = WAVESHARE_ORIENTATION_LANDSCAPE;
  }
  else
  {
    WAVESHARECtx.Orientation = WAVESHARE_ORIENTATION_PORTRAIT;
  }
  
  if(ret != WAVESHARE_OK)
  {
    ret = WAVESHARE_ERROR;
  }
  
  return ret;
}

/**
  * @brief  Set the display Orientation.
  * @param  pObj Component object
  * @param  Orientation   display Orientation to be returned
  * @retval Component status
  */
int32_t WAVESHARE_GetOrientation(WAVESHARE_Object_t *pObj, uint32_t *Orientation)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  *Orientation = WAVESHARECtx.Orientation;
  
  return WAVESHARE_OK;
}

/**
  * @brief  Set the display Width.
  * @param  pObj Component object
  * @param  Xsize   display Width to be set
  * @retval Component status
  */
int32_t WAVESHARE_GetXSize(WAVESHARE_Object_t *pObj, uint32_t *Xsize)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  switch(WAVESHARECtx.Orientation)
  {
  case WAVESHARE_ORIENTATION_PORTRAIT:
    *Xsize = WAVESHARE_2P8IN_640X480_WIDTH;
    break;
  case WAVESHARE_ORIENTATION_LANDSCAPE:
    *Xsize = WAVESHARE_2P8IN_640X480_WIDTH;
    break;
  default:
    *Xsize = WAVESHARE_2P8IN_640X480_WIDTH;
    break;
  }
  
  return WAVESHARE_OK;
}

/**
  * @brief  Set the display Height.
  * @param  pObj Component object
  * @param  Ysize   display Height to be set
  * @retval Component status
  */
int32_t WAVESHARE_GetYSize(WAVESHARE_Object_t *pObj, uint32_t *Ysize)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  switch(WAVESHARECtx.Orientation)
  {
  case WAVESHARE_ORIENTATION_PORTRAIT:
    *Ysize = WAVESHARE_2P8IN_480X640_HEIGHT;
    break;
  case WAVESHARE_ORIENTATION_LANDSCAPE:
    *Ysize = WAVESHARE_2P8IN_640X480_HEIGHT;
    break;
  default:
    *Ysize = WAVESHARE_2P8IN_480X640_HEIGHT;
    break;
  }
  
  return WAVESHARE_OK;
}

/**
  * @brief  Set the display cursor.
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @retval Component status
  */
int32_t WAVESHARE_SetCursor(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return WAVESHARE_ERROR;
}

/**
  * @brief  Draw Bitmap image
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  pBmp pointer to bmp data
  * @retval Component status
  */
int32_t WAVESHARE_DrawBitmap(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return WAVESHARE_ERROR;
}

int32_t WAVESHARE_FillRGBRect(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return WAVESHARE_ERROR;
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
int32_t WAVESHARE_DrawHLine(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return WAVESHARE_ERROR;
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
int32_t WAVESHARE_DrawVLine(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return WAVESHARE_ERROR;
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
int32_t WAVESHARE_FillRect(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return WAVESHARE_ERROR;
}

/**
  * @brief  Get pixel color
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Color Color to be returned
  * @retval Component status
  */
int32_t WAVESHARE_GetPixel(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t *Color)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return WAVESHARE_ERROR;
}

/**
  * @brief  Set pixel color
  * @param  pObj Component object
  * @param  Xpos X position
  * @param  Ypos Y position
  * @param  Color Color to be returned
  * @retval Component status
  */
int32_t WAVESHARE_SetPixel(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Color)
{
  /* Prevent unused argument(s) compilation warning */  
  (void)(pObj);
  
  return WAVESHARE_ERROR;
}

/**
  * @}
  */

/** @addtogroup WAVESHARE_Private_FunctionsPrototyes
  * @{
  */

static void WAVESHARE_SetI2C_Address(WAVESHARE_Object_t *pObj, uint8_t Address)
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
static int32_t WAVESHARE_ReadRegWrap(void *Handle, uint16_t Reg, uint8_t* pData, uint16_t Length)
{
  WAVESHARE_Object_t *pObj = (WAVESHARE_Object_t *)Handle;

  return pObj->IO.ReadReg(pObj->IO.Address << 1, Reg, pData, Length);
}

/**
  * @brief  Wrap component WriteReg to Bus Write function
  * @param  handle  Component object handle
  * @param  Reg  The target register address to write
  * @param  pData  The target register value to be written
  * @param  Length  buffer size to be written
  * @retval error status
  */
static int32_t WAVESHARE_WriteRegWrap(void *Handle, uint16_t Reg, uint8_t *pData, uint16_t Length)
{
  WAVESHARE_Object_t *pObj = (WAVESHARE_Object_t *)Handle;

  return pObj->IO.WriteReg(pObj->IO.Address << 1, Reg, pData, Length);
}

/**
  * @brief  WAVESHARE delay
  * @param  Delay  Delay in ms
  */
static int32_t WAVESHARE_IO_Delay(WAVESHARE_Object_t *pObj, uint32_t Delay)
{
  uint32_t tickstart;
  tickstart = pObj->IO.GetTick();
  while((pObj->IO.GetTick() - tickstart) < Delay)
  {
  }
  return WAVESHARE_OK;
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
