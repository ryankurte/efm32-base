/***************************************************************************//**
 * @file debug_ci.c
 * @brief This file implements the debug commands for RAIL test applications.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include <stdio.h>
#include <string.h>

#include "bsp.h"
#include "command_interpreter.h"
#include "response_print.h"
#include "rail_types.h"

#include "rail.h"
#include "app_common.h"
#include "app_trx.h"
#include "hal_common.h"

#include "em_cmu.h"
#include "em_gpio.h"

/*
 * setFrequency
 *
 * Allows the user to set an arbitrary frequency if
 * the frequency override debug mode is enabled.
 */
void setFrequency(int argc, char **argv)
{
  uint32_t newFrequency = ciGetUnsigned(argv[1]);

  if (!inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    return;
  }

  if ((RAIL_DebugModeGet() & RAIL_DEBUG_MODE_FREQ_OVERRIDE) == RAIL_DEBUG_MODE_FREQ_OVERRIDE) {
    if (!RAIL_DebugFrequencyOverride(newFrequency)) {
      responsePrint(argv[0], "NewFrequency:%u", newFrequency);
    } else {
      // This won't take effect until we parse divider ranges.
      responsePrintError(argv[0], 0x14, "%u Hz is out of range and cannot be "
                                        "set as the frequency", newFrequency);
    }
  } else {
    responsePrintError(argv[0], 0x13, "Not currently in frequency override debug mode.");
  }
}

char * lookupDebugModeString(uint32_t debugMode)
{
  if (debugMode == 0) {
    return "Disabled";
  }
  if (debugMode & RAIL_DEBUG_MODE_FREQ_OVERRIDE) {
    return "Frequency Override";
  }

  return "NULL";
}
/*
 * setDebugMode()
 *
 * Configure RAIL into a debug mode.
 *
 * Current debug modes:
 * RAIL_DEBUG_MODE_FREQ_OVERRIDE - Disable RAIL's channel scheme and
 *  uses a specific frequency defined by the user.
 */
void setDebugMode(int argc, char **argv)
{
  uint32_t debugMode;

  debugMode = ciGetUnsigned(argv[1]);
  if (!RAIL_DebugModeSet(debugMode)) {
    responsePrint(argv[0], "DebugMode:%s", lookupDebugModeString(debugMode));
  } else {
    responsePrintError(argv[0], 0x15, "%d is an invalid debug mode!", debugMode);
  }
}

void getMemWord(int argc, char **argv)
{
  uint32_t *address = (uint32_t*)ciGetUnsigned(argv[1]);
  uint32_t count = 1;
  uint32_t value;

  // If there was a length given then read it out
  if (argc > 2) {
    count = ciGetUnsigned(argv[2]);
  }

  // Check for alignment
  if (((uint32_t)address % 4) != 0) {
    responsePrintError(argv[0], 0xFF, "Address 0x%.8X is not 32bit aligned", address);
    return;
  }

  responsePrintHeader(argv[0], "address:0x%.8x,value:0x%.8x");
  for (uint32_t i = 0; i < count; i++) {
    value = address[i];
    responsePrintMulti("address:0x%.8x,value:0x%.8x", address + i, value);
  }
}

void setMemWord(int argc, char **argv)
{
  uint32_t *address = (uint32_t*)ciGetUnsigned(argv[1]);
  int count = 0;
  char lengthStr[10];

  // Check for alignment
  if (((uint32_t)address % 4) != 0) {
    responsePrintError(argv[0], 0xFF, "Address 0x%.8X is not 32bit aligned", address);
    return;
  }

  // Write each word given sequentially
  for (int i = 2; i < argc; i++) {
    address[count] = ciGetUnsigned(argv[i]);
    count++;
  }

  // Make getMemWord print out everything we just wrote to
  sprintf(lengthStr, "%d", count);
  argv[2] = lengthStr;
  getMemWord(3, argv);
}

void setTxUnderflow(int argc, char **argv)
{
  bool enable = !!ciGetUnsigned(argv[1]);
  enableAppMode(TX_UNDERFLOW, enable, argv[0]);
}

void setRxOverflow(int argc, char **argv)
{
  bool enable = !!ciGetUnsigned(argv[1]);
  enableAppMode(RX_OVERFLOW, enable, argv[0]);
}

void setCalibrations(int argc, char **argv)
{
  bool enable = !!ciGetUnsigned(argv[1]);
  skipCalibrations = !enable;
  responsePrint(argv[0], "Calibrations:%s", enable ? "Enabled" : "Disabled");
}

