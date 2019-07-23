/***************************************************************************//**
 * @file
 * @brief Provide SWO/ETM TRACE configuration parameters.
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

#ifndef __TRACECONFIG_H
#define __TRACECONFIG_H

#define BSP_TRACE_SWO_LOCATION GPIO_ROUTE_SWLOCATION_LOC1

/* Enable output on pin - GPIO Port C, Pin 15. */
#define TRACE_ENABLE_PINS()                         \
  GPIO->P[2].MODEH &= ~(_GPIO_P_MODEH_MODE15_MASK); \
  GPIO->P[2].MODEH |= GPIO_P_MODEH_MODE15_PUSHPULL

#endif
