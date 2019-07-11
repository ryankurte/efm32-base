/***************************************************************************//**
 * @file
 * @brief Provide configuration parameters for EM4 wakeup button.
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

#ifndef __SILICON_LABS_EM4CONFIG_H__
#define __SILICON_LABS_EM4CONFIG_H__

#include "bspconfig.h"

#define EM4_WU_PB           PB1
#define EM4_WU_PB_EN        (1 << 17) /* GPIO_EM4WU1 = PF7 = pushbutton 1 */
#define EM4_WU_PB_PIN       BSP_GPIO_PB1_PIN
#define EM4_WU_PB_PORT      BSP_GPIO_PB1_PORT
#define EM4_WU_PB_STR       "PB1"

#define EM4_NON_WU_PB          PB0
#define EM4_NON_WU_PB_PIN      BSP_GPIO_PB0_PIN
#define EM4_NON_WU_PB_PORT     BSP_GPIO_PB0_PORT
#define EM4_NON_WU_PB_STR      "PB0"

#endif /* __SILICON_LABS_EM4CONFIG_H__ */
