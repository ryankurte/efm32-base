/***************************************************************************//**
 * @file descriptors.h
 * @brief USB descriptor prototypes for composite device example project.
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
#ifndef __SILICON_LABS_DESCRIPTORS_H__
#define __SILICON_LABS_DESCRIPTORS_H__

#include "em_usb.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const USB_DeviceDescriptor_TypeDef   USBDESC_deviceDesc;
extern const uint8_t                        USBDESC_configDesc[];
extern const void * const                   USBDESC_strings[4];
extern const uint8_t                        USBDESC_bufferingMultiplier[];

#define nibble2Ascii(n) ((n) + (((n) < 10) ? '0'  : 'A' - 10));

EFM32_PACK_START(1)
typedef struct {
  uint8_t  len;
  uint8_t  type;
  uint16_t name[17];
} sernum;
EFM32_PACK_END()

extern sernum iSerialNumber;

#if (defined EMBER_AF_PLUGIN_USB_HIDKBD)
extern const char  HIDKBD_ReportDescriptor[69];
extern const void *USBDESC_HidDescriptor;
#endif

#ifdef __cplusplus
}
#endif

#endif /* __SILICON_LABS_DESCRIPTORS_H__ */
