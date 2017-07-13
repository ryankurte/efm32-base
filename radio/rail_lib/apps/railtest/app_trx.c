/***************************************************************************//**
 * @file app_trx.c
 * @brief RAILTEST transmit and receive events
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rail.h"
#include "rail_types.h"

#include "buffer_pool_allocator.h"
#include "circular_queue.h"

#include "app_ci.h"
#include "app_common.h"
#include "rail_ble.h"
/******************************************************************************
 * Variables
 *****************************************************************************/
bool receivingPacket = false;
uint8_t  lastTxStatus = 0;
uint32_t previousTxTime = 0;

static uint16_t dataLeft = 0;
static uint8_t *dataLeftPtr = NULL;

// Variables to keep track of the receive packet
static uint16_t rxLengthTarget;
static uint16_t rxLengthCount;
static void *rxFifoPacketHandle = 0;
static RAIL_RxPacketInfo_t *rxFifoPacketInfo;
static uint8_t *currentRxFifoPacketPtr;

// Variables to keep track of BER test statistics
bool berTestModeEnabled = false;
static RAIL_BerStatus_t berStats = { 0 };
static uint32_t berTotalErrors;
static uint32_t berTotalBits;
static uint32_t berTotalBytesLeft;

uint32_t abortRxDelay = 0;

// fifo mode test variables
/*
 * This test bit is used to prevent RAILTEST from automatically loading packet
 * data into transmit fifo
 */
bool txFifoManual = false;
/*
 * This test bit is used to prevent RAILTEST from automatically reading out the
 * packet from the fifo
 */
bool rxFifoManual = false;

/******************************************************************************
 * Static
 *****************************************************************************/
static void packetMode_RxPacketReceived(void * rxPacketHandle)
{
  // Always load, in case we have to transition to TX
  RAIL_TxDataLoad(&transmitPayload);

  receivingPacket = false;
  RAIL_RxPacketInfo_t *rxPacketInfo;

  // Count packets that we received but had no memory to store
  rxPacketInfo = (RAIL_RxPacketInfo_t*)memoryPtrFromHandle(rxPacketHandle);
  if (rxPacketInfo == NULL) {
    counters.noRxBuffer++;
  } else {
    // If we have just received an ACK, don't respond with an ACK
    if (rxPacketInfo->dataPtr[2] == 0xF1) {
      RAIL_AutoAckCancelAck();
    }

    // Cancel ack if user requested
    if (afterRxCancelAck) {
      afterRxCancelAck = false;
      RAIL_AutoAckCancelAck();
    }

    // Use Tx Buffer for Ack if user requested
    if (afterRxUseTxBufferForAck) {
      afterRxUseTxBufferForAck = false;
      RAIL_AutoAckUseTxBuffer();
    }

    if (currentAppMode() == SCHTX_AFTER_RX) {
      // Schedule the next transmit after this receive
      setNextPacketTime(rxPacketInfo->appendedInfo.timeUs + txAfterRxDelay,
                        true);
      txCount = 1;
      pendPacketTx();
    }
  }

  if (logLevel & ASYNC_RESPONSE) {
    redrawDisplay = true;

    // If we ran out of buffers then we can't queue this up
    if (rxPacketInfo != NULL) {
      // Take an extra reference to this rx packet pointer so it's not released
      memoryTakeReference(rxPacketHandle);

      // Copy this received packet into our circular queue
      queueAdd(&rxPacketQueue, rxPacketHandle);
    }
  }

  // Track the state of scheduled Rx to figure out when it ends
  if (inAppMode(RX_SCHEDULED, NULL) && schRxStopOnRxEvent) {
    enableAppMode(RX_SCHEDULED, false, NULL);
  }

  // In Rx overflow test mode hang in this ISR to prevent processing new
  // packets to force an overflow
  if ((currentAppMode() == RX_OVERFLOW)) {
    enableAppMode(RX_OVERFLOW, false, NULL); // Switch back after the overflow
    changeAppModeIfPending();
    // 10 seconds should be enough to trigger an overflow
    usDelay(10 * 1000000);
  }
  updateStats(rxPacketInfo->appendedInfo.rssiLatch, &counters.rssi);
}

