/**************************************************************************//**
* @file main.c
* @brief USB Composite Device example.
* @version 4.0.0
******************************************************************************
* @section License
* <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
*******************************************************************************
*
* This file is licensed under the Silabs License Agreement. See the file
* "Silabs_License_Agreement.txt" for details. Before using this software for
* any purpose, you must agree to the terms of that agreement.
*
******************************************************************************/
#include <stdio.h>
#include "em_device.h"

#include "em_usb.h"
#include "em_cmu.h"
#include "descriptors.h"

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER

#include "stack/include/ember.h"
#include "usb-common.h"

#if (defined EMBER_AF_PLUGIN_USB_CDC)
#include "usb-cdc/usb-cdc.h"
#endif

#if (defined EMBER_AF_PLUGIN_USB_HIDKBD)
#include "usb-hidkbd/usb-hidkbd.h"
#endif

#if (defined EMBER_AF_PLUGIN_FREE_RTOS)
#include "FreeRTOS.h"
#include "task.h"
#include EMBER_AF_API_FREE_RTOS
#define RTOS_DELAY(t) rtosTaskDelay(t)
#elif (defined EMBER_AF_PLUGIN_USB_COMMON)
#define RTOS_DELAY(t) USBTIMER_DelayMs(t)
#else
#define RTOS_DELAY(t)
#endif

#if (defined EMBER_AF_PLUGIN_SERIAL)
#if defined(EMBER_TEST)
#include "com-sim.h"
#else
#include "serial/com.h"
#endif // EMBER_TEST

#include "serial/ember-printf.h"
#endif

/**************************************************************************//**
 *
 * This example shows how a Composite USB Device can be implemented.
 *
 *****************************************************************************/

/* USB_STRING_MAX should be the same as USB_FS_BULK_EP_MAXSIZE */
#define USB_STRING_MAX 64

#if (defined EMBER_AF_PLUGIN_SERIAL)
static uint8_t  usbStringBuf[USB_STRING_MAX];
static uint16_t usbStringLen;
#endif

STATIC_UBUF(usbTxBuffer0, USB_STRING_MAX);    /* USB send buffers.   */
STATIC_UBUF(usbTxBuffer1, USB_STRING_MAX);
STATIC_UBUF(usbTxBuffer2, USB_STRING_MAX);
STATIC_UBUF(usbTxBuffer3, USB_STRING_MAX);
STATIC_UBUF(usbTxBuffer4, USB_STRING_MAX);
STATIC_UBUF(usbTxBuffer5, USB_STRING_MAX);
STATIC_UBUF(usbTxBuffer6, USB_STRING_MAX);
STATIC_UBUF(usbTxBuffer7, USB_STRING_MAX);

/* Minimal circular buffer implementation. */

#define QUEUE_SIZE 8             /* Smaller code if 2^n !! */
#define WRAP(a) ((a) % QUEUE_SIZE)

static const uint8_t  *usbTxBuffer[QUEUE_SIZE] =
{ usbTxBuffer0, usbTxBuffer1, usbTxBuffer2, usbTxBuffer3,
  usbTxBuffer4, usbTxBuffer5, usbTxBuffer6, usbTxBuffer7 };
static int usbTxByteCount[QUEUE_SIZE];

static unsigned int   usbTxPutIdx;
static unsigned int   usbTxGetIdx;

static int SetupCmd(const USB_Setup_TypeDef *setup);
static void StateChangeEvent(USBD_State_TypeDef oldState,
                             USBD_State_TypeDef newState);

static const USBD_Callbacks_TypeDef callbacks =
{
  .usbReset        = NULL,
  .usbStateChange  = StateChangeEvent,
  .setupCmd        = SetupCmd,
  .isSelfPowered   = NULL,
  .sofInt          = NULL
};

const USBD_Init_TypeDef usbInitStruct =
{
  .deviceDescriptor    = &USBDESC_deviceDesc,
  .configDescriptor    = USBDESC_configDesc,
  .stringDescriptors   = USBDESC_strings,
  .numberOfStrings     = sizeof(USBDESC_strings) / sizeof(void*),
  .callbacks           = &callbacks,
  .bufferingMultiplier = USBDESC_bufferingMultiplier,
  .reserved            = 0
};

