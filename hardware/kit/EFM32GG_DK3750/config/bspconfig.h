/***************************************************************************//**
 * @file
 * @brief Provide BSP (board support package) configuration parameters.
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

#ifndef __BSPCONFIG_H
#define __BSPCONFIG_H

#define BSP_DK
#define BSP_DK_BRD3201
#define BSP_MCUBOARD_USB
#define BSP_MCUBOARD_BRD3600

#define BSP_GPIO_INT_PORT       gpioPortE
#define BSP_GPIO_INT_PIN        0
#define BSP_USB_STATUSLED_PORT  gpioPortE
#define BSP_USB_STATUSLED_PIN   1
#define BSP_USB_OCFLAG_PORT     gpioPortE
#define BSP_USB_OCFLAG_PIN      2
#define BSP_USB_VBUSEN_PORT     gpioPortF
#define BSP_USB_VBUSEN_PIN      5

#define BSP_CONFIG_NORFLASH_EBI_BANK (EBI_BANK3)

#include "bsp_dk_bcreg_3201.h"

#define BSP_DK_LEDS
#define BSP_NO_OF_LEDS  16
#define BSP_LED_MASK    0xFFFF
#define BSP_LED_PORT    (&BC_REGISTER->UIF_LEDS)

#define BSP_INIT_DEFAULT  BSP_INIT_DK_EBI

#endif
