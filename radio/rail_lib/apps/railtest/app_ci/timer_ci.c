/***************************************************************************//**
 * @file timer_ci.c
 * @brief This file is for testing the RAIL timer interface.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "bsp.h"
#include "command_interpreter.h"
#include "response_print.h"

#include "rail_types.h"
#include "rail.h"
#include "app_common.h"

void printTimerStats(int argc, char **argv);

void setTimer(int argc, char **argv)
{
  if (!inAppMode(NONE, argv[0])) {
    return;
  }

  uint32_t timeOut = ciGetUnsigned(argv[1]);
  RAIL_TimeMode_t mode = RAIL_TIME_DELAY;

  // If this is absolute mode then switch the delay mode
  if (strcmp(argv[2], "abs") == 0) {
    mode = RAIL_TIME_ABSOLUTE;
  }

  RAIL_TimerCancel();
  if (RAIL_TimerSet(timeOut, mode) != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 0x40, "TimerSet failed");
    return;
  }

  printTimerStats(1, argv);
}

void printTimerStats(int argc, char **argv)
{
  bool enabled = RAIL_TimerIsRunning();
  bool expired = RAIL_TimerExpired();
  uint32_t expirationTime = RAIL_TimerGet();
  uint32_t currentTime = RAIL_GetTime();

  responsePrint(argv[0],
                "timeUs:%u,ExpirationTime:%u,"
                "IsRunning:%s,IsExpired:%s",
                currentTime,
                expirationTime,
                (enabled ? "True" : "False"),
                (expired ? "True" : "False"));
}

void timerCancel(int argc, char** argv)
{
  if (inAppMode(NONE, argv[0])) {
    RAIL_TimerCancel();
    printTimerStats(1, argv);
  }
}
