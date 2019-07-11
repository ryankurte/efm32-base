/***************************************************************************//**
 * @file
 * @brief SPV1840LR5H-B MEMS Microphone configuration file
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

#ifndef MIC_CONFIG_H
#define MIC_CONFIG_H

#include "em_adc.h"

#define MIC_CONFIG_ADC_POSSEL           adcPosSelAPORT2XCH23
#define MIC_CONFIG_ADC_ACQ_TIME         adcAcqTime1
#define MIC_CONFIG_ADC_CLOCK_FREQ       1000000

#define MIC_CONFIG_USE_LETIMER          0

#if MIC_CONFIG_USE_LETIMER
   #define MIC_CONFIG_TIMER                LETIMER0
   #define MIC_CONFIG_TIMER_CMU_CLK        cmuClock_LETIMER0

   #define MIC_CONFIG_PRS_CH               0
   #define MIC_CONFIG_ADC_PRSSEL           adcPRSSELCh0
   #define MIC_CONFIG_PRS_SOURCE           PRS_CH_CTRL_SOURCESEL_LETIMER0
   #define MIC_CONFIG_PRS_SIGNAL           PRS_CH_CTRL_SIGSEL_LETIMER0CH0
#else
   #define MIC_CONFIG_PRS_CH               4
   #define MIC_CONFIG_ADC_PRSSEL           adcPRSSELCh4
   #define MIC_CONFIG_PRS_SOURCE           PRS_CH_CTRL_SOURCESEL_CMU
   #define MIC_CONFIG_PRS_SIGNAL           PRS_CH_CTRL_SIGSEL_CMUCLKOUT1
#endif

#define MIC_CONFIG_DMA_CH               0

#endif // MIC_CONFIG_H
