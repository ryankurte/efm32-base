/***************************************************************************//**
 * @file app_main.c
 * @brief This is the base test application. It handles basic RAIL configuration
 * as well as transmit, receive, and various debug modes.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rail.h"
#include "rail_ieee802154.h"
#include "rail_types.h"
#include "rail_chip_specific.h"
#include "rail_assert_error_codes.h"

#include "em_chip.h"
#include "em_core.h"
#include "em_rmu.h"

#include "retargetserial.h"
#include "gpiointerrupt.h"
#ifdef EMBER_AF_PLUGIN_LCD_GRAPHICS
#include "graphics.h"
#endif
#include "command_interpreter.h"
#include "response_print.h"
#include "buffer_pool_allocator.h"
#include "circular_queue.h"

#include "rail_config.h"

#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

#include "app_ci.h"
#include "app_common.h"
#include "app_trx.h"

// Includes for Silicon Labs-only, internal testing
#ifdef RPC_TESTING
#include "vs_rpc.h"
#else
#define RPC_Server_Init()
#define RPC_Server_Tick()
#endif

// Configuration defines
#ifndef APP_MAX_PACKET_LENGTH
#define APP_MAX_PACKET_LENGTH  (MAX_BUFFER_SIZE - sizeof(RAIL_RxPacketInfo_t))
#endif
#ifndef APP_CONTINUOUS_TRANSFER_PERIOD
#define APP_CONTINUOUS_TRANSFER_PERIOD 250UL
#endif
#ifndef APP_COMMAND_INTERFACE_BUFFER_SIZE
#define APP_COMMAND_INTERFACE_BUFFER_SIZE 256
#endif
#ifndef CALLBACK_QUEUE_SIZE
#define CALLBACK_QUEUE_SIZE 10
#endif

// External control and status variables
Counters_t counters = { 0 };
bool receiveModeEnabled = true;
uint8_t logLevel = PERIPHERAL_ENABLE | ASYNC_RESPONSE;
int32_t txCount = 0;
uint32_t continuousTransferPeriod = APP_CONTINUOUS_TRANSFER_PERIOD;
uint32_t txAfterRxDelay = 0;
int32_t txCancelDelay = -1;
int currentConfig = 0; //default is first in list
bool skipCalibrations = false;
bool afterRxCancelAck = false;
bool afterRxUseTxBufferForAck = false;
bool schRxStopOnRxEvent = false;
uint32_t rssiDoneCount = 0; // HW rssi averaging
float averageRssi = -128;

// Internal app state variables
static uint32_t startTransmitCounter = 0;
uint32_t internalTransmitCounter = 0;
uint32_t failPackets = 0;
static bool packetTx = false;
static bool finishTxSequence = false;
static RAIL_ScheduleTxConfig_t nextPacketTxTime = { 0, RAIL_TIME_ABSOLUTE, };
Queue_t  rxPacketQueue;
static uint32_t railTimerExpireTime = 0;
static uint32_t railTimerConfigExpireTime = 0;
static bool     railTimerExpired = false;
static bool     calibrateRadio = false;
bool newTxError = false;
static bool     rxAckTimeout = false;
static uint32_t ackTimeoutDuration = 0;
CallbackData_t callbackQueue[CALLBACK_QUEUE_SIZE];
uint8_t callbackQueueMarker = 0;
uint32_t enablePrintCallbacks = 0;
uint32_t callbacksMissed = 0;

uint32_t railRxConfig;

// Names of functions used for printing callbacks. The number
// of elements in this array should match the number of enum
// values in RailtestCallbacks_t
const char* RailCbNames[] = {
  "RxRadioStatusExt",
  "TxRadioStatus",
  "TxPacketSent",
  "RxPacketReceived",
  "TxFifoAlmostEmpty",
  "RxFifoAlmostFull",
  "RfReady",
  "CalNeeded",
  "RadioStateChanged",
  "TimerExpired",
  "RxAckTimeout",
  "IEEE802154_DataRequestCommand",
  "RssiAverageDone",
  "AssertFailed"
};

// Allow local echo to be turned on/off for the command prompt
#ifdef DISABLE_LOCAL_ECHO
  #define localEcho 0
#else
  #define localEcho 1
#endif

//Command line variables
static CommandEntry_t commands[] = {
  APP_CI_COMMANDS,
  COMMAND_ENTRY(NULL, NULL, NULL, NULL)
};
static CommandState_t state;
static char ciBuffer[APP_COMMAND_INTERFACE_BUFFER_SIZE];

// Channel Variables
uint8_t channel = 0;

// Generic
uint8_t txData[APP_MAX_PACKET_LENGTH] = {
  0x0F, 0x0E, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
  0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
};
RAIL_TxData_t transmitPayload = { &txData[0], 16 };

uint8_t ackData[RAIL_AUTOACK_MAX_LENGTH] = {
  0x0F, 0x0E, 0xF1, 0xE2, 0xD3, 0xC4, 0xB5, 0xA6,
  0x97, 0x88, 0x79, 0x6A, 0x5B, 0x4C, 0x3D, 0x2E,
};
RAIL_AutoAckData_t ackPayload = { &ackData[0], 16 };

// Helper buffer for printing received packet data
static char packetPrintBuffer[(APP_MAX_PACKET_LENGTH + 1) * 5];

static const RAIL_Init_t railInitParams = {
  APP_MAX_PACKET_LENGTH,
  RADIO_CONFIG_XTAL_FREQUENCY,
  RAIL_CAL_ALL
};

static const RAIL_CalInit_t railCalInitParams = {
  RAIL_CAL_ALL,
  irCalConfig
};

// Structure that holds txOptions
RAIL_TxOptions_t txOptions;

// If this pointer is not NULL, call RAIL_TxStartWithOptions
RAIL_TxOptions_t *txOptionsPtr = NULL;

// Data Management
RAIL_DataConfig_t railDataConfig = {
  .txSource = TX_PACKET_DATA,
  .rxSource = RX_PACKET_DATA,
  .txMethod = PACKET_MODE,
  .rxMethod = PACKET_MODE,
};

// Prototypes
void changeRadioConfig(int newConfig);
void changeChannelConfig(int newConfig);
// Called during main loop
void processInputCharacters(void);
void sendPacketIfPending(void);
void finishTxSequenceIfPending(void);
void changeAppModeIfPending(void);
void printReceivedPacket(void);
void printNewTxError(void);
void checkTimerExpiration(void);
void updateDisplay(void);
void processPendingCalibrations(void);
void printAckTimeout(void);
void printCallbacks(void);

int main(void)
{
  // Initialize the chip
  CHIP_Init();

  // Initialize the RPC Server
  RPC_Server_Init();

  // Grab the reset cause
  uint32_t resetCause = RMU_ResetCauseGet();
  RMU_ResetCauseClear(); // So resetCause is rational and not an accmulated mess
  // Release GPIOs that were held by EM4h
  // This is reportedly a workaround that I've found needs to be done
  // *before* appHalInit() tries to start oscillators, otherwise we'll
  // hang indefinitely waiting for the oscillator to become ready.
  if (resetCause & RMU_RSTCAUSE_EM4RST) {
    EMU->CMD = EMU_CMD_EM4UNLATCH;
  }

  // Initialize hardware for application
  appHalInit();

  // Initialize Radio
  RAIL_RfInit(&railInitParams);

  // Initialize Radio Calibrations
  RAIL_CalInit(&railCalInitParams);

  // Configure modem, packet handler
  changeRadioConfig(currentConfig);

  // Configure RAIL callbacks with no appended info
  railRxConfig = RAIL_RX_CONFIG_FRAME_ERROR
                 | RAIL_RX_CONFIG_SYNC1_DETECT
                 | RAIL_RX_CONFIG_SYNC2_DETECT
                 | RAIL_RX_CONFIG_ADDRESS_FILTERED
                 | RAIL_RX_CONFIG_BUFFER_OVERFLOW
                 | RAIL_RX_CONFIG_BUFFER_UNDERFLOW
                 | RAIL_RX_CONFIG_SCHEDULED_RX_END
                 | RAIL_RX_CONFIG_PACKET_ABORTED;
  RAIL_RxConfig(railRxConfig, true);
  RAIL_TxConfig(RAIL_TX_CONFIG_BUFFER_UNDERFLOW
                | RAIL_TX_CONFIG_BUFFER_OVERFLOW
                | RAIL_TX_CONFIG_TX_ABORTED
                | RAIL_TX_CONFIG_TX_BLOCKED
                | RAIL_TX_CONFIG_CHANNEL_BUSY
                | RAIL_TX_CONFIG_CHANNEL_CLEAR
                | RAIL_TX_CONFIG_CCA_RETRY
                | RAIL_TX_CONFIG_START_CCA);

  RAIL_SetRxTransitions(RAIL_RF_STATE_RX, RAIL_RF_STATE_RX,
                        RAIL_IGNORE_NO_ERRORS);
  RAIL_SetTxTransitions(RAIL_RF_STATE_RX, RAIL_RF_STATE_RX);
  // Initialize the queue we use for tracking packets
  if (!queueInit(&rxPacketQueue, MAX_QUEUE_LENGTH)) {
    while (1) ;
  }

  updateDisplay();

  printf("\n"APP_DEMO_STRING_INIT "\n");
  if (resetCause & RMU_RSTCAUSE_EM4RST) {
    responsePrint("sleepWoke", "EM:4%c,SerialWakeup:No,RfSensed:%s",
                  (((EMU->EM4CTRL & EMU_EM4CTRL_EM4STATE)
                    == EMU_EM4CTRL_EM4STATE_EM4S) ? 's' : 'h'),
                  RAIL_RfSensed() ? "Yes" : "No");
  }
  printf("> ");
  ciInitState(&state, ciBuffer, sizeof(ciBuffer), commands);

  // Initialize autoack data
  RAIL_AutoAckLoadBuffer(&ackPayload);

  RAIL_RxStart(channel); // Start in receive mode
  while (1) {
    RPC_Server_Tick();

    processInputCharacters();

    rfSensedCheck();

    sendPacketIfPending();

    finishTxSequenceIfPending();

    changeAppModeIfPending();

    printReceivedPacket();

    printNewTxError();

    printCallbacks();

    checkTimerExpiration();

    updateDisplay();

    processPendingCalibrations();

    printAckTimeout();
  }
} //main()

/******************************************************************************
 * RAIL Callback Implementation
 *****************************************************************************/
