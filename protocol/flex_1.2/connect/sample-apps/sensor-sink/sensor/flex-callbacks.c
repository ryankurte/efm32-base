// Sensor sample application.
//
// Copyright 2013 Silicon Laboratories, Inc.                                *80*

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER

#if !defined(UNIX_HOST) && !defined(CORTEXM3_EMBER_MICRO)
#include "em_device.h"
#include "em_chip.h"
#include "bsp.h"
#endif

#include "stack/include/ember.h"
#include "hal/hal.h"
#ifndef UNIX_HOST
#include "heartbeat/heartbeat.h"
#include "wstk-sensors/wstk-sensors.h"
#endif
#include "poll/poll.h"
#include "command-interpreter/command-interpreter2.h"
#include "debug-print/debug-print.h"

#if (defined EMBER_AF_PLUGIN_FREE_RTOS)
#include "FreeRTOS.h"
#include "task.h"
#include EMBER_AF_API_FREE_RTOS
extern long stack_blocked_deep_sleep;
extern long stack_allowed_deep_sleep;
#endif

#define SENSOR_SINK_TX_POWER    0
#define SENSOR_SINK_PAN_ID      0x01FF
#define SENSOR_SINK_PROTOCOL_ID 0xC00F

#define SENSOR_SINK_PROTOCOL_ID_OFFSET  0
#define SENSOR_SINK_COMMAND_ID_OFFSET   2
#define SENSOR_SINK_EUI64_OFFSET        3
#define SENSOR_SINK_NODE_ID_OFFSET      11
#define SENSOR_SINK_DATA_OFFSET         13
#define SENSOR_SINK_MAXIMUM_DATA_LENGTH 8
#define SENSOR_SINK_MINIMUM_LENGTH      SENSOR_SINK_DATA_OFFSET
#define SENSOR_SINK_MAXIMUM_LENGTH      (SENSOR_SINK_MINIMUM_LENGTH \
                                         + SENSOR_SINK_MAXIMUM_DATA_LENGTH)

#define SINK_ADVERTISEMENT_PERIOD_MS (60 * MILLISECOND_TICKS_PER_SECOND)
#define SENSOR_TIMEOUT_MS            (60 * MILLISECOND_TICKS_PER_SECOND)
#define SENSOR_PAIR_TIMEOUT_MS       (5 * MILLISECOND_TICKS_PER_SECOND)

#define NETWORK_UP_LED               BOARDLED0

#define SENSOR_SINK_SECURITY_KEY    { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, \
                                      0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, \
                                      0xAA, 0xAA, 0xAA, 0xAA }

enum {
  SENSOR_SINK_COMMAND_ID_ADVERTISE_REQUEST = 0,
  SENSOR_SINK_COMMAND_ID_ADVERTISE         = 1,
  SENSOR_SINK_COMMAND_ID_PAIR_REQUEST      = 2,
  SENSOR_SINK_COMMAND_ID_PAIR_CONFIRM      = 3,
  SENSOR_SINK_COMMAND_ID_DATA              = 4,
};
typedef uint8_t SensorSinkCommandId;

static int8_t txPower = SENSOR_SINK_TX_POWER;
static EmberNodeId sinkNodeId = EMBER_NULL_NODE_ID;
static bool confirmed;
static EmberKeyData securityKey = SENSOR_SINK_SECURITY_KEY;

static EmberStatus send(EmberNodeId nodeId,
                        SensorSinkCommandId commandId,
                        uint8_t *buffer,
                        uint8_t bufferLength);
static EmberStatus requestAdvertise(void);

static uint8_t message[SENSOR_SINK_MAXIMUM_LENGTH];
static EmberMessageLength messageLength;
static EmberMessageOptions txOptions = EMBER_OPTIONS_ACK_REQUESTED;

static uint16_t sensorReportPeriodMs =  (1 * MILLISECOND_TICKS_PER_SECOND);

EmberEventControl reportControl;

//------------------------------------------------------------------------------
// Callbacks

