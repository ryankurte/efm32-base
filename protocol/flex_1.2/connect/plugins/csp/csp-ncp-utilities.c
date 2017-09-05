// File: csp-ncp-utilities.c
//
// Description: Functions to assist processing of CSP commands and returns.
//
// Copyright 2016 Silicon Laboratories, Inc.                                *80*

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"

#include "csp/csp-common.h"
#include "csp/csp-enum.h"
#include "csp/csp-ncp-utilities.h"

void emberGetStandaloneBootloaderInfo(void)
{
  uint16_t version;
  #ifdef NULL_BTL
  version = 0;
  #else
  version = halGetStandaloneBootloaderVersion();
  #endif
  emberGetStandaloneBootloaderInfoReturn(version, PLAT, MICRO, PHY);
}

void emberLaunchStandaloneBootloader(uint8_t mode)
{
  #ifndef NULL_BTL
  if (BOOTLOADER_INVALID_VERSION != halGetStandaloneBootloaderVersion()) {
    // This will NOT return on success.
    halLaunchStandaloneBootloader(mode);
  }
  #endif
  emberLaunchStandaloneBootloaderReturn(EMBER_ERR_FATAL);
}

uint16_t emberGetCspVersion(void)
{
  return CSP_VERSION;
}

// -----------------------------------------------------------------------------
// CSP pre/post hooks.

extern uint8_t emBootloaderMode;

bool cspNcpLaunchStandaloneBootloaderPreHook(uint8_t mode)
{
  #ifdef NULL_BTL
  return false;
  #else
  EmberStatus status = EMBER_ERR_FATAL;
  if (BOOTLOADER_INVALID_VERSION != halGetStandaloneBootloaderVersion()) {
    // If the bootloader checks out ok, defer executing until we have a
    // chance to send back the TMSP response frame.
    status = EMBER_SUCCESS;
    emBootloaderMode = mode;
  }
  // Never immediately call halLaunchStandaloneBootloader() on an NCP.
  // Let the main loop in ip-modem-app call it when it notices a change
  // to emBootloaderMode. Respond now so that the host knows whether
  // the NCP is launching the bootloader or not.
  emberLaunchStandaloneBootloaderReturn(status);
  return false;
  #endif
}
