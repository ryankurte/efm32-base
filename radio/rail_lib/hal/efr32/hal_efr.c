/***************************************************************************//**
 * @file hal_efr.c
 * @brief This file contains EFR32 specific HAL code to handle chip startup.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include <stdint.h>
#include <stdio.h>

#include "em_device.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "bsp.h"
#include "em_chip.h"
#include "pti.h"
#include "pa.h"

#include "rail_config.h"
#include "hal_common.h"

static void boardDisableSpiFlash(void);
static void boardLowPowerInit(void);
static void usecDelay(uint32_t usecs);

void halInitChipSpecific(void)
{
  CMU_HFXOInit_TypeDef hfxoInit = CMU_HFXOINIT_WSTK_DEFAULT;
  RADIO_PTIInit_t ptiInit = RADIO_PTI_INIT;
  RADIO_PAInit_t paInit;

  // Init DCDC regulator and HFXO with WSTK radio board specific parameters
  // from s025_sw\kits\SLWSTK6100A_EFR32MG\config\bspconfig.h
#ifdef EMU_DCDCINIT_WSTK_DEFAULT
  EMU_DCDCInit_TypeDef dcdcInit = EMU_DCDCINIT_WSTK_DEFAULT;
  EMU_DCDCInit(&dcdcInit);
#else
  EMU_DCDCPowerOff();
#endif
  CMU_HFXOInit(&hfxoInit);
  SystemHFXOClockSet(RADIO_CONFIG_XTAL_FREQUENCY);

  // Initialize the Packet Trace Interface (PTI) to match the configuration in
  // the board header
  RADIO_PTI_Init(&ptiInit);

  /* Switch HFCLK to HFXO and disable HFRCO */
  CMU_ClockSelectSet(cmuClock_HF, cmuSelect_HFXO);
  CMU_OscillatorEnable(cmuOsc_HFRCO, false, false);

  // Initialize the PA now that the HFXO is up and the timing is correct
  #if (RADIO_CONFIG_BASE_FREQUENCY < 1000000000UL)
  paInit = (RADIO_PAInit_t) RADIO_PA_SUBGIG_INIT;
  #else
  paInit = (RADIO_PAInit_t) RADIO_PA_2P4_INIT;
  #endif

  if (!RADIO_PA_Init(&paInit)) {
    // Error: The PA could not be initialized due to an improper configuration.
    // Please ensure your configuration is valid for the selected part.
    while (1) ;
  }

  /* Initialize other chip clocks, choose the best available clock source in this order:
        LFXO    - Crystal
        PLFRCO  - Precision RC Oscillator (500 ppm) - if available
        LFRCO   - Regular RC Oscillator
   */

  // Default to LFRCO, it's the only option always present,
  // the code below will pick a better option if available
  CMU_Osc_TypeDef lowFrequencyClockSource = cmuOsc_LFRCO;
  CMU_Select_TypeDef lowFrequencyClockSelect = cmuSelect_LFRCO;

  if (CMU->STATUS & CMU_STATUS_LFXOENS) {
    // Is the LFXO already being used? if so, use it
    lowFrequencyClockSource = cmuOsc_LFXO;
    lowFrequencyClockSelect = cmuSelect_LFXO;
#ifdef CMU_OSCENCMD_PLFRCOEN
  } else {
    // Otherwise, if the PLFCO is present, use that instead
    lowFrequencyClockSource = cmuOsc_PLFRCO;
    lowFrequencyClockSelect = cmuSelect_PLFRCO;
#endif
  }

  // Enable necessary clocks and set clock selections
  CMU_ClockEnable(cmuClock_CORELE, true);
  CMU_OscillatorEnable(lowFrequencyClockSource, true, true);
  CMU_ClockSelectSet(cmuClock_LFA, lowFrequencyClockSelect);
  CMU_ClockSelectSet(cmuClock_LFB, lowFrequencyClockSelect);
  CMU_ClockSelectSet(cmuClock_LFE, lowFrequencyClockSelect);

  // Disable any unused peripherals to ensure we enter a low power mode
  boardLowPowerInit();
}

static void boardLowPowerInit(void)
{
  boardDisableSpiFlash();
}

#define CMD_POWER_DOWN                      (0xB9)
#define CMD_POWER_UP                        (0xAB)

/** Synchronous mode init structure. */
static const USART_InitSync_TypeDef usartInit =
{
  /** Specifies whether TX and/or RX shall be enabled when init completed. */
  //USART_Enable_TypeDef    enable;
  usartEnable,

  /**
   * USART/UART reference clock assumed when configuring baudrate setup. Set
   * it to 0 if currently configurated reference clock shall be used.
   */
  //uint32_t                refFreq;
  38400000U,

  /** Desired baudrate. */
  //uint32_t                baudrate;
  12000000U,

  /** Number of databits in frame. */
  //USART_Databits_TypeDef  databits;
  usartDatabits8,

  /** Select if to operate in master or slave mode. */
  //bool                    master;
  true,

  /** Select if to send most or least significant bit first. */
  //bool                    msbf;
  true,

  /** Clock polarity/phase mode. */
  //USART_ClockMode_TypeDef clockMode;
  usartClockMode0,

  /** Enable USART Rx via PRS. */
  //bool                    prsRxEnable;
  false,

  /** Select PRS channel for USART Rx. (Only valid if prsRxEnable is true). */
  //USART_PrsRxCh_TypeDef   prsRxCh;
  usartPrsRxCh0,

  /** Enable AUTOTX mode. Transmits as long as RX is not full.
   *  If TX is empty, underflows are generated. */
  //bool                    autoTx;
  false,

  /** Auto CS enabling */
  //bool autoCsEnable;
  false,

  /** Auto CS hold time in baud cycles */
  //uint8_t autoCsHold;
  0,

  /** Auto CS setup time in baud cycles */
  //uint8_t autoCsSetup;
  0,
};

