/***************************************************************************//**
 * @file
 * @brief CLI implementations for the scripting features of the multiprotocol
 *   app. These features allow a user to upload a series of commands to the
 *   chip to be executed consecutively without waiting for further CLI input
 *   until the uploaded script is completed. This is useful in testing
 *   scenarios where they delay related to entering commands one-by-one
 *   is too great.
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

#if !defined(__ICCARM__)
// IAR doesn't have strings.h and puts those declarations in string.h
#include <strings.h>
#endif

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "app_common.h"
#ifdef EMBER_AF_PLUGIN_FLASH_DATA
#include "flash_data.h"
#endif

// The command script buffer in RAM
static char ramScript[SCRIPT_LENGTH] = { '\0' };

// The pointer used to refer to the command script used in scripted mode.
// Point to the RAM script buffer by default (as opposed to one in flash).
char *script = &ramScript[0];
// The index of the current character being read while in scripted mode
uint16_t scriptMarker = 0;
// The length of the string in script. Memory for 'script' is not allocated
// by default, otherwise we could just use strlen(script)
uint16_t scriptLength = 0;

// Duration of time for which CI suspension is effective
uint32_t suspension = 0;
// Time at which CI suspension started
uint32_t suspensionStartTime = 0;

void printScript(int argc, char **argv)
{
  bool useFlash = (argc >= 2) && !!ciGetUnsigned(argv[1]);
  bool success = true;
  script = &ramScript[0]; // default to script buffer in RAM

  if (useFlash) {
#ifndef EMBER_AF_PLUGIN_FLASH_DATA
    responsePrintError(argv[0], 0x12, "Flash support not enabled");
    return;
#else
    if (RAIL_STATUS_NO_ERROR == FD_ReadData((uint8_t **)&script, NULL)) {
      success = ('\0' == *script) ? false : true;
    } else {
      success = false;
    }
#endif // EMBER_AF_PLUGIN_FLASH_DATA
  } else if ('\0' == *script) {
    success = false;
  }

  responsePrint(argv[0],
                "location:%s,status:%s,script:%s",
                useFlash ? "flash" : "RAM",
                success ? "Success" : "Failure",
                success ? script : "(none)");
}

void enterScript(int argc, char **argv)
{
  bool useFlash = (argc >= 2) && !!ciGetUnsigned(argv[1]);
  bool success = true;

  if (useFlash) {
#ifndef EMBER_AF_PLUGIN_FLASH_DATA
    responsePrintError(argv[0], 0x12, "Flash support not enabled");
    return;
#endif // EMBER_AF_PLUGIN_FLASH_DATA
  }

  // Scripted mode is indicated by scriptMarker being less than scriptLength.
  // Set this all the way to the end of the script so we're definitely not in
  // scripted mode.
  scriptMarker = SCRIPT_LENGTH;
  ramScript[0] = '\0';

  uint16_t index = 0;
  char input;
  bool endScriptFound = false;
  // Read from the input until we hit the max length, or until we hit
  // 'endScript', which gets us out of scriptEntry mode.
  while (index < (SCRIPT_LENGTH - 1)) {
    input = getchar();
    if (input != '\0' && input != 0xFF) {
      ramScript[index] = input;
      index++;

      RAILTEST_PRINTF("%c", input);
      if (input == '\r') {
        RAILTEST_PRINTF("\n"); // retargetserial no longer does CR => CRLF
        ramScript[index] = '\n';
        index++;
      }

      if (index >= sizeof("endScript") - 1
          && strncasecmp("endScript",
                         &(ramScript[index - sizeof("endScript") + 1]),
                         sizeof("endScript") - 1) == 0) {
        RAILTEST_PRINTF("\r\n");
        endScriptFound = true;
        ramScript[index - sizeof("endScript") + 1] = '\0';
        break;
      }
    }
  }

  // Take measures whether or not a script was successfully entered just now.
  if (endScriptFound) {
    scriptLength = strlen(ramScript);
  } else {
    ramScript[0] = '\0';
    success = false;
  }

#ifdef EMBER_AF_PLUGIN_FLASH_DATA
  // Determine if the script should be saved to flash.
  if (useFlash && success) {
    // Only indicate a successful flash if the write was successful.
    if (RAIL_STATUS_NO_ERROR
        != FD_WriteData((uint8_t *)&ramScript[0],
                        scriptLength + 1)) { // + 1 for null terminator
      success = false;
    }
  }
#endif // EMBER_AF_PLUGIN_FLASH_DATA

  // Print what was entered
  responsePrint(argv[0],
                "location:%s,status:%s,script:%s",
                useFlash ? "flash" : "RAM",
                success ? "Success" : "Failure",
                *ramScript == '\0' ? "(none)" : ramScript);
}

void clearScript(int argc, char **argv)
{
  bool useFlash = (argc >= 2) && !!ciGetUnsigned(argv[1]);
  bool success = true;

  // Determine if the script should be cleared from flash or not.
  if (useFlash) {
#ifndef EMBER_AF_PLUGIN_FLASH_DATA
    responsePrintError(argv[0], 0x12, "Flash support not enabled");
    return;
#else
    if (RAIL_STATUS_NO_ERROR != FD_ClearData()) { // clear script in flash
      success = false;
    }
#endif // EMBER_AF_PLUGIN_FLASH_DATA
  } else {
    ramScript[0] = '\0';
  }

  // Scripted mode is indicated by scriptMarker being less than scriptLength
  scriptLength = 0;

  responsePrint(argv[0],
                "location:%s,status:%s",
                useFlash ? "flash" : "RAM",
                success ? "Success" : "Failure");
}

void runScript(int argc, char **argv)
{
  bool useFlash = (argc >= 2) && !!ciGetUnsigned(argv[1]);
  bool success = true;
  script = &ramScript[0]; // default to script in RAM

  // Determine if the script should be run from flash or RAM.
  if (useFlash) {
#ifndef EMBER_AF_PLUGIN_FLASH_DATA
    responsePrintError(argv[0], 0x12, "Flash support not enabled");
    return;
#else
    uint32_t length;
    if (RAIL_STATUS_NO_ERROR == FD_ReadData((uint8_t **)&script, &length)) {
      scriptLength = length - 1; // -1 to remove the NULL string terminator
      success = ('\0' == *script) ? false : true;
    } else {
      success = false;
    }
#endif // EMBER_AF_PLUGIN_FLASH_DATA
  } else if (*script == '\0') {
    success = false;
  } else {
    scriptLength = strlen(script); // NULL string terminator already removed
  }

  // Scripted mode is indicated by scriptMarker being less than scriptLength.
  scriptMarker = success ? 0 : SCRIPT_LENGTH;

  responsePrint(argv[0],
                "location:%s,status:%s",
                useFlash ? "flash" : "RAM",
                success ? "Running" : "Failure");
}

// If there's a script save in flash, run it.
void runFlashScript(void)
{
#ifdef EMBER_AF_PLUGIN_FLASH_DATA
  // Only run a flash script if one exists.
  RAIL_Status_t status = FD_ReadData((uint8_t **)&script, NULL);
  if ((RAIL_STATUS_NO_ERROR == status) && (script[0] != '\0')) {
    char *input[2];
    input[0] = "runScript";
    input[1] = "1";
    runScript(2, input); // emulate CLI command "runScript 1"
  }
#endif // EMBER_AF_PLUGIN_FLASH_DATA
}

void wait(int argc, char **argv)
{
  // Relative time by default
  RAIL_TimeMode_t timeMode = RAIL_TIME_DELAY;

  if (argc > 2 && !parseTimeModeFromString(argv[2], &timeMode)) {
    responsePrintError(argv[0], 0x3, "Invalid time mode");
    return;
  }

  suspensionStartTime = RAIL_GetTime();
  suspension = ciGetUnsigned(argv[1]);

  // In the code, everything is handled as relative time, so convert this
  // to a relative value
  if (timeMode == RAIL_TIME_ABSOLUTE) {
    suspension -= suspensionStartTime;
  }

  responsePrint(argv[0],
                "currentTime:%u,delay:%u,resumeTime:%u",
                suspensionStartTime,
                suspension,
                suspensionStartTime + suspension);
}