void reportHandler(void)
{
  if (!emberStackIsUp() || sinkNodeId == EMBER_NULL_NODE_ID || !confirmed) {
    emberEventControlSetInactive(reportControl);
    emberAfPluginPollEnableShortPolling(FALSE);
  } else {
    EmberStatus status, sensorStatus;
    uint8_t buffer[SENSOR_SINK_MAXIMUM_DATA_LENGTH];
    int32_t tempData = 0;
    uint32_t rhData = 0;
    uint8_t i;

    // Sample temperature and humidity from sensors.
    // Temperature is sampled in "millicelsius".
    #ifndef UNIX_HOST
    sensorStatus = emberAfPluginWstkSensorsGetSample(&rhData, &tempData);
    #else
    sensorStatus = EMBER_SUCCESS;
    #endif

    if (sensorStatus == EMBER_SUCCESS) {
      emberStoreLowHighInt32u(buffer, tempData);
      emberStoreLowHighInt32u(buffer + 4, rhData);

      status = send(sinkNodeId,
                    SENSOR_SINK_COMMAND_ID_DATA,
                    buffer,
                    SENSOR_SINK_MAXIMUM_DATA_LENGTH);
      emberAfCorePrint("TX: Data to 0x%2x:", sinkNodeId);
      for (i = 0; i < SENSOR_SINK_MAXIMUM_DATA_LENGTH; i++) {
        emberAfCorePrint(" %x", buffer[i]);
      }
      emberAfCorePrintln(": 0x%x", status);
      emberEventControlSetDelayMS(reportControl, sensorReportPeriodMs);
    }
  }
}

void emberAfIncomingMessageCallback(EmberIncomingMessage *message)
{
  if (message->length < SENSOR_SINK_MINIMUM_LENGTH
      || (emberFetchLowHighInt16u(message->payload + SENSOR_SINK_PROTOCOL_ID_OFFSET)
          != SENSOR_SINK_PROTOCOL_ID)) {
    return;
  }

  switch (message->payload[SENSOR_SINK_COMMAND_ID_OFFSET]) {
    case SENSOR_SINK_COMMAND_ID_ADVERTISE_REQUEST:
      emberAfCorePrintln("RX: Advertise Request from 0x%2x", message->source);
      break;
    case SENSOR_SINK_COMMAND_ID_ADVERTISE:
    {
      emberAfCorePrintln("RX: Advertise from 0x%2x", message->source);
      if (sinkNodeId == EMBER_NULL_NODE_ID || !confirmed) {
        EmberStatus status = send(EMBER_COORDINATOR_ADDRESS,
                                  SENSOR_SINK_COMMAND_ID_PAIR_REQUEST,
                                  NULL,
                                  0);
        emberAfCorePrintln("TX: Pair Request to 0x%2x: 0x%x",
                           EMBER_COORDINATOR_ADDRESS,
                           status);
        if (status == EMBER_SUCCESS) {
          sinkNodeId = EMBER_COORDINATOR_ADDRESS;
          confirmed = FALSE;

          // If the node is sleepy switch to short poll mode to get the confirm
          // command.
          if (emberGetNodeType() == EMBER_STAR_SLEEPY_END_DEVICE) {
            emberAfPluginPollEnableShortPolling(TRUE);
          }
        }
      } else {
        emberAfPluginPollEnableShortPolling(FALSE);
      }
      break;
    }
    case SENSOR_SINK_COMMAND_ID_PAIR_REQUEST:
      emberAfCorePrintln("RX: Pair Request from 0x%2x", message->source);
      break;
    case SENSOR_SINK_COMMAND_ID_PAIR_CONFIRM:
    {
      emberAfCorePrintln("RX: Pair Confirm from 0x%2x", message->source);
      if (message->source == sinkNodeId) {
        confirmed = TRUE;
        reportHandler();
        emberAfPluginPollEnableShortPolling(FALSE);
      }
      break;
    }
    case SENSOR_SINK_COMMAND_ID_DATA:
    {
      uint8_t i;
      emberAfCorePrint("RX: Data from 0x%2x:", message->source);
      for (i = SENSOR_SINK_DATA_OFFSET; i < message->length; i++) {
        emberAfCorePrint(" %x", message->payload[i]);
      }
      emberAfCorePrintln("");
      break;
    }
    default:
      emberAfCorePrintln("RX: Unknown from 0x%2x", message->source);
      break;
  }
}

