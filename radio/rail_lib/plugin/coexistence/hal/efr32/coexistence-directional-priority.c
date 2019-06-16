/***************************************************************************//**
 * @file
 * @brief Radio coexistence directional priority priority(DP)
 * @details Use the coexistence priority GPIO to communicate to the coexistence
 *   master device whether the request is for an RX or TX. In the case of an
 *   RX, the priority GPIO will be pulsed for a configurable period of time.
 *   In the case of a TX, the priority GPIO will be held through out the
 *   duration of the request.
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

#include "em_cmu.h"
#include "em_timer.h"
#include "em_prs.h"
#include "coexistence-hal.h"

#if HAL_COEX_DP_ENABLED
#if HAL_CONFIG
#ifdef RAILTEST
#include "rail_common.h"
#else //RAILTEST
#include PLATFORM_HEADER
#endif //RAILTEST
#ifdef BSP_COEX_PWM_REQ_PORT
#define BSP_COEX_DP_CC0_INTNO BSP_COEX_PWM_REQ_INTNO
#ifdef _SILICON_LABS_32B_SERIES_1
STATIC_ASSERT(BSP_COEX_DP_CC0_PORT == BSP_COEX_PWM_REQ_PORT, "BSP_COEX_DP_CC0_PORT must match BSP_COEX_PWM_REQ_PORT");
STATIC_ASSERT(BSP_COEX_DP_CC0_PIN == BSP_COEX_PWM_REQ_PIN, "BSP_COEX_DP_CC0_PIN must match BSP_COEX_PWM_REQ_PIN");
#else //!_SILICON_LABS_32B_SERIES_1
#define BSP_COEX_DP_CC0_PORT (BSP_COEX_PWM_REQ_PORT)
#define BSP_COEX_DP_CC0_PIN (BSP_COEX_PWM_REQ_PIN)
#endif //_SILICON_LABS_32B_SERIES_1
#else //!BSP_COEX_PWM_REQ_PORT
#define BSP_COEX_DP_CC0_INTNO BSP_COEX_REQ_INTNO
#ifdef _SILICON_LABS_32B_SERIES_1
STATIC_ASSERT(BSP_COEX_DP_CC0_PORT == BSP_COEX_REQ_PORT, "BSP_COEX_DP_CC0_PORT must match BSP_COEX_REQ_PORT");
STATIC_ASSERT(BSP_COEX_DP_CC0_PIN == BSP_COEX_REQ_PIN, "BSP_COEX_DP_CC0_PIN must match BSP_COEX_REQ_PIN");
#else //!_SILICON_LABS_32B_SERIES_1
#define BSP_COEX_DP_CC0_PORT (BSP_COEX_REQ_PORT)
#define BSP_COEX_DP_CC0_PIN (BSP_COEX_REQ_PIN)
#endif //_SILICON_LABS_32B_SERIES_1
#endif //BSP_COEX_PWM_REQ_PORT
#endif //HAL_CONFIG
#if BSP_COEX_PRI_ASSERT_LEVEL == 0
#error "Directional priority does not support active low priority(BSP_COEX_PRI_ASSERT_LEVEL == 0)"
#endif //BSP_COEX_PRI_ASSERT_LEVEL == 0
#if BSP_COEX_REQ_ASSERT_LEVEL == 0
#error "Directional priority does not support active low request(BSP_COEX_REQ_ASSERT_LEVEL == 0)"
#endif //BSP_COEX_REQ_ASSERT_LEVEL == 0

#define cmuClock_TIMER_DP GET_TIMER_REG(cmuClock, _BSP_COEX_DP_TIMER)
#ifdef _SILICON_LABS_32B_SERIES_1
#define TIMER_DIVISOR 16
#define TIMER_PRESC_DIV_PREFIX TIMER_CTRL_PRESC_DIV
#define PRS_CH_CTRL_SOURCESEL_TIMER_DP GET_TIMER_REG(PRS_CH_CTRL_SOURCESEL, _BSP_COEX_DP_TIMER)
#define PRS_CH_CTRL_SIGSEL_TIMER_DP GET_TIMER_REG(PRS_CH_CTRL_SIGSEL, _BSP_COEX_DP_TIMER)
#define PRS_CH_CTRL_SIGSEL_TIMERCC0_DP GET_TIMER_REG(PRS_CH_CTRL_SIGSEL_TIMER_DP, CC0)
#else //!_SILICON_LABS_32B_SERIES_1
#define TIMER_DIVISOR 2
#define TIMER_PRESC_DIV_PREFIX TIMER_CFG_PRESC_DIV
#define PRS_CH_CTRL_SIGSEL_TIMER_DP GET_TIMER_REG(PRS_ASYNC, _BSP_COEX_DP_TIMER)
#define PRS_CH_CTRL_SIGSEL_TIMERCC0_DP GET_TIMER_REG(PRS_CH_CTRL_SIGSEL_TIMER_DP, _CC0)
#endif //_SILICON_LABS_32B_SERIES_1
#define GET_TIMER_REG(reg, timer) GET_TIMER_REG_(reg, timer)
#define GET_TIMER_REG_(reg, timer) reg ## timer

#if HAL_COEX_PRI_SHARED
#define BSP_COEX_DP_MODE gpioModeWiredOr
#else //!HAL_COEX_PRI_SHARED
#define BSP_COEX_DP_MODE gpioModePushPull
#endif //HAL_COEX_PRI_SHARED

#if defined(TIMER0) && HAL_COEX_DP_TIMER == HAL_TIMER_TIMER0
#define _BSP_COEX_DP_TIMER _TIMER0
#define BSP_COEX_DP_TIMER (TIMER0)
#elif defined(TIMER1) && HAL_COEX_DP_TIMER == HAL_TIMER_TIMER1
#define _BSP_COEX_DP_TIMER _TIMER1
#define BSP_COEX_DP_TIMER (TIMER1)
#elif defined(TIMER2) && HAL_COEX_DP_TIMER == HAL_TIMER_TIMER2
#define _BSP_COEX_DP_TIMER _TIMER2
#define BSP_COEX_DP_TIMER (TIMER2)
#elif defined(TIMER3) && HAL_COEX_DP_TIMER == HAL_TIMER_TIMER3
#define _BSP_COEX_DP_TIMER _TIMER3
#define BSP_COEX_DP_TIMER (TIMER3)
#elif defined(TIMER4) && HAL_COEX_DP_TIMER == HAL_TIMER_TIMER4
#define _BSP_COEX_DP_TIMER _TIMER4
#define BSP_COEX_DP_TIMER (TIMER4)
#elif defined(TIMER5) && HAL_COEX_DP_TIMER == HAL_TIMER_TIMER5
#define _BSP_COEX_DP_TIMER _TIMER5
#define BSP_COEX_DP_TIMER (TIMER5)
#elif defined(TIMER6) && HAL_COEX_DP_TIMER == HAL_TIMER_TIMER6
#define _BSP_COEX_DP_TIMER _TIMER6
#define BSP_COEX_DP_TIMER (TIMER6)
#elif defined(WTIMER0) && HAL_COEX_DP_TIMER == HAL_TIMER_WTIMER0
#define _BSP_COEX_DP_TIMER _WTIMER0
#define BSP_COEX_DP_TIMER (WTIMER0)
#elif defined(WTIMER1) && HAL_COEX_DP_TIMER == HAL_TIMER_WTIMER1
#define _BSP_COEX_DP_TIMER _WTIMER1
#define BSP_COEX_DP_TIMER (WTIMER1)
#elif defined(WTIMER2) && HAL_COEX_DP_TIMER == HAL_TIMER_WTIMER2
#define _BSP_COEX_DP_TIMER _WTIMER2
#define BSP_COEX_DP_TIMER (WTIMER2)
#elif defined(WTIMER3) && HAL_COEX_DP_TIMER == HAL_TIMER_WTIMER3
#define _BSP_COEX_DP_TIMER _WTIMER3
#define BSP_COEX_DP_TIMER (WTIMER3)
#else
#error "Unrecognized timer selection!"
#endif

#define PRS_GPIO_SIGNAL(pin) ((pin) & 7U)

#ifdef _SILICON_LABS_32B_SERIES_1
#define PRS_GPIO_SOURCE(pin)                  \
  (((pin) > 7U) ? PRS_CH_CTRL_SOURCESEL_GPIOH \
   : PRS_CH_CTRL_SOURCESEL_GPIOL)
#define PRS_CHANNEL_SOURCE(ch)              \
  (((ch) > 7U) ? PRS_CH_CTRL_SOURCESEL_PRSH \
   : PRS_CH_CTRL_SOURCESEL_PRSL)
#else //!_SILICON_LABS_32B_SERIES_1
#define PRS_GPIO_SOURCE(pin) (PRS_ASYNC_CH_CTRL_SOURCESEL_GPIO)
#endif //_SILICON_LABS_32B_SERIES_1

#define PRS_CHANNEL_SIGNAL(ch) ((ch) & 7U)

static uint8_t directionalPriorityPulseWidthUs;
static bool directionalPriorityInitialized = false;

typedef struct PRS_ChannelConfig {
  uint32_t source;
  uint32_t signal;
  uint32_t ctrl;
  uint32_t channel;
} PRS_ChannelConfig_t;

#define TIMER_PRESC_DIV GET_TIMER_REG(TIMER_PRESC_DIV_PREFIX, TIMER_DIVISOR)
#define TIMER_FREQUENCY (CMU_ClockFreqGet(cmuClock_TIMER_DP) / TIMER_DIVISOR)
#define MICROSECONDS_PER_SECOND (1000000UL)

/** Map TIMER reference to index of device. */
#define TIMER_DEVICE_ID(timer) ( \
    (timer) == TIMER0   ? 0      \
    : (timer) == TIMER1 ? 1      \
    : (timer) == TIMER2 ? 2      \
    : (timer) == TIMER3 ? 3      \
    : -1)

