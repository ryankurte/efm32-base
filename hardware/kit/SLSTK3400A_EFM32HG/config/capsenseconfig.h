/***************************************************************************//**
 * @file
 * @brief capsense configuration parameters.
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

#ifndef __SILICON_LABS_CAPSENSCONFIG_H
#define __SILICON_LABS_CAPSENSCONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

#define ACMP_CAPSENSE                           ACMP0
#define ACMP_CAPSENSE_CLKEN                     CMU_HFPERCLKEN0_ACMP0
#define PRS_CH_CTRL_SOURCESEL_ACMP_CAPSENSE     PRS_CH_CTRL_SOURCESEL_ACMP0
#define PRS_CH_CTRL_SIGSEL_ACMPOUT_CAPSENSE     PRS_CH_CTRL_SIGSEL_ACMP0OUT

#define ACMP_CHANNELS           8             /**< Number of channels for the Analog Comparator */

#define BUTTON0_CHANNEL         4             /**< Button 0 channel */
#define BUTTON1_CHANNEL         3             /**< Button 1 channel */

#define CAPSENSE_CH_IN_USE  { false, false, false, true, true }

#ifdef __cplusplus
}
#endif
#endif /* __SILICON_LABS_CAPSENSECONFIG_H */