sernum iSerialNumber =
{
  .len  = 32,
  .type = USB_STRING_DESCRIPTOR,
  .name = { '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0' },
  .name[16] = '\0'
};

/**************************************************************************//**
 * @brief main - the entrypoint after reset.
 *****************************************************************************/
void emberAfPluginUsbCommonInit(void)
{
  uint8_t *tokEui64 = emberGetEui64();

  /* Windows only supports 15 characters in serial Number, so
   * rotate the Eui64 by 1 nibble. Windows also adds it to the
   * registry in order, so use BigEndian format (>). */
  uint8_t i = 1;
  int8_t j = 0;
  for ( j = 6; j >= 0; j-- ) {
    iSerialNumber.name[i++] = nibble2Ascii((tokEui64[j] >> 4) & 0xF);
    iSerialNumber.name[i++] = nibble2Ascii((tokEui64[j] >> 0) & 0xF);
  }
  iSerialNumber.name[0] = nibble2Ascii((tokEui64[7] >> 0) & 0xF);
  iSerialNumber.name[i++] = nibble2Ascii((tokEui64[7] >> 4) & 0xF);

  /* Initialize and start USB device stack. */
  USBD_Init(&usbInitStruct);
  /*
   * When using a debugger it is practical to uncomment the following three
   * lines to force host to re-enumerate the device.
   */
// USBD_Disconnect();
// USBTIMER_DelayMs(1000);
// USBD_Connect();
}

/**************************************************************************//**
 * @brief
 *   Called whenever a USB setup command is received.
 *
 * @param[in] setup
 *   Pointer to an USB setup packet.
 *
 * @return
 *   An appropriate status/error code. See USB_Status_TypeDef.
 *****************************************************************************/
static int SetupCmd(const USB_Setup_TypeDef *setup)
{
  int retVal = -1;

  /* Call SetupCmd handlers for all functions within the composite device. */

#if (defined EMBER_AF_PLUGIN_USB_CDC)
  retVal = UsbCdc_SetupCmd(setup);
#endif
#if (defined EMBER_AF_PLUGIN_USB_HIDKBD)
  retVal = UsbHidkbd_SetupCmd(setup);
#endif

  return retVal;
}

/**************************************************************************//**
 * @brief
 *   Called whenever the USB device has changed its device state.
 *
 * @param[in] oldState
 *   The device USB state just leaved. See USBD_State_TypeDef.
 *
 * @param[in] newState
 *   New (the current) USB device state. See USBD_State_TypeDef.
 *****************************************************************************/
static void StateChangeEvent(USBD_State_TypeDef oldState,
                             USBD_State_TypeDef newState)
{
  /* Call device StateChange event handlers for all relevant functions within
     the composite device. */

#if (defined EMBER_AF_PLUGIN_USB_CDC)
  UsbCdc_StateChangeEvent(oldState, newState);
#endif

#if (defined EMBER_AF_PLUGIN_USB_HIDKBD)
  UsbHidkbd_StateChangeEvent(oldState, newState);
#endif
}

void usbQueueInit(void)
{
  usbTxGetIdx = 0;
  usbTxPutIdx = 0;
}

bool usbQueueEmpty(void)
{
  return (usbTxPutIdx - usbTxGetIdx) == 0;
}

bool usbQueueFull(void)
{
  return WRAP(usbTxPutIdx + 1) == usbTxGetIdx;
}

bool usbQueueGet(uint8_t **data, int *ByteCount)
{
  if ( !usbQueueEmpty() ) {
    *data = (uint8_t *)usbTxBuffer[usbTxGetIdx];
    *ByteCount = usbTxByteCount[usbTxGetIdx];
    usbTxGetIdx = WRAP(usbTxGetIdx + 1);
    return true;
  }
  return false;
}

