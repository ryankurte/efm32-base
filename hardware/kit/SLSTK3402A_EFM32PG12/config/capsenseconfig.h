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

#ifndef __SILICON_LABS_CAPSENSCONFIG_H__
#define __SILICON_LABS_CAPSENSCONFIG_H__
#ifdef __cplusplus
extern "C" {
#endif

/* Use ACMP0 module for capsense */
#define ACMP_CAPSENSE                           ACMP0
#define ACMP_CAPSENSE_CMUCLOCK                  cmuClock_ACMP0
#define PRS_CH_CTRL_SOURCESEL_ACMP_CAPSENSE     PRS_CH_CTRL_SOURCESEL_ACMP0
#define PRS_CH_CTRL_SIGSEL_ACMPOUT_CAPSENSE     PRS_CH_CTRL_SIGSEL_ACMP0OUT

/* On the SLSTK3402A the touch buttons are connected to PC0, PC1, PC2 and PC3.
 *
 * Pin  | APORT Channel (for ACMP0)
 * -------------------------
 * PC0  | APORT1XCH0 or APORT2YCH0
 * PC1  | APORT1YCH1 or APORT2XCH1
 * PC2  | APORT1XCH2 or APORT2YCH2
 * PC3  | APORT1YCH3 or APORT2XCH3
 *
 */
#define CAPSENSE_CHANNELS       { acmpInputAPORT1XCH0, acmpInputAPORT2XCH1, acmpInputAPORT1XCH2, acmpInputAPORT2XCH3 }
#define BUTTON0_CHANNEL         0             /**< Button 0 channel */
#define BUTTON1_CHANNEL         3             /**< Button 1 channel */
#define ACMP_CHANNELS           4             /**< Number of channels in use for capsense */
#define NUM_SLIDER_CHANNELS     4             /**< The kit has a slider with 4 pads */

#ifdef __cplusplus
}
#endif
#endif /* __SILICON_LABS_CAPSENSCONFIG_H__ */
