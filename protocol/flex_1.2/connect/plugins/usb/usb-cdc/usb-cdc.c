/**************************************************************************//**
* @file usb-cdc.c
* @brief USB CDC Serial Port adapter example.
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
#include "usb-cdc/usb-cdc.h"

/**************************************************************************//**
 *
 * This example shows how a CDC based USB to Serial port adapter can be
 * implemented.
 *
 *****************************************************************************/

/*** Typedef's and defines. ***/

#define CDC_USB_RX_BUF_SIZ  USB_FS_BULK_EP_MAXSIZE /* Packet size when receiving on USB. */
#define CDC_USB_TX_BUF_SIZ  USB_FS_BULK_EP_MAXSIZE /* Packet size when sending on USB. */

/* Calculate a timeout in ms corresponding to 5 char times on current     */
/* baudrate. Minimum timeout is set to 10 ms.                             */
#define CDC_RX_TIMEOUT    EFM32_MAX(10U, 50000 / (cdcLineCoding.dwDTERate))

/* The serial port LINE CODING data structure, used to carry information  */
/* about serial port baudrate, parity etc. between host and device.       */
EFM32_PACK_START(1)
typedef struct {
  uint32_t dwDTERate;               /** Baudrate                            */
  uint8_t  bCharFormat;             /** Stop bits, 0=1 1=1.5 2=2            */
  uint8_t  bParityType;             /** 0=None 1=Odd 2=Even 3=Mark 4=Space  */
  uint8_t  bDataBits;               /** 5, 6, 7, 8 or 16                    */
  uint8_t  dummy;                   /** To ensure size is a multiple of 4 bytes */
} cdcLineCoding_TypeDef;
EFM32_PACK_END()

/*** Function prototypes. ***/

static int  UsbCdcDataReceived(USB_Status_TypeDef status, uint32_t xferred,
                               uint32_t remaining);
static int  LineCodingReceived(USB_Status_TypeDef status,
                               uint32_t xferred,
                               uint32_t remaining);

/*** Variables ***/

/*
 * The LineCoding variable must be 4-byte aligned as it is used as USB
 * transmit and receive buffer.
 */
EFM32_ALIGN(4)
EFM32_PACK_START(1)
static cdcLineCoding_TypeDef cdcLineCoding =
{
  115200, 0, 0, 8, 0
};
EFM32_PACK_END()

STATIC_UBUF(usbRxBuffer0, CDC_USB_RX_BUF_SIZ);    /* USB receive buffers.   */
STATIC_UBUF(usbRxBuffer1, CDC_USB_RX_BUF_SIZ);

static const uint8_t  *usbRxBuffer[2] = { usbRxBuffer0, usbRxBuffer1 };

static int             usbRxIndex;

/** @endcond */

/**************************************************************************//**
 * @brief CDC device initialization.
 *****************************************************************************/

extern void emUsbCdcCliInit(void);

void emberAfPluginUsbCdcInit(void)
{
  usbQueueInit();
  emUsbCdcCliInit();
}

/**************************************************************************//**
 * @brief
 *   Handle USB setup commands. Implements CDC class specific commands.
 *
 * @param[in] setup Pointer to the setup packet received.
 *
 * @return USB_STATUS_OK if command accepted.
 *         USB_STATUS_REQ_UNHANDLED when command is unknown, the USB device
 *         stack will handle the request.
 *****************************************************************************/
int UsbCdc_SetupCmd(const USB_Setup_TypeDef *setup)
{
  int retVal = USB_STATUS_REQ_UNHANDLED;

  if ( (setup->Type      == USB_SETUP_TYPE_CLASS)
       && (setup->Recipient == USB_SETUP_RECIPIENT_INTERFACE)    ) {
    switch (setup->bRequest) {
      case USB_CDC_GETLINECODING:
        /********************/
        if ( (setup->wValue    == 0)
             && (setup->wIndex    == CDC_CTRL_INTERFACE_NO) /* Interface no. */
             && (setup->wLength   == 7)                     /* Length of cdcLineCoding. */
             && (setup->Direction == USB_SETUP_DIR_IN)    ) {
          /* Send current settings to USB host. */
          USBD_Write(0, (void*) &cdcLineCoding, 7, NULL);
          retVal = USB_STATUS_OK;
        }
        break;

      case USB_CDC_SETLINECODING:
        /********************/
        if ( (setup->wValue    == 0)
             && (setup->wIndex    == CDC_CTRL_INTERFACE_NO) /* Interface no. */
             && (setup->wLength   == 7)                     /* Length of cdcLineCoding. */
             && (setup->Direction != USB_SETUP_DIR_IN)    ) {
          /* Get new settings from USB host. */
          USBD_Read(0, (void*) &cdcLineCoding, 7, LineCodingReceived);
          retVal = USB_STATUS_OK;
        }
        break;

      case USB_CDC_SETCTRLLINESTATE:
        /********************/
        if ( (setup->wIndex  == CDC_CTRL_INTERFACE_NO)      /* Interface no.  */
             && (setup->wLength == 0)    ) {                /* No data.       */
          /* Do nothing ( Non compliant behaviour !! ) */
          retVal = USB_STATUS_OK;
        }
        break;
    }
  }

  return retVal;
}

/**************************************************************************//**
 * @brief
 *   Callback function called each time the USB device state is changed.
 *   Starts CDC operation when device has been configured by USB host.
 *
 * @param[in] oldState The device state the device has just left.
 * @param[in] newState The new device state.
 *****************************************************************************/
