/***************************************************************************//**
 * @file
 * @brief Energy Aware drivers error code definitions.
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
#ifndef __SILICON_LABS_ECODE_H__
#define __SILICON_LABS_ECODE_H__

#include <stdint.h>

/***************************************************************************//**
 * @defgroup emdrv EMDRV
 * @brief Gecko Platform driver library
 * @details
 *
 *  EMDRV is a set of function specific high performance drivers for EFM32, EZR32
 *  and EFR32 on-chip peripherals. Drivers are typically DMA based and are using
 *  all available low-energy features. For most drivers, the API offers both
 *  synchronous and asynchronous functions. The APIs are fully reentrant and
 *  callback based.
 *
 *  Most drivers are compile-time configurable to allow the highest possible
 *  code size optimization for the application. Compile-time configuration is
 *  defined in *_config.h files. Templates for the configuration files can be
 *  found in emdrv\\<driver>\\config.
 *
 * @note
 *	EMDRV APIs use @ref Ecode_t to return status.
 * @n @n
 *  With the exception of ezradiodrv, EMDRV does not implement RF support.
 *  Please refer to stack documentation for more information.
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief Typedef for API function error code return values.
 *
 * @details
 *        Bit 24-31:  Component, for example @ref emdrv @n
 *        Bit 16-23:  Module, for example @ref UARTDRV or @ref RTCDRV @n
 *        Bit 0-15:   Error code
 ******************************************************************************/
typedef uint32_t Ecode_t;

#define ECODE_EMDRV_BASE  (0xF0000000U)   ///< Base value for all EMDRV errorcodes.

#define ECODE_OK          (0U)            ///< Generic success return value.

#define ECODE_EMDRV_RTCDRV_BASE      (ECODE_EMDRV_BASE | 0x00001000U)   ///< Base value for RTCDRV error codes.
#define ECODE_EMDRV_SPIDRV_BASE      (ECODE_EMDRV_BASE | 0x00002000U)   ///< Base value for SPIDRV error codes.
#define ECODE_EMDRV_NVM_BASE         (ECODE_EMDRV_BASE | 0x00003000U)   ///< Base value for NVM error codes.
#define ECODE_EMDRV_USTIMER_BASE     (ECODE_EMDRV_BASE | 0x00004000U)   ///< Base value for USTIMER error codes.
#define ECODE_EMDRV_UARTDRV_BASE     (ECODE_EMDRV_BASE | 0x00007000U)   ///< Base value for UARTDRV error codes.
#define ECODE_EMDRV_DMADRV_BASE      (ECODE_EMDRV_BASE | 0x00008000U)   ///< Base value for DMADRV error codes.
#define ECODE_EMDRV_EZRADIODRV_BASE  (ECODE_EMDRV_BASE | 0x00009000U)   ///< Base value for EZRADIODRV error codes.
#define ECODE_EMDRV_TEMPDRV_BASE     (ECODE_EMDRV_BASE | 0x0000D000U)   ///< Base value for TEMPDRV error codes.
#define ECODE_EMDRV_NVM3_BASE        (ECODE_EMDRV_BASE | 0x0000E000U)   ///< Base value for NVM3 error codes.
#define ECODE_EMDRV_SLEEPTIMER_BASE  (ECODE_EMDRV_BASE | 0x0000F000U)   ///< Base value for SLEEPTIMER error codes.

/** @} (end addtogroup emdrv) */

#endif // __SILICON_LABS_ECODE_H__
