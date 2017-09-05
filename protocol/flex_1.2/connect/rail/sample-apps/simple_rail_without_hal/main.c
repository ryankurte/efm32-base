/***************************************************************************//**
 * @file main.c
 * @brief Simple RAIL application which does not include hal
 * @copyright Copyright 2017 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include "em_chip.h"
#include "em_emu.h"
#include "em_cmu.h"
#include "pa.h"

#include "rail.h"
#include "rail_config.h"

#include "bsp.h"

void initRadio()
{
  RAIL_Init_t railInitParams = {
    256,
    RADIO_CONFIG_XTAL_FREQUENCY,
    RAIL_CAL_ALL,
  };

  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_WSTK_DEFAULT;
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_WSTK_DEFAULT;
  RADIO_PAInit_t paInit;

  EMU_DCDCInit(&dcdcInit);
  CMU_HFXOInit(&hfxoInit);

  /* Switch HFCLK to HFXO and disable HFRCO */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

  // Initialize the PA now that the HFXO is up and the timing is correct
  #if (RADIO_CONFIG_BASE_FREQUENCY < 1000000000UL)
  paInit = (RADIO_PAInit_t) RADIO_PA_SUBGIG_INIT;
  #else
  paInit = (RADIO_PAInit_t) RADIO_PA_2P4_INIT;
  #endif

  RADIO_PA_Init(&paInit);

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