void UsbCdc_StateChangeEvent(USBD_State_TypeDef oldState,
                             USBD_State_TypeDef newState)
{
  if ( newState == USBD_STATE_CONFIGURED ) {
    /* We have been configured, start CDC functionality ! */

    if ( oldState == USBD_STATE_SUSPENDED ) { /* Resume ?   */
    }

    /* Start sending data to USB host. */
    if ( !usbQueueEmpty() ) {
      UsbCdcDataTransmitted(USB_STATUS_OK, 0, 0);
    }

    /* Start receiving data from USB host. */
    usbRxIndex  = 0;

    USBD_Read(CDC_EP_DATA_OUT, (void*) usbRxBuffer[usbRxIndex],
              CDC_USB_RX_BUF_SIZ, UsbCdcDataReceived);
  } else if ( (oldState == USBD_STATE_CONFIGURED)
              && (newState != USBD_STATE_SUSPENDED)     ) {
    /* We have been de-configured, stop CDC functionality. */
    USBD_AbortTransfer(CDC_EP_DATA_IN);
    USBD_AbortTransfer(CDC_EP_DATA_OUT);
  } else if ( newState == USBD_STATE_SUSPENDED ) {
    /* We have been suspended, stop CDC functionality. */
    USBD_AbortTransfer(CDC_EP_DATA_IN);
    USBD_AbortTransfer(CDC_EP_DATA_OUT);
  }
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/**************************************************************************//**
 * @brief Callback function called whenever a new packet with data is received
 *        on USB.
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK.
 *****************************************************************************/
static int UsbCdcDataReceived(USB_Status_TypeDef status,
                              uint32_t xferred,
                              uint32_t remaining)
{
  (void) remaining;            /* Unused parameter. */

  if ( status == USB_STATUS_OK ) {
#ifdef USB_CDC_AUTO_ECHO
    /* Echo back the received data */
    usbTxString((void *)usbRxBuffer[usbRxIndex], xferred);
#endif
    emberUsbCdcRxCallback((char *)usbRxBuffer[usbRxIndex], xferred);

    /* Start a new USB receive transfer. */
    usbRxIndex ^= 1;
    USBD_Read(CDC_EP_DATA_OUT, (void*) usbRxBuffer[usbRxIndex],
              CDC_USB_RX_BUF_SIZ, UsbCdcDataReceived);

    /* Your USB data is already in the corresponding element of usbRxBuffer. */
  }
  return USB_STATUS_OK;
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
int UsbCdcDataTransmitted(USB_Status_TypeDef status,
                          uint32_t xferred,
                          uint32_t remaining)
{
  (void) xferred;              /* Unused parameter. */
  (void) remaining;            /* Unused parameter. */

  if ( status == USB_STATUS_OK ) {
    uint8_t *txData;
    int      txByteCount;

    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();
    if ( usbQueueGet(&txData, &txByteCount) ) {
      USBD_Write(CDC_EP_DATA_IN, txData, txByteCount,
                 UsbCdcDataTransmitted);
    }
    CORE_EXIT_CRITICAL();
  }
  return USB_STATUS_OK;
}

/**************************************************************************//**
 * @brief
 *   Callback function called when the data stage of a CDC_SET_LINECODING
 *   setup command has completed.
 *
 * @param[in] status    Transfer status code.
 * @param[in] xferred   Number of bytes transferred.
 * @param[in] remaining Number of bytes not transferred.
 *
 * @return USB_STATUS_OK if data accepted.
 *         USB_STATUS_REQ_ERR if data calls for modes we can not support.
 *****************************************************************************/
static int LineCodingReceived(USB_Status_TypeDef status,
                              uint32_t xferred,
                              uint32_t remaining)
{
  (void) remaining;

  /* We have received new serial port communication settings from USB host. */
  if ( (status == USB_STATUS_OK) && (xferred == 7) ) {
    /* For a virtual device, we ignore this. Possible code
     * to handle if needed below. */
#if 0
    /* Check bDataBits, valid values are: 5, 6, 7, 8 or 16 bits. */
    if ( cdcLineCoding.bDataBits == 5 ) {
      /* Do your job here for data length change */
    } else if ( cdcLineCoding.bDataBits == 6 ) {
      /* Do your job here for data length change */
    } else if ( cdcLineCoding.bDataBits == 7 ) {
      /* Do your job here for data length change */
    } else if ( cdcLineCoding.bDataBits == 8 ) {
      /* Do your job here for data length change */
    } else if ( cdcLineCoding.bDataBits == 16 ) {
      /* Do your job here for data length change */
    } else {
      return USB_STATUS_REQ_ERR;
    }

    /* Check bParityType, valid values are: 0=None 1=Odd 2=Even 3=Mark 4=Space  */
    if ( cdcLineCoding.bParityType == 0 ) {
      /* Do your job here for parity type change */
    } else if ( cdcLineCoding.bParityType == 1 ) {
      /* Do your job here for parity type change */
    } else if ( cdcLineCoding.bParityType == 2 ) {
      /* Do your job here for parity type change */
    } else if ( cdcLineCoding.bParityType == 3 ) {
      return USB_STATUS_REQ_ERR;
    } else if ( cdcLineCoding.bParityType == 4 ) {
      return USB_STATUS_REQ_ERR;
    } else {
      return USB_STATUS_REQ_ERR;
    }

    /* Check bCharFormat, valid values are: 0=1 1=1.5 2=2 stop bits */
    if ( cdcLineCoding.bCharFormat == 0 ) {
      /* Do your job here for character format change */
    } else if ( cdcLineCoding.bCharFormat == 1 ) {
      /* Do your job here for character format change */
    } else if ( cdcLineCoding.bCharFormat == 2 ) {
      /* Do your job here for character format change */
    } else {
      return USB_STATUS_REQ_ERR;
    }
#endif
    return USB_STATUS_OK;
  }
  return USB_STATUS_REQ_ERR;
}

/** @endcond */
