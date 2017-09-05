#include <stdio.h>
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
#include "usb-cdc/usb-cdc.h"

#define MAX_USB_BUF 200
static char usbRxLine[MAX_USB_BUF];
static int usbRxLineStart;
static int usbRxLineLen;
static int usbRxCount;

void emUsbCdcCliInit(void)
{
  memset((void *)usbRxLine, 0, sizeof(usbRxLine));
  usbRxLineStart = 0;
  usbRxLineLen = 0;
  usbRxCount = 0;
}

void emberUsbCdcRxCallback(char * data, int length)
{
  int cur = usbRxLineStart + usbRxLineLen;
  int inCur = 0;
  usbRxCount += length;
  while ( length > 0 ) {
    char cc = data[inCur++];
    if ( cur + 1 >= MAX_USB_BUF ) {
      usbRxLineStart = 0;
      usbRxLineLen = 0;
      cur = 0;
    }
    usbRxLine[cur++] = cc;
    length--;
    usbRxLineLen++;
    if ( (cc == '\n') || (cc == '\r') ) {
      usbRxLineStart = cur;
      usbRxLineLen = 0;
    }
  }
}

void emberAfPluginUsbCdcIncomingMessage(EmberIncomingMessage *message)
{
#if (defined USB_CDC_SEND_RX_DATA)
  emberAfCorePrint("Output to USB: ");
  if ( usbTxString(message->payload, message->length) ) {
    emberAfCorePrintln("True");
  } else {
    emberAfCorePrintln("False");
  }
#endif
}

void usbCdcDataCommand(void)
{
  uint8_t length;
  uint8_t *contents = emberStringCommandArgument(0, &length);

  emberAfCorePrint("usb: Data {");
  emberAfCorePrintBuffer(contents, length, TRUE);
  emberAfCorePrint("}: ");

  if ( usbTxString(contents, length) ) {
    emberAfCorePrintln("True");
  } else {
    emberAfCorePrintln("False");
  }
}

void usbCdcPeekCommand(void)
{
  int idx = emberUnsignedCommandArgument(0);
  int len = emberUnsignedCommandArgument(1);

  if ( idx < 0 ) {
    idx = 0;
  }
  if ( idx >= MAX_USB_BUF ) {
    idx = MAX_USB_BUF - 1;
  }
  if ( len < 1 ) {
    len = 1;
  }
  if ( idx + len >= MAX_USB_BUF ) {
    len = MAX_USB_BUF - 1 - idx;
  }
  emberAfCorePrint("usb: rxData[%d],%d { ", idx, len);
  emberAfCorePrintBuffer((unsigned char const *)&usbRxLine[idx], len, TRUE);
  emberAfCorePrintln("}");
}

void usbCdcInfoCommand(void)
{
  static char curLine[MAX_USB_BUF + 1];

  emberAfCorePrintln("      USB state: %p",
                     USBD_GetUsbStateName(USBD_GetUsbState()));
  emberAfCorePrintln(" usbRxLineStart: %d", (uint16_t)usbRxLineStart);
  emberAfCorePrintln("   usbRxLineLen: %d", (uint16_t)usbRxLineLen);
  emberAfCorePrintln("     usbRxCount: %d", (uint16_t)usbRxCount);
  /* Display data as a string. */
  memcpy((void *)curLine, (void *)&usbRxLine[usbRxLineStart], usbRxLineLen);
  curLine[usbRxLineLen] = 0;
  emberAfCorePrintln("        usbData: %s", curLine);
}
