/***************************************************************************//**
 * @file
 * @brief RTCDRV configuration file.
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
#ifndef SILICON_LABS_RTCDRV_CONFIG_H
#define SILICON_LABS_RTCDRV_CONFIG_H

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
#if defined(HAL_CONFIG)
#include "hal-config.h"
#if defined(HAL_CLK_LFECLK_SOURCE)
#if (HAL_CLK_LFECLK_SOURCE == HAL_CLK_LFCLK_SOURCE_LFRCO)
#define EMDRV_RTCDRV_USE_LFRCO 1
#endif
#else
#if (HAL_CLK_LFACLK_SOURCE == HAL_CLK_LFCLK_SOURCE_LFRCO)
#define EMDRV_RTCDRV_USE_LFRCO 1
#endif
#endif
#endif
/// @endcond

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup RTCDRV
 * @{
 ******************************************************************************/

/// @brief Define the number of timers the application needs.
#define EMDRV_RTCDRV_NUM_TIMERS     (4)

/// @brief Define to include wallclock functionality.
//#define EMDRV_RTCDRV_WALLCLOCK_CONFIG

/// @brief Define to enable integration with SLEEP driver.
//#define EMDRV_RTCDRV_SLEEPDRV_INTEGRATION

/// @brief Define to configure RTCDRV for LFRCO or PLFRCO. The default is LFXO.
//#define EMDRV_RTCDRV_USE_LFRCO
//#define EMDRV_RTCDRV_USE_PLFRCO

/** @} (end addtogroup RTCDRV) */
/** @} (end addtogroup emdrv) */

#endif /* SILICON_LABS_RTCDRV_CONFIG_H */
