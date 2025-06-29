/**
  ******************************************************************************
  * @file    waveshare.h
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
#ifndef __WAVESHARE_H
#define __WAVESHARE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "waveshare_reg.h"
#include <stdint.h>

/** @addtogroup BSP
  * @{
  */

/** @addtogroup Components
  * @{
  */

/** @addtogroup WAVESHARE
  * @{
  */

/** @addtogroup WAVESHARE_Exported_Variables
  * @{
  */
typedef int32_t (*WAVESHARE_Init_Func)    (void);
typedef int32_t (*WAVESHARE_DeInit_Func)  (void);
typedef int32_t (*WAVESHARE_GetTick_Func) (void);
typedef int32_t (*WAVESHARE_Delay_Func)   (uint32_t);
typedef int32_t (*WAVESHARE_WriteReg_Func)(uint16_t, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*WAVESHARE_ReadReg_Func) (uint16_t, uint16_t, uint8_t*, uint16_t);

/** @addtogroup WAVESHARE_Exported_Types
  * @{
  */

typedef struct
{ 
  uint32_t  Orientation;
  uint32_t  ColorCode;
  uint32_t  Brightness;
} WAVESHARE_LCD_Ctx_t;

typedef struct
{
  WAVESHARE_Init_Func          Init;
  WAVESHARE_DeInit_Func        DeInit;
  uint16_t                  Address;  
  WAVESHARE_WriteReg_Func   WriteReg;
  WAVESHARE_ReadReg_Func    ReadReg;  
  WAVESHARE_GetTick_Func    GetTick; 
} WAVESHARE_IO_t;

typedef struct
{
  WAVESHARE_IO_t       IO;
  waveshare_ctx_t      Ctx; 
  uint8_t              IsInitialized;
} WAVESHARE_Object_t;

