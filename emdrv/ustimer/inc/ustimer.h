/***************************************************************************//**
 * @file
 * @brief Microsecond delay function API definition.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#ifndef __SILICON_LABS_USTIMER_H
#define __SILICON_LABS_USTIMER_H

#include <stdint.h>
#include "ecode.h"
#include "ustimer_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup USTIMER
 * @{
 ******************************************************************************/

#define ECODE_EMDRV_USTIMER_OK ( ECODE_OK ) ///< Success return value.

Ecode_t USTIMER_Init(void);
Ecode_t USTIMER_DeInit(void);
Ecode_t USTIMER_Delay(uint32_t usec);
Ecode_t USTIMER_DelayIntSafe(uint32_t usec);

#ifdef __cplusplus
}
#endif

/** @} (end group Ustimer) */
/** @} (end group Drivers) */

#endif
