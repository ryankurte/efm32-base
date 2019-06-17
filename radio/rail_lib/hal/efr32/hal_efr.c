/***************************************************************************//**
 * @file
 * @brief This file contains EFR32 specific HAL code to handle chip startup.
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

#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_ldma.h"
#include "dmadrv.h"
#include "bsp.h"
#include "bsp_init.h"
#include "em_chip.h"
#if defined(_SILICON_LABS_32B_SERIES_2)
#include "em_prs.h"
#endif

#include "rail.h"
#include "rail_chip_specific.h"
#include "hal_common.h"
#include "hal-config.h"

#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

#ifdef BSP_EXTFLASH_USART
#include "mx25flash_spi.h"
#endif

#if HAL_FEM_ENABLE
#include "util/plugin/plugin-common/fem-control/fem-control.h"
#endif

static void boardDisableSpiFlash(void);
static void boardLowPowerInit(void);

#ifdef RAIL_PA_CONVERSIONS
#include RAIL_PA_CONVERSIONS

#ifndef BSP_PA_VOLTAGE // Work with older Gecko SDK
#define BSP_PA_VOLTAGE HAL_PA_VOLTAGE
#endif//BSP_PA_VOLTAGE
#endif

RAIL_AntennaConfig_t halAntennaConfig;

// The EFR32XG2 series doesn't use locations so the HAL configurator doesn't
// provide any, but it has RfPath selection so use BSP_ANTDIV_SEL_LOC for that.
// Provide defaults to sate the API, using RfPath 1.
#ifdef  _SILICON_LABS_32B_SERIES_2
 #if (!defined(BSP_ANTDIV_SEL_LOC))
  #define BSP_ANTDIV_SEL_LOC 1 // Choose RfPath 1
 #endif
 #if (!defined(BSP_ANTDIV_NSEL_LOC) && defined(BSP_ANTDIV_NSEL_PORT))
  #define BSP_ANTDIV_NSEL_LOC 1 // Dummy selection
 #endif
#endif

static void initAntenna(void)
{
 #if (HAL_ANTDIV_ENABLE               \
      && defined(BSP_ANTDIV_SEL_PORT) \
  && defined(BSP_ANTDIV_SEL_PIN)      \
  && defined(BSP_ANTDIV_SEL_LOC))
  halAntennaConfig.ant0PinEn = true;
  halAntennaConfig.ant0Port = (uint8_t)BSP_ANTDIV_SEL_PORT;
  halAntennaConfig.ant0Pin  = BSP_ANTDIV_SEL_PIN;
  halAntennaConfig.ant0Loc  = BSP_ANTDIV_SEL_LOC;
 #endif
 #ifdef _SILICON_LABS_32B_SERIES_2
  halAntennaConfig.defaultPath = BSP_ANTDIV_SEL_LOC;
 #endif
 #if (HAL_ANTDIV_ENABLE                \
      && defined(BSP_ANTDIV_NSEL_PORT) \
  && defined(BSP_ANTDIV_NSEL_PIN)      \
  && defined(BSP_ANTDIV_NSEL_LOC))
  halAntennaConfig.ant1PinEn = true;
  halAntennaConfig.ant1Port = (uint8_t)BSP_ANTDIV_NSEL_PORT;
  halAntennaConfig.ant1Pin  = BSP_ANTDIV_NSEL_PIN;
  halAntennaConfig.ant1Loc  = BSP_ANTDIV_NSEL_LOC;
 #endif
 #if (HAL_ANTDIV_ENABLE || defined(_SILICON_LABS_32B_SERIES_2))
  (void) RAIL_ConfigAntenna(RAIL_EFR32_HANDLE, &halAntennaConfig);
 #endif
}

void halInitChipSpecific(void)
{
#if defined(BSP_DK) && !defined(RAIL_IC_SIM_BUILD)
  BSP_Init(BSP_INIT_DK_SPI);
#endif
  BSP_initDevice();

#if !defined(RAIL_IC_SIM_BUILD)
  BSP_initBoard();
#endif

#if HAL_PTI_ENABLE
  RAIL_PtiConfig_t railPtiConfig = {
#if HAL_PTI_MODE == HAL_PTI_MODE_SPI
    .mode = RAIL_PTI_MODE_SPI,
#elif HAL_PTI_MODE == HAL_PTI_MODE_UART
    .mode = RAIL_PTI_MODE_UART,
#elif HAL_PTI_MODE == HAL_PTI_MODE_UART_ONEWIRE
    .mode = RAIL_PTI_MODE_UART_ONEWIRE,
#else
    .mode = RAIL_PTI_MODE_DISABLED,
#endif
    .baud = HAL_PTI_BAUD_RATE,
#ifdef BSP_PTI_DOUT_LOC
    .doutLoc = BSP_PTI_DOUT_LOC,
#endif
    .doutPort = (uint8_t)BSP_PTI_DOUT_PORT,
    .doutPin = BSP_PTI_DOUT_PIN,
#if HAL_PTI_MODE == HAL_PTI_MODE_SPI
#ifdef BSP_PTI_DCLK_LOC
    .dclkLoc = BSP_PTI_DCLK_LOC,
#endif
    .dclkPort = (uint8_t)BSP_PTI_DCLK_PORT,
    .dclkPin = BSP_PTI_DCLK_PIN,
#endif
#if HAL_PTI_MODE != HAL_PTI_MODE_UART_ONEWIRE
#ifdef BSP_PTI_DFRAME_LOC
    .dframeLoc = BSP_PTI_DFRAME_LOC,
#endif
    .dframePort = (uint8_t)BSP_PTI_DFRAME_PORT,
    .dframePin = BSP_PTI_DFRAME_PIN
#endif
  };

  RAIL_ConfigPti(RAIL_EFR32_HANDLE, &railPtiConfig);
#endif // HAL_PTI_ENABLE

  // Only create and save the curves if the customer wants them
  #ifdef RAIL_PA_CONVERSIONS
  #if BSP_PA_VOLTAGE > 1800
  RAIL_InitTxPowerCurvesAlt(&RAIL_TxPowerCurvesVbat);
  #else
  RAIL_InitTxPowerCurvesAlt(&RAIL_TxPowerCurvesDcdc);
  #endif
  #endif

#if (HAL_FEM_ENABLE)
  initFem();
#endif

#if !defined(RAIL_IC_SIM_BUILD)
  initAntenna();

  // Disable any unused peripherals to ensure we enter a low power mode
  boardLowPowerInit();
#endif

#if RAIL_DMA_CHANNEL == DMA_CHANNEL_DMADRV
  Ecode_t dmaError = DMADRV_Init();
  if ((dmaError == ECODE_EMDRV_DMADRV_ALREADY_INITIALIZED)
      || (dmaError == ECODE_EMDRV_DMADRV_OK)) {
    unsigned int channel;
    dmaError = DMADRV_AllocateChannel(&channel, NULL);
    if (dmaError == ECODE_EMDRV_DMADRV_OK) {
      RAIL_UseDma(channel);
    }
  }
#elif defined(RAIL_DMA_CHANNEL) && (RAIL_DMA_CHANNEL != DMA_CHANNEL_INVALID)
  LDMA_Init_t ldmaInit = LDMA_INIT_DEFAULT;
  LDMA_Init(&ldmaInit);
  RAIL_UseDma(RAIL_DMA_CHANNEL);
#endif
}

static void boardLowPowerInit(void)
{
  boardDisableSpiFlash();
}

#define CMD_POWER_DOWN                      (0xB9)
#define CMD_POWER_UP                        (0xAB)

static void boardDisableSpiFlash(void)
{
#if defined(BSP_EXTFLASH_USART) && !defined(HAL_DISABLE_EXTFLASH)
  MX25_init();
  MX25_DP();
#endif
}

// Create defines for the different PRS signal sources as they vary per chip
#if _SILICON_LABS_32B_SERIES_1_CONFIG == 1
// Defines for EFR32xG1 chips
#define _PRS_CH_CTRL_SOURCESEL_FRC     0x00000025UL
#else
// Defines for EFR32xG12 and newer chips
#define _PRS_CH_CTRL_SOURCESEL_FRC     0x00000055UL
#endif

#ifdef _SILICON_LABS_32B_SERIES_1
/**
 * Define the signals that are supported for debug in railtest. These are chip
 * specific because on some chips these are supported by the PRS while on others
 * the debugging must come from the library directly.
 */
