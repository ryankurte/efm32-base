/***************************************************************************//**
 * @file
 * @brief BRD4101B specific configuration for the display driver for
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

#ifndef DISPLAY_LS013B7DH03_CONFIG_H
#define DISPLAY_LS013B7DH03_CONFIG_H

#include "displayconfigapp.h"

/* Display device name. */
#define SHARP_MEMLCD_DEVICE_NAME   "Sharp LS013B7DH03 #1"

/* LCD and SPI GPIO pin connections on the BRD4101B. */
#define LCD_PORT_SCLK             (gpioPortB)  /* EFM_DISP_SCLK on PB11 */
#define LCD_PIN_SCLK              (11)
#define LCD_PORT_SI               (gpioPortA)  /* EFM_DISP_MOSI on PA0 */
#define LCD_PIN_SI                (0)
#define LCD_PORT_SCS              (gpioPortC)  /* EFM_DISP_CS on PC6 */
#define LCD_PIN_SCS               (6)

/* The EFM8 I/O-expander will take care of display EXTCOMIN toggling. */
/* The following two defines will achieve this.                       */
#define POLARITY_INVERSION_EXTCOMIN_PAL_AUTO_TOGGLE
#define POLARITY_INVERSION_EXTCOMIN_MANUAL

#endif /* DISPLAY_LS013B7DH03_CONFIG_H */
