/*
 * File: ncp-main.c
 * Description: Test application for developing an IP stack.
 * Author(s): Richard Kelsey, kelsey@ember.com
 *
 * Copyright 2013 by Silicon Laboratories. All rights reserved.             *80*
 */

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "stack/include/ember.h"
#include "command-interpreter/command-interpreter2.h"
#include "stack/config/config.h"
#include "hal/hal.h"
#include "serial/serial.h"
#include "host-ncp/binary-management.h"
#include "host-ncp/ash-v3-test.h"
#include "command-interpreter/command-interpreter2-util.h"
#include "host-ncp/uart-link-protocol.h"
#include "host-ncp/ncp-link.h"
#include "host-ncp/serial-link.h"
#include "hal/micro/generic/ash-v3.h"
#include "hal/micro/led.h"
#include "stack/framework/event-queue.h"
#include <stdlib.h>
#include "hal/micro/bootloader-interface.h"

#ifdef RTOS
  #include "rtos/rtos.h"
#endif

#ifdef C8051_COBRA
  #include "hal/micro/c8051/micro.h"
#else
  #ifdef CORTEXM3
    #include "hal/micro/cortexm3/diagnostic.h"
  #endif
#endif

EmberEventData emApiAppEvents[] = {
  { NULL, NULL }
};

#if defined(__CONNECT_CONFIG__) && !defined(CORTEXM3_EMBER_MICRO)
extern const EmberEventData emAppEvents[];
#endif

uint8_t serialPort = 1;

#if !defined APP_SERIAL
  #define APP_SERIAL serialPort
#endif

EventQueue emApiAppEventQueue;

// Don't have printing enabled in library builds to save space and prevent
// clutter on the output channel
#ifdef NCP_LIBRARY
  #define NCP_PRINT(x)
#else
  #define NCP_PRINT(x) x
#endif

#define SECURITY_TO_UART_DEFAULT false

extern void halHostSerialTick(void);

//----------------------------------------------------------------
// Main.

// Command interpreter state variables
static EmberCommandState ciStateValue;

uint8_t emBootloaderMode = NO_BOOTLOADER_MODE;

static void checkBootloaderLaunch(void)
{
  #ifndef NULL_BTL
  if (emBootloaderMode != NO_BOOTLOADER_MODE
      && !ncpLinkPreparingForPowerDown()) {
    // This will NOT return on success.
    halLaunchStandaloneBootloader(emBootloaderMode);

    // Should not get here, but if we do clear the flag to make sure
    // we don't keep executing this code.
    emBootloaderMode = NO_BOOTLOADER_MODE;
  }
  #endif
}

void ncpThreadMain(void* pvParameters)
{
  // Configure our command interpreter variables
  emCommandReaderInitStateVar(&ciStateValue);

  // Initialize the event queue
  emInitializeEventQueue(&emApiAppEventQueue);

  // Initialize the NCP communications link
  ncpLinkInit();

  // Initialize the Ember stack
  emberInit();

#if defined(__CONNECT_CONFIG__) && !defined(CORTEXM3_EMBER_MICRO)
  emberAfInit();
#endif

  // Initialize the command reader so that we can process incomming commands
  emberCommandReaderInit();

  while (true) {
    halResetWatchdog();
    checkBootloaderLaunch();
    ncpLinkProcessIncoming();  // Process management and IP packets
    emberTick();
#if defined(__CONNECT_CONFIG__) && !defined(CORTEXM3_EMBER_MICRO)
    emberAfTick();
#endif
#ifdef EMBER_TEST
    simulatedTimePasses();
#endif
#if defined(__CONNECT_CONFIG__) && !defined(CORTEXM3_EMBER_MICRO)
    emberRunEvents(emAppEvents);
#endif
    emberRunEvents(emApiAppEvents);
    emberRunEventQueue(&emApiAppEventQueue);
  }
}

#ifndef NCP_LIBRARY // Non RTOS modem

const char * const emAppName = "connect-ncp";

// Our entry point is typically main(), except during testing.
#ifdef EMBER_TEST
  #define MAIN nodeMain
#else
  #define MAIN main
#endif

int MAIN(void)
{
#if defined(__CONNECT_CONFIG__) && !defined(CORTEXM3_EMBER_MICRO)
  emberAfMain();
#endif

  halInit();
  INTERRUPTS_ON();
  emberSerialInit((uint8_t)APP_SERIAL, (SerialBaudRate)APP_BAUD_RATE, (SerialParity)PARITY_NONE, 1);

  emberSerialPrintfLine(APP_SERIAL, "%s-app ready", emAppName);
  emberSerialPrintfLine(APP_SERIAL,
                        "channel %u (0x%x), node id 0x%2x, pan id 0x%2x",
                        emberGetRadioChannel(), emberGetRadioChannel(),
                        emberGetNodeId(), emberGetPanId());
  emberSerialPrintf(APP_SERIAL, "%s> ", emAppName);

  // Call the main NCP thread. This should never return
  ncpThreadMain(NULL);

  // should never hit this, but here to suppress compiler warning
  return 0;
}

#endif // NCP_LIBRARY

//----------------------------------------------------------------
// NCP stubs

#ifdef CORTEXM3_EMBER_MICRO
void halInternalRestartUart(void)
{
}
void halButtonIsr(uint8_t button, uint8_t state)
{
}
void halSimEepromCallback(uint8_t integer)
{
}
void emberStackIsrHandler(void)
{
}
#endif

void emApiEventDelayUpdatedFromIsrHandler(Event *event)
{
}

void emberAshStatusHandler(AshState state)
{
  assert(state < ASH_STATE_LAST);

  if (state == ASH_STATE_RUNNING) {
    emberSerialPrintfLine(APP_SERIAL, "ASHv3 is up");
  }
}

void emberCounterHandler(EmberCounterType type, uint16_t increment)
{
}

// stubs (this is referenced from the ashv3 code).
void emApiCounterHandler(EmberCounterType type, uint16_t increment)
{
}

void emberMarkApplicationBuffersHandler(void)
{
  ncpLinkMarkBuffers();
}

void emberEcho(uint8_t *data, uint8_t length)
{
  emberEchoReturn(data, length);
}

//---------------------------------------------------------------------------
// xon/xoff software flow control

#ifdef EMBER_APPLICATION_USES_SOFTWARE_FLOW_CONTROL
static void xOnXOffTestEventHandler(Event *event);

static EventActions xOnXOffTestEventActions = {
  &emApiAppEventQueue,
  xOnXOffTestEventHandler,
  NULL, // no marking function is needed
  "XON/XOFF test"
};

static Event xOnXOffTestEvent = { &xOnXOffTestEventActions, NULL };

void emberStartXOnXOffTest(void)
{
  // move functionality to an event handler so ASH will ack the incoming packet
  emberEventSetActive(&xOnXOffTestEvent);
}

static void xOnXOffTestEventHandler(Event *event)
{
  emberEventSetInactive(&xOnXOffTestEvent);

  // busy ourselves for a few seconds and let the UART fill up
  uint32_t limit = halCommonGetInt32uMillisecondTick() + 5000;

  while (halCommonGetInt32uMillisecondTick() < limit) {
    halResetWatchdog();
  }
}

void emberXOnHandler(void)
{
}

void emberXOffHandler(void)
{
}
#endif // EMBER_APPLICATION_USES_SOFTWARE_FLOW_CONTROL
