/***************************************************************************//**
 * @file
 * @brief VDD Voltage Check using EMLIB Voltage Comparator API
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
#include <stdbool.h>
#include "em_cmu.h"
#include "em_vcmp.h"
#include "vddcheck.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup VddCheck
 * @brief VDD Voltage Check using EMLIB Voltage Comparator (VCOMP) API
 * @details
 * @{
 ******************************************************************************/

/**************************************************************************//**
 * @brief VCMP initialization routine
 *****************************************************************************/
void VDDCHECK_Init(void)
{
  /* Enable LE peripherals */
  CMU_ClockEnable(cmuClock_CORELE, true);

  /* Enable VCMP clock */
  CMU_ClockEnable(cmuClock_VCMP, true);
}

/**************************************************************************//**
 * @brief VCMP deinitialization routine
 *****************************************************************************/
void VDDCHECK_Disable(void)
{
  /* Disable VCMP */
  VCMP_Disable();

  /* Disable clock to VCMP */
  CMU_ClockEnable(cmuClock_VCMP, false);
}

/**************************************************************************//**
 * @brief Check if voltage is higher than indicated
 *
 * @param vdd
 *        The voltage level to compare against.
 * @return
 *        Returns true if voltage is lower, false otherwise
 *****************************************************************************/
bool VDDCHECK_LowVoltage(float vdd)
{
  VCMP_Init_TypeDef vcmp = VCMP_INIT_DEFAULT;

  /* Configure VCMP */
  vcmp.triggerLevel = VCMP_VoltageToLevel(vdd);
  vcmp.warmup       = vcmpWarmTime128Cycles;
  vcmp.lowPowerRef  = false;
  vcmp.enable       = true;

  VCMP_Init(&vcmp);

  /* Delay until warm up ready */
  while (!VCMP_Ready()) ;

  /* If zero result, voltage is lower */
  if (VCMP_VDDHigher()) {
    return false;
  }

  /* Otherwise return false */
  return true;
}

/** @} (end group VddCheck) */
/** @} (end group kitdrv) */