void txCancel(int argc, char **argv)
{
  int32_t delay = ciGetSigned(argv[1]);
  txCancelDelay = delay;

  enableAppMode(TX_CANCEL, delay != 0, argv[0]);
  if ((txCancelDelay > 0) && (currentAppMode() == TX_CANCEL)) {
    txCount = 1;
    pendPacketTx();
  }
}

void getLogLevels(int argc, char **argv)
{
  responsePrint(argv[0], "Peripherals:%s,Notifications:%s",
                ((logLevel & PERIPHERAL_ENABLE) ? "Enabled" : "Disabled"),
                ((logLevel & ASYNC_RESPONSE) ? "Enabled" : "Disabled"));
}

void setPeripheralEnable(int argc, char **argv)
{
  bool enable = !!ciGetUnsigned(argv[1]);
  logLevel = enable ? (logLevel | PERIPHERAL_ENABLE)
             : (logLevel & ~(PERIPHERAL_ENABLE));
  getLogLevels(1, argv);

  // Actually turn on/off the peripherals as requested to save power
  if (enable) {
    PeripheralEnable();
  } else {
    PeripheralDisable();
  }
}

void setNotifications(int argc, char **argv)
{
  bool enable = !!ciGetUnsigned(argv[1]);
  logLevel = enable ? (logLevel | ASYNC_RESPONSE)
             : (logLevel & ~(ASYNC_RESPONSE));
  getLogLevels(1, argv);
}

void resetChip(int argc, char **argv)
{
  // Wait for any serial traffic to be done before resetting so we don't
  // output garbage characters
  serialWaitForTxIdle();

  // Use the NVIC to reset the chip
  NVIC_SystemReset();
}

void printDataRates(int argc, char **argv)
{
  responsePrint(argv[0],
                "Symbolrate:%d,Bitrate:%d",
                RAIL_SymbolRateGet(),
                RAIL_BitRateGet());
}

#define MAX_RANDOM_BYTES (1024)
#define MAX_PRINTABLE_RANDOM_BYTES (200)
static char randomPrintBuffer[2 * MAX_PRINTABLE_RANDOM_BYTES + 2];
static uint8_t randomDataBuffer[MAX_RANDOM_BYTES];

void getRandom(int argc, char **argv)
{
  uint16_t length = ciGetUnsigned(argv[1]);
  bool hidden = false;
  uint16_t result, offset = 0;
  int i;

  // Read out the hidden option if specified
  if (argc > 2) {
    hidden = !!ciGetUnsigned(argv[2]);
  }

  if (length > MAX_RANDOM_BYTES) {
    responsePrintError(argv[0], 0x10,
                       "Cannot collect more than %d random bytes.",
                       MAX_RANDOM_BYTES);
    return;
  }

  // Collect the random data
  result = RAIL_GetRadioEntropy(randomDataBuffer, length);
  if (result != length) {
    responsePrintError(argv[0], 0x11, "Error collecting random data.");
  }

  if (!hidden) {
    for (i = 0; i < length; i++) {
      int n = snprintf(randomPrintBuffer + offset,
                       sizeof(randomPrintBuffer) - offset,
                       "%.2x",
                       randomDataBuffer[i]);
      if (n < 0) {
        responsePrintError(argv[0], 0x12, "Error printing random data.");
        return;
      }
      offset += n;

      // Make sure we don't try to print too much data
      if (offset >= sizeof(randomPrintBuffer)) {
        break;
      }
    }

    responsePrint(argv[0], "Length:%u,Data:%s", result, randomPrintBuffer);
  } else {
    responsePrint(argv[0],
                  "Length:%u,DataPtr:0x%.8x",
                  result,
                  &randomDataBuffer);
  }
}

static void printDebugSignalHelp(char *cmdName,
                                 const debugPin_t *pins,
                                 const debugSignal_t *signals,
                                 uint32_t numPins,
                                 uint32_t numSignals)
{
  uint32_t i;

  printf("%s [pin] [signal] [options]\n", cmdName);
  printf("Pins: ");
  for (i = 0; i < numPins; i++) {
    if (i != 0) {
      printf(", ");
    }
    printf("%s", pins[i].name);
  }

  // Print information about the supported debug signals
  printf("\nSignals: \n");
  printf("  OFF\n  CUSTOM_PRS <source> <signal>\n  CUSTOM_LIB <event>\n");
  for (i = 0; i < numSignals; i++) {
    printf("  %s\n", signals[i].name);
  }
}

