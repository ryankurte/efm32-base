/***************************************************************************//**
 * @file
 * @brief Gecko USB Communication Device Class (CDC) driver.
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
#ifndef __CDC_H
#define __CDC_H

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup gusbcdc
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

void CDC_Init(void);
int  CDC_SetupCmd(const USB_Setup_TypeDef *setup);
void CDC_StateChangeEvent(USBD_State_TypeDef oldState,
                          USBD_State_TypeDef newState);

#ifdef __cplusplus
}
#endif

/** @} (end group gusbcdc) */
/** @} (end group kitdrv) */

#endif /* __CDC_H */