void RAILCb_RfReady(void)
{
  enqueueCallback(RAILCB_RF_READY);
  LedSet(0);
  LedSet(1);
}

void RAILCb_CalNeeded()
{
  enqueueCallback(RAILCB_CAL_NEEDED);
  calibrateRadio = true;
}

void RAILCb_RadioStateChanged(uint8_t state)
{
  enqueueCallback(RAILCB_RADIO_STATE_CHANGED);
}

void RAILCb_TimerExpired(void)
{
  enqueueCallback(RAILCB_TIMER_EXPIRED);
  if (inAppMode(NONE, NULL)) {
    if (abortRxDelay != 0) {
      RAIL_RfIdleExt(RAIL_IDLE_ABORT, true);
    } else {
      railTimerExpireTime = RAIL_GetTime();
      railTimerConfigExpireTime = RAIL_TimerGet();
      railTimerExpired = true;
    }
  } else if (currentAppMode() == PER) {
    GPIO_PinOutToggle(PER_PORT, PER_PIN);
    counters.perTriggers += GPIO_PinOutGet(PER_PORT, PER_PIN);
    perCount -= GPIO_PinOutGet(PER_PORT, PER_PIN);
    if (perCount < 1) {
      GPIO_PinOutClear(PER_PORT, PER_PIN);
      enableAppMode(PER, false, NULL);
    } else {
      RAIL_TimerSet(perDelay, RAIL_TIME_DELAY);
    }
  } else {
    pendPacketTx();
  }
}

