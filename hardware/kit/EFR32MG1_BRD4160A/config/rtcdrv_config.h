/***************************************************************************//**
 * @file
 * @brief RTCDRV configuration file
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

#ifndef RTCDRV_CONFIG_H
#define RTCDRV_CONFIG_H

/***************************************************************************//**
 * @addtogroup EM_Drivers
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup RTCDRV
 * @{
 ******************************************************************************/

/// @brief Define the number of timers the application needs.
#define EMDRV_RTCDRV_NUM_TIMERS     (5)

/// @brief Define to include wallclock functionality.
#define EMDRV_RTCDRV_WALLCLOCK_CONFIG

/// @brief Define to enable integration with SLEEP driver.
//#define EMDRV_RTCDRV_SLEEPDRV_INTEGRATION

/** @} (end addtogroup RTCDRV) */
/** @} (end addtogroup EM_Drivers) */

#endif // RTCDRV_CONFIG_H
