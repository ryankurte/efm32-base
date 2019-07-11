/***************************************************************************//**
 * @file
 * @brief SSD2119 LCD controller driver
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
#ifndef __SSD2119_H
#define __SSD2119_H

#include <stdbool.h>
#include "em_device.h"

#ifdef __cplusplus
extern "C" {
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#ifdef SSD2119_REGISTER_ACCESS_HOOKS
  #define LCD_DEVFUNC_WRITEREGISTER 0x30
  #define LCD_DEVFUNC_READREGISTER  0x31
  #define LCD_DEVFUNC_WRITEDATA     0x32
  #define LCD_DEVFUNC_READDATA      0x33
#endif

#define LCD_DEVFUNC_INITIALIZE    0x34
#define LCD_DEVFUNC_CONTRADDR     0x35

/** @endcond */

#ifdef __cplusplus
}
#endif

#endif /* __SSD2119_H */