static bool configDpTimer(uint8_t pulseWidthUs)
{
  uint32_t ticks = (pulseWidthUs * TIMER_FREQUENCY) / MICROSECONDS_PER_SECOND;
  // Setup TIMER for ONE-SHOT Triggers by REQUEST rising edge-----------------
  CMU_ClockEnable(cmuClock_TIMER_DP, true);     // turn on clock to TIMER

  // Reset compare output at start, run off HFPERCLK / 16, run in debug, count up,
  // Reload on rising edge, use one-shot mode
  // set PRS to track CC out level, set on start, clear on compare, PWM
  #ifdef _SILICON_LABS_32B_SERIES_1
  TIMER_Enable(BSP_COEX_DP_TIMER, false);    // stop TIMER

  BSP_COEX_DP_TIMER->CTRL = TIMER_CTRL_OSMEN
                            | TIMER_CTRL_DEBUGRUN
                            | TIMER_CTRL_RISEA_RELOADSTART
                            | TIMER_PRESC_DIV
                            | TIMER_CTRL_RSSCOIST;
  BSP_COEX_DP_TIMER->CC[0].CTRL = TIMER_CC_CTRL_MODE_PWM
                                  | TIMER_CC_CTRL_OUTINV
                                  | TIMER_CC_CTRL_CMOA_SET
                                  | TIMER_CC_CTRL_PRSCONF;

  // Setup REQUEST GPIO as TIMx_CC0 trigger input
  // Configure TX/RX PRS output to selected channel and location
  // The TIMER_CC0 GPIO selected in HWCONF must match the selected
  // BSP_COEX_REQ GPIO(or BSP_COEX_PWM_REQ GPIO if available)
  BUS_RegMaskedWrite(&BSP_COEX_DP_TIMER->ROUTELOC0,
                     _TIMER_ROUTELOC0_CC0LOC_MASK,
                     BSP_COEX_DP_CC0_LOC);
  #else // !_SILICON_LABS_32B_SERIES_1
  BSP_COEX_DP_TIMER->EN_CLR = TIMER_EN_EN;
  BSP_COEX_DP_TIMER->CFG = TIMER_CFG_OSMEN
                           | TIMER_CFG_DEBUGRUN
                           | TIMER_PRESC_DIV
                           | TIMER_CFG_RSSCOIST;
  BSP_COEX_DP_TIMER->CC[0].CFG = TIMER_CC_CFG_MODE_PWM
                                 | TIMER_CC_CFG_PRSCONF;
  BSP_COEX_DP_TIMER->EN_SET = TIMER_EN_EN;

  BSP_COEX_DP_TIMER->CTRL = TIMER_CTRL_RISEA_RELOADSTART;
  BSP_COEX_DP_TIMER->CC[0].CTRL = TIMER_CC_CTRL_OUTINV
                                  | TIMER_CC_CTRL_CMOA_SET;
  GPIO->TIMERROUTE[TIMER_DEVICE_ID(BSP_COEX_DP_TIMER)].CC0ROUTE = (BSP_COEX_DP_CC0_PORT << _GPIO_TIMER_CC0ROUTE_PORT_SHIFT)
                                                                  | (BSP_COEX_DP_CC0_PIN
                                                                     << _GPIO_TIMER_CC0ROUTE_PIN_SHIFT);
  #endif // _SILICON_LABS_32B_SERIES_1
  // pulse => CEIL(1200kHz*PULSE-1)
  TIMER_TopBufSet(BSP_COEX_DP_TIMER, ticks);
  TIMER_TopSet(BSP_COEX_DP_TIMER, ticks);
  // reset count
  TIMER_CounterSet(BSP_COEX_DP_TIMER, 0);

  // pulse => CEIL(1200kHz*PULSE-1)
  TIMER_CompareBufSet(BSP_COEX_DP_TIMER, 0, ticks);
  TIMER_CompareSet(BSP_COEX_DP_TIMER, 0, ticks);
  TIMER_Enable(BSP_COEX_DP_TIMER, true);   // start TIMER

  return true;
}

