/***************************************************************************//**
 * @file descriptors.c
 * @brief USB descriptors for CDC Serial Port adapter example project.
 *   and  USB descriptors for HID keyboard example project.
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
#include "descriptors.h"

#include CONFIGURATION_HEADER

#if (defined EMBER_AF_PLUGIN_USB_CDC)
#define NUM_INTERFACES CDC_NUM_INTERFACES
#else
#if (defined EMBER_AF_PLUGIN_USB_HIDKBD)
#define NUM_INTERFACES HIDKBD_NUM_INTERFACES
#else
#error Just the USB common plugin does not make sense.
#endif
#endif

EFM32_ALIGN(4)
const USB_DeviceDescriptor_TypeDef USBDESC_deviceDesc =
{
  .bLength            = USB_DEVICE_DESCSIZE,
  .bDescriptorType    = USB_DEVICE_DESCRIPTOR,
  .bcdUSB             = 0x0200,
#if (defined EMBER_AF_PLUGIN_USB_CDC)
  .bDeviceClass       = USB_CLASS_CDC,
#else
  .bDeviceClass       = 0,
#endif
  .bDeviceSubClass    = 0,
  .bDeviceProtocol    = 0,
  .bMaxPacketSize0    = USB_FS_CTRL_EP_MAXSIZE,
  .idVendor           = 0x10C4,
#if (defined EMBER_AF_PLUGIN_USB_CDC)
  .idProduct          = 0x0003,
#else
  .idProduct          = 0x0002,
#endif
  .bcdDevice          = 0x0000,
  .iManufacturer      = 1,
  .iProduct           = 2,
  .iSerialNumber      = 3,
  .bNumConfigurations = 1
};

#define CDC_MISC_DESCRIPTOR_LEN (USB_CDC_HEADER_FND_DESCSIZE          \
                                 + USB_CDC_CALLMNG_FND_DESCSIZE       \
                                 + USB_CDC_ACM_FND_DESCSIZE           \
                                 + USB_INTERFACE_ASSOCIATION_DESCSIZE \
                                 + 5)/*CDC Union Functional descriptor length*/

#if (defined EMBER_AF_PLUGIN_USB_CDC)
#define CONFIG_DESC_TOTAL_LEN                  \
  (USB_CONFIG_DESCSIZE                         \
   + (USB_INTERFACE_DESCSIZE * NUM_INTERFACES) \
   + (USB_ENDPOINT_DESCSIZE  * NUM_EP_USED)    \
   + CDC_MISC_DESCRIPTOR_LEN)
#else
#define CONFIG_DESC_TOTAL_LEN                  \
  (USB_CONFIG_DESCSIZE                         \
   + (USB_INTERFACE_DESCSIZE * NUM_INTERFACES) \
   + (USB_ENDPOINT_DESCSIZE  * NUM_EP_USED)    \
   + USB_HID_DESCSIZE)
#endif

#if (defined EMBER_AF_PLUGIN_USB_HIDKBD)
/** HID report descriptor created with usb.org's "HID Descriptor Tool(DT)"
    version 2.4. The descriptor defines an input report which matches
    the HIDKBD_KeyReport_t typedef, and an output report with 3 bits
    for controlling NumLock, CapsLock and ScrollLock keyboard LED's
 */
