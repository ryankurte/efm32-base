/***************************************************************************//**
 * @file
 * @brief This file contains helpers for transitioning into the various
 *   AppModes
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

#include "rail.h"
#include "rail_types.h"
#include "response_print.h"
#include "app_common.h"

// Guard for CI functions to ensure a certain AppMode before running
bool inAppMode(AppMode_t appMode, char *command)
{
  bool ret = (currentAppMode() == appMode);
  if (!ret && command) {
    responsePrintError(command, 0x16,
                       "Need to be in %s mode for this command",
                       appModeNames(appMode));
  }
  return ret;
}

// Setting semantics for enabling AppMode
void setNextAppMode(AppMode_t next, char *command)
{
  if (next == NONE) {
    enableAppMode(currentAppMode(), false, command);
  } else {
    enableAppMode(next, true, command);
  }
}

// Used when the AppMode is needed to be enabled to continue the function
bool enableAppModeSync(AppMode_t mode, bool enable, char *command)
{
  changeAppModeIfPending();
  enableAppMode(mode, enable, command);
  changeAppModeIfPending();
  // inAppMode shouldn't print, since enableAppMode will cause a print
  return inAppMode(enable ? mode : NONE, NULL);
}

void scheduleNextTx(void)
{
  // Schedule the next tx if there are more coming
  if (txCount > 0 || currentAppMode() == TX_CONTINUOUS) {
    RAIL_SetTimer(railHandle, continuousTransferPeriod * 1000, RAIL_TIME_DELAY, &RAILCb_TimerExpired);
  } else if (currentAppMode() == TX_N_PACKETS || currentAppMode() == TX_SCHEDULED
             || currentAppMode() == TX_UNDERFLOW || currentAppMode() == TX_CANCEL) {
    setNextAppMode(NONE, NULL);
  } else {
    // If we automatically transitioned to SCHTX_AFTER_RX, or don't know how we
    // got here, just finish whatever transmission just happened
    pendFinishTxSequence();
  }
}

void radioTransmit(uint32_t iterations, char *command)
{
  // Do the normal work of just increasing the tx count
  txCount = iterations;

  if (iterations > 0) {
    if (currentAppMode() == TX_UNDERFLOW) {
      setNextAppMode(TX_UNDERFLOW, command); // Get a printout for TX
      pendPacketTx(); // Start transmitting without waiting for the mode change
    } else {
      // Make a change if we're supposed to transmit more packets
      setNextAppMode(TX_N_PACKETS, command);
    }
  } else {
    // A tx count of 0 will stop a continuous or fixed transmit if we're in the
    // middle of one. Otherwise, we'll enable continuous mode.
    if (currentAppMode() == TX_N_PACKETS || currentAppMode() == TX_CONTINUOUS
        || currentAppMode() == TX_UNDERFLOW) {
      setNextAppMode(NONE, command);
    } else {
      setNextAppMode(TX_CONTINUOUS, command);
    }
  }
}
