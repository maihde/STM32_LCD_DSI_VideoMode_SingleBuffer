The Missing MB1233 Guide
========================

![stm32_with_rpi_display](https://github.com/user-attachments/assets/bb17e08c-3da7-40ec-a0ba-767148e9d99f)

ST sells an the [B-LCDAD-RP1](https://www.st.com/en/evaluation-tools/b-lcdad-rpi1.html) (aka MB1233) adapter that converts a Samtec High speed connector QTH-030 to the Raspberry Pi LCD connector (TE 1-1734248).

The problem is, there is little documentation or examples for how to *actually* get a RaspberryPi display running.

If you browse the STM32 forums, you will find lots of frustration.

```
I asked few display vendors and they told me that their
DSI displays are only compatible with RPi and cannot be
compatible with STM32. They do not explain why.

I see several posts on the Web where people tried using STM32
with B-LCDAD-RPI1 with RPi displays and failed. I cannot find
a single successful report.

But ST is selling this adapter B-LCDAD-RPI1 since 2016,
it must have tested it with something? Could you, please,
give us at least few displays, which were known to work.
```

This example finally shows how to get a RaspberryPi display working with the B-LCDAD-RP1.

Why was this so hard?
---------------------

Why was this so hard, for so long, with no one fixing it?  Here are my thoughts:

- display vendors appear to be allergic to sharing details about their displays
- you need to reverse-engineer the open-source Raspberry Pi Linux drivers to get those details.  The code is spread across multiple modules.
- the RaspberryPi displays require both DSI commands and I2C commands to work correctly
- over time the implementation of the Raspberry Pi display has been moved around, but old code remains in the Github repos (i.e. panel-raspberrypi-touchscreen.c has moved to panel-simple.c)
- the kernel includes various [quirks](https://github.com/6by9/linux/blob/ca6c31b49ae3d67bd21096ab8149cdc104bb8f82/drivers/gpu/drm/vc4/vc4_dsi.c#L888) that don't necessarily apply to clean STM32 implementation
- the raspberry PI dispaly *only* works with [one clock lane](https://github.com/raspberrypi/linux/blob/274049e5bbfc42888f6cff29beb404b7edf21237/drivers/gpu/drm/bridge/tc358762.c#L279).
- the displays are often very finicky about timings and clock rates, minor differences that should have no effect (i.e. DSI Byte Clock Lane 100 MHz vs 125 MHz) often do have an effect
- most STM32 devices only support a Maximum lane rate of 500 Mbit/s [page 134 of AN4860](https://www.st.com/resource/en/application_note/an4860-introduction-to-dsi-host-on-stm32-mcus-and-mpus-stmicroelectronics.pdf).  Because the Pi displays only appear to work with a single lane you need to drive the display beyond 500 Mbits/s.
- the STM32 CubeIDE [incorrectly](https://community.st.com/t5/stm32cubemx-mcus/why-dsi-clock-can-t-set-to-more-than-500m-in-stm32h747/td-p/783982) states that STM32H747 DSI byte lane cannot exceed 62MHz.  To get the STM32H747 (and possibly STM32H757 and STM32MP157 to work) you need to manually code the DSI display setup.
- some RaspberryPi displays will hold the DSI lines if the I2C initialization hasn't occured, preventing `HAL_DSI_Start` from working in the ST provided examples
- hobbyists give up trying to use off the shelf RasperryPi displays; professionals are working directly with display vendors and get the details they need

Displays Tested
---------------

"Raspberry PI" (i.e. tc358762 compatible bridges with a ATTINY controling power, backlight, and display setup):

- OK: The official [RaspberryPi 7" display](https://www.amazon.com/Raspberry-Pi-Official-Touch-Screen/dp/B073S3LQ6Q)
- Generally OK (Powers off randomly): [Hosyond 5" display](https://www.amazon.com/Hosyond-Touchscreen-Compatible-Capacitive-Driver-Free/dp/B0CXTFN8K9)
- NOT OKAY: [OSOYOO 3.5" display](https://www.amazon.com/dp/B0DQD3RFCT)

Other Raspberry Pi Displays:
- OK: [WaveShare 2.8"](https://www.amazon.com/2-8inch-Capacitive-Compatible-Resolution-DSI/dp/B0B2LKQ2QB)

How To Use This Example
-----------------------

This example is a modified version of the [LCD_DSI_VideoMode_SingleBuffer](https://github.com/STMicroelectronics/STM32CubeH7/tree/master/Projects/STM32H747I-DISCO/Examples/LCD_DSI/LCD_DSI_VideoMode_SingleBuffer) provided by ST for the STM32H747.

Edit the stm32H747i_discovery_conf.h and set one (and only one) of the appropriate `#define` lines to `1U`.

```
/* LCD controllers defines */
#define USE_LCD_CTRL_OTM8009A               0U
#define USE_LCD_CTRL_NT35510                0U
#define USE_LCD_CTRL_WAVESHARE_2P8          0U
#define USE_LCD_CTRL_RASPBERRYPI            0U
#define USE_LCD_CTRL_ADV7533                0U
```

TODO
----
- Touch events and integration
