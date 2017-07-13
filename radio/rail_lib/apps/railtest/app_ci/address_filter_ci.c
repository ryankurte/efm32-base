/***************************************************************************//**
 * @file address_filter_ci.c
 * @brief This file implements the address filtering commands in RAIL test apps.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
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

/// The current address field offsets
static uint8_t offsets[ADDRFILT_FIELD_COUNT];

/// The current address field sizes
static uint8_t sizes[ADDRFILT_FIELD_COUNT];

/// The current address field configuration
static RAIL_AddrConfig_t config = { ADDRFILT_FIELD_COUNT, offsets, sizes, 0 };

/// Buffer to help print address values to the screen
static char addressPrintBuffer[(ADDRFILT_ENTRY_SIZE * 5) + 8];

void setAddressFilterConfig(int argc, char **argv)
{
  int i = 0, count = ADDRFILT_FIELD_COUNT * 2;

  // Reset the config struct to zero as the default for everything
  memset(offsets, 0, sizeof(offsets));
  memset(sizes, 0, sizeof(sizes));
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

  if (RAIL_AddressFilterConfig(&config)) {
    printAddresses(1, argv);
  } else {
    responsePrintError(argv[0], 0x30, "Invalid address filtering configuration.");
  }
}

void addressFilterByFrame(int argc, char **argv)
{
  uint8_t validFrames = ciGetUnsigned(argv[1]);
  if (RAIL_AddressFilterByFrameType(validFrames)) {
    responsePrint(argv[0], "AddressFilterByFrame:Set");
  } else {
    responsePrintError(argv[0], 0x35, "Invalid filtering by frame config.");
  }
}

void setAddressFilter(int argc, char **argv)
{
  uint32_t enable = ciGetUnsigned(argv[1]);

  if (enable == 0) {
    RAIL_AddressFilterDisable();
  } else {
    RAIL_AddressFilterEnable();
  }

  getAddressFilter(1, argv);
}

void getAddressFilter(int argc, char **argv)
{
  bool filteringEnabled = RAIL_AddressFilterIsEnabled();

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

      if (sizes[i] > 0) {
        // offset = sprintf(addressPrintBuffer, "0x");
        for (k = 0; k < sizes[i]; k++) {
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
                         offsets[i],
                         sizes[i],
                         addressPrintBuffer,
                         addressEnabled[i][j] ? "Enabled" : "Disabled");
    }
  }
}

void setAddress(int argc, char **argv)
{
  int i;
  bool result;
  uint8_t field = ciGetUnsigned(argv[1]);
  uint8_t index = ciGetUnsigned(argv[2]);
  uint8_t address[ADDRFILT_ENTRY_SIZE];
  int location = field * ADDRFILT_ENTRY_COUNT + index;

  // Make sure the field and index parameters are in range
  if (field > ADDRFILT_FIELD_COUNT || index > ADDRFILT_ENTRY_COUNT) {
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

  result = RAIL_AddressFilterSetAddress(field,
                                        index,
                                        address,
                                        addressEnabled[field][index]);
  if (!result) {
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
  bool result;

  if (enable == 0) {
    result = RAIL_AddressFilterDisableAddress(field, index);
  } else {
    result = RAIL_AddressFilterEnableAddress(field, index);
  }

  if (!result) {
    responsePrintError(argv[0], 0x34, "Could not enable/disable address!");
    return;
  } else {
    addressEnabled[field][index] = (enable != 0);
    responsePrint(argv[0],
                  "Field:%u,Index:%u,Offset:%u,"
                  "Size:%u,Address:%s,Status:%s",
                  field,
                  index,
                  offsets[field],
                  sizes[field],
                  addressPrintBuffer,
                  addressEnabled[field][index] ? "Enabled" : "Disabled");
  }
}