static const debugSignal_t debugSignals[] =
{
  {
    .name = "RXACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = (PRS_RAC_RX & _PRS_CH_CTRL_SIGSEL_MASK) >> _PRS_CH_CTRL_SIGSEL_SHIFT,
        .source = (PRS_RAC_RX & _PRS_CH_CTRL_SOURCESEL_MASK) >> _PRS_CH_CTRL_SOURCESEL_SHIFT,
      }
    }
  },
  {
    .name = "TXACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = (PRS_RAC_TX & _PRS_CH_CTRL_SIGSEL_MASK) >> _PRS_CH_CTRL_SIGSEL_SHIFT,
        .source = (PRS_RAC_TX & _PRS_CH_CTRL_SOURCESEL_MASK) >> _PRS_CH_CTRL_SOURCESEL_SHIFT,
      }
    }
  },
  {
    .name = "LNAEN",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = (PRS_RAC_LNAEN & _PRS_CH_CTRL_SIGSEL_MASK) >> _PRS_CH_CTRL_SIGSEL_SHIFT,
        .source = (PRS_RAC_LNAEN & _PRS_CH_CTRL_SOURCESEL_MASK) >> _PRS_CH_CTRL_SOURCESEL_SHIFT,
      }
    }
  },
  {
    .name = "PAEN",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = (PRS_RAC_PAEN & _PRS_CH_CTRL_SIGSEL_MASK) >> _PRS_CH_CTRL_SIGSEL_SHIFT,
        .source = (PRS_RAC_PAEN & _PRS_CH_CTRL_SOURCESEL_MASK) >> _PRS_CH_CTRL_SOURCESEL_SHIFT,
      }
    }
  },
  {
    .name = "RACACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = (PRS_RAC_ACTIVE & _PRS_CH_CTRL_SIGSEL_MASK) >> _PRS_CH_CTRL_SIGSEL_SHIFT,
        .source = (PRS_RAC_ACTIVE & _PRS_CH_CTRL_SOURCESEL_MASK) >> _PRS_CH_CTRL_SOURCESEL_SHIFT,
      }
    }
  },
  {
    .name = "PTIDATA",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = 0x00,
        .source = _PRS_CH_CTRL_SOURCESEL_FRC
      }
    }
  },
  {
    .name = "FRAMEDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = (PRS_MODEM_FRAMEDET & _PRS_CH_CTRL_SIGSEL_MASK) >> _PRS_CH_CTRL_SIGSEL_SHIFT,
        .source = (PRS_MODEM_FRAMEDET & _PRS_CH_CTRL_SOURCESEL_MASK) >> _PRS_CH_CTRL_SOURCESEL_SHIFT,
      }
    }
  },
  {
    .name = "PREAMBLEDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = (PRS_MODEM_PREDET & _PRS_CH_CTRL_SIGSEL_MASK) >> _PRS_CH_CTRL_SIGSEL_SHIFT,
        .source = (PRS_MODEM_PREDET & _PRS_CH_CTRL_SOURCESEL_MASK) >> _PRS_CH_CTRL_SOURCESEL_SHIFT,
      }
    }
  },
  {
    .name = "TIMINGDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = (PRS_MODEM_TIMDET & _PRS_CH_CTRL_SIGSEL_MASK) >> _PRS_CH_CTRL_SIGSEL_SHIFT,
        .source = (PRS_MODEM_TIMDET & _PRS_CH_CTRL_SOURCESEL_MASK) >> _PRS_CH_CTRL_SOURCESEL_SHIFT,
      }
    }
  },
  {
    .name = "FRAMESENT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = (PRS_MODEM_FRAMESENT & _PRS_CH_CTRL_SIGSEL_MASK) >> _PRS_CH_CTRL_SIGSEL_SHIFT,
        .source = (PRS_MODEM_FRAMESENT & _PRS_CH_CTRL_SOURCESEL_MASK) >> _PRS_CH_CTRL_SOURCESEL_SHIFT,
      }
    }
  },
  {
    .name = "SYNCSENT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = (PRS_MODEM_SYNCSENT & _PRS_CH_CTRL_SIGSEL_MASK) >> _PRS_CH_CTRL_SIGSEL_SHIFT,
        .source = (PRS_MODEM_SYNCSENT & _PRS_CH_CTRL_SOURCESEL_MASK) >> _PRS_CH_CTRL_SOURCESEL_SHIFT,
      }
    }
  },
};
#elif defined(_SILICON_LABS_32B_SERIES_2_CONFIG_1)
/**
 * Define the signals that are supported for debug in railtest. These are chip
 * specific because on some chips these are supported by the PRS while on others
 * the debugging must come from the library directly.
 */
