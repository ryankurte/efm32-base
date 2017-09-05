/**************************************************************************//**
* @file usb-hidkbd.c
* @brief USB HID keyboard device example.
* @version 4.0.0
******************************************************************************
* @section License
* <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
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

#include CONFIGURATION_HEADER

#include "descriptors.h"
#include "usb-common.h"
#include "usb-hidkbd/usb-hidkbd.h"
#include "usb-hidkbd/usbkbdscancodes.h"

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

/** Default idle-rate recommended in the USB HID class specification. */
#define DEFAULT_IDLE_RATE  500

/*** Variables ***/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

static uint8_t           *txData;
static int                txByteCount;
static int                sendUpKey;
static uint8_t            lastModifier;
static uint32_t           tmpBuffer;
static uint8_t            idleRate;

/* The last keyboard report sent to host. */
EFM32_ALIGN(4)
static HIDKBD_KeyReport_t lastSentReport;

/* The last keyboard report reported to the driver. */
EFM32_ALIGN(4)
static HIDKBD_KeyReport_t lastKnownReport;

static bool QueueEmpty(void);
static bool QueueFull(void);
static bool QueueGet(HIDKBD_KeyReport_t *element);
static void QueueInit(void);
static bool QueuePut(HIDKBD_KeyReport_t *element);

/**************************************************************************//**
 * @brief HIDKBD device initialization.
 *****************************************************************************/

extern void emUsbHibkbdCliInit(void);

void emberAfPluginUsbHidkbdInit(void)
{
  usbQueueInit();
  QueueInit();
  memset(&lastSentReport, 0, sizeof(HIDKBD_KeyReport_t));
  memset(&lastKnownReport, 0, sizeof(HIDKBD_KeyReport_t));
  txByteCount = 0;
  lastModifier = 0;
  sendUpKey = 0;
  idleRate = DEFAULT_IDLE_RATE / 4;     /* Unit is 4 millisecond. */
}

/**************************************************************************//**
 * @brief Callback function called whenever a packet with data has been
 *        transmitted on USB
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK.
 *****************************************************************************/
int UsbHidkbd_DataTransmitted(USB_Status_TypeDef status,
                              uint32_t xferred,
                              uint32_t remaining)
{
  (void) xferred;              /* Unused parameter. */
  (void) remaining;            /* Unused parameter. */

  if ( (status == USB_STATUS_OK) && (QueueEmpty()) ) {
    if ( sendUpKey ) {
      HIDKBD_KeyReport_t upKey = { 0 };

      upKey.modifier = lastModifier;
      if ( lastModifier == 0 ) {
        if ( txByteCount ) {
          txData++;
          txByteCount--;
        }
        sendUpKey = 0;
      }
      lastModifier = 0;
      UsbHidkbdTxKeyScancode(&upKey);
    } else {
      if ( !txByteCount ) {
        /* Get more characters to send. */
        CORE_DECLARE_IRQ_STATE;
        CORE_ENTER_CRITICAL();
        if ( !usbQueueGet(&txData, &txByteCount) ) {
          txByteCount = 0;
        }
        CORE_EXIT_CRITICAL();
      }
      if ( txByteCount ) {
        uint8_t cc = *txData;
        HIDKBD_KeyReport_t newKey = { 0 };

        newKey.modifier = USB_AsciiToKeyModifier(cc);
        newKey.key[0] = USB_AsciiToKeyScancode(cc);
        lastModifier = newKey.modifier;
        UsbHidkbdTxKeyScancode(&newKey);
        /* Need to allways include an Up event */
        sendUpKey = 1;
      }
    }
  }
  return USB_STATUS_OK;
}

/**************************************************************************//**
 * @brief
 *   Timeout function for the idleRate timer. The idleRate is set by the host
 *   device driver with the SET_IDLE setup command.
 *   If idleRate is set to 0 the idleRate timer is completely stopped.
 *   This function will always send a keyboard report to host.
 *****************************************************************************/
static void IdleTimeout(void)
{
  /* If there is a keyboard event in the queue, we send it to the host. */
  /* If not we just resend the last one sent. */
  if ( !QueueEmpty() ) {
    /* A new keyboard event. */
    QueueGet(&lastSentReport);
  }

  USBD_Write(HIDKBD_INTR_IN_EP_ADDR, &lastSentReport,
             sizeof(HIDKBD_KeyReport_t),
             UsbHidkbd_DataTransmitted);

  /* Schedule next idle event at current idle rate, idleRate unit is 4 ms. */
  USBTIMER_Start(HIDKBD_IDLE_TIMER, idleRate * 4, IdleTimeout);
}

/**************************************************************************//**
 * @brief
 *   Callback function called when the data stage of a USB_HID_SET_REPORT
 *   setup command has completed.
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK.
 *****************************************************************************/
static int OutputReportReceived(USB_Status_TypeDef status,
                                uint32_t xferred,
                                uint32_t remaining)
{
  (void) remaining;

  if (    (status        == USB_STATUS_OK)
          && (xferred       == 1) ) {
    emberUsbHidkbdRxCallback((uint8_t)tmpBuffer);
  }

  return USB_STATUS_OK;
}

