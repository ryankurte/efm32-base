/***************************************************************************//**
 * @file
 * @brief EFM32LG_STK3600 specific configuration for the display driver for
 *        the Sharp Memory LCD model LS013B7DH03.
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

#ifndef _DISPLAY_LS013B7DH03_CONFIG_H_
#define _DISPLAY_LS013B7DH03_CONFIG_H_

/* Display device name. */
#define SHARP_MEMLCD_DEVICE_NAME   "Sharp LS013B7DH03 #1"

/* LCD and SPI GPIO pin connections on the EFM32LG_STK3600. */
#define LCD_PORT_SCLK             (2)  /* = gpioPortC */
#define LCD_PIN_SCLK             (15)
#define LCD_PORT_SI               (3)  /* = gpioPortD */
#define LCD_PIN_SI                (7)
#define LCD_PORT_SCS              (4)  /* = gpioPortE */
#define LCD_PIN_SCS              (11)
#define LCD_PORT_EXTCOMIN         (4)  /* = gpioPortE */
#define LCD_PIN_EXTCOMIN         (10)
#define LCD_PORT_DISP             (0)  /* = gpioPortA */
#define LCD_PIN_DISP              (8)

/* PRS settings for polarity inversion extcomin auto toggle.  */
#define LCD_AUTO_TOGGLE_PRS_CH    (2)  /* PRS channel 2.      */
#define LCD_AUTO_TOGGLE_PRS_ROUTE_LOC   PRS_ROUTE_LOCATION_LOC2
#define LCD_AUTO_TOGGLE_PRS_ROUTE_PEN   PRS_ROUTE_CH2PEN

#define LCD_PORT_EXTMODE          (0)  /* = gpioPortA */
#define LCD_PIN_EXTMODE           (0)

/*
 * Select how LCD polarity inversion should be handled:
 *
 * If POLARITY_INVERSION_EXTCOMIN is defined,
 * the polarity inversion is armed for every rising edge of the EXTCOMIN
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
 */
#define POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE

#endif /* _DISPLAY_LS013B7DH03_CONFIG_H_ */
