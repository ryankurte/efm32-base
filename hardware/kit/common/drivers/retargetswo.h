/***************************************************************************//**
 * @file
 * @brief Retarget stdout to SWO.
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

#ifndef RETARGETSWO_H
#define RETARGETSWO_H

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

int RETARGET_WriteChar(char c);
int RETARGET_ReadChar(void);
void RETARGET_SwoInit(void);

/**
 * @deprecated Use @ref RETARGET_SwoInit instead
 */
#define setupSWOForPrint RETARGET_SwoInit

#ifdef __cplusplus
}
#endif

/** @} (end group RetargetSWO) */
/** @} (end group kitdrv) */

#endif
