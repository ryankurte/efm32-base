/*
 * File: hearbeat.c
 * Description: Heartbeat support code.
 *
 * Copyright 2015 Silicon Laboratories, Inc.
 */

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"

EmberEventControl emberAfPluginHeartbeatEventControl;

void emberAfPluginHeartbeatInit(void)
{
  emberEventControlSetActive(emberAfPluginHeartbeatEventControl);
}

void emberAfPluginHeartbeatEventHandler(void)
{
  halToggleLed((HalBoardLed)EMBER_AF_PLUGIN_HEARTBEAT_LED);

  emberEventControlSetDelayQS(emberAfPluginHeartbeatEventControl,
                              EMBER_AF_PLUGIN_HEARTBEAT_BLINK_QS);
}
