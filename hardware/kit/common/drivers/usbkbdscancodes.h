/***************************************************************************//**
 * @file
 * @brief USB HID keyboard scan codes.
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

#ifndef __USBKBDSCANCODES_H
#define __USBKBDSCANCODES_H

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup UsbHid
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

char USB_HidScancodeToAscii(uint8_t scancode);

#ifdef __cplusplus
}
#endif

/** @} (end group UsbHid) */
/** @} (end group kitdrv) */

#endif /* __USBKBDSCANCODES_H */
