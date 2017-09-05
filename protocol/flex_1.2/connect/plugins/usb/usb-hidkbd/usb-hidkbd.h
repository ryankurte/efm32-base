/**
 * @file usb-hidkbd.h
 * Copyright 2015 Silicon Laboratories, Inc.                              *80*
 */

#ifndef _USB_HIDKBD_H_
#define _USB_HIDKBD_H_

#include "usb-hidkbd/hidkbd.h"

/**
 * @addtogroup usb
 *
 * See usb-hidkbd.c for source code.
 * @{
 */

/**************************************************************************//**
 * @brief
 *   Callback function for HID output reports.
 *   This function will be called by the driver each time an output report is
 *   received by the device.
 *
 * @param[in] leds Output report byte.
 *                @n Bit 0 : State of keyboard NumLock LED.
 *                @n Bit 1 : State of keyboard CapsLock LED.
 *                @n Bit 2 : State of keyboard ScrollLock LED.
 *****************************************************************************/
void emberUsbHidkbdRxCallback(uint8_t leds);

void UsbHidkbdTxKeyScancode(HIDKBD_KeyReport_t *report);

void UsbHidkbdSetIdlerate(int newRate);
int  UsbHidkbdGetIdlerate(void);

int  UsbHidkbd_DataTransmitted(USB_Status_TypeDef status,
                               uint32_t xferred,
                               uint32_t remaining);
int  UsbHidkbd_SetupCmd(const USB_Setup_TypeDef *setup);
void UsbHidkbd_StateChangeEvent(USBD_State_TypeDef oldState,
                                USBD_State_TypeDef newState);

/** @} // END addtogroup
 */

#endif // _USB_HIDKBD_H_
