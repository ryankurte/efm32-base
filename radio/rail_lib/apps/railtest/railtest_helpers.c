/***************************************************************************//**
 * @file
 * @brief This file contains helpers for transitioning into the various
 *   AppModes
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

#include "app_common.h"

#include "response_print.h"
#include "command_interpreter.h"
#include "buffer_pool_allocator.h"

#include "rail.h"
#include "em_core.h"

const char *getRfStateName(RAIL_RadioState_t state)
{
  switch (state) {
    case RAIL_RF_STATE_IDLE:
      return "Idle";
    case RAIL_RF_STATE_RX:
      return "Rx";
    case RAIL_RF_STATE_TX:
      return "Tx";
    case RAIL_RF_STATE_RX_ACTIVE:
      return "RxActive";
    case RAIL_RF_STATE_TX_ACTIVE:
      return "TxActive";
    case RAIL_RF_STATE_INACTIVE:
      return "Inactive";
    default:
      //Check individual rail state bits if RAIL state is unknown
      return "Unknown";
  }
}

// Guard for CI functions to ensure a certain radio state before running
bool inRadioState(RAIL_RadioState_t state, char *command)
{
  RAIL_RadioState_t currentState = RAIL_GetRadioState(railHandle);
  bool ret;
  switch (state) {
    case RAIL_RF_STATE_INACTIVE:  // Match exactly
    case RAIL_RF_STATE_TX_ACTIVE: // Match exactly
    case RAIL_RF_STATE_RX_ACTIVE: // Match exactly
      ret = (currentState == state);
      break;
    case RAIL_RF_STATE_IDLE:      // Match IDLE or INACTIVE
      ret = (currentState <= RAIL_RF_STATE_IDLE);
      break;
    case RAIL_RF_STATE_RX:        // Match RX or RX_ACTIVE
    case RAIL_RF_STATE_TX:        // Match TX or TX_ACTIVE
      ret = ((currentState & state) == state);
      break;
    default:                      // Illegal state!?
      ret = false;
      break;
  }
  if (!ret && command) {
    responsePrintError(command, 0x17,
                       "Need to be in %s radio state for this command",
                       getRfStateName(state));
  }
  return ret;
}

const char *configuredRxAntenna(RAIL_RxOptions_t rxOptions)
{
  switch (rxOptions & (RAIL_RX_OPTION_ANTENNA_AUTO)) {
    case (RAIL_RX_OPTION_ANTENNA_AUTO): {
      return "Auto";
      break;
    }
    case (RAIL_RX_OPTION_ANTENNA0): {
      return "Antenna0";
      break;
    }
    case (RAIL_RX_OPTION_ANTENNA1): {
      return "Antenna1";
      break;
    }
    default: {
      return "Any";
      break;
    }
  }
}

void printRailAppEvents(void)
{
  // Print any newly received app events
  if (!queueIsEmpty(&railAppEventQueue)) {
    void *railtestEventHandle = queueRemove(&railAppEventQueue);
    RailAppEvent_t *railtestEvent =
      (RailAppEvent_t*) memoryPtrFromHandle(railtestEventHandle);
    if (railtestEvent->type == RX_PACKET) {
      // Print the received packet and appended info
      if (railtestEvent != NULL) {
        char *cmdName;
        uint8_t *dataPtr = NULL;
        switch (railtestEvent->rxPacket.packetStatus) {
          case RAIL_RX_PACKET_ABORT_FORMAT:
            cmdName = "rxErrFmt";
            break;
          case RAIL_RX_PACKET_ABORT_FILTERED:
            cmdName = "rxErrFlt";
            break;
          case RAIL_RX_PACKET_ABORT_ABORTED:
            cmdName = "rxErrAbt";
            break;
          case RAIL_RX_PACKET_ABORT_OVERFLOW:
            cmdName = "rxErrOvf";
            break;
          case RAIL_RX_PACKET_ABORT_CRC_ERROR:
            cmdName = "rxErrCrc";
            break;
          case RAIL_RX_PACKET_READY_CRC_ERROR:
          case RAIL_RX_PACKET_READY_SUCCESS:
            cmdName = "rxPacket";
            dataPtr = railtestEvent->rxPacket.dataPtr;
            break;
          default:
            cmdName = "rxErr???";
            break;
        }
        printPacket(cmdName,
                    dataPtr,
                    railtestEvent->rxPacket.dataLength,
                    &railtestEvent->rxPacket);
      }
    } else if (railtestEvent->type == BEAM_PACKET) {
      // Now print the most recent packet we may have received in Z-Wave mode
      responsePrint("ZWaveBeamFrame", "nodeId:0x%x,channelHopIdx:%d",
                    railtestEvent->beamPacket.nodeId, railtestEvent->beamPacket.channelIndex);
    } else if (railtestEvent->type == RAIL_EVENT) {
      printRailEvents(&railtestEvent->railEvent);
    } else if (railtestEvent->type == MULTITIMER) {
      responsePrint("multiTimerCb",
                    "TimerExpiredCallback:%u,"
                    "ConfiguredExpireTime:%u,"
                    "MultiTimerIndex:%d",
                    railtestEvent->multitimer.currentTime,
                    railtestEvent->multitimer.expirationTime,
                    railtestEvent->multitimer.index);
    } else if (railtestEvent->type == AVERAGE_RSSI) {
      char bufAverageRssi[10];
      averageRssi = (float)railtestEvent->rssi.rssi / 4;
      if (railtestEvent->rssi.rssi == RAIL_RSSI_INVALID) {
        responsePrint("getAvgRssi", "Could not read RSSI.");
      } else {
        sprintfFloat(bufAverageRssi, sizeof(bufAverageRssi), averageRssi, 2);
        responsePrint("getAvgRssi", "rssi:%s", bufAverageRssi);
      }
    }
    memoryFree(railtestEventHandle);
  }
  uint32_t eventsMissedCache = 0;
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  eventsMissedCache = eventsMissed;
  eventsMissed = 0;
  CORE_EXIT_CRITICAL();
  if (eventsMissedCache > 0) {
    responsePrint("missedRailAppEvents", "count:%u", eventsMissedCache);
  }
}
