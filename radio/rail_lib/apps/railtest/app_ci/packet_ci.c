/***************************************************************************//**
 * @file packet_ci.c
 * @brief This file implements the packet commands for RAIL test applications.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
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
              transmitPayload.dataPtr,
              transmitPayload.dataLength,
              NULL);
}

void setTxPayload(int argc, char **argv)
{
  uint16_t offset = ciGetUnsigned(argv[1]);

  // Read as many bytes as have been supplied and set them
  for (int i = 2; i < argc; i++) {
    uint32_t index = offset + i - 2;
    uint8_t value = ciGetUnsigned(argv[i]);

    // Make sure this fits in the txData buffer
    if (index >= sizeof(txData)) {
      responsePrintError(argv[0], 5, "Data overflows txData buffer");
      return;
    }
    txData[index] = value;
  }

  transmitPayload.dataPtr = &txData[0];
  if (railDataConfig.txMethod == PACKET_MODE) {
    RAIL_TxDataLoad(&transmitPayload);
  }
  printTxPacket(1, argv);
}

void setTxLength(int argc, char **argv)
{
  uint32_t length = ciGetUnsigned(argv[1]);

  if (length > sizeof(txData)) {
    responsePrintError(argv[0], 6, "Invalid length %d", length);
    return;
  }

  // Make sure we're using the txData array and set the length
  transmitPayload.dataPtr = &txData[0];
  transmitPayload.dataLength = length;
  if (railDataConfig.txMethod == PACKET_MODE) {
    RAIL_TxDataLoad(&transmitPayload);
  }
  responsePrint(argv[0], "TxLength:%d", transmitPayload.dataLength);
}

void printAckPacket(int argc, char **argv)
{
  // Use the packet print helper to print out the transmit payload
  printPacket(argv[0],
              ackPayload.dataPtr,
              ackPayload.dataLength,
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

  ackPayload.dataPtr = &ackData[0];
  RAIL_AutoAckLoadBuffer(&ackPayload);
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
  ackPayload.dataPtr = &ackData[0];
  ackPayload.dataLength = length;
  RAIL_AutoAckLoadBuffer(&ackPayload);
  responsePrint(argv[0], "TxLength:%d", ackPayload.dataLength);
}

void setFixedLength(int argc, char **argv)
{
  if ((!inAppMode(NONE, argv[0]))
      || (!inRadioState(RAIL_RF_STATE_IDLE, argv[0]))) {
    return;
  }
  uint16_t fixedLength = ciGetUnsigned(argv[1]);
  fixedLength = RAIL_SetFixedLength(fixedLength);
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

  status = RAIL_DataConfig(&railDataConfig);
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
    void *packetHandle = memoryAllocate(bytesToRead);
    RAIL_RxPacketInfo_t *packetInfo = (RAIL_RxPacketInfo_t *)memoryPtrFromHandle(packetHandle);

    // dataLength is number of bytes read from the fifo
    packetInfo->dataLength = RAIL_ReadRxFifo(packetInfo->dataPtr, bytesToRead);

    if (readAppendedInfo) {
      RAIL_ReadRxFifoAppendedInfo(&(packetInfo->appendedInfo));
    }

    queueAdd(&rxPacketQueue, packetHandle);
  }
}

void fifoReset(int argc, char **argv)
{
  char *outputStr[] = { "DoNothing", "Reset" };
  bool txReset = ciGetUnsigned(argv[1]);
  bool rxReset = ciGetUnsigned(argv[2]);

  RAIL_ResetFifo(txReset, rxReset);

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
    loadTxData(&transmitPayload);
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
  if (RAIL_PeekRxPacket(pDst, bytesToPeek, offset) != bytesToPeek) {
    responsePrintError(argv[0], 0x53, "Requested bytes not in receive buffer");
    return;
  }
  printPacket(argv[0], pDst, bytesToPeek, NULL);
}
