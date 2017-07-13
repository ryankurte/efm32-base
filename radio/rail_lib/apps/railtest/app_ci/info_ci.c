/***************************************************************************//**
 * @file info_ci.c
 * @brief This file implements informational commands for RAIL test applications.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include <string.h>
#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "rail_types.h"
#include "app_common.h"

const char *rfStates[] = { "Idle", "Rx", "Tx" };

void getStatus(int argc, char **argv)
{
  responsePrintStart(argv[0]);
  responsePrintContinue("TxCount:%u,"
                        "RxCount:%u,"
                        "SyncDetect:%u,"
                        "FrameErrors:%u,"
                        "RxOverflow:%u,"
                        "AddrFilt:%u,"
                        "Aborted:%u,"
                        "Calibrations:%u,"
                        "TxAbort:%u,"
                        "TxChannelBusy:%u,"
                        "TxClear:%u,"
                        "TxCca:%u,"
                        "TxRetry:%u",
                        counters.transmit,
                        counters.receive,
                        counters.syncDetect,
                        counters.frameError,
                        counters.rxOfEvent,
                        counters.addrFilterEvent,
                        counters.rxFail,
                        counters.calibrations,
                        counters.txAbort,
                        counters.txChannelBusy,
                        counters.lbtSuccess,
                        counters.lbtStartCca,
                        counters.lbtRetry
                        );
  responsePrintEnd("NoRxBuffer:%u,"
                   "RfSensed:%u,"
                   "ackTimeout:%u,"
                   "Channel:%u,"
                   "AppMode:%s,"
                   "RfState:%s,"
                   "TimingLost:%u,"
                   "TimingDetect:%u",
                   counters.noRxBuffer,
                   counters.rfSensedEvent,
                   counters.ackTimeout,
                   channel,
                   appModeNames(currentAppMode()),
                   rfStates[RAIL_RfStateGet()],
                   counters.timingLost,
                   counters.timingDetect
                   );
}

void fifoStatus(int argc, char**argv)
{
  uint16_t spaceCount =  RAIL_GetTxFifoSpaceAvailable();
  uint16_t byteCount = RAIL_GetRxFifoBytesAvailable();
  responsePrint(argv[0],
                "TxSpaceCount:%u,"
                "RxByteCount:%u,"
                "TxFifoThreshold:%d,"
                "RxFifoThreshold:%d,"
                "TxFifoAlmostEmpty:%u,"
                "RxFifoAlmostFull:%u,"
                "RxOverflow:%u,"
                "TxOverflow:%u,"
                "RxUnderflow:%u,"
                "TxUnderflow:%u",
                spaceCount,
                byteCount,
                RAIL_GetTxFifoThreshold(),
                RAIL_GetRxFifoThreshold(),
                counters.txFifoAlmostEmpty,
                counters.rxFifoAlmostFull,
                counters.rxOfEvent,
                counters.txOfEvent,
                counters.rxUfEvent,
                counters.txUfEvent
                );
}

void getVersion(int argc, char **argv)
{
  RAIL_Version_t rail_ver;
  RAIL_VersionGet(&rail_ver, false);
  responsePrint(argv[0], "App:%d.%d.%d,RAIL:%d.%d.%d",
                rail_ver.major, rail_ver.minor, rail_ver.rev,
                rail_ver.major, rail_ver.minor, rail_ver.rev);
}

void setPtiProtocol(int argc, char **argv)
{
  RAIL_Status_t status = RAIL_SetPtiProtocol((RAIL_PtiProtocol_t) ciGetUnsigned(argv[1]));
  responsePrint(argv[0], "Pti:%s", status ? "Error" : "Set");
}

void getVersionVerbose(int argc, char **argv)
{
  RAIL_Version_t rail_ver;
  RAIL_VersionGet(&rail_ver, true);
  responsePrint(argv[0], "App:%d.%d.%d,RAIL:%d.%d.%d.%d",
                rail_ver.major, rail_ver.minor, rail_ver.rev,
                rail_ver.major, rail_ver.minor, rail_ver.rev, rail_ver.build);
  responsePrint(argv[0], "hash:0x%.8X,flags:0x%.2X",
                rail_ver.hash, rail_ver.flags);
}

void getRssi(int argc, char **argv)
{
  uint32_t averageTimeUs = 0;
  if (argc == 2) {
    averageTimeUs = ciGetUnsigned(argv[1]);
  } else {
    averageTimeUs = 0;
  }
  int16_t rssi = RAIL_PollAverageRSSI(averageTimeUs);

  // The lowest negative value is used to indicate an error reading the RSSI
  if (rssi == RAIL_RSSI_INVALID) {
    responsePrintError(argv[0], 0x08, "Could not read RSSI. Ensure Rx is enabled");
    return;
  }
  responsePrint(argv[0], "rssi:%.2f", ((float) rssi / 4));
}
void startAvgRssi(int argc, char **argv)
{
  uint32_t averageTimeUs = ciGetUnsigned(argv[1]);
  uint8_t avgChannel = channel;
  if (argc == 3) {
    avgChannel = ciGetUnsigned(argv[2]);
  }
  if (!inRadioState(RAIL_RF_STATE_IDLE, NULL)) {
    responsePrintError(argv[0], 0x08, "Could not read RSSI. Ensure RX is disabled.");
    return;
  } else if (RAIL_StartAverageRSSI(avgChannel, averageTimeUs) != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x08, "Could not read RSSI.");
    return;
  }
}
void getAvgRssi(int argc, char **argv)
{
  int16_t rssi = RAIL_GetAverageRSSI();
  if (rssi == RAIL_RSSI_INVALID) {
    responsePrintError(argv[0], 0x08, "Invalid RSSI. Make sure startAvgRssi ran successfully.");
    return;
  }
  responsePrint(argv[0], "rssi:%.2f", ((float) rssi / 4));
}
void sweepPower(int argc, char **argv)
{
  int32_t lowPower = ciGetUnsigned(argv[1]);
  int32_t hiPower = ciGetUnsigned(argv[2]);
  int32_t period = ciGetUnsigned(argv[3]);
  int32_t halfPeriodStepUs = period / 2;
  uint32_t expired = RAIL_GetTime() + (uint32_t)5000000;
  while (expired > RAIL_GetTime()) {
    RAIL_TxToneStop();
    RAIL_RfIdle();
    RAIL_TxPowerSet(lowPower);
    RAIL_TxToneStart(channel);
    usDelay(halfPeriodStepUs);
    RAIL_TxToneStop();
    RAIL_RfIdle();
    RAIL_TxPowerSet(hiPower);
    RAIL_TxToneStart(channel);
    usDelay(halfPeriodStepUs);
  }
  RAIL_TxToneStop();
}

void isRssiRdy(int argc, char **argv)
{
  if (RAIL_AverageRSSIReady()) {
    responsePrint(argv[0], "isReady:True");
  } else {
    responsePrint(argv[0], "isReady:False");
  }
  return;
}

void resetCounters(int argc, char **argv)
{
  memset(&counters, 0, sizeof(counters));
  getStatus(1, argv);
}

void getTime(int argc, char **argv)
{
  responsePrint(argv[0], "Time:%u", RAIL_GetTime());
}

void setTime(int argc, char **argv)
{
  uint32_t timeUs = ciGetUnsigned(argv[1]);
  if (RAIL_STATUS_NO_ERROR == RAIL_SetTime(timeUs)) {
    responsePrint(argv[0], "Status:Success,CurrentTime:%u", RAIL_GetTime());
  } else {
    responsePrint(argv[0], "Status:Error,CurrentTime:%u", RAIL_GetTime());
  }
}
