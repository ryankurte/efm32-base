/**************************************************************************//**
 * Copyright 2017 Silicon Laboratories, Inc.
 *
 *****************************************************************************/
//
// *** Generated file. Do not edit! ***
//
// Description: Enumerations for CSP.

#ifndef __CSP_ENUM_H__
#define __CSP_ENUM_H__

#define CSP_VERSION 0x0100
//------------------------------------------------------------------------------
// Frame IDs

enum {
// Core

  EMBER_SET_PHY_MODE_COMMAND_IDENTIFIER                                = 0x6900,
  EMBER_SET_PHY_MODE_RETURN_IDENTIFIER                                 = 0x7500,
  EMBER_GET_PHY_MODE_COMMAND_IDENTIFIER                                = 0x6901,
  EMBER_GET_PHY_MODE_RETURN_IDENTIFIER                                 = 0x7501,
  EMBER_NETWORK_STATE_COMMAND_IDENTIFIER                               = 0x6902,
  EMBER_NETWORK_STATE_RETURN_IDENTIFIER                                = 0x7502,
  EMBER_STACK_IS_UP_COMMAND_IDENTIFIER                                 = 0x6903,
  EMBER_STACK_IS_UP_RETURN_IDENTIFIER                                  = 0x7503,
  EMBER_SET_SECURITY_KEY_COMMAND_IDENTIFIER                            = 0x6904,
  EMBER_SET_SECURITY_KEY_RETURN_IDENTIFIER                             = 0x7504,
  EMBER_GET_COUNTER_COMMAND_IDENTIFIER                                 = 0x6905,
  EMBER_GET_COUNTER_RETURN_IDENTIFIER                                  = 0x7505,
  EMBER_SET_RADIO_CHANNEL_COMMAND_IDENTIFIER                           = 0x6906,
  EMBER_SET_RADIO_CHANNEL_RETURN_IDENTIFIER                            = 0x7506,
  EMBER_GET_RADIO_CHANNEL_COMMAND_IDENTIFIER                           = 0x6907,
  EMBER_GET_RADIO_CHANNEL_RETURN_IDENTIFIER                            = 0x7507,
  EMBER_SET_RADIO_POWER_COMMAND_IDENTIFIER                             = 0x6908,
  EMBER_SET_RADIO_POWER_RETURN_IDENTIFIER                              = 0x7508,
  EMBER_GET_RADIO_POWER_COMMAND_IDENTIFIER                             = 0x6909,
  EMBER_GET_RADIO_POWER_RETURN_IDENTIFIER                              = 0x7509,
  EMBER_SET_RADIO_POWER_MODE_COMMAND_IDENTIFIER                        = 0x690A,
  EMBER_SET_RADIO_POWER_MODE_RETURN_IDENTIFIER                         = 0x750A,
  EMBER_SET_MAC_PARAMS_COMMAND_IDENTIFIER                              = 0x690B,
  EMBER_SET_MAC_PARAMS_RETURN_IDENTIFIER                               = 0x750B,
  EMBER_CURRENT_STACK_TASKS_COMMAND_IDENTIFIER                         = 0x690C,
  EMBER_CURRENT_STACK_TASKS_RETURN_IDENTIFIER                          = 0x750C,
  EMBER_OK_TO_NAP_COMMAND_IDENTIFIER                                   = 0x690D,
  EMBER_OK_TO_NAP_RETURN_IDENTIFIER                                    = 0x750D,
  EMBER_OK_TO_HIBERNATE_COMMAND_IDENTIFIER                             = 0x690E,
  EMBER_OK_TO_HIBERNATE_RETURN_IDENTIFIER                              = 0x750E,
  EMBER_GET_EUI64_COMMAND_IDENTIFIER                                   = 0x690F,
  EMBER_GET_EUI64_RETURN_IDENTIFIER                                    = 0x750F,
  EMBER_IS_LOCAL_EUI64_COMMAND_IDENTIFIER                              = 0x6910,
  EMBER_IS_LOCAL_EUI64_RETURN_IDENTIFIER                               = 0x7510,
  EMBER_GET_NODE_ID_COMMAND_IDENTIFIER                                 = 0x6911,
  EMBER_GET_NODE_ID_RETURN_IDENTIFIER                                  = 0x7511,
  EMBER_GET_PAN_ID_COMMAND_IDENTIFIER                                  = 0x6912,
  EMBER_GET_PAN_ID_RETURN_IDENTIFIER                                   = 0x7512,
  EMBER_GET_PARENT_ID_COMMAND_IDENTIFIER                               = 0x6913,
  EMBER_GET_PARENT_ID_RETURN_IDENTIFIER                                = 0x7513,
  EMBER_GET_NODE_TYPE_COMMAND_IDENTIFIER                               = 0x6914,
  EMBER_GET_NODE_TYPE_RETURN_IDENTIFIER                                = 0x7514,
  EMBER_GET_CSP_VERSION_COMMAND_IDENTIFIER                             = 0x6915,
  EMBER_GET_CSP_VERSION_RETURN_IDENTIFIER                              = 0x7515,
  CB_STACK_STATUS_COMMAND_IDENTIFIER                                   = 0x6300,
  CB_CHILD_JOIN_COMMAND_IDENTIFIER                                     = 0x6301,

// Messaging

