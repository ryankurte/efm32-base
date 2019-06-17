/***************************************************************************//**
 * @file
 * @brief NVM3 data access lock API implementation
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

#include "nvm3_lock.h"
#include "nvm3.h"

#ifdef NVM3_HOST_BUILD
#include "nvm3_config.h"
#include "nvm3_trace.h"
#include "nvm3_hal.h"
#else
#include "em_core.h"
#endif

#ifdef NVM3_HOST_BUILD
#define SL_WEAK
#endif

#ifdef NVM3_HOST_BUILD
static int lockCount = 0;
#else
/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
CORE_DECLARE_IRQ_STATE;
/// @endcond
#endif

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup NVM3
 * @{
 ******************************************************************************/

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
nvm3_Obj_t nvm3_internalObjectHandleA;
nvm3_Obj_t nvm3_internalObjectHandleB;
nvm3_Obj_t nvm3_internalObjectHandleC;
nvm3_Obj_t nvm3_internalObjectHandleD;
const uint8_t nvm3_maxFragmentCount = NVM3_FRAGMENT_COUNT;
const size_t  nvm3_objHandleSize = sizeof(nvm3_Obj_t);

#ifdef NVM3_SUPPORT_ENCRYPTION
// These variables are only required if the nvm3 library supports encryption
uint8_t nvm3_ccmBuf[NVM3_MAX_OBJECT_SIZE + 8];
#endif
/// @endcond

/***************************************************************************//**
 * @addtogroup NVM3Lock
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @details
 * The default implementation is using @ref CORE_ENTER_CRITICAL().
 ******************************************************************************/
SL_WEAK void nvm3_lockBegin(void)
{
#ifdef NVM3_HOST_BUILD
  lockCount++;
#else
/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
  CORE_ENTER_CRITICAL();
/// @endcond
#endif
}

/***************************************************************************//**
 * @details
 * The default implementation is using @ref CORE_EXIT_CRITICAL().
 ******************************************************************************/
SL_WEAK void nvm3_lockEnd(void)
{
#ifdef NVM3_HOST_BUILD
  if (lockCount == 0) {
    nvm3_tracePrint(NVM3_TRACE_LEVEL_ERROR, "NVM3 ERROR - lockEnd: invalid lock count.\n");
  }
  lockCount--;
#else
  CORE_EXIT_CRITICAL();
#endif
}

/** @} (end addtogroup NVM3Lock) */
/** @} (end addtogroup NVM3) */
/** @} (end addtogroup emdrv) */
