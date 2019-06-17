/***************************************************************************//**
 * @file
 * @brief This file implements the packet commands for RAIL test applications.
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
#include "circular_queue.h"
#include "buffer_pool_allocator.h"

#include "rail.h"
#include "rail_types.h"
#include "app_common.h"
#include "app_trx.h"

void printTxPacket(int argc, char **argv)
{
  // Use the packet print helper to print out the transmit payload
  printPacket(argv[0],
              txData,
              txDataLen,
              NULL);
}

static bool setTxPayloadHelper(int argc, char **argv)
{
  uint16_t offset = ciGetUnsigned(argv[1]);

  // Read as many bytes as have been supplied and set them
  for (int i = 2; i < argc; i++) {
    uint32_t index = offset + i - 2;
    uint8_t value = ciGetUnsigned(argv[i]);

    // Make sure this fits in the txData buffer
    if (index >= sizeof(txData)) {
      responsePrintError(argv[0], 5, "Data overflows txData buffer");
      return false;
    }
    txData[index] = value;
  }

  if (railDataConfig.txMethod == PACKET_MODE) {
    RAIL_WriteTxFifo(railHandle, txData, txDataLen, true);
  }
  return true;
}

void setTxPayload(int argc, char **argv)
{
  if (setTxPayloadHelper(argc, argv)) {
    printTxPacket(1, argv);
  }
}

void setTxPayloadQuiet(int argc, char **argv)
{
  if (setTxPayloadHelper(argc, argv)) {
    uint16_t start = ciGetUnsigned(argv[1]);
    // Ignore first two arguments in end (command name, start index)
    responsePrint(argv[0], "start:%u,end:%u", start, start + argc - 2);
  }
}

void setTxLength(int argc, char **argv)
{
  uint32_t length = ciGetUnsigned(argv[1]);

  if (length > sizeof(txData)) {
    responsePrintError(argv[0], 6, "Invalid length %d", length);
    return;
  }

  txDataLen = length;
  if (railDataConfig.txMethod == PACKET_MODE) {
    RAIL_WriteTxFifo(railHandle, txData, txDataLen, true);
  }
  responsePrint(argv[0], "TxLength:%d", txDataLen);
}

void printAckPacket(int argc, char **argv)
{
  // Use the packet print helper to print out the transmit payload
  printPacket(argv[0],
              ackData,
              ackDataLen,
              NULL);
}

void setAckPayload(int argc, char **argv)
{
  uint16_t offset = ciGetUnsigned(argv[1]);

  // Read as many bytes as have been supplied and set them
  for (int i = 2; i < argc; i++) {
    uint32_t index = offset + i - 2;
    uint8_t value = ciGetUnsigned(argv[i]);

    // Make sure this fits in the txData buffer
    if (index >= sizeof(ackData)) {
      responsePrintError(argv[0], 5, "Data overflows ackData buffer");
      return;
    }
    ackData[index] = value;
  }

  RAIL_WriteAutoAckFifo(railHandle, ackData, ackDataLen);
  printAckPacket(1, argv);
}

void setAckLength(int argc, char **argv)
{
  uint32_t length = ciGetUnsigned(argv[1]);

  if (length > sizeof(ackData)) {
    responsePrintError(argv[0], 6, "Invalid length %d", length);
    return;
  }

  // Make sure we're using the txData array and set the length
  ackDataLen = length;
  RAIL_WriteAutoAckFifo(railHandle, ackData, ackDataLen);
  responsePrint(argv[0], "TxLength:%d", ackDataLen);
}

void setFixedLength(int argc, char **argv)
{
  if ((!inAppMode(NONE, argv[0]))
      || (!inRadioState(RAIL_RF_STATE_IDLE, argv[0]))) {
    return;
  }
  uint16_t fixedLength = ciGetUnsigned(argv[1]);
  fixedLength = RAIL_SetFixedLength(railHandle, fixedLength);
  configRxLengthSetting(fixedLength);

  // Print configured length
  responsePrint(argv[0], "FixedLength:%d", fixedLength);
}

void dataConfig(int argc, char **argv)
{
  if ((!inAppMode(NONE, argv[0]))
      || (!inRadioState(RAIL_RF_STATE_IDLE, argv[0]))) {
    return;
  }

  RAIL_Status_t status;

  // Don't configure other sources here
  railDataConfig.txSource = TX_PACKET_DATA;
  railDataConfig.rxSource = RX_PACKET_DATA;

  if (memcmp(argv[1], "pkt", 3) == 0) {
    // Packet mode
    railDataConfig.txMethod = PACKET_MODE;
  } else if (memcmp(argv[1], "fifo", 4) == 0) {
    railDataConfig.txMethod = FIFO_MODE;
  } else {
    responsePrintError(argv[0], 0x50, "Invalid Data Method selection.");
  }

  if (memcmp(argv[2], "pkt", 3) == 0) {
    // Packet mode
    railDataConfig.rxMethod = PACKET_MODE;
  } else if (memcmp(argv[2], "fifo", 4) == 0) {
    railDataConfig.rxMethod = FIFO_MODE;
  } else {
    responsePrintError(argv[0], 0x50, "Invalid Data Method selection.");
  }

  status = RAIL_ConfigData(railHandle, &railDataConfig);
  if (status) {
    responsePrintError(argv[0], 0x50, "Failed to successfully call RAIL_DataConfig: %d", status);
  } else {
    // @TODO Make this print nicer
    responsePrint(argv[0],
                  "TxMethod:%s,"
                  "RxMethod:%s",
                  argv[1],
                  argv[2]);
  }
}

static uint8_t rxFifo[RX_BUFFER_SIZE];

void setRxFifo(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  uint16_t rxFifoSize = ciGetUnsigned(argv[1]);
  if (rxFifoSize > RX_BUFFER_SIZE) {
    responsePrintError(argv[0], 0x54, "Max RX FIFO size in this build is %u",
                       RX_BUFFER_SIZE);
    return;
  }
  RAIL_Status_t status = RAIL_SetRxFifo(railHandle, &rxFifo[0], &rxFifoSize);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x55, "Setting RX FIFO failed");
    return;
  }
  responsePrint(argv[0], "RxFifoSize:%u", rxFifoSize);
}

RAIL_Status_t RAILCb_SetupRxFifo(RAIL_Handle_t railHandle)
{
  uint16_t rxFifoSize = RX_BUFFER_SIZE;
  RAIL_Status_t status = RAIL_SetRxFifo(railHandle, &rxFifo[0], &rxFifoSize);
  if (status == RAIL_STATUS_INVALID_STATE) {
    // Allow failures due to multiprotocol
    return RAIL_STATUS_NO_ERROR;
  }
  return status;
}

void fifoModeTestOptions(int argc, char **argv)
{
  char *outputStr[] = { "Disabled", "Enabled" };
  txFifoManual = ciGetUnsigned(argv[1]);
  rxFifoManual = ciGetUnsigned(argv[2]);

  responsePrint(argv[0],
                "TxFifoManual:%s,"
                "RxFifoManual:%s",
                outputStr[txFifoManual],
                outputStr[rxFifoManual]);
}

/**
 * Specify how many bytes to read out of the fifo and print to terminal
 */