void RAILCb_RxAckTimeout(void)
{
  enqueueCallback(RAILCB_RX_ACK_TIMEOUT);
  counters.ackTimeout++;
  rxAckTimeout = true;
  ackTimeoutDuration = RAIL_GetTime() - previousTxTime;
}

void RAILCb_IEEE802154_DataRequestCommand(RAIL_IEEE802154_Address_t *data)
{
  enqueueCallback(RAILCB_IEEE802154_DATA_REQUEST_COMMAND);
  // Placeholder validation for when a data request should have the frame
  // pending bit set in the ACK.
  if (data->length == RAIL_IEEE802154_LongAddress) {
    if (data->longAddress[0] == 0xAA) {
      RAIL_IEEE802154_SetFramePending();
    }
  } else {
    if ((data->shortAddress & 0xFF) == 0xAA) {
      RAIL_IEEE802154_SetFramePending();
    }
  }
}

void RAILCb_RssiAverageDone(int16_t avgRssi)
{
  enqueueCallback(RAILCB_RSSI_AVERAGE_DONE);
  rssiDoneCount++;
  averageRssi = (float)avgRssi / 4;
  if (avgRssi == RAIL_RSSI_INVALID) {
    responsePrint("getAvgRssi", "Could not read RSSI.");
    return;
  }
  responsePrint("getAvgRssi", "rssi:%.2f", averageRssi);
}

