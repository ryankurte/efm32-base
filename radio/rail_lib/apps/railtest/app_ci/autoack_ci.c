/***************************************************************************//**
 * @file
 * @brief This file implements the autoack commands in RAIL test apps.
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

#include <stdio.h>
#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "app_common.h"
#include "app_ci.h"

static RAIL_AutoAckConfig_t config;

void autoAckConfig(int argc, char **argv)
{
  config.enable = true;

  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  uint16_t timing;
  RAIL_Status_t status;

  if (memcmp(argv[1], "idle", 4) == 0) {
    config.rxTransitions.success = RAIL_RF_STATE_IDLE;
    config.txTransitions.success = RAIL_RF_STATE_IDLE;
  } else if (memcmp(argv[1], "rx", 2) == 0) {
    config.rxTransitions.success = RAIL_RF_STATE_RX;
    config.txTransitions.success = RAIL_RF_STATE_RX;
  } else {
    responsePrintError(argv[0], 0x20, "Unknown auto ack default state.");
    return;
  }

  RAIL_StateTiming_t timings = { 0 };

  timing = ciGetUnsigned(argv[2]);
  if (timing > 13000) {
    responsePrintError(argv[0], 0x21, "Invalid idle timing.");
    return;
  } else {
    timings.idleToTx = timing;
    timings.idleToRx = timing;
  }

  timing = ciGetUnsigned(argv[3]);
  if (timing > 13000) {
    responsePrintError(argv[0], 0x22, "Invalid turnaround timing");
    return;
  } else {
    timings.rxToTx = timing;
    // Make txToRx a little lower than desired. See documentation
    // on RAIL_ConfigAutoAck.
    timings.txToRx = timing - 10;
  }

  timing = ciGetUnsigned(argv[4]);
  config.ackTimeout = timing;

  RAIL_SetStateTiming(railHandle, &timings);
  status = RAIL_ConfigAutoAck(railHandle, &config);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], status, "Call to RAIL_ConfigAutoAck returned an error");
  } else {
    responsePrint(argv[0],
                  "rxDefaultState:%s,"
                  "txDefaultState:%s,"
                  "idleTiming:%d,"
                  "turnaroundTime:%d,"
                  "ackTimeout:%d",
                  getRfStateName(config.rxTransitions.success),
                  getRfStateName(config.txTransitions.success),
                  timings.idleToTx,
                  timings.rxToTx,
                  config.ackTimeout);
  }
}

void getAutoAck(int argc, char **argv)
{
  bool filteringEnabled = RAIL_IsAutoAckEnabled(railHandle);
  bool rxPaused = RAIL_IsRxAutoAckPaused(railHandle);
  bool txPaused = RAIL_IsTxAutoAckPaused(railHandle);

  responsePrint(argv[0],
                "AutoAck:%s,"
                "RxAutoAckStatus:%s,"
                "TxAutoAckStatus:%s",
                filteringEnabled ? "Enabled" : "Disabled",
                rxPaused ? "Paused" : "Unpaused",
                txPaused ? "Paused" : "Unpaused");
}

void autoAckDisable(int argc, char **argv)
{
  RAIL_StateTransitions_t transitions = {
    .success = RAIL_RF_STATE_IDLE,
    .error = RAIL_RF_STATE_IDLE
  };

  RAIL_AutoAckConfig_t autoAckConfig = {
    false,
    0,
    transitions,
    transitions
  };
  RAIL_Status_t status = RAIL_ConfigAutoAck(railHandle, &autoAckConfig);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], status, "Error disabling AutoAck");
  }
  getAutoAck(1, argv);
}

void autoAckPause(int argc, char **argv)
{
  uint8_t rxPause = ciGetUnsigned(argv[1]);
  uint8_t txPause = ciGetUnsigned(argv[2]);

  RAIL_PauseRxAutoAck(railHandle, !!rxPause);

  RAIL_PauseTxAutoAck(railHandle, !!txPause);

  getAutoAck(1, argv);
}

void setTxAckOptions(int argc, char **argv)
{
  uint8_t cancelAck = ciGetUnsigned(argv[1]);
  uint8_t useTxBuffer = ciGetUnsigned(argv[2]);

  afterRxCancelAck = ((cancelAck != 0) ? true : false);
  afterRxUseTxBufferForAck = ((useTxBuffer != 0) ? true : false);

  responsePrint(argv[0],
                "CancelAck:%s,"
                "UseTxBuffer:%s",
                afterRxCancelAck ? "True" : "False",
                afterRxUseTxBufferForAck ? "True" : "False");
}
