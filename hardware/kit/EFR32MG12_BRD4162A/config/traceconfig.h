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

#ifndef TRACECONFIG_H
#define TRACECONFIG_H

#define BSP_TRACE_SWO_LOCATION GPIO_ROUTELOC0_SWVLOC_LOC0

/* Enable output on pin - GPIO Port F, Pin 2. */
#define TRACE_ENABLE_PINS()                        \
  GPIO->P[5].MODEL &= ~(_GPIO_P_MODEL_MODE2_MASK); \
  GPIO->P[5].MODEL |= GPIO_P_MODEL_MODE2_PUSHPULL

#define BSP_ETM_TRACE              /* This board supports ETM trace. */
#define BSP_TRACE_ETM_CLKLOC     0 /* ETM_TCLK = PF8  */
#define BSP_TRACE_ETM_TD0LOC     0 /* ETM_TD0  = PF9  */
#define BSP_TRACE_ETM_TD1LOC     0 /* ETM_TD1  = PF10 */
#define BSP_TRACE_ETM_TD2LOC     0 /* ETM_TD2  = PF11 */
#define BSP_TRACE_ETM_TD3LOC     0 /* ETM_TD3  = PF12 */

#endif
