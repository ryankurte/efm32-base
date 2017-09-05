/**
 * @file usb-cdc.h
 * Copyright 2015 Silicon Laboratories, Inc.                              *80*
 */

#ifndef _USB_CDC_H_
#define _USB_CDC_H_

/**
 * @addtogroup usb
 *
 * See usb-cdc.c for source code.
 * @{
 */

/**************************************************************************//**
 * @brief
 *   Callback function for CDC output reports. I.E. Rx dta
 *   This function will be called by the driver each time an output report is
 *   received by the device.
 *
 * @param[in] data
 *               Address of bytes
 * @param[in] length
 *               Number of bytes
 *****************************************************************************/
void emberUsbCdcRxCallback(char * data, int length);

int UsbCdcDataTransmitted(USB_Status_TypeDef status,
                          uint32_t xferred,
                          uint32_t remaining);

int  UsbCdc_SetupCmd(const USB_Setup_TypeDef *setup);
void UsbCdc_StateChangeEvent(USBD_State_TypeDef oldState,
                             USBD_State_TypeDef newState);

/** @} // END addtogroup
 */

void emberUsbCdcRxCallback(char * data, int length);
#endif // _USB_CDC_H_