EFM32_ALIGN(4)
const char HIDKBD_ReportDescriptor[69] =
{
  0x05, 0x01,                      // USAGE_PAGE (Generic Desktop)
  0x09, 0x06,                      // USAGE (Keyboard)
  0xa1, 0x01,                      // COLLECTION (Application)
  0x05, 0x07,                      //   USAGE_PAGE (Keyboard)
  0x19, 0xe0,                      //   USAGE_MINIMUM (Keyboard LeftControl)
  0x29, 0xe7,                      //   USAGE_MAXIMUM (Keyboard Right GUI)
  0x15, 0x00,                      //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                      //   LOGICAL_MAXIMUM (1)
  0x75, 0x01,                      //   REPORT_SIZE (1)
  0x95, 0x08,                      //   REPORT_COUNT (8)
  0x81, 0x02,                      //   INPUT (Data,Var,Abs)
  0x15, 0x00,                      //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                      //   LOGICAL_MAXIMUM (1)
  0x75, 0x01,                      //   REPORT_SIZE (1)
  0x95, 0x08,                      //   REPORT_COUNT (8)
  0x81, 0x01,                      //   INPUT (Cnst,Ary,Abs)
  0x19, 0x00,                      //   USAGE_MINIMUM (Reserved (no event indicated))
  0x29, 0x65,                      //   USAGE_MAXIMUM (Keyboard Application)
  0x15, 0x00,                      //   LOGICAL_MINIMUM (0)
  0x25, 0x65,                      //   LOGICAL_MAXIMUM (101)
  0x75, 0x08,                      //   REPORT_SIZE (8)
  0x95, 0x06,                      //   REPORT_COUNT (6)
  0x81, 0x00,                      //   INPUT (Data,Ary,Abs)
  0x05, 0x08,                      //   USAGE_PAGE (LEDs)
  0x19, 0x01,                      //   USAGE_MINIMUM (Num Lock)
  0x29, 0x03,                      //   USAGE_MAXIMUM (Scroll Lock)
  0x15, 0x00,                      //   LOGICAL_MINIMUM (0)
  0x25, 0x01,                      //   LOGICAL_MAXIMUM (1)
  0x75, 0x01,                      //   REPORT_SIZE (1)
  0x95, 0x03,                      //   REPORT_COUNT (3)
  0x91, 0x02,                      //   OUTPUT (Data,Var,Abs)
  0x75, 0x01,                      //   REPORT_SIZE (1)
  0x95, 0x05,                      //   REPORT_COUNT (5)
  0x91, 0x01,                      //   OUTPUT (Cnst,Ary,Abs)
  0xc0                             // END_COLLECTION
};
#endif