void rxFifoManualRead(int argc, char **argv)
{
  if (!rxFifoManual) {
    responsePrintError(argv[0], 0x51, "Must be in rx fifo manual mode (fifoModeTestOptions).");
  } else {
    bool readAppendedInfo = ciGetUnsigned(argv[1]);
    uint16_t bytesToRead = ciGetUnsigned(argv[2]);
    bool printTimingInfo = (argc > 3) ? ciGetUnsigned(argv[3]) : false;
    void *rxPacketHandle = memoryAllocate(bytesToRead + sizeof(RailAppEvent_t));
    RailAppEvent_t *packetData = (RailAppEvent_t *)memoryPtrFromHandle(rxPacketHandle);

    if (packetData == NULL) {
      RAIL_ReleaseRxPacket(railHandle, RAIL_RX_PACKET_HANDLE_OLDEST);
      memoryFree(rxPacketHandle);
      return;
    }
    RAIL_RxPacketDetails_t *appendedInfo = &packetData->rxPacket.appendedInfo;

    // dataLength is number of bytes read from the fifo
    packetData->type = RX_PACKET;
    packetData->rxPacket.dataPtr = (uint8_t *)&packetData[1];
    packetData->rxPacket.dataLength = RAIL_ReadRxFifo(railHandle, packetData->rxPacket.dataPtr,
                                                      bytesToRead);

    if (readAppendedInfo) {
      RAIL_Status_t status = RAIL_STATUS_NO_ERROR;
      // Note the packet's status
      RAIL_RxPacketInfo_t packetInfo;
      RAIL_RxPacketHandle_t packetHandle
        = RAIL_GetRxPacketInfo(railHandle, RAIL_RX_PACKET_HANDLE_OLDEST,
                               &packetInfo);
      // assert(packetHandle != NULL);
      packetData->rxPacket.packetStatus = packetInfo.packetStatus;

      if (printTimingInfo) {
        RAIL_PacketTimePosition_t positions[] = {
          RAIL_PACKET_TIME_AT_PREAMBLE_START,
          RAIL_PACKET_TIME_AT_SYNC_END,
          RAIL_PACKET_TIME_AT_PACKET_END,
        };
        typedef RAIL_Status_t (*Adjustment)(RAIL_Handle_t, uint16_t, RAIL_Time_t *);
        Adjustment funcs[] = {
          RAIL_GetRxTimePreambleStart,
          RAIL_GetRxTimeSyncWordEnd,
          RAIL_GetRxTimeFrameEnd,
        };
        #define NUM_POSITIONS (sizeof(positions) / sizeof(positions[0]))
        RAIL_Time_t times[2 * NUM_POSITIONS];

        appendedInfo->timeReceived.totalPacketBytes = packetData->rxPacket.dataLength;
        for (uint8_t i = 0; i < NUM_POSITIONS; i++) {
          appendedInfo->timeReceived.timePosition = positions[i];
          RAIL_GetRxPacketDetails(railHandle, packetHandle, appendedInfo);
          times[i] = appendedInfo->timeReceived.packetTime;
        }
        // Get the appended info details and release this packet
        RAIL_GetRxPacketDetailsAlt(railHandle, packetHandle, appendedInfo);
        for (uint8_t i = 0; i < NUM_POSITIONS; i++) {
          times[i + NUM_POSITIONS] = appendedInfo->timeReceived.packetTime;
          funcs[i](railHandle, packetData->rxPacket.dataLength, &times[i + NUM_POSITIONS]);
        }
        responsePrint(argv[0],
                      "Pre:%u,Sync:%u,End:%u,PreAlt:%u,SyncAlt:%u,EndAlt:%u",
                      times[0], times[1], times[2],
                      times[3], times[4], times[5]);
      } else {
        // Get the appended info details and release this packet
        status = RAIL_GetRxPacketDetailsAlt(railHandle, packetHandle, appendedInfo);
        if (status == RAIL_STATUS_NO_ERROR) {
          RAIL_Time_t *sync = &appendedInfo->timeReceived.packetTime;
          status = RAIL_GetRxTimeSyncWordEnd(railHandle,
                                             packetData->rxPacket.dataLength, sync);
        }
      }

      RAIL_ReleaseRxPacket(railHandle, packetHandle);

      // Make sure there was a valid packet
      if (status != RAIL_STATUS_NO_ERROR) {
        memset(&packetData->rxPacket.appendedInfo, 0, sizeof(RAIL_RxPacketDetails_t));
        packetData->rxPacket.appendedInfo.rssi = RAIL_RSSI_INVALID_DBM;
        if (packetData->rxPacket.dataLength == 0) {
          responsePrintError(argv[0], 0x52, "No packet found in rx fifo!");
          memoryFree(rxPacketHandle);
          return;
        }
      }
    }

    queueAdd(&railAppEventQueue, rxPacketHandle);
  }
}