static const debugSignal_t debugSignals[] =
{
  {
    .name = "RXACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACRX,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RAC,
      }
    }
  },
  {
    .name = "TXACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACTX,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RAC,
      }
    }
  },
  {
    .name = "LNAEN",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLNAEN,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RAC,
      }
    }
  },
  {
    .name = "PAEN",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACPAEN,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RAC,
      }
    }
  },
  {
    .name = "RACACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACACTIVE,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RAC,
      }
    }
  },
  {
    .name = "PTIDATA",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_FRCDOUT,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_FRC
      }
    }
  },
  {
    .name = "FRAMEDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMLFRAMEDET,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEML,
      }
    }
  },
  {
    .name = "PREAMBLEDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMPREDET,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEM,
      }
    }
  },
  {
    .name = "TIMINGDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMHTIMDET,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEMH,
      }
    }
  },
  {
    .name = "FRAMESENT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMFRAMESENT,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEM,
      }
    }
  },
  {
    .name = "SYNCSENT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMHSYNCSENT,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEMH,
      }
    }
  },
  {
    .name = "EOF",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMHEOF,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEMH,
      }
    }
  },
  {
    .name = "CC0",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_PROTIMERLCC0,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_PROTIMERL,
      }
    }
  },
};
#elif defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2)
// @TODO REMOVE THIS HACK
/**
 * Define the signals that are supported for debug in railtest. These are chip
 * specific because on some chips these are supported by the PRS while on others
 * the debugging must come from the library directly.
 */
