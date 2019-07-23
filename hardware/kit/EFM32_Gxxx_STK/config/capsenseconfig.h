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

#ifndef __CAPSENSCONFIG_H
#define __CAPSENSCONFIG_H
#ifdef __cplusplus
extern "C" {
#endif

#define ACMP_CAPSENSE                           ACMP1
#define ACMP_CAPSENSE_CLKEN                     CMU_HFPERCLKEN0_ACMP1
#define PRS_CH_CTRL_SOURCESEL_ACMP_CAPSENSE     PRS_CH_CTRL_SOURCESEL_ACMP1
#define PRS_CH_CTRL_SIGSEL_ACMPOUT_CAPSENSE     PRS_CH_CTRL_SIGSEL_ACMP1OUT

#define ACMP_CHANNELS           8             /**< Number of channels for the Analog Comparator */

#define BUTTON_CHANNEL          5             /**< Button channel */

#define SLIDER_PART0_CHANNEL    0             /**< Touch slider channel Part 0 */
#define SLIDER_PART1_CHANNEL    1             /**< Touch slider channel Part 1 */
#define SLIDER_PART2_CHANNEL    2             /**< Touch slider channel Part 2 */
#define SLIDER_PART3_CHANNEL    3             /**< Touch slider channel Part 3 */

#define CAPSENSE_CH_IN_USE  { true, true, true, true, false, true, false, false }

#ifdef __cplusplus
}
#endif
#endif
