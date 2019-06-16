/***************************************************************************//**
 * @file
 * @brief This file provides functionality to test RAIL error rates.
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

#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "app_common.h"
#include "app_ci.h"
#include "app_trx.h"
#include "em_core.h"

// Variables for PER testing
uint32_t perCount;
uint32_t perDelay;

// Variables for BER testing
BerStatus_t berStats = { 0 };
bool berTestModeEnabled = false;
static uint32_t berBytesToTest = 0;

void startPerMode(int argc, char **argv)
{
  uint32_t packets = ciGetUnsigned(argv[1]);
  uint32_t delayUs = ciGetUnsigned(argv[2]);

  if (!enableAppModeSync(PER, packets != 0, argv[0])) {
    return;
  }
  resetCounters(argc, argv);

  perCount = packets;
  perDelay = delayUs / 2;
  if (packets > 0) {
    RAIL_SetTimer(railHandle, perDelay, RAIL_TIME_DELAY, &RAILCb_TimerExpired);
  }
}

void updateStats(int32_t newValue, Stats_t *stats)
{
  stats->samples++;
  if (stats->samples == 1) {
    stats->min = newValue;
    stats->max = newValue;
    stats->mean = newValue;
    stats->varianceTimesSamples = 0;
  } else {
    stats->min = (newValue < stats->min) ? newValue : stats->min;
    stats->max = (newValue > stats->max) ? newValue : stats->max;

    float delta = newValue - stats->mean;
    stats->mean += delta / stats->samples;
    // wikipedia.org/wiki/Algorithms_for_calculating_variance#On-line_algorithm
    // Update by adding (newValue - oldMean) * (newValue - newMean)
    stats->varianceTimesSamples += delta * (newValue - stats->mean);
  }
}

float variance(const Stats_t stats)
{
  return stats.varianceTimesSamples / (stats.samples - 1);
}

void getPerStats(int argc, char **argv)
{
  char bufRssiMean[10];
  char bufRssiMin[10];
  char bufRssiMax[10];
  char bufRssiVariance[10];

  sprintfFloat(bufRssiMean, sizeof(bufRssiMean), counters.rssi.mean / 4, 0);
  sprintfFloat(bufRssiMin, sizeof(bufRssiMin), ((float) counters.rssi.min) / 4, 2);
  sprintfFloat(bufRssiMax, sizeof(bufRssiMax), ((float) counters.rssi.max) / 4, 2);
  sprintfFloat(bufRssiVariance, sizeof(bufRssiVariance), variance(counters.rssi) / 16, 0);

  responsePrint(argv[0],
                "PerTriggers:%u,"
                "RssiMean:%s,"
                "RssiMin:%s,"
                "RssiMax:%s,"
                "RssiVariance:%s",
                counters.perTriggers,
                bufRssiMean,
                bufRssiMin,
                bufRssiMax,
                bufRssiVariance);
}

void berResetStats(uint32_t numBytes)
{
  // Reset test statistics
  memset(&berStats, 0, sizeof(BerStatus_t));

  // 0x1FFFFFFF bytes (0xFFFFFFF8 bits) is max number of bytes that can be
  // tested without uint32_t math rollover; numBytes = 0 is same as max
  if ((0 == numBytes) || (numBytes > 0x1FFFFFFF)) {
    numBytes = 0x1FFFFFFF;
  }
  berStats.bytesTotal = numBytes;
}

void berConfigSet(int argc, char **argv)
{
  RAIL_Status_t status;
  uint16_t rxThreshold, packetLength;

  RAIL_Idle(railHandle, RAIL_IDLE_ABORT, true);
  RAIL_ResetFifo(railHandle, true, true);

  // configure radio
  railDataConfig.txSource = TX_PACKET_DATA;
  // railDataConfig.rxSource = RX_BER_STREAM;
  railDataConfig.rxSource = RX_PACKET_DATA;
  railDataConfig.txMethod = FIFO_MODE;
  railDataConfig.rxMethod = FIFO_MODE;
  status = RAIL_ConfigData(railHandle, &railDataConfig);
  if (status) {
    responsePrintError(argv[0], status, "Error calling RAIL_ConfigData().");
  }

  // configure RX FIFO
  rxThreshold = 100;
  rxThreshold = RAIL_SetRxFifoThreshold(railHandle, rxThreshold);

  // specify overall packet length info (infinite)
  packetLength = 0;
  packetLength = RAIL_SetFixedLength(railHandle, packetLength);

  RAIL_EnablePti(railHandle, false);

  berBytesToTest = ciGetUnsigned(argv[1]);
  berResetStats(berBytesToTest);

  responsePrint(argv[0], "NumBytes:%d", berBytesToTest);
}

void berRx(int argc, char **argv)
{
  bool enable = !!ciGetUnsigned(argv[1]);

  if (!enableAppModeSync(BER, enable, argv[0])) {
    return;
  }
  resetCounters(argc, argv);

  RAIL_Idle(railHandle, RAIL_IDLE_ABORT, true);
  RAIL_ResetFifo(railHandle, true, true);
  if (enable) {
    RAIL_EnablePti(railHandle, false);
    berResetStats(berBytesToTest);
    berTestModeEnabled = true;
    RAIL_StartRx(railHandle, channel, NULL);
  }
}

void berStatusGet(int argc, char **argv)
{
  float percentDone;
  float percentBitError;
  uint32_t bytesTotal; /**< Number of bytes to receive */
  uint32_t bytesTested; /**< Number of bytes currently tested */
  uint32_t bitErrors; /**< Number of bits errors detected */
  int8_t rssi; /**< Current RSSI value during pattern acquisition */
  CORE_DECLARE_IRQ_STATE;

  // be sure we don't get half new, half stale data
  CORE_ENTER_CRITICAL();
  bytesTotal = berStats.bytesTotal;
  bytesTested = berStats.bytesTested;
  bitErrors = berStats.bitErrors;
  rssi = berStats.rssi;
  CORE_EXIT_CRITICAL();

  // don't divide by 0
  if (0 != bytesTotal) {
    percentDone = (float)((((double)bytesTested) / bytesTotal) * 100);
  } else {
    percentDone = 0.0;
  }
  // don't divide by 0
  if (0 != bytesTested) {
    percentBitError = (float)((((double)bitErrors) / (bytesTested * 8)) * 100);
  } else {
    percentBitError = 0.0;
  }

  // If rxOfEvent is > 0, then we're overflowing the incoming RX buffer
  // probably because the BER test isn't processing incoming bits fast
  // enough. The test will automatically try to re-synchronize and read in bits
  // from the stream, but the bits under test will not be continuous. Abort
  // testing and notify the user if this is the case.
  if (counters.rxOfEvent) {
    responsePrint(argv[0],
                  "BitsToTest:%u,"
                  "BitsTested:0,"
                  "PercentDone:0.00,"
                  "RSSI:%d,"
                  "BitErrors:0,"
                  "PercentBitError:0.00,"
                  "Status:TestAbortedRxOverflow",
                  bytesTotal * 8,
                  rssi);
  } else {
    char bufPercentDone[10];
    char bufPercentBitError[10];

    sprintfFloat(bufPercentDone, sizeof(bufPercentDone), percentDone, 2);
    sprintfFloat(bufPercentBitError, sizeof(bufPercentBitError), percentBitError, 2);

    responsePrint(argv[0],
                  "BitsToTest:%u,"
                  "BitsTested:%u,"
                  "PercentDone:%s,"
                  "RSSI:%d,"
                  "BitErrors:%u,"
                  "PercentBitError:%s",
                  bytesTotal * 8,
                  bytesTested * 8,
                  bufPercentDone,
                  rssi,
                  bitErrors,
                  bufPercentBitError);
  }
}

void throughput(int argc, char **argv)
{
  uint32_t numberOfPackets = ciGetUnsigned(argv[1]);
  RAIL_Status_t txStatus = RAIL_STATUS_INVALID_STATE;
  uint32_t start = RAIL_GetTime();
  if (RAIL_WriteTxFifo(railHandle,
                       txData,
                       txDataLen,
                       true) != txDataLen) {
    responsePrint(argv[0], "WriteTxFifo Error");
    return;
  }
  for (uint32_t packets = 0; packets < numberOfPackets; packets++) {
    txStatus = RAIL_STATUS_INVALID_STATE;
    while (txStatus != RAIL_STATUS_NO_ERROR) {
      txStatus = RAIL_StartTx(railHandle, channel, RAIL_TX_OPTIONS_DEFAULT, NULL);
    }
    if (RAIL_WriteTxFifo(railHandle,
                         txData,
                         txDataLen,
                         true) != txDataLen) {
      responsePrint(argv[0], "WriteTxFifo Error");
      return;
    }
  }
  uint32_t stop = RAIL_GetTime();
  responsePrint(argv[0],
                "elapsedTime:%u", (stop - start));
}
