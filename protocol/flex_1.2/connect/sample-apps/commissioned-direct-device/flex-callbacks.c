// Commissioned Direct Device sample application.
//
// Copyright 2015 Silicon Laboratories, Inc.                                *80*

#if !defined(UNIX_HOST) && !defined(CORTEXM3_EMBER_MICRO)
#include "em_device.h"
#include "em_chip.h"
#include "bsp.h"
#endif

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER

#include "stack/include/ember.h"
#ifndef UNIX_HOST
#include "heartbeat/heartbeat.h"
#endif
#include "hal/hal.h"
#include "command-interpreter/command-interpreter2.h"
#include "debug-print/debug-print.h"

#if (defined EMBER_AF_PLUGIN_FREE_RTOS)
#include "FreeRTOS.h"
#include "task.h"
#include EMBER_AF_API_FREE_RTOS
extern long stack_blocked_deep_sleep;
extern long stack_allowed_deep_sleep;
#endif

#define NETWORK_UP_LED                       BOARDLED0

#define DEFAULT_SECURITY_KEY    { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, \
                                  0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, \
                                  0xAA, 0xAA, 0xAA, 0xAA }

static EmberMessageOptions txOptions = EMBER_OPTIONS_ACK_REQUESTED;
static EmberKeyData securityKey = DEFAULT_SECURITY_KEY;

#define DATA_ENDPOINT           1
#define TX_TEST_ENDPOINT        2

static uint8_t txTestPacketSize;
static uint8_t txTestInFlightPackets, txTestCurrentInflightPackets = 0;
static EmberNodeId txTestDestination;
static bool txTestPrintEnabled = false;

//------------------------------------------------------------------------------
// Forward declarations

static EmberStatus sendTxTestPacket(void);

//------------------------------------------------------------------------------
// Callbacks

// This callback is called when the application starts and can be used to
// perform any additional initialization required at system startup.
void emberAfMainInitCallback(void)
{
  emberAfCorePrintln("Powered UP");
  emberAfCorePrintln("\n%p>", EMBER_AF_DEVICE_NAME);

  emberNetworkInit();
}

// This callback is called in each iteration of the main application loop and
// can be used to perform periodic functions.
void emberAfMainTickCallback(void)
{
  #ifndef UNIX_HOST
  if (emberStackIsUp()) {
    halSetLed(NETWORK_UP_LED);
  } else {
    halClearLed(NETWORK_UP_LED);
  }
  #endif
}

void emberAfIncomingMessageCallback(EmberIncomingMessage *message)
{
  if (message->endpoint == DATA_ENDPOINT) {
    uint8_t i;
    emberAfCorePrint("RX: Data from 0x%2x:{", message->source);
    for ( i = 0; i < message->length; i++ ) {
      emberAfCorePrint(" %x", message->payload[i]);
    }
    emberAfCorePrintln("}");
  } else if (message->endpoint == TX_TEST_ENDPOINT
             && txTestPrintEnabled) {
    uint8_t i;
    emberAfCorePrint("RX: TX test packet from 0x%2x:{", message->source);
    for ( i = 0; i < message->length; i++ ) {
      emberAfCorePrint(" %x", message->payload[i]);
    }
    emberAfCorePrintln("}");
  }
}

void emberAfMessageSentCallback(EmberStatus status,
                                EmberOutgoingMessage *message)
{
  if (message->endpoint == DATA_ENDPOINT) {
    if ( status != EMBER_SUCCESS ) {
      emberAfCorePrintln("TX: 0x%x", status);
    }
  } else if (message->endpoint == TX_TEST_ENDPOINT) {
    txTestCurrentInflightPackets--;
    if (txTestCurrentInflightPackets < txTestInFlightPackets) {
      if (sendTxTestPacket() == EMBER_SUCCESS) {
        txTestCurrentInflightPackets++;
      }
    }
  }
}

void emberAfStackStatusCallback(EmberStatus status)
{
  switch ( status ) {
    case EMBER_NETWORK_UP:
      emberAfCorePrintln("Network up");
      break;
    case EMBER_NETWORK_DOWN:
      emberAfCorePrintln("Network down");
      break;
    default:
      emberAfCorePrintln("Stack status: 0x%x", status);
      break;
  }
}

