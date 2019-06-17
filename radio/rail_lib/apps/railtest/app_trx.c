/***************************************************************************//**
 * @file
 * @brief RAILTEST transmit and receive events
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
#include <stdlib.h>
#include <string.h>

#include "rail.h"
#include "rail_types.h"

#include "buffer_pool_allocator.h"
#include "circular_queue.h"

#include "app_ci.h"
#include "app_common.h"
#include "app_trx.h"
#include "rail_ble.h"
/******************************************************************************
 * Variables
 *****************************************************************************/
RAIL_TxPacketDetails_t previousTxAppendedInfo = { .isAck = false, };
RAIL_TxPacketDetails_t previousTxAckAppendedInfo = { .isAck = true, };

static uint16_t dataLeft = 0;
static uint8_t *dataLeftPtr = NULL;

// Variables to keep track of the receive packet
static uint16_t rxLengthTarget;
static uint16_t rxLengthCount;
static void *rxFifoPacketHandle = 0;
static RailAppEvent_t *rxFifoPacketData;
static uint8_t *currentRxFifoPacketPtr;

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
static void packetMode_RxPacketAborted(RAIL_Handle_t railHandle)
{
  if (!printRxErrorPackets) {
    return;
  }
  RAIL_RxPacketInfo_t packetInfo;
  RAIL_RxPacketHandle_t packetHandle
    = RAIL_GetRxPacketInfo(railHandle, RAIL_RX_PACKET_HANDLE_NEWEST,
                           &packetInfo);
  // assert(packetHandle != NULL);
  // assert(packetInfo.packetBytes == 0);
  // Get a memory buffer for the received packet details
  void *rxPacketMemoryHandle = memoryAllocate(sizeof(RailAppEvent_t));
  RailAppEvent_t *rxPacket = (RailAppEvent_t *)memoryPtrFromHandle(rxPacketMemoryHandle);
  if (rxPacket != NULL) {
    rxPacket->type = RX_PACKET;
    rxPacket->rxPacket.dataPtr = NULL;
    rxPacket->rxPacket.packetStatus = packetInfo.packetStatus;
    rxPacket->rxPacket.dataLength = 0U;
    // Read what packet details are available into our packet structure
    if (RAIL_GetRxPacketDetailsAlt(railHandle, packetHandle,
                                   &rxPacket->rxPacket.appendedInfo)
        != RAIL_STATUS_NO_ERROR) {
      // assert(false);
      memset(&rxPacket->rxPacket.appendedInfo, 0, sizeof(rxPacket->rxPacket.appendedInfo));
    }
    if (logLevel & ASYNC_RESPONSE) {
      // Take an extra reference to this rx packet pointer so it's not released
      memoryTakeReference(rxPacketMemoryHandle);
      // Copy this received packet into our circular queue
      queueAdd(&railAppEventQueue, rxPacketMemoryHandle);
    }
    // Do not include Rx Error packets in PER's RSSI stats:
    // updateStats(rxPacket->appendedInfo.rssi, &counters.rssi);
  } else {
    counters.noRxBuffer++;
    eventsMissed++;
  }
  // Free the allocated memory now that we're done with it
  memoryFree(rxPacketMemoryHandle);
}

