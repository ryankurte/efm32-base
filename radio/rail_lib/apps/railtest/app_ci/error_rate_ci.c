/***************************************************************************//**
 * @file error_rate_ci.c
 * @brief This file provides functionality to test RAIL error rates.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "app_common.h"
#include "app_ci.h"
#include "app_trx.h"
#include "em_core.h"
#include "pti.h"

uint32_t perCount;
uint32_t perDelay;
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
    RAIL_TimerSet(perDelay, RAIL_TIME_DELAY);
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
  responsePrint(argv[0],
                "PerTriggers:%u,"
                "RssiMean:%f,"
                "RssiMin:%.2f,"
                "RssiMax:%.2f,"
                "RssiVariance:%f",
                counters.perTriggers,
                counters.rssi.mean / 4,
                ((float) counters.rssi.min) / 4,
                ((float) counters.rssi.max) / 4,
                variance(counters.rssi) / 16);
}

void berConfigSet(int argc, char **argv)
{
  RAIL_Status_t status;
  uint16_t rxThreshold, packetLength;

  RAIL_RfIdleExt(RAIL_IDLE_ABORT, true);
  RAIL_ResetFifo(true, true);

  // configure radio
  railDataConfig.txSource = TX_PACKET_DATA;
  // railDataConfig.rxSource = RX_BER_STREAM;
  railDataConfig.rxSource = RX_PACKET_DATA;
  railDataConfig.txMethod = FIFO_MODE;
  railDataConfig.rxMethod = FIFO_MODE;
  status = RAIL_DataConfig(&railDataConfig);
  if (status) {
    responsePrintError(argv[0], status, "Error calling RAIL_DataConfig().");
  }

  // configure RX FIFO
  rxThreshold = 100;
  rxThreshold = RAIL_SetRxFifoThreshold(rxThreshold);

  // specify overall packet length info (infinite)
  packetLength = 0;
  packetLength = RAIL_SetFixedLength(packetLength);

  RADIO_PTI_Disable();

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

  RAIL_RfIdleExt(RAIL_IDLE_ABORT, true);
  RAIL_ResetFifo(true, true);
  if (enable) {
    RADIO_PTI_Disable();
    berResetStats(berBytesToTest);
    berTestModeEnabled = true;
    RAIL_RxStart(channel);
  }
}

void berStatusGet(int argc, char **argv)
{
  RAIL_BerStatus_t berStats;
  float percentDone;
  float percentBitError;
  uint32_t bitsTotal; /**< Number of bits to receive */
  uint32_t bitsTested; /**< Number of bits currently tested */
  uint32_t bitErrors; /**< Number of bits errors detected */
  int8_t   rssi; /**< Latched RSSI value at pattern detect */
  CORE_DECLARE_IRQ_STATE;

  // be sure we don't get half new, half stale data
  CORE_ENTER_CRITICAL();
  berGetStats(&berStats);
  bitsTotal = berStats.bitsTotal;
  bitsTested = berStats.bitsTested;
  bitErrors = berStats.bitErrors;
  rssi = berStats.rssi;
  CORE_EXIT_CRITICAL();

  // don't divide by 0
  if (0 != bitsTotal) {
    percentDone = (float)((((double)bitsTested) / bitsTotal) * 100);
  } else {
    percentDone = 0.0;
  }
  // don't divide by 0
  if (0 != bitsTested) {
    percentBitError = (float)((((double)bitErrors) / bitsTested) * 100);
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
                  bitsTotal,
                  rssi);
  } else {
    responsePrint(argv[0],
                  "BitsToTest:%u,"
                  "BitsTested:%u,"
                  "PercentDone:%0.2f,"
                  "RSSI:%d,"
                  "BitErrors:%u,"
                  "PercentBitError:%0.2f",
                  bitsTotal,
                  bitsTested,
                  percentDone,
                  rssi,
                  bitErrors,
                  percentBitError);
  }
}

void throughput(int argc, char **argv)
{
  uint32_t numberOfPackets = ciGetUnsigned(argv[1]);
  uint8_t txStatus = RAIL_STATUS_INVALID_STATE;
  uint32_t start = RAIL_GetTime();
  if (RAIL_TxDataLoad(&transmitPayload) != RAIL_STATUS_NO_ERROR) {
    responsePrint(argv[0], "TxDataLoad Error");
    return;
  }
  for (uint32_t packets = 0; packets < numberOfPackets; packets++) {
    txStatus = RAIL_STATUS_INVALID_STATE;
    while (txStatus != RAIL_STATUS_NO_ERROR) {
      txStatus = RAIL_TxStart(channel, NULL, NULL);
    }
    if (RAIL_TxDataLoad(&transmitPayload) != RAIL_STATUS_NO_ERROR) {
      responsePrint(argv[0], "TxDataLoad Error");
      return;
    }
  }
  uint32_t stop = RAIL_GetTime();
  responsePrint(argv[0],
                "elapsedTime:%u", (stop - start));
}
