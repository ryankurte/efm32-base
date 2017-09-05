/***************************************************************************//**
 * @file usbkbdscancodes.h
 * @brief USB HID keyboard scancodes.
 * @version 4.0.0
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silabs License Agreement. See the file
 * "Silabs_License_Agreement.txt" for details. Before using this software for
 * any purpose, you must agree to the terms of that agreement.
 *
 ******************************************************************************/

#ifndef __USBKBDSCANCODES_H
#define __USBKBDSCANCODES_H

/***************************************************************************//**
 * @addtogroup Drivers
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup UsbHid
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

char USB_Hid0ScancodeToAscii(uint8_t scancode);
char USB_Hid2ScancodeToAscii(uint8_t scancode);

uint8_t USB_AsciiToKeyModifier(char ascii);
uint8_t USB_AsciiToKeyScancode(char ascii);

#ifdef __cplusplus
}
#endif

/** @} (end group UsbHid) */
/** @} (end group Drivers) */

#endif /* __USBKBDSCANCODES_H */