static void boardDisableSpiFlash(void)
{
  // Enable clocks needed for using the USART
  CMU_ClockEnable(cmuClock_HFPER, true);
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(cmuClock_USART1, true);

  USART_InitSync(USART1, &usartInit);

  USART1->ROUTEPEN  =   USART_ROUTEPEN_TXPEN
                      | USART_ROUTEPEN_RXPEN
                      | USART_ROUTEPEN_CLKPEN;

  USART1->ROUTELOC0 =   USART_ROUTELOC0_TXLOC_LOC11
                      | USART_ROUTELOC0_RXLOC_LOC11
                      | USART_ROUTELOC0_CLKLOC_LOC11;

  // MOSI
  GPIO_PinModeSet(gpioPortC, 6, gpioModePushPull, 0);
  // MISO
  GPIO_PinModeSet(gpioPortC, 7, gpioModeInputPull, 0);
  // CLK
  GPIO_PinModeSet(gpioPortC, 8, gpioModePushPull, 0);
  // CS#
  GPIO_PinModeSet(gpioPortA, 4, gpioModePushPull, 1);

  // Delay for 10ms to wait for the part to be ready to talk to
  usecDelay(10000);

  // Send the power on command so we can talk to it
  GPIO_PinOutClear(gpioPortA, 4);
  USART_Tx(USART1, CMD_POWER_UP);
  USART_Rx(USART1);
  GPIO_PinOutSet(gpioPortA, 4);

  // Delay for 30 usecs to make sure it's awake
  usecDelay(30);

  // Finally, send the power down command to put it in low power mode
  GPIO_PinOutClear(gpioPortA, 4);
  USART_Tx(USART1, CMD_POWER_DOWN);
  USART_Rx(USART1);
  GPIO_PinOutSet(gpioPortA, 4);
}

// Simple microsecond delay routine that has been calibrated for an EFR32
// running at 38.4MHz. This code should be moved to a more clean location
// along with all board initialization in the future.
static const uint32_t iterations_per_microsecond = 3;
static void usecDelay(uint32_t usecs)
{
  volatile uint64_t iterations = iterations_per_microsecond * usecs;

  while (iterations--) ;
}

