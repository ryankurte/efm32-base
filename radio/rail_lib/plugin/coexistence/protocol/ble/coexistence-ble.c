/***************************************************************************//**
 * @file
 * @brief Radio coexistence BLE utilities
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

#include "coexistence-ble.h"

static RAIL_Handle_t myRailHandle;
static COEX_ReqState_t txReq;
static COEX_ReqState_t rxReq;
static COEX_ReqState_t swReq;

extern uint16_t RFRAND_GetPseudoRandom(void);

static void randomDelayCb(uint16_t randomDelayMaskUs)
{
  uint32_t startTime = RAIL_GetTime();
  uint16_t delay = RFRAND_GetPseudoRandom() & randomDelayMaskUs;

  while ((uint16_t)(RAIL_GetTime() - startTime) > delay) ;
}

static void eventsCb(COEX_Events_t events)
{
  if ((events & COEX_EVENT_GRANT_RELEASED) != 0U
      && (COEX_GetOptions() & COEX_OPTION_TX_ABORT) != 0U
      && (txReq.coexReq & COEX_REQ_ON) != 0U) {
    COEX_SetRequest(&txReq, COEX_REQ_OFF, NULL);
    (void)RAIL_StopTx(myRailHandle, RAIL_STOP_MODE_ACTIVE);
  }
  if ((events & COEX_EVENT_HOLDOFF_CHANGED) != 0U) {
    RAIL_EnableTxHoldOff(myRailHandle, (COEX_GetOptions() & COEX_OPTION_HOLDOFF_ACTIVE) != 0U);
  }
}

bool COEX_BLE_SetTxRequest(COEX_Req_t coexReq, COEX_ReqCb_t cb)
{
  return COEX_SetRequest(&txReq, coexReq, cb);
}

bool COEX_BLE_SetRxRequest(COEX_Req_t coexReq, COEX_ReqCb_t cb)
{
  return COEX_SetRequest(&rxReq, coexReq, cb);
}

bool COEX_BLE_SetSwRequest(COEX_Req_t coexReq, COEX_ReqCb_t cb)
{
  return COEX_SetRequest(&swReq, coexReq, cb);
}

void COEX_BLE_Init(RAIL_Handle_t railHandle)
{
  myRailHandle = railHandle;

  COEX_SetRandomDelayCallback(&randomDelayCb);
  COEX_SetRadioCallback(&eventsCb);
  COEX_HAL_Init();
}
