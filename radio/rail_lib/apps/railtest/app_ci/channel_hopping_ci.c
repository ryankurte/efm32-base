/***************************************************************************//**
 * @file
 * @brief This file implements the autoack commands in RAIL test apps.
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
#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "rail_features.h"
#include "app_common.h"
#include "app_ci.h"

#if RAIL_FEAT_CHANNEL_HOPPING

#define MAX_NUMBER_CHANNELS 4

RAIL_RxChannelHoppingConfigEntry_t channelHoppingEntries[MAX_NUMBER_CHANNELS];
RAIL_RxChannelHoppingConfig_t channelHoppingConfig = {
  .entries = channelHoppingEntries,
  .bufferLength = CHANNEL_HOPPING_BUFFER_SIZE,
  .numberOfChannels = 0
};

void configRxChannelHopping(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  channelHoppingConfig.buffer = channelHoppingBuffer;
  uint8_t i;
  for (i = 1; i * 4 < argc; i++) {
    if (ciGetUnsigned(argv[(i * 4) - 3]) > (uint32_t)UINT16_MAX) {
      responsePrintError(argv[0], 0x16, "Channel must be a 16 bit value.");
      return;
    }
    channelHoppingEntries[i - 1].channel = ciGetUnsigned(argv[(i * 4) - 3]);
    channelHoppingEntries[i - 1].mode = ciGetUnsigned(argv[(i * 4) - 2]);
    channelHoppingEntries[i - 1].parameter = ciGetUnsigned(argv[i * 4 - 1]);
    channelHoppingEntries[i - 1].delay = ciGetUnsigned(argv[i * 4]);
    channelHoppingEntries[i - 1].delayMode = RAIL_RX_CHANNEL_HOPPING_DELAY_MODE_STATIC;
  }
  channelHoppingConfig.numberOfChannels = i - 1;

  RAIL_Status_t status = RAIL_ConfigRxChannelHopping(railHandle, &channelHoppingConfig);
  responsePrint(argv[0], "numberOfChannels:%d,buffer:0x%x,Success:%s",
                i - 1,
                channelHoppingConfig.buffer,
                status == RAIL_STATUS_NO_ERROR ? "True" : "False");
}

void enableRxChannelHopping(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }
  bool enable = !!ciGetUnsigned(argv[1]);

  bool reset = false;
  if (argc > 2) {
    reset = !!ciGetUnsigned(argv[2]);
  }

  RAIL_Status_t status = RAIL_EnableRxChannelHopping(railHandle, enable, reset);
  responsePrint(argv[0], "Success:%s", status == RAIL_STATUS_NO_ERROR ? "True" : "False");
}

void getChannelHoppingRssi(int argc, char **argv)
{
  uint8_t channelIndex = ciGetUnsigned(argv[1]);

  int16_t result = RAIL_GetChannelHoppingRssi(railHandle, channelIndex);

  char bufRssi[10];

  // The lowest negative value is used to indicate an error reading the RSSI
  if (result == RAIL_RSSI_INVALID) {
    responsePrintError(argv[0], 0x08, "Could not read RSSI. Ensure channel hopping was configured and enabled.");
    return;
  }

  sprintfFloat(bufRssi, sizeof(bufRssi), ((float) result / 4), 2);

  responsePrint(argv[0], "rssi:%s", bufRssi);
}

void configRxDutyCycle(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }
  RAIL_RxDutyCycleConfig_t config = {
    .mode = ciGetUnsigned(argv[1]),
    .parameter = ciGetUnsigned(argv[2]),
    .delay = ciGetUnsigned(argv[3]),
    .delayMode = RAIL_RX_CHANNEL_HOPPING_DELAY_MODE_STATIC
  };

  RAIL_Status_t status = RAIL_ConfigRxDutyCycle(railHandle, &config);

  responsePrint(argv[0], "Success:%s,Mode:%d,Parameter:%d,Delay:%d",
                status == RAIL_STATUS_NO_ERROR ? "True" : "False",
                config.mode,
                config.parameter,
                config.delay);
}

void enableRxDutyCycle(int argc, char **argv)
{
  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }
  bool enable = !!ciGetUnsigned(argv[1]);

  RAIL_Status_t status = RAIL_EnableRxDutyCycle(railHandle, enable);
  responsePrint(argv[0], "Success:%s", status == RAIL_STATUS_NO_ERROR ? "True" : "False");
}

#else

static void channelHoppingNotSupported(char **argv)
{
  responsePrintError(argv[0], 0x17, "Channel hopping not suppported on this chip");
}

void configRxChannelHopping(int argc, char **argv)
{
  channelHoppingNotSupported(argv);
}

void enableRxChannelHopping(int argc, char **argv)
{
  channelHoppingNotSupported(argv);
}

void getChannelHoppingRssi(int argc, char **argv)
{
  channelHoppingNotSupported(argv);
}

void configRxDutyCycle(int argc, char **argv)
{
  channelHoppingNotSupported(argv);
}

void enableRxDutyCycle(int argc, char **argv)
{
  channelHoppingNotSupported(argv);
}

#endif
