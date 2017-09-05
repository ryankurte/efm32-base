/**************************************************************************//**
 * Copyright 2017 Silicon Laboratories, Inc.
 *
 *****************************************************************************/
//
// *** Generated file. Do not edit! ***
//
// Description: Host functions for sending Connect management commands
// to the NCP.
//
// CSP Version: 1.0

#define EMBER_APPLICATION_HAS_BINARY_COMMAND_INTERPRETER

#include PLATFORM_HEADER
#ifdef CONFIGURATION_HEADER
  #include CONFIGURATION_HEADER
#endif
#include "include/ember.h"
#include "host-ncp/binary-management.h"
#include "host-ncp/ash-v3-test.h"
#include "command-interpreter/command-interpreter2.h"
#include "csp/csp-common.h"
#include "csp/csp-enum.h"
#include "csp/callbacks-frame-utilities.h"
#include "csp/csp-frame-utilities.h"

//------------------------------------------------------------------------------
// Core
//------------------------------------------------------------------------------

// setPhyMode
EmberStatus emberSetPhyMode(uint8_t mode)
{
  emSendBinaryManagementBlockingCommand(EMBER_SET_PHY_MODE_COMMAND_IDENTIFIER,
                                        "u",
                                        mode);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberSetPhyModeReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// getPhyMode
uint8_t emberGetPhyMode(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_PHY_MODE_COMMAND_IDENTIFIER,
                                        "");

  uint8_t mode;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &mode);
  return mode;
}
static void emberGetPhyModeReturn(void)
{
  uint8_t mode;
  mode = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &mode);
}

// networkState
EmberNetworkStatus emberNetworkState(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_NETWORK_STATE_COMMAND_IDENTIFIER,
                                        "");

  EmberNetworkStatus networkStatus;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &networkStatus);
  return networkStatus;
}
static void emberNetworkStateReturn(void)
{
  EmberNetworkStatus networkStatus;
  networkStatus = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &networkStatus);
}

// stackIsUp
bool emberStackIsUp(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_STACK_IS_UP_COMMAND_IDENTIFIER,
                                        "");

  bool isStackUp;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &isStackUp);
  return isStackUp;
}
static void emberStackIsUpReturn(void)
{
  bool isStackUp;
  isStackUp = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &isStackUp);
}

// setSecurityKey
EmberStatus emberSetSecurityKey(EmberKeyData *key)
{
  emSendBinaryManagementBlockingCommand(EMBER_SET_SECURITY_KEY_COMMAND_IDENTIFIER,
                                        "b",
                                        key, sizeof(EmberKeyData));

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberSetSecurityKeyReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// getCounter
EmberStatus emberGetCounter(EmberCounterType counterType,
                            uint32_t* count)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_COUNTER_COMMAND_IDENTIFIER,
                                        "uw",
                                        counterType,
                                        *count);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "uw",
                    &status,
                    count);
  return status;
}
static void emberGetCounterReturn(void)
{
  EmberStatus status;
  uint32_t count;
  status = emberUnsignedCommandArgument(0);
  count = emberUnsignedCommandArgument(1);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "uw",
                    &status,
                    &count);
}

// setRadioChannel
EmberStatus emberSetRadioChannel(uint8_t channel)
{
  emSendBinaryManagementBlockingCommand(EMBER_SET_RADIO_CHANNEL_COMMAND_IDENTIFIER,
                                        "u",
                                        channel);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberSetRadioChannelReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// getRadioChannel
uint8_t emberGetRadioChannel(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_RADIO_CHANNEL_COMMAND_IDENTIFIER,
                                        "");

  uint8_t channel;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &channel);
  return channel;
}
static void emberGetRadioChannelReturn(void)
{
  uint8_t channel;
  channel = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &channel);
}

// setRadioPower
EmberStatus emberSetRadioPower(int8_t power)
{
  emSendBinaryManagementBlockingCommand(EMBER_SET_RADIO_POWER_COMMAND_IDENTIFIER,
                                        "s",
                                        power);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberSetRadioPowerReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// getRadioPower
int8_t emberGetRadioPower(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_RADIO_POWER_COMMAND_IDENTIFIER,
                                        "");

  int8_t power;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &power);
  return power;
}
static void emberGetRadioPowerReturn(void)
{
  int8_t power;
  power = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &power);
}