// Create defines for the different PRS signal sources as they vary per chip
#if _SILICON_LABS_32B_SERIES_1_CONFIG == 1
// Defines for EFR32xG1 chips
#define _PRS_CH_CTRL_SOURCESEL_RAC     0x00000020UL
#define _PRS_CH_CTRL_SOURCESEL_FRC     0x00000025UL
#define _PRS_CH_CTRL_SOURCESEL_MODEML  0x00000026UL
#else
// Defines for EFR32xG12 and newer chips
#define _PRS_CH_CTRL_SOURCESEL_RAC     0x00000051UL
#define _PRS_CH_CTRL_SOURCESEL_FRC     0x00000055UL
#define _PRS_CH_CTRL_SOURCESEL_MODEML  0x00000056UL
#endif

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
        .signal = 0x02,
        .source = _PRS_CH_CTRL_SOURCESEL_RAC
      }
    }
  },
  {
    .name = "TXACTIVE",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = 0x01,
        .source = _PRS_CH_CTRL_SOURCESEL_RAC
      }
    }
  },
  {
    .name = "LNAEN",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = 0x03,
        .source = _PRS_CH_CTRL_SOURCESEL_RAC
      }
    }
  },
  {
    .name = "PAEN",
    .isPrs = true,
    .loc = {
      .prs = {
        .signal = 0x04,
        .source = _PRS_CH_CTRL_SOURCESEL_RAC
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
};

const debugSignal_t* halGetDebugSignals(uint32_t *size)
{
  if (size != NULL) {
    *size = sizeof(debugSignals) / sizeof(debugSignal_t);
  }
  return debugSignals;
}

/**
 * Define the pins that are supported for debugging on the EFR32. This includes
 * PF2, PF3, PF4, PF5, PC10, and PC11. Along with these pins there are specific
 * PRS channels that will be used to output debug information on these pins.
 * This is allo for debug and very specific to the EFR32.
 */
static const debugPin_t debugPins[] = {
  {
    .name = "PC10", // Dumbo/Jumbo/Nerio - EXP_HEADER15, WSTK_P12
    .prsChannel = 9, // PRS 0/12 9/15 10/4 11/3
    .prsLocation = 15,
    .gpioPort = gpioPortC,
    .gpioPin = 10
  },
  {
    .name = "PC11", // Dumbo/Jumbo/Nerio - EXP_HEADER16, WSTK_P13
    .prsChannel = 10, // PRS 0/13 9/16 10/5 11/4
    .prsLocation = 5,
    .gpioPort = gpioPortC,
    .gpioPin = 11
  },
  {
    .name = "PF2", // Dumbo/Nerio - WSTK_P28 (SWO)
    .prsChannel = 0, // PRS 0/2 1/1 2/0 3/7
    .prsLocation = 2,
    .gpioPort = gpioPortF,
    .gpioPin = 2
  },
  {
    .name = "PF3", // Dumbo/Nerio - EXP_HEADER13, WSTK_10 (TDI) [Jumbo PC9]
    .prsChannel = 1, // PRS 0/3 1/2 2/1 3/0
    .prsLocation = 2,
    .gpioPort = gpioPortF,
    .gpioPin = 3
  },
  {
    .name = "PF4", // Dumbo/Nerio - EXP_HEADER11, WSTK_P8 (LED0) [Jumbo PD12]
    .prsChannel = 2, // PRS 0/4 1/3 2/2 3/1
    .prsLocation = 2,
    .gpioPort = gpioPortF,
    .gpioPin = 4
  },
  {
    .name = "PF5", // Dumbo/Nerio - WSTK_P32 (LED1)
    .prsChannel = 3, // PRS 0/5 1/4 2/3 3/2
    .prsLocation = 2,
    .gpioPort = gpioPortF,
    .gpioPin = 5
  },
  {
    .name = "PC9", // Jumbo - EXP_HEADER13, WSTK_P10 [Dumbo/Nerio PF3]
    .prsChannel = 11, // PRS 0/11 9/14 10/3 11/2
    .prsLocation = 2,
    .gpioPort = gpioPortC,
    .gpioPin = 9
  },
  {
    .name = "PD9", // Jumbo - EXP_HEADER5, WSTK_P2 [Dumbo/Nerio PA3 (VCOM_CTS)]
    .prsChannel = 3, // PRS 3/8 4/0 5/6 6/11
    .prsLocation = 8,
    .gpioPort = gpioPortD,
    .gpioPin = 9
  },
  {
    .name = "PD10", // Jumbo - EXP_HEADER7, WSTK_P4 [Dumbo/Nerio PF6 (BUTTON0)]
    .prsChannel = 4, // PRS 3/9 4/1 5/0 6/12
    .prsLocation = 1,
    .gpioPort = gpioPortD,
    .gpioPin = 10
  },
  {
    .name = "PD11", // Jumbo - EXP_HEADER9, WSTK_P6 [Dumbo/Nerio PF7 (BUTTON1)]
    .prsChannel = 5, // PRS 3/10 4/2 5/1 6/13
    .prsLocation = 1,
    .gpioPort = gpioPortD,
    .gpioPin = 11
  },
  {
    .name = "PD12", // Jumbo - EXP_HEADER11, WSTK_P8 [Dumbo/Nerio PF4 (LED0)]
    .prsChannel = 6, // PRS 3/11 4/3 5/2 6/14
    .prsLocation = 14,
    .gpioPort = gpioPortD,
    .gpioPin = 12
  },
};

const debugPin_t* halGetDebugPins(uint32_t *size)
{
  if (size != NULL) {
    *size = sizeof(debugPins) / sizeof(debugPin_t);
  }
  return debugPins;
}

void halDisablePrs(uint8_t channel)
{
  // Turn the specified PRS channel off
  BUS_RegBitWrite(&PRS->ROUTEPEN,
                  _PRS_ROUTEPEN_CH0PEN_SHIFT + channel,
                  0);
}

void halEnablePrs(uint8_t channel, uint8_t loc, uint8_t source, uint8_t signal)
{
  volatile uint32_t *routeLocPtr;

  // Make sure the PRS is on and clocked
  CMU_ClockEnable(cmuClock_PRS, true);

  // Make sure this PRS channel is off while reconfiguring
  BUS_RegBitWrite(&PRS->ROUTEPEN,
                  _PRS_ROUTEPEN_CH0PEN_SHIFT + channel,
                  0);

  PRS->CH[channel].CTRL = signal << _PRS_CH_CTRL_SIGSEL_SHIFT
                          | source << _PRS_CH_CTRL_SOURCESEL_SHIFT
                          | PRS_CH_CTRL_EDSEL_OFF;

  // Configure the output location for this PRS channel
  routeLocPtr   = &PRS->ROUTELOC0 + (channel / 4);
  *routeLocPtr &= ~(0xFF << (_PRS_ROUTELOC0_CH1LOC_SHIFT
                             * (channel % 4)));
  *routeLocPtr |= loc << (_PRS_ROUTELOC0_CH1LOC_SHIFT
                          * (channel % 4));

  // Set the enable bit for this PRS channel
  BUS_RegBitWrite(&PRS->ROUTEPEN,
                  _PRS_ROUTEPEN_CH0PEN_SHIFT + channel,
                  1);
}
