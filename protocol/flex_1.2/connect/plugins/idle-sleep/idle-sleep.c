// Copyright 2015 Silicon Laboratories, Inc.

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "serial/serial.h"
#include "flex-callbacks.h"

#ifdef EMBER_AF_PLUGIN_FREE_RTOS
#include "FreeRTOS.h"
#include "task.h"
#include EMBER_AF_API_FREE_RTOS
#include "rtcdriver.h"
#endif

extern const EmberEventData emAppEvents[];
extern EmberTaskId emAppTask;

#ifdef EMBER_AF_PLUGIN_FREE_RTOS
extern RtosSemaphore_t idle_sleep_sem;
#endif

// In testing, we cannot sleep or idle if the simulator is transmitting on any
// of the serial ports.
#ifdef EMBER_TEST
extern bool doingSerialTx[];
  #define simulatorDoingSerialTx(port) doingSerialTx[port]
#else
  #define simulatorDoingSerialTx(port) false
#endif

static bool okToIdleOrSleep(void)
{
#ifdef EMBER_AF_PLUGIN_SERIAL
#ifdef EM_NUM_SERIAL_PORTS
  // With numbered serial ports, if any port has pending incoming or outgoing
  // data or if the simulator is transmitting, we do not sleep.
  uint8_t i;
  for (i = 0; i < EM_NUM_SERIAL_PORTS; i++) {
    if (!emberSerialUnused(i)
        && (emberSerialReadAvailable(i) != 0
            || emberSerialWriteUsed(i) != 0
            || simulatorDoingSerialTx(i))) {
      return false;
    }
  }
#else // EM_NUM_SERIAL_PORTS
  // Without numbered ports, if there is pending incoming or outgoing data, we
  // do not sleep.
  if (!emberSerialUnused(APP_SERIAL)
      && (emberSerialReadAvailable(APP_SERIAL) != 0
          || emberSerialWriteUsed(APP_SERIAL) != 0
          || simulatorDoingSerialTx(APP_SERIAL))) {
    return false;
  }
#endif // EM_NUM_SERIAL_PORTS
#endif // EMBER_AF_PLUGIN_SERIAL
  return true;
}

#ifdef EMBER_AF_PLUGIN_FREE_RTOS
long stack_blocked_deep_sleep;
long stack_allowed_deep_sleep;
long too_long_sleep;

// Implement a rtos idle handler so that idle can work.
bool rtosIdleHandler(uint32_t * msToNextEvent, bool deepOk)
{
  uint32_t durationMs = *msToNextEvent;
  uint32_t startTicks = RTCDRV_GetWallClockTicks32();

  if ( !deepOk ) {
    stack_blocked_deep_sleep++;
    rtosBlockDeepSleep();
  } else {
    stack_allowed_deep_sleep++;
  }

  INTERRUPTS_ON();
  // We do not care if we get the semaphore or not.
  // Just used to wait for durationMs and can
  // be shortened if needed.
  rtosSemaphoreTake(idle_sleep_sem, durationMs);

  if ( !deepOk ) {
    if (rtosGetDeepSleepBlockCount() > 0) {
      rtosUnblockDeepSleep();
    }
  }

  uint32_t endTicks = RTCDRV_GetWallClockTicks32();
  uint32_t deltaMs = RTCDRV_TicksToMsec(endTicks - startTicks);
  if ( deltaMs <= durationMs ) {
    *msToNextEvent = durationMs - deltaMs;
  } else {
    *msToNextEvent = 0;
    too_long_sleep++;
  }

  return true;
}
#else
bool rtosIdleHandler(uint32_t * msToNextEvent, bool deepOk);  // Use provided one
#endif

void emberAfPluginIdleSleepTickCallback(void)
{
  // Turn interrupts off so that we can safely determine whether we can sleep
  // or idle.  Both halSleepForMilliseconds and emberMarkTaskIdle forcibly
  // enable interrupts.  We can call neither function or exactly one of them
  // without risking a race condition.  Note that if we don't call either, we
  // have to enable interrupts ourselves.
  INTERRUPTS_OFF();

  // We can idle or sleep if some basic conditions are satisfied.  If not, we
  // stay awake and active.  Otherwise, we try to sleep or idle, in that order.
  if (okToIdleOrSleep()) {
    // If the stack says we can nap, it means that we may sleep for some amount
    // of time.  Otherwise, we can't sleep at all, although we can try to idle.
    if (emberOkToNap()) {
      // If the stack says we can hiberate, it means we can sleep as long as we
      // want.  Otherwise, we cannot sleep longer than the duration to its next
      // event.  In either case, we will never sleep longer than the duration
      // to our next event.
      uint32_t durationMs = (emberOkToHibernate()
                             ? MAX_INT32U_VALUE
                             : emberMsToNextStackEvent());
      durationMs = emberMsToNextEvent(emAppEvents, durationMs);

      // If the sleep duration is below our minimum threshold, we don't bother
      // sleeping.  It takes time to shut everything down and bring everything
      // back up and, at some point, it becomes useless to sleep.  We also give
      // the application one last chance to stay awake if it wants.  If the
      // duration is long enough and the application says okay, we will sleep.
      // Otherwise, we will try to idle instead.
      if (EMBER_AF_PLUGIN_IDLE_SLEEP_MINIMUM_SLEEP_DURATION_MS <= durationMs
          && emberAfPluginIdleSleepOkToSleepCallback(durationMs)) {
        uint32_t attemptedDurationMs = durationMs;
        emberStackPowerDown();
        // WARNING: The following function enables interrupts, so
        // we will cease to be atomic by the time it returns.
        // One of rtosIdleHandler() or halSleepForMilliseconds()
        // will enable interrupts.
        if ( !rtosIdleHandler(&durationMs, true) ) {
          halPowerDown();
          halSleepForMilliseconds(&durationMs);
          halPowerUp();
        }
        emberStackPowerUp();
        emberAfPluginIdleSleepWakeUpCallback(attemptedDurationMs - durationMs);
        return;
      }
    }

#ifndef EMBER_NO_IDLE_SUPPORT
    // If we are here, it means we could not sleep, so we will try to idle
    // instead.  We give the application one last chance to stay active if it
    // wants.  Note that interrupts are still disabled at this point.
    if (emberAfPluginIdleSleepOkToIdleCallback()) {
      // We are going to try to idle.  Setting our task to idle will trigger a
      // check of every other task.  If the others are also idle, the device
      // will idle.  If not, it won't.  In either case, we set our task back to
      // active again.  This ensures that we are in control of idling and will
      // prevent the device from idling without our knowledge.
      // WARNING: The following function enables interrupts, so we will cease
      // to be atomic by the time it returns.
      bool idled = emberMarkTaskIdle(emAppTask);
      emberMarkTaskActive(emAppTask);
      if (idled) {
        emberAfPluginIdleSleepActiveCallback();
      }
      return;
    }
#endif // !EMBER_NO_IDLE_SUPPORT
  } // emAfOkToIdleOrSleep

  // If we are here, it means we did not sleep or idle.  Interrupts are still
  // disabled at this point, so we have to enable them again.
  INTERRUPTS_ON();
}