bool usbQueuePut(uint8_t *data, int byteCount)
{
  while ( byteCount > 0 ) {
    int sendCount = byteCount;

    if ( sendCount > USB_STRING_MAX ) {
      sendCount = USB_STRING_MAX;
    }
    if ( !usbQueueFull() ) {
      memcpy((void *)usbTxBuffer[usbTxPutIdx], data, sendCount);
      usbTxByteCount[usbTxPutIdx] = sendCount;
      usbTxPutIdx = WRAP(usbTxPutIdx + 1);
      data += sendCount;
      byteCount -= sendCount;
    } else {
      break;
    }
  }
  return byteCount <= 0;
}

/**************************************************************************//**
 * @brief Function to send a string out the USB.
 *
 * @param[in] data	Pointer to transfer data buffer.
 * @param[in] len       Number of bytes to transfer.
 *
 * @return true if sent
 *****************************************************************************/
bool usbTxString(void *data, uint32_t len)
{
  /* Ok if no data */
  if ( !len ) {
    return true;
  }
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL();
  /* USB not available */
  if ( USBD_GetUsbState() != USBD_STATE_CONFIGURED ) {
    CORE_EXIT_CRITICAL();
    return false;
  }

  bool ret = usbQueuePut(data, len);
#if (defined EMBER_AF_PLUGIN_USB_CDC)
  /* If ready for data, start it up. */
  if ( !USBD_EpIsBusy(CDC_EP_DATA_IN) ) {
    UsbCdcDataTransmitted(USB_STATUS_OK, 0, 0);
  }
#endif
#if (defined EMBER_AF_PLUGIN_USB_HIDKBD)
  /* If ready for data, start it up. */
  if ( !USBD_EpIsBusy(HIDKBD_INTR_IN_EP_ADDR) ) {
    UsbHidkbd_DataTransmitted(USB_STATUS_OK, 0, 0);
  }
#endif
  CORE_EXIT_CRITICAL();
  return ret;
}

#if (defined EMBER_AF_PLUGIN_SERIAL)
static void sendUsbString(void)
{
  int retry;

  for ( retry = 0; retry < RETRY_MAX; retry++ ) {
    if ( usbTxString(usbStringBuf, usbStringLen) ) {
      break;
    } else {
      RTOS_DELAY(RETRY_DELAY);
    }
  }
}

/* helper function to use emPrintfInternal */
static Ecode_t usbStringWrite(COM_Port_t port, uint8_t *contents,
                              uint8_t length)
{
  while ( length > 0 ) {
    uint8_t sendCount = length;

    if ( usbStringLen + sendCount > USB_STRING_MAX ) {
      sendCount = USB_STRING_MAX - usbStringLen;
    }
    memcpy((void *)&usbStringBuf[usbStringLen], contents, sendCount);
    usbStringLen += sendCount;
    contents += sendCount;
    length -= sendCount;
    if ( usbStringLen >= USB_STRING_MAX ) {
      sendUsbString();
      usbStringLen = 0;
    }
  }
  return EMBER_SUCCESS;
}
#endif

/* prints to the usb device */
uint8_t usbPrintf(const char* formatString, ...)
{
  uint8_t ret = 0;
  va_list ap;
#if (defined EMBER_AF_PLUGIN_SERIAL)
  usbStringLen = 0;
  va_start(ap, formatString);
  ret = emPrintfInternal(usbStringWrite, COM_USB, formatString, ap);
  va_end(ap);
  if ( usbStringLen ) {
    sendUsbString();
  }
#endif
  return ret;
}

uint8_t usbPrintfln(const char* formatString, ...)
{
  uint8_t ret = 0;
  va_list ap;
#if (defined EMBER_AF_PLUGIN_SERIAL)
  usbStringLen = 0;
  va_start(ap, formatString);
  ret = emPrintfInternal(usbStringWrite, COM_USB, formatString, ap);
  va_end(ap);
  usbStringWrite(COM_USB, "\r\n", 2);
  if ( usbStringLen ) {
    sendUsbString();
  }
#endif
  return ret;
}