// setRadioPowerMode
EmberStatus emberSetRadioPowerMode(bool radioOn)
{
  emSendBinaryManagementBlockingCommand(EMBER_SET_RADIO_POWER_MODE_COMMAND_IDENTIFIER,
                                        "u",
                                        radioOn);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberSetRadioPowerModeReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// setMacParams
EmberStatus emberSetMacParams(bool checkCca,
                              uint8_t maxCcaAttempts,
                              uint8_t minBackoffExp,
                              uint8_t maxBackoffExp,
                              uint8_t maxRetries)
{
  emSendBinaryManagementBlockingCommand(EMBER_SET_MAC_PARAMS_COMMAND_IDENTIFIER,
                                        "uuuuu",
                                        checkCca,
                                        maxCcaAttempts,
                                        minBackoffExp,
                                        maxBackoffExp,
                                        maxRetries);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberSetMacParamsReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// currentStackTasks
uint16_t emberCurrentStackTasks(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_CURRENT_STACK_TASKS_COMMAND_IDENTIFIER,
                                        "");

  uint16_t currentTasks;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "v",
                    &currentTasks);
  return currentTasks;
}
static void emberCurrentStackTasksReturn(void)
{
  uint16_t currentTasks;
  currentTasks = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "v",
                    &currentTasks);
}

// okToNap
bool emberOkToNap(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_OK_TO_NAP_COMMAND_IDENTIFIER,
                                        "");

  bool isOkToNap;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &isOkToNap);
  return isOkToNap;
}
static void emberOkToNapReturn(void)
{
  bool isOkToNap;
  isOkToNap = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &isOkToNap);
}

// okToHibernate
bool emberOkToHibernate(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_OK_TO_HIBERNATE_COMMAND_IDENTIFIER,
                                        "");

  bool isOkToHibernate;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &isOkToHibernate);
  return isOkToHibernate;
}
static void emberOkToHibernateReturn(void)
{
  bool isOkToHibernate;
  isOkToHibernate = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &isOkToHibernate);
}

// getEui64
uint8_t* emberGetEui64(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_EUI64_COMMAND_IDENTIFIER,
                                        "");

  static uint8_t eui64[EUI64_SIZE];
  uint8_t eui64Size = EUI64_SIZE;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "b",
                    eui64,
                    &eui64Size);
  return &eui64;
}
static void emberGetEui64Return(void)
{
  uint8_t* eui64;
  uint8_t eui64Size;
  eui64 = (uint8_t*)emberStringCommandArgument(0, &(eui64Size));
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "b",
                    eui64,
                    &eui64Size);
}

// isLocalEui64
bool emberIsLocalEui64(EmberEUI64 eui64)
{
  emSendBinaryManagementBlockingCommand(EMBER_IS_LOCAL_EUI64_COMMAND_IDENTIFIER,
                                        "b",
                                        eui64, EUI64_SIZE);

  bool localEui64;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &localEui64);
  return localEui64;
}
static void emberIsLocalEui64Return(void)
{
  bool localEui64;
  localEui64 = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &localEui64);
}

// getNodeId
EmberNodeId emberGetNodeId(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_NODE_ID_COMMAND_IDENTIFIER,
                                        "");

  EmberNodeId nodeId;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "v",
                    &nodeId);
  return nodeId;
}
static void emberGetNodeIdReturn(void)
{
  EmberNodeId nodeId;
  nodeId = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "v",
                    &nodeId);
}

// getPanId
EmberPanId emberGetPanId(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_PAN_ID_COMMAND_IDENTIFIER,
                                        "");

  EmberPanId panId;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "v",
                    &panId);
  return panId;
}
static void emberGetPanIdReturn(void)
{
  EmberPanId panId;
  panId = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "v",
                    &panId);
}

// getParentId
EmberNodeId emberGetParentId(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_PARENT_ID_COMMAND_IDENTIFIER,
                                        "");

  EmberNodeId parentNodeId;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "v",
                    &parentNodeId);
  return parentNodeId;
}
static void emberGetParentIdReturn(void)
{
  EmberNodeId parentNodeId;
  parentNodeId = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "v",
                    &parentNodeId);
}

