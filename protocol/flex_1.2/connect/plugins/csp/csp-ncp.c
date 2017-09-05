/**************************************************************************//**
 * Copyright 2017 Silicon Laboratories, Inc.
 *
 *****************************************************************************/
//
// *** Generated file. Do not edit! ***
//
// Description: NCP functions for sending Connect management commands
// to the Host.
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
#include "csp/csp-frame-utilities.h"
#include "csp/csp-ncp-utilities.h"

//------------------------------------------------------------------------------
// Core
//------------------------------------------------------------------------------

// setPhyMode
static void ncpSetPhyModeCommand(void)
{
  uint8_t mode;
  mode = (uint8_t) emberUnsignedCommandArgument(0);
  EmberStatus status = emberSetPhyMode(mode);
  emSendBinaryManagementCommand(EMBER_SET_PHY_MODE_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// getPhyMode
static void ncpGetPhyModeCommand(void)
{
  uint8_t mode = emberGetPhyMode();
  emSendBinaryManagementCommand(EMBER_GET_PHY_MODE_RETURN_IDENTIFIER,
                                "u",
                                mode);
}

// networkState
static void ncpNetworkStateCommand(void)
{
  EmberNetworkStatus networkStatus = emberNetworkState();
  emSendBinaryManagementCommand(EMBER_NETWORK_STATE_RETURN_IDENTIFIER,
                                "u",
                                networkStatus);
}

// stackIsUp
static void ncpStackIsUpCommand(void)
{
  bool isStackUp = emberStackIsUp();
  emSendBinaryManagementCommand(EMBER_STACK_IS_UP_RETURN_IDENTIFIER,
                                "u",
                                isStackUp);
}

// setSecurityKey
static void ncpSetSecurityKeyCommand(void)
{
  EmberKeyData key;
  MEMSET(&key, 0, sizeof(EmberKeyData));
  emberKeyDataCommandArgument(0, &key);
  EmberStatus status = emberSetSecurityKey(&key);
  emSendBinaryManagementCommand(EMBER_SET_SECURITY_KEY_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// getCounter
static void ncpGetCounterCommand(void)
{
  EmberCounterType counterType;
  uint32_t count;
  counterType = (EmberCounterType) emberUnsignedCommandArgument(0);
  count = (uint32_t) emberUnsignedCommandArgument(1);
  EmberStatus status = emberGetCounter(counterType,
                                       &count);
  emSendBinaryManagementCommand(EMBER_GET_COUNTER_RETURN_IDENTIFIER,
                                "uw",
                                status,
                                count);
}

// setRadioChannel
static void ncpSetRadioChannelCommand(void)
{
  uint8_t channel;
  channel = (uint8_t) emberUnsignedCommandArgument(0);
  EmberStatus status = emberSetRadioChannel(channel);
  emSendBinaryManagementCommand(EMBER_SET_RADIO_CHANNEL_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// getRadioChannel
static void ncpGetRadioChannelCommand(void)
{
  uint8_t channel = emberGetRadioChannel();
  emSendBinaryManagementCommand(EMBER_GET_RADIO_CHANNEL_RETURN_IDENTIFIER,
                                "u",
                                channel);
}

// setRadioPower
static void ncpSetRadioPowerCommand(void)
{
  int8_t power;
  power = (int8_t) emberSignedCommandArgument(0);
  EmberStatus status = emberSetRadioPower(power);
  emSendBinaryManagementCommand(EMBER_SET_RADIO_POWER_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// getRadioPower
static void ncpGetRadioPowerCommand(void)
{
  int8_t power = emberGetRadioPower();
  emSendBinaryManagementCommand(EMBER_GET_RADIO_POWER_RETURN_IDENTIFIER,
                                "u",
                                power);
}

// setRadioPowerMode
static void ncpSetRadioPowerModeCommand(void)
{
  bool radioOn;
  radioOn = (bool) emberUnsignedCommandArgument(0);
  EmberStatus status = emberSetRadioPowerMode(radioOn);
  emSendBinaryManagementCommand(EMBER_SET_RADIO_POWER_MODE_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// setMacParams
static void ncpSetMacParamsCommand(void)
{
  bool checkCca;
  uint8_t maxCcaAttempts;
  uint8_t minBackoffExp;
  uint8_t maxBackoffExp;
  uint8_t maxRetries;
  checkCca = (bool) emberUnsignedCommandArgument(0);
  maxCcaAttempts = (uint8_t) emberUnsignedCommandArgument(1);
  minBackoffExp = (uint8_t) emberUnsignedCommandArgument(2);
  maxBackoffExp = (uint8_t) emberUnsignedCommandArgument(3);
  maxRetries = (uint8_t) emberUnsignedCommandArgument(4);
  EmberStatus status = emberSetMacParams(checkCca,
                                         maxCcaAttempts,
                                         minBackoffExp,
                                         maxBackoffExp,
                                         maxRetries);
  emSendBinaryManagementCommand(EMBER_SET_MAC_PARAMS_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// currentStackTasks
static void ncpCurrentStackTasksCommand(void)
{
  uint16_t currentTasks = emberCurrentStackTasks();
  emSendBinaryManagementCommand(EMBER_CURRENT_STACK_TASKS_RETURN_IDENTIFIER,
                                "v",
                                currentTasks);
}

// okToNap
static void ncpOkToNapCommand(void)
{
  bool isOkToNap = emberOkToNap();
  emSendBinaryManagementCommand(EMBER_OK_TO_NAP_RETURN_IDENTIFIER,
                                "u",
                                isOkToNap);
}

// okToHibernate
static void ncpOkToHibernateCommand(void)
{
  bool isOkToHibernate = emberOkToHibernate();
  emSendBinaryManagementCommand(EMBER_OK_TO_HIBERNATE_RETURN_IDENTIFIER,
                                "u",
                                isOkToHibernate);
}

// getEui64
static void ncpGetEui64Command(void)
{
  uint8_t* eui64 = emberGetEui64();
  emSendBinaryManagementCommand(EMBER_GET_EUI64_RETURN_IDENTIFIER,
                                "b",
                                eui64,
                                EUI64_SIZE);
}

// isLocalEui64
static void ncpIsLocalEui64Command(void)
{
  uint8_t eui64[EUI64_SIZE];
  emberGetStringArgument(0, eui64, EUI64_SIZE, false);
  bool localEui64 = emberIsLocalEui64(eui64);
  emSendBinaryManagementCommand(EMBER_IS_LOCAL_EUI64_RETURN_IDENTIFIER,
                                "u",
                                localEui64);
}

// getNodeId
static void ncpGetNodeIdCommand(void)
{
  EmberNodeId nodeId = emberGetNodeId();
  emSendBinaryManagementCommand(EMBER_GET_NODE_ID_RETURN_IDENTIFIER,
                                "v",
                                nodeId);
}

// getPanId
static void ncpGetPanIdCommand(void)
{
  EmberPanId panId = emberGetPanId();
  emSendBinaryManagementCommand(EMBER_GET_PAN_ID_RETURN_IDENTIFIER,
                                "v",
                                panId);
}

// getParentId
static void ncpGetParentIdCommand(void)
{
  EmberNodeId parentNodeId = emberGetParentId();
  emSendBinaryManagementCommand(EMBER_GET_PARENT_ID_RETURN_IDENTIFIER,
                                "v",
                                parentNodeId);
}

// getNodeType
static void ncpGetNodeTypeCommand(void)
{
  EmberNodeType nodeType = emberGetNodeType();
  emSendBinaryManagementCommand(EMBER_GET_NODE_TYPE_RETURN_IDENTIFIER,
                                "u",
                                nodeType);
}

// getCspVersion
static void ncpGetCspVersionCommand(void)
{
  uint16_t cspVersion = emberGetCspVersion();
  emSendBinaryManagementCommand(EMBER_GET_CSP_VERSION_RETURN_IDENTIFIER,
                                "v",
                                cspVersion);
}

// stackStatusHandler
void emberStackStatusHandler(EmberStatus status)
{
  emSendBinaryManagementCommand(CB_STACK_STATUS_COMMAND_IDENTIFIER,
                                "u",
                                status);
}

// childJoinHandler
void emberChildJoinHandler(EmberNodeType nodeType,
                           EmberNodeId nodeId)
{
  emSendBinaryManagementCommand(CB_CHILD_JOIN_COMMAND_IDENTIFIER,
                                "uv",
                                nodeType,
                                nodeId);
}

//------------------------------------------------------------------------------
// Messaging
//------------------------------------------------------------------------------

// messageSend
static void ncpMessageSendCommand(void)
{
  EmberNodeId destination;
  uint8_t endpoint;
  uint8_t messageTag;
  EmberMessageLength messageLength;
  uint8_t *message;
  EmberMessageOptions options;
  destination = emberUnsignedCommandArgument(0);
  endpoint = emberUnsignedCommandArgument(1);
  messageTag = emberUnsignedCommandArgument(2);
  messageLength = emberUnsignedCommandArgument(3);
  message = (uint8_t *)emberStringCommandArgument(4, &(messageLength));
  options = emberUnsignedCommandArgument(5);
  EmberStatus status = emberMessageSend(destination,
                                        endpoint,
                                        messageTag,
                                        messageLength,
                                        message,
                                        options);
  emSendBinaryManagementCommand(EMBER_MESSAGE_SEND_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// pollForData
static void ncpPollForDataCommand(void)
{
  EmberStatus status = emberPollForData();
  emSendBinaryManagementCommand(EMBER_POLL_FOR_DATA_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// messageSentHandler
void emberMessageSentHandler(EmberStatus status,
                             EmberOutgoingMessage *message)
{
  emSendBinaryManagementCommand(CB_MESSAGE_SENT_COMMAND_IDENTIFIER,
                                "uuvuuub",
                                status,
                                message->options,
                                message->destination,
                                message->endpoint,
                                message->tag,
                                message->length,
                                message->payload,
                                message->length);
}

// incomingMessageHandler
void emberIncomingMessageHandler(EmberIncomingMessage *message)
{
  emSendBinaryManagementCommand(CB_INCOMING_MESSAGE_COMMAND_IDENTIFIER,
                                "uvuuub",
                                message->options,
                                message->source,
                                message->endpoint,
                                message->rssi,
                                message->length,
                                message->payload,
                                message->length);
}

//------------------------------------------------------------------------------
// Network Management
//------------------------------------------------------------------------------

// networkInit
static void ncpNetworkInitCommand(void)
{
  EmberStatus status = emberNetworkInit();
  emSendBinaryManagementCommand(EMBER_NETWORK_INIT_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// startActiveScan
static void ncpStartActiveScanCommand(void)
{
  uint8_t channel;
  channel = (uint8_t) emberUnsignedCommandArgument(0);
  EmberStatus status = emberStartActiveScan(channel);
  emSendBinaryManagementCommand(EMBER_START_ACTIVE_SCAN_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// startEnergyScan
static void ncpStartEnergyScanCommand(void)
{
  uint8_t channel;
  uint8_t samples;
  channel = (uint8_t) emberUnsignedCommandArgument(0);
  samples = (uint8_t) emberUnsignedCommandArgument(1);
  EmberStatus status = emberStartEnergyScan(channel,
                                            samples);
  emSendBinaryManagementCommand(EMBER_START_ENERGY_SCAN_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// setApplicationBeaconPayload
static void ncpSetApplicationBeaconPayloadCommand(void)
{
  uint8_t payloadLength;
  uint8_t *payload;
  payload = emberStringCommandArgument(0, &payloadLength);
  EmberStatus status = emberSetApplicationBeaconPayload(payloadLength,
                                                        payload);
  emSendBinaryManagementCommand(EMBER_SET_APPLICATION_BEACON_PAYLOAD_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// formNetwork
static void ncpFormNetworkCommand(void)
{
  EmberNetworkParameters parameters;
  MEMSET(&parameters, 0, sizeof(EmberNetworkParameters));
  parameters.panId = emberUnsignedCommandArgument(0);
  parameters.radioTxPower = emberUnsignedCommandArgument(1);
  parameters.radioChannel = emberUnsignedCommandArgument(2);
  EmberStatus status = emberFormNetwork(&parameters);
  emSendBinaryManagementCommand(EMBER_FORM_NETWORK_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// joinNetworkExtended
static void ncpJoinNetworkExtendedCommand(void)
{
  EmberNodeType nodeType;
  EmberNodeId nodeId;
  EmberNetworkParameters parameters;
  MEMSET(&parameters, 0, sizeof(EmberNetworkParameters));
  nodeType = emberUnsignedCommandArgument(0);
  nodeId = emberUnsignedCommandArgument(1);
  parameters.panId = emberUnsignedCommandArgument(2);
  parameters.radioTxPower = emberUnsignedCommandArgument(3);
  parameters.radioChannel = emberUnsignedCommandArgument(4);
  EmberStatus status = emberJoinNetworkExtended(nodeType,
                                                nodeId,
                                                &parameters);
  emSendBinaryManagementCommand(EMBER_JOIN_NETWORK_EXTENDED_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// joinNetwork
static void ncpJoinNetworkCommand(void)
{
  EmberNodeType nodeType;
  EmberNetworkParameters parameters;
  MEMSET(&parameters, 0, sizeof(EmberNetworkParameters));
  nodeType = emberUnsignedCommandArgument(0);
  parameters.panId = emberUnsignedCommandArgument(1);
  parameters.radioTxPower = emberUnsignedCommandArgument(2);
  parameters.radioChannel = emberUnsignedCommandArgument(3);
  EmberStatus status = emberJoinNetwork(nodeType,
                                        &parameters);
  emSendBinaryManagementCommand(EMBER_JOIN_NETWORK_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// permitJoining
static void ncpPermitJoiningCommand(void)
{
  uint8_t duration;
  duration = (uint8_t) emberUnsignedCommandArgument(0);
  EmberStatus status = emberPermitJoining(duration);
  emSendBinaryManagementCommand(EMBER_PERMIT_JOINING_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// joinCommissioned
static void ncpJoinCommissionedCommand(void)
{
  EmberNodeType nodeType;
  EmberNodeId nodeId;
  EmberNetworkParameters parameters;
  MEMSET(&parameters, 0, sizeof(EmberNetworkParameters));
  nodeType = emberUnsignedCommandArgument(0);
  nodeId = emberUnsignedCommandArgument(1);
  parameters.panId = emberUnsignedCommandArgument(2);
  parameters.radioTxPower = emberUnsignedCommandArgument(3);
  parameters.radioChannel = emberUnsignedCommandArgument(4);
  EmberStatus status = emberJoinCommissioned(nodeType,
                                             nodeId,
                                             &parameters);
  emSendBinaryManagementCommand(EMBER_JOIN_COMMISSIONED_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// resetNetworkState
static void ncpResetNetworkStateCommand(void)
{
  emberResetNetworkState();
}

// getStandaloneBootloaderInfo
static void ncpGetStandaloneBootloaderInfoCommand(void)
{
  emberGetStandaloneBootloaderInfo();
}

// launchStandaloneBootloader
static void ncpLaunchStandaloneBootloaderCommand(void)
{
  uint8_t mode;
  mode = (uint8_t) emberUnsignedCommandArgument(0);
  bool preHookResult = cspNcpLaunchStandaloneBootloaderPreHook(mode);
  if (preHookResult) {
    emberLaunchStandaloneBootloader(mode);
  }
}

// frequencyHoppingStartServer
static void ncpFrequencyHoppingStartServerCommand(void)
{
  EmberStatus status = emberFrequencyHoppingStartServer();
  emSendBinaryManagementCommand(EMBER_FREQUENCY_HOPPING_START_SERVER_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// frequencyHoppingStartClient
static void ncpFrequencyHoppingStartClientCommand(void)
{
  EmberNodeId serverNodeId;
  EmberPanId serverPanId;
  serverNodeId = (EmberNodeId) emberUnsignedCommandArgument(0);
  serverPanId = (EmberPanId) emberUnsignedCommandArgument(1);
  EmberStatus status = emberFrequencyHoppingStartClient(serverNodeId,
                                                        serverPanId);
  emSendBinaryManagementCommand(EMBER_FREQUENCY_HOPPING_START_CLIENT_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// frequencyHoppingStop
static void ncpFrequencyHoppingStopCommand(void)
{
  EmberStatus status = emberFrequencyHoppingStop();
  emSendBinaryManagementCommand(EMBER_FREQUENCY_HOPPING_STOP_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// setAuxiliaryAddressFilteringEntry
static void ncpSetAuxiliaryAddressFilteringEntryCommand(void)
{
  EmberNodeId nodeId;
  uint8_t entryIndex;
  nodeId = (EmberNodeId) emberUnsignedCommandArgument(0);
  entryIndex = (uint8_t) emberUnsignedCommandArgument(1);
  EmberStatus status = emberSetAuxiliaryAddressFilteringEntry(nodeId,
                                                              entryIndex);
  emSendBinaryManagementCommand(EMBER_SET_AUXILIARY_ADDRESS_FILTERING_ENTRY_RETURN_IDENTIFIER,
                                "u",
                                status);
}

// getAuxiliaryAddressFilteringEntry
static void ncpGetAuxiliaryAddressFilteringEntryCommand(void)
{
  uint8_t entryIndex;
  entryIndex = (uint8_t) emberUnsignedCommandArgument(0);
  EmberNodeId nodeId = emberGetAuxiliaryAddressFilteringEntry(entryIndex);
  emSendBinaryManagementCommand(EMBER_GET_AUXILIARY_ADDRESS_FILTERING_ENTRY_RETURN_IDENTIFIER,
                                "v",
                                nodeId);
}

// incomingBeaconHandler
void emberIncomingBeaconHandler(EmberPanId panId,
                                EmberNodeId nodeId,
                                uint8_t payloadLength,
                                uint8_t *payload)
{
  emSendBinaryManagementCommand(CB_INCOMING_BEACON_COMMAND_IDENTIFIER,
                                "vvub",
                                panId,
                                nodeId,
                                payloadLength,
                                payload,
                                payloadLength);
}

// activeScanCompleteHandler
void emberActiveScanCompleteHandler(void)
{
  emSendBinaryManagementCommand(CB_ACTIVE_SCAN_COMPLETE_COMMAND_IDENTIFIER,
                                "");
}

// energyScanCompleteHandler
void emberEnergyScanCompleteHandler(int8_t mean,
                                    int8_t min,
                                    int8_t max,
                                    uint16_t variance)
{
  emSendBinaryManagementCommand(CB_ENERGY_SCAN_COMPLETE_COMMAND_IDENTIFIER,
                                "uuuv",
                                mean,
                                min,
                                max,
                                variance);
}

// getStandaloneBootloaderInfoReturn
void emberGetStandaloneBootloaderInfoReturn(uint16_t version,
                                            uint8_t platformId,
                                            uint8_t microId,
                                            uint8_t phyId)
{
  emSendBinaryManagementCommand(CB_GET_STANDALONE_BOOTLOADER_INFO_COMMAND_IDENTIFIER,
                                "vuuu",
                                version,
                                platformId,
                                microId,
                                phyId);
}

// launchStandaloneBootloaderReturn
void emberLaunchStandaloneBootloaderReturn(EmberStatus status)
{
  emSendBinaryManagementCommand(CB_LAUNCH_STANDALONE_BOOTLOADER_COMMAND_IDENTIFIER,
                                "u",
                                status);
}

// frequencyHoppingStartClientCompleteHandler
void emberFrequencyHoppingStartClientCompleteHandler(EmberStatus status)
{
  emSendBinaryManagementCommand(CB_FREQUENCY_HOPPING_START_CLIENT_COMPLETE_COMMAND_IDENTIFIER,
                                "u",
                                status);
}

//------------------------------------------------------------------------------
// EMBER_TEST
//------------------------------------------------------------------------------

// echo
static void ncpEchoCommand(void)
{
  uint8_t *data;
  uint8_t length;
  data = emberStringCommandArgument(0, &length);
  emberEcho(data,
            length);
}

// echoReturn
void emberEchoReturn(uint8_t *data,
                     uint8_t length)
{
  emSendBinaryManagementCommand(CB_ECHO_COMMAND_IDENTIFIER,
                                "b",
                                data,
                                length);
}

//------------------------------------------------------------------------------
// APP_USES_SOFTWARE_FLOW_CONTROL
//------------------------------------------------------------------------------
#ifdef EMBER_APPLICATION_USES_SOFTWARE_FLOW_CONTROL

// startXOnXOffTest
static void ncpStartXOnXOffTestCommand(void)
{
  emberStartXOnXOffTest();
}
#endif /* EMBER_APPLICATION_USES_SOFTWARE_FLOW_CONTROL */

//------------------------------------------------------------------------------
// Command Handler Table

const EmberCommandEntry emberCommandTable[] = {
  emberBinaryCommandEntryAction(EMBER_SET_PHY_MODE_COMMAND_IDENTIFIER,
                                ncpSetPhyModeCommand,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_PHY_MODE_COMMAND_IDENTIFIER,
                                ncpGetPhyModeCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_NETWORK_STATE_COMMAND_IDENTIFIER,
                                ncpNetworkStateCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_STACK_IS_UP_COMMAND_IDENTIFIER,
                                ncpStackIsUpCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_SECURITY_KEY_COMMAND_IDENTIFIER,
                                ncpSetSecurityKeyCommand,
                                "b",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_COUNTER_COMMAND_IDENTIFIER,
                                ncpGetCounterCommand,
                                "uw",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_RADIO_CHANNEL_COMMAND_IDENTIFIER,
                                ncpSetRadioChannelCommand,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_RADIO_CHANNEL_COMMAND_IDENTIFIER,
                                ncpGetRadioChannelCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_RADIO_POWER_COMMAND_IDENTIFIER,
                                ncpSetRadioPowerCommand,
                                "s",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_RADIO_POWER_COMMAND_IDENTIFIER,
                                ncpGetRadioPowerCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_RADIO_POWER_MODE_COMMAND_IDENTIFIER,
                                ncpSetRadioPowerModeCommand,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_MAC_PARAMS_COMMAND_IDENTIFIER,
                                ncpSetMacParamsCommand,
                                "uuuuu",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_CURRENT_STACK_TASKS_COMMAND_IDENTIFIER,
                                ncpCurrentStackTasksCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_OK_TO_NAP_COMMAND_IDENTIFIER,
                                ncpOkToNapCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_OK_TO_HIBERNATE_COMMAND_IDENTIFIER,
                                ncpOkToHibernateCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_EUI64_COMMAND_IDENTIFIER,
                                ncpGetEui64Command,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_IS_LOCAL_EUI64_COMMAND_IDENTIFIER,
                                ncpIsLocalEui64Command,
                                "b",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_NODE_ID_COMMAND_IDENTIFIER,
                                ncpGetNodeIdCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_PAN_ID_COMMAND_IDENTIFIER,
                                ncpGetPanIdCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_PARENT_ID_COMMAND_IDENTIFIER,
                                ncpGetParentIdCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_NODE_TYPE_COMMAND_IDENTIFIER,
                                ncpGetNodeTypeCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_CSP_VERSION_COMMAND_IDENTIFIER,
                                ncpGetCspVersionCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_MESSAGE_SEND_COMMAND_IDENTIFIER,
                                ncpMessageSendCommand,
                                "vuuubu",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_POLL_FOR_DATA_COMMAND_IDENTIFIER,
                                ncpPollForDataCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_NETWORK_INIT_COMMAND_IDENTIFIER,
                                ncpNetworkInitCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_START_ACTIVE_SCAN_COMMAND_IDENTIFIER,
                                ncpStartActiveScanCommand,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_START_ENERGY_SCAN_COMMAND_IDENTIFIER,
                                ncpStartEnergyScanCommand,
                                "uu",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_APPLICATION_BEACON_PAYLOAD_COMMAND_IDENTIFIER,
                                ncpSetApplicationBeaconPayloadCommand,
                                "b",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_FORM_NETWORK_COMMAND_IDENTIFIER,
                                ncpFormNetworkCommand,
                                "vuu",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_JOIN_NETWORK_EXTENDED_COMMAND_IDENTIFIER,
                                ncpJoinNetworkExtendedCommand,
                                "uvvuu",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_JOIN_NETWORK_COMMAND_IDENTIFIER,
                                ncpJoinNetworkCommand,
                                "uvuu",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_PERMIT_JOINING_COMMAND_IDENTIFIER,
                                ncpPermitJoiningCommand,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_JOIN_COMMISSIONED_COMMAND_IDENTIFIER,
                                ncpJoinCommissionedCommand,
                                "uvvuu",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_RESET_NETWORK_STATE_COMMAND_IDENTIFIER,
                                ncpResetNetworkStateCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_STANDALONE_BOOTLOADER_INFO_COMMAND_IDENTIFIER,
                                ncpGetStandaloneBootloaderInfoCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_LAUNCH_STANDALONE_BOOTLOADER_COMMAND_IDENTIFIER,
                                ncpLaunchStandaloneBootloaderCommand,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_FREQUENCY_HOPPING_START_SERVER_COMMAND_IDENTIFIER,
                                ncpFrequencyHoppingStartServerCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_FREQUENCY_HOPPING_START_CLIENT_COMMAND_IDENTIFIER,
                                ncpFrequencyHoppingStartClientCommand,
                                "vv",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_FREQUENCY_HOPPING_STOP_COMMAND_IDENTIFIER,
                                ncpFrequencyHoppingStopCommand,
                                "",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_SET_AUXILIARY_ADDRESS_FILTERING_ENTRY_COMMAND_IDENTIFIER,
                                ncpSetAuxiliaryAddressFilteringEntryCommand,
                                "vu",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_GET_AUXILIARY_ADDRESS_FILTERING_ENTRY_COMMAND_IDENTIFIER,
                                ncpGetAuxiliaryAddressFilteringEntryCommand,
                                "u",
                                NULL),
  emberBinaryCommandEntryAction(EMBER_ECHO_COMMAND_IDENTIFIER,
                                ncpEchoCommand,
                                "b",
                                NULL),
#ifdef EMBER_APPLICATION_USES_SOFTWARE_FLOW_CONTROL
  emberBinaryCommandEntryAction(EMBER_START_XON_XOFF_TEST_COMMAND_IDENTIFIER,
                                ncpStartXOnXOffTestCommand,
                                "",
                                NULL),
#endif /* EMBER_APPLICATION_USES_SOFTWARE_FLOW_CONTROL */
  { NULL }    // terminator
};