EFM32_ALIGN(4)
const uint8_t USBDESC_configDesc[] =
{
  /*** Configuration descriptor ***/
  USB_CONFIG_DESCSIZE,    /* bLength                                   */
  USB_CONFIG_DESCRIPTOR,  /* bDescriptorType                           */
  CONFIG_DESC_TOTAL_LEN,  /* wTotalLength (LSB)                        */
  CONFIG_DESC_TOTAL_LEN >> 8, /* wTotalLength (MSB)                      */
  NUM_INTERFACES,         /* bNumInterfaces                            */
  1,                      /* bConfigurationValue                       */
  0,                      /* iConfiguration                            */
  CONFIG_DESC_BM_RESERVED_D7      /* bmAttrib: Self powered            */
  | CONFIG_DESC_BM_SELFPOWERED,
  CONFIG_DESC_MAXPOWER_mA(100),   /* bMaxPower: 100 mA                 */

#if (defined EMBER_AF_PLUGIN_USB_CDC)
  /*** CDC Function                                                 ***/
  /*** IAD (Interface Association Descriptor) for the CDC function  ***/
  USB_INTERFACE_ASSOCIATION_DESCSIZE, /* bLength: Interface Descriptor size */
  USB_INTERFACE_ASSOCIATION_DESCRIPTOR, /* bDescriptorType      */
  CDC_CTRL_INTERFACE_NO,  /* bFirstInterface                    */
  2,                      /* bInterfaceCount                    */
  USB_CLASS_CDC,          /* bFunctionClass                     */
  USB_CLASS_CDC_ACM,      /* bFunctionSubClass                  */
  0,                      /* bFunctionProtocol                  */
  0,                      /* iFunction                          */

  /*** Communication Class Interface descriptor ***/
  USB_INTERFACE_DESCSIZE, /* bLength               */
  USB_INTERFACE_DESCRIPTOR,/* bDescriptorType      */
  CDC_CTRL_INTERFACE_NO,  /* bInterfaceNumber      */
  0,                      /* bAlternateSetting     */
  1,                      /* bNumEndpoints         */
  USB_CLASS_CDC,          /* bInterfaceClass       */
  USB_CLASS_CDC_ACM,      /* bInterfaceSubClass    */
  0,                      /* bInterfaceProtocol    */
  0,                      /* iInterface            */

  /*** CDC Header Functional descriptor ***/
  USB_CDC_HEADER_FND_DESCSIZE, /* bFunctionLength  */
  USB_CS_INTERFACE_DESCRIPTOR, /* bDescriptorType  */
  USB_CLASS_CDC_HFN,      /* bDescriptorSubtype    */
  0x20,                   /* bcdCDC spec.no LSB    */
  0x01,                   /* bcdCDC spec.no MSB    */

  /*** CDC Call Management Functional descriptor ***/
  USB_CDC_CALLMNG_FND_DESCSIZE, /* bFunctionLength */
  USB_CS_INTERFACE_DESCRIPTOR,  /* bDescriptorType */
  USB_CLASS_CDC_CMNGFN,   /* bDescriptorSubtype    */
  0,                      /* bmCapabilities        */
  CDC_DATA_INTERFACE_NO,  /* bDataInterface        */

  /*** CDC Abstract Control Management Functional descriptor ***/
  USB_CDC_ACM_FND_DESCSIZE, /* bFunctionLength     */
  USB_CS_INTERFACE_DESCRIPTOR, /* bDescriptorType  */
  USB_CLASS_CDC_ACMFN,    /* bDescriptorSubtype    */
  0x02,                   /* bmCapabilities        */
  /* The capabilities that this configuration supports:                   */
  /* D7..D4: RESERVED (Reset to zero)                                     */
  /* D3: 1 - Device supports the notification Network_Connection.         */
  /* D2: 1 - Device supports the request Send_Break                       */
  /* D1: 1 - Device supports the request combination of Set_Line_Coding,  */
  /*         Set_Control_Line_State, Get_Line_Coding, and the             */
  /*         notification Serial_State.                                   */
  /* D0: 1 - Device supports the request combination of Set_Comm_Feature, */
  /*         Clear_Comm_Feature, and Get_Comm_Feature.                    */

  /*** CDC Union Functional descriptor ***/
  5,                      /* bFunctionLength       */
  USB_CS_INTERFACE_DESCRIPTOR, /* bDescriptorType  */
  USB_CLASS_CDC_UNIONFN,  /* bDescriptorSubtype    */
  CDC_CTRL_INTERFACE_NO,  /* bControlInterface     */
  CDC_DATA_INTERFACE_NO,  /* bSubordinateInterface0*/

  /*** CDC Notification endpoint descriptor ***/
  USB_ENDPOINT_DESCSIZE,  /* bLength               */
  USB_ENDPOINT_DESCRIPTOR,/* bDescriptorType       */
  CDC_EP_NOTIFY,          /* bEndpointAddress (IN) */
  USB_EPTYPE_INTR,        /* bmAttributes          */
  USB_FS_INTR_EP_MAXSIZE, /* wMaxPacketSize (LSB)  */
  0,                      /* wMaxPacketSize (MSB)  */
  0xFF,                   /* bInterval             */

  /*** Data Class Interface descriptor ***/
  USB_INTERFACE_DESCSIZE, /* bLength               */
  USB_INTERFACE_DESCRIPTOR,/* bDescriptorType      */
  CDC_DATA_INTERFACE_NO,  /* bInterfaceNumber      */
  0,                      /* bAlternateSetting     */
  2,                      /* bNumEndpoints         */
  USB_CLASS_CDC_DATA,     /* bInterfaceClass       */
  0,                      /* bInterfaceSubClass    */
  0,                      /* bInterfaceProtocol    */
  0,                      /* iInterface            */

  /*** CDC Data interface endpoint descriptors ***/
  USB_ENDPOINT_DESCSIZE,  /* bLength               */
  USB_ENDPOINT_DESCRIPTOR,/* bDescriptorType       */
  CDC_EP_DATA_IN,         /* bEndpointAddress (IN) */
  USB_EPTYPE_BULK,        /* bmAttributes          */
  USB_FS_BULK_EP_MAXSIZE, /* wMaxPacketSize (LSB)  */
  0,                      /* wMaxPacketSize (MSB)  */
  0,                      /* bInterval             */

  USB_ENDPOINT_DESCSIZE,  /* bLength               */
  USB_ENDPOINT_DESCRIPTOR,/* bDescriptorType       */
  CDC_EP_DATA_OUT,        /* bEndpointAddress (OUT)*/
  USB_EPTYPE_BULK,        /* bmAttributes          */
  USB_FS_BULK_EP_MAXSIZE, /* wMaxPacketSize (LSB)  */
  0,                      /* wMaxPacketSize (MSB)  */
  0,                      /* bInterval             */
#else
  /*** Interface descriptor ***/
  USB_INTERFACE_DESCSIZE, /* bLength               */
  USB_INTERFACE_DESCRIPTOR,/* bDescriptorType      */
  HIDKBD_INTERFACE_NO,    /* bInterfaceNumber      */
  0,                      /* bAlternateSetting     */
  NUM_EP_USED,            /* bNumEndpoints         */
  0x03,                   /* bInterfaceClass (HID) */
  0,                      /* bInterfaceSubClass    */
  1,                      /* bInterfaceProtocol    */
  0,                      /* iInterface            */

  /*** HID descriptor ***/
  USB_HID_DESCSIZE,       /* bLength               */
  USB_HID_DESCRIPTOR,     /* bDescriptorType       */
  0x11,                   /* bcdHID (LSB)          */
  0x01,                   /* bcdHID (MSB)          */
  0,                      /* bCountryCode          */
  1,                      /* bNumDescriptors       */
  USB_HID_REPORT_DESCRIPTOR,            /* bDecriptorType        */
  sizeof(HIDKBD_ReportDescriptor),      /* wDescriptorLength(LSB)*/
  sizeof(HIDKBD_ReportDescriptor) >> 8, /* wDescriptorLength(MSB)*/

  /*** Endpoint descriptor ***/
  USB_ENDPOINT_DESCSIZE,  /* bLength               */
  USB_ENDPOINT_DESCRIPTOR,/* bDescriptorType       */
  HIDKBD_INTR_IN_EP_ADDR, /* bEndpointAddress (IN) */
  USB_EPTYPE_INTR,        /* bmAttributes          */
  USB_FS_INTR_EP_MAXSIZE, /* wMaxPacketSize (LSB)  */
  0,                      /* wMaxPacketSize (MSB)  */
  HIDKBD_POLL_RATE,       /* bInterval             */
#endif
};

