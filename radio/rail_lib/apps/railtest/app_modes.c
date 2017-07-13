/***************************************************************************//**
 * @file app_modes.c
 * @brief This is the states machine for the base test application. It handles
 * transmit, receive, and various debug modes.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rail.h"
#include "response_print.h"
#include "app_common.h"

// At any given time, at least one of (currAppMode, prevAppMode)
// should be NONE, due to the way that setAppModeInternal works
char nextCommandBuf[16];
char *nextCommand;
bool enableMode = true;
AppMode_t nextAppMode = NONE;
AppMode_t currAppMode = NONE;
AppMode_t prevAppMode = NONE;
bool transitionPend = false;

AppMode_t currentAppMode()
{
  return currAppMode;
}

AppMode_t previousAppMode()
{
  return prevAppMode;
}

void enableAppMode(AppMode_t next, bool enable, char *command)
{
  // Should disable current mode instead of enabling NONE
  if (!transitionPend && (next != NONE)) {
    transitionPend = true;
    nextAppMode = next;
    enableMode = enable;
    if (command == NULL) {
      nextCommand = (logLevel & ASYNC_RESPONSE) ? "appMode" : NULL;
    } else {
      memcpy(&nextCommandBuf[0], command, sizeof(nextCommandBuf));
      nextCommand = &nextCommandBuf[0];
    }
  }
}

char *appModeNames(AppMode_t appMode)
{
  char *appModes[] = { "None", "Stream", "Tone", "ContinuousTx", "DirectMode",
                       "PacketTx", "ScheduledTx", "SchTxAfterRx", "RxOverflow",
                       "TxUnderflow", "TxCancel", "RfSense", "PER", "BER",
                       "ScheduledRx" };
  return appModes[appMode];
}

// Has the logic for disabling and enabling AppMode
//  Note that at least one of (nextAppMode, currAppMode) should be NONE,
//  due to the way we handling AppMode changing
static void transitionAppMode(AppMode_t nextAppMode)
{
  if (currAppMode == NONE) {
    RAIL_TimerCancel();
  } else if (currAppMode == TX_STREAM) {
    RAIL_TxStreamStop();
  } else if (currAppMode == TX_TONE) {
    RAIL_TxToneStop();
  } else if (currAppMode == DIRECT) {
    RAIL_DirectModeConfig(false);
  } else if (currAppMode == TX_CONTINUOUS   || currAppMode == TX_N_PACKETS
             || currAppMode == TX_SCHEDULED || currAppMode == TX_UNDERFLOW
             || currAppMode == TX_CANCEL) {
    // Disable timer just in case
    RAIL_TimerCancel();
    txCount = 0;
    pendFinishTxSequence();
  } else if (currAppMode == RF_SENSE) {
    (void) RAIL_RfSense(RAIL_RFSENSE_OFF, 0, false);
  } else if (currAppMode == PER) {
    RAIL_TimerCancel();
  } else if (currAppMode == BER) {
    RAIL_RfIdle();
  }

  if (nextAppMode == TX_STREAM) {
    RAIL_TxStreamStart(channel, PN9_STREAM);
  } else if (nextAppMode == TX_TONE) {
    RAIL_TxToneStart(channel);
  } else if (nextAppMode == DIRECT) {
    RAIL_DirectModeConfig(true);
  } else if (nextAppMode == TX_CONTINUOUS || nextAppMode == TX_N_PACKETS) {
    pendPacketTx();
  } else if (nextAppMode == TX_SCHEDULED || nextAppMode == TX_CANCEL) {
    txCount = 1;
    pendPacketTx();
  } else if (nextAppMode == SCHTX_AFTER_RX || nextAppMode == RX_OVERFLOW) {
    RAIL_RxStart(channel);
  }
  prevAppMode = currAppMode;
  currAppMode = nextAppMode;
}

// This has the rules of when we are allowed to change the AppMode, along
// with the printouts that should happen when a AppMode change happens
static void setAppModeInternal(void)
{
  AppMode_t next = enableMode ? nextAppMode : NONE;
  if (currAppMode == nextAppMode && enableMode) {
    if (nextCommand) {
      responsePrint(nextCommand, "%s:Enabled", appModeNames(next));
    }
  } else if ((currAppMode == nextAppMode && !enableMode)
             || (currAppMode == NONE && enableMode)) {
    if (nextCommand) {
      responsePrint(nextCommand,
                    "%s:Enabled,%s:Disabled,Time:%u",
                    appModeNames(next),
                    appModeNames(currAppMode),
                    RAIL_GetTime());
    }
    transitionAppMode(next);
  } else { // Ignore mode change request
    if (nextCommand) {
      responsePrintError(nextCommand, 1, "Can't %s %s during %s",
                         enableMode ? "enable" : "disable",
                         appModeNames(nextAppMode), appModeNames(currAppMode));
    }
  }
}

// This should be called from a main loop, to update the AppMode
void changeAppModeIfPending()
{
  if (transitionPend) {
    transitionPend = false;
    setAppModeInternal();
  }
}