// Only support fixed length
static void fifoMode_RxPacketReceived(void)
{
  uint16_t bytesRead;
  // Discard incoming data stream in BER mode.
  if (currentAppMode() == BER) {
    RAIL_ResetFifo(true, true);
  } else {
    if (rxLengthCount > 0) {
      // Read the rest of the bytes out of the fifo
      bytesRead = RAIL_ReadRxFifo(currentRxFifoPacketPtr, rxLengthCount);
      rxLengthCount -= bytesRead;
      currentRxFifoPacketPtr += bytesRead;
    }
    // Configure how many bytes were received
    rxFifoPacketInfo->dataLength = rxLengthTarget;

    // Read Appended info out of the fifo
    RAIL_ReadRxFifoAppendedInfo(&(rxFifoPacketInfo->appendedInfo));
    queueAdd(&rxPacketQueue, rxFifoPacketHandle);
  }
}

/**
 * In Fifo mode, prepare for the upcoming rx
 *
 * Grab a buffer to store rx data
 * Keep track of writing data to this buffer
 */
static void rxFifoPrep(void)
{
  // Don't allocate memory to save incoming data in BER mode.
  if ((railDataConfig.rxMethod == FIFO_MODE)
      && (currentAppMode() != BER)
      && !rxFifoManual) {
    rxLengthCount = rxLengthTarget;
    rxFifoPacketHandle = memoryAllocate(rxLengthTarget);
    rxFifoPacketInfo = (RAIL_RxPacketInfo_t *)memoryPtrFromHandle(rxFifoPacketHandle);
    currentRxFifoPacketPtr = rxFifoPacketInfo->dataPtr;
  }
}

/******************************************************************************
 * Public
 *****************************************************************************/
void loadTxData(RAIL_TxData_t *transmitData)
{
  uint16_t dataWritten;

  if (railDataConfig.txMethod == PACKET_MODE) {
    RAIL_TxDataLoad(transmitData);
  } else {
    dataWritten = RAIL_WriteTxFifo(transmitData->dataPtr, transmitData->dataLength);
    dataLeft = transmitData->dataLength - dataWritten;
    dataLeftPtr = transmitData->dataPtr + dataWritten;
  }
}

void configRxLengthSetting(uint16_t rxLength)
{
  if (railDataConfig.rxMethod == FIFO_MODE) {
    rxLengthTarget = rxLength;
  }
}

/******************************************************************************
 * RAIL Callback Implementation
 *****************************************************************************/
void RAILCb_RxRadioStatusExt(uint32_t status)
{
  enqueueCallback(RAILCB_RX_RADIO_STATUS_EXT);
  if (status & RAIL_RX_CONFIG_FRAME_ERROR) {
    receivingPacket = false;
    counters.frameError++;
    LedToggle(1);
  }
  if (status & RAIL_RX_CONFIG_SYNC1_DETECT) {
    receivingPacket = true;
    counters.syncDetect++;
    rxFifoPrep();
    if (abortRxDelay != 0) {
      RAIL_TimerSet(abortRxDelay, RAIL_TIME_DELAY);
    }
  }
  if (status & RAIL_RX_CONFIG_PREAMBLE_DETECT) {
    counters.preambleDetect++;
  }
  if (status & RAIL_RX_CONFIG_BUFFER_OVERFLOW) {
    receivingPacket = false;
    counters.rxOfEvent++;
  }
  if (status & RAIL_RX_CONFIG_BUFFER_UNDERFLOW) {
    receivingPacket = false;
    counters.rxUfEvent++;
  }
  if (status & RAIL_RX_CONFIG_ADDRESS_FILTERED) {
    receivingPacket = false;
    counters.addrFilterEvent++;
  }
  if (status & RAIL_RX_CONFIG_RF_SENSED) {
    counters.rfSensedEvent++;
    if (counters.rfSensedEvent == 0) { // Wrap it to 1 not 0
      counters.rfSensedEvent = 1;
    }
  }
  if (status & RAIL_RX_CONFIG_PACKET_ABORTED) {
    counters.rxFail++;
  }
  // End scheduled receive mode if an appropriate end or error event is received
  if ((status & RAIL_RX_CONFIG_SCHEDULED_RX_END)
      || ((schRxStopOnRxEvent && inAppMode(RX_SCHEDULED, NULL))
          && (status & RAIL_RX_CONFIG_ADDRESS_FILTERED
              || status & RAIL_RX_CONFIG_BUFFER_OVERFLOW
              || status & RAIL_RX_CONFIG_FRAME_ERROR))) {
    enableAppMode(RX_SCHEDULED, false, NULL);
  }
  if (status & RAIL_RX_CONFIG_TIMING_LOST) {
    counters.timingLost++;
  }
  if (status & RAIL_RX_CONFIG_TIMING_DETECT) {
    counters.timingDetect++;
  }
}

