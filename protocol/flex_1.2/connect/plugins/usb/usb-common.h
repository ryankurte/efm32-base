/**
 * @file usb-common.h
 * Copyright 2015 Silicon Laboratories, Inc.                              *80*
 */

#ifndef _USB_COMMON_H_
#define _USB_COMMON_H_

#define RETRY_MAX 10
#define RETRY_DELAY 10

/**
 * @addtogroup usb
 *
 * See usb-common.h for source code.
 * @{
 */

void usbQueueInit(void);
bool usbQueueEmpty(void);
bool usbQueueFull(void);
bool usbQueueGet(uint8_t **data, int *ByteCount);
bool usbQueuePut(uint8_t *data, int byteCount);

bool usbTxString(void *data, uint32_t len);

// Returns number of characters written
uint8_t usbPrintf(const char* formatString, ...);
uint8_t usbPrintfln(const char* formatString, ...);

/** @} // END addtogroup
 */

#endif // _USB_COMMON_H_
