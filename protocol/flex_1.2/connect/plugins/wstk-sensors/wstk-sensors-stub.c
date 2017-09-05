/*
 * File: wstk/sensors-stub.c
 * Description: Sensor Stubs for EM357 / EM357Pro2+
 *
 * Author(s): Rehan Rasool, rehan.rasool@silabs.com
 *
 * Copyright 2015 by Silicon Labs. All rights reserved.
 */

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "stack/include/ember.h"

bool emberAfPluginWstkSensorsInit(void)
{
  return true;
}

EmberStatus emberAfPluginWstkSensorsGetSample(uint32_t *rhData, int32_t *tData)
{
  return EMBER_SUCCESS;
}
