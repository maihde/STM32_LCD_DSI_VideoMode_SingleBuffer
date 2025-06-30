/**
  ******************************************************************************
  * @file    raspberrypi.h
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RASPBERRYPI_H
#define __RASPBERRYPI_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "raspberrypi_reg.h"
#include <stdint.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup RASPBERRYPI
  * @{
  */

/** @addtogroup RASPBERRYPI_Exported_Variables
  * @{
  */
typedef int32_t (*RASPBERRYPI_Init_Func)    (void);
typedef int32_t (*RASPBERRYPI_DeInit_Func)  (void);
typedef int32_t (*RASPBERRYPI_GetTick_Func) (void);
typedef int32_t (*RASPBERRYPI_Delay_Func)   (uint32_t);
typedef int32_t (*RASPBERRYPI_WriteI2CReg_Func)(uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*RASPBERRYPI_ReadI2CReg_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*RASPBERRYPI_WriteDSIReg_Func)(uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*RASPBERRYPI_ReadDSIReg_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);

/** @addtogroup RASPBERRYPI_Exported_Types
  * @{
  */

typedef struct
{ 
  uint32_t  Orientation;
  uint32_t  ColorCode;
  uint32_t  Brightness;
} RASPBERRYPI_LCD_Ctx_t;

typedef struct
{
  RASPBERRYPI_Init_Func          Init;
  RASPBERRYPI_DeInit_Func        DeInit;
  uint16_t                  Address;  
  RASPBERRYPI_WriteI2CReg_Func   WriteI2CReg;
  RASPBERRYPI_ReadI2CReg_Func    ReadI2CReg;
  RASPBERRYPI_WriteDSIReg_Func   WriteDSIReg;
  RASPBERRYPI_ReadDSIReg_Func    ReadDSIReg;  
  RASPBERRYPI_GetTick_Func    GetTick; 
} RASPBERRYPI_IO_t;

typedef struct
{
  RASPBERRYPI_IO_t       IO;
  raspberrypi_ctx_t      Ctx; 
  uint8_t              IsInitialized;
} RASPBERRYPI_Object_t;

