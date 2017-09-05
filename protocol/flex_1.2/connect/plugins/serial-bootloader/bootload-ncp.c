// ****************************************************************************
// * bootload-ncp.c
// *
// * Routines for bootloading an NCP UART.
// *
// * Copyright 2015 Silicon Laboratories, Inc.
// *****************************************************************************

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "stack/include/ember.h"
#include "bootload-ncp.h"
#include "bootload-xmodem.h"

#include <string.h>     // ""
#include <stdio.h>      // fopen, fread, fwrite, fclose, fseek, fprintf
#include <sys/types.h>  // stat
#include <sys/stat.h>   // ""
#include <unistd.h>     // ""
#include <errno.h>      // errno, strerror

//------------------------------------------------------------------------------
// Globals

// Xmodem requires all blocks be 128 bytes in size
#define TRANSFER_BLOCK_SIZE 128

// Until we have a formal facility for this code, we just use core.
#define bootloadPrintf(...) printf(__VA_ARGS__)

// We arbitrarily chose 5% as the minimum update amount when we are
// transfering the file to the NCP.  This provides a good amount of feedback
// during the process but not too much.
#define BOOTLOAD_PERCENTAGE_UPDATE  5

#if !defined(EZSP_SPI)
// UART assumed
  #define START_IMMEDIATELY false
#else
// SPI assumed
  #define START_IMMEDIATELY true
#endif

//------------------------------------------------------------------------------
// Forward Declarations

static bool transferFile(const char* filepath, uint32_t offset, uint32_t length);
static uint8_t printPercentage(PGM_P prefixString,
                               uint8_t updateFrequency,
                               uint32_t offsetStart,
                               uint32_t currentOffset,
                               uint32_t endOffset);

//------------------------------------------------------------------------------

// This hands control of the entire application to this code to perform
// the bootloading.  It will not return until the bootload has completed
// or it has failed. It is assumed that the bootloader is already running
// on the NCP prior to the invokation of this function.

bool emberBootloadImage(const char* filepath, uint32_t offset, uint32_t length)
{
  bool success;

  bootloadPrintf("Starting bootloader communications.\n");
  if (!emStartNcpBootloaderCommunications()) {
    bootloadPrintf("Failed to start bootloading communications.\n");
    return false;
  }

  // If the length is not given assume that we are sending everything to the
  // end of the file.
  if (length == 0xFFFFFFFF) {
    struct stat st;
    if (stat(filepath, &st) != 0) {
      bootloadPrintf("Cannot determine size of %s: %s\n",
                     filepath, strerror(errno));
      return false;
    }
    length = (uint32_t)st.st_size - offset;
  }

  success = transferFile(filepath, offset, length);
  // Regardless of success or failure we reboot the NCP in hopes
  // of returning the system back to its previous state.
  // Use &= here to preserve the possible failed status returned
  // by transferFile()
  success &= emRebootNcpAfterBootload();
  emPostNcpBootload(success);
  return success;
}

static bool transferFile(const char* filepath, uint32_t offset, uint32_t length)
{
  uint32_t endOffset = offset + length;
  char* filenameStart = strrchr(filepath, '/');

  if (filenameStart == NULL) {
    filenameStart = (char*)filepath;
  } else {
    filenameStart++;  // +1 for the '/' character
  }

  // We extract the EBL (upgrade image) from the file
  // and pass it to the bootloader.  The bootloader
  // has no knowledge of the original file format,
  // only EBL images.

  bootloadPrintf("Transferring EBL file to NCP...\n");

  // Windows requires the 'b' (binary) as part of the mode so that line endings
  // are not truncated.  POSIX ignores this.
  FILE* fileHandle = fopen(filepath, "rb");
  if (fileHandle == NULL) {
    bootloadPrintf("Error: Failed to open file '%s' for reading: %s\n",
                   filenameStart,
                   strerror(errno));
    return false;
  }

  printPercentage(NULL,           // prefix string (ignored)
                  0,              // update frequency (ignored)
                  offset,
                  0,              // current offset (ignored)
                  endOffset);
  bootloadPrintf("EBL data start: 0x%4X, end: 0x%4X, size: %d bytes\n",
                 offset,
                 endOffset,
                 endOffset - offset);

  emInitXmodemState(START_IMMEDIATELY);

  while (offset < endOffset) {
    uint32_t returnedLength;
    uint32_t readSize = TRANSFER_BLOCK_SIZE;
    uint8_t block[TRANSFER_BLOCK_SIZE];

    MEMSET(block, 0, TRANSFER_BLOCK_SIZE);

    if ((endOffset - offset) < TRANSFER_BLOCK_SIZE) {
      readSize = endOffset - offset;
    }

    if (0 != fseek(fileHandle, offset, SEEK_SET)) {
      bootloadPrintf("Error: Failed to seek in file '%s' to offset %d\n",
                     filenameStart,
                     offset);
      fclose(fileHandle);
      return false;
    }
    returnedLength = fread(block, 1, readSize, fileHandle);
    if (returnedLength != readSize) {
      bootloadPrintf("Failed to read image data at offset 0x%4X\n", offset);
      fclose(fileHandle);
      return false;
    }

    offset += returnedLength;
    if (!emSendXmodemData(block,
                          (uint16_t)returnedLength,
                          (offset == endOffset))) {   // finish?
      bootloadPrintf("Failed to send data to NCP.\n");
      fclose(fileHandle);
      return false;
    }

    printPercentage("Transfer",
                    BOOTLOAD_PERCENTAGE_UPDATE,
                    0,               // offset start (ignored)
                    offset,
                    0);              // offset end (ignored)
  }

  fclose(fileHandle);
  bootloadPrintf("Transfer completed successfully.\n");
  return true;
}

// 32-bit math may be expensive and is only needed for printing
// purposes.  So this functionality should be conditionally compiled in.

// The 'currentOffset' and 'endOffset' parameters are relative to the static
// variable 'startingOffset'.  It is expected the 'startingOffset' is set by a
// previous call that did nothing but pass in the 'offsetStart' value
// (other parameters are ignored in that case).
static uint8_t printPercentage(PGM_P prefixString,
                               uint8_t updateFrequency,
                               uint32_t offsetStart,
                               uint32_t currentOffset,
                               uint32_t endOffset)
{
  static uint8_t percentageComplete = 0;
  static uint8_t oldPercentageComplete = 255; // invalid value
  static uint32_t startingOffset = 0;
  static uint32_t imageSize = 0;

  if (offsetStart > 0 || endOffset > 0) {
    oldPercentageComplete = 255;
    startingOffset = offsetStart;
    imageSize = endOffset - startingOffset;
    return 0;
  }
  // The rest of this code should NOT use 'offsetStart', but 'startingOffset'.

  currentOffset -= startingOffset;

  percentageComplete = (imageSize == 0) ? 0
                       : ((currentOffset >= imageSize) ? 100
                          : ((currentOffset * 100) / imageSize));

  if (oldPercentageComplete == 255
      || (percentageComplete >= updateFrequency
          && (percentageComplete
              >= (oldPercentageComplete + updateFrequency)))) {
    bootloadPrintf("%p: %d%% complete\n", prefixString, percentageComplete);
    oldPercentageComplete = percentageComplete;
  }
  return percentageComplete;
}