__STATIC_INLINE void configPrsChain(PRS_ChannelConfig_t *prsConfig,
                                    unsigned int channelCount)
{
  for (unsigned int ch = 0; ch < channelCount; ++ch) {
#ifdef _SILICON_LABS_32B_SERIES_1
    PRS->CH[prsConfig[ch].channel].CTRL = 0U;
#endif //_SILICON_LABS_32B_SERIES_1
    PRS_SourceAsyncSignalSet(prsConfig[ch].channel,
                             prsConfig[ch].source,
                             prsConfig[ch].signal);
#ifdef _SILICON_LABS_32B_SERIES_1
    PRS->CH[prsConfig[ch].channel].CTRL |= prsConfig[ch].ctrl;
#else //!_SILICON_LABS_32B_SERIES_1
    PRS_Combine(prsConfig[ch].channel,
                WRAP_PRS_ASYNC(prsConfig[ch].channel - 1),
                (PRS_Logic_t)prsConfig[ch].ctrl);
#endif //_SILICON_LABS_32B_SERIES_1
  }
}

#define  CONFIG_PRS_CHAIN(prsChain) (configPrsChain(prsChain, \
                                                    sizeof(prsChain) / sizeof(prsChain[0])))

//#define ANDNEXT_APPROACH // incompatible with default FEM controls
#define ORPREV_APPROACH // compatible with default FEM controls