static const debugSignal_t debugSignals[] =
{
  {
    .name = "RXACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLRX,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RACL,
      }
    }
  },
  {
    .name = "TXACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLTX,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RACL,
      }
    }
  },
  {
    .name = "LNAEN",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLLNAEN,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RACL,
      }
    }
  },
  {
    .name = "PAEN",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLPAEN,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RACL,
      }
    }
  },
  {
    .name = "RACACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLACTIVE,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RACL,
      }
    }
  },
  {
    .name = "PTIDATA",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_FRCDOUT,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_FRC
      }
    }
  },
  {
    .name = "FRAMEDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMLFRAMEDET,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEML,
      }
    }
  },
  {
    .name = "PREAMBLEDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMPREDET,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEM,
      }
    }
  },
  {
    .name = "TIMINGDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMHTIMDET,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEMH,
      }
    }
  },
  {
    .name = "FRAMESENT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMFRAMESENT,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEM,
      }
    }
  },
  {
    .name = "SYNCSENT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMHSYNCSENT,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEMH,
      }
    }
  },
  {
    .name = "EOF",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMHEOF,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEMH,
      }
    }
  },
  {
    .name = "CC0",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_PROTIMERLCC0,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_PROTIMERL,
      }
    }
  },
};
#elif defined(_SILICON_LABS_32B_SERIES_2_CONFIG_3)
// @TODO REMOVE THIS HACK
/**
 * Define the signals that are supported for debug in railtest. These are chip
 * specific because on some chips these are supported by the PRS while on others
 * the debugging must come from the library directly.
 */