#define MAX_TX_FAILURES     10
static uint8_t txFailureCount = 0;

void emberAfMessageSentCallback(EmberStatus status,
                                EmberOutgoingMessage *message)
{
  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("TX: 0x%x", status);
    txFailureCount++;
    if (SENSOR_SINK_MINIMUM_LENGTH <= message->length
        && (emberFetchLowHighInt16u(message->payload + SENSOR_SINK_PROTOCOL_ID_OFFSET)
            == SENSOR_SINK_PROTOCOL_ID
            && (message->payload[SENSOR_SINK_COMMAND_ID_OFFSET]
                == SENSOR_SINK_COMMAND_ID_DATA))
        && txFailureCount >= MAX_TX_FAILURES) {
      emberAfCorePrintln("EVENT: dropped sink 0x%2x", sinkNodeId);
      sinkNodeId = EMBER_NULL_NODE_ID;
    }
  } else {
    // Success: reset the failures count.
    txFailureCount = 0;
  }
}

void emberAfStackStatusCallback(EmberStatus status)
{
  switch (status) {
    case EMBER_NETWORK_UP:
      emberAfCorePrintln("Network up");

      // Unicast an advertise request to the sink.
      requestAdvertise();
      break;
    case EMBER_NETWORK_DOWN:
      sinkNodeId = EMBER_NULL_NODE_ID;
      emberAfCorePrintln("Network down");
      break;
    case EMBER_JOIN_FAILED:
      emberAfCorePrintln("Join failed");
      break;
    default:
      emberAfCorePrintln("Stack status: 0x%x", status);
      break;
  }
}

