/*
 * File: host-network-management.c
 * Description: Host CLI for Connect API.
 *
 * Author(s): Maurizio Nanni, maurizio.nanni@silabs.com
 *
 * Copyright 2013 by Silicon Laboratories. All rights reserved.
 */

#include PLATFORM_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "command-interpreter/command-interpreter2.h"
#include "debug-print/debug-print.h"
#include "poll/poll.h"

#define EMBER_PHY_MAX_PAYLOAD_LENGTH 107

/*------------------------------------------------------------------------*/
/*              Static variables and functions                            */
/*------------------------------------------------------------------------*/

EmberEventControl sendPktEvent;
void sendPktEventHandler(void);

static uint16_t pktsToSend = 0;
static uint8_t pktPayloadLength = 0;
static EmberNodeId destNodeId = EMBER_NULL_NODE_ID;
static uint8_t sendOptions = (EMBER_OPTIONS_ACK_REQUESTED);

static uint16_t pktsDelivered = 0;
static uint16_t ccaFails = 0;
static uint16_t ackFails = 0;
static uint16_t unknownErrors = 0;
static EmberKeyData defaultKey = { 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
                                   0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
                                   0xAA, 0xAA, 0xAA, 0xAA };

static void sendDataPacket(void)
{
  uint8_t packet[EMBER_PHY_MAX_PAYLOAD_LENGTH + 4];
  uint16_t i = 0;
  uint16_t cnt;
  EmberStatus status;

  // we will only process packet sizes up to 0x6F and greater than 8
  if (pktPayloadLength > EMBER_PHY_MAX_PAYLOAD_LENGTH + 4) {
    emberAfCorePrintln("Packet size too big.");
    return;
  } else if (pktPayloadLength < 8) {
    emberAfCorePrintln("Packet size too small.");
    return;
  }

  packet[i++] = 'S';
  packet[i++] = 'T';
  packet[i++] = 'A';
  packet[i++] = 'R';
  packet[i++] = 'T';

  for (cnt = 0; cnt < (pktPayloadLength - 8); cnt++) {
    packet[i++] = cnt;
  }

  packet[i++] = 'E';
  packet[i++] = 'N';
  packet[i++] = 'D';

  status = emberMessageSend(destNodeId,
                            0, // endpoint
                            0, // messageTag
                            pktPayloadLength,
                            packet,
                            sendOptions);

  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("Send failed, status 0x%x", status);
  }
}

/*------------------------------------------------------------------------*/
/*              Handlers and callbacks                                    */
/*------------------------------------------------------------------------*/

void emberAfMainInitCallback(void)
{
  emberAfCorePrintln("<Power UP>");

  emberSetSecurityKey(&defaultKey);

  EmberStatus status = emberNetworkInit();
  assert(status == EMBER_SUCCESS || status == EMBER_NOT_JOINED);
}

void emberAfMainTickCallback(void)
{
}

void emberAfIncomingMessageCallback(EmberIncomingMessage *message)
{
  EmberMessageLength i = 0;
  bool rxSuccess = FALSE;

  // Data packet received
  if ((message->payload[i++] == 'S')
      && (message->payload[i++] == 'T')
      && (message->payload[i++] == 'A')
      && (message->payload[i++] == 'R')
      && (message->payload[i++] == 'T')) {
    i += message->length - 8;

    if ((message->payload[i++] == 'E')
        && (message->payload[i++] == 'N')
        && (message->payload[i++] == 'D')) {
      rxSuccess = TRUE;

      emberAfCorePrintln("RX %s data packet, size 0x%x",
                         ((message->options & EMBER_OPTIONS_SECURITY_ENABLED)
                          ? "secured"
                          : "unsecured"),
                         message->length);
    }
  }

  // Packet content is not what was expected
  if (!rxSuccess) {
    emberAfCorePrintln("***RX_ERR***");
  }
}

void emberAfMessageSentCallback(EmberStatus status,
                                EmberOutgoingMessage *message)
{
  UNUSED_VAR(message);

  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("TX success");
    pktsDelivered++;
  } else {
    emberAfCorePrintln("TX failed, 0x%x", status);
    if (status == EMBER_PHY_TX_CCA_FAIL) {
      ccaFails++;
    } else if (status == EMBER_MAC_NO_ACK_RECEIVED) {
      ackFails++;
    } else {
      unknownErrors++;
    }
  }

  if (pktsToSend != 0 && --pktsToSend > 0) {
    // Schedule a new packet in 100msec
    emberEventControlSetDelayMS(sendPktEvent, 100);
  } else {
    emberEventControlSetInactive(sendPktEvent);
    emberAfCorePrintln("Test finished");
  }
}