void fifoReset(int argc, char **argv)
{
  char *outputStr[] = { "DoNothing", "Reset" };
  bool txReset = ciGetUnsigned(argv[1]);
  bool rxReset = ciGetUnsigned(argv[2]);

  RAIL_ResetFifo(railHandle, txReset, rxReset);

  responsePrint(argv[0],
                "TxFifo:%s,"
                "RxFifo:%s",
                outputStr[txReset],
                outputStr[rxReset]);
}

void txFifoManualLoad(int argc, char**argv)
{
  if (!txFifoManual) {
    responsePrintError(argv[0], 0x51, "Must be in tx fifo manual mode (fifoModeTestOptions).");
  } else {
    // Test loading unaligned data
    loadTxData(txData, 1U);
    loadTxData(&txData[1], txDataLen - 1U);
    responsePrint(argv[0], "Status:Fifo Written");
  }
}

void abortRxPacket(int argc, char **argv)
{
  abortRxDelay = ciGetUnsigned(argv[1]);

  responsePrint(argv[0], "TimeAfterSyncToAbort:%d", abortRxDelay);
}

void peekRx(int argc, char **argv)
{
  uint16_t offset = 0;
  uint16_t bytesToPeek = ciGetUnsigned(argv[1]);
  uint8_t pDst[10];
  if (bytesToPeek > 10) {
    responsePrintError(argv[0], 0x52, "Can't peek more than 10 bytes");
    return;
  }
  if (argc > 2) {
    offset = ciGetUnsigned(argv[2]);
  }
  if (RAIL_PeekRxPacket(railHandle, RAIL_RX_PACKET_HANDLE_OLDEST,
                        pDst, bytesToPeek, offset) != bytesToPeek) {
    responsePrintError(argv[0], 0x53, "Requested bytes not in receive buffer");
    return;
  }
  printPacket(argv[0], pDst, bytesToPeek, NULL);
}
