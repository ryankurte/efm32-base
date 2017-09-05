/***************************************************************************//**
 * @file usbconfig.h
 * @brief USB protocol stack library, application supplied configuration options.
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
#ifndef __USBCONFIG_H
#define __USBCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

#define USB_DEVICE        /* Compile stack for device mode. */
#define USB_PWRSAVE_MODE (USB_PWRSAVE_MODE_ONSUSPEND | USB_PWRSAVE_MODE_ONVBUSOFF)

#define USB_TIMER USB_TIMER1

/****************************************************************************
**                                                                         **
** Specify total number of endpoints used (in addition to EP0).            **
**                                                                         **
*****************************************************************************/
/* Defined in plugin */

/****************************************************************************
**                                                                         **
** Specify number of application timers you need.                          **
**                                                                         **
*****************************************************************************/
/* Defined in plugin */

/****************************************************************************
**                                                                         **
** USB HID keyboard class device driver definitions.                       **
**                                                                         **
*****************************************************************************/

/* Define interface numbers */
#define HIDKBD_INTERFACE_NO     (0)
#define HIDKBD_NUM_INTERFACES   (1)

#define HIDKBD_NUM_EP_USED      (1)    /* Number of EP's used by CDC function */

#define HIDKBD_INTR_IN_EP_ADDR  (0x81)    /* Bit 7 must be set.               */

/* Define timer ID's */
#define HIDKBD_IDLE_TIMER       (0)     /* Timer used to implement the idle-  */
/* rate defined in the HID class spec.*/
#define HIDKBD_POLL_RATE        (24)    /* The bInterval reported with the    */
/* interrupt IN endpoint descriptor.  */

/****************************************************************************
**                                                                         **
** Configuration options for the CDC driver.                               **
**                                                                         **
*****************************************************************************/

/* Define interface numbers */
#define CDC_CTRL_INTERFACE_NO   (0)
#define CDC_DATA_INTERFACE_NO   (1)
#define CDC_NUM_INTERFACES      (2)

#define CDC_NUM_EP_USED         (3)   /* Number of EP's used by CDC function */

/* Define USB endpoint addresses for the interfaces */
#define CDC_EP_DATA_OUT   (0x01)   /* Endpoint for CDC data reception.       */
#define CDC_EP_DATA_IN    (0x81)   /* Endpoint for CDC data transmission.    */
#define CDC_EP_NOTIFY     (0x82)   /* The notification endpoint (not used).  */

#ifdef __cplusplus
}
#endif

#endif /* __USBCONFIG_H */