#if (defined EMBER_AF_PLUGIN_USB_HIDKBD)
const void *USBDESC_HidDescriptor = (void*)
                                    &USBDESC_configDesc[USB_CONFIG_DESCSIZE + USB_INTERFACE_DESCSIZE];
#endif

STATIC_CONST_STRING_DESC_LANGID(langID, 0x04, 0x09);
STATIC_CONST_STRING_DESC(iManufacturer, 'S', 'i', 'l', 'i', 'c', 'o', 'n', ' ', 'L', \
                         'a', 'b', 'o', 'r', 'a', 't', 'o', 'r', 'i',                \
                         'e', 's', ' ', 'I', 'n', 'c', '.');
#if (defined EMBER_AF_PLUGIN_USB_CDC)
STATIC_CONST_STRING_DESC(iProduct, 'E', 'Z', 'R', '3', '2', ' ', 'U', 'S', 'B', \
                         ' ', 'C', 'D', 'C', ' ', 's', 'e', 'r', 'i',           \
                         'a', 'l', ' ', 'p', 'o', 'r', 't', ' ', 'd',           \
                         'e', 'v', 'i', 'c', 'e');
#else
STATIC_CONST_STRING_DESC(iProduct, 'E', 'Z', 'R', '3', '2', ' ', 'U', 'S', 'B', \
                         ' ', 'H', 'I', 'D', ' ', 'K', 'e', 'y', 'b',           \
                         'o', 'a', 'r', 'd');
#endif

const void * const USBDESC_strings[] =
{
  &langID,
  &iManufacturer,
  &iProduct,
  &iSerialNumber
};

/* Endpoint buffer sizes */
/* 1 = single buffer, 2 = double buffering, 3 = tripple buffering ... */
const uint8_t USBDESC_bufferingMultiplier[NUM_EP_USED + 1] =
{
  1,        /* Common Control endpoint.           */
#if (defined EMBER_AF_PLUGIN_USB_CDC)
  1, 2, 2,  /* CDC interrupt and bulk endpoints.  */
#else
  1,        /* UHIDKBD endpoints. */
#endif
};