/** @endcond */

/***************************************************************************//**
 * @brief
 *  Report a keyboard press/release event.
 *
 * @param[in] report
 *  Pointer to a HIDKBD_KeyReport_t struct with new key pressed information.
 ******************************************************************************/
void UsbHidkbdTxKeyScancode(HIDKBD_KeyReport_t *report)
{
  int retry;

  lastKnownReport = *report;

  if ( idleRate != 0 ) {    /* Put keyboard events into a queue. */
    /* Put the kbd event in the queue, it will be retrieved and reported */
    /* to host in the idleRate timeout function.                         */
    for ( retry = 0; retry < RETRY_MAX; retry++ ) {
      if ( QueuePut(report) ) {
        break;
      } else {
        RTOS_DELAY(RETRY_DELAY);
      }
    }
  } else { /* idleRate == 0, send report immediately. */
    lastSentReport = *report;
    for ( retry = 0; retry < RETRY_MAX; retry++ ) {
      if ( USBD_Write(HIDKBD_INTR_IN_EP_ADDR, &lastSentReport,
                      sizeof(HIDKBD_KeyReport_t),
                      UsbHidkbd_DataTransmitted) != USB_STATUS_EP_BUSY ) {
        break;
      } else {
        RTOS_DELAY(RETRY_DELAY);
      }
    }
  }
}

void UsbHidkbdSetIdlerate(int newRate)
{
  if ( (newRate != 0) && (newRate > HIDKBD_POLL_RATE) ) {
    newRate = HIDKBD_POLL_RATE;
  }
  idleRate = newRate / 4;
  USBTIMER_Stop(HIDKBD_IDLE_TIMER);
  if ( (USBD_GetUsbState() == USBD_STATE_CONFIGURED) && (idleRate != 0) ) {
    /* Schedule next idle event at current idle rate, idleRate unit is 4 ms. */
    USBTIMER_Start(HIDKBD_IDLE_TIMER, idleRate * 4, IdleTimeout);
  }
}

int  UsbHidkbdGetIdlerate(void)
{
  return idleRate * 4;
}

/**************************************************************************//**
 * @brief
 *   Handle USB setup commands. Implements HID class specific commands.
 *   This function must be called each time the device receive a setup command.
 *
 *
 * @param[in] setup Pointer to the setup packet received.
 *
 * @return USB_STATUS_OK if command accepted,
 *         USB_STATUS_REQ_UNHANDLED when command is unknown. In the latter case
 *         the USB device stack will handle the request.
 *****************************************************************************/
int UsbHidkbd_SetupCmd(const USB_Setup_TypeDef *setup)
{
  STATIC_UBUF(hidDesc, USB_HID_DESCSIZE);

  int retVal = USB_STATUS_REQ_UNHANDLED;

  if ( (setup->Type      == USB_SETUP_TYPE_STANDARD)
       && (setup->Direction == USB_SETUP_DIR_IN)
       && (setup->Recipient == USB_SETUP_RECIPIENT_INTERFACE)    ) {
    /* A HID device must extend the standard GET_DESCRIPTOR command   */
    /* with support for HID descriptors.                              */
    switch ( setup->bRequest ) {
      case GET_DESCRIPTOR:
        /********************/
        if ( (setup->wValue >> 8) == USB_HID_REPORT_DESCRIPTOR ) {
          USBD_Write(0, (void*)HIDKBD_ReportDescriptor,
                     EFM32_MIN(sizeof(HIDKBD_ReportDescriptor), setup->wLength),
                     NULL);
          retVal = USB_STATUS_OK;
        } else if ( (setup->wValue >> 8) == USB_HID_DESCRIPTOR ) {
          /* The HID descriptor might be misaligned ! */
          memcpy(hidDesc, USBDESC_HidDescriptor, USB_HID_DESCSIZE);
          USBD_Write(0, hidDesc, EFM32_MIN(USB_HID_DESCSIZE, setup->wLength),
                     NULL);
          retVal = USB_STATUS_OK;
        }
        break;
    }
  } else if ( (setup->Type      == USB_SETUP_TYPE_CLASS)
              && (setup->Recipient == USB_SETUP_RECIPIENT_INTERFACE)
              && (setup->wIndex    == HIDKBD_INTERFACE_NO)    ) {
    /* Implement the necessary HID class specific commands.           */
    switch ( setup->bRequest ) {
      case USB_HID_SET_REPORT:
        /********************/
        if ( ( (setup->wValue >> 8) == 2)                       /* Output report */
             && ( (setup->wValue & 0xFF) == 0)                  /* Report ID     */
             && (setup->wLength           == 1)                 /* Report length */
             && (setup->Direction         != USB_SETUP_DIR_IN)    ) {
          USBD_Read(0, (void*)&tmpBuffer, 1, OutputReportReceived);
          retVal = USB_STATUS_OK;
        }
        break;

      case USB_HID_GET_REPORT:
        /********************/
        if ( ( (setup->wValue >> 8) == 1)                       /* Input report  */
             && ( (setup->wValue & 0xFF) == 0)                  /* Report ID     */
             && (setup->wLength           == 8)                 /* Report length */
             && (setup->Direction         == USB_SETUP_DIR_IN)    ) {
          USBD_Write(HIDKBD_INTR_IN_EP_ADDR, &lastKnownReport,
                     sizeof(HIDKBD_KeyReport_t), UsbHidkbd_DataTransmitted);
          retVal = USB_STATUS_OK;
        }
        break;

      case USB_HID_SET_IDLE:
        /********************/
        if ( ( (setup->wValue & 0xFF) == 0)                    /* Report ID     */
             && (setup->wLength          == 0)
             && (setup->Direction        != USB_SETUP_DIR_IN)    ) {
          idleRate = setup->wValue >> 8;
          if ( (idleRate != 0) && (idleRate > (HIDKBD_POLL_RATE / 4) ) ) {
            idleRate = HIDKBD_POLL_RATE / 4;
          }
          USBTIMER_Stop(HIDKBD_IDLE_TIMER);
          if ( idleRate != 0 ) {
            /* Schedule next idle event at current idle rate, idleRate unit is 4 ms. */
            USBTIMER_Start(HIDKBD_IDLE_TIMER, idleRate * 4, IdleTimeout);
          }
          retVal = USB_STATUS_OK;
        }
        break;

      case USB_HID_GET_IDLE:
        /********************/
        if ( (setup->wValue    == 0)                    /* Report ID     */
             && (setup->wLength   == 1)
             && (setup->Direction == USB_SETUP_DIR_IN)    ) {
          *(uint8_t*)&tmpBuffer = idleRate;
          USBD_Write(0, (void*)&tmpBuffer, 1, NULL);
          retVal = USB_STATUS_OK;
        }
        break;
    }
  }

  return retVal;
}

