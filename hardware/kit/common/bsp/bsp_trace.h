/***************************************************************************//**
 * @file
 * @brief SWO Trace API (for eAProfiler)
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

#ifndef __BSP_TRACE_H
#define __BSP_TRACE_H

#include "em_device.h"
#if (defined(BSP_ETM_TRACE) && defined(ETM_PRESENT)) \
  || defined(GPIO_ROUTE_SWOPEN)                      \
  || defined(GPIO_ROUTEPEN_SWVPEN)                   \
  || defined(GPIO_TRACEROUTEPEN_SWVPEN)

#include <stdint.h>
#include <stdbool.h>
#if defined(HAL_CONFIG)
#include "tracehalconfig.h"
#else
#include "traceconfig.h"
#endif

/***************************************************************************//**
 * @addtogroup BSP
 * @{
 ******************************************************************************/
/***************************************************************************//**
 * @addtogroup BSPCOMMON Common BSP for all kits
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(BSP_ETM_TRACE) && defined(ETM_PRESENT)
void BSP_TraceEtmSetup(void);
#endif

#if defined(GPIO_ROUTE_SWOPEN) || defined(_GPIO_ROUTEPEN_SWVPEN_MASK) \
  || defined(GPIO_TRACEROUTEPEN_SWVPEN)
bool BSP_TraceProfilerSetup(void);
void BSP_TraceSwoSetup(void);
#endif

#ifdef __cplusplus
}
#endif

/** @} (end group BSP) */
/** @} (end group BSP) */

#endif /* (defined(BSP_ETM_TRACE) && defined( ETM_PRESENT )) || defined( GPIO_ROUTE_SWOPEN ) */
#endif /* __BSP_TRACE_H */
