/***************************************************************************//**
 * @file 154_rx_ci.c
 * @brief This file implements commands for configuring 802.15.4 options
 * relevant to receiving packets
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "rail_ieee802154.h"

#include "app_common.h"

extern char *rfStates[];

void ieee802154Enable(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  uint16_t timing;
  RAIL_IEEE802154_Config_t config = { false, false,
                                      RAIL_IEEE802154_ACCEPT_STANDARD_FRAMES,
                                      RAIL_RF_STATE_RX, 100, 192, 1000, NULL };

  if (memcmp(argv[1], "idle", 4) == 0) {
    config.defaultState = RAIL_RF_STATE_IDLE;
  } else if (memcmp(argv[1], "rx", 2) == 0) {
    config.defaultState = RAIL_RF_STATE_RX;
  } else {
    responsePrintError(argv[0], 0x20, "Unknown auto ack default state.");
    return;
  }

  timing = ciGetUnsigned(argv[2]);
  if (timing > 13000) {
    responsePrintError(argv[0], 0x21, "Invalid idle timing.");
    return;
  } else {
    config.idleTime = timing;
  }

  timing = ciGetUnsigned(argv[3]);
  if (timing > 13000) {
    responsePrintError(argv[0], 0x22, "Invalid turnaround timing");
    return;
  } else {
    config.turnaroundTime = timing;
  }

  timing = ciGetUnsigned(argv[4]);
  config.ackTimeout = timing;

  RAIL_Status_t status = RAIL_IEEE802154_Init(&config);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], status, "Call to RAIL_IEEE802154_Init returned an error");
  } else {
    responsePrint(argv[0],
                  "802.15.4:%s,"
                  "defaultState:%s,"
                  "idleTiming:%d,"
                  "turnaroundTime:%d,"
                  "ackTimeout:%d",
                  status ? "Disabled" : "Enabled",
                  rfStates[config.defaultState],
                  config.idleTime,
                  config.turnaroundTime,
                  config.ackTimeout);
  }
}

void config2p4Ghz802154(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  RAIL_Status_t status = RAIL_IEEE802154_2p4GHzRadioConfig();
  changeChannel(11);
  responsePrint(argv[0], "802.15.4:%s", status ? "Disabled" : "Enabled");
}

void ieee802154AcceptFrames(int argc, char **argv)
{
  uint8_t framesEnable = 0;
  bool commandFrame = ciGetUnsigned(argv[1]);
  bool ackFrame = ciGetUnsigned(argv[2]);
  bool dataFrame = ciGetUnsigned(argv[3]);
  bool beaconFrame = ciGetUnsigned(argv[4]);

  // Command
  if (commandFrame) {
    framesEnable |= RAIL_IEEE802154_ACCEPT_COMMAND_FRAMES;
  }
  if (ackFrame) {
    framesEnable |= RAIL_IEEE802154_ACCEPT_ACK_FRAMES;
  }
  if (dataFrame) {
    framesEnable |= RAIL_IEEE802154_ACCEPT_DATA_FRAMES;
  }
  if (beaconFrame) {
    framesEnable |= RAIL_IEEE802154_ACCEPT_BEACON_FRAMES;
  }

  RAIL_Status_t status = RAIL_IEEE802154_AcceptFrames(framesEnable);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x23, "Failed to set which frames to accept.");
  } else {
    responsePrint(argv[0],
                  "CommandFrame:%s,"
                  "AckFrame:%s,"
                  "DataFrame:%s,"
                  "BeaconFrame:%s",
                  commandFrame ? "Enabled" : "Disabled",
                  ackFrame ? "Enabled" : "Disabled",
                  dataFrame ? "Enabled" : "Disabled",
                  beaconFrame ? "Enabled" : "Disabled");
  }
}

void ieee802154SetPromiscuousMode(int argc, char **argv)
{
  bool promiscuous = ciGetUnsigned(argv[1]);
  RAIL_Status_t status = RAIL_IEEE802154_SetPromiscuousMode(promiscuous);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x24, "Failed to (un)set promiscuous mode.");
  } else {
    responsePrint(argv[0], "PromiscuousMode:%s",
                  promiscuous ? "Enabled" : "Disabled");
  }
}

void ieee802154SetPanCoordinator(int argc, char **argv)
{
  bool panCoord = ciGetUnsigned(argv[1]);
  RAIL_Status_t status = RAIL_IEEE802154_SetPanCoordinator(panCoord);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x24, "Failed to (un)set PAN Coordinator.");
  } else {
    responsePrint(argv[0], "PanCoordinator:%s",
                  panCoord ? "Enabled" : "Disabled");
  }
}

void ieee802154SetPanId(int argc, char **argv)
{
  uint16_t panId = ciGetUnsigned(argv[1]);
  bool status = RAIL_IEEE802154_SetPanId(panId);
  responsePrint(argv[0], "802.15.4PanId:%s",
                status ? "Enabled" : "Disabled");
}

void ieee802154SetShortAddress(int argc, char **argv)
{
  uint16_t shortAddr = ciGetUnsigned(argv[1]);
  bool status = RAIL_IEEE802154_SetShortAddress(shortAddr);
  responsePrint(argv[0], "802.15.4ShortAddress:%s",
                status ? "Enabled" : "Disabled");
}

void ieee802154SetLongAddress(int argc, char **argv)
{
  uint8_t longAddr[8];
  for (int i = 0; i < 8; i++) {
    longAddr[i] = ciGetUnsigned(argv[i + 1]);
  }
  bool status = RAIL_IEEE802154_SetLongAddress(longAddr);
  responsePrint(argv[0], "802.15.4LongAddress:%s",
                status ? "Enabled" : "Disabled");
}
