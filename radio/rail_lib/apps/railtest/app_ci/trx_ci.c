/***************************************************************************//**
 * @file trx_ci.c
 * @brief This file implements the tx/rx commands for RAIL test applications.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include <string.h>

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "app_common.h"

// Helper routine to parse the RAIL timer mode from a command line string
bool parseTimeModeFromString(char *str, RAIL_TimeMode_t *mode);

void tx(int argc, char **argv)
{
  uint32_t newTxCount = ciGetUnsigned(argv[1]);
  radioTransmit(newTxCount, argv[0]);
  // Do not use RAIL_TxStartWithOptions
  txOptionsPtr = NULL;
}

void txWithOptions(int argc, char **argv)
{
  uint32_t newTxCount = ciGetUnsigned(argv[1]);
  radioTransmit(newTxCount, argv[0]);
  // Do not use RAIL_TxStartWithOptions
  txOptionsPtr = &txOptions;
}

void configTxOptions(int argc, char **argv)
{
  // Putting masks here for this because they should
  // ONLY be used here. In RAIL, TX Options are
  // maintained with RAIL_TxOptions_t, not bitmasks.
  #define CONFIG_WAIT_FOR_ACK_MASK  1
  #define CONFIG_REMOVE_CRC_MASK  2
  #define CONFIG_SYNC_WORD_ID_MASK 4

  uint32_t options = ciGetUnsigned(argv[1]);

  txOptions.waitForAck = !!(options & CONFIG_WAIT_FOR_ACK_MASK);
  txOptions.removeCrc = !!(options & CONFIG_REMOVE_CRC_MASK);
  txOptions.syncWordId = !!(options & CONFIG_SYNC_WORD_ID_MASK);

  txOptionsPtr = &txOptions;
  responsePrint(argv[0], "waitForAck:%s,removeCrc:%s,syncWordId:%d",
                (txOptions.waitForAck ? "True" : "False"),
                (txOptions.removeCrc ? "True" : "False"),
                txOptions.syncWordId);

  // Making sure these are not used outside this function
  #undef CONFIG_WAIT_FOR_ACK_MASK
  #undef CONFIG_REMOVE_CRC_MASK
  #undef CONFIG_SYNC_WORD_ID_MASK
}

void txAtTime(int argc, char **argv)
{
  bool isAbs = true;
  RAIL_TimeMode_t mode;
  uint32_t txTime = ciGetUnsigned(argv[1]);

  // Attempt to parse the time mode if specified
  if (argc >= 3) {
    if (!parseTimeModeFromString(argv[2], &mode)) {
      responsePrintError(argv[0], 28, "Invalid time mode");
      return;
    }
    if (mode == RAIL_TIME_DELAY) {
      isAbs = false;
    } else if (mode == RAIL_TIME_ABSOLUTE) {
      isAbs = true;
    } else {
      responsePrintError(argv[0], 28, "Invalid time mode");
      return;
    }
  }
  setNextPacketTime(txTime, isAbs);
  setNextAppMode(TX_SCHEDULED, argv[0]);
}

void abortScheduledTxDuringRx(int argc, char **argv)
{
  bool abort = !!ciGetUnsigned(argv[1]);

  RAIL_SetAbortScheduledTxDuringRx(abort);

  responsePrint(argv[0],
                "abortScheduledTxDuringRx:%s",
                abort ? "True" : "False");
}

void txAfterRx(int argc, char **argv)
{
  uint32_t delay = ciGetUnsigned(argv[1]);
  txAfterRxDelay = delay;
  enableAppMode(SCHTX_AFTER_RX, (delay != 0), argv[0]);
}

void rxConfig(int argc, char ** argv)
{
  if ((!inAppMode(NONE, argv[0]))
      || (!inRadioState(RAIL_RF_STATE_IDLE, argv[0]))) {
    return;
  }

  bool appendedInfo = ciGetUnsigned(argv[1]);

  RAIL_RxConfig((RAIL_RX_CONFIG_FRAME_ERROR
                 | RAIL_RX_CONFIG_SYNC1_DETECT
                 | RAIL_RX_CONFIG_ADDRESS_FILTERED
                 | RAIL_RX_CONFIG_BUFFER_OVERFLOW
                 | RAIL_RX_CONFIG_SCHEDULED_RX_END
                 | RAIL_RX_CONFIG_PACKET_ABORTED),
                appendedInfo);
}

void rx(int argc, char **argv)
{
  bool enable = ciGetUnsigned(argv[1]);

  // Don't allow Rx enable calls when scheduled Rx has been started
  if ((currentAppMode() == RX_SCHEDULED) && enable) {
    responsePrintError(argv[0], 29, "Can't turn on receive when in ScheduledRx");
    return;
  }

  // Only allow calls to Rx if we're in direct mode, no mode, or this is a
  // scheduled Rx disable call
  if ((currentAppMode() != DIRECT)
      && (currentAppMode() != RX_SCHEDULED)
      && (!inAppMode(NONE, argv[0]))) {
    return;
  }

  // Make sure this transition is allowed
  if ((enable && (RAIL_RfStateGet() == RAIL_RF_STATE_RX))
      || (!enable && (RAIL_RfStateGet() == RAIL_RF_STATE_IDLE))) {
    // Do nothing since we're already in the right state
  } else if (enable && inRadioState(RAIL_RF_STATE_IDLE, argv[0])) {
    RAIL_RxStart(channel);
    receiveModeEnabled = enable;
  } else if (!enable) {
    RAIL_RfIdle();
    receiveModeEnabled = enable;

    // Turn off ScheduledRx if we were in it
    if (currentAppMode() == RX_SCHEDULED) {
      enableAppModeSync(RX_SCHEDULED, false, NULL);
    }
  }

  // Print out the current status of receive mode
  responsePrint(argv[0],
                "Rx:%s,Idle:%s,Time:%u",
                (enable ? "Enabled" : "Disabled"),
                ((!enable) ? "Enabled" : "Disabled"),
                RAIL_GetTime());
}

void rxAt(int argc, char **argv)
{
  uint32_t startTime, endTime;
  RAIL_TimeMode_t startMode, endMode;
  bool rxTransitionEndSchedule = false, hardEnd = false;
  bool scheduledRxUpdate = false;
  RAIL_ScheduleRxConfig_t rxCfg;

  // Only allow this call when in idle or to reconfigure an active scheduled Rx
  if ((!inAppMode(NONE, NULL) && !inAppMode(RX_SCHEDULED, NULL))
      || (inAppMode(NONE, NULL) && !inRadioState(RAIL_RF_STATE_IDLE, NULL))) {
    responsePrintError(argv[0], 30, "Cannot enter ScheduledRx when not in Idle.");
    return;
  }

  // Parse out the command line parameters
  startTime = ciGetUnsigned(argv[1]);
  endTime   = ciGetUnsigned(argv[3]);

  if (!parseTimeModeFromString(argv[2], &startMode)) {
    responsePrintError(argv[0], 25, "Invalid startMode specified");
    return;
  }
  if (!parseTimeModeFromString(argv[4], &endMode)) {
    responsePrintError(argv[0], 26, "Invalid endMode specified");
    return;
  }

  // Read the rxTransitionEndSchedule option if available
  if (argc > 5) {
    rxTransitionEndSchedule = !!ciGetUnsigned(argv[5]);
  }
  // Read the hardEnd option if available
  if (argc > 6) {
    hardEnd = !!ciGetUnsigned(argv[6]);
  }

  // Configure scheduled receive as requested
  rxCfg.start = startTime;
  rxCfg.startMode = startMode;
  rxCfg.end = endTime;
  rxCfg.endMode = endMode;
  rxCfg.rxTransitionEndSchedule = rxTransitionEndSchedule;
  rxCfg.hardWindowEnd = hardEnd;

  // Tell the app what we're doing so it can correctly track the end of the
  // scheduled receive state
  schRxStopOnRxEvent = rxTransitionEndSchedule;

  // Store whether this is an update or not
  if (inAppMode(RX_SCHEDULED, NULL)) {
    scheduledRxUpdate = true;
  }

  if (!scheduledRxUpdate) {
    // Attempt to put the app into scheduled receive mode
    if (!enableAppModeSync(RX_SCHEDULED, true, argv[0])) {
      return;
    }
  }

  // Enable scheduled receive mode
  uint8_t res = RAIL_ScheduleRx(channel, &rxCfg);
  if (res != RAIL_STATUS_NO_ERROR) {
    responsePrintError(argv[0], 27, "Could not start scheduled receive %d", res);
    if (!scheduledRxUpdate) {
      enableAppModeSync(RX_SCHEDULED, false, argv[0]);
    }
    return;
  }
}

void setRxOptions(int argc, char **argv)
{
  uint32_t rxOptions = ciGetUnsigned(argv[1]);
  RAIL_Status_t status = RAIL_SetRxOptions(rxOptions);

  if (status == RAIL_STATUS_NO_ERROR) {
    responsePrint(argv[0], "storeCrc:%s,ignoreCrcErrors:%s,enableDualSync:%s",
                  (rxOptions & RAIL_RX_OPTION_STORE_CRC) ? "True" : "False",
                  (rxOptions & RAIL_RX_OPTION_IGNORE_CRC_ERRORS) ? "True" : "False",
                  (rxOptions & RAIL_RX_OPTION_ENABLE_DUALSYNC) ? "True" : "False");
  } else {
    responsePrintError(argv[0], 31, "RxOptions:Failed");
  }
}

void setTxTone(int argc, char **argv)
{
  uint8_t enable = ciGetUnsigned(argv[1]);
  enableAppMode(TX_TONE, enable, argv[0]);
}

void setTxStream(int argc, char **argv)
{
  uint8_t enable = ciGetUnsigned(argv[1]);
  enableAppMode(TX_STREAM, enable, argv[0]);
}

void setDirectMode(int argc, char **argv)
{
  uint8_t enable = ciGetUnsigned(argv[1]);
  enableAppMode(DIRECT, enable, argv[0]);
}

void setDirectTx(int argc, char **argv)
{
  uint8_t enable = ciGetUnsigned(argv[1]);

  // Make sure that direct mode is enabled to do a direct Tx
  if (currentAppMode() != DIRECT) {
    responsePrintError(argv[0], 7, "DirectMode not enabled");
    return;
  }

  // Stop whatever we were doing so we can go into Tx
  RAIL_RfIdle();

  // Either enable or disable the transmitter
  if (enable) {
    // Turn on Tx
    RAIL_TxStart(channel, NULL, NULL);
  } else {
    // Wait for RxStart to succeed
    while (receiveModeEnabled && RAIL_RxStart(channel)) {
      RAIL_RfIdle();
    }
  }
  responsePrint(argv[0], "DirectTx:%s", (enable ? "Enabled" : "Disabled"));
}

#include <stdio.h>
#include "em_gpio.h"
#include "em_emu.h"
#include "em_core.h"
#include "retargetserial.h"

#ifndef DEBUG_SLEEP_LOOP
#define DEBUG_SLEEP_LOOP 0
#endif//DEBUG_SLEEP_LOOP

extern volatile bool serEvent;
static const char *rfBands[] = { "Off", "GHz", "MHz", "Any", };
static RAIL_RfSenseBand_t rfBand = RAIL_RFSENSE_OFF;
static uint32_t rfUs = 0;

void sleep(int argc, char **argv)
{
  if (!enableAppModeSync(RF_SENSE, true, argv[0])) {
    return;
  }

  uint8_t emMode = ciGetUnsigned(argv[1]);
  bool enableCb = (emMode & 0x80) ? false : true;
  emMode &= ~0x80;

  if (emMode <= 4) {
    char* em4State = "";
    if (emMode == 4) {
      switch (argv[1][1]) {
        case 's': case 'S': // Configure EM4 Shutoff state
          CORE_CRITICAL_SECTION(
            EMU->EM4CTRL = (EMU->EM4CTRL & ~EMU_EM4CTRL_EM4STATE)
                           | _EMU_EM4CTRL_EM4STATE_EM4S;
            em4State = "s";
            );
          break;

        case 'h': case 'H': // Configure EM4 Hibernate state
          CORE_CRITICAL_SECTION(
            EMU->EM4CTRL = (EMU->EM4CTRL & ~EMU_EM4CTRL_EM4STATE)
                           | _EMU_EM4CTRL_EM4STATE_EM4H;
            em4State = "h";
            );
          break;

        default:
          // Use whatever prior EM4 state was specified
          em4State = (((EMU->EM4CTRL & EMU_EM4CTRL_EM4STATE)
                       == EMU_EM4CTRL_EM4STATE_EM4S) ? "s" : "h");
          break;
      }
    }

    // sleep is MODAL -- we'll block here in foreground.
    // If there are new RfSense parameters, grab 'em:
    if (argc > 2) {
      rfUs = ciGetUnsigned(argv[2]);
      if (argc > 3) {
        rfBand = (RAIL_RfSenseBand_t) ciGetUnsigned(argv[3]);
      }
    }

    // Shut down radio packet reception and Peripherals for EM2+ sleep
    if (emMode >= 2) {
      RAIL_RfIdle();
      PeripheralDisable();
      GPIO_EM4SetPinRetention(true);
    }

    // We cannot configure UART RxD for EM4 wakeup on our EFR32's so the
    // *only* wakeup possible out of EM4 is RFsense (or reset).
    responsePrint(argv[0], "EM:%u%s,SerialWakeup:%s,RfSense:%s",
                  emMode, em4State,
                  (emMode < 4) ? "On" : "Off",
                  rfBands[rfBand & 0x3]);
    serialWaitForTxIdle();

    // Disable interrupts heading into RAIL_RfSense() so we don't miss
    // the event occurring before we try to sleep.
    CORE_DECLARE_IRQ_STATE;
    CORE_ENTER_CRITICAL();

    rfUs = RAIL_RfSense(rfBand, rfUs, enableCb);

    // Configure the USART Rx pin as a GPIO interrupt for sleep-wake purposes,
    // falling-edge only
    GPIO_IntConfig(RETARGET_RXPORT, RETARGET_RXPIN, false, true, true);

    serEvent = false;

    bool rfSensed;
   #if     DEBUG_SLEEP_LOOP
    uint64_t sleeps = 0;
    typedef struct wakeReasons{
      uint32_t scbInts;
      uint32_t nvicInts[2];
      uint32_t gpioInts;
    } wakeReasons_t;
    // Maintain the most recent two wakeup events in 'circular' list
    wakeReasons_t wakeReasons[2] = { { 0, 0, }, { 0, 0, }, };
   #endif//DEBUG_SLEEP_LOOP

    do { // Loop modally here until either RfSense or Serial event occurs
         //@TODO Should we WDOG_Feed()??
      switch (emMode) {
        case 0:                      break;
        case 1:  EMU_EnterEM1(    ); break;
        case 2:  EMU_EnterEM2(true); break;
        case 3:  EMU_EnterEM3(true); break;
        case 4:  EMU_EnterEM4(    ); break; // NOTREACHED
        default:                     break;
      }
      // After waking, interrupts need to be re-enabled so ISRs can run
     #if     DEBUG_SLEEP_LOOP
      // But grab why we woke first (of course this is meaningless for EM0):
      sleeps++;
      wakeReasons[sleeps & 1].gpioInts = GPIO_IntGetEnabled();
      // No NVIC_ API for this
      wakeReasons[sleeps & 1].nvicInts[0] = NVIC->ISPR[0] /*& NVIC->ISER[0]*/;
      wakeReasons[sleeps & 1].nvicInts[1] = NVIC->ISPR[1] /*& NVIC->ISER[1]*/;
      wakeReasons[sleeps & 1].scbInts = SCB->ICSR;
     #endif//DEBUG_SLEEP_LOOP
      CORE_EXIT_CRITICAL(); // Briefly enable IRQs to let them run
      CORE_ENTER_CRITICAL(); // but shut back off in case we loop
      rfSensed = RAIL_RfSensed();
    } while (!rfSensed && !serEvent);
    CORE_EXIT_CRITICAL(); // Back on permanently

    // Here we've awoken for at least one of the desired events.
    // If we woke on serial, don't need RFSense active any more.
    enableAppMode(RF_SENSE, false, NULL);

    // Disable serial interrupt so it's not bothersome
    GPIO_IntConfig(RETARGET_RXPORT, RETARGET_RXPIN, false, true, false);

    responsePrint("sleepWoke",
                  "EM:%u%s,"
                  "SerialWakeup:%s,"
                  "RfSensed:%s,"
                  "RfUs:%u"
                 #if     DEBUG_SLEEP_LOOP
                  ",\nSleeps:%llu,"
                  "\nScb[-]:0x%08x,"
                  "Nvic[-]:0x%x.%08x,Gpios[-]:0x%08x,"
                  "\nScb[0]:0x%08x,"
                  "Nvic[0]:0x%x.%08x,Gpios[0]:0x%08x"
                 #endif//DEBUG_SLEEP_LOOP
                  , emMode,
                  em4State,
                  serEvent ? "Yes" : "No",
                  rfSensed ? "Yes" : "No",
                  rfUs
                 #if     DEBUG_SLEEP_LOOP
                  , sleeps,
                  wakeReasons[(sleeps - 1) & 1].scbInts,
                  wakeReasons[(sleeps - 1) & 1].nvicInts[1],
                  wakeReasons[(sleeps - 1) & 1].nvicInts[0],
                  wakeReasons[(sleeps - 1) & 1].gpioInts,
                  wakeReasons[(sleeps) & 1].scbInts,
                  wakeReasons[(sleeps) & 1].nvicInts[1],
                  wakeReasons[(sleeps) & 1].nvicInts[0],
                  wakeReasons[(sleeps) & 1].gpioInts
                 #endif//DEBUG_SLEEP_LOOP
                  );

    if (serEvent) { // Consume the character entered
      if (emMode >= 2) {
        // If UART was shut down, delay and gobble likely junk
        usDelay(250000); // Pause for 250 ms
        (void) getchar();
        (void) getchar();
        (void) getchar();
        (void) getchar();
      } else {
        while (getchar() < 0) {
        }                         // Gobble the character entered
      }
    }

    // Restart Rx if we're in Rx mode
    while (receiveModeEnabled && RAIL_RxStart(channel)) {
      RAIL_RfIdle();
    }
    // Restart peripherals if they were active before sleeping
    if ((emMode >= 2) && (logLevel & PERIPHERAL_ENABLE)) {
      PeripheralEnable();
    }
  } else {
    responsePrintError(argv[0], 1, "Invalid EM mode %u (valid 0-4)", emMode);
  }
}