void emberFrequencyHoppingStartClientCompleteHandler(EmberStatus status)
{
  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("FH Client sync failed, status=0x%x", status);
  } else {
    emberAfCorePrintln("FH Client Sync Success");
  }
}

void emberAfEnergyScanCompleteCallback(int8_t mean,
                                       int8_t min,
                                       int8_t max,
                                       uint16_t variance)
{
  emberAfCorePrintln("Energy scan complete, mean=%d min=%d max=%d var=%d",
                     mean, min, max, variance);
}

//------------------------------------------------------------------------------
// Static functions

static EmberStatus sendTxTestPacket(void)
{
  uint8_t i;
  uint8_t txTestPacket[127];

  for (i = 0; i < txTestPacketSize; i++) {
    txTestPacket[i] = i;
  }

  return emberMessageSend(txTestDestination,
                          TX_TEST_ENDPOINT,
                          0, // messageTag
                          txTestPacketSize,
                          txTestPacket,
                          txOptions);
}

//------------------------------------------------------------------------------
// CLI commands

// When we start as a master, we form a network and enable permit join.
void commissionCommand(void)
{
  EmberStatus status;
  EmberNodeId nodeId;
  EmberNetworkParameters params;

  // Initialize the security key to the default key prior to commissioning the
  // node.
  emberSetSecurityKey(&securityKey);

  nodeId = emberUnsignedCommandArgument(0);
  params.panId = emberUnsignedCommandArgument(1);
  params.radioChannel = emberUnsignedCommandArgument(2);
  params.radioTxPower = emberSignedCommandArgument(3);

  status = emberJoinCommissioned(EMBER_DIRECT_DEVICE,
                                 nodeId,
                                 &params);

  if ( status != EMBER_SUCCESS ) {
    emberAfCorePrintln("Commissioning failed, 0x%x", status);
  }
}

void setTxOptionsCommand(void)
{
  txOptions = emberUnsignedCommandArgument(0);
  emberAfCorePrintln("TX options set: MAC acks %s, security %s, priority %s",
                     ((txOptions & EMBER_OPTIONS_ACK_REQUESTED) ? "enabled" : "disabled"),
                     ((txOptions & EMBER_OPTIONS_SECURITY_ENABLED) ? "enabled" : "disabled"),
                     ((txOptions & EMBER_OPTIONS_HIGH_PRIORITY) ? "enabled" : "disabled"));
}

void setSecurityKeyCommand(void)
{
  EmberKeyData key;
  emberCopyKeyArgument(0, &key);

  if ( emberSetSecurityKey(&key) == EMBER_SUCCESS ) {
    emberAfCorePrint("Security key set { ");
    emberAfCorePrintBuffer(key.contents, EMBER_ENCRYPTION_KEY_SIZE, TRUE);
    emberAfCorePrintln("}");
  } else {
    emberAfCorePrintln("Security key set failed");
  }
}

void dataCommand(void)
{
  EmberStatus status;
  uint8_t length;
  EmberNodeId destination = emberUnsignedCommandArgument(0);
  uint8_t *contents = emberStringCommandArgument(1, &length);

  status = emberMessageSend(destination,
                            DATA_ENDPOINT,
                            0, // messageTag
                            length,
                            contents,
                            txOptions);

  emberAfCorePrint("TX: Data to 0x%2x:{", destination);
  emberAfCorePrintBuffer(contents, length, TRUE);
  emberAfCorePrintln("}: status=0x%x", status);
}

void infoCommand(void)
{
  emberAfCorePrintln("%p:", EMBER_AF_DEVICE_NAME);
  emberAfCorePrintln("  network state: 0x%x", emberNetworkState());
  emberAfCorePrintln("      node type: 0x%x", emberGetNodeType());
  emberAfCorePrintln("          eui64: >%x%x%x%x%x%x%x%x",
                     emberGetEui64()[7],
                     emberGetEui64()[6],
                     emberGetEui64()[5],
                     emberGetEui64()[4],
                     emberGetEui64()[3],
                     emberGetEui64()[2],
                     emberGetEui64()[1],
                     emberGetEui64()[0]);
  emberAfCorePrintln("        node id: 0x%2x", emberGetNodeId());
  emberAfCorePrintln("         pan id: 0x%2x", emberGetPanId());
  emberAfCorePrintln("        channel: %d", (uint16_t)emberGetRadioChannel());
  emberAfCorePrintln("          power: %d", (int16_t)emberGetRadioPower());
  emberAfCorePrintln("     TX options: MAC acks %s, security %s, priority %s",
                     ((txOptions & EMBER_OPTIONS_ACK_REQUESTED) ? "enabled" : "disabled"),
                     ((txOptions & EMBER_OPTIONS_SECURITY_ENABLED) ? "enabled" : "disabled"),
                     ((txOptions & EMBER_OPTIONS_HIGH_PRIORITY) ? "enabled" : "disabled"));

#ifdef EMBER_AF_PLUGIN_FREE_RTOS
  emberAfCorePrintln("rtos deep block: %l", rtosGetDeepSleepBlockCount());
  emberAfCorePrintln(" stk block deep: %l", stack_blocked_deep_sleep);
  emberAfCorePrintln(" stk allow deep: %l", stack_allowed_deep_sleep);
#endif
}

