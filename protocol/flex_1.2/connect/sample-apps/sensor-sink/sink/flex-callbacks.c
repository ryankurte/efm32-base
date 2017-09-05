// Sink sample application.
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
#ifndef UNIX_HOST
#include "heartbeat/heartbeat.h"
#endif
#include "hal/hal.h"
#include "command-interpreter/command-interpreter2.h"
#include "debug-print/debug-print.h"

#if !defined(UNIX_HOST) && !defined(CORTEXM3_EMBER_MICRO)
#include "em_gpio.h"
#include "em_cmu.h"
#endif

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
#define SENSOR_REPORT_PERIOD_MS      (1 * MILLISECOND_TICKS_PER_SECOND)
#define SENSOR_TIMEOUT_MS            (60 * MILLISECOND_TICKS_PER_SECOND)
#define SINK_DATA_DUMP_PERIOD_MS     (1 * MILLISECOND_TICKS_PER_SECOND)

#define NETWORK_UP_LED               BOARDLED0

#define SENSOR_SINK_SECURITY_KEY    { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, \
                                      0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, \
                                      0xAA, 0xAA, 0xAA, 0xAA }

#define SENSOR_TABLE_SIZE 10

enum {
  SENSOR_SINK_COMMAND_ID_ADVERTISE_REQUEST = 0,
  SENSOR_SINK_COMMAND_ID_ADVERTISE         = 1,
  SENSOR_SINK_COMMAND_ID_PAIR_REQUEST      = 2,
  SENSOR_SINK_COMMAND_ID_PAIR_CONFIRM      = 3,
  SENSOR_SINK_COMMAND_ID_DATA              = 4,
};
typedef uint8_t SensorSinkCommandId;

typedef struct {
  EmberNodeId nodeId;
  EmberEUI64  nodeEui64;
  uint8_t       reportedData[SENSOR_SINK_MAXIMUM_DATA_LENGTH];
  uint8_t       reportedDataLength;
  uint32_t      lastReportMs;
} Sensor;

static int8_t txPower = SENSOR_SINK_TX_POWER;
static Sensor sensors[SENSOR_TABLE_SIZE];
static EmberKeyData securityKey = SENSOR_SINK_SECURITY_KEY;

static void sinkInit(void);
static EmberStatus send(EmberNodeId nodeId,
                        SensorSinkCommandId commandId,
                        uint8_t *buffer,
                        uint8_t bufferLength);

EmberEventControl advertiseControl;
EmberEventControl dataReportControl;

static uint8_t message[SENSOR_SINK_MAXIMUM_LENGTH];
static EmberMessageLength messageLength;
static EmberMessageOptions txOptions = EMBER_OPTIONS_ACK_REQUESTED;

//------------------------------------------------------------------------------
// Callbacks

// An advertisement message consists of the sensor/sink protocol id, the
// advertisement command id, and the long and short ids of the sink.  Each sink
// on the network periodically broadcasts its advertisement to all other nodes.
void advertiseHandler(void)
{
  // If the sink is not on the network, the periodic event is cancelled and
  // advertisements are not set.
  if (!emberStackIsUp()) {
    emberEventControlSetInactive(advertiseControl);
  } else {
    EmberStatus status = send(EMBER_BROADCAST_ADDRESS,
                              SENSOR_SINK_COMMAND_ID_ADVERTISE,
                              NULL,
                              0);
    emberAfCorePrintln("TX: Advertise to 0x%2x: 0x%x",
                       EMBER_BROADCAST_ADDRESS,
                       status);
    emberEventControlSetDelayMS(advertiseControl,
                                SINK_ADVERTISEMENT_PERIOD_MS);
  }
}