static const debugSignal_t debugSignals[] =
{
  {
    .name = "RXACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLRX,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RACL,
      }
    }
  },
  {
    .name = "TXACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLTX,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RACL,
      }
    }
  },
  {
    .name = "LNAEN",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLLNAEN,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RACL,
      }
    }
  },
  {
    .name = "PAEN",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLPAEN,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RACL,
      }
    }
  },
  {
    .name = "RACACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_RACLACTIVE,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_RACL,
      }
    }
  },
  {
    .name = "PTIDATA",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_FRCDOUT,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_FRC
      }
    }
  },
  {
    .name = "FRAMEDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMLFRAMEDET,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEML,
      }
    }
  },
  {
    .name = "PREAMBLEDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMPREDET,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEM,
      }
    }
  },
  {
    .name = "TIMINGDETECT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMHTIMDET,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEMH,
      }
    }
  },
  {
    .name = "FRAMESENT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMFRAMESENT,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEM,
      }
    }
  },
  {
    .name = "SYNCSENT",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMHSYNCSENT,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEMH,
      }
    }
  },
  {
    .name = "EOF",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_MODEMHEOF,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_MODEMH,
      }
    }
  },
  {
    .name = "CC0",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = _PRS_ASYNC_CH_CTRL_SIGSEL_PROTIMERLCC0,
        .source = _PRS_ASYNC_CH_CTRL_SOURCESEL_PROTIMERL,
      }
    }
  },
};
#else
#warning "Implement debugSignals for this platform"
#endif

const debugSignal_t* halGetDebugSignals(uint32_t *size)
{
  if (size != NULL) {
    *size = sizeof(debugSignals) / sizeof(debugSignal_t);
  }
  return debugSignals;
}

#if defined(_SILICON_LABS_32B_SERIES_1)
/**
 * Define the pins that are supported for debugging on the EFR32. This includes
 * PF2, PF3, PF4, PF5, PC10, and PC11. Along with these pins there are specific
 * PRS channels that will be used to output debug information on these pins.
 * This is all for debug and very specific to the EFR32.
 */