void rfSense(int argc, char **argv)
{
  // If there are RfSense parameters, grab 'em; otherwise use prior settings
  if (argc > 1) {
    rfUs = ciGetUnsigned(argv[1]);
    if (argc > 2) {
      rfBand = (RAIL_RfSenseBand_t) ciGetUnsigned(argv[2]);
    }
  }

  if (!enableAppModeSync(RF_SENSE, rfBand != 0, argv[0])) {
    return;
  }

  rfUs = RAIL_RfSense(rfBand, rfUs, true); // Need the callback for counting
  if (rfUs == 0) { // error
    rfBand = RAIL_RFSENSE_OFF;
  }
  responsePrint(argv[0], "RfSense:%s,RfUs:%u",
                rfBands[rfBand & 0x3], rfUs);
}

void rfSensedCheck(void)
{
  // If in RfSensing background mode, see if Rf got sensed
  if ((currentAppMode() == RF_SENSE) && RAIL_RfSensed()) {
    // Yes, it did!  Terminate RfSensing background mode
    enableAppMode(RF_SENSE, false, NULL);
    responsePrint("rfSensedCheck", "RfSensed:%s,RfUs:%u",
                  rfBands[rfBand & 0x3], rfUs);
  }
}

/**
 * Routine to parse a RAIL timer mode from a string.
 *
 * @param str The string to parse. Must be one of the following:
 * - 'rel': RAIL_TIME_DELAY
 * - 'abs': RAIL_TIME_ABSOLUTE
 * - 'dis': RAIL_TIME_DISABLED
 * @param mode The RAIL_TimeMode_t variable to fill in with the result of this
 * conversion.
 * @return Returns true if we were able to successfully convert the string into
 * a time mode and false if we could not.
 */
bool parseTimeModeFromString(char *str, RAIL_TimeMode_t *mode)
{
  if (strcasecmp("dis", str) == 0) {
    *mode = RAIL_TIME_DISABLED;
  } else if (strcasecmp("rel", str) == 0) {
    *mode = RAIL_TIME_DELAY;
  } else if (strcasecmp("abs", str) == 0) {
    *mode = RAIL_TIME_ABSOLUTE;
  } else {
    *mode = RAIL_TIME_DISABLED;
    return false;
  }
  return true;
}