void RAILCb_AssertFailed(uint32_t errorCode)
{
  enqueueCallback(RAILCB_ASSERT_FAILED);
  static const char* railErrorMessages[] = RAIL_ASSERT_ERROR_MESSAGES;
  const char *errorMessage = "Unknown";

  // If this error code is within the range of known error messages then use
  // the appropriate error message.
  if (errorCode < (sizeof(railErrorMessages) / sizeof(char*))) {
    errorMessage = railErrorMessages[errorCode];
  }
  // Print a message about the assert that triggered
  responsePrint("assert",
                "code:%d,message:%s",
                errorCode,
                errorMessage);
  // Reset the chip since an assert is a fatal error
  NVIC_SystemReset();
}

/******************************************************************************
 * Application Helper Functions
 *****************************************************************************/
void processPendingCalibrations()
{
  // Only calibrate the radio when not currently transmitting or in a
  // transmit mode. Also don't try to calibrate while receiving a packet
  bool calsInMode = inAppMode(NONE, NULL);
  if (calibrateRadio && calsInMode && !skipCalibrations && !receivingPacket) {
    RAIL_CalMask_t pendingCals = RAIL_CalPendingGet();

    // Disable the radio if we have to do an offline calibration
    if (pendingCals & RAIL_CAL_OFFLINE) {
      RAIL_RfIdle();
    }

    // Perform the necessary calibrations and don't save the results
    counters.calibrations++;
    calibrateRadio = false;
    RAIL_CalStart(NULL, pendingCals, false);

    // Turn the radio back on if we disabled it above
    if ((pendingCals & RAIL_CAL_OFFLINE)) {
      // Wait for RxStart to succeed
      while (receiveModeEnabled && RAIL_RxStart(channel)) {
        RAIL_RfIdle();
      }
    }
  }
}

void checkTimerExpiration()
{
  if (railTimerExpired) {
    railTimerExpired = false;
    responsePrint("timerCb",
                  "TimerExpiredCallback:%u,ConfiguredExpireTime:%u",
                  railTimerExpireTime,
                  railTimerConfigExpireTime);
  }
}