static const debugPin_t debugPins[] = {
  {
    .name = "PC10", // EFR32xG1/EFR32xG12/EFR32xG13 - EXP_HEADER15, WSTK_P12
    .prsChannel = 9, // PRS 0/12 9/15 10/4 11/3
    .prsLocation = 15,
    .gpioPort = gpioPortC,
    .gpioPin = 10
  },
  {
    .name = "PC11", // EFR32xG1/EFR32xG12/EFR32xG13 - EXP_HEADER16, WSTK_P13
    .prsChannel = 10, // PRS 0/13 9/16 10/5 11/4
    .prsLocation = 5,
    .gpioPort = gpioPortC,
    .gpioPin = 11
  },
  {
    .name = "PF2", // EFR32xG1/EFR32xG13 - WSTK_P28 (SWO)
    .prsChannel = 0, // PRS 0/2 1/1 2/0 3/7
    .prsLocation = 2,
    .gpioPort = gpioPortF,
    .gpioPin = 2
  },
  {
    .name = "PF3", // EFR32xG1/EFR32xG13 - EXP_HEADER13, WSTK_10 (TDI) [EFR32xG12 PC9]
    .prsChannel = 1, // PRS 0/3 1/2 2/1 3/0
    .prsLocation = 2,
    .gpioPort = gpioPortF,
    .gpioPin = 3
  },
  {
    .name = "PF4", // EFR32xG1/EFR32xG13 - EXP_HEADER11, WSTK_P8 (LED0) [EFR32xG12 PD12]
    .prsChannel = 2, // PRS 0/4 1/3 2/2 3/1
    .prsLocation = 2,
    .gpioPort = gpioPortF,
    .gpioPin = 4
  },
  {
    .name = "PF5", // EFR32xG1/EFR32xG13 - WSTK_P32 (LED1)
    .prsChannel = 3, // PRS 0/5 1/4 2/3 3/2
    .prsLocation = 2,
    .gpioPort = gpioPortF,
    .gpioPin = 5
  },
  {
    .name = "PF6", // EFR32xG1/EFR32xG13 - WSTK_P32 (LED1)
    .prsChannel = 0, // PRS 0/5 1/4 2/3 3/2
    .prsLocation = 6,
    .gpioPort = gpioPortF,
    .gpioPin = 6
  },
  {
    .name = "PF7", // EFR32xG1/EFR32xG13 - WSTK_P32 (LED1)
    .prsChannel = 1, // PRS 0/5 1/4 2/3 3/2
    .prsLocation = 6,
    .gpioPort = gpioPortF,
    .gpioPin = 7
  },
  {
    .name = "PC9", // EFR32xG12 - EXP_HEADER13, WSTK_P10 [EFR32xG1/EFR32xG13 PF3]
    .prsChannel = 11, // PRS 0/11 9/14 10/3 11/2
    .prsLocation = 2,
    .gpioPort = gpioPortC,
    .gpioPin = 9
  },
  {
    .name = "PD9", // EFR32xG12 - EXP_HEADER5, WSTK_P2 [EFR32xG1/EFR32xG13 PA3 (VCOM_CTS)]
    .prsChannel = 3, // PRS 3/8 4/0 5/6 6/11
    .prsLocation = 8,
    .gpioPort = gpioPortD,
    .gpioPin = 9
  },
  {
    .name = "PD10", // EFR32xG12 - EXP_HEADER7, WSTK_P4 [EFR32xG1/EFR32xG13 PF6 (BUTTON0)]
    .prsChannel = 4, // PRS 3/9 4/1 5/0 6/12
    .prsLocation = 1,
    .gpioPort = gpioPortD,
    .gpioPin = 10
  },
  {
    .name = "PD11", // EFR32xG12 - EXP_HEADER9, WSTK_P6 [EFR32xG1/EFR32xG13 PF7 (BUTTON1)]
    .prsChannel = 5, // PRS 3/10 4/2 5/1 6/13
    .prsLocation = 1,
    .gpioPort = gpioPortD,
    .gpioPin = 11
  },
  {
    .name = "PD12", // EFR32xG12 - EXP_HEADER11, WSTK_P8 [EFR32xG1/EFR32xG13 PF4 (LED0)]
    .prsChannel = 6, // PRS 3/11 4/3 5/2 6/14
    .prsLocation = 14,
    .gpioPort = gpioPortD,
    .gpioPin = 12
  },
};
#elif defined(_SILICON_LABS_32B_SERIES_2)
/**
 * Define the pins that are supported for debugging on the EFR32xG2.
 * Along with these pins there are specific ASYNC PRS channels that
 * will be used to output debug information on these pins.
 * This is all for debug and very specific to the EFR32xG2.
 */