typedef struct
{
  /* Control functions */
  int32_t (*Init             )(RASPBERRYPI_Object_t*, uint32_t, uint32_t);
  int32_t (*DeInit           )(RASPBERRYPI_Object_t*);
  int32_t (*ReadID           )(RASPBERRYPI_Object_t*, uint32_t*);
  int32_t (*DisplayOn        )(RASPBERRYPI_Object_t*);
  int32_t (*DisplayOff       )(RASPBERRYPI_Object_t*);
  int32_t (*SetBrightness    )(RASPBERRYPI_Object_t*, uint32_t);
  int32_t (*GetBrightness    )(RASPBERRYPI_Object_t*, uint32_t*);
  int32_t (*SetOrientation   )(RASPBERRYPI_Object_t*, uint32_t);
  int32_t (*GetOrientation   )(RASPBERRYPI_Object_t*, uint32_t*);

  /* Drawing functions*/
  int32_t (*SetCursor        )(RASPBERRYPI_Object_t*, uint32_t, uint32_t);
  int32_t (*DrawBitmap       )(RASPBERRYPI_Object_t*, uint32_t, uint32_t, uint8_t*);
  int32_t (*FillRGBRect      )(RASPBERRYPI_Object_t*, uint32_t, uint32_t, uint8_t*, uint32_t, uint32_t);
  int32_t (*DrawHLine        )(RASPBERRYPI_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t (*DrawVLine        )(RASPBERRYPI_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t (*FillRect         )(RASPBERRYPI_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t (*GetPixel         )(RASPBERRYPI_Object_t*, uint32_t, uint32_t, uint32_t*);
  int32_t (*SetPixel         )(RASPBERRYPI_Object_t*, uint32_t, uint32_t, uint32_t);
  int32_t (*GetXSize         )(RASPBERRYPI_Object_t*, uint32_t *);
  int32_t (*GetYSize         )(RASPBERRYPI_Object_t*, uint32_t *);

}RASPBERRYPI_LCD_Drv_t;

/**
  * @}
  */

#define RASPBERRYPI_OK                (0)
#define RASPBERRYPI_ERROR             (-1)

#define RASPBERRYPI_I2C_ADDR          0x45U

/* RASPBERRYPI ID */
#define RASPBERRYPI_VER1_ID          0xDEU
#define RASPBERRYPI_VER2_ID          0xC3U

/**
 *  @brief LCD_OrientationTypeDef
 *  Possible values of Display Orientation
 */
#define RASPBERRYPI_ORIENTATION_PORTRAIT    ((uint32_t)0x00) /* Portrait orientation choice of LCD screen  */
#define RASPBERRYPI_ORIENTATION_LANDSCAPE   ((uint32_t)0x01) /* Landscape orientation choice of LCD screen */

/**
 *  @brief  Possible values of
 *  pixel data format (ie color coding) transmitted on DSI Data lane in DSI packets
 */
#define RASPBERRYPI_FORMAT_RGB888    ((uint32_t)0x00) /* Pixel format chosen is RGB888 : 24 bpp */
#define RASPBERRYPI_FORMAT_RBG565    ((uint32_t)0x02) /* Pixel format chosen is RGB565 : 16 bpp */

/**
  * @brief  RASPBERRYPI_480X800 Size
  */

/* Width and Height in Portrait mode */
#define  RASPBERRYPI_480X800_WIDTH             ((uint16_t)480)     /* LCD PIXEL WIDTH   */
#define  RASPBERRYPI_480X800_HEIGHT            ((uint16_t)800)     /* LCD PIXEL HEIGHT  */

/* Width and Height in Landscape mode */ // TODO IS THIS SUPPORTED?
#define  RASPBERRYPI_800X480_WIDTH             ((uint16_t)800)     /* LCD PIXEL WIDTH   */
#define  RASPBERRYPI_800X480_HEIGHT            ((uint16_t)480)     /* LCD PIXEL HEIGHT  */

/**
  * @brief  RASPBERRYPI_480X800 Timing parameters for Portrait orientation mode
  */
#define  RASPBERRYPI_480X800_HSYNC     ((uint16_t)2)      /* Horizontal synchronization */
#define  RASPBERRYPI_480X800_HBP       ((uint16_t)22)     /* Horizontal back porch      */
#define  RASPBERRYPI_480X800_HFP       ((uint16_t)7)     /* Horizontal front porch     */

#define  RASPBERRYPI_480X800_VSYNC     ((uint16_t)2)      /* Vertical synchronization   */
#define  RASPBERRYPI_480X800_VBP       ((uint16_t)45)     /* Vertical back porch        */
#define  RASPBERRYPI_480X800_VFP       ((uint16_t)131)     /* Vertical front porch       */

//#define  RASPBERRYPI_480X800_HSYNC     ((uint16_t)2)      /* Horizontal synchronization */
//#define  RASPBERRYPI_480X800_HBP       ((uint16_t)21)     /* Horizontal back porch      */
//#define  RASPBERRYPI_480X800_HFP       ((uint16_t)7)     /* Horizontal front porch     */

//#define  RASPBERRYPI_480X800_VSYNC     ((uint16_t)2)      /* Vertical synchronization   */
//#define  RASPBERRYPI_480X800_VBP       ((uint16_t)46)     /* Vertical back porch        */
//#define  RASPBERRYPI_480X800_VFP       ((uint16_t)1)     /* Vertical front porch       */

/**
  * @brief  RASPBERRYPI_480X800 Timing parameters for Landscape orientation mode
  *         Same values as for Portrait mode in fact.
  */
#define  RASPBERRYPI_800X480_HSYNC     RASPBERRYPI_480X800_VSYNC  /* Horizontal synchronization */
#define  RASPBERRYPI_800X480_HBP       RASPBERRYPI_480X800_VBP    /* Horizontal back porch      */
#define  RASPBERRYPI_800X480_HFP       RASPBERRYPI_480X800_VFP    /* Horizontal front porch     */
#define  RASPBERRYPI_800X480_VSYNC     RASPBERRYPI_480X800_HSYNC  /* Vertical synchronization   */
#define  RASPBERRYPI_800X480_VBP       RASPBERRYPI_480X800_HBP    /* Vertical back porch        */
#define  RASPBERRYPI_800X480_VFP       RASPBERRYPI_480X800_HFP    /* Vertical front porch       */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup RASPBERRYPI_Exported_Macros RASPBERRYPI_2P8IN Exported Macros
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @addtogroup RASPBERRYPI_Exported_Functions
  * @{
  */
int32_t RASPBERRYPI_RegisterBusIO (RASPBERRYPI_Object_t *pObj, RASPBERRYPI_IO_t *pIO);
int32_t RASPBERRYPI_PreInit(RASPBERRYPI_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation);
int32_t RASPBERRYPI_Init(RASPBERRYPI_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation);
int32_t RASPBERRYPI_DeInit(RASPBERRYPI_Object_t *pObj);
int32_t RASPBERRYPI_ReadID(RASPBERRYPI_Object_t *pObj, uint32_t *Id);
int32_t RASPBERRYPI_DisplayOn(RASPBERRYPI_Object_t *pObj);
int32_t RASPBERRYPI_DisplayOff(RASPBERRYPI_Object_t *pObj);
int32_t RASPBERRYPI_SetBrightness(RASPBERRYPI_Object_t *pObj, uint32_t Brightness);
int32_t RASPBERRYPI_GetBrightness(RASPBERRYPI_Object_t *pObj, uint32_t *Brightness);
int32_t RASPBERRYPI_SetOrientation(RASPBERRYPI_Object_t *pObj, uint32_t Orientation);
int32_t RASPBERRYPI_GetOrientation(RASPBERRYPI_Object_t *pObj, uint32_t *Orientation);


int32_t RASPBERRYPI_SetCursor(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos);
int32_t RASPBERRYPI_DrawBitmap(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp);
int32_t RASPBERRYPI_FillRGBRect(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height);
int32_t RASPBERRYPI_DrawHLine(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t RASPBERRYPI_DrawVLine(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t RASPBERRYPI_FillRect(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color);
int32_t RASPBERRYPI_SetPixel(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Color);
int32_t RASPBERRYPI_GetPixel(RASPBERRYPI_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t *Color);
int32_t RASPBERRYPI_GetXSize(RASPBERRYPI_Object_t *pObj, uint32_t *XSize);
int32_t RASPBERRYPI_GetYSize(RASPBERRYPI_Object_t *pObj, uint32_t *YSize);

extern RASPBERRYPI_LCD_Drv_t   RASPBERRYPI_LCD_Driver;

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __RASPBERRYPI_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