void printNewTxError()
{
  if (newTxError) {
    newTxError = false;
    if (lastTxStatus & RAIL_TX_CONFIG_BUFFER_UNDERFLOW) {
      if (logLevel & ASYNC_RESPONSE) {
        responsePrint("txPacket",
                      "txStatus:Error,"
                      "errorReason:Tx underflow or abort,"
                      "errorCode:%u",
                      lastTxStatus);
      }
      counters.txAbort++;
    }
    if (lastTxStatus & RAIL_TX_CONFIG_BUFFER_OVERFLOW) {
      if (logLevel & ASYNC_RESPONSE) {
        responsePrint("txPacket",
                      "txStatus:Error,"
                      "errorReason:Tx overflow or abort,"
                      "errorCode:%u",
                      lastTxStatus);
      }
      counters.txAbort++;
    }
    if (lastTxStatus & RAIL_TX_CONFIG_CHANNEL_BUSY) {
      if (logLevel & ASYNC_RESPONSE) {
        responsePrint("txPacket",
                      "txStatus:Error,"
                      "errorReason:Tx channel busy,"
                      "errorCode:%u",
                      lastTxStatus);
      }
      counters.txChannelBusy++;
    }
  }
}

void printAckTimeout()
{
  if (rxAckTimeout) {
    rxAckTimeout = false;
    responsePrint("rxAckTimeout",
                  "ackTimeoutDuration:%d",
                  ackTimeoutDuration);
  }
}

void changeChannel(uint32_t i)
{
  channel = i;
  redrawDisplay = true;
}

void changeRadioConfig(int newConfig)
{
  // Turn off the radio before reconfiguring it
  RAIL_RfIdle();

  // Reconfigure the radio parameters
  if (RAIL_RadioConfig((void*)configList[newConfig])) {
    while (1) ;
  }
  RAIL_PacketLengthConfigFrameType(frameTypeConfigList[newConfig]);

  // Set us to a valid channel for this config and force an update in the main
  // loop to restart whatever action was going on
  changeChannelConfig(newConfig);
  currentConfig = newConfig;
}

void changeChannelConfig(int newConfig)
{
  channel = RAIL_ChannelConfig(channelConfigs[newConfig]);
  changeChannel(channel);
}

void pendPacketTx()
{
  packetTx = true;
}

void sendPacketIfPending()
{
  if (packetTx) {
    packetTx = false;
    uint8_t txStatus;
    uint32_t storedTransmitCounter = internalTransmitCounter;

    // Don't decrement in continuous mode
    if (currentAppMode() != TX_CONTINUOUS) {
      txCount--;
    }

    // Generate the payload and start transmitting
    if (currentAppMode() != TX_UNDERFLOW) { // Force underflows in this mode
      // Load packet data before transmitting if manual loading is not enabled
      if (!txFifoManual) {
        loadTxData(&transmitPayload);
      }
    }
    if (currentAppMode() == TX_SCHEDULED || currentAppMode() == SCHTX_AFTER_RX) {
      if (txOptionsPtr == NULL) {
        txStatus = RAIL_TxStart(channel, &RAIL_ScheduleTx,
                                &nextPacketTxTime);
      } else {
        txStatus = RAIL_TxStartWithOptions(channel, txOptionsPtr,
                                           &RAIL_ScheduleTx,
                                           &nextPacketTxTime);
      }
    } else if ((startTransmitCounter == storedTransmitCounter)
               && (failPackets == 0)) {
      if (txOptionsPtr == NULL) {
        txStatus = RAIL_TxStart(channel, txPreTxOp, txPreTxOpArgs);
      } else {
        txStatus = RAIL_TxStartWithOptions(channel, txOptionsPtr,
                                           txPreTxOp, txPreTxOpArgs);
      }
    } else {
      // Sending NULL for txPreTxOpArgs reuses previous arguments, which
      // saves computation. Reuse arguments for all but the first
      // packet in a sequence
      if (txOptionsPtr == NULL) {
        txStatus = RAIL_TxStart(channel, txPreTxOp, NULL);
      } else {
        txStatus = RAIL_TxStartWithOptions(channel, txOptionsPtr,
                                           txPreTxOp, NULL);
      }
    }

    if (txStatus != 0) {
      lastTxStatus = txStatus;
      failPackets++;
      scheduleNextTx(); // No callback will fire, so fake it
    } else if (currentAppMode() == TX_CANCEL) {
      usDelay(txCancelDelay);
      RAIL_RfIdle();
    }
  }
}