// This callback is called when the application starts and can be used to
// perform any additional initialization required at system startup.
void emberAfMainInitCallback(void)
{
  emberAfCorePrintln("INIT: %p", EMBER_AF_DEVICE_NAME);
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

static EmberStatus send(EmberNodeId nodeId,
                        SensorSinkCommandId commandId,
                        uint8_t *buffer,
                        uint8_t bufferLength)
{
  messageLength = 0;
  emberStoreLowHighInt16u(message + messageLength, SENSOR_SINK_PROTOCOL_ID);
  messageLength += 2;
  message[messageLength++] = commandId;
  MEMCOPY(message + messageLength, emberGetEui64(), EUI64_SIZE);
  messageLength += EUI64_SIZE;
  emberStoreLowHighInt16u(message + messageLength, emberGetNodeId());
  messageLength += 2;
  if (bufferLength != 0) {
    MEMCOPY(message + messageLength, buffer, bufferLength);
    messageLength += bufferLength;
  }
  return emberMessageSend(nodeId,
                          0, // endpoint
                          0, // messageTag
                          messageLength,
                          message,
                          txOptions);
}

static EmberStatus requestAdvertise(void)
{
  EmberStatus status = send(EMBER_COORDINATOR_ADDRESS,
                            SENSOR_SINK_COMMAND_ID_ADVERTISE_REQUEST,
                            NULL,
                            0);
  // Enable short poll to get the advertise unicast. This implicitly kicks
  // off the periodic polling.
  if (emberGetNodeType() == EMBER_STAR_SLEEPY_END_DEVICE) {
    emberAfPluginPollEnableShortPolling(TRUE);
  }
  // Use the report event to timeout on the pairing process.
  emberEventControlSetDelayMS(reportControl, SENSOR_PAIR_TIMEOUT_MS);
  emberAfCorePrintln("TX: Advertise Request (unicast), status 0x%x",
                     status);

  return status;
}

//------------------------------------------------------------------------------
// CLI commands

void joinCommand(void)
{
  EmberStatus status;
  EmberNetworkParameters parameters;

  // Initialize the security key to the default key prior to joining the
  // network.
  emberSetSecurityKey(&securityKey);

  MEMSET(&parameters, 0, sizeof(EmberNetworkParameters));
  parameters.radioTxPower = txPower;
  parameters.radioChannel = emberUnsignedCommandArgument(0);
  parameters.panId = SENSOR_SINK_PAN_ID;
  status = emberJoinNetwork(EMBER_STAR_END_DEVICE, &parameters);
  emberAfCorePrintln("join end device 0x%x", status);
}

void joinSleepyCommand(void)
{
  EmberStatus status;
  EmberNetworkParameters parameters;

  // Initialize the security key to the default key prior to joining the
  // network.
  emberSetSecurityKey(&securityKey);

  MEMSET(&parameters, 0, sizeof(EmberNetworkParameters));
  parameters.radioTxPower = txPower;
  parameters.radioChannel = emberUnsignedCommandArgument(0);
  parameters.panId = SENSOR_SINK_PAN_ID;
  status = emberJoinNetwork(EMBER_STAR_SLEEPY_END_DEVICE, &parameters);
  emberAfCorePrintln("join sleepy 0x%x", status);
}

void joinRangeExtenderCommand(void)
{
  EmberStatus status;
  EmberNetworkParameters parameters;

  // Initialize the security key to the default key prior to joining the
  // network.
  emberSetSecurityKey(&securityKey);

  MEMSET(&parameters, 0, sizeof(EmberNetworkParameters));
  parameters.radioTxPower = txPower;
  parameters.radioChannel = emberUnsignedCommandArgument(0);
  parameters.panId = SENSOR_SINK_PAN_ID;
  status = emberJoinNetwork(EMBER_STAR_RANGE_EXTENDER, &parameters);
  emberAfCorePrintln("join range extender 0x%x", status);
}

void pjoinCommand(void)
{
  uint8_t duration = (uint8_t)emberUnsignedCommandArgument(0);
  emberPermitJoining(duration);
}

void setTxOptionsCommand(void)
{
  txOptions = emberUnsignedCommandArgument(0);
  emberAfCorePrintln("TX options set: MAC acks %s, security %s, priority %s",
                     ((txOptions & EMBER_OPTIONS_ACK_REQUESTED) ? "enabled" : "disabled"),
                     ((txOptions & EMBER_OPTIONS_SECURITY_ENABLED) ? "enabled" : "disabled"),
                     ((txOptions & EMBER_OPTIONS_HIGH_PRIORITY) ? "enabled" : "disabled"));
}

void setTxPowerCommand(void)
{
  txPower = emberSignedCommandArgument(0);

  if (emberSetRadioPower(txPower) == EMBER_SUCCESS) {
    emberAfCorePrintln("TX power set: %d", (int8_t)emberGetRadioPower());
  } else {
    emberAfCorePrintln("TX power set failed");
  }
}

void setSecurityKeyCommand(void)
{
  EmberKeyData key;
  emberCopyKeyArgument(0, &key);

  if (emberSetSecurityKey(&key) == EMBER_SUCCESS) {
    uint8_t i;

    emberAfCorePrint("Security key set {");
    for (i = 0; i < EMBER_ENCRYPTION_KEY_SIZE; i++) {
      emberAfCorePrint("%x", key.contents[i]);
    }
    emberAfCorePrintln("}");
  } else {
    emberAfCorePrintln("Security key set failed");
  }
}

void setReportPeriodCommand(void)
{
  sensorReportPeriodMs = emberUnsignedCommandArgument(0);

  emberAfCorePrintln("Report period set to %d ms", sensorReportPeriodMs);
}

void advertiseRequestCommand(void)
{
  requestAdvertise();
}

void dataCommand(void)
{
  emberEventControlSetActive(reportControl);
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
