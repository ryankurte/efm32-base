/***************************************************************************//**
 * @file rtcdrv_config.h
 * @brief RTCDRV configuration file.
 * @version 5.2.1
 *******************************************************************************
 * # License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/
#ifndef SILICON_LABS_RTCDRV_CONFIG_H
#define SILICON_LABS_RTCDRV_CONFIG_H

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

/// @brief Defines to let RTCDRV clock on LFRCO or PLFRCO, the default is LFXO.
//#define EMDRV_RTCDRV_USE_LFRCO
//#define EMDRV_RTCDRV_USE_PLFRCO

/** @} (end addtogroup RTCDRV) */
/** @} (end addtogroup emdrv) */

#endif /* SILICON_LABS_RTCDRV_CONFIG_H */