void pendFinishTxSequence()
{
  finishTxSequence = true;
}

void finishTxSequenceIfPending()
{
  if (finishTxSequence) {
    finishTxSequence = false;

    if (logLevel & ASYNC_RESPONSE) {
      // Compute the number of packets sent
      uint32_t sentPackets = internalTransmitCounter - startTransmitCounter;
      // Print the number of sent and failed packets
      responsePrint("txEnd",
                    "txStatus:%s,"
                    "transmitted:%u,"
                    "lastTxTime:%u,"
                    "failed:%u,"
                    "lastTxStatus:0x%x",
                    failPackets == 0
                    ? "Complete"
                    : (sentPackets == 0 ? "Error" : "Partial"),
                    sentPackets,
                    previousTxTime,
                    failPackets,
                    lastTxStatus);
    }
    startTransmitCounter = internalTransmitCounter;
    failPackets = 0;
    lastTxStatus = 0;
  }
}

void setNextPacketTime(uint32_t time, bool isAbs)
{
  nextPacketTxTime.when = time;
  if (isAbs) {
    nextPacketTxTime.mode = RAIL_TIME_ABSOLUTE;
  } else {
    nextPacketTxTime.mode = RAIL_TIME_DELAY;
  }
}
//
void printReceivedPacket()
{
  // Print any newly received packets
  if (!queueIsEmpty(&rxPacketQueue)) {
    void *rxPacketHandle = queueRemove(&rxPacketQueue);
    RAIL_RxPacketInfo_t *rxPacketInfo =
      (RAIL_RxPacketInfo_t*) memoryPtrFromHandle(rxPacketHandle);

    // Print the received packet and appended info
    printPacket("rxPacket",
                rxPacketInfo->dataPtr,
                rxPacketInfo->dataLength,
                rxPacketInfo);

    // Free the memory allocated for this packet since we're now done with it
    memoryFree(rxPacketHandle);
  }
}

void printPacket(char *cmdName,
                 uint8_t *data,
                 uint16_t dataLength,
                 RAIL_RxPacketInfo_t *packetInfo)
{
  uint32_t offset = 0;
  int i;

  // Print out a length 0 packet message if no packet was given
  if (data == NULL) {
    responsePrint(cmdName, "len:0");
    return;
  }

  for (i = 0; i < dataLength; i++) {
    int n = snprintf(packetPrintBuffer + offset,
                     sizeof(packetPrintBuffer) - offset,
                     " 0x%.2x",
                     data[i]);
    if (n >= 0) {
      offset += n;
    } else {
      snprintf(packetPrintBuffer, sizeof(packetPrintBuffer), "Invalid Packet");
      break;
    }

    // If we've filled up the packet buffer make sure we stop trying to print
    if (offset >= sizeof(packetPrintBuffer)) {
      snprintf(packetPrintBuffer,
               sizeof(packetPrintBuffer),
               "Packet too large!");
      break;
    }
  }

  // If this is an Rx packet print the appended info
  if (packetInfo != NULL) {
    responsePrint(cmdName,
                  "len:%d,timeUs:%u,crc:%s,coding:%s,rssi:%d,lqi:%d,phy:%d,isAck:%s,syncWordId:%d,payload:%s",
                  packetInfo->dataLength,
                  packetInfo->appendedInfo.timeUs,
                  (packetInfo->appendedInfo.crcStatus) ? "Pass" : "Fail",
                  (packetInfo->appendedInfo.frameCodingStatus) ? "Pass" : "Fail",
                  packetInfo->appendedInfo.rssiLatch,
                  packetInfo->appendedInfo.lqi,
                  packetInfo->appendedInfo.subPhy,
                  packetInfo->appendedInfo.isAck ? "True" : "False",
                  packetInfo->appendedInfo.syncWordId,
                  packetPrintBuffer);
  } else {
    responsePrint(cmdName, "len:%d,payload:%s", dataLength, packetPrintBuffer);
  }
}

