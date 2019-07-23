/***************************************************************************//**
 * @file
 * @brief SLSTK3701A specific configuration for the display driver for
 *        the Sharp Memory LCD model LS013B7DH06.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#ifndef __SILICON_LABS_DISPLAYLS013B7DH06CONFIG_H__
#define __SILICON_LABS_DISPLAYLS013B7DH06CONFIG_H__

#include "displayconfigapp.h"
#include "em_gpio.h"

/* Display device name. */
#define SHARP_MEMLCD_DEVICE_NAME   "Sharp LS013B7DH06 #1"

/* LCD and SPI GPIO pin connections on the SLSTK3701A kit. */
#define LCD_PORT_SCLK             (gpioPortC)  /* EFM_DISP_SCLK on PC15 */
#define LCD_PIN_SCLK              (15)
#define LCD_PORT_SI               (gpioPortA)  /* EFM_DISP_MOSI on PA14 */
#define LCD_PIN_SI                (14)
#define LCD_PORT_SCS              (gpioPortC)  /* EFM_DISP_CS on PC14 */
#define LCD_PIN_SCS               (14)
#define LCD_PORT_EXTCOMIN         (gpioPortA)  /* EFM_DISP_COM on PA11 */
#define LCD_PIN_EXTCOMIN          (11)
#define LCD_PORT_DISP_SEL         (gpioPortA)  /* EFM_DISP_ENABLE on PA9 */
#define LCD_PIN_DISP_SEL          (9)

/* PRS settings for polarity inversion extcomin auto toggle.  */
#define LCD_AUTO_TOGGLE_PRS_CH    (11)  /* PRS channel 11.      */
#define LCD_AUTO_TOGGLE_PRS_ROUTELOC()  PRS->ROUTELOC2 = \
  ((PRS->ROUTELOC2 & ~_PRS_ROUTELOC2_CH11LOC_MASK) | PRS_ROUTELOC2_CH11LOC_LOC0)
#define LCD_AUTO_TOGGLE_PRS_ROUTEPEN    PRS_ROUTEPEN_CH11PEN

/*
 * Select how LCD polarity inversion should be handled:
 *
 * If POLARITY_INVERSION_EXTCOMIN is defined,
 * and the polarity inversion is armed for every rising edge of the EXTCOMIN
 * pin. The actual polarity inversion is triggered at the next transision of
 * SCS. This mode is recommended because it causes less CPU and SPI load than
 * the alternative mode, see below.
 * If POLARITY_INVERSION_EXTCOMIN is undefined,
 * the polarity inversion is toggled by sending an SPI command. This mode
 * causes more CPU and SPI load than using the EXTCOMIN pin mode.
 */
#define POLARITY_INVERSION_EXTCOMIN

/* Define POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE if you want the PAL
 * (Platform Abstraction Layer interface) to automatically toggle the EXTCOMIN
 *  pin.
 * If the PAL_TIMER_REPEAT function is defined the EXTCOMIN toggling is handled
 * by a timer repeat system, therefore we must undefine
 * POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE;
 */
#ifndef PAL_TIMER_REPEAT_FUNCTION
  #define POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE
#endif

#endif /* __SILICON_LABS_DISPLAYLS013B7DH06CONFIG_H__ */