void RAILCb_TxRadioStatus(uint8_t status)
{
  enqueueCallback(RAILCB_TX_RADIO_STATUS);
  if (status & (RAIL_TX_CONFIG_TX_ABORTED
                | RAIL_TX_CONFIG_TX_BLOCKED
                | RAIL_TX_CONFIG_CHANNEL_BUSY
                | RAIL_TX_CONFIG_BUFFER_UNDERFLOW
                | RAIL_TX_CONFIG_BUFFER_OVERFLOW)) {
    lastTxStatus = status;
    newTxError = true;
    failPackets++;
    scheduleNextTx();
  }
  if (status & RAIL_TX_CONFIG_BUFFER_UNDERFLOW) {
    counters.txUfEvent++;
  }
  if (status & RAIL_TX_CONFIG_BUFFER_OVERFLOW) {
    counters.txOfEvent++;
  }
  if (status & RAIL_TX_CONFIG_CHANNEL_CLEAR) {
    counters.lbtSuccess++;
  }
  if (status & RAIL_TX_CONFIG_CCA_RETRY) {
    counters.lbtRetry++;
  }
  if (status & RAIL_TX_CONFIG_START_CCA) {
    counters.lbtStartCca++;
  }
}

void RAILCb_TxPacketSent(RAIL_TxPacketInfo_t *txPacketInfo)
{
  enqueueCallback(RAILCB_TX_PACKET_SENT);
  counters.transmit++;
  internalTransmitCounter++;
  LedToggle(1);
  redrawDisplay = true;

  // Store the previous tx time for printing later
  previousTxTime = txPacketInfo->timeUs;

  scheduleNextTx();
}

void RAILCb_RxPacketReceived(void *rxPacketHandle)
{
  enqueueCallback(RAILCB_RX_PACKET_RECEIVED);
  counters.receive++;
  LedToggle(0);

  if (railDataConfig.rxMethod == PACKET_MODE) {
    packetMode_RxPacketReceived(rxPacketHandle);
  } else {
    if (!rxFifoManual) {
      fifoMode_RxPacketReceived();
    }
  }
}

void RAILCb_TxFifoAlmostEmpty(uint16_t spaceAvailable)
{
  enqueueCallback(RAILCB_TX_FIFO_ALMOST_EMPTY);
  uint16_t dataWritten;
  counters.txFifoAlmostEmpty++;

  if (dataLeft) {
    dataWritten = RAIL_WriteTxFifo(dataLeftPtr, dataLeft);
    dataLeft -= dataWritten;
    dataLeftPtr += dataWritten;
  }
}

void berGetStats(RAIL_BerStatus_t *status)
{
  *status =  (RAIL_BerStatus_t) {
    berStats.bitsTotal,
    berStats.bitsTested,
    berStats.bitErrors,
    berStats.rssi
  };
}