// Here we print out the first two bytes reported by the sinks as a little
// endian 16-bits decimal.
void dataReportHandler(void)
{
  // If the sink is not on the network, the periodic event is cancelled and
  // sensors data is no longer printed out.
  if (!emberStackIsUp()) {
    emberEventControlSetInactive(dataReportControl);
  } else {
    uint8_t i;
    for (i = 0; i < SENSOR_TABLE_SIZE; i++) {
      if (sensors[i].nodeId != EMBER_NULL_NODE_ID
          && sensors[i].reportedDataLength >= 2) {
        // Temperature is sampled in "millicelsius".
        int32_t temperature = emberFetchLowHighInt32u(sensors[i].reportedData);
        emberAfCorePrintln("< %x%x%x%x%x%x%x%x , %d.%d%d >",
                           sensors[i].nodeEui64[7], sensors[i].nodeEui64[6],
                           sensors[i].nodeEui64[5], sensors[i].nodeEui64[4],
                           sensors[i].nodeEui64[3], sensors[i].nodeEui64[2],
                           sensors[i].nodeEui64[1], sensors[i].nodeEui64[0],
                           temperature / 1000,
                           (temperature % 1000) / 100,
                           (temperature % 100) / 10);
      }
    }
    emberEventControlSetDelayMS(dataReportControl,
                                SINK_DATA_DUMP_PERIOD_MS);
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
    {
      EmberStatus status;
      emberAfCorePrintln("RX: Advertise Request from 0x%2x", message->source);

      // We received an advertise request from a sensor, unicast back an advertise
      // command.
      status = send(message->source,
                    SENSOR_SINK_COMMAND_ID_ADVERTISE,
                    NULL,
                    0);
      emberAfCorePrintln("TX: Advertise to 0x%2x: 0x%x",
                         message->source,
                         status);
    }
    break;
    case SENSOR_SINK_COMMAND_ID_ADVERTISE:
      emberAfCorePrintln("RX: Advertise from 0x%2x", message->source);
      break;
    case SENSOR_SINK_COMMAND_ID_PAIR_REQUEST:
    {
      uint8_t i;
      emberAfCorePrintln("RX: Pair Request from 0x%2x", message->source);
      // Check whether the sensor is already present in the table first.
      for (i = 0; i < SENSOR_TABLE_SIZE; i++) {
        if (sensors[i].nodeId != EMBER_NULL_NODE_ID
            && MEMCOMPARE(sensors[i].nodeEui64,
                          message->payload + SENSOR_SINK_EUI64_OFFSET,
                          EUI64_SIZE) == 0) {
          break;
        }
      }

      // Find an empty entry in the table.
      if (i == SENSOR_TABLE_SIZE) {
        for (i = 0; i < SENSOR_TABLE_SIZE; i++) {
          if (sensors[i].nodeId == EMBER_NULL_NODE_ID) {
            break;
          }
        }
      }

      // Add or update the entry in the table.
      if (i < SENSOR_TABLE_SIZE) {
        EmberStatus status = send(message->source,
                                  SENSOR_SINK_COMMAND_ID_PAIR_CONFIRM,
                                  NULL,
                                  0);
        emberAfCorePrintln("TX: Pair Confirm to 0x%2x: 0x%x",
                           message->source,
                           status);
        if (status == EMBER_SUCCESS) {
          sensors[i].nodeId = message->source;

          MEMCOPY(sensors[i].nodeEui64,
                  message->payload + SENSOR_SINK_EUI64_OFFSET,
                  EUI64_SIZE);

          sensors[i].lastReportMs = halCommonGetInt32uMillisecondTick();
        }
      }
    }
    break;
    case SENSOR_SINK_COMMAND_ID_PAIR_CONFIRM:
      emberAfCorePrintln("RX: Pair Confirm from 0x%2x", message->source);
      break;
    case SENSOR_SINK_COMMAND_ID_DATA:
    {
      uint8_t i, j;
      for (i = 0; i < SENSOR_TABLE_SIZE; i++) {
        if (MEMCOMPARE(sensors[i].nodeEui64,
                       message->payload + SENSOR_SINK_EUI64_OFFSET,
                       EUI64_SIZE) == 0) {
          emberAfCorePrint("RX: Data from 0x%2x:", message->source);
          for (j = SENSOR_SINK_DATA_OFFSET; j < message->length; j++) {
            emberAfCorePrint(" %x", message->payload[j]);
          }
          emberAfCorePrintln("");

          sensors[i].reportedDataLength = message->length - SENSOR_SINK_DATA_OFFSET;

          MEMCOPY(sensors[i].reportedData,
                  message->payload + SENSOR_SINK_DATA_OFFSET,
                  sensors[i].reportedDataLength);

          sensors[i].lastReportMs = halCommonGetInt32uMillisecondTick();
          break;
        }
      }
    }
    break;
    default:
      emberAfCorePrintln("RX: Unknown from 0x%2x", message->source);
      break;
  }
}

