/***************************************************************************//**
 * @file ezsp-host-ui.c
 * @brief EZSP Host user interface functions
 *
 * This includes command option parsing, trace and counter functions.
 *
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#include PLATFORM_HEADER
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include "stack/include/ember-types.h"
#include "ezsp/ezsp-protocol.h"
#include "hal/micro/generic/em2xx-reset-defs.h"
#include "hal/micro/system-timer.h"
#include "ezsp/ezsp-enum-decode.h"
#include "ezsp-host/ezsp-host-common.h"
#include "ezsp-host/ezsp-host-priv.h"
#include "ezsp-host/ezsp-host-io.h"
#include "ezsp-host/ezsp-host-queues.h"
#include "ezsp-host/ezsp-host-ui.h"

#include "gateway/backchannel.h"

//------------------------------------------------------------------------------
// Preprocessor definitions

#define ERR_LEN               128   // max length error message

//------------------------------------------------------------------------------
// Local Variables

//------------------------------------------------------------------------------
// Command line option parsing

void ezspPrintUsage(char *name)
{
  char *shortName = strrchr(name, '/');
  shortName = shortName ? shortName + 1 : name;
  fprintf(stderr, "Usage: %s", shortName);
  fprintf(stderr, usage);
}

#ifdef __CYGWIN__
bool checkSerialPort(const char* portString, bool silent)
{
  // Not sure what we can do on Windows to check whether
  // the serial port designation is correct (RBA).
  return true;
}
#else  // POSIX OS
bool checkSerialPort(const char* portString, bool silent)
{
  struct stat buffer;
  if (0 != stat(portString, &buffer)) {
    if (!silent) {
      fprintf(stderr, "Error: No such device '%s'\n", portString);
    }
    return false;
  }

  if (!S_ISCHR(buffer.st_mode)) {
    if (!silent) {
      fprintf(stderr, "Error: '%s' is not a character device.\n", portString);
    }
    return false;
  }

  return true;
}
#endif

bool ezspProcessCommandOptions(int argc, char *argv[])
{
  char errStr[ERR_LEN] = "";

  if (!ezspInternalProcessCommandOptions(argc, argv, errStr)) {
    return false;
  }
  if (*errStr != '\0') {
    fprintf(stderr, "%s", errStr);
    ezspPrintUsage(argv[0]);
  }
  return (*errStr == '\0');
}

//------------------------------------------------------------------------------
// Error/status code to string conversion

const uint8_t* ezspErrorString(uint8_t error)
{
  switch (error) {
    case EM2XX_RESET_UNKNOWN:
      return (uint8_t *) "unknown reset";
    case EM2XX_RESET_EXTERNAL:
      return (uint8_t *) "external reset";
    case EM2XX_RESET_POWERON:
      return (uint8_t *) "power on reset";
    case EM2XX_RESET_WATCHDOG:
      return (uint8_t *) "watchdog reset";
    case EM2XX_RESET_ASSERT:
      return (uint8_t *) "assert reset";
    case EM2XX_RESET_BOOTLOADER:
      return (uint8_t *) "bootloader reset";
    case EM2XX_RESET_SOFTWARE:
      return (uint8_t *) "software reset";
    default:
      return (uint8_t *) "";
  }
} // end of ezspErrorString()

#define DATE_MAX 11 //10/10/2000 = 11 characters including NULL
#define TIME_MAX 13 //10:10:10.123 = 13 characters including NULL

void ezspPrintElapsedTime(void)
{
  static bool first = true;
  static uint32_t base;
  uint32_t now;

  struct tm tmToday;
  time_t currentTime;
  char dateString[DATE_MAX];
  char timeString[TIME_MAX];
  time(&currentTime);
  localtime_r(&currentTime, &tmToday);
  strftime(dateString, DATE_MAX, "%m/%d/%Y", &tmToday);
  strftime(timeString, TIME_MAX, "%H:%M:%S", &tmToday);

  if (first) {
    base = halCommonGetInt32uMillisecondTick();
    first = false;
  }
  now = halCommonGetInt32uMillisecondTick() - base;
  ezspDebugPrintf("%s %s: %d.%03d ", dateString, timeString, now / 1000, now % 1000);
}

void ezspTraceEvent(const char *string)
{
  if (readConfig(traceFlags) & TRACE_EVENTS) {
    ezspDebugPrintf("%s", string);
    ezspDebugPrintf("\r\n");
  }
}

void ezspTraceEzspFrameId(const char *message, uint8_t *ezspFrame)
{
  if (readConfig(traceFlags) & TRACE_EZSP) {
    uint8_t frameId = ezspFrame[EZSP_FRAME_ID_INDEX];
    ezspPrintElapsedTime();
    ezspDebugPrintf("EZSP: %s (%02X)\r\n",
                    message, frameId);
  }
}

void ezspTraceEzspVerbose(char *format, ...)
{
  if (readConfig(traceFlags) & TRACE_EZSP_VERBOSE) {
    va_list argPointer;
    ezspPrintElapsedTime();
    ezspDebugPrintf("EZSP: ");
    va_start(argPointer, format);
    ezspDebugVfprintf(format, argPointer);
    va_end(argPointer);
    ezspDebugPrintf("\r\n");
  }
}
