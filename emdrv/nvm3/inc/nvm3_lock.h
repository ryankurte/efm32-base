/***************************************************************************//**
 * @file
 * @brief NVM3 data access lock API definition
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

#ifndef NVM3_LOCK_H
#define NVM3_LOCK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup NVM3
 * @{
 ******************************************************************************/

extern uint8_t nvm3_ccmBuf[];

/***************************************************************************//**
 * @addtogroup NVM3Lock
 * @brief NVM3 lock module
 * @{
 * @details
 * This module provides data protection tools for NVM3.
 *
 * The functions in this module are the default protection tools for NVM3.
 * The application can substitute the nvm3_lockBegin and nvm3_lockEnd
 * functions as long as the NVM3 functions are protected from
 * being called re-entrant.
 *
 * @note These functions are used by the NVM3 and should not be used by
 * any applications.
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *  Begin a lock section.
 ******************************************************************************/
void nvm3_lockBegin(void);

/***************************************************************************//**
 * @brief
 *  End a lock section.
 ******************************************************************************/
void nvm3_lockEnd(void);

/** @} (end addtogroup NVM3Lock) */
/** @} (end addtogroup NVM3) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif //NVM3_LOCK_H