// getNodeType
EmberNodeType emberGetNodeType(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_NODE_TYPE_COMMAND_IDENTIFIER,
                                        "");

  EmberNodeType nodeType;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &nodeType);
  return nodeType;
}
static void emberGetNodeTypeReturn(void)
{
  EmberNodeType nodeType;
  nodeType = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &nodeType);
}

// getCspVersion
uint16_t emberGetCspVersion(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_CSP_VERSION_COMMAND_IDENTIFIER,
                                        "");

  uint16_t cspVersion;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "v",
                    &cspVersion);
  return cspVersion;
}
static void emberGetCspVersionReturn(void)
{
  uint16_t cspVersion;
  cspVersion = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "v",
                    &cspVersion);
}

// Callback: stackStatusHandler
void fireStackStatusHandlerCallback(void)
{
  EmberStatus status;
  emAfCallbacksFetchOrAppend(TRUE /* FETCH */,
                             "u",
                             &status);
  emberStackStatusHandler(status);
}
static void stackStatusHandlerCallback(void)
{
  uint16_t identifier = CB_STACK_STATUS_COMMAND_IDENTIFIER;
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "vu",
                             &identifier,
                             &status);
}

// Callback: childJoinHandler
void fireChildJoinHandlerCallback(void)
{
  EmberNodeType nodeType;
  EmberNodeId nodeId;
  emAfCallbacksFetchOrAppend(TRUE /* FETCH */,
                             "uv",
                             &nodeType,
                             &nodeId);
  emberChildJoinHandler(nodeType,
                        nodeId);
}
static void childJoinHandlerCallback(void)
{
  uint16_t identifier = CB_CHILD_JOIN_COMMAND_IDENTIFIER;
  EmberNodeType nodeType;
  EmberNodeId nodeId;
  nodeType = emberUnsignedCommandArgument(0);
  nodeId = emberUnsignedCommandArgument(1);
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "vuv",
                             &identifier,
                             &nodeType,
                             &nodeId);
}

//------------------------------------------------------------------------------
// Messaging
//------------------------------------------------------------------------------

// messageSend
EmberStatus emberMessageSend(EmberNodeId destination,
                             uint8_t endpoint,
                             uint8_t messageTag,
                             EmberMessageLength messageLength,
                             uint8_t *message,
                             EmberMessageOptions options)
{
  emSendBinaryManagementBlockingCommand(EMBER_MESSAGE_SEND_COMMAND_IDENTIFIER,
                                        "vuuubu",
                                        destination,
                                        endpoint,
                                        messageTag,
                                        messageLength,
                                        message,
                                        messageLength,
                                        options);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberMessageSendReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// pollForData
EmberStatus emberPollForData(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_POLL_FOR_DATA_COMMAND_IDENTIFIER,
                                        "");

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberPollForDataReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// Callback: messageSentHandler
void fireMessageSentHandlerCallback(void)
{
  EmberStatus status;
  EmberOutgoingMessage message;
  uint8_t payload[EMBER_MAX_UNSECURED_MAC_PAYLOAD_LENGTH];
  message.payload = payload;
  emAfCallbacksFetchOrAppend(TRUE /* FETCH */,
                             "uuvuuub",
                             &status,
                             &message.options,
                             &message.destination,
                             &message.endpoint,
                             &message.tag,
                             &message.length,
                             message.payload,
                             &message.length);
  emberMessageSentHandler(status,
                          &message);
}
static void messageSentHandlerCallback(void)
{
  uint16_t identifier = CB_MESSAGE_SENT_COMMAND_IDENTIFIER;
  EmberStatus status;
  EmberOutgoingMessage message;
  status = emberUnsignedCommandArgument(0);
  message.options = emberUnsignedCommandArgument(1);
  message.destination = emberUnsignedCommandArgument(2);
  message.endpoint = emberUnsignedCommandArgument(3);
  message.tag = emberUnsignedCommandArgument(4);
  message.length = emberUnsignedCommandArgument(5);
  message.payload = (uint8_t *)emberStringCommandArgument(6, &(message.length));
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "vuuvuuub",
                             &identifier,
                             &status,
                             &message.options,
                             &message.destination,
                             &message.endpoint,
                             &message.tag,
                             &message.length,
                             message.payload,
                             &message.length);
}

