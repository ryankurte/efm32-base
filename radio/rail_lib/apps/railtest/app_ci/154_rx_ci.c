/***************************************************************************//**
 * @file
 * @brief This file implements commands for configuring 802.15.4 options
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
#include <stdlib.h>
#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "rail_ieee802154.h"

#include "app_common.h"

bool ieee802154EnhAckEnabled = false;
uint8_t ieee802154PhrLen = 1U; // Default is 1-byte PHY Header (length byte)

uint32_t dataReqLatencyUs = 0U;

void ieee802154Enable(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  RAIL_StateTiming_t timings = {
    .idleToTx = 100,
    .idleToRx = 100,
    .rxToTx = 192,
    // Make txToRx slightly lower than desired to make sure we get to
    // RX in time
    .txToRx = 192 - 10,
    .rxSearchTimeout = 0,
    .txToRxSearchTimeout = 0
  };

  RAIL_IEEE802154_Config_t config = {
    .addresses = NULL,
    .ackConfig = {
      .enable = true,
      .ackTimeout = 1000,
      .rxTransitions = {
        .success = RAIL_RF_STATE_RX,
        .error = RAIL_RF_STATE_RX // ignored
      },
      .txTransitions = {
        .success = RAIL_RF_STATE_RX,
        .error = RAIL_RF_STATE_RX // ignored
      }
    },
    .timings = timings,
    .framesMask = RAIL_IEEE802154_ACCEPT_STANDARD_FRAMES,
    .promiscuousMode = false,
    .isPanCoordinator = false
  };

  if (memcmp(argv[1], "idle", 4) == 0) {
    config.ackConfig.rxTransitions.success = RAIL_RF_STATE_IDLE;
    config.ackConfig.txTransitions.success = RAIL_RF_STATE_IDLE;
  } else if (memcmp(argv[1], "rx", 2) == 0) {
    config.ackConfig.rxTransitions.success = RAIL_RF_STATE_RX;
    config.ackConfig.txTransitions.success = RAIL_RF_STATE_RX;
  } else {
    responsePrintError(argv[0], 0x20, "Unknown auto ack default state.");
    return;
  }

  uint16_t timing = ciGetUnsigned(argv[2]);
  if (timing > 13000) {
    responsePrintError(argv[0], 0x21, "Invalid idle timing.");
    return;
  } else {
    config.timings.idleToTx = timing;
    config.timings.idleToRx = timing;
  }

  timing = ciGetUnsigned(argv[3]);
  if (timing > 13000) {
    responsePrintError(argv[0], 0x22, "Invalid turnaround timing");
    return;
  } else {
    config.timings.rxToTx = timing;
    config.timings.txToRx = timing - 10;
  }

  timing = ciGetUnsigned(argv[4]);
  config.ackConfig.ackTimeout = timing;

  RAIL_Status_t status = RAIL_IEEE802154_Init(railHandle, &config);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], status, "Call to RAIL_IEEE802154_Init returned an error");
  } else {
    responsePrint(argv[0],
                  "802.15.4:%s,"
                  "rxDefaultState:%s,"
                  "txDefaultState:%s,"
                  "idleTiming:%d,"
                  "turnaroundTime:%d,"
                  "ackTimeout:%d",
                  status ? "Disabled" : "Enabled",
                  getRfStateName(config.ackConfig.txTransitions.success),
                  getRfStateName(config.ackConfig.rxTransitions.success),
                  config.timings.idleToTx,
                  config.timings.rxToTx,
                  config.ackConfig.ackTimeout);
  }
}

void config2p4Ghz802154(int argc, char **argv)
{
  RAIL_Status_t status;
  bool antDivConfig = false;
  bool coexConfig = false;
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }
  if (argc > 1) {
    antDivConfig = ciGetUnsigned(argv[1]);
  }
  if (argc > 2) {
    coexConfig = ciGetUnsigned(argv[2]);
  }
  if (antDivConfig) {
    if (coexConfig) {
      status = RAIL_IEEE802154_Config2p4GHzRadioAntDivCoex(railHandle);
    } else {
      status = RAIL_IEEE802154_Config2p4GHzRadioAntDiv(railHandle);
    }
  } else if (coexConfig) {
    status = RAIL_IEEE802154_Config2p4GHzRadioCoex(railHandle);
  } else {
    status = RAIL_IEEE802154_Config2p4GHzRadio(railHandle);
  }
  if (status == RAIL_STATUS_NO_ERROR) {
    ieee802154PhrLen = 1U;
    changeChannel(11);
  }
  responsePrint(argv[0], "802.15.4:%s", status ? "Disabled" : "Enabled");
}

void config863Mhz802154(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  RAIL_Status_t status = RAIL_IEEE802154_ConfigGB863MHzRadio(railHandle);
  if (status == RAIL_STATUS_NO_ERROR) {
    ieee802154PhrLen = 2U;
    changeChannel(0x80);
  }
  responsePrint(argv[0], "802.15.4:%s", status ? "Disabled" : "Enabled");
}

void config915Mhz802154(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  RAIL_Status_t status = RAIL_IEEE802154_ConfigGB915MHzRadio(railHandle);
  if (status == RAIL_STATUS_NO_ERROR) {
    ieee802154PhrLen = 2U;
    changeChannel(0xE0);
  }
  responsePrint(argv[0], "802.15.4:%s", status ? "Disabled" : "Enabled");
}

void ieee802154AcceptFrames(int argc, char **argv)
{
  uint8_t framesEnable = 0;
  bool commandFrame = ciGetUnsigned(argv[1]);
  bool ackFrame = ciGetUnsigned(argv[2]);
  bool dataFrame = ciGetUnsigned(argv[3]);
  bool beaconFrame = ciGetUnsigned(argv[4]);
  bool multipurposeFrame = false;

  if (argc > 5) {
    multipurposeFrame = ciGetUnsigned(argv[5]);
  }

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
  if (multipurposeFrame) {
    framesEnable |= RAIL_IEEE802154_ACCEPT_MULTIPURPOSE_FRAMES;
  }

  RAIL_Status_t status = RAIL_IEEE802154_AcceptFrames(railHandle, framesEnable);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x23, "Failed to set which frames to accept.");
  } else {
    responsePrint(argv[0],
                  "CommandFrame:%s,"
                  "AckFrame:%s,"
                  "DataFrame:%s,"
                  "BeaconFrame:%s,"
                  "MultiPurposeFrame:%s",
                  commandFrame ? "Enabled" : "Disabled",
                  ackFrame ? "Enabled" : "Disabled",
                  dataFrame ? "Enabled" : "Disabled",
                  beaconFrame ? "Enabled" : "Disabled",
                  multipurposeFrame ? "Enabled" : "Disabled");
  }
}

void ieee802154SetPromiscuousMode(int argc, char **argv)
{
  bool promiscuous = ciGetUnsigned(argv[1]);
  RAIL_Status_t status = RAIL_IEEE802154_SetPromiscuousMode(railHandle, promiscuous);
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
  RAIL_Status_t status = RAIL_IEEE802154_SetPanCoordinator(railHandle, panCoord);
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
  uint8_t index = (argc > 2) ? ciGetUnsigned(argv[2]) : 0;
  RAIL_Status_t status = RAIL_IEEE802154_SetPanId(railHandle, panId, index);
  responsePrint(argv[0], "802.15.4PanId:%s", getStatusMessage(status));
}

void ieee802154SetShortAddress(int argc, char **argv)
{
  uint16_t shortAddr = ciGetUnsigned(argv[1]);
  uint8_t index = (argc > 2) ? ciGetUnsigned(argv[2]) : 0;
  RAIL_Status_t status = RAIL_IEEE802154_SetShortAddress(railHandle,
                                                         shortAddr,
                                                         index);
  responsePrint(argv[0], "802.15.4ShortAddress:%s", getStatusMessage(status));
}

void ieee802154SetLongAddress(int argc, char **argv)
{
  uint8_t longAddr[8];
  for (int i = 0; i < 8; i++) {
    longAddr[i] = ciGetUnsigned(argv[i + 1]);
  }
  uint8_t index = (argc > 9) ? ciGetUnsigned(argv[9]) : 0;
  RAIL_Status_t status = RAIL_IEEE802154_SetLongAddress(railHandle,
                                                        longAddr,
                                                        index);
  responsePrint(argv[0], "802.15.4LongAddress:%s", getStatusMessage(status));
}

void ieee802154SetAddresses(int argc, char **argv)
{
  RAIL_IEEE802154_AddrConfig_t addresses = {
    { 0xFFFF, 0xFFFF, 0xFFFF },
    { 0xFFFF, 0xFFFF, 0xFFFF },
    { { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
      { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }
  };

  uint8_t argIndex = 1; // Skip the command argument
  for (uint8_t i = 0; i < RAIL_IEEE802154_MAX_ADDRESSES; i++) {
    if (argIndex >= argc) {
      break;
    }
    addresses.panId[i] = ciGetUnsigned(argv[argIndex]);
    argIndex++;

    if (argIndex >= argc) {
      break;
    }
    addresses.shortAddr[i] = ciGetUnsigned(argv[argIndex]);
    argIndex++;

    if (argIndex >= argc) {
      break;
    }
    uint64_t longAddr = strtoull(argv[argIndex], NULL, 0);
    for (int j = 0; j < 8; j++) {
      addresses.longAddr[i][j] = (longAddr >> (i * 8)) & 0xFF;
    }
    argIndex++;
  }
  RAIL_Status_t status = RAIL_IEEE802154_SetAddresses(railHandle, &addresses);
  responsePrint(argv[0], "802.15.4Addresses:%s", getStatusMessage(status));
}

void ieee802154SetDataReqLatency(int argc, char **argv)
{
  dataReqLatencyUs = ciGetUnsigned(argv[1]);
  responsePrint(argv[0], "DataReqLatency:%u", dataReqLatencyUs);
}

void ieee802154SetE(int argc, char **argv)
{
  RAIL_IEEE802154_EOptions_t options = (RAIL_IEEE802154_EOptions_t) ciGetUnsigned(argv[1]);
  RAIL_Status_t status = RAIL_IEEE802154_ConfigEOptions(railHandle,
                                                        RAIL_IEEE802154_E_OPTIONS_ALL,
                                                        options);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 31, "802.15.4E:Failed");
    return;
  }
  ieee802154EnhAckEnabled = ((options & RAIL_IEEE802154_E_OPTION_ENH_ACK) != 0U);
  responsePrint(argv[0],
                "15.4E_GB868:%s,"
                "15.4E_EnhAck:%s,"
                "15.4E_ImplicitBroadcast:%s",
                (options & RAIL_IEEE802154_E_OPTION_GB868) ? "True" : "False",
                (options & RAIL_IEEE802154_E_OPTION_ENH_ACK) ? "True" : "False",
                (options & RAIL_IEEE802154_E_OPTION_IMPLICIT_BROADCAST) ? "True" : "False");
}

void ieee802154SetG(int argc, char **argv)
{
  RAIL_IEEE802154_GOptions_t options = (RAIL_IEEE802154_GOptions_t) ciGetUnsigned(argv[1]);
  RAIL_Status_t status = RAIL_IEEE802154_ConfigGOptions(railHandle,
                                                        RAIL_IEEE802154_G_OPTIONS_ALL,
                                                        options);
  if (status != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 31, "802.15.4G:Failed");
    return;
  } else {
    ieee802154PhrLen = (((options & RAIL_IEEE802154_G_OPTION_GB868) != 0U)
                        ? 2U : 1U);
  }
  responsePrint(argv[0],
                "15.4G_GB868:%s",
                (options & RAIL_IEEE802154_G_OPTION_GB868) ? "True" : "False");
}

void ieee802154SetFpMode(int argc, char **argv)
{
  bool earlyFp = ciGetUnsigned(argv[1]);
  bool dataFp = ciGetUnsigned(argv[2]);

  RAIL_Status_t earlyStatus = RAIL_IEEE802154_EnableEarlyFramePending(railHandle, earlyFp);
  const char *earlyDisplay;
  if (earlyStatus != RAIL_STATUS_NO_ERROR) {
    earlyDisplay = "Unsupported";
  } else {
    earlyDisplay = (earlyFp) ? "Enabled" : "Disabled";
  }

  RAIL_Status_t dataStatus = RAIL_IEEE802154_EnableDataFramePending(railHandle, dataFp);
  const char *dataDisplay;
  if (dataStatus != RAIL_STATUS_NO_ERROR) {
    dataDisplay = "Unsupported";
  } else {
    dataDisplay = (dataFp) ? "Enabled" : "Disabled";
  }

  responsePrint(argv[0], "EarlyFp:%s,DataFp:%s", earlyDisplay, dataDisplay);
}

// 802.15.4-2015 Frame Control Field definitions for Beacon, Ack, Data, Command
#define MAC_FRAME_SOURCE_MODE_MASK          0xC000U
#define MAC_FRAME_SOURCE_MODE_LONG          0xC000U
#define MAC_FRAME_SOURCE_MODE_SHORT         0x8000U
#define MAC_FRAME_SOURCE_MODE_RESERVED      0x4000U
#define MAC_FRAME_SOURCE_MODE_NONE          0x0000U
#define MAC_FRAME_VERSION_MASK              0x3000U
#define MAC_FRAME_VERSION_RESERVED          0x3000U
#define MAC_FRAME_VERSION_2012              0x2000U
#define MAC_FRAME_VERSION_2006              0x1000U
#define MAC_FRAME_VERSION_2003              0x0000U
#define MAC_FRAME_DESTINATION_MODE_MASK     0x0C00U
#define MAC_FRAME_DESTINATION_MODE_LONG     0x0C00U
#define MAC_FRAME_DESTINATION_MODE_SHORT    0x0800U
#define MAC_FRAME_DESTINATION_MODE_RESERVED 0x0400U
#define MAC_FRAME_DESTINATION_MODE_NONE     0x0000U
#define MAC_FRAME_FLAG_IE_LIST_PRESENT      0x0200U
#define MAC_FRAME_FLAG_SEQ_SUPPRESSION      0x0100U
#define MAC_FRAME_FLAG_RESERVED             0x0080U
#define MAC_FRAME_FLAG_PANID_COMPRESSION    0x0040U
#define MAC_FRAME_FLAG_ACK_REQUIRED         0x0020U
#define MAC_FRAME_FLAG_FRAME_PENDING        0x0010U
#define MAC_FRAME_FLAG_SECURITY_ENABLED     0x0008U
#define MAC_FRAME_TYPE_MASK                 0x0007U
#define MAC_FRAME_TYPE_BEACON               0x0000U
#define MAC_FRAME_TYPE_DATA                 0x0001U
#define MAC_FRAME_TYPE_ACK                  0x0002U
#define MAC_FRAME_TYPE_CONTROL              0x0003U
// 802.15.4-2015 Frame Control Field definitions for Multipurpose
#define MAC_FRAME_TYPE_MULTIPURPOSE         0x0005U
#define MAC_MP_FRAME_FLAG_LONG_FCF          0x0008U
#define MAC_MP_FRAME_DESTINATION_MODE_MASK  0x0030U
#define MAC_MP_FRAME_SOURCE_MODE_MASK       0x00C0U
#define MAC_MP_FRAME_FLAG_PANID_PRESENT     0x0100U
#define MAC_MP_FRAME_FLAG_SECURITY_ENABLED  0x0200U
#define MAC_MP_FRAME_FLAG_SEQ_SUPPRESSION   0x0400U
#define MAC_MP_FRAME_FLAG_FRAME_PENDING     0x0800U
#define MAC_MP_FRAME_FRAME_VERSION_MASK     MAC_FRAME_VERSION_MASK
#define MAC_MP_FRAME_FLAG_ACK_REQUIRED      0x4000U
#define MAC_MP_FRAME_FLAG_IE_LIST_PRESENT   0x8000U

// This table is derived from 802.15.4-2015 Section 7.2.1.5 PAN ID
// Compression field and Table 7-2 for both 2003/2006 and 2015
// frame versions.  It is indexed by 6 bits of the MacFCF:
//   SrcAdrMode FrameVer<msbit> DstAdrMode PanIdCompression
// and each address' length is encoded in a nibble:
//    15:12  11:8     7:4     3:0
//   SrcAdr  SrcPan  DstAdr  DstPan
// Illegal combinations are indicated by 0xFFFFU.
#define ADDRSIZE_DST_PAN_SHIFT 0
#define ADDRSIZE_DST_PAN_MASK  (0x0FU << ADDRSIZE_DST_PAN_SHIFT)
#define ADDRSIZE_DST_ADR_SHIFT 4
#define ADDRSIZE_DST_ADR_MASK  (0x0FU << ADDRSIZE_DST_ADR_SHIFT)
#define ADDRSIZE_SRC_PAN_SHIFT 8
#define ADDRSIZE_SRC_PAN_MASK  (0x0FU << ADDRSIZE_SRC_PAN_SHIFT)
#define ADDRSIZE_SRC_ADR_SHIFT 12
#define ADDRSIZE_SRC_ADR_MASK  (0x0FU << ADDRSIZE_SRC_ADR_SHIFT)

static const uint16_t ieee802154Table7p2[64] = {
  0x0000U, 0x0000U, 0xFFFFU, 0xFFFFU, 0x0022U, 0x0022U, 0x0082U, 0x0082U,
  0x0000U, 0x0002U, 0xFFFFU, 0xFFFFU, 0x0022U, 0x0020U, 0x0082U, 0x0080U,
  0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU,
  0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU, 0xFFFFU,
  0x2200U, 0x2200U, 0xFFFFU, 0xFFFFU, 0x2222U, 0x2022U, 0x2282U, 0x2082U,
  0x2200U, 0x2000U, 0xFFFFU, 0xFFFFU, 0x2222U, 0x2022U, 0x2282U, 0x2082U,
  0x8200U, 0x8200U, 0xFFFFU, 0xFFFFU, 0x8222U, 0x8022U, 0x8282U, 0x8082U,
  0x8200U, 0x8000U, 0xFFFFU, 0xFFFFU, 0x8222U, 0x8022U, 0x8082U, 0x8080U,
};

void RAILCb_IEEE802154_DataRequestCommand(RAIL_Handle_t railHandle)
{
  RAIL_IEEE802154_Address_t address;
  bool setFramePending = false;
  if (dataReqLatencyUs > 0U) {
    (void) RAIL_DelayUs(dataReqLatencyUs);
  }
  if (ieee802154EnhAckEnabled) {
    // This callback occurs after the address fields of an incoming
    // ACK-requesting CMD or DATA frame have been received and we
    // can do a frame pending check.  We must also figure out what
    // kind of ACK is being requesting -- Immediate or Enhanced.
    // For an Enhanced ACK, we need to generate that ourselves;
    // RAIL will generate an Immediate ACK for us, though we can
    // tell it to go out with its FramePending bit set.
    // An 802.15.4 packet from RAIL should look like:
    // 1/2 |   1/2  | 0/1  |  0/2   | 0/2/8  |  0/2   | 0/2/8  | ...
    // PHR | MacFCF | Seq# | DstPan | DstAdr | SrcPan | SrcAdr | ...
    #define MaxExpectedBytes (2U + 2U + 1U + 2U + 8U + 2U + 8U)
    RAIL_RxPacketInfo_t packetInfo;
    uint8_t pkt[MaxExpectedBytes];
    uint8_t pktOffset = ieee802154PhrLen; // No need to parse the PHR byte(s)
    RAIL_GetRxIncomingPacketInfo(railHandle, &packetInfo);
    if (packetInfo.packetBytes < (pktOffset + 2U)) {
      counters.ackTxFpAddrFail++;
      return;
    }
    // Only extract what we care about
    if (packetInfo.packetBytes > MaxExpectedBytes) {
      packetInfo.packetBytes = MaxExpectedBytes;
      if (packetInfo.firstPortionBytes >= MaxExpectedBytes) {
        packetInfo.firstPortionBytes = MaxExpectedBytes;
        packetInfo.lastPortionData = NULL;
      }
    }
    RAIL_CopyRxPacket(pkt, &packetInfo);
    uint16_t macFcf = pkt[pktOffset++];
    if ((macFcf & MAC_FRAME_TYPE_MASK) == MAC_FRAME_TYPE_MULTIPURPOSE) {
      // Multipurpose frames have an arcane FCF structure
      if ((macFcf & MAC_MP_FRAME_FLAG_LONG_FCF) != 0U) {
        macFcf |= (pkt[pktOffset++] << 8);
      }
      // Map Multipurpose FCF to a 'normal' Version 2012 FCF
      // as best we can.
      macFcf = (MAC_FRAME_TYPE_MULTIPURPOSE
                | ((macFcf & (MAC_MP_FRAME_FLAG_SECURITY_ENABLED
                              | MAC_MP_FRAME_FLAG_IE_LIST_PRESENT)) >> 6)
                | ((macFcf & MAC_MP_FRAME_FLAG_FRAME_PENDING) >> 7)
                | ((macFcf & MAC_MP_FRAME_FLAG_ACK_REQUIRED) >> 9)
                | ((macFcf & (MAC_MP_FRAME_FLAG_PANID_PRESENT
                              | MAC_MP_FRAME_FLAG_SEQ_SUPPRESSION)) >> 2)
                | ((macFcf & MAC_MP_FRAME_DESTINATION_MODE_MASK) << 6)
                | MAC_FRAME_VERSION_2012
                | ((macFcf & MAC_MP_FRAME_SOURCE_MODE_MASK) << 8)
                );
      // MultiPurpose's PANID_PRESENT is not equivalent to 2012/5's
      // PANID_COMPRESSION so we map it best we can by flipping it
      // in the following address-combination situations:
      uint16_t addrCombo = (macFcf & (MAC_FRAME_SOURCE_MODE_MASK | MAC_FRAME_DESTINATION_MODE_MASK));
      if ((addrCombo == (MAC_FRAME_SOURCE_MODE_NONE | MAC_FRAME_DESTINATION_MODE_NONE))
          || (addrCombo == (MAC_FRAME_SOURCE_MODE_SHORT | MAC_FRAME_DESTINATION_MODE_SHORT))
          || (addrCombo == (MAC_FRAME_SOURCE_MODE_SHORT | MAC_FRAME_DESTINATION_MODE_LONG))
          || (addrCombo == (MAC_FRAME_SOURCE_MODE_LONG | MAC_FRAME_DESTINATION_MODE_SHORT))
          ) {
        // 802.15.4-2015 PANID_COMPRESSION = MP PANID_PRESENT
      } else {
        // 802.15.4-2015 PANID_COMPRESSION = !MP PANID_PRESENT
        macFcf ^= MAC_FRAME_FLAG_PANID_COMPRESSION; // Flip it
      }
    } else {
      macFcf |= (pkt[pktOffset++] << 8);
    }
    bool enhAck = ((macFcf & MAC_FRAME_VERSION_MASK) == MAC_FRAME_VERSION_2012);
    // Compress MAC FCF to index into 64-entry address-length table:
    // SrcAdrMode FrameVer<msbit> DstAdrMode PanIdCompression
    uint16_t index = (((macFcf & (MAC_FRAME_SOURCE_MODE_MASK | MAC_FRAME_VERSION_2012)) >> 10)
                      | ((macFcf & MAC_FRAME_DESTINATION_MODE_MASK) >> 9)
                      | ((macFcf & MAC_FRAME_FLAG_PANID_COMPRESSION) >> 6));
    uint16_t addrSizes = ieee802154Table7p2[index];
    // Illegal combinations mean illegal packets which we ignore
    if (addrSizes == 0xFFFFU) {
      counters.ackTxFpAddrFail++;
      return;
    }
    uint8_t seqNo = ((enhAck && ((macFcf & MAC_FRAME_FLAG_SEQ_SUPPRESSION) != 0U))
                     ? 0U : pkt[pktOffset++]); // Seq#
    if (enhAck) {
      // Enhanced ACK -- need to construct it since RAIL cannot.
      // First extract addresses from incoming packet since we may
      // need to reflect them in a different order in the outgoing ACK.
      // Use byte[0] to hold each one's length.
      uint8_t dstPan[3] = { 0, }; // Initialized only to eliminate false gcc warning
      dstPan[0] = ((addrSizes & ADDRSIZE_DST_PAN_MASK) >> ADDRSIZE_DST_PAN_SHIFT);
      if ((dstPan[0] + pktOffset) > packetInfo.packetBytes) {
        counters.ackTxFpAddrFail++;
        return;
      }
      if (dstPan[0] > 0U) {
        dstPan[1] = pkt[pktOffset++];
        dstPan[2] = pkt[pktOffset++];
      }
      uint8_t dstAdr[9];
      dstAdr[0] = ((addrSizes & ADDRSIZE_DST_ADR_MASK) >> ADDRSIZE_DST_ADR_SHIFT);
      if ((dstAdr[0] + pktOffset) > packetInfo.packetBytes) {
        counters.ackTxFpAddrFail++;
        return;
      }
      for (uint8_t i = 1U; i <= dstAdr[0]; i++) {
        dstAdr[i] = pkt[pktOffset++];
      }
      uint8_t srcPan[3];
      srcPan[0] = ((addrSizes & ADDRSIZE_SRC_PAN_MASK) >> ADDRSIZE_SRC_PAN_SHIFT);
      if ((srcPan[0] + pktOffset) > packetInfo.packetBytes) {
        counters.ackTxFpAddrFail++;
        return;
      }
      if (srcPan[0] > 0U) {
        srcPan[1] = pkt[pktOffset++];
        srcPan[2] = pkt[pktOffset++];
      }
      uint8_t srcAdr[9];
      srcAdr[0] = ((addrSizes & ADDRSIZE_SRC_ADR_MASK) >> ADDRSIZE_SRC_ADR_SHIFT);
      if ((srcAdr[0] + pktOffset) > packetInfo.packetBytes) {
        counters.ackTxFpAddrFail++;
        return;
      }
      for (uint8_t i = 1U; i <= srcAdr[0]; i++) {
        srcAdr[i] = pkt[pktOffset++];
      }

      // Reuse packet[] buffer for outgoing Enhanced ACK.
      // Phr1 Phr2 FcfL FcfH [Seq#] [DstPan] [DstAdr] [SrcPan] [SrcAdr]
      // Will fill in PHR later.
      // MAC Fcf:
      // - Frame Type = ACK
      // - Security Enabled = 0 in this implementation
      // - Frame Pending = 0 or as appropriate
      // - ACK Request = 0
      // - PanId compression = incoming packet's
      // - Seq# suppression = incoming packet's
      // - IE Present = 0 in this implementation
      // - DstAdrMode = SrcAdrMode of incoming packet's
      // - Frame Version = 2 (154E)
      // - SrcAdrMode = DstAdrMode of incoming packet's (for convenience)
      uint16_t ackFcf = (MAC_FRAME_TYPE_ACK
                         | (macFcf & MAC_FRAME_FLAG_PANID_COMPRESSION)
                         | (macFcf & MAC_FRAME_FLAG_SEQ_SUPPRESSION)
                         | MAC_FRAME_VERSION_2012
                         | ((macFcf & MAC_FRAME_SOURCE_MODE_MASK) >> 4)
                         | ((macFcf & MAC_FRAME_DESTINATION_MODE_MASK) << 4)
                         );
      // Do frame-pending check now
      if ((srcAdr[0] > 0U) && (srcAdr[1] == 0xAAU)) {
        setFramePending = true;
      }
      if (setFramePending) {
        ackFcf |= MAC_FRAME_FLAG_FRAME_PENDING;
      }
      pktOffset = ieee802154PhrLen;
      pkt[pktOffset++] = (uint8_t)ackFcf;
      pkt[pktOffset++] = (uint8_t)(ackFcf >> 8);

      if ((macFcf & MAC_FRAME_FLAG_SEQ_SUPPRESSION) == 0U) {
        pkt[pktOffset++] = seqNo;
      }
      // Determine outgoing ACK's address field sizes
      index = (((ackFcf & (MAC_FRAME_SOURCE_MODE_MASK | MAC_FRAME_VERSION_2012)) >> 10)
               | ((ackFcf & MAC_FRAME_DESTINATION_MODE_MASK) >> 9)
               | ((ackFcf & MAC_FRAME_FLAG_PANID_COMPRESSION) >> 6));
      addrSizes = ieee802154Table7p2[index];
      if (addrSizes == 0xFFFFU) {
        // Uh-oh! Enh-ACK would be malformed?!  Something funky happened!
        // Possibly a latency-induced issue.
        counters.ackTxFpAddrFail++;
        return;
      }
      // DstPan = SrcPan of incoming if avail otherwise DstPan of incoming
      if ((addrSizes & ADDRSIZE_DST_PAN_MASK) != 0U) {
        if (srcPan[0] > 0U) {
          pkt[pktOffset++] = srcPan[1];
          pkt[pktOffset++] = srcPan[2];
        } else if (dstPan[0] > 0U) {
          pkt[pktOffset++] = dstPan[1];
          pkt[pktOffset++] = dstPan[2];
        } else {
          // Uh-oh! Outgoing packet needs a DstPanId but incoming had neither!
          // Possibly a latency-induced issue.
          counters.ackTxFpAddrFail++;
          return;
        }
      }
      // DstAdr = SrcAdr of incoming packet -- their sizes should match
      if ((addrSizes & ADDRSIZE_DST_ADR_MASK) != 0U) {
        for (uint8_t i = 1U; i <= srcAdr[0]; i++) {
          pkt[pktOffset++] = srcAdr[i];
        }
      }
      // SrcPan = DstPan of incoming if avail otherwise SrcPan of incoming
      if ((addrSizes & ADDRSIZE_SRC_PAN_MASK) != 0U) {
        if (dstPan[0] > 0U) {
          pkt[pktOffset++] = dstPan[1];
          pkt[pktOffset++] = dstPan[2];
        } else if (srcPan[0] > 0U) {
          pkt[pktOffset++] = srcPan[1];
          pkt[pktOffset++] = srcPan[2];
        } else {
          // Uh-oh! Outgoing packet needs a SrcPanId but incoming had neither!
          // Possibly a latency-induced issue.
          counters.ackTxFpAddrFail++;
          return;
        }
      }
      // SrcAdr = DstAdr of incoming packet -- their sizes should match
      if ((addrSizes & ADDRSIZE_SRC_ADR_MASK) != 0U) {
        for (uint8_t i = 1U; i <= dstAdr[0]; i++) {
          pkt[pktOffset++] = dstAdr[i];
        }
      }

      // Fill in PHR now that we know Enh-ACK's length
      if (ieee802154PhrLen == 2U) {
        pkt[0] = (0x08U /*FCS=2byte*/ | 0x10U /*Whiten=enabled*/);
        pkt[1] = (uint8_t)(__RBIT(pktOffset - 2U /*PHRLen*/ + 2U /*FCS*/) >> 24);
      } else {
        pkt[0] = (pktOffset - 1U /*PHRLen*/ + 2U /*FCS*/);
      }
      if (RAIL_IEEE802154_WriteEnhAck(railHandle, pkt, pktOffset)
          == RAIL_STATUS_NO_ERROR) {
        if (setFramePending) {
          counters.ackTxFpSet++;
        }
      } else if (setFramePending) {
        counters.ackTxFpFail++;
      }
      return;
    }

    // Immediate ACK -- RAIL will send, but may need to do FP lookup.
    uint16_t srcAdrLen = ((addrSizes & ADDRSIZE_SRC_ADR_MASK) >> ADDRSIZE_SRC_ADR_SHIFT);
    if (srcAdrLen == 0U) {
      // No source address info to do FP lookup
      counters.ackTxFpAddrFail++;
      return;
    }
    // Skip over DstPan, DstAdr, SrcPan to get to SrcAdr
    pktOffset += (((addrSizes & ADDRSIZE_DST_PAN_MASK) >> ADDRSIZE_DST_PAN_SHIFT)
                  + ((addrSizes & ADDRSIZE_DST_ADR_MASK) >> ADDRSIZE_DST_ADR_SHIFT)
                  + ((addrSizes & ADDRSIZE_SRC_PAN_MASK) >> ADDRSIZE_SRC_PAN_SHIFT));
    // At this point we expect the addressing info to be there
    if ((srcAdrLen + pktOffset) > packetInfo.packetBytes) {
      counters.ackTxFpAddrFail++;
      return; // Oops, maybe latency caused us to see a later incoming frame
    }
    // Placeholder validation for when a data request should have the frame
    // pending bit set in the ACK.
    // First byte of short or long address being 0xAA has frame pending
    setFramePending = (pkt[pktOffset] == 0xAAU);
  } else {
    if (RAIL_IEEE802154_GetAddress(railHandle, &address)
        != RAIL_STATUS_NO_ERROR) {
      counters.ackTxFpAddrFail++;
      return;
    }
    // Placeholder validation for when a data request should have the frame
    // pending bit set in the ACK.
    setFramePending = (((address.length == RAIL_IEEE802154_LongAddress)
                        && (address.longAddress[0] == 0xAA))
                       || ((address.shortAddress & 0xFF) == 0xAA));
  }
  if (setFramePending) {
    if (RAIL_IEEE802154_SetFramePending(railHandle) == RAIL_STATUS_NO_ERROR) {
      counters.ackTxFpSet++;
    } else {
      counters.ackTxFpFail++;
    }
  }
}
