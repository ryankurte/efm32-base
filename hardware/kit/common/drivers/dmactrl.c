/***************************************************************************//**
 * @file
 * @brief DMA control data block.
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

#include "em_device.h"
#include "em_common.h"
#include "dmactrl.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup DmaCtrl
 * @brief uDMA control block array.
 *
 * @details
 *  Declaration of uDMA control block array.
 *
 * @{
 ******************************************************************************/

#if ( (DMA_CHAN_COUNT > 0) && (DMA_CHAN_COUNT <= 4) )
#define DMACTRL_CH_CNT      4
#define DMACTRL_ALIGNMENT   128

#elif ( (DMA_CHAN_COUNT > 4) && (DMA_CHAN_COUNT <= 8) )
#define DMACTRL_CH_CNT      8
#define DMACTRL_ALIGNMENT   256

#elif ( (DMA_CHAN_COUNT > 8) && (DMA_CHAN_COUNT <= 12) )
#define DMACTRL_CH_CNT      16
#define DMACTRL_ALIGNMENT   256

#else
#error "Unsupported DMA channel count"
#endif

/** DMA control block array aligned to @ref DMACTRL_ALIGNMENT */
SL_ALIGN(DMACTRL_ALIGNMENT)
DMA_DESCRIPTOR_TypeDef dmaControlBlock[DMACTRL_CH_CNT * 2] SL_ATTRIBUTE_ALIGN(DMACTRL_ALIGNMENT);

/** @} (end group DmaCtrl) */
/** @} (end group kitdrv) */
