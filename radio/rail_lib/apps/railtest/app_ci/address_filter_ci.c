/***************************************************************************//**
 * @file
 * @brief This file implements the address filtering commands in RAIL test apps.
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
#include "app_common.h"
#include "app_ci.h"

// Define for the maximum number of filter fields
#define ADDRFILT_FIELD_COUNT (2)
#define ADDRFILT_ENTRY_COUNT (4)
#define ADDRFILT_ENTRY_SIZE (8)

/// This holds the addresses used by this application. We get 4 addresses per
/// field of up to 8 bytes in length.
static uint8_t addresses[ADDRFILT_FIELD_COUNT * ADDRFILT_ENTRY_COUNT][ADDRFILT_ENTRY_SIZE];

/// Track which addresses we have enabled
static bool addressEnabled[ADDRFILT_FIELD_COUNT][ADDRFILT_ENTRY_COUNT];

/// The current address field configuration
static RAIL_AddrConfig_t config = { { 0, 0 }, { 0, 0 }, 0 };

/// Buffer to help print address values to the screen
static char addressPrintBuffer[(ADDRFILT_ENTRY_SIZE * 5) + 8];

void setAddressFilterConfig(int argc, char **argv)
{
  int i = 0, count = ADDRFILT_FIELD_COUNT * 2;

  // Reset the config struct to zero as the default for everything
  memset(addresses, 0, sizeof(addresses));
  memset(addressEnabled, 0, sizeof(addressEnabled));

  // Set the match table in the config struct
  config.matchTable = ciGetUnsigned(argv[1]);

  // Set count to the maximum allowed size or the maximum provided parameters
  if ((argc - 2) < count) {
    count = argc - 2;
  }

  // Set as many offset and sizes as we can
  for (i = 0; i < count; i++) {
    if ((i % 2) == 0) {
      config.offsets[i / 2] = ciGetUnsigned(argv[i + 2]);
    } else {
      config.sizes[i / 2] = ciGetUnsigned(argv[i + 2]);
    }
  }

  if (RAIL_ConfigAddressFilter(railHandle, &config)
      == RAIL_STATUS_NO_ERROR) {
    printAddresses(1, argv);
  } else {
    responsePrintError(argv[0], 0x30, "Invalid address filtering configuration.");
  }
}

void setAddressFilter(int argc, char **argv)
{
  uint32_t enable = ciGetUnsigned(argv[1]);

  RAIL_EnableAddressFilter(railHandle, !!enable);

  getAddressFilter(1, argv);
}

void getAddressFilter(int argc, char **argv)
{
  bool filteringEnabled = RAIL_IsAddressFilterEnabled(railHandle);

  responsePrint(argv[0],
                "AddressFiltering:%s",
                filteringEnabled ? "Enabled" : "Disabled");
}

void printAddresses(int argc, char **argv)
{
  int i, j, k;

  responsePrintHeader(argv[0],
                      "Field:%u,Index:%u,Offset:%u,"
                      "Size:%u,Address:%s,Status:%s");
  for (i = 0; i < ADDRFILT_FIELD_COUNT; i++) {
    for (j = 0; j < ADDRFILT_ENTRY_COUNT; j++) {
      int offset = 0;

      if (config.sizes[i] > 0) {
        // offset = sprintf(addressPrintBuffer, "0x");
        for (k = 0; k < config.sizes[i]; k++) {
          offset += sprintf(addressPrintBuffer + offset,
                            "0x%.2x ",
                            addresses[(i * ADDRFILT_ENTRY_COUNT) + j][k]);
        }
        if (offset > 0) {
          addressPrintBuffer[offset - 1] = '\0'; // Get rid of the last space
        }
      } else {
        addressPrintBuffer[0] = '\0';
      }
      responsePrintMulti("Field:%u,Index:%u,Offset:%u,"
                         "Size:%u,Address:%s,Status:%s",
                         i,
                         j,
                         config.offsets[i],
                         config.sizes[i],
                         addressPrintBuffer,
                         addressEnabled[i][j] ? "Enabled" : "Disabled");
    }
  }
}

void setAddress(int argc, char **argv)
{
  int i;
  RAIL_Status_t result;
  uint8_t field = ciGetUnsigned(argv[1]);
  uint8_t index = ciGetUnsigned(argv[2]);
  uint8_t address[ADDRFILT_ENTRY_SIZE];
  int location = field * ADDRFILT_ENTRY_COUNT + index;

  // Make sure the field and index parameters are in range
  if (field >= ADDRFILT_FIELD_COUNT || index >= ADDRFILT_ENTRY_COUNT) {
    responsePrintError(argv[0], 0x31, "Address field or index out of range!");
    return;
  }

  // Make sure this isn't too large of an entry
  if (argc > (ADDRFILT_ENTRY_SIZE + 3)) {
    responsePrintError(argv[0], 0x32, "Too many address bytes given!");
    return;
  }

  // Read out all the bytes given into the address cache
  memset(address, 0, sizeof(address));
  for (i = 3; i < argc; i++) {
    address[(i - 3)] = ciGetUnsigned(argv[i]);
  }

  result = RAIL_SetAddressFilterAddress(railHandle,
                                        field,
                                        index,
                                        address,
                                        addressEnabled[field][index]);
  if (result != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x33, "Could not configure address!");
    return;
  }
  // Copy this into our local structure for tracking
  memcpy(addresses[location], address, sizeof(address));

  printAddresses(1, argv);
}

void enableAddress(int argc, char **argv)
{
  uint8_t field = ciGetUnsigned(argv[1]);
  uint8_t index = ciGetUnsigned(argv[2]);
  uint8_t enable = ciGetUnsigned(argv[3]);
  RAIL_Status_t result;

  result = RAIL_EnableAddressFilterAddress(railHandle, !!enable, field, index);

  if (result != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x34, "Could not enable/disable address!");
    return;
  } else {
    addressEnabled[field][index] = (enable != 0);
    responsePrint(argv[0],
                  "Field:%u,Index:%u,Offset:%u,"
                  "Size:%u,Address:%s,Status:%s",
                  field,
                  index,
                  config.offsets[field],
                  config.sizes[field],
                  addressPrintBuffer,
                  addressEnabled[field][index] ? "Enabled" : "Disabled");
  }
}