PRS_ChannelConfig_t prsChainOff[] = {
#ifdef BSP_COEX_PRI_INTNO
  {
    .source = PRS_GPIO_SOURCE(BSP_COEX_PRI_INTNO),
    .signal = PRS_GPIO_SIGNAL(BSP_COEX_PRI_INTNO),
#ifndef _SILICON_LABS_32B_SERIES_1
    .ctrl = prsLogic_A,
#endif
    .channel = BSP_COEX_DP_CHANNEL
  }
#elif defined(_SILICON_LABS_32B_SERIES_1) //!defined(BSP_COEX_PRI_INTNO)
  {
    .source = PRS_CHANNEL_SOURCE(BSP_COEX_DP_REQUEST_INV_CHANNEL),
    .signal = PRS_CHANNEL_SIGNAL(BSP_COEX_DP_REQUEST_INV_CHANNEL),
    .ctrl = PRS_CH_CTRL_INV,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 2)
  },
  {
    .source = PRS_CHANNEL_SOURCE(BSP_COEX_DP_REQUEST_INV_CHANNEL),
    .signal = PRS_CHANNEL_SIGNAL(BSP_COEX_DP_REQUEST_INV_CHANNEL),
    .ctrl = PRS_CH_CTRL_ORPREV | PRS_CH_CTRL_INV,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 1)
  },
  {
    .source = PRS_CHANNEL_SOURCE(BSP_COEX_DP_REQUEST_INV_CHANNEL),
    .signal = PRS_CHANNEL_SIGNAL(BSP_COEX_DP_REQUEST_INV_CHANNEL),
    .ctrl = PRS_CH_CTRL_ORPREV | PRS_CH_CTRL_INV,
    .channel = BSP_COEX_DP_CHANNEL
  },
