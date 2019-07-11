/***************************************************************************//**
 * @file
 * @brief Configuration for the display driver for the Sharp Memory LCD
 *        LS013B7DH03
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

#ifndef _DISPLAY_LS013B7DH03_H_
#define _DISPLAY_LS013B7DH03_H_

#include "emstatus.h"

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 ********************************  DEFINES  ************************************
 ******************************************************************************/

/* Display geometry */
#define LS013B7DH03_WIDTH           (128)
#define LS013B7DH03_HEIGHT          (128)

/*******************************************************************************
 **************************    FUNCTION PROTOTYPES    **************************
 ******************************************************************************/

/* Initialization function for the LS013B7DH03 device driver. */
EMSTATUS DISPLAY_Ls013b7dh03Init(void);

#ifdef __cplusplus
}
#endif

/** @endcond */

#endif /* _DISPLAY_LS013B7DH03_H_ */
