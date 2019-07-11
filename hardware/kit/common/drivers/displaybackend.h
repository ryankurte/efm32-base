/***************************************************************************//**
 * @file
 * @brief Display device backend interface
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

#ifndef _DISPLAY_BACKEND_H_
#define _DISPLAY_BACKEND_H_

#include "display.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Display
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 **************************    FUNCTION PROTOTYPES    **************************
 ******************************************************************************/

EMSTATUS DISPLAY_DeviceRegister(DISPLAY_Device_t *device);

#ifdef __cplusplus
}
#endif

/** @} (end group Display) */
/** @} (end group kitdrv) */

#endif /* _DISPLAY_BACKEND_H_ */
