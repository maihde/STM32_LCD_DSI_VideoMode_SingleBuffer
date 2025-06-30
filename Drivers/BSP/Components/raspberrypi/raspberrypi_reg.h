
/**
  ******************************************************************************
  * @file    raspberrypi_reg.h
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
#ifndef RASPBERRYPI_REG_H
#define RASPBERRYPI_REG_H

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

/** @addtogroup RASPBERRYPI
  * @{
  */


/** @defgroup RASPBERRYPI_Exported_Constants
  * @{
  */
    
#define _RPI_BIT(nr)                          (1UL << (nr))
#define LCDCTRL_MSF		                        _RPI_BIT(0) /* Magic square in RGB666 */
#define LCDCTRL_VTGEN		                      _RPI_BIT(4)/* Use chip clock for timing */
#define LCDCTRL_UNK6		                      _RPI_BIT(6) /* Unknown */
#define LCDCTRL_EVTMODE		                    _RPI_BIT(5) /* Event mode */
#define LCDCTRL_RGB888		                    _RPI_BIT(8) /* RGB888 mode */
#define LCDCTRL_HSPOL	                      	_RPI_BIT(17) /* Polarity of HSYNC signal */
#define LCDCTRL_DEPOL	                      	_RPI_BIT(18) /* Polarity of DE signal */
#define LCDCTRL_VSPOL		                      _RPI_BIT(19) /* Polarity of VSYNC signal */
#define LCDCTRL_VSDELAY(v)	                  (((v) & 0xfff) << 20) /* VSYNC delay */

/* Derived from Linux Device Driver */

#define  RASPBERRYPI_REG_ID                   0x80
#define  RASPBERRYPI_REG_PORTA                0x81
#define  RASPBERRYPI_REG_PORTB                0x82
#define  RASPBERRYPI_REG_PORTC                0x83
#define  RASPBERRYPI_REG_PORTD                0x84
#define  RASPBERRYPI_REG_POWERON              0x85
#define  RASPBERRYPI_REG_PWM                  0x86
#define  RASPBERRYPI_REG_DDRA                 0x87
#define  RASPBERRYPI_REG_DDRB                 0x88
#define  RASPBERRYPI_REG_DDRC                 0x89
#define  RASPBERRYPI_REG_DDRD                 0x8A
#define  RASPBERRYPI_REG_TEST                 0x8B
#define  RASPBERRYPI_REG_ADDRL                0x8C
#define  RASPBERRYPI_REG_ADDRH                0x8D
#define  RASPBERRYPI_REG_READH                0x8E
#define  RASPBERRYPI_REG_READL                0x8F
#define  RASPBERRYPI_REG_WRITEH               0x90
#define  RASPBERRYPI_REG_WRITEL               0x91
#define  RASPBERRYPI_REG_ID2                  0x92

#define  RASPBERRYPI_GPIO_RST_BRIDGE_N                    0x00	/* TC358762 bridge reset */
#define  RASPBERRYPI_GPIO_RST_RST_TP_N                    0x01	/* Touch controller reset */


#define RASPBERRYPI_PA_LCD_DITHB	                      	_RPI_BIT(0)
#define RASPBERRYPI_PA_LCD_MODE	                        	_RPI_BIT(1)
#define RASPBERRYPI_PA_LCD_LR	                          	_RPI_BIT(2)
#define RASPBERRYPI_PA_LCD_UD		                          _RPI_BIT(3)

#define RASPBERRYPI_PB_BRIDGE_PWRDNX_N	                  _RPI_BIT(0)
#define RASPBERRYPI_PB_LCD_VCC_N		                      _RPI_BIT(1)
#define RASPBERRYPI_PB_LCD_MAIN		                        _RPI_BIT(7)

#define RASPBERRYPI_PC_LED_EN		                          _RPI_BIT(0)
#define RASPBERRYPI_PC_RST_TP_N		                        _RPI_BIT(1)
#define RASPBERRYPI_PC_RST_LCD_N		                      _RPI_BIT(2)
#define RASPBERRYPI_PC_RST_BRIDGE_N	                     	_RPI_BIT(3)

// From tc358762.c
//   https://github.com/raspberrypi/linux/blob/rpi-6.12.y/drivers/gpu/drm/bridge/tc358762.c
// this file has more defines
//   https://github.com/leadercxn/linux-5.10.10/blob/d4794e27699bec3f6e8145f400e0b744329de38c/drivers/gpu/drm/panel/panel-raspberrypi-touchscreen.c

/* PPI layer registers */
#define RASPBERRYPI_PPI_STARTPPI		                      0x0104 /* START control bit */
#define RASPBERRYPI_PPI_LPTXTIMECNT		                    0x0114 /* LPTX timing signal */
#define RASPBERRYPI_PPI_D0S_ATMR		                      0x0144
#define RASPBERRYPI_PPI_D1S_ATMR		                      0x0148
#define RASPBERRYPI_PPI_D0S_CLRSIPOCOUNT	                0x0164 /* Assertion timer for Lane 0 */
#define RASPBERRYPI_PPI_D1S_CLRSIPOCOUNT	                0x0168 /* Assertion timer for Lane 1 */
#define RASPBERRYPI_PPI_START_FUNCTION	                  1

/* DSI layer registers */
#define RASPBERRYPI_DSI_STARTDSI		                      0x0204 /* START control bit of DSI-TX */
#define RASPBERRYPI_DSI_LANEENABLE		                    0x0210 /* Enables each lane */
#define RASPBERRYPI_DSI_RX_START		                      1

