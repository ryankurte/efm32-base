/***************************************************************************//**
 * @file
 * @brief Main configuration file for the DISPLAY driver software stack.
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

#ifndef __DISPLAYCONFIGALL_H
#define __DISPLAYCONFIGALL_H

#ifdef HAL_CONFIG
#include "displayhalconfig.h"
#else
/*
 * First, we list the default INCLUDE_XXX #defines which may be excluded later
 * by the kit or application specific configuration files.
 */
#define INCLUDE_PAL_GPIO_PIN_AUTO_TOGGLE

/* Then include the kit specific display configuration files which also includes
   the application specific configuration file and further selects which modules
   modules to include below. */

#include "displaypalconfig.h"
#include "displayconfig.h"
#endif

#ifdef INCLUDE_DISPLAY_SHARP_LS013B7DH03
#include "displayls013b7dh03config.h"
#endif

#ifdef INCLUDE_DISPLAY_SHARP_LS013B7DH06
#include "displayls013b7dh06config.h"
#endif

#ifdef INCLUDE_TEXTDISPLAY_SUPPORT
#include "textdisplayconfig.h"
#include "retargettextdisplayconfig.h"
#endif

#endif /* __DISPLAYCONFIGALL_H */