static void packetMode_RxPacketReceived(RAIL_Handle_t railHandle)
{
  RAIL_RxPacketInfo_t packetInfo;
  RAIL_RxPacketHandle_t packetHandle
    = RAIL_GetRxPacketInfo(railHandle, RAIL_RX_PACKET_HANDLE_NEWEST,
                           &packetInfo);
  // assert(packetHandle != NULL);
  uint16_t length = packetInfo.packetBytes;
  void *rxPacketMemoryHandle = memoryAllocate(sizeof(RailAppEvent_t) + length);
  RailAppEvent_t *rxPacket = (RailAppEvent_t *)memoryPtrFromHandle(rxPacketMemoryHandle);
  uint8_t *rxPacketData = (uint8_t *)&rxPacket[1];

  if (rxPacket != NULL) {
    rxPacket->type = RX_PACKET;
    rxPacket->rxPacket.dataPtr = rxPacketData;
    rxPacket->rxPacket.packetStatus = packetInfo.packetStatus;
    // Read packet data into our packet structure
    RAIL_CopyRxPacket(rxPacketData, &packetInfo);
    rxPacket->rxPacket.dataLength = length;
    // Read the appended info into our packet structure
    if (RAIL_GetRxPacketDetailsAlt(railHandle, packetHandle,
                                   &rxPacket->rxPacket.appendedInfo)
        != RAIL_STATUS_NO_ERROR) {
      // assert(false);
      memset(&rxPacket->rxPacket.appendedInfo, 0, sizeof(rxPacket->rxPacket.appendedInfo));
    }
    // Note that this does not take into account CRC bytes unless
    // RAIL_RX_OPTION_STORE_CRC is used
    rxPacket->rxPacket.appendedInfo.timeReceived.totalPacketBytes = length;
    if (RAIL_GetRxTimeSyncWordEndAlt(railHandle, &rxPacket->rxPacket.appendedInfo)
        != RAIL_STATUS_NO_ERROR) {
      // assert(false);
    }
  }

  if ((rxSuccessTransition == RAIL_RF_STATE_TX)
      || (RAIL_IsAutoAckEnabled(railHandle) && afterRxUseTxBufferForAck)) {
    // Load packet for either the non-AutoACK RXSUCCESS => TX transition,
    // or for the ACK transition when we intend to use the TX buffer
    // We don't do this in other circumstances in case of an RX2TX
    // transition to send a packet that's already been loaded,
    // which could cause TX underflow if we were to disturb it.
    RAIL_WriteTxFifo(railHandle, txData, txDataLen, true);
  }

  // Count packets that we received but had no memory to store
  if (rxPacket == NULL) {
    counters.noRxBuffer++;
  } else {
    // If we have just received an ACK, don't respond with an ACK
    if (rxPacketData[2] == 0xF1) {
      RAIL_CancelAutoAck(railHandle);
    }

    // Cancel ack if user requested
    if (afterRxCancelAck) {
      afterRxCancelAck = false;
      RAIL_CancelAutoAck(railHandle);
    }

    // Use Tx Buffer for Ack if user requested
    if (afterRxUseTxBufferForAck) {
      afterRxUseTxBufferForAck = false;
      RAIL_UseTxFifoForAutoAck(railHandle);
    }

    if (currentAppMode() == SCHTX_AFTER_RX) {
      // Schedule the next transmit after this receive
      RAIL_ScheduleTxConfig_t scheduledTxOptions = {
        .when = rxPacket->rxPacket.appendedInfo.timeReceived.packetTime
                + txAfterRxDelay,
        .mode = RAIL_TIME_ABSOLUTE,
        .txDuringRx = RAIL_SCHEDULED_TX_DURING_RX_POSTPONE_TX
      };

      setNextPacketTime(&scheduledTxOptions);

      txCount = 1;
      pendPacketTx();
    }

    if (logLevel & ASYNC_RESPONSE) {
      redrawDisplay = true;

      // Take an extra reference to this rx packet pointer so it's not released
      memoryTakeReference(rxPacketMemoryHandle);

      // Copy this received packet into our circular queue
      queueAdd(&railAppEventQueue, rxPacketMemoryHandle);
    }

    updateStats(rxPacket->rxPacket.appendedInfo.rssi, &counters.rssi);
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
    // Trigger an overflow by waiting in the interrupt handler
    usDelay(rxOverflowDelay);
  }

  if (phySwitchToRx.enable) {
    uint32_t syncTime = rxPacket->rxPacket.appendedInfo.timeReceived.packetTime;
    (void) RAIL_BLE_PhySwitchToRx(railHandle,
                                  phySwitchToRx.phy,
                                  phySwitchToRx.physicalChannel,
                                  phySwitchToRx.timeDelta + syncTime,
                                  phySwitchToRx.crcInit,
                                  phySwitchToRx.accessAddress,
                                  phySwitchToRx.logicalChannel,
                                  phySwitchToRx.disableWhitening);
  }

  // Free the allocated memory now that we're done with it
  memoryFree(rxPacketMemoryHandle);
}

