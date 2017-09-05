#include "em_device.h"

#include "em_usb.h"
#include "em_cmu.h"

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER

#include "stack/include/ember.h"
#include "command-interpreter/command-interpreter2.h"
#include "debug-print/debug-print.h"

#include "descriptors.h"
#include "usb-common.h"
#include "usb-hidkbd/usb-hidkbd.h"
#include "usb-hidkbd/usbkbdscancodes.h"

#if (defined EMBER_AF_PLUGIN_FREE_RTOS)
#include "FreeRTOS.h"
#include "task.h"
#include EMBER_AF_API_FREE_RTOS
#define RTOS_DELAY(t) rtosTaskDelay(t)
#else
#define RTOS_DELAY(t) USBTIMER_DelayMs(t)
#endif

static uint32_t usbStartMsec = 1000;
#define MAX_LEDS_BUF 8
static char usbLedCnt[MAX_LEDS_BUF];
static int usbLedTotalCnt;
static int usbLedBadCount;
static int usbLedLast;

void emUsbHibkbdCliInit(void)
{
  memset((void *)usbLedCnt, 0, sizeof(usbLedCnt));
  usbLedTotalCnt = 0;
  usbLedBadCount = 0;
  usbLedLast = 0;
}

void emberUsbHidkbdRxCallback(uint8_t leds)
{
  usbLedTotalCnt++;
  if ( leds < MAX_LEDS_BUF ) {
    usbLedCnt[leds]++;
  } else {
    usbLedBadCount++;
  }
  usbLedLast = leds;
}

void emberAfPluginUsbHidIncomingMessage(EmberIncomingMessage *message)
{
#if (defined USB_HIDKBD_SEND_RX_DATA)
  emberAfCorePrint("Output to USB: ");
  if ( usbStartMsec ) {
    RTOS_DELAY(usbStartMsec);
  }
  if ( usbTxString(message->payload, message->length) ) {
    emberAfCorePrintln("True");
  } else {
    emberAfCorePrintln("False");
  }
#endif
}

void setUsbDelayCommand(void)
{
  usbStartMsec = emberUnsignedCommandArgument(0);
  emberAfCorePrintln("usb start delay: %d msec", usbStartMsec);
}

void setUsbIdlerateCommand(void)
{
  int idlerateMsec = emberUnsignedCommandArgument(0);

  UsbHidkbdSetIdlerate(idlerateMsec);
  emberAfCorePrintln("usb start idleRate: %d msec", idlerateMsec);
}

void usbHidDataCommand(void)
{
  uint8_t length;
  uint8_t *contents = emberStringCommandArgument(0, &length);

  emberAfCorePrint("usb: Data {");
  emberAfCorePrintBuffer(contents, length, TRUE);
  emberAfCorePrint("}: ");

  if ( usbStartMsec ) {
    RTOS_DELAY(usbStartMsec);
  }

  if ( usbTxString(contents, length) ) {
    emberAfCorePrintln("True");
  } else {
    emberAfCorePrintln("False");
  }
}

void usbkeyCommand(void)
{
  uint8_t length;
  uint8_t *contents = emberStringCommandArgument(1, &length);
  int i;
  HIDKBD_KeyReport_t newKey = { 0 };

  if ( length > HIDKBD_KEYREPORT_SIZE ) {
    length = HIDKBD_KEYREPORT_SIZE;
  }
  for ( i = 0; i < length; i++ ) {
    newKey.key[i] = contents[i];
  }
  newKey.modifier = emberUnsignedCommandArgument(0);
  emberAfCorePrint("Output to USB: { ");
  emberAfCorePrintBuffer((unsigned char const *)&newKey,
                         sizeof(newKey), TRUE);
  if ( usbStartMsec ) {
    RTOS_DELAY(usbStartMsec);
  }
  UsbHidkbdTxKeyScancode(&newKey);
  emberAfCorePrintln("} Done");
}

void usbHidPeekCommand(void)
{
  int idx = emberUnsignedCommandArgument(0);
  int len = emberUnsignedCommandArgument(1);

  if ( idx < 0 ) {
    idx = 0;
  }
  if ( idx >= MAX_LEDS_BUF ) {
    idx = MAX_LEDS_BUF - 1;
  }
  if ( len < 1 ) {
    len = 1;
  }
  if ( idx + len >= MAX_LEDS_BUF ) {
    len = MAX_LEDS_BUF - 1 - idx;
  }
  emberAfCorePrint("usb: usbLedCnt[%d],%d { ", idx, len);
  emberAfCorePrintBuffer((unsigned char const *)&usbLedCnt[idx], len, TRUE);
  emberAfCorePrintln("}");
}

void usbHidInfoCommand(void)
{
  emberAfCorePrintln("      USB state: %p",
                     USBD_GetUsbStateName(USBD_GetUsbState()));
  emberAfCorePrintln("usb start delay: %d msec", usbStartMsec);
  emberAfCorePrintln("       idleRate: %d", UsbHidkbdGetIdlerate());
  emberAfCorePrintln("     usbLedLast: %x", usbLedLast);
  emberAfCorePrintln(" usbLedTotalCnt: %d", (uint16_t)usbLedTotalCnt);
  emberAfCorePrintln(" usbLedBadCount: %d", (uint16_t)usbLedBadCount);
}