void berResetStats(uint32_t numBytes)
{
  /* Reset BER statistics */
  berTotalErrors = 0;
  berTotalBits = 0;
  // 0x1FFFFFFF bytes (0xFFFFFFF8 bits) is max number of bytes that can be
  // tested without uint32_t math rollover; numBytes = 0 is same as max
  if ((0 == numBytes) || (numBytes > 0x1FFFFFFF)) {
    numBytes = 0x1FFFFFFF;
  }
  berTotalBytesLeft = numBytes;

  /* Record num of bytes to test */
  berStats.bitsTotal = numBytes * 8;
  berStats.bitsTested = 0;
  berStats.rssi = 0;
  berStats.bitErrors = 0;
}

// count number of 1s in a byte without a loop
static uint8_t countBits(uint8_t num)
{
  uint8_t count = 0;
  static const uint8_t nibblebits[] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4 };
  count += nibblebits[num & 0x0F];
  count += nibblebits[num >> 4];
  return count;
}

static void berSource_RxFifoAlmostFull(uint16_t bytesAvailable)
{
  // All incoming bytes are received and validated here.
  uint16_t numBytes;
  bool stopBerRx = false;

  // If rxOfEvent is > 0, then we're overflowing the incoming RX buffer
  // probably because the BER test isn't processing incoming bits fast
  // enough. The test will automatically try to re-synchronize and read in bits
  // from the stream, but the bits under test will not be continuous. Abort
  // testing and notify the user if this is the case.
  if (counters.rxOfEvent) {
    stopBerRx = true;
  }

  while ((RAIL_GetRxFifoBytesAvailable() > RAIL_GetRxFifoThreshold())
         && !stopBerRx) {
    // Read multiple bytes in if they're available.
    // Reuse the txData[APP_MAX_PACKET_LENGTH] array since we won't be
    // transmitting in BER Test mode anyway.
    numBytes = RAIL_ReadRxFifo(txData, APP_MAX_PACKET_LENGTH);

    for (uint16_t x = 0; x < numBytes; x++) {
      // Update BER statistics
      if (berTotalBytesLeft > 0) {
        berTotalErrors += countBits(txData[x]);
        berTotalBits += 8;
        berTotalBytesLeft--;
      } else {
        stopBerRx = true; // statistics are all gathered - stop now
      }
    }
  }

  berStats.rssi = (int8_t)(RAIL_RxGetRSSI() >> 2); // chop off decimal point
  // always update this information and prevent overflow
  if (berTotalBits >= berStats.bitsTested) {
    berStats.bitsTested = berTotalBits;
  } else {
    berStats.bitsTested = 0xFFFFFFFF;
  }
  if (berTotalErrors >= berStats.bitErrors) {
    berStats.bitErrors = berTotalErrors;
  } else {
    berStats.bitErrors = 0xFFFFFFFF;
  }
  if (stopBerRx) {
    RAIL_RfIdleExt(RAIL_IDLE_FORCE_SHUTDOWN, true);
    RAIL_ResetFifo(true, true);
    berTestModeEnabled = false;
  }
}

// @TODO It would be better if we could 'register' callback contents
static void packetSource_RxFifoAlmostFull(uint16_t bytesAvailable)
{
  uint16_t bytesRead;
  if (rxLengthCount > 0) {
    // Amount to read is either bytes avilable or number of bytes remaining in packet
    bytesRead = (rxLengthCount > bytesAvailable) ? bytesAvailable : rxLengthCount;
    bytesRead = RAIL_ReadRxFifo(currentRxFifoPacketPtr, bytesRead);
    rxLengthCount -= bytesRead;
    currentRxFifoPacketPtr += bytesRead;
  }
}

void RAILCb_RxFifoAlmostFull(uint16_t bytesAvailable)
{
  enqueueCallback(RAILCB_RX_FIFO_ALMOST_FULL);
  counters.rxFifoAlmostFull++;

  if (berTestModeEnabled) {
    berSource_RxFifoAlmostFull(bytesAvailable);
  } else if (RAIL_BLE_IsEnabled()) {
    RAIL_DisableRxFifoThreshold();
  } else {
    packetSource_RxFifoAlmostFull(bytesAvailable);
  }
}