// Only support fixed length
static void fifoMode_RxPacketReceived(void)
{
  uint16_t bytesRead;
  // Discard incoming data stream in BER mode.
  if (currentAppMode() == BER) {
    RAIL_ResetFifo(railHandle, true, true);
  } else {
    // Note the packet's status
    RAIL_RxPacketInfo_t packetInfo;
    RAIL_RxPacketHandle_t packetHandle
      = RAIL_GetRxPacketInfo(railHandle, RAIL_RX_PACKET_HANDLE_OLDEST,
                             &packetInfo);
    // assert(packetHandle != NULL);
    if (printRxErrorPackets
        || (packetInfo.packetStatus == RAIL_RX_PACKET_READY_CRC_ERROR)
        || (packetInfo.packetStatus == RAIL_RX_PACKET_READY_SUCCESS)) {
      // Keep and display this frame
      rxFifoPacketData->rxPacket.packetStatus = packetInfo.packetStatus;

      if (rxLengthCount > 0) {
        // Read the rest of the bytes out of the fifo
        bytesRead = RAIL_ReadRxFifo(railHandle, currentRxFifoPacketPtr, rxLengthCount);
        rxLengthCount -= bytesRead;
        currentRxFifoPacketPtr += bytesRead;
      }

      // Configure how many bytes were received
      rxFifoPacketData->rxPacket.dataLength = rxLengthTarget;

      // Get the appended info details
      if (RAIL_GetRxPacketDetailsAlt(railHandle, packetHandle,
                                     &rxFifoPacketData->rxPacket.appendedInfo)
          != RAIL_STATUS_NO_ERROR) {
        // assert(false);
        memset(&rxFifoPacketData->rxPacket.appendedInfo, 0, sizeof(rxFifoPacketData->rxPacket.appendedInfo));
      }
      // Note that this does not take into account CRC bytes unless
      // RAIL_RX_OPTION_STORE_CRC is used
      rxFifoPacketData->rxPacket.appendedInfo.timeReceived.totalPacketBytes = rxLengthTarget;
      RAIL_GetRxTimeSyncWordEndAlt(railHandle, &rxFifoPacketData->rxPacket.appendedInfo);
      queueAdd(&railAppEventQueue, rxFifoPacketHandle);
    } else {
      // Toss this frame and any of its data accumlated so far
      memoryFree(rxFifoPacketHandle);
    }
  }
}

/**
 * In Fifo mode, prepare for the upcoming rx
 *
 * Grab a buffer to store rx data
 * Keep track of writing data to this buffer
 */
void rxFifoPrep(void)
{
  // Don't allocate memory to save incoming data in BER mode.
  if ((railDataConfig.rxMethod == FIFO_MODE)
      && (currentAppMode() != BER)
      && !rxFifoManual) {
    rxLengthCount = rxLengthTarget;
    rxFifoPacketHandle = memoryAllocate(sizeof(RailAppEvent_t) + rxLengthTarget);
    rxFifoPacketData = (RailAppEvent_t *)memoryPtrFromHandle(rxFifoPacketHandle);
    uint8_t *rxPacketData = (uint8_t *)&rxFifoPacketData[1];
    rxFifoPacketData->type = RX_PACKET;
    rxFifoPacketData->rxPacket.dataPtr = rxPacketData;
    currentRxFifoPacketPtr = rxPacketData;
  }
}

/******************************************************************************
 * Public
 *****************************************************************************/
