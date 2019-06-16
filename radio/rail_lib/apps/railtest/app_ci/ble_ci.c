/***************************************************************************//**
 * @file
 * @brief This file implements commands for configuring BLE RAIL options
 *   relevant to receiving packets
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <stdio.h>
#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "rail_ble.h"

#include "app_common.h"
#include "app_ci.h"

static const uint8_t blePacket[] = {
  0x02, 0x1A, 0xFF, 0xEE, 0xDD, 0xCC, 0xBB, 0xAA, 0x02,
  0x01, 0x06, 0x10, 0x08, 'S', 'i', 'l', 'a', 'b', 's',
  ' ', 'R', 'A', 'I', 'L', 'T', 'E', 'S', 'T',
};
PhySwitchToRx_t phySwitchToRx;

void bleStatus(int argc, char **argv);

void bleEnable(int argc, char **argv)
{
  bool enable = !!ciGetUnsigned(argv[1]);

  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  // Turn BLE mode on or off as requested
  if (enable) {
    RAIL_BLE_Init(railHandle);
  } else {
    RAIL_BLE_Deinit(railHandle);
  }

  // Report the current status of BLE mode
  bleStatus(1, argv);
}

void bleStatus(int argc, char **argv)
{
  bool enabled = RAIL_BLE_IsEnabled(railHandle);

  // Report the current enabled status for BLE
  responsePrint(argv[0], "BLE:%s", enabled ? "Enabled" : "Disabled");
}

void bleSet1MbpsPhy(int argc, char **argv)
{
  RAIL_Status_t status;
  bool isViterbi;
  if (argc == 2) {
    isViterbi = ciGetUnsigned(argv[1]);
  } else {
    isViterbi = 0;
  }

  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  if (isViterbi) {
    status = RAIL_BLE_ConfigPhy1MbpsViterbi(railHandle);
  } else {
    status = RAIL_BLE_ConfigPhy1Mbps(railHandle);
  }
  responsePrint(argv[0], "Status:%s", getStatusMessage(status));
}

void bleSet2MbpsPhy(int argc, char **argv)
{
  RAIL_Status_t status;
  bool isViterbi;
  if (argc == 2) {
    isViterbi = ciGetUnsigned(argv[1]);
  } else {
    isViterbi = 0;
  }

  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  if (isViterbi) {
    status = RAIL_BLE_ConfigPhy2MbpsViterbi(railHandle);
  } else {
    status = RAIL_BLE_ConfigPhy2Mbps(railHandle);
  }
  responsePrint(argv[0], "Status:%s", getStatusMessage(status));
}

void bleSetCoding(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }
  // Make sure BLE mode is enabled so that the call below can succeed
  if (!RAIL_BLE_IsEnabled(railHandle)) {
    responsePrintError(argv[0], 0x31, "BLE mode not enabled");
    return;
  }

  RAIL_BLE_Coding_t coding = (RAIL_BLE_Coding_t) ciGetUnsigned(argv[1]);
  RAIL_Status_t status = RAIL_BLE_ConfigPhyCoded(railHandle, coding);
  responsePrint(argv[0], "Status:%s", getStatusMessage(status));
}

// channel, accessAddress, crcInit, whitening
void bleSetChannelParams(int argc, char **argv)
{
  // Default to the parameters for advertising channels
  uint32_t accessAddress = 0x8E89BED6UL;
  uint32_t crcInit = 0x00555555UL;
  uint8_t logicalChannel = 37;
  bool disableWhitening = false;
  RAIL_Status_t res;

  // Make sure BLE mode is enabled so that the call below can succeed
  if (!RAIL_BLE_IsEnabled(railHandle)) {
    responsePrintError(argv[0], 0x31, "BLE mode not enabled");
    return;
  }

  if (argc > 1) {
    logicalChannel = ciGetUnsigned(argv[1]);
  }
  if (argc > 2) {
    accessAddress = ciGetUnsigned(argv[2]);
  }
  if (argc > 3) {
    crcInit = ciGetUnsigned(argv[3]);
  }
  if (argc > 4) {
    disableWhitening = !!ciGetUnsigned(argv[4]);
  }

  res = RAIL_BLE_ConfigChannelRadioParams(railHandle,
                                          crcInit,
                                          accessAddress,
                                          logicalChannel,
                                          disableWhitening);
  if (res == RAIL_STATUS_NO_ERROR) {
    responsePrint(argv[0],
                  "LogicalChannel:%d,"
                  "AccessAddress:0x%0.8X,"
                  "CrcInit:0x%0.8X,"
                  "Whitening:%s",
                  logicalChannel,
                  accessAddress,
                  crcInit,
                  disableWhitening ? "Disabled" : "Enabled");
  } else {
    responsePrintError(argv[0], 0x32, "Setting channel parameters failed");
  }
}

void bleAdvertisingConfig(int argc, char **argv)
{
  uint8_t advChannel = ciGetUnsigned(argv[1]);
  RAIL_Status_t res;

  // Make sure BLE mode is enabled so that the call below can succeed
  if (!RAIL_BLE_IsEnabled(railHandle)) {
    responsePrintError(argv[0], 0x31, "BLE mode not enabled");
    return;
  }

  if ((advChannel < 37) || (advChannel > 39)) {
    responsePrintError(argv[0], 0x30, "Invalid advertising channel");
    return;
  }

  // Configure RAIL for BLE advertising on channel 37
  res = RAIL_BLE_ConfigChannelRadioParams(railHandle,  // RAIL instance
                                          0x00555555UL, // CRC Init
                                          0x8E89BED6UL, // Access Address
                                          advChannel,  // Channel
                                          false);      // Disable Whitening
  if (res != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x32, "Setting channel parameters failed");
    return;
  }

  if (advChannel == 37) {
    changeChannel(0); // Logical channel 37 is physical channel 0
  } else if (advChannel == 38) {
    changeChannel(12); // Logical channel 38 is physical channel 12
  } else if (advChannel == 39) {
    changeChannel(39); // Logical channel 39 is physical channel 39
  }

  responsePrint(argv[0], "AdvertisingChannel:%d", advChannel);

  // Load up a suitable advertising packet
  memcpy(txData, blePacket, sizeof(blePacket));
  txDataLen = sizeof(blePacket);
  printTxPacket(1, argv);
}

void blePhySwitchToRx(int argc, char **argv)
{
  phySwitchToRx.enable = ciGetUnsigned(argv[1]);
  if (phySwitchToRx.enable) {
    phySwitchToRx.phy = RAIL_BLE_1Mbps;
    phySwitchToRx.physicalChannel = 0U;
    phySwitchToRx.timeDelta = 1000U;
    phySwitchToRx.crcInit = 0x00555555UL;
    phySwitchToRx.accessAddress = 0x8E89BED6UL;
    phySwitchToRx.logicalChannel = 37U;
    phySwitchToRx.disableWhitening = false;

    if (argc > 2) {
      phySwitchToRx.phy = ciGetUnsigned(argv[2]);
    }
    if (argc > 3) {
      phySwitchToRx.timeDelta = ciGetUnsigned(argv[3]);
    }
    if (argc > 4) {
      phySwitchToRx.physicalChannel = ciGetUnsigned(argv[4]);
    }
    if (argc > 5) {
      phySwitchToRx.logicalChannel = ciGetUnsigned(argv[5]);
    }
    if (argc > 6) {
      phySwitchToRx.accessAddress = ciGetUnsigned(argv[6]);
    }
    if (argc > 7) {
      phySwitchToRx.crcInit = ciGetUnsigned(argv[7]);
    }
    if (argc > 8) {
      phySwitchToRx.disableWhitening = ciGetUnsigned(argv[8]);
    }
  }

  char *enabled = phySwitchToRx.enable ? "Enabled" : "Disabled";
  responsePrint(argv[0], "PhySwitchToRx:%s", enabled);
}