void counterCommand(void)
{
  uint8_t counterType = emberUnsignedCommandArgument(0);
  uint32_t counter;
  EmberStatus status = emberGetCounter(counterType, &counter);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Get counter failed, status=0x%x", status);
  } else {
    emberAfCorePrintln("Counter type=0x%x: %d", counterType, counter);
  }
}

void toggleRadioCommand(void)
{
  bool radioOn = (emberUnsignedCommandArgument(0) > 0);

  EmberStatus status = emberSetRadioPowerMode(radioOn);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Toggle radio failed, status=0x%x", status);
  } else {
    emberAfCorePrintln("Radio turned %s", (radioOn) ? "on" : "off");
  }
}

void txTestCommand(void)
{
  uint8_t i;
  txTestPacketSize = emberUnsignedCommandArgument(0);
  txTestInFlightPackets = emberUnsignedCommandArgument(1);
  txTestDestination = emberUnsignedCommandArgument(2);

  for (i = 0; i < txTestInFlightPackets; i++) {
    if (sendTxTestPacket() == EMBER_SUCCESS) {
      txTestCurrentInflightPackets++;
    } else {
      break;
    }
  }
}

void txTestPrintEnableCommand(void)
{
  txTestPrintEnabled = (emberUnsignedCommandArgument(0) > 0);

  emberAfCorePrintln("TX test prints %s",
                     (txTestPrintEnabled) ? "enabled" : "disabled");
}

void startEnergyScanCommand(void)
{
  EmberStatus status;
  uint8_t channelToScan = emberUnsignedCommandArgument(0);
  uint8_t samples = emberUnsignedCommandArgument(1);
  status = emberStartEnergyScan(channelToScan, samples);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Start energy scanning failed, status=0x%x", status);
  } else {
    emberAfCorePrintln("Start energy scanning: channel %d, samples %d",
                       channelToScan, samples);
  }
}

#ifdef UNIX_HOST
void setPhyModeCommand(void)
{
  uint8_t mode = emberUnsignedCommandArgument(0);
  EmberStatus status = emberSetPhyMode(mode);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Phy mode change failed, status=0x%x", status);
  } else {
    emberAfCorePrintln("Phy mode changed successfully");
  }
}

void getPhyModeCommand(void)
{
  uint8_t mode = emberGetPhyMode();

  if (mode == 0) {
    emberAfCorePrintln("Phy mode: SubGHz");
  } else if (mode == 1) {
    emberAfCorePrintln("Phy mode: 2.4GHz");
  } else {
    emberAfCorePrintln("Invalid image");
  }
}
#endif

void startFrequencyHoppingServerCommand(void)
{
  EmberStatus status = emberFrequencyHoppingStartServer();

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("FH Server Failed, 0x%x", status);
  } else {
    emberAfCorePrintln("FH Server Success");
  }
}

void startFrequencyHoppingClientCommand(void)
{
  EmberStatus status;
  EmberNodeId nodeId;
  EmberPanId panId;

  nodeId = emberUnsignedCommandArgument(0);
  panId = emberUnsignedCommandArgument(1);

  status = emberFrequencyHoppingStartClient(nodeId, panId);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("FH Client Failed, 0x%x", status);
  } else {
    emberAfCorePrintln("FH Client Success");
  }
}

void stopFrequencyHoppingCommand(void)
{
  EmberStatus status = emberFrequencyHoppingStop();

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("FH Stop Failed, 0x%x", status);
  } else {
    emberAfCorePrintln("FH Stopped");
  }
}