void loadTxData(uint8_t *data, uint16_t dataLen)
{
  uint16_t dataWritten;

  if (railDataConfig.txMethod == PACKET_MODE) {
    RAIL_WriteTxFifo(railHandle, data, dataLen, true);
  } else {
    dataWritten = RAIL_WriteTxFifo(railHandle, data, dataLen, false);
    dataLeft = dataLen - dataWritten;
    dataLeftPtr = data + dataWritten;
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
void RAILCb_TxPacketSent(RAIL_Handle_t railHandle, bool isAck)
{
  LedToggle(1);
  redrawDisplay = true;

  // Store the previous tx time for printing later
  if (isAck) {
    sentAckPackets++;
    // previousTxAckAppendedInfo.isAck already initialized true
    RAIL_Time_t *time = &previousTxAckAppendedInfo.timeSent.packetTime;
    (void) RAIL_GetTxPacketDetailsAlt(railHandle, true, time);
    (void) RAIL_GetTxTimeFrameEnd(railHandle, ackDataLen, time);
    pendFinishTxAckSequence();
  } else {
    internalTransmitCounter++;
    // previousTxAppendedInfo.isAck already initialized false
    RAIL_Time_t *time = &previousTxAppendedInfo.timeSent.packetTime;
    (void) RAIL_GetTxPacketDetailsAlt(railHandle, false, time);
    (void) RAIL_GetTxTimeFrameEnd(railHandle, txDataLen, time);
    scheduleNextTx();
  }
}

void RAILCb_RxPacketAborted(RAIL_Handle_t railHandle)
{
  if (railDataConfig.rxMethod == PACKET_MODE) {
    packetMode_RxPacketAborted(railHandle);
  } else if (!rxFifoManual) {
    fifoMode_RxPacketReceived();
  }
}

void RAILCb_RxPacketReceived(RAIL_Handle_t railHandle)
{
  counters.receive++;
  LedToggle(0);

  if (railDataConfig.rxMethod == PACKET_MODE) {
    packetMode_RxPacketReceived(railHandle);
  } else if (!rxFifoManual) {
    fifoMode_RxPacketReceived();
  }
}

void RAILCb_TxFifoAlmostEmpty(RAIL_Handle_t railHandle)
{
  uint16_t dataWritten;
  counters.txFifoAlmostEmpty++;

  if (dataLeft) {
    dataWritten = RAIL_WriteTxFifo(railHandle, dataLeftPtr, dataLeft, false);
    dataLeft -= dataWritten;
    dataLeftPtr += dataWritten;
  }
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

  while ((RAIL_GetRxFifoBytesAvailable(railHandle) > RAIL_GetRxFifoThreshold(railHandle))
         && !stopBerRx) {
    // Read multiple bytes in if they're available.
    // Reuse the txData[APP_MAX_PACKET_LENGTH] array since we won't be
    // transmitting in BER Test mode anyway.
    numBytes = RAIL_ReadRxFifo(railHandle, txData, APP_MAX_PACKET_LENGTH);

    for (uint16_t x = 0; x < numBytes && !stopBerRx; x++) {
      // Update BER statistics
      if (berStats.bytesTested < berStats.bytesTotal) {
        // Counters will not overflow since bytesTotal max value is capped.
        berStats.bitErrors += countBits(txData[x]);
        berStats.bytesTested++;
      } else {
        stopBerRx = true; // statistics are all gathered - stop now
      }
    }
  }
  berStats.rssi = (int8_t)(RAIL_GetRssi(railHandle, true) / 4); // disregard decimal point

  // stop RXing when enough bits are acquired or an error (i.e. RX overflow)
  if (stopBerRx) {
    RAIL_Idle(railHandle, RAIL_IDLE_FORCE_SHUTDOWN, true);
    RAIL_ResetFifo(railHandle, true, true);
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
    bytesRead = RAIL_ReadRxFifo(railHandle, currentRxFifoPacketPtr, bytesRead);
    rxLengthCount -= bytesRead;
    currentRxFifoPacketPtr += bytesRead;
  }
}

void RAILCb_RxFifoAlmostFull(RAIL_Handle_t railHandle)
{
  uint16_t bytesAvailable = RAIL_GetRxFifoBytesAvailable(railHandle);
  counters.rxFifoAlmostFull++;

  if (berTestModeEnabled) {
    berSource_RxFifoAlmostFull(bytesAvailable);
  } else if (RAIL_BLE_IsEnabled(railHandle)) {
    RAIL_ConfigEvents(railHandle, RAIL_EVENT_RX_FIFO_ALMOST_FULL,
                      RAIL_EVENTS_NONE); // Disable this event
  } else {
    packetSource_RxFifoAlmostFull(bytesAvailable);
  }
}

void setNextPacketTime(RAIL_ScheduleTxConfig_t *scheduledTxOptions)
{
  memcpy(&nextPacketTxTime, scheduledTxOptions, sizeof(RAIL_ScheduleTxConfig_t));
}