void emberAfStackStatusCallback(EmberStatus status)
{
  emberAfCorePrintln("Stack status:0x%x", status);
}

void emberAfChildJoinCallback(EmberNodeType nodeType,
                              EmberNodeId nodeId)
{
  emberAfCorePrintln("Node 0x%2x joined as %s", nodeId,
                     (nodeType == EMBER_STAR_RANGE_EXTENDER)
                     ? "range extender"
                     : ((nodeType == EMBER_STAR_END_DEVICE)
                        ? "end device"
                        : "sleepy"));
}

void emberAfIncomingBeaconCallback(EmberPanId panId,
                                   EmberNodeId nodeId,
                                   uint8_t payloadLength,
                                   uint8_t *payload)
{
  emberAfCorePrint("BEACON: panId 0x%2X nodeId 0x%2X payload {", panId, nodeId);
  emberAfCorePrintBuffer(payload, payloadLength, FALSE);
  emberAfCorePrintln("}");
}

void emberAfActiveScanCompleteCallback(void)
{
  emberAfCorePrintln("Active scan complete");
}

void emberAfEnergyScanCompleteCallback(int8_t mean,
                                       int8_t min,
                                       int8_t max,
                                       uint16_t variance)
{
  emberAfCorePrintln("Energy scan complete, mean=%d min=%d max=%d var=%d",
                     mean, min, max, variance);
}

void emberFrequencyHoppingStartClientCompleteHandler(EmberStatus status)
{
  if (status != EMBER_SUCCESS) {
    emberAfCorePrintln("FH Client sync failed, status=0x%x", status);
  } else {
    emberAfCorePrintln("FH Client Sync Success");
  }
}

/*------------------------------------------------------------------------*/

void setChannelCommand(void)
{
  emberSetRadioChannel(emberUnsignedCommandArgument(0));
  emberAfCorePrintln("Radio channel set to 0x%X", emberGetRadioChannel());
}

void sendPacketsCommand(void)
{
  pktsToSend = emberUnsignedCommandArgument(0);
  pktPayloadLength = emberUnsignedCommandArgument(1);
  destNodeId = emberUnsignedCommandArgument(2);

  pktsDelivered = 0;
  ccaFails = 0;
  ackFails = 0;
  unknownErrors = 0;

  if (pktsToSend > 0) {
    sendDataPacket();
  }
}

void activeScanCommand(void)
{
  EmberStatus status;
  uint8_t channelToScan = emberUnsignedCommandArgument(0);
  status = emberStartActiveScan(channelToScan);
  emberAfCorePrintln("Start active scanning: channel %d, status=0x%x", channelToScan, status);
}

void energyScanCommand(void)
{
  EmberStatus status;
  uint8_t channelToScan = emberUnsignedCommandArgument(0);
  uint8_t samples = emberUnsignedCommandArgument(1);
  status = emberStartEnergyScan(channelToScan, samples);
  emberAfCorePrintln("Start energy scanning: channel %d, samples %d, status=0x%x", channelToScan, samples, status);
}

void setBeaconPayloadCommand(void)
{
  EmberStatus status;
  uint8_t length;
  uint8_t *contents = emberStringCommandArgument(0, &length);

  status = emberSetApplicationBeaconPayload(length, contents);

  emberAfCorePrint("Set beacon payload: {");
  emberAfCorePrintBuffer(contents, length, FALSE);
  emberAfCorePrintln("}: status=0x%x", status);
}

void setPermitJoinCommand(void)
{
  uint8_t duration = emberUnsignedCommandArgument(0);
  emberPermitJoining(duration);
  emberAfCorePrintln("Permit join set 0x%X", duration);
}

void formNetworkCommand(void)
{
  EmberNetworkParameters parameters;
  EmberStatus status;
  parameters.panId = emberUnsignedCommandArgument(0);
  parameters.radioTxPower = emberSignedCommandArgument(1);
  parameters.radioChannel = emberUnsignedCommandArgument(2);

  status = emberFormNetwork(&parameters);

  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Form network succeeded");
  } else {
    emberAfCorePrintln("Form network failed, status 0x%X", status);
  }
}

