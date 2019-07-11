/***************************************************************************//**
 * @file
 * @brief Kit specific board defines for BRD8001A sensor expansion board.
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

#ifndef BRD8001A_CONFIG_H
#define BRD8001A_CONFIG_H

#include "em_device.h"
#include "em_gpio.h"

// Definition for the proximity detector part of SI1147
#define BRD8001A_INT_INPUT_PORT   gpioPortA
#define BRD8001A_INT_INPUT_PIN    1

// Definition for powering up sensor board STK
#define BRD8001A_POWER_PORT       gpioPortA
#define BRD8001A_POWER_PIN        0

#endif /* BRD8001A_CONFIG_H */
