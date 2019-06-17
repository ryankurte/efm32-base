/***************************************************************************//**
 * @file
 * @brief DMADRV configuration file.
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
#ifndef __SILICON_LABS_DMADRV_CONFIG_H__
#define __SILICON_LABS_DMADRV_CONFIG_H__

#include "em_device.h"

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup DMADRV
 * @{
 ******************************************************************************/

/// DMADRV DMA interrupt priority configuration option.
/// Set DMA interrupt priority. Range is 0..7, 0 is highest priority.
#ifndef EMDRV_DMADRV_DMA_IRQ_PRIORITY
#if (__NVIC_PRIO_BITS == 2)
#define EMDRV_DMADRV_DMA_IRQ_PRIORITY 3
#else
#define EMDRV_DMADRV_DMA_IRQ_PRIORITY 4
#endif
#endif

/// DMADRV DMA channel priority configuration option.
/// Set DMA channel priority. Range 0..EMDRV_DMADRV_DMA_CH_COUNT.
/// On LDMA, this will configure channel 0 to CH_PRIORITY - 1 as fixed priority,
/// and CH_PRIORITY to CH_COUNT as round-robin.
/// On DMA, this will have no impact, since high priority is unuseable with
/// peripherals.
#ifndef EMDRV_DMADRV_DMA_CH_PRIORITY
#define EMDRV_DMADRV_DMA_CH_PRIORITY 0
#endif

/// DMADRV channel count configuration option.
/// Number of DMA channels to support. A lower DMA channel count will reduce
/// ram memory footprint.
#ifndef EMDRV_DMADRV_DMA_CH_COUNT
#define EMDRV_DMADRV_DMA_CH_COUNT DMA_CHAN_COUNT
#endif

/// DMADRV native API configuration option.
/// Use the native emlib api of the DMA controller, but still use DMADRV
/// housekeeping functions as AllocateChannel/FreeChannel etc.
#if defined(DOXY_DOC_ONLY)
#define EMDRV_DMADRV_USE_NATIVE_API
#else
//#define EMDRV_DMADRV_USE_NATIVE_API
#endif

/** @} (end addtogroup DMADRV) */
/** @} (end addtogroup emdrv) */

#endif /* __SILICON_LABS_DMADRV_CONFIG_H__ */