typedef struct
{
  /* Control functions */
  int32_t (*Init             )(WAVESHARE_Object_t*, uint32_t, uint32_t);
  int32_t (*DeInit           )(WAVESHARE_Object_t*);
  int32_t (*ReadID           )(WAVESHARE_Object_t*, uint32_t*);
  int32_t (*DisplayOn        )(WAVESHARE_Object_t*);
  int32_t (*DisplayOff       )(WAVESHARE_Object_t*);
  int32_t (*SetBrightness    )(WAVESHARE_Object_t*, uint32_t);
  int32_t (*GetBrightness    )(WAVESHARE_Object_t*, uint32_t*);
  int32_t (*SetOrientation   )(WAVESHARE_Object_t*, uint32_t);
  int32_t (*GetOrientation   )(WAVESHARE_Object_t*, uint32_t*);

  /* Drawing functions*/
  int32_t (*SetCursor        )(WAVESHARE_Object_t*, uint32_t, uint32_t);
  int32_t (*DrawBitmap       )(WAVESHARE_Object_t*, uint32_t, uint32_t, uint8_t*);
  int32_t (*FillRGBRect      )(WAVESHARE_Object_t*, uint32_t, uint32_t, uint8_t*, uint32_t, uint32_t);
  int32_t (*DrawHLine        )(WAVESHARE_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t (*DrawVLine        )(WAVESHARE_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t (*FillRect         )(WAVESHARE_Object_t*, uint32_t, uint32_t, uint32_t, uint32_t, uint32_t);
  int32_t (*GetPixel         )(WAVESHARE_Object_t*, uint32_t, uint32_t, uint32_t*);
  int32_t (*SetPixel         )(WAVESHARE_Object_t*, uint32_t, uint32_t, uint32_t);
  int32_t (*GetXSize         )(WAVESHARE_Object_t*, uint32_t *);
  int32_t (*GetYSize         )(WAVESHARE_Object_t*, uint32_t *);

}WAVESHARE_LCD_Drv_t;

/**
  * @}
  */

#define WAVESHARE_OK                (0)
#define WAVESHARE_ERROR             (-1)

#define WAVESHARE_I2C_ADDR          0x45U

/* WAVESHARE ID */
#define WAVESHARE_2P8IN_ID          0x28U // 2.8"

/**
 *  @brief LCD_OrientationTypeDef
 *  Possible values of Display Orientation
 */
#define WAVESHARE_ORIENTATION_PORTRAIT    ((uint32_t)0x00) /* Portrait orientation choice of LCD screen  */
#define WAVESHARE_ORIENTATION_LANDSCAPE   ((uint32_t)0x01) /* Landscape orientation choice of LCD screen */

/**
 *  @brief  Possible values of
 *  pixel data format (ie color coding) transmitted on DSI Data lane in DSI packets
 */
#define WAVESHARE_RGB888    ((uint32_t)0x00) /* Pixel format chosen is RGB888 : 24 bpp */
#define WAVESHARE_RBG565    ((uint32_t)0x02) /* Pixel format chosen is RGB565 : 16 bpp */

/**
  * @brief  WAVESHARE_2P8IN_480X640 Size
  */

/* Width and Height in Portrait mode */
#define  WAVESHARE_2P8IN_480X640_WIDTH             ((uint16_t)480)     /* LCD PIXEL WIDTH   */
#define  WAVESHARE_2P8IN_480X640_HEIGHT            ((uint16_t)640)     /* LCD PIXEL HEIGHT  */

/* Width and Height in Landscape mode */ // TODO IS THIS SUPPORTED?
#define  WAVESHARE_2P8IN_640X480_WIDTH             ((uint16_t)640)     /* LCD PIXEL WIDTH   */
#define  WAVESHARE_2P8IN_640X480_HEIGHT            ((uint16_t)480)     /* LCD PIXEL HEIGHT  */

/**
  * @brief  WAVESHARE_2P8IN_480X640 Timing parameters for Portrait orientation mode
  */
#define  WAVESHARE_2P8IN_480X640_HSYNC     ((uint16_t)50)      /* Horizontal synchronization */
#define  WAVESHARE_2P8IN_480X640_HBP       ((uint16_t)150)     /* Horizontal back porch      */
#define  WAVESHARE_2P8IN_480X640_HFP       ((uint16_t)150)     /* Horizontal front porch     */

#define  WAVESHARE_2P8IN_480X640_VSYNC     ((uint16_t)50)      /* Vertical synchronization   */
#define  WAVESHARE_2P8IN_480X640_VBP       ((uint16_t)150)     /* Vertical back porch        */
#define  WAVESHARE_2P8IN_480X640_VFP       ((uint16_t)150)     /* Vertical front porch       */

/**
  * @brief  WAVESHARE_2P8IN_480X640 Timing parameters for Landscape orientation mode
  *         Same values as for Portrait mode in fact.
  */
#define  WAVESHARE_2P8IN_640X480_HSYNC     WAVESHARE_2P8IN_480X640_VSYNC  /* Horizontal synchronization */
#define  WAVESHARE_2P8IN_640X480_HBP       WAVESHARE_2P8IN_480X640_VBP    /* Horizontal back porch      */
#define  WAVESHARE_2P8IN_640X480_HFP       WAVESHARE_2P8IN_480X640_VFP    /* Horizontal front porch     */
#define  WAVESHARE_2P8IN_640X480_VSYNC     WAVESHARE_2P8IN_480X640_HSYNC  /* Vertical synchronization   */
#define  WAVESHARE_2P8IN_640X480_VBP       WAVESHARE_2P8IN_480X640_HBP    /* Vertical back porch        */
#define  WAVESHARE_2P8IN_640X480_VFP       WAVESHARE_2P8IN_480X640_HFP    /* Vertical front porch       */

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/

/** @defgroup WAVESHARE_2P8IN_Exported_Macros WAVESHARE_2P8IN Exported Macros
  * @{
  */

/**
  * @}
  */

/* Exported functions --------------------------------------------------------*/

/** @addtogroup WAVESHARE_2P8IN_Exported_Functions
  * @{
  */
int32_t WAVESHARE_RegisterBusIO (WAVESHARE_Object_t *pObj, WAVESHARE_IO_t *pIO);
int32_t WAVESHARE_Init(WAVESHARE_Object_t *pObj, uint32_t ColorCoding, uint32_t Orientation);
int32_t WAVESHARE_DeInit(WAVESHARE_Object_t *pObj);
int32_t WAVESHARE_ReadID(WAVESHARE_Object_t *pObj, uint32_t *Id);
int32_t WAVESHARE_DisplayOn(WAVESHARE_Object_t *pObj);
int32_t WAVESHARE_DisplayOff(WAVESHARE_Object_t *pObj);
int32_t WAVESHARE_SetBrightness(WAVESHARE_Object_t *pObj, uint32_t Brightness);
int32_t WAVESHARE_GetBrightness(WAVESHARE_Object_t *pObj, uint32_t *Brightness);
int32_t WAVESHARE_SetOrientation(WAVESHARE_Object_t *pObj, uint32_t Orientation);
int32_t WAVESHARE_GetOrientation(WAVESHARE_Object_t *pObj, uint32_t *Orientation);


int32_t WAVESHARE_SetCursor(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos);
int32_t WAVESHARE_DrawBitmap(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pBmp);
int32_t WAVESHARE_FillRGBRect(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint8_t *pData, uint32_t Width, uint32_t Height);
int32_t WAVESHARE_DrawHLine(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t WAVESHARE_DrawVLine(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Length, uint32_t Color);
int32_t WAVESHARE_FillRect(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Width, uint32_t Height, uint32_t Color);
int32_t WAVESHARE_SetPixel(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t Color);
int32_t WAVESHARE_GetPixel(WAVESHARE_Object_t *pObj, uint32_t Xpos, uint32_t Ypos, uint32_t *Color);
int32_t WAVESHARE_GetXSize(WAVESHARE_Object_t *pObj, uint32_t *XSize);
int32_t WAVESHARE_GetYSize(WAVESHARE_Object_t *pObj, uint32_t *YSize);

extern WAVESHARE_LCD_Drv_t   WAVESHARE_LCD_Driver;

/**
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* __WAVESHARE_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