void joinNetworkCommand(void)
{
  EmberNetworkParameters parameters;
  EmberStatus status;
  uint8_t nodeType = emberUnsignedCommandArgument(0);
  parameters.panId = emberUnsignedCommandArgument(1);
  parameters.radioTxPower = emberSignedCommandArgument(2);
  parameters.radioChannel = emberUnsignedCommandArgument(3);

  status = emberJoinNetwork(nodeType, &parameters);

  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Started the joining process");
  } else {
    emberAfCorePrintln("Join network failed, status 0x%X", status);
  }
}

void joinExtendedNetworkCommand(void)
{
  EmberNetworkParameters parameters;
  EmberStatus status;
  uint8_t nodeType = emberUnsignedCommandArgument(0);
  EmberNodeId nodeId = emberUnsignedCommandArgument(1);
  parameters.panId = emberUnsignedCommandArgument(2);
  parameters.radioTxPower = emberSignedCommandArgument(3);
  parameters.radioChannel = emberUnsignedCommandArgument(4);

  status = emberJoinNetworkExtended(nodeType, nodeId, &parameters);

  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Started the joining process");
  } else {
    emberAfCorePrintln("Join network failed, status 0x%X", status);
  }
}

void joinCommissionedCommand(void)
{
  EmberNetworkParameters parameters;
  EmberStatus status;
  uint8_t nodeType = emberUnsignedCommandArgument(0);
  EmberNodeId nodeId = emberUnsignedCommandArgument(1);
  parameters.panId = emberUnsignedCommandArgument(2);
  parameters.radioTxPower = emberSignedCommandArgument(3);
  parameters.radioChannel = emberUnsignedCommandArgument(4);

  status = emberJoinCommissioned(nodeType, nodeId, &parameters);

  if (status == EMBER_SUCCESS) {
    emberAfCorePrintln("Network commissioned");
  } else {
    emberAfCorePrintln("Commissioning failed, status 0x%X", status);
  }
}

void printInfoCommand(void)
{
  emberAfCorePrintln("Node ID      0x%2x", emberGetNodeId());
  emberAfCorePrint("Node EUI64    { ");
  uint8_t* eui64 = emberGetEui64();
  uint8_t i;
  for (i = 0; i < EUI64_SIZE; i++) {
    emberAfCorePrint("%x ", eui64[i]);
  }
  emberAfCorePrintln("}");
  emberAfCorePrintln("Pan ID        0x%2x", emberGetPanId());
  emberAfCorePrintln("Node type     0x%x", emberGetNodeType());
  emberAfCorePrintln("Parent ID     0x%2x", emberGetParentId());
  emberAfCorePrintln("Nwk state     0x%x", emberNetworkState());
}

void stackIsUpCommand(void)
{
  bool stackIsUp = emberStackIsUp();
  if (stackIsUp) {
    emberAfCorePrintln("Stack is up");
  } else {
    emberAfCorePrintln("Stack is NOT up");
  }
}

void napCommand(void)
{
  bool okToNap = emberOkToNap();
  if (okToNap) {
    emberAfCorePrintln("It is ok to Nap");
  } else {
    emberAfCorePrintln("It is NOT ok to Nap");
  }
}

void hibernateCommand(void)
{
  bool okToHibernate = emberOkToHibernate();
  if (okToHibernate) {
    emberAfCorePrintln("It is ok to Hibernate");
  } else {
    emberAfCorePrintln("It is NOT ok to Hibernate");
  }
}

void resetNetworkCommand(void)
{
  emberResetNetworkState();
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

void setOptionsCommand(void)
{
  sendOptions = emberUnsignedCommandArgument(0);
  emberAfCorePrintln("Send options set: 0x%x", sendOptions);
}

void pollCommand(void)
{
  EmberStatus status = emberPollForData();

  emberAfCorePrintln("Poll status 0x%x", status);
}

void shortPollCommand(void)
{
  bool enable = (emberUnsignedCommandArgument(0) > 0);

  emberAfPluginPollEnableShortPolling(enable);

  emberAfCorePrintln("Short poll %s",
                     ((enable) ? "enabled" : "disabled"));
}

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

void echoCommand(void)
{
  uint8_t length;
  uint8_t *data = emberStringCommandArgument(0, &length);
  emberEcho(data, length);
}

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

/*------------------------------------------------------------------------*/
/*                           Event handlers                               */
/*------------------------------------------------------------------------*/

void sendPktEventHandler(void)
{
  emberEventControlSetInactive(sendPktEvent);
  sendDataPacket();
}
