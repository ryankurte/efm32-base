/***************************************************************************//**
 * @file autoack.c
 * @brief This file implements the autoack commands in RAIL test apps.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "app_common.h"
#include "app_ci.h"

static RAIL_AutoAckConfig_t config;
extern char *rfStates[];

void autoAckConfig(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  uint16_t timing;
  RAIL_Status_t status;

  if (memcmp(argv[1], "idle", 4) == 0) {
    config.defaultState = RAIL_RF_STATE_IDLE;
  } else if (memcmp(argv[1], "rx", 2) == 0) {
    config.defaultState = RAIL_RF_STATE_RX;
  } else {
    responsePrintError(argv[0], 0x20, "Unknown auto ack default state.");
    return;
  }

  timing = ciGetUnsigned(argv[2]);
  if (timing > 13000) {
    responsePrintError(argv[0], 0x21, "Invalid idle timing.");
    return;
  } else {
    config.idleTiming = timing;
  }

  timing = ciGetUnsigned(argv[3]);
  if (timing > 13000) {
    responsePrintError(argv[0], 0x22, "Invalid turnaround timing");
    return;
  } else {
    config.turnaroundTime = timing;
  }

  timing = ciGetUnsigned(argv[4]);
  config.ackTimeout = timing;

  status = RAIL_AutoAckConfig(&config);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], status, "Call to RAIL_AutoAckConfig returned an error");
  } else {
    responsePrint(argv[0],
                  "defaultState:%s,"
                  "idleTiming:%d,"
                  "turnaroundTime:%d,"
                  "ackTimeout:%d",
                  rfStates[config.defaultState],
                  config.idleTiming,
                  config.turnaroundTime,
                  config.ackTimeout);
  }
}

void getAutoAck(int argc, char **argv)
{
  bool filteringEnabled = RAIL_AutoAckIsEnabled();
  bool rxPaused = RAIL_AutoAckRxIsPaused();
  bool txPaused = RAIL_AutoAckTxIsPaused();

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
  RAIL_Status_t status;
  status = RAIL_AutoAckDisable();
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], status, "Error disabling AutoAck");
  }
  getAutoAck(1, argv);
}

void autoAckPause(int argc, char **argv)
{
  uint8_t rxEnable = ciGetUnsigned(argv[1]);
  uint8_t txEnable = ciGetUnsigned(argv[2]);

  if (rxEnable) {
    RAIL_AutoAckRxPause();
  } else {
    RAIL_AutoAckRxResume();
  }

  if (txEnable) {
    RAIL_AutoAckTxPause();
  } else {
    RAIL_AutoAckTxResume();
  }
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
