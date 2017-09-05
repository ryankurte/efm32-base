// ****************************************************************************
// * bootload-ncp-app.c
// *
// * This application provides the platform specific means to bootload an NCP
// * over SPI or UART. This host code assumes that the serial standalone bootloader
// * is already running on the NCP. It transfers a file over to the NCP via the
// * xmodem protocol then reboots the NCP.
// *
// * Copyright 2015 Silicon Laboratories, Inc.
// *****************************************************************************

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "stack/include/ember.h"
#include "ezsp/ezsp-enum.h"
#include "ezsp-host/ezsp-host-ui.h"
// #include "hal/micro/unix/host/spi-protocol-common.h"
#include "bootload-ncp.h"

#include <stdio.h>
#include <stdlib.h>

//------------------------------------------------------------------------------
// Preprocessor definitions

#define ERR_LEN               128   // max length error message

#if !defined(EZSP_SPI)
static void printResults(EzspStatus status)
{
  if (status == EZSP_SUCCESS) {
    printf("succeeded.\n");
    return;
  } else {
    printf("failed.\n");
    switch (status) {
      case EZSP_HOST_FATAL_ERROR:
        printf("Host error: %s (0x%02X).\n", ezspErrorString(hostError), hostError);
        break;
      case EZSP_ASH_NCP_FATAL_ERROR:
        printf("NCP error: %s (0x%02X).\n", ezspErrorString(ncpError), ncpError);
        break;
      default:
        printf("\nEZSP error: %s (0x%02X).\n", ezspErrorString(status), status);
        break;
    }
  }
  exit(1);
}
#endif

static void printUsage(char *name)
{
  char *shortName = strrchr(name, '/');
  shortName = shortName ? shortName + 1 : name;
  fprintf(stderr, "Usage: %s <image-path> <begin-offset> <length> [serial options]\n", shortName);
  fprintf(stderr, "  <image-path>     required - pathname to image\n");
  fprintf(stderr, "  <begin-offset>   required - offset within image to begin\n");
  fprintf(stderr, "  <length>         required - number of bytes to send, 0xFFFFFFFF = everything to end of file\n");
  fprintf(stderr, "  [serial options]%s\n", usage);
}

int main(MAIN_FUNCTION_PARAMETERS)
{
  char* progName = argv[0];
  char* filepath;
  uint32_t offset = 0;
  uint32_t length = 0xFFFFFFFF;
  char errStr[ERR_LEN] = "";

  // The first 3 arguments must specifiy information about the
  // image to be bootloaded: filepath, begin offset, and length.
  // Following these arguments can be the SPI or UART interface
  // specific options.
  if (argc < 4) {
    fprintf(stderr, "Invalid number of arguments.\n");
    printUsage(progName);
    return 1;
  }

  filepath = argv[1];
  offset = (uint32_t)atoi(argv[2]);
  length = (uint32_t)atoi(argv[3]);

  if (argc > 4) {
    // Skip required args.
    argv[3] = progName;
    argv += 3; argc -= 3;
    if (!ezspInternalProcessCommandOptions(argc, argv, errStr)) {
      return 1;
    }
    if (*errStr != '\0') {
      fprintf(stderr, "%s", errStr);
      printUsage(progName);
      return 1;
    }
  }

#if !defined(EZSP_SPI)
  // Open serial port - verifies the name and permissions
  // Disable RTS/CTS since the open might fail if CTS is not asserted.
  printf("\nOpening serial port... ");
  ashWriteConfig(resetMethod, ASH_RESET_METHOD_NONE);
  ashWriteConfig(rtsCts, false);
  printResults(ashResetNcp());           // opens the serial port
#else
  // halNcpEnableLogging(EMBER_APP_NAME ".log");
  halNcpSerialInit();
#endif

  bool success = emberBootloadImage(filepath, offset, length);
  if (success) {
    printf("Image loaded.\n");
  } else {
    printf("Image failed to load.\n");
  }

  return success ? 0 : 1;
}

// STUBS that are not used, but required to build
#if !defined(EZSP_SPI)
bool ncpHasCallbacks = false;
#endif

#ifdef EZSP_SPI
void halNcpIsAwakeIsr(bool isAwake)
{
}
#endif