static const debugPin_t debugPins[] = {
  {
    .name         = "PA0",      // PA0 (BRD4171A - EXP11)
    .prsChannel   = 0,
    .prsLocation  = 0,
    .gpioPort     = gpioPortA,
    .gpioPin      = 0
  },
  {
    .name         = "PB2",
    .prsChannel   = 0,
    .prsLocation  = 0,
    .gpioPort     = gpioPortB,
    .gpioPin      = 2
  },
  {
    .name         = "PB3",
    .prsChannel   = 0,
    .prsLocation  = 0,
    .gpioPort     = gpioPortB,
    .gpioPin      = 3
  },
  {
    .name         = "PC0",      // PC0 (BRD4171A - EXP3, LED0)
    .prsChannel   = 6,
    .prsLocation  = 0,
    .gpioPort     = gpioPortC,
    .gpioPin      = 0
  },
  {
    .name         = "PC1",      // PC1 (BRD4171A - EXP5, LED1)
    .prsChannel   = 7,
    .prsLocation  = 0,
    .gpioPort     = gpioPortC,
    .gpioPin      = 1
  },
  {
    .name         = "PC2",      // PC2 (BRD4171A - EXP7, BTN0)
    .prsChannel   = 8,
    .prsLocation  = 0,
    .gpioPort     = gpioPortC,
    .gpioPin      = 2
  },
  {
    .name         = "PC3",      // PC3 (BRD4171A - EXP9, BTN1)
    .prsChannel   = 9,
    .prsLocation  = 0,
    .gpioPort     = gpioPortC,
    .gpioPin      = 3
  },
};
#else
#warning Implement debugPins for this platform
#endif

const debugPin_t* halGetDebugPins(uint32_t *size)
{
  if (size != NULL) {
    *size = sizeof(debugPins) / sizeof(debugPin_t);
  }
  return debugPins;
}

void halDisablePrs(uint8_t channel)
{
#if defined(_SILICON_LABS_32B_SERIES_1)
  // Turn the specified PRS channel off
  BUS_RegBitWrite(&PRS->ROUTEPEN,
                  _PRS_ROUTEPEN_CH0PEN_SHIFT + channel,
                  0);
#elif defined(_SILICON_LABS_32B_SERIES_2)
  GPIO->PRSROUTE[0].ROUTEEN &= ~(0x1 << (channel + _GPIO_PRS_ROUTEEN_ASYNCH0PEN_SHIFT));
  // PRS_FreeChannel(unsigned int ch, PRS_ChType_t type, GPIO_Port_TypeDef port, uint8_t pin)
#else
  #error "Unsupported platform!"
#endif
}

void halEnablePrs(uint8_t channel,
                  uint8_t loc,
                  GPIO_Port_TypeDef port,
                  uint8_t pin,
                  uint8_t source,
                  uint8_t signal)
{
#if defined(_SILICON_LABS_32B_SERIES_1)
  volatile uint32_t *routeLocPtr;

  // Make sure the PRS is on and clocked
  CMU_ClockEnable(cmuClock_PRS, true);

  // Make sure this PRS channel is off while reconfiguring
  halDisablePrs(channel);

  PRS->CH[channel].CTRL = signal << _PRS_CH_CTRL_SIGSEL_SHIFT
                          | source << _PRS_CH_CTRL_SOURCESEL_SHIFT
                          | PRS_CH_CTRL_EDSEL_OFF;

  // Configure the output location for this PRS channel
  routeLocPtr   = &PRS->ROUTELOC0 + (channel / 4);
  *routeLocPtr &= ~(0xFFUL << (_PRS_ROUTELOC0_CH1LOC_SHIFT
                               * (channel % 4)));
  *routeLocPtr |= loc << (_PRS_ROUTELOC0_CH1LOC_SHIFT
                          * (channel % 4));

  // Set the enable bit for this PRS channel
  BUS_RegBitWrite(&PRS->ROUTEPEN,
                  _PRS_ROUTEPEN_CH0PEN_SHIFT + channel,
                  1);
#elif defined(_SILICON_LABS_32B_SERIES_2)
  // Make sure the PRS is on and clocked
  CMU_ClockEnable(cmuClock_PRS, true);

  PRS_SourceAsyncSignalSet(channel,
                           ( ( uint32_t ) source << _PRS_ASYNC_CH_CTRL_SOURCESEL_SHIFT),
                           ( ( uint32_t ) signal << _PRS_ASYNC_CH_CTRL_SIGSEL_SHIFT) );
  PRS_PinOutput(channel, prsTypeAsync, port, pin);
#else
  #error "Unsupported platform!"
#endif
}