#else //!(defined(_SILICON_LABS_32B_SERIES_1) || defined(BSP_COEX_PRI_INTNO))
  {
    .source = PRS_GPIO_SOURCE(BSP_COEX_DP_CC0_INTNO),
    .signal = PRS_GPIO_SIGNAL(BSP_COEX_DP_CC0_INTNO),
    .ctrl = prsLogic_A,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 2)
  },
  {
    .source = PRS_GPIO_SOURCE(BSP_COEX_DP_CC0_INTNO),
    .signal = PRS_GPIO_SIGNAL(BSP_COEX_DP_CC0_INTNO),
    .ctrl = prsLogic_A,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 1)
  },
  {
    .source = PRS_GPIO_SOURCE(BSP_COEX_DP_CC0_INTNO),
    .signal = PRS_GPIO_SIGNAL(BSP_COEX_DP_CC0_INTNO),
    .ctrl = prsLogic_A_AND_NOT_B,
    .channel = BSP_COEX_DP_CHANNEL
  }
#endif //BSP_COEX_PRI_INTNO
};

#ifdef _SILICON_LABS_32B_SERIES_2_CONFIG_2
#define PRS_RAC_LNAEN PRS_RACL_LNAEN
#endif //_SILICON_LABS_32B_SERIES_2_CONFIG_2

#ifndef _SILICON_LABS_32B_SERIES_1
PRS_ChannelConfig_t prsChainOn[] = {
#ifdef BSP_COEX_PRI_INTNO
  {
    .source = PRS_GPIO_SOURCE(BSP_COEX_PRI_INTNO),
    .signal = PRS_GPIO_SIGNAL(BSP_COEX_PRI_INTNO),
    .ctrl = prsLogic_A,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 3)
  },
  {
    .signal = PRS_CH_CTRL_SIGSEL_TIMERCC0_DP,
    .ctrl = prsLogic_NOT_A_AND_B,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 2)
  },
  {
    .signal = PRS_RAC_LNAEN,
    .ctrl = prsLogic_A_AND_NOT_B,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 1)
  },
#else //!BSP_COEX_PRI_INTNO
  {
    .signal = PRS_CH_CTRL_SIGSEL_TIMERCC0_DP,
    .ctrl = prsLogic_NOT_A,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 2)
  },
  {
    .signal = PRS_RAC_LNAEN,
    .ctrl = prsLogic_A,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 1)
  },