// Callback: incomingMessageHandler
void fireIncomingMessageHandlerCallback(void)
{
  EmberIncomingMessage message;
  uint8_t payload[EMBER_MAX_UNSECURED_MAC_PAYLOAD_LENGTH];
  message.payload = payload;
  emAfCallbacksFetchOrAppend(TRUE /* FETCH */,
                             "uvuuub",
                             &message.options,
                             &message.source,
                             &message.endpoint,
                             &message.rssi,
                             &message.length,
                             message.payload,
                             &message.length);
  emberIncomingMessageHandler(&message);
}
static void incomingMessageHandlerCallback(void)
{
  uint16_t identifier = CB_INCOMING_MESSAGE_COMMAND_IDENTIFIER;
  EmberIncomingMessage message;
  message.options = emberUnsignedCommandArgument(0);
  message.source = emberUnsignedCommandArgument(1);
  message.endpoint = emberUnsignedCommandArgument(2);
  message.rssi = emberUnsignedCommandArgument(3);
  message.length = emberUnsignedCommandArgument(4);
  message.payload = (uint8_t *)emberStringCommandArgument(5, &(message.length));
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "vuvuuub",
                             &identifier,
                             &message.options,
                             &message.source,
                             &message.endpoint,
                             &message.rssi,
                             &message.length,
                             message.payload,
                             &message.length);
}

//------------------------------------------------------------------------------
// Network Management
//------------------------------------------------------------------------------

