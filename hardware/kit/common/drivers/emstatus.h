/***************************************************************************//**
 * @file
 * @brief EMSTATUS definitions.
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

#ifndef _EMSTATUS_H_
#define _EMSTATUS_H_

#include <stdint.h>
#include "em_types.h"          /* typedef uint32_t EMSTATUS; */

/** Common EMSTATUS codes: */
#define EMSTATUS_OK                   (0)            /**< Operation successful. */

/** EMSTATUS base codes for display modules. */
#define DISPLAY_EMSTATUS_BASE         (0x8011UL << 16) /**< EMSTATUS base code of DISPLAY driver interface. */
#define TEXTDISPLAY_EMSTATUS_BASE     (0x8012UL << 16) /**< EMSTATUS base code of TEXTDISPLAY module. */
#define PAL_EMSTATUS_BASE             (0x8013UL << 16) /**< EMSTATUS base code of PAL interface. */

#endif /*  _EMSTATUS_H_  */
