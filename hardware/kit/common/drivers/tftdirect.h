/***************************************************************************//**
 * @file
 * @brief SSD2119 TFT initialization and setup for Direct Drive mode
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

#ifndef __TFTDIRECT_H
#define __TFTDIRECT_H

#include <stdbool.h>
#include "em_device.h"
#include "em_ebi.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Tft
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void TFT_DirectGPIOConfig(void);
bool TFT_DirectInit(const EBI_TFTInit_TypeDef *tftInit);

#ifdef __cplusplus
}
#endif

/** @} (end group Tft) */
/** @} (end group kitdrv) */

#endif
