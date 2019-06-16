/***************************************************************************//**
 * @file
 * @brief Source file for RAIL Ram Modem Reconfiguration functionality
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

#include <stdlib.h>
#include <stdint.h>

#include "rail.h"
#include "em_common.h"

#include "railapp_rmr.h"
#include "app_common.h"
#include "command_interpreter.h"
#include "response_print.h"

#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

static uint32_t rmr_phyInfo[RMR_PHY_INFO_LEN];
static uint8_t rmr_irCalConfig[RMR_IRCAL_LEN];
static uint32_t rmr_modemConfigEntry[RMR_MODEM_CONFIG_LEN];
static RAIL_FrameType_t rmr_frameTypeConfig;
static uint16_t rmr_frameLenList[RMR_FRAME_LENGTH_LIST_LEN];
static uint32_t rmr_frameCodingTable[RMR_FRAME_CODING_TABLE_LEN];
static RAIL_ChannelConfigEntryAttr_t rmr_generatedEntryAttr;
static RAIL_ChannelConfigEntry_t rmr_generatedChannels[1];
SL_ALIGN(4) static uint8_t rmr_convDecodeBuffer[RMR_CONV_DECODE_BUFFER_LEN] SL_ATTRIBUTE_ALIGN(4);

const RAIL_ChannelConfig_t rmr_ChannelConfig = {
  rmr_modemConfigEntry,
  NULL,
  rmr_generatedChannels,
  1,
  0U,
};

const RAIL_ChannelConfig_t *rmr_ChannelConfigs[] = {
  &rmr_ChannelConfig
};

// Internal commands
RAIL_Status_t Rmr_writeRmrStructure(RAIL_RMR_StructureIndex_t structure, uint16_t offset, uint8_t count, uint8_t *dataPtr);
RAIL_Status_t Rmr_updateConfigurationPointer(uint8_t structToModify, uint16_t offset, uint8_t structToPointTo);
RAIL_Status_t Rmr_reconfigureModem(RAIL_Handle_t railHandle);

//----------------------------------------------------------------------------
// Ram Modem Reconfiguration Commands
//-----------------------------------------------------------------------------
RAIL_Status_t Rmr_updateConfigurationPointer(uint8_t structToModify, uint16_t offset, uint8_t structToPointTo)
{
  uint32_t structPointer = 0u; // NULL
  // First get the addres of the structure we are trying to reference
  switch (structToPointTo) {
    case (RMR_STRUCT_PHY_INFO): {
      structPointer = (uint32_t)&rmr_phyInfo;
      break;
    }
    case (RMR_STRUCT_IRCAL_CONFIG): {
      structPointer = (uint32_t)&rmr_irCalConfig;
      break;
    }
    case (RMR_STRUCT_FRAME_TYPE_CONFIG): {
      structPointer = (uint32_t)&rmr_frameTypeConfig;
      break;
    }
    case (RMR_STRUCT_FRAME_LENGTH_LIST): {
      structPointer = (uint32_t)&rmr_frameLenList;
      break;
    }
    case (RMR_STRUCT_FRAME_CODING_TABLE): {
      structPointer = (uint32_t)rmr_frameCodingTable;
      break;
    }
    case (RMR_STRUCT_CONV_DECODE_BUFFER): {
      structPointer = (uint32_t)&rmr_convDecodeBuffer;
      break;
    }
    case (RMR_STRUCT_NULL): {
      structPointer = 0u; // NULL
      break;
    }
    default: {
      // Error, unrecognized structure
      return RAIL_STATUS_INVALID_PARAMETER;
    }
  }
  switch (structToModify) {
    case (RMR_STRUCT_MODEM_CONFIG): {
      if (offset >= RMR_MODEM_CONFIG_LEN) {
        return RAIL_STATUS_INVALID_PARAMETER;
      }
      rmr_modemConfigEntry[offset] = structPointer;
      break;
    }
    case (RMR_STRUCT_PHY_INFO): {
      if (offset >= RMR_PHY_INFO_LEN) {
        return RAIL_STATUS_INVALID_PARAMETER;
      }
      rmr_phyInfo[offset] = structPointer;
      break;
    }
    case (RMR_STRUCT_FRAME_TYPE_CONFIG): {
      if (offset != 0) {
        return RAIL_STATUS_INVALID_PARAMETER;
      }
      rmr_frameTypeConfig.frameLen = (uint16_t *)structPointer;
      break;
    }
    default: {
      // Error unrecognized structure
      return RAIL_STATUS_INVALID_PARAMETER;
    }
  }
  return RAIL_STATUS_NO_ERROR;
}

RAIL_Status_t Rmr_reconfigureModem(RAIL_Handle_t railHandle)
{
  RAIL_RadioState_t currentState = RAIL_GetRadioState(railHandle);
  if (currentState != RAIL_RF_STATE_IDLE) {
    return RAIL_STATUS_INVALID_STATE;
  }

  // Always include frame type length functionality when using RMR or config
  // channels won't work for frame type based lengths.
  RAIL_IncludeFrameTypeLength(railHandle);

  // Configure with the downloaded channel configuration.
  RAIL_ConfigChannels(railHandle, rmr_ChannelConfigs[0], &RAILCb_RadioConfigChanged);

  // Make sure that we stay in idle after the reconfiguration.
  RAIL_Idle(railHandle, RAIL_IDLE_FORCE_SHUTDOWN_CLEAR_FLAGS, false);

  return RAIL_STATUS_NO_ERROR;
}

//-----------------------------------------------------------------------------
// RMR CI Commands
//-----------------------------------------------------------------------------
RAIL_Status_t Rmr_writeRmrStructure(RAIL_RMR_StructureIndex_t structure, uint16_t offset, uint8_t count, uint8_t *dataPtr)
{
  uint8_t *targetStruct;
  uint32_t size;
  switch (structure) {
    case (RMR_STRUCT_PHY_INFO): {
      size = sizeof(rmr_phyInfo);
      targetStruct = (uint8_t *) &rmr_phyInfo;
      break;
    }
    case (RMR_STRUCT_IRCAL_CONFIG): {
      size = sizeof(rmr_irCalConfig);
      targetStruct = (uint8_t *)&rmr_irCalConfig;
      break;
    }
    case (RMR_STRUCT_MODEM_CONFIG): {
      size = sizeof(rmr_modemConfigEntry);
      targetStruct = (uint8_t *)&rmr_modemConfigEntry;
      break;
    }
    case (RMR_STRUCT_FRAME_TYPE_CONFIG): {
      size = sizeof(rmr_frameTypeConfig);
      targetStruct = (uint8_t *) &rmr_frameTypeConfig;
      break;
    }
    case (RMR_STRUCT_FRAME_LENGTH_LIST): {
      size = sizeof(rmr_frameLenList);
      targetStruct = (uint8_t *) &rmr_frameLenList;
      break;
    }
    case (RMR_STRUCT_FRAME_CODING_TABLE): {
      size = sizeof(rmr_frameCodingTable);
      targetStruct = (uint8_t *) &rmr_frameCodingTable;
      break;
    }
    case (RMR_STRUCT_CHANNEL_CONFIG_ATTRIBUTES): {
      size = sizeof(rmr_generatedEntryAttr);
      targetStruct = (uint8_t *) &rmr_generatedEntryAttr;
      break;
    }
    case (RMR_STRUCT_CHANNEL_CONFIG_ENTRY): {
      size = sizeof(rmr_generatedChannels);
      targetStruct = (uint8_t *) &rmr_generatedChannels;
      break;
    }
    default: {
      return RAIL_STATUS_INVALID_PARAMETER;
      break;
    }
  }
  // Check that we are not writing out of bounds
  if ((offset + count) > size) {
    return RAIL_STATUS_INVALID_PARAMETER;
  }
  targetStruct += offset;
  while (count-- != 0u) {
    *targetStruct = *dataPtr;
    targetStruct++;
    dataPtr++;
  }
  return RAIL_STATUS_NO_ERROR;
}

void CI_writeRmrStructure(int argc, char **argv)
{
  uint8_t count = ciGetUnsigned(argv[RMR_CI_COUNT]);
  uint8_t bufferedData[RMR_ARGUMENT_BUFFER_SIZE];
  if (argc != (count + RMR_CI_DATA_START)) {
    responsePrintError(argv[RMR_CI_RESPONSE], 0x80, "Argument count does not match number of arguments.");
    return;
  }
  if (count > RMR_ARGUMENT_BUFFER_SIZE) {
    responsePrintError(argv[RMR_CI_RESPONSE], 0x81, "Number of arguments greater than local buffer.");
    return;
  }
  RAIL_RMR_StructureIndex_t structure = ciGetUnsigned(argv[RMR_CI_RMR_STRUCTURE]);
  uint16_t offset = ciGetUnsigned(argv[RMR_CI_OFFSET]);
  uint8_t i;
  for (i = 0u; i < count; i++) {
    bufferedData[i] = ciGetUnsigned(argv[RMR_CI_DATA_START + i]);
  }
  if (Rmr_writeRmrStructure(structure, offset, count, bufferedData) != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[RMR_CI_RESPONSE], 0x82, "Error writing to structure.");
    return;
  }
  responsePrint(argv[RMR_CI_RESPONSE], "CommandStatus:Success");
  return;
}

void CI_updateConfigurationPointer(int argc, char **argv)
{
  if (argc != 4) {
    responsePrintError(argv[RMR_CI_RESPONSE], 0x83, "Incorrect number of arguments");
    return;
  }
  uint8_t structure = ciGetUnsigned(argv[1]);
  uint16_t location = ciGetUnsigned(argv[2]);
  uint8_t pointer = ciGetUnsigned(argv[3]);
  if (Rmr_updateConfigurationPointer(structure, location, pointer) != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[RMR_CI_RESPONSE], 0x84, "Error updating structure");
    return;
  }
  responsePrint(argv[RMR_CI_RESPONSE], "CommandStatus:Success");
}

void CI_reconfigureModem(int argc, char **argv)
{
  if (Rmr_reconfigureModem(railHandle) == RAIL_STATUS_NO_ERROR) {
    responsePrint(argv[RMR_CI_RESPONSE], "CommandStatus:Success");
  } else {
    responsePrintError(argv[RMR_CI_RESPONSE], 0x85, "Need to be in Idle radio state for this command");
  }
}