  EMBER_MESSAGE_SEND_COMMAND_IDENTIFIER                                = 0x6916,
  EMBER_MESSAGE_SEND_RETURN_IDENTIFIER                                 = 0x7516,
  EMBER_POLL_FOR_DATA_COMMAND_IDENTIFIER                               = 0x6917,
  EMBER_POLL_FOR_DATA_RETURN_IDENTIFIER                                = 0x7517,
  CB_MESSAGE_SENT_COMMAND_IDENTIFIER                                   = 0x6302,
  CB_INCOMING_MESSAGE_COMMAND_IDENTIFIER                               = 0x6303,

// Network Management

  EMBER_NETWORK_INIT_COMMAND_IDENTIFIER                                = 0x6918,
  EMBER_NETWORK_INIT_RETURN_IDENTIFIER                                 = 0x7518,
  EMBER_START_ACTIVE_SCAN_COMMAND_IDENTIFIER                           = 0x6919,
  EMBER_START_ACTIVE_SCAN_RETURN_IDENTIFIER                            = 0x7519,
  EMBER_START_ENERGY_SCAN_COMMAND_IDENTIFIER                           = 0x691A,
  EMBER_START_ENERGY_SCAN_RETURN_IDENTIFIER                            = 0x751A,
  EMBER_SET_APPLICATION_BEACON_PAYLOAD_COMMAND_IDENTIFIER              = 0x691B,
  EMBER_SET_APPLICATION_BEACON_PAYLOAD_RETURN_IDENTIFIER               = 0x751B,
  EMBER_FORM_NETWORK_COMMAND_IDENTIFIER                                = 0x691C,
  EMBER_FORM_NETWORK_RETURN_IDENTIFIER                                 = 0x751C,
  EMBER_JOIN_NETWORK_EXTENDED_COMMAND_IDENTIFIER                       = 0x691D,
  EMBER_JOIN_NETWORK_EXTENDED_RETURN_IDENTIFIER                        = 0x751D,
  EMBER_JOIN_NETWORK_COMMAND_IDENTIFIER                                = 0x691E,
  EMBER_JOIN_NETWORK_RETURN_IDENTIFIER                                 = 0x751E,
  EMBER_PERMIT_JOINING_COMMAND_IDENTIFIER                              = 0x691F,
  EMBER_PERMIT_JOINING_RETURN_IDENTIFIER                               = 0x751F,
  EMBER_JOIN_COMMISSIONED_COMMAND_IDENTIFIER                           = 0x6920,
  EMBER_JOIN_COMMISSIONED_RETURN_IDENTIFIER                            = 0x7520,
  EMBER_RESET_NETWORK_STATE_COMMAND_IDENTIFIER                         = 0x6921,
  EMBER_GET_STANDALONE_BOOTLOADER_INFO_COMMAND_IDENTIFIER              = 0x6922,
  EMBER_LAUNCH_STANDALONE_BOOTLOADER_COMMAND_IDENTIFIER                = 0x6923,
  EMBER_FREQUENCY_HOPPING_START_SERVER_COMMAND_IDENTIFIER              = 0x6926,
  EMBER_FREQUENCY_HOPPING_START_SERVER_RETURN_IDENTIFIER               = 0x7526,
  EMBER_FREQUENCY_HOPPING_START_CLIENT_COMMAND_IDENTIFIER              = 0x6927,
  EMBER_FREQUENCY_HOPPING_START_CLIENT_RETURN_IDENTIFIER               = 0x7527,
  EMBER_FREQUENCY_HOPPING_STOP_COMMAND_IDENTIFIER                      = 0x6928,
  EMBER_FREQUENCY_HOPPING_STOP_RETURN_IDENTIFIER                       = 0x7528,
  EMBER_SET_AUXILIARY_ADDRESS_FILTERING_ENTRY_COMMAND_IDENTIFIER       = 0x6929,
  EMBER_SET_AUXILIARY_ADDRESS_FILTERING_ENTRY_RETURN_IDENTIFIER        = 0x7529,
  EMBER_GET_AUXILIARY_ADDRESS_FILTERING_ENTRY_COMMAND_IDENTIFIER       = 0x692A,
  EMBER_GET_AUXILIARY_ADDRESS_FILTERING_ENTRY_RETURN_IDENTIFIER        = 0x752A,
  CB_INCOMING_BEACON_COMMAND_IDENTIFIER                                = 0x6304,
  CB_ACTIVE_SCAN_COMPLETE_COMMAND_IDENTIFIER                           = 0x6305,
  CB_ENERGY_SCAN_COMPLETE_COMMAND_IDENTIFIER                           = 0x6306,
  CB_GET_STANDALONE_BOOTLOADER_INFO_COMMAND_IDENTIFIER                 = 0x6307,
  CB_LAUNCH_STANDALONE_BOOTLOADER_COMMAND_IDENTIFIER                   = 0x6308,
  CB_FREQUENCY_HOPPING_START_CLIENT_COMPLETE_COMMAND_IDENTIFIER        = 0x630A,

// EMBER_TEST

  EMBER_ECHO_COMMAND_IDENTIFIER                                        = 0x6924,
  CB_ECHO_COMMAND_IDENTIFIER                                           = 0x6309,

// APP_USES_SOFTWARE_FLOW_CONTROL

  EMBER_START_XON_XOFF_TEST_COMMAND_IDENTIFIER                         = 0x6925,
};

#endif // __CSP_ENUM_H__
