/***************************************************************************//**
 * @file
 * @brief Retarget stdout to text display module.
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

#ifndef _RETARGETTEXTDISPLAY_H_
#define _RETARGETTEXTDISPLAY_H_

#include "emstatus.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup RetargetIo
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

#if defined(__CROSSWORKS_ARM)
int __putchar(int ch);
int __getchar(void);
#endif

/*******************************************************************************
 **************************    FUNCTION PROTOTYPES    **************************
 ******************************************************************************/

EMSTATUS RETARGET_TextDisplayInit(void);
EMSTATUS RETARGET_WriteString(char*   str);

#ifdef __cplusplus
}
#endif

/** @} (end group RetargetIo) */
/** @} (end group kitdrv) */

#endif /* _RETARGETTEXTDISPLAY_H_ */