void processInputCharacters()
{
  char input = getchar();
  while (input != '\0' && input != 0xFF) {
    if (localEcho) {
      if (input != '\n') {
        printf("%c", input);
      }
    }
    if (ciProcessInput(&state, &input, 1) > 0) {
      printf("> ");
    }
    input = getchar();
  }
}

void enqueueCallback(uint32_t callbackId)
{
  if (enablePrintCallbacks & (1 << callbackId)) {
    // Disable callbacks to avoid the risk of trying to enqueueing
    // two callbacks at once.
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    if (callbackQueueMarker < CALLBACK_QUEUE_SIZE) {
      callbackQueue[callbackQueueMarker].timestamp = RAIL_GetTime();
      callbackQueue[callbackQueueMarker].callbackId = callbackId;
      callbackQueueMarker++;
    } else {
      callbacksMissed++;
    }
    CORE_EXIT_CRITICAL();
  }
}

// Allows for a consistent way of printing callbacks in the order
// they were called, regardless of when they occur in the main
// loop.
void printCallbacks(void)
{
  CallbackData_t cache_callbackQueue[CALLBACK_QUEUE_SIZE];
  uint8_t cache_callbackQueueMarker = 0;
  uint32_t cache_callbacksMissed = 0;

  if (callbackQueueMarker) {
    // We don't want more callbacks added as we print
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();

    // cache the callbackQueue
    memcpy(cache_callbackQueue, callbackQueue, sizeof(callbackQueue));

    // cache the callbackQueueMarker
    cache_callbackQueueMarker = callbackQueueMarker;

    // resetting the callbackQueueMarker - don't need to clear
    // the actual queue, resetting the marker will just let us overwrite it
    callbackQueueMarker = 0;

    cache_callbacksMissed = callbacksMissed;
    callbacksMissed = 0;

    CORE_EXIT_CRITICAL();

    for (int position = 0; position < cache_callbackQueueMarker; position++) {
      responsePrint("callback",
                    "timestamp:%d,callbackName:%s%s",
                    cache_callbackQueue[position].timestamp,
                    "RAILCb_",
                    RailCbNames[cache_callbackQueue[position].callbackId]);
    }
    if (cache_callbacksMissed) {
      responsePrintError("printCallbacksMissed",
                         0x36,
                         "Callback queue limited to %d callbacks. %d callbacks not enqueued.",
                         CALLBACK_QUEUE_SIZE,
                         cache_callbacksMissed);
    }
  }
}

/******************************************************************************
 * Command Interpreter Override Functions
 *****************************************************************************/

// Error callback function using responsePrint
void ciErrorCallback(char* command, CommandError_t error)
{
  if (error == CI_UNKNOWN_COMMAND) {
    responsePrintError("ci",
                       CI_UNKNOWN_COMMAND,
                       "Unknown command '%s'",
                       command);
  } else if (error == CI_MAX_ARGUMENTS) {
    responsePrintError("ci",
                       CI_MAX_ARGUMENTS,
                       "CI_MAX_ARGUMENTS '%s'",
                       command);
  } else if (error == CI_INVALID_ARGUMENTS) {
    responsePrintError("ci",
                       CI_INVALID_ARGUMENTS,
                       "Invalid arguments for '%s'",
                       command);
  }
}

#ifdef USE_RESPONSE_HELP
// Help callback function using responsePrint
bool ciPrintHelp(CommandEntry_t *commands)
{
  responsePrintHeader("help", "command:%s,args:%s,help:%s");
  while (commands != NULL && commands->command != NULL) {
    char *args = commands->arguments;
    char *helpStr = commands->helpStr;

    if (commands->callback != NULL) {
      // Make sure that we print something for NULL pointers
      if (args == NULL) {
        args = "";
      }
      if (helpStr == NULL) {
        helpStr = "";
      }

      // Print the multiline response for this entry
      responsePrintMulti("command:%s,args:%s,help:%s",
                         commands->command,
                         args,
                         helpStr);
    }
    commands++;
  }
  return true;
}
#endif //USE_RESPONSE_HELP