#endif //BSP_COEX_PRI_INTNO
  {
    .source = PRS_GPIO_SOURCE(BSP_COEX_DP_CC0_INTNO),
    .signal = PRS_GPIO_SIGNAL(BSP_COEX_DP_CC0_INTNO),
    .ctrl = prsLogic_A_AND_NOT_B,
    .channel = BSP_COEX_DP_CHANNEL
  }
};
#elif defined(ORPREV_APPROACH)
PRS_ChannelConfig_t prsChainOn[] = {
  {
    .signal = PRS_RAC_LNAEN,
    .ctrl = PRS_CH_CTRL_INV,
    .channel = BSP_COEX_DP_RACLNAEN_INV_CHANNEL
  },
  {
    .source = PRS_GPIO_SOURCE(BSP_COEX_DP_CC0_INTNO),
    .signal = PRS_GPIO_SIGNAL(BSP_COEX_DP_CC0_INTNO),
    .ctrl = PRS_CH_CTRL_INV,
    .channel = BSP_COEX_DP_REQUEST_INV_CHANNEL
  },
#ifdef BSP_COEX_PRI_INTNO
  {
    .source = PRS_GPIO_SOURCE(BSP_COEX_PRI_INTNO),
    .signal = PRS_GPIO_SIGNAL(BSP_COEX_PRI_INTNO),
    .ctrl = PRS_CH_CTRL_INV,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 3)
  },
  {
    .source = PRS_CH_CTRL_SOURCESEL_TIMER_DP,
    .signal = PRS_CH_CTRL_SIGSEL_TIMERCC0_DP,
    .ctrl = PRS_CH_CTRL_ORPREV | PRS_CH_CTRL_INV,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 2)
  },
  {
    .source = PRS_CHANNEL_SOURCE(BSP_COEX_DP_RACLNAEN_INV_CHANNEL),
    .signal = PRS_CHANNEL_SIGNAL(BSP_COEX_DP_RACLNAEN_INV_CHANNEL),
    .ctrl = PRS_CH_CTRL_ORPREV | PRS_CH_CTRL_INV,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 1)
  },
#else //!BSP_COEX_PRI_INTNO
  {
    .source = PRS_CH_CTRL_SOURCESEL_TIMER_DP,
    .signal = PRS_CH_CTRL_SIGSEL_TIMERCC0_DP,
    .ctrl = PRS_CH_CTRL_INV,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 2)
  },
  {
    .source = PRS_CHANNEL_SOURCE(BSP_COEX_DP_RACLNAEN_INV_CHANNEL),
    .signal = PRS_CHANNEL_SIGNAL(BSP_COEX_DP_RACLNAEN_INV_CHANNEL),
    .ctrl = PRS_CH_CTRL_INV,
    .channel = WRAP_PRS_ASYNC(BSP_COEX_DP_CHANNEL - 1)
  },
#endif //BSP_COEX_PRI_INTNO
  {
    .source = PRS_CHANNEL_SOURCE(BSP_COEX_DP_REQUEST_INV_CHANNEL),
    .signal = PRS_CHANNEL_SIGNAL(BSP_COEX_DP_REQUEST_INV_CHANNEL),
    .ctrl = PRS_CH_CTRL_ORPREV | PRS_CH_CTRL_INV,
    .channel = BSP_COEX_DP_CHANNEL
  }
};
#elif ANDNEXT_APPROACH
PRS_ChannelConfig_t prsChainOn[] = {
  {
    .source = COEX_DP_PRI_PRS_SOURCE,
    .signal = COEX_DP_PRI_PRS_SIGNAL,
    .channel = 6
  },
  {
    .source = PRS_CH_CTRL_SOURCESEL_TIMER_DP,
    .signal = PRS_CH_CTRL_SIGSEL_TIMERCC0_DP,
    .ctrl = PRS_CH_CTRL_ANDNEXT | PRS_CH_CTRL_INV,
    .channel = 5
  },
  {
    .signal = PRS_RAC_LNAEN,
    .ctrl = PRS_CH_CTRL_ANDNEXT | PRS_CH_CTRL_INV,
    .channel = 4
  },
  {
    .source = COEX_DP_REQ_PRS_SOURCE,
    .signal = COEX_DP_REQ_PRS_SIGNAL,
    .ctrl = PRS_CH_CTRL_ANDNEXT,
    .channel = 3
  }
};
#else
#error "No PRS Chain selected!"
#endif //ANDNEXT_APPROACH