/**************************************************************************//**
 * @brief
 *   Handle USB state change events, this function must be called each time
 *   the USB device state is changed.
 *
 * @param[in] oldState The device state the device has just left.
 * @param[in] newState The new device state.
 *****************************************************************************/
void UsbHidkbd_StateChangeEvent(USBD_State_TypeDef oldState,
                                USBD_State_TypeDef newState)
{
  if ( newState == USBD_STATE_CONFIGURED ) {
    /* We have been configured, start HID functionality ! */
    if ( oldState != USBD_STATE_SUSPENDED ) { /* Resume ?   */
      idleRate = DEFAULT_IDLE_RATE / 4;     /* Unit is 4 millisecond. */
      QueueInit();
    }
    if ( idleRate ) {
      USBTIMER_Start(HIDKBD_IDLE_TIMER, idleRate * 4, IdleTimeout);
    } else {
      UsbHidkbd_DataTransmitted(USB_STATUS_OK, 0, 0);
    }
  } else if ( (oldState == USBD_STATE_CONFIGURED)
              && (newState != USBD_STATE_SUSPENDED) ) {
    /* We have been de-configured, stop HID functionality */
    USBD_AbortTransfer(HIDKBD_INTR_IN_EP_ADDR);
    /* Reduce current consumption to below 2.5 mA.    */
    USBTIMER_Stop(HIDKBD_IDLE_TIMER);
  } else if ( newState == USBD_STATE_SUSPENDED ) {
    /* We have been suspended, stop HID functionality */
    USBD_AbortTransfer(HIDKBD_INTR_IN_EP_ADDR);
    /* Reduce current consumption to below 2.5 mA.    */
    USBTIMER_Stop(HIDKBD_IDLE_TIMER);
  }
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/* Minimal circular buffer implementation. */

#define QUEUE_SIZE 16             /* Smaller code if 2^n !! */
#define WRAP(a) ((a) % QUEUE_SIZE)

typedef struct ringBuffer_t{
  unsigned int        putIdx;
  unsigned int        getIdx;
  HIDKBD_KeyReport_t  buf[QUEUE_SIZE];
} ringBuffer_t;

ringBuffer_t queue;

static bool QueueEmpty(void)
{
  return (queue.putIdx - queue.getIdx) == 0;
}

static bool QueueFull(void)
{
  return WRAP(queue.putIdx + 1) == queue.getIdx;
}

static bool QueueGet(HIDKBD_KeyReport_t *element)
{
  if ( !QueueEmpty() ) {
    *element = queue.buf[queue.getIdx];
    queue.getIdx = WRAP(queue.getIdx + 1);
    return true;
  }
  return false;
}

static void QueueInit(void)
{
  queue.getIdx = 0;
  queue.putIdx = 0;
}

static bool QueuePut(HIDKBD_KeyReport_t *element)
{
  if ( !QueueFull() ) {
    queue.buf[queue.putIdx] = *element;
    queue.putIdx = WRAP(queue.putIdx + 1);
    return true;
  }
  return false;
}

/** @endcond */
