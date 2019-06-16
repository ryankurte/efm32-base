/***************************************************************************//**
 * @file
 * @brief This file contains the BLE radio coexistence interface.
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

#ifndef __COEXISTENCE_BLE_H__
#define __COEXISTENCE_BLE_H__

#include "rail.h"
#include "coexistence-hal.h"

/**
 * Request permission to transmit from COEX master.
 *
 * @param[in] railHandle A RAIL instance handle.
 */
void COEX_BLE_Init(RAIL_Handle_t railHandle);

/**
 * Request permission to transmit from COEX master.
 *
 * @param[in] coexReq This parameter is either ON, OFF, PRIORITY or FORCED. PRIORITY AND FORCED can be
 *                    combined with ON and OFF.
 * @param[in] cb Callback fired when REQUEST is asserted.
 *
 * @return This function returns the true if request was set, false otherwise.
 */
bool COEX_BLE_SetTxRequest(COEX_Req_t coexReq, COEX_ReqCb_t cb);

/**
 * Request permission to receive from COEX master.
 *
 * @param[in] coexReq This parameter is either ON, OFF, PRIORITY or FORCED. PRIORITY AND FORCED can be
 *                    combined with ON and OFF.
 * @param[in] cb Callback fired when REQUEST is asserted.
 *
 * @return This function returns the true if request was set, false otherwise.
 */
bool COEX_BLE_SetRxRequest(COEX_Req_t coexReq, COEX_ReqCb_t cb);

/**
 * Software triggered COEX request.
 *
 * @param[in] coexReq This parameter is either ON, OFF, PRIORITY or FORCED. PRIORITY AND FORCED can be
 *                    combined with ON and OFF.
 * @param[in] cb Callback fired when REQUEST is asserted.
 *
 * @return This function returns the true if request was set, false otherwise.
 */
bool COEX_BLE_SetSwRequest(COEX_Req_t coexReq, COEX_ReqCb_t cb);

#endif  // __COEXISTENCE_BLE_H__