bool COEX_HAL_ConfigDp(uint8_t pulseWidthUs)
{
  // Common PRS setup (clock enable, REQUEST and PRIORITY GPIO INT PRS sources)
  // enable clock to PRS
  CMU_ClockEnable(cmuClock_PRS, true);

#ifdef BSP_COEX_PRI_PORT
  // Disable priority and request interrupts
  GPIO_ExtIntConfig(BSP_COEX_PRI_PORT,
                    BSP_COEX_PRI_PIN,
                    BSP_COEX_PRI_INTNO,
                    false,
                    false,
                    false);
#endif //BSP_COEX_PRI_PORT
  GPIO_ExtIntConfig(BSP_COEX_DP_CC0_PORT,
                    BSP_COEX_DP_CC0_PIN,
                    BSP_COEX_DP_CC0_INTNO,
                    false,
                    false,
                    false);
  // Common PRS wrap-up (enable PRIORITY GPIO, route PRS output to GPIO)
  // enable PRIORITY output pin with initial value of 0
  GPIO_PinModeSet(BSP_COEX_DP_PORT,
                  BSP_COEX_DP_PIN,
                  BSP_COEX_DP_MODE,
                  0);
#ifdef _SILICON_LABS_32B_SERIES_1
  volatile uint32_t * routeRegister;
  // Configure directional priority PRS output to selected channel and location
  if (BSP_COEX_DP_CHANNEL < 4) {
    routeRegister = &PRS->ROUTELOC0;
  } else if (BSP_COEX_DP_CHANNEL < 8) {
    routeRegister = &PRS->ROUTELOC1;
  } else if (BSP_COEX_DP_CHANNEL < 12) {
    routeRegister = &PRS->ROUTELOC2;
  } else {
    return false; // error
  }
  // Route PRS CH/LOC to PRIORITY GPIO output
  BUS_RegMaskedWrite(routeRegister,
                     0xFFU << ((BSP_COEX_DP_CHANNEL & 3) * 8),
                     BSP_COEX_DP_LOC << ((BSP_COEX_DP_CHANNEL & 3) * 8));
  BUS_RegMaskedSet(&PRS->ROUTEPEN, (1 << BSP_COEX_DP_CHANNEL));
#else //!_SILICON_LABS_32B_SERIES_1
  PRS_PinOutput(BSP_COEX_DP_CHANNEL, prsTypeAsync, BSP_COEX_DP_PORT, BSP_COEX_DP_PIN);
#endif //_SILICON_LABS_32B_SERIES_1
  return COEX_HAL_SetDpPulseWidth(pulseWidthUs);
}

bool COEX_HAL_SetDpPulseWidth(uint8_t pulseWidthUs)
{
  if (directionalPriorityInitialized
      && directionalPriorityPulseWidthUs == pulseWidthUs) {
    return true;
  }
  directionalPriorityInitialized = true;
  directionalPriorityPulseWidthUs = pulseWidthUs;
  if (pulseWidthUs == 0) {
    #ifndef _SILICON_LABS_32B_SERIES_1
    BSP_COEX_DP_TIMER->EN_SET = TIMER_EN_EN;
    #endif //!_SILICON_LABS_32B_SERIES_1
    TIMER_Enable(BSP_COEX_DP_TIMER, false);
    CONFIG_PRS_CHAIN(prsChainOff);
    return true;
  }
  if (!configDpTimer(directionalPriorityPulseWidthUs)) {
    return false;
  }
  CONFIG_PRS_CHAIN(prsChainOn);
  return true;
}

uint8_t COEX_HAL_GetDpPulseWidth(void)
{
  return directionalPriorityPulseWidthUs;
}

#else //!HAL_COEX_DP_ENABLED

bool COEX_HAL_ConfigDp(uint8_t pulseWidthUs)
{
  (void)pulseWidthUs;
  return false;
}

uint8_t COEX_HAL_GetDpPulseWidth(void)
{
  return 0;
}

bool COEX_HAL_SetDpPulseWidth(uint8_t pulseWidthUs)
{
  (void)pulseWidthUs;
  return false;
}
#endif //HAL_COEX_DP_ENABLED