void emberAfMessageSentCallback(EmberStatus status,
                                EmberOutgoingMessage *message)
{
  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("TX: 0x%x", status);
  }
}

void emberAfStackStatusCallback(EmberStatus status)
{
  switch (status) {
    case EMBER_NETWORK_UP:
      emberAfCorePrintln("Network up");

      emberEventControlSetActive(advertiseControl);
      emberEventControlSetActive(dataReportControl);
      break;
    case EMBER_NETWORK_DOWN:
      emberAfCorePrintln("Network down");
      sinkInit();
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

  // Initialize the list of sensors from which we receive reports.
  sinkInit();

  emberNetworkInit();
}

// This callback is called in each iteration of the main application loop and
// can be used to perform periodic functions.
void emberAfMainTickCallback(void)
{
  // Time out sensors that have not reported in a while.
  uint32_t nowMs = halCommonGetInt32uMillisecondTick();
  uint8_t i;
  for (i = 0; i < SENSOR_TABLE_SIZE; i++) {
    if (sensors[i].nodeId != EMBER_NULL_NODE_ID) {
      if (SENSOR_TIMEOUT_MS
          < elapsedTimeInt32u(sensors[i].lastReportMs, nowMs)) {
        emberAfCorePrintln("EVENT: timed out sensor 0x%2x",
                           sensors[i].nodeId);
        sensors[i].nodeId = EMBER_NULL_NODE_ID;
      }
    }
  }

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

static void sinkInit(void)
{
  uint8_t i;
  for (i = 0; i < SENSOR_TABLE_SIZE; i++) {
    sensors[i].nodeId = EMBER_NULL_NODE_ID;
  }
}

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

//------------------------------------------------------------------------------
// CLI commands

void formCommand(void)
{
  EmberStatus status;
  EmberNetworkParameters parameters;

  // Initialize the security key to the default key prior to forming the
  // network.
  emberSetSecurityKey(&securityKey);

  MEMSET(&parameters, 0, sizeof(EmberNetworkParameters));
  parameters.radioTxPower = txPower;
  parameters.radioChannel = emberUnsignedCommandArgument(0);
  parameters.panId = SENSOR_SINK_PAN_ID;

  status = emberFormNetwork(&parameters);

  emberAfCorePrintln("form 0x%x", status);
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

void advertiseCommand(void)
{
  emberEventControlSetActive(advertiseControl);
}

void sensorsCommand(void)
{
  uint8_t i;
  emberAfCorePrintln("### Sensors table ###");
  for (i = 0; i < SENSOR_TABLE_SIZE; i++) {
    if (sensors[i].nodeId != EMBER_NULL_NODE_ID) {
      emberAfCorePrintln("entry:%d id:0x%2x eui64:(>)%x%x%x%x%x%x%x%x last report:0x%4x",
                         (uint16_t)i,
                         sensors[i].nodeId,
                         sensors[i].nodeEui64[7], sensors[i].nodeEui64[6],
                         sensors[i].nodeEui64[5], sensors[i].nodeEui64[4],
                         sensors[i].nodeEui64[3], sensors[i].nodeEui64[2],
                         sensors[i].nodeEui64[1], sensors[i].nodeEui64[0],
                         sensors[i].lastReportMs);
    }
  }
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
