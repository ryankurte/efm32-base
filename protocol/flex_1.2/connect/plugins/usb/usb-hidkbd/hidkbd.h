/***************************************************************************//**
 * @file hidkbd.h
 * @brief USB Human Interface Devices (HID) class keyboard driver.
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
#ifndef __SILICON_LABS_HIDKBD_H__
#define __SILICON_LABS_HIDKBD_H__

/***************************************************************************//**
 * @addtogroup Drivers
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup HidKeyboard
 * @{
 ******************************************************************************/

#include <stdint.h>

#include "em_usb.h"

#ifdef __cplusplus
extern "C" {
#endif

/** HID keyboard input report definition. */
#define HIDKBD_KEYREPORT_SIZE 6

EFM32_PACK_START(1)
typedef struct {
  uint8_t modifier; /**< Key modifier byte as defined in HID
                     * spec. section "8.3 Report Format for Array
                     * Items". */
  uint8_t reserved; /**< Reserved, should be set to zero. */
  uint8_t key[HIDKBD_KEYREPORT_SIZE];    /**< Array of 6 key array
                                            indices, key index codes are defined in the
                                            "Keyboard page" section of "USB HID Usage
                                            Tables document", also refer to
                                            usbkbdscancodes.c. */
} HIDKBD_KeyReport_t;
EFM32_PACK_END()

#ifdef __cplusplus
}
#endif

/** @} (end group HidKeyboard) */
/** @} (end group Drivers) */

#endif /* __SILICON_LABS_HIDKBD_H__ */
