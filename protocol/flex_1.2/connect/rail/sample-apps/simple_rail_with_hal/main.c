/***************************************************************************//**
 * @file main.c
 * @brief Simple RAIL application which includes hal
 * @copyright Copyright 2017 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include "rail.h"
#include "hal_common.h"
#include "rail_config.h"

#include "em_chip.h"

void initRadio()
{
  RAIL_Init_t railInitParams = {
    256,
    RADIO_CONFIG_XTAL_FREQUENCY,
    RAIL_CAL_ALL,
  };
  halInit();
  RAIL_RfInit(&railInitParams);
  RAIL_RfIdleExt(RAIL_IDLE, true);

  RAIL_CalInit_t calInit = {
    RAIL_CAL_ALL,
    irCalConfig,
  };
  RAIL_CalInit(&calInit);

  RAIL_PacketLengthConfigFrameType(frameTypeConfigList[0]);
  if (RAIL_RadioConfig((void*)configList[0])) {
    while (1) ;
  }

  RAIL_ChannelConfig(channelConfigs[0]);
}

int main(void)
{
  CHIP_Init();
  initRadio();

  while (1) ;
  return 0;
}
