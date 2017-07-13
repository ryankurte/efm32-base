/***************************************************************************//**
 * @file parameter_ci.c
 * @brief This file implements the parameter commands for RAIL test applications.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include <string.h>
#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "rail_types.h"
#include "app_common.h"

#include "rail_config.h"

void getChannel(int argc, char **argv)
{
  if (RAIL_DebugModeGet() & RAIL_DEBUG_MODE_FREQ_OVERRIDE) {
    responsePrintError(argv[0], 0x12, "Channels are not valid in Debug Mode");
  } else {
    responsePrint(argv[0], "channel:%d", channel);
  }
}

void setChannel(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  int proposedChannel = ciGetUnsigned(argv[1]);
  bool success = false;

  // Make sure this is a valid channel
  if (RAIL_ChannelExists(proposedChannel) == RAIL_STATUS_NO_ERROR) {
    changeChannel(proposedChannel);
    success = true;
  }

  if (!success) {
    responsePrintError(argv[0], 0x11, "Invalid channel '%d'", proposedChannel);
    return;
  }

  getChannel(1, argv);
}

void getPower(int argc, char **argv)
{
  responsePrint(argv[0], "power:%d", RAIL_TxPowerGet());
}

void setPower(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  int32_t power = ciGetSigned(argv[1]);

  power = RAIL_TxPowerSet(power);
  responsePrint(argv[0], "power:%d", power);
}

void getTxDelay(int argc, char **argv)
{
  responsePrint(argv[0], "txDelay:%d", continuousTransferPeriod);
}

void setTxDelay(int argc, char **argv)
{
  uint32_t delay = ciGetUnsigned(argv[1]);

  continuousTransferPeriod = delay;
  getTxDelay(1, argv);
}

void getCtune(int argc, char **argv)
{
  uint32_t ctune = RAIL_GetTune();

  responsePrint(argv[0], "CTUNE:0x%.3x", ctune);
}

void setCtune(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  RAIL_SetTune(ciGetUnsigned(argv[1]));

  // Read out and print the current CTUNE value
  getCtune(1, argv);
}

void setPaCtune(int argc, char **argv)
{
  RAIL_Status_t status;
  uint8_t txVal = ciGetUnsigned(argv[1]);
  uint8_t rxVal = ciGetUnsigned(argv[2]);

  status = RAIL_PaCtuneSet(txVal, rxVal);

  if (status == RAIL_STATUS_NO_ERROR) {
    responsePrint(argv[0], "PACTUNETX:%d,PACTUNERX:%d", txVal, rxVal);
  } else {
    responsePrintError(argv[0], status, "Error");
  }
}

void getConfig(int argc, char **argv)
{
  responsePrint(argv[0], "Index:%u,Config:%s", currentConfig, configNames[currentConfig]);
}

void listConfigs(int argc, char **argv)
{
  responsePrintHeader(argv[0], "Index:%u,Config:%s");
  int i;
  for (i = 0; i < NUM_RAIL_CONFIGS; i++) {
    responsePrintMulti("Index:%u,Config:%s", i, configNames[i]);
  }
}

void setConfig(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  int newIndex = ciGetUnsigned(argv[1]);
  //Make sure index is valid
  if (newIndex < NUM_RAIL_CONFIGS) {
    //Change and print new config and channel
    changeRadioConfig(newIndex);
    getConfig(1, argv);
    getChannel(1, argv);
  } else {
    responsePrintError(argv[0], 8, "Invalid index %u", newIndex);
  }
}

// Helper to convert two strings to two RAIL RadioStates
static int8_t stringsToStates(char **strings, RAIL_RadioState_t *states)
{
  for (int i = 0; i < 2; i++) {
    switch (strings[i][0]) {
      case 'i': case 'I':
        states[i] =  RAIL_RF_STATE_IDLE;
        break;

      case 'r': case 'R':
        states[i] =  RAIL_RF_STATE_RX;
        break;

      case 't': case 'T':
        states[i] =  RAIL_RF_STATE_TX;
        break;

      default:
        return 1;
    }
  }
  return 0;
}

void setTxTransitions(int argc, char **argv)
{
  RAIL_RadioState_t states[2];
  if (stringsToStates(&argv[1], &states[0])) {
    responsePrintError(argv[0], 0x16, "Invalid states");
    return;
  }

  RAIL_Status_t ret = RAIL_SetTxTransitions(states[0], states[1]);
  responsePrint(argv[0], "TxTransitions:%s", (ret ? "Error" : "Set"));
}

void setRxTransitions(int argc, char **argv)
{
  RAIL_RadioState_t states[2];
  if (stringsToStates(&argv[1], &states[0])) {
    responsePrintError(argv[0], 0x16, "Invalid states");
    return;
  }
  uint8_t ignoreErrors = ciGetUnsigned(argv[3]);
  RAIL_Status_t ret = RAIL_SetRxTransitions(states[0], states[1], ignoreErrors);
  responsePrint(argv[0], "RxTransitions:%s", (ret ? "Error" : "Set"));
}

void setTimings(int argc, char **argv)
{
  uint16_t timing[4];

  for (int i = 1; i < 5; i++) {
    timing[i - 1] = ciGetUnsigned(argv[i]);
  }
  RAIL_StateTiming_t timings = (RAIL_StateTiming_t)
  {timing[0], timing[1], timing[2], timing[3] };
  if (!RAIL_SetStateTiming(&timings)) {
    responsePrint(argv[0], "IdleToRx:%u,RxToTx:%u,IdleToTx:%u,TxToRx:%u",
                  timings.idleToRx, timings.rxToTx,
                  timings.idleToTx, timings.txToRx);
  } else {
    responsePrintError(argv[0], 0x18, "Setting timings failed");
  }
}

void setTxFifoThreshold(int argc, char **argv)
{
  if (railDataConfig.txMethod != FIFO_MODE) {
    responsePrintError(argv[0], 0x19, "Tx is not in FIFO mode");
    return;
  }

  uint16_t txFifoThreshold = ciGetUnsigned(argv[1]);
  txFifoThreshold = RAIL_SetTxFifoThreshold(txFifoThreshold);
  responsePrint(argv[0], "TxFifoThreshold:%d", txFifoThreshold);
}

void setRxFifoThreshold(int argc, char **argv)
{
  uint16_t rxFifoThreshold = ciGetUnsigned(argv[1]);
  rxFifoThreshold = RAIL_SetRxFifoThreshold(rxFifoThreshold);
  responsePrint(argv[0], "RxFifoThreshold:%d", rxFifoThreshold);
}

void setRxConfig(int argc, char **argv)
{
  uint32_t rxConfig = ciGetUnsigned(argv[2]);

  if (strcasecmp("set", argv[1]) == 0) {
    railRxConfig = rxConfig;
  } else if (strcasecmp("and", argv[1]) == 0) {
    railRxConfig &= ~rxConfig;
  } else if (strcasecmp("or", argv[1]) == 0) {
    railRxConfig |= rxConfig;
  } else {
  }

  RAIL_RxConfig(railRxConfig, true);

  responsePrint(argv[0], "RxConfig:0x%u", railRxConfig);
}