void setDebugSignal(int argc, char **argv)
{
  const debugPin_t *pin = NULL, *pinList;
  const debugSignal_t *signal = NULL, *signalList;
  debugSignal_t customSignal;
  uint32_t i;
  uint32_t numSignals, numPins;
  bool disablePin = false;

  // Get the debug signals and pins for this chip
  signalList = halGetDebugSignals(&numSignals);
  pinList = halGetDebugPins(&numPins);

  // Provide information about the pins and signals supported by this chip if
  // the help command is given. @TODO: It would be nice if this ignored the next
  // parameter somehow...
  if ((argc < 2) || (strcasecmp(argv[1], "help") == 0)) {
    printDebugSignalHelp(argv[0], pinList, signalList, numPins, numSignals);
    return;
  }

  // Make sure the pin they're trying to use is valid for this chip
  for (i = 0; i < numPins; i++) {
    if (strcasecmp(pinList[i].name, argv[1]) == 0) {
      pin = &pinList[i];
    }
  }
  if (pin == NULL) {
    responsePrintError(argv[0], 0x50, "%s is not a valid pin name", argv[1]);
    return;
  }

  // Make sure the signal they're trying to use is valid for this chip
  if (strcasecmp("CUSTOM_LIB", argv[2]) == 0) {
    // Make sure the correct parameters were given for this command
    if (!((argc == 4) && ciValidateInteger(argv[3], 'w'))) {
      responsePrintError(argv[0],
                         0x51,
                         "Invalid parameters passed to CUSTOM_LIB");
      return;
    }
    customSignal.name = "CUSTOM_LIB";
    customSignal.isPrs = false;
    customSignal.loc.debugEventNum = ciGetUnsigned(argv[3]);
    signal = &customSignal;
  } else if (strcasecmp("CUSTOM_PRS", argv[2]) == 0) {
    // Make sure that the right arguments were given for this command
    if (!((argc == 5)
          && ciValidateInteger(argv[3], 'u')
          && ciValidateInteger(argv[4], 'u'))) {
      responsePrintError(argv[0],
                         0x53,
                         "Invalid parameters passed to CUSTOM_PRS");
      return;
    }
    customSignal.name = "CUSTOM_PRS";
    customSignal.isPrs = true;
    customSignal.loc.prs.source = ciGetUnsigned(argv[3]);
    customSignal.loc.prs.signal = ciGetUnsigned(argv[4]);
    signal = &customSignal;
  } else if (strcasecmp("OFF", argv[2]) == 0) {
    disablePin = true;
  } else {
    // Search through the list of known signals for the requested one
    for (i = 0; i < numSignals; i++) {
      if (strcasecmp(signalList[i].name, argv[2]) == 0) {
        signal = &signalList[i];
      }
    }
    if (signal == NULL) {
      responsePrintError(argv[0], 0x54, "%s is not a valid signal name", argv[2]);
      return;
    }
  }

  // Turn on the GPIO clock since we're going to need that
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Disable the GPIO while configuring it
  GPIO_PinModeSet(pin->gpioPort, pin->gpioPin, gpioModeDisabled, 0);

  // If this is a disable command then just turn everything off for this debug
  // pin to stop it from outputting.
  if (disablePin) {
    // Turn off the PRS output on this pin's channel
    halDisablePrs(pin->prsChannel);
    // @TODO: Turn off the RAIL debug event for this pin
    responsePrint(argv[0], "Pin:%s,Signal:OFF", pin->name);
    return;
  }

  // Configure the PRS or library signal as needed
  if (signal->isPrs) {
    // Enable this PRS signal
    halEnablePrs(pin->prsChannel,
                 pin->prsLocation,
                 signal->loc.prs.source,
                 signal->loc.prs.signal);
  } else {
    // Turn on the RAIL debug event for this signal
  }

  // Configure this GPIO as an output low to finish enabling this signal
  GPIO_PinModeSet(pin->gpioPort, pin->gpioPin, gpioModePushPull, 0);

  responsePrint(argv[0], "Pin:%s,Signal:%s", pin->name, signal->name);
}

void forceAssert(int argc, char**argv)
{
  uint32_t errorCode = ciGetUnsigned(argv[1]);

  responsePrint(argv[0], "code:%d", errorCode);
  RAILCb_AssertFailed(errorCode);
}

void configPrintCallbacks(int argc, char**argv)
{
  enablePrintCallbacks = ciGetUnsigned(argv[1]);

  responsePrint(argv[0], "enablePrintCallbacks:0x%x", enablePrintCallbacks);
}