// networkInit
EmberStatus emberNetworkInit(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_NETWORK_INIT_COMMAND_IDENTIFIER,
                                        "");

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberNetworkInitReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// startActiveScan
EmberStatus emberStartActiveScan(uint8_t channel)
{
  emSendBinaryManagementBlockingCommand(EMBER_START_ACTIVE_SCAN_COMMAND_IDENTIFIER,
                                        "u",
                                        channel);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberStartActiveScanReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// startEnergyScan
EmberStatus emberStartEnergyScan(uint8_t channel,
                                 uint8_t samples)
{
  emSendBinaryManagementBlockingCommand(EMBER_START_ENERGY_SCAN_COMMAND_IDENTIFIER,
                                        "uu",
                                        channel,
                                        samples);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberStartEnergyScanReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// setApplicationBeaconPayload
EmberStatus emberSetApplicationBeaconPayload(uint8_t payloadLength,
                                             uint8_t *payload)
{
  emSendBinaryManagementBlockingCommand(EMBER_SET_APPLICATION_BEACON_PAYLOAD_COMMAND_IDENTIFIER,
                                        "b",
                                        payload, payloadLength);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberSetApplicationBeaconPayloadReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// formNetwork
EmberStatus emberFormNetwork(EmberNetworkParameters *parameters)
{
  emSendBinaryManagementBlockingCommand(EMBER_FORM_NETWORK_COMMAND_IDENTIFIER,
                                        "vuu",
                                        parameters->panId,
                                        parameters->radioTxPower,
                                        parameters->radioChannel);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberFormNetworkReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// joinNetworkExtended
EmberStatus emberJoinNetworkExtended(EmberNodeType nodeType,
                                     EmberNodeId nodeId,
                                     EmberNetworkParameters *parameters)
{
  emSendBinaryManagementBlockingCommand(EMBER_JOIN_NETWORK_EXTENDED_COMMAND_IDENTIFIER,
                                        "uvvuu",
                                        nodeType,
                                        nodeId,
                                        parameters->panId,
                                        parameters->radioTxPower,
                                        parameters->radioChannel);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberJoinNetworkExtendedReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// joinNetwork
EmberStatus emberJoinNetwork(EmberNodeType nodeType,
                             EmberNetworkParameters *parameters)
{
  emSendBinaryManagementBlockingCommand(EMBER_JOIN_NETWORK_COMMAND_IDENTIFIER,
                                        "uvuu",
                                        nodeType,
                                        parameters->panId,
                                        parameters->radioTxPower,
                                        parameters->radioChannel);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberJoinNetworkReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// permitJoining
EmberStatus emberPermitJoining(uint8_t duration)
{
  emSendBinaryManagementBlockingCommand(EMBER_PERMIT_JOINING_COMMAND_IDENTIFIER,
                                        "u",
                                        duration);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberPermitJoiningReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// joinCommissioned
EmberStatus emberJoinCommissioned(EmberNodeType nodeType,
                                  EmberNodeId nodeId,
                                  EmberNetworkParameters *parameters)
{
  emSendBinaryManagementBlockingCommand(EMBER_JOIN_COMMISSIONED_COMMAND_IDENTIFIER,
                                        "uvvuu",
                                        nodeType,
                                        nodeId,
                                        parameters->panId,
                                        parameters->radioTxPower,
                                        parameters->radioChannel);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberJoinCommissionedReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// resetNetworkState
void emberResetNetworkState(void)
{
  emSendBinaryManagementCommand(EMBER_RESET_NETWORK_STATE_COMMAND_IDENTIFIER,
                                "");
}

// getStandaloneBootloaderInfo
void emberGetStandaloneBootloaderInfo(void)
{
  emSendBinaryManagementCommand(EMBER_GET_STANDALONE_BOOTLOADER_INFO_COMMAND_IDENTIFIER,
                                "");
}

// launchStandaloneBootloader
void emberLaunchStandaloneBootloader(uint8_t mode)
{
  emSendBinaryManagementCommand(EMBER_LAUNCH_STANDALONE_BOOTLOADER_COMMAND_IDENTIFIER,
                                "u",
                                mode);
}

// frequencyHoppingStartServer
EmberStatus emberFrequencyHoppingStartServer(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_FREQUENCY_HOPPING_START_SERVER_COMMAND_IDENTIFIER,
                                        "");

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberFrequencyHoppingStartServerReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// frequencyHoppingStartClient
EmberStatus emberFrequencyHoppingStartClient(EmberNodeId serverNodeId,
                                             EmberPanId serverPanId)
{
  emSendBinaryManagementBlockingCommand(EMBER_FREQUENCY_HOPPING_START_CLIENT_COMMAND_IDENTIFIER,
                                        "vv",
                                        serverNodeId,
                                        serverPanId);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberFrequencyHoppingStartClientReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// frequencyHoppingStop
EmberStatus emberFrequencyHoppingStop(void)
{
  emSendBinaryManagementBlockingCommand(EMBER_FREQUENCY_HOPPING_STOP_COMMAND_IDENTIFIER,
                                        "");

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberFrequencyHoppingStopReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// setAuxiliaryAddressFilteringEntry
EmberStatus emberSetAuxiliaryAddressFilteringEntry(EmberNodeId nodeId,
                                                   uint8_t entryIndex)
{
  emSendBinaryManagementBlockingCommand(EMBER_SET_AUXILIARY_ADDRESS_FILTERING_ENTRY_COMMAND_IDENTIFIER,
                                        "vu",
                                        nodeId,
                                        entryIndex);

  EmberStatus status;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "u",
                    &status);
  return status;
}
static void emberSetAuxiliaryAddressFilteringEntryReturn(void)
{
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "u",
                    &status);
}

// getAuxiliaryAddressFilteringEntry
EmberNodeId emberGetAuxiliaryAddressFilteringEntry(uint8_t entryIndex)
{
  emSendBinaryManagementBlockingCommand(EMBER_GET_AUXILIARY_ADDRESS_FILTERING_ENTRY_COMMAND_IDENTIFIER,
                                        "u",
                                        entryIndex);

  EmberNodeId nodeId;
  emAfFetchOrAppend(TRUE /* FETCH */,
                    "v",
                    &nodeId);
  return nodeId;
}
static void emberGetAuxiliaryAddressFilteringEntryReturn(void)
{
  EmberNodeId nodeId;
  nodeId = emberUnsignedCommandArgument(0);
  emAfFetchOrAppend(FALSE /* APPEND */,
                    "v",
                    &nodeId);
}

// Callback: incomingBeaconHandler
void fireIncomingBeaconHandlerCallback(void)
{
  EmberPanId panId;
  EmberNodeId nodeId;
  uint8_t payloadLength;
  static uint8_t payload[EMBER_MAC_MAX_APP_BEACON_PAYLOAD_LENGTH];
  emAfCallbacksFetchOrAppend(TRUE /* FETCH */,
                             "vvub",
                             &panId,
                             &nodeId,
                             &payloadLength,
                             payload,
                             &payloadLength);
  emberIncomingBeaconHandler(panId,
                             nodeId,
                             payloadLength,
                             payload);
}
static void incomingBeaconHandlerCallback(void)
{
  uint16_t identifier = CB_INCOMING_BEACON_COMMAND_IDENTIFIER;
  EmberPanId panId;
  EmberNodeId nodeId;
  uint8_t payloadLength;
  uint8_t *payload;
  panId = emberUnsignedCommandArgument(0);
  nodeId = emberUnsignedCommandArgument(1);
  payloadLength = emberUnsignedCommandArgument(2);
  payload = (uint8_t *)emberStringCommandArgument(3, &(payloadLength));
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "vvvub",
                             &identifier,
                             &panId,
                             &nodeId,
                             &payloadLength,
                             payload,
                             &payloadLength);
}

// Callback: activeScanCompleteHandler
void fireActiveScanCompleteHandlerCallback(void)
{
  emberActiveScanCompleteHandler();
}
static void activeScanCompleteHandlerCallback(void)
{
  uint16_t identifier = CB_ACTIVE_SCAN_COMPLETE_COMMAND_IDENTIFIER;
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "v",
                             &identifier);
}

// Callback: energyScanCompleteHandler
void fireEnergyScanCompleteHandlerCallback(void)
{
  int8_t mean;
  int8_t min;
  int8_t max;
  uint16_t variance;
  emAfCallbacksFetchOrAppend(TRUE /* FETCH */,
                             "uuuv",
                             &mean,
                             &min,
                             &max,
                             &variance);
  emberEnergyScanCompleteHandler(mean,
                                 min,
                                 max,
                                 variance);
}
static void energyScanCompleteHandlerCallback(void)
{
  uint16_t identifier = CB_ENERGY_SCAN_COMPLETE_COMMAND_IDENTIFIER;
  int8_t mean;
  int8_t min;
  int8_t max;
  uint16_t variance;
  mean = emberUnsignedCommandArgument(0);
  min = emberUnsignedCommandArgument(1);
  max = emberUnsignedCommandArgument(2);
  variance = emberUnsignedCommandArgument(3);
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "vuuuv",
                             &identifier,
                             &mean,
                             &min,
                             &max,
                             &variance);
}

// Callback: getStandaloneBootloaderInfoReturn
void fireGetStandaloneBootloaderInfoReturnCallback(void)
{
  uint16_t version;
  uint8_t platformId;
  uint8_t microId;
  uint8_t phyId;
  emAfCallbacksFetchOrAppend(TRUE /* FETCH */,
                             "vuuu",
                             &version,
                             &platformId,
                             &microId,
                             &phyId);
  emberGetStandaloneBootloaderInfoReturn(version,
                                         platformId,
                                         microId,
                                         phyId);
}
static void getStandaloneBootloaderInfoReturnCallback(void)
{
  uint16_t identifier = CB_GET_STANDALONE_BOOTLOADER_INFO_COMMAND_IDENTIFIER;
  uint16_t version;
  uint8_t platformId;
  uint8_t microId;
  uint8_t phyId;
  version = emberUnsignedCommandArgument(0);
  platformId = emberUnsignedCommandArgument(1);
  microId = emberUnsignedCommandArgument(2);
  phyId = emberUnsignedCommandArgument(3);
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "vvuuu",
                             &identifier,
                             &version,
                             &platformId,
                             &microId,
                             &phyId);
}

// Callback: launchStandaloneBootloaderReturn
void fireLaunchStandaloneBootloaderReturnCallback(void)
{
  EmberStatus status;
  emAfCallbacksFetchOrAppend(TRUE /* FETCH */,
                             "u",
                             &status);
  emberLaunchStandaloneBootloaderReturn(status);
}
static void launchStandaloneBootloaderReturnCallback(void)
{
  uint16_t identifier = CB_LAUNCH_STANDALONE_BOOTLOADER_COMMAND_IDENTIFIER;
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "vu",
                             &identifier,
                             &status);
}

// Callback: frequencyHoppingStartClientCompleteHandler
void fireFrequencyHoppingStartClientCompleteHandlerCallback(void)
{
  EmberStatus status;
  emAfCallbacksFetchOrAppend(TRUE /* FETCH */,
                             "u",
                             &status);
  emberFrequencyHoppingStartClientCompleteHandler(status);
}
static void frequencyHoppingStartClientCompleteHandlerCallback(void)
{
  uint16_t identifier = CB_FREQUENCY_HOPPING_START_CLIENT_COMPLETE_COMMAND_IDENTIFIER;
  EmberStatus status;
  status = emberUnsignedCommandArgument(0);
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "vu",
                             &identifier,
                             &status);
}

//------------------------------------------------------------------------------
// EMBER_TEST
//------------------------------------------------------------------------------

// echo
void emberEcho(uint8_t *data,
               uint8_t length)
{
  emSendBinaryManagementCommand(EMBER_ECHO_COMMAND_IDENTIFIER,
                                "b",
                                data, length);
}

// Callback: echoReturn
void fireEchoReturnCallback(void)
{
  uint8_t length;
  static uint8_t data[EMBER_MAX_UNSECURED_MAC_PAYLOAD_LENGTH];
  emAfCallbacksFetchOrAppend(TRUE /* FETCH */,
                             "b",
                             data,
                             &length);
  emberEchoReturn(data,
                  length);
}
static void echoReturnCallback(void)
{
  uint16_t identifier = CB_ECHO_COMMAND_IDENTIFIER;
  uint8_t *data;
  uint8_t length;
  data = (uint8_t *)emberStringCommandArgument(0, &(length));
  emAfCallbacksFetchOrAppend(FALSE /* APPEND */,
                             "vb",
                             &identifier,
                             data,
                             &length);
}

//------------------------------------------------------------------------------
// APP_USES_SOFTWARE_FLOW_CONTROL
//------------------------------------------------------------------------------
#ifdef EMBER_APPLICATION_USES_SOFTWARE_FLOW_CONTROL

// startXOnXOffTest
void emberStartXOnXOffTest(void)
{
  emSendBinaryManagementCommand(EMBER_START_XON_XOFF_TEST_COMMAND_IDENTIFIER,
                                "");
}
#endif /* EMBER_APPLICATION_USES_SOFTWARE_FLOW_CONTROL */

//------------------------------------------------------------------------------
// Command Handler Table

const EmberCommandEntry managementCallbackCommandTable[] = {
  emberBinaryCommandEntryAction(EMBER_SET_PHY_MODE_RETURN_IDENTIFIER,
                                emberSetPhyModeReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_PHY_MODE_RETURN_IDENTIFIER,
                                emberGetPhyModeReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_NETWORK_STATE_RETURN_IDENTIFIER,
                                emberNetworkStateReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_STACK_IS_UP_RETURN_IDENTIFIER,
                                emberStackIsUpReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_SECURITY_KEY_RETURN_IDENTIFIER,
                                emberSetSecurityKeyReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_COUNTER_RETURN_IDENTIFIER,
                                emberGetCounterReturn,
                                "uw",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_RADIO_CHANNEL_RETURN_IDENTIFIER,
                                emberSetRadioChannelReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_RADIO_CHANNEL_RETURN_IDENTIFIER,
                                emberGetRadioChannelReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_RADIO_POWER_RETURN_IDENTIFIER,
                                emberSetRadioPowerReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_RADIO_POWER_RETURN_IDENTIFIER,
                                emberGetRadioPowerReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_RADIO_POWER_MODE_RETURN_IDENTIFIER,
                                emberSetRadioPowerModeReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_MAC_PARAMS_RETURN_IDENTIFIER,
                                emberSetMacParamsReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_CURRENT_STACK_TASKS_RETURN_IDENTIFIER,
                                emberCurrentStackTasksReturn,
                                "v",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_OK_TO_NAP_RETURN_IDENTIFIER,
                                emberOkToNapReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_OK_TO_HIBERNATE_RETURN_IDENTIFIER,
                                emberOkToHibernateReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_EUI64_RETURN_IDENTIFIER,
                                emberGetEui64Return,
                                "b",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_IS_LOCAL_EUI64_RETURN_IDENTIFIER,
                                emberIsLocalEui64Return,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_NODE_ID_RETURN_IDENTIFIER,
                                emberGetNodeIdReturn,
                                "v",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_PAN_ID_RETURN_IDENTIFIER,
                                emberGetPanIdReturn,
                                "v",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_PARENT_ID_RETURN_IDENTIFIER,
                                emberGetParentIdReturn,
                                "v",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_NODE_TYPE_RETURN_IDENTIFIER,
                                emberGetNodeTypeReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_CSP_VERSION_RETURN_IDENTIFIER,
                                emberGetCspVersionReturn,
                                "v",
                                NULL),
  emberBinaryCommandEntryAction(CB_STACK_STATUS_COMMAND_IDENTIFIER,
                                stackStatusHandlerCallback,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(CB_CHILD_JOIN_COMMAND_IDENTIFIER,
                                childJoinHandlerCallback,
                                "uv",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_MESSAGE_SEND_RETURN_IDENTIFIER,
                                emberMessageSendReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_POLL_FOR_DATA_RETURN_IDENTIFIER,
                                emberPollForDataReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(CB_MESSAGE_SENT_COMMAND_IDENTIFIER,
                                messageSentHandlerCallback,
                                "uuvuuub",
                                NULL),
  emberBinaryCommandEntryAction(CB_INCOMING_MESSAGE_COMMAND_IDENTIFIER,
                                incomingMessageHandlerCallback,
                                "uvuuub",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_NETWORK_INIT_RETURN_IDENTIFIER,
                                emberNetworkInitReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_START_ACTIVE_SCAN_RETURN_IDENTIFIER,
                                emberStartActiveScanReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_START_ENERGY_SCAN_RETURN_IDENTIFIER,
                                emberStartEnergyScanReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_APPLICATION_BEACON_PAYLOAD_RETURN_IDENTIFIER,
                                emberSetApplicationBeaconPayloadReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_FORM_NETWORK_RETURN_IDENTIFIER,
                                emberFormNetworkReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_JOIN_NETWORK_EXTENDED_RETURN_IDENTIFIER,
                                emberJoinNetworkExtendedReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_JOIN_NETWORK_RETURN_IDENTIFIER,
                                emberJoinNetworkReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_PERMIT_JOINING_RETURN_IDENTIFIER,
                                emberPermitJoiningReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_JOIN_COMMISSIONED_RETURN_IDENTIFIER,
                                emberJoinCommissionedReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_FREQUENCY_HOPPING_START_SERVER_RETURN_IDENTIFIER,
                                emberFrequencyHoppingStartServerReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_FREQUENCY_HOPPING_START_CLIENT_RETURN_IDENTIFIER,
                                emberFrequencyHoppingStartClientReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_FREQUENCY_HOPPING_STOP_RETURN_IDENTIFIER,
                                emberFrequencyHoppingStopReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_AUXILIARY_ADDRESS_FILTERING_ENTRY_RETURN_IDENTIFIER,
                                emberSetAuxiliaryAddressFilteringEntryReturn,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_AUXILIARY_ADDRESS_FILTERING_ENTRY_RETURN_IDENTIFIER,
                                emberGetAuxiliaryAddressFilteringEntryReturn,
                                "v",
                                NULL),
  emberBinaryCommandEntryAction(CB_INCOMING_BEACON_COMMAND_IDENTIFIER,
                                incomingBeaconHandlerCallback,
                                "vvub",
                                NULL),
  emberBinaryCommandEntryAction(CB_ACTIVE_SCAN_COMPLETE_COMMAND_IDENTIFIER,
                                activeScanCompleteHandlerCallback,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(CB_ENERGY_SCAN_COMPLETE_COMMAND_IDENTIFIER,
                                energyScanCompleteHandlerCallback,
                                "uuuv",
                                NULL),
  emberBinaryCommandEntryAction(CB_GET_STANDALONE_BOOTLOADER_INFO_COMMAND_IDENTIFIER,
                                getStandaloneBootloaderInfoReturnCallback,
                                "vuuu",
                                NULL),
  emberBinaryCommandEntryAction(CB_LAUNCH_STANDALONE_BOOTLOADER_COMMAND_IDENTIFIER,
                                launchStandaloneBootloaderReturnCallback,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(CB_FREQUENCY_HOPPING_START_CLIENT_COMPLETE_COMMAND_IDENTIFIER,
                                frequencyHoppingStartClientCompleteHandlerCallback,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(CB_ECHO_COMMAND_IDENTIFIER,
                                echoReturnCallback,
                                "b",
                                NULL),
#ifdef EMBER_APPLICATION_USES_SOFTWARE_FLOW_CONTROL
#endif /* EMBER_APPLICATION_USES_SOFTWARE_FLOW_CONTROL */
  { NULL }    // terminator
};