/* LCDC/DPI Host Registers, based on guesswork that this matches TC358764 */
#define RASPBERRYPI_LCDCTRL			                          0x0420 /* Video Path Control */
#define RASPBERRYPI_LCDCTRL_MSF		                        _RPI_BIT(0) /* Magic square in RGB666 */
#define RASPBERRYPI_LCDCTRL_VTGEN		                      _RPI_BIT(4)/* Use chip clock for timing */
#define RASPBERRYPI_LCDCTRL_UNK6		                      _RPI_BIT(6) /* Unknown */
#define RASPBERRYPI_LCDCTRL_EVTMODE		                    _RPI_BIT(5) /* Event mode */
#define RASPBERRYPI_LCDCTRL_RGB888		                    _RPI_BIT(8) /* RGB888 mode */
#define RASPBERRYPI_LCDCTRL_HSPOL		                      _RPI_BIT(17) /* Polarity of HSYNC signal */
#define RASPBERRYPI_LCDCTRL_DEPOL		                      _RPI_BIT(18) /* Polarity of DE signal */
#define RASPBERRYPI_LCDCTRL_VSPOL		                      _RPI_BIT(19) /* Polarity of VSYNC signal */
#define RASPBERRYPI_LCDCTRL_VSDELAY(v)	                  (((v) & 0xfff) << 20) /* VSYNC delay */

/* First parameter is in the 16bits, second is in the top 16bits */
#define RASPBERRYPI_LCD_HS_HBP		                        0x0424
#define RASPBERRYPI_LCD_HDISP_HFP	                      	0x0428
#define RASPBERRYPI_LCD_VS_VBP	                        	0x042c
#define RASPBERRYPI_LCD_VDISP_VFP		                      0x0430

/* SPI Master Registers */
#define RASPBERRYPI_SPICMR			                          0x0450
#define RASPBERRYPI_SPITCR			                          0x0454

/* System Controller Registers */
#define RASPBERRYPI_SYSCTRL		                          	0x0464

/* System registers */
#define RASPBERRYPI_LPX_PERIOD		                        3

#define RASPBERRYPI_DRM_MODE_FLAG_PHSYNC			            (1<<0)
#define RASPBERRYPI_DRM_MODE_FLAG_NHSYNC		            	(1<<1)
#define RASPBERRYPI_DRM_MODE_FLAG_PVSYNC		             	(1<<2)
#define RASPBERRYPI_DRM_MODE_FLAG_NVSYNC		            	(1<<3)
#define RASPBERRYPI_DRM_MODE_FLAG_INTERLACE	          		(1<<4)
#define RASPBERRYPI_DRM_MODE_FLAG_DBLSCAN		            	(1<<5)
#define RASPBERRYPI_DRM_MODE_FLAG_CSYNC		              	(1<<6)
#define RASPBERRYPI_DRM_MODE_FLAG_PCSYNC		            	(1<<7)
#define RASPBERRYPI_DRM_MODE_FLAG_NCSYNC		            	(1<<8)
#define RASPBERRYPI_DRM_MODE_FLAG_HSKEW		               	(1<<9) /* hskew provided */
#define RASPBERRYPI_DRM_MODE_FLAG_BCAST			              (1<<10) /* deprecated */
#define RASPBERRYPI_DRM_MODE_FLAG_PIXMUX			            (1<<11) /* deprecated */
#define RASPBERRYPI_DRM_MODE_FLAG_DBLCLK			            (1<<12)
#define RASPBERRYPI_DRM_MODE_FLAG_CLKDIV2			            (1<<13)

/* Lane enable PPI and DSI register bits */
#define RASPBERRYPI_LANEENABLE_CLEN		                    _RPI_BIT(0)
#define RASPBERRYPI_LANEENABLE_L0EN		                    _RPI_BIT(1)
#define RASPBERRYPI_LANEENABLE_L1EN		                    _RPI_BIT(2)

/**
  * @}
  */

/* Exported macro ------------------------------------------------------------*/
   
/** @defgroup RASPBERRYPI_Exported_Macros RASPBERRYPI Exported Macros
  * @{
  */ 

/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/
  
/** @addtogroup RASPBERRYPI_Exported_Functions
  * @{
  */
/************** Generic Function  *******************/

typedef int32_t (*RASPBERRYPI_WriteI2C_Func)(void *, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*RASPBERRYPI_ReadI2C_Func) (void *, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*RASPBERRYPI_WriteDSI_Func)(void *, uint16_t, uint8_t*, uint16_t);
typedef int32_t (*RASPBERRYPI_ReadDSI_Func) (void *, uint16_t, uint8_t*, uint16_t);

typedef struct
{
  RASPBERRYPI_WriteI2C_Func   WriteI2CReg;
  RASPBERRYPI_ReadI2C_Func    ReadI2CReg;
  RASPBERRYPI_WriteDSI_Func   WriteDSIReg;
  RASPBERRYPI_ReadDSI_Func    ReadDSIReg;
  void                  *handle;
} raspberrypi_ctx_t;

/*******************************************************************************
* Register      : Generic - All
* Address       : Generic - All
* Bit Group Name: None
* Permission    : W
*******************************************************************************/
int32_t raspberrypi_read_i2c_reg(raspberrypi_ctx_t *ctx, uint16_t reg, uint8_t *pdata, uint16_t length);
int32_t raspberrypi_write_i2c_reg(raspberrypi_ctx_t *ctx, uint16_t reg, const uint8_t *pdata, uint16_t length);
int32_t raspberrypi_read_dsi_reg(raspberrypi_ctx_t *ctx, uint16_t reg, uint32_t pdata);
int32_t raspberrypi_write_dsi_reg(raspberrypi_ctx_t *ctx, uint16_t reg, const uint32_t pdata);

/**
  * @}
  */ 
#ifdef __cplusplus
}
#endif

#endif /* RASPBERRYPI_REG_H */
/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */
