/**
 * @file event.h
 * @brief Scheduling events for future execution.
 * See @ref event for documentation.
 *
 * <!--Copyright 2014 Silicon Laboratories, Inc.                         *80*-->
 */

#if defined(EZSP_HOST)
  #define EMBER_NO_IDLE_SUPPORT
#endif

/**
 * @addtogroup event
 *
 * These macros implement an event abstraction that allows the application
 * to schedule code to run after some specified time interval.  An event
 * consists of a procedure to be called at some point in the future and
 * a control object that determines the procedure should be called.  Events
 * are also useful for when an ISR needs to initiate an action that should
 * run outside of ISR context.
 *
 * See event.h for source code.
 *
 * Note that while not required, it is recommended that the event-handling
 * procedure explicitly define the recurrence of the next event, either by
 * rescheduling it via some kind of @e emberEventControlSetDelayXX() call or by
 * deactivating it via a call to ::emberEventControlSetInactive().
 * In cases where the handler does not explicitly reschedule or cancel the
 * event, the default behavior of the event control system is to keep the
 * event immediately active as if the handler function had called
 * ::emberEventControlSetActive(someEvent) or
 * ::emberEventControlSetDelayMS(someEvent, 0)
 *
 * The base time units for events are ticks.  Each tick is approximately equal
 * to a millisecond, but the true duration depends on the platform.  The
 * duration of a tick is 1000 / ::MILLISECOND_TICKS_PER_SECOND, where 1000 is
 * the number of milliseconds per second and ::MILLISECOND_TICKS_PER_SECOND is
 * the platform-specific number of ticks per second.  For example,
 * ::MILLISECOND_TICKS_PER_SECOND on the EM357 SoC is 1024, so each tick is
 * therefore 1000 / 1024 = ~0.98 milliseconds.  Calling
 * ::emberEventControlSetDelayMS(someEvent, 100) on the EM357 SoC will schedule
 * the event for 100 ticks * (1000 milliseconds / 1024 ticks) =
 * ~97.7 milliseconds.  Note however that the accuracy of the base tick depends
 * on your timer source.  Further, the scheduled delay is the minimum delay.
 * If ::emberRunEvents or ::emberRunTask are not called frequently enough, the
 * actual delay may be longer than the scheduled delay.
 *
 * Additionally, the APIs for quarter second and minute delays
 * (::emberEventControlSetDelayQS and ::emberEventControlSetDelayMinutes) use
 * "binary" units.  One quarter second is 256 ticks and one minute is 65536
 * ticks.  Calling ::emberEventControlSetDelayMinutes(someEvent, 3) on the
 * EM357 SoC will schedule the event for 3 minutes * (65536 ticks / minute) *
 * (1000 milliseconds / 1024 ticks) = ~3.2 minutes.  It is possible to avoid
 * these binary units by using ::emberEventControlSetDelayMS and the various
 * MILLISECOND_TICKS_PER_XXX multipliers.  For example, calling
 * ::emberEventControlSetDelayMS(someEvent, 3 * MILLISECOND_TICKS_PER_MINUTE)
 * will delay for 3 minutes on any platform.  Be aware of
 * ::EMBER_MAX_EVENT_CONTROL_DELAY_MS when using this approach.
 *
 * Following are some brief usage examples.
 * @code
 * EmberEventControl delayEvent;
 * EmberEventControl signalEvent;
 * EmberEventControl periodicEvent;
 *
 * void delayEventHandler(void)
 * {
 *   // Disable this event until its next use.
 *   emberEventControlSetInactive(delayEvent);
 * }
 *
 * void signalEventHandler(void)
 * {
 *   // Disable this event until its next use.
 *   emberEventControlSetInactive(signalEvent);
 *
 *   // Sometimes we need to do something 100 ms later.
 *   if (somethingIsExpected)
 *     emberEventControlSetDelayMS(delayEvent, 100);
 * }
 *
 * void periodicEventHandler(void)
 * {
 *   emberEventControlSetDelayQS(periodicEvent, 4);
 * }
 *
 * void someIsr(void)
 * {
 *   // Set the signal event to run at the first opportunity.
 *   emberEventControlSetActive(signalEvent);
 * }
 *
 * // Put the controls and handlers in an array.  They will be run in
 * // this order.
 * EmberEventData events[] =
 *  {
 *    { &delayEvent,    delayEventHandler },
 *    { &signalEvent,   signalEentHandler },
 *    { &periodicEvent, periodicEventHandler },
 *    { NULL, NULL }                            // terminator
 *  };
 *
 * void main(void)
 * {
 *   // Cause the periodic event to occur once a second.
 *   emberEventControlSetDelayQS(periodicEvent, 4);
 *
 *   while (TRUE) {
 *     emberRunEvents(events);
 *   }
 * }
 * @endcode
 *
 * @{
 */

// Controlling events

// Possible event status values.  Having zero as the 'inactive' value
// causes events to initially be inactive.
//
#ifndef __EVENT_H__
#define __EVENT_H__

/** @brief Sets this ::EmberEventControl as inactive (no pending event).
 */
#define emberEventControlSetInactive(control) \
  do { (control).status = EMBER_EVENT_INACTIVE; } while (0)

/** @brief Returns TRUE if the event is active, FALSE otherwise.
 */
#define emberEventControlGetActive(control) \
  ((control).status != EMBER_EVENT_INACTIVE)

/** @brief Sets this ::EmberEventControl to run at the next available
    opportunity.
 */
#define emberEventControlSetActive(control) \
  do { emEventControlSetActive(&(control)); } while (0)

/** @brief Sets this ::EmberEventControl to run at the next available
    opportunity.
 */
void emEventControlSetActive(EmberEventControl *event);

/** @brief The maximum delay that may be passed to
 * ::emberEventControlSetDelayMS.
 */
#define EMBER_MAX_EVENT_CONTROL_DELAY_MS (HALF_MAX_INT32U_VALUE - 1)

/** @brief Sets this ::EmberEventControl to run "delay" milliseconds in the
 *  future.
 *  NOTE: To avoid rollover errors in event calculation, the delay must be
 *  less than ::EMBER_MAX_EVENT_CONTROL_DELAY_MS.
 */
#define emberEventControlSetDelayMS(control, delay) \
  do { emEventControlSetDelayMS(&(control), (delay)); } while (0)

/** @brief Sets this ::EmberEventControl to run "delay" milliseconds in the
 *  future.
 *  NOTE: To avoid rollover errors in event calculation, the delay must be
 *  less than ::EMBER_MAX_EVENT_CONTROL_DELAY_MS.
 */
void emEventControlSetDelayMS(EmberEventControl*event, uint32_t delay);

/** @brief The maximum delay that may be passed to
 * ::emberEventControlSetDelayQS.
 */
#define EMBER_MAX_EVENT_CONTROL_DELAY_QS (EMBER_MAX_EVENT_CONTROL_DELAY_MS >> 8)

/** @brief Sets this ::EmberEventControl to run "delay" quarter seconds
 *  in the future. The 'quarter seconds' are actually 256 milliseconds long.
 *  NOTE: To avoid rollover errors in event calculation, the delay must be
 *  less than ::EMBER_MAX_EVENT_CONTROL_DELAY_QS.
 */
#define emberEventControlSetDelayQS(control, delay) \
  do { emEventControlSetDelayMS(&(control), (delay) << 8); } while (0)

/** @brief The maximum delay that may be passed to
 * ::emberEventControlSetDelayMinutes.
 */
#define EMBER_MAX_EVENT_CONTROL_DELAY_MINUTES (EMBER_MAX_EVENT_CONTROL_DELAY_MS >> 16)

/** @brief Sets this ::EmberEventControl to run "delay" minutes in the future.
 *  The 'minutes' are actually 65536 (0x10000) milliseconds long.
 *  NOTE: To avoid rollover errors in event calculation, the delay must be
 *  less than ::EMBER_MAX_EVENT_CONTROL_DELAY_MINUTES.
 */
#define emberEventControlSetDelayMinutes(control, delay) \
  do { emEventControlSetDelayMS(&(control), (delay) << 16); } while (0)

/** @brief Returns The amount of milliseconds remaining before the event is
 *  scheduled to run.  If the event is inactive, MAX_INT32U_VALUE is returned.
 */
#define emberEventControlGetRemainingMS(control) \
  (emEventControlGetRemainingMS(&(control)))

/** @brief Returns The amount of milliseconds remaining before the event is
 *  scheduled to run.  If the event is inactive, MAX_INT32U_VALUE is returned.
 */
uint32_t emEventControlGetRemainingMS(EmberEventControl *event);

// Running events

/** @brief An application typically creates an array of events
 * along with their handlers.
 *
 * The main loop passes the array to ::emberRunEvents() in order to call
 * the handlers of any events whose time has arrived.
 */
void emberRunEvents(EmberEventData *events);

/** @brief If an application has initialized a task via emberTaskInit, to
 *  run the events associated with that task, it should could ::emberRunTask()
 *  instead of ::emberRunEvents().
 */
void emberRunTask(EmberTaskId taskid);

/** @brief Returns the number of milliseconds before the next event
 *  is scheduled to expire, or maxMs if no event is scheduled to expire
 *  within that time.
 *  NOTE: If any events are modified within an interrupt, in order to guarantee
 *  the accuracy of this API, it must be called with interrupts disabled or
 *  from within an ATOMIC() block.
 */
uint32_t emberMsToNextEvent(EmberEventData *events, uint32_t maxMs);

/** @brief This function does the same as emberMsToNextEvent() with the
 *  following addition.  If the returnIndex is non-NULL, it will set the value
 *  pointed to by the pointer to be equal to the index of the event that is
 *  ready to fire next.  If no events are active, then it returns 0xFF.
 */
uint32_t emberMsToNextEventExtended(EmberEventData *events, uint32_t maxMs, uint8_t* returnIndex);

/** @brief Returns the number of milliseconds before the next stack event is
 * scheduled to expire.
 */
uint32_t emberMsToNextStackEvent(void);

/** @brief Initializes a task to be used for managing events and processor
 *  idling state.
 *  Returns the ::EmberTaskId which represents the newly created task
 */
EmberTaskId emberTaskInit(EmberEventData *events);

/** @brief Indicates that a task has nothing to do (unless any events are
 *   pending) and that it would be safe to idle the CPU if all other tasks
 *   also have nothing to do.
 *  This API should always be called with interrupts disabled.  It will forcibly
 *   re-enable interrupts before returning
 *  Returns TRUE if the processor was idled, FALSE if idling wasn't permitted
 *   because some other task has something to do.
 */
bool emberMarkTaskIdle(EmberTaskId taskid);

#ifndef EMBER_NO_IDLE_SUPPORT
/** @brief Call this to indicate that an application supports processor idling
 */
  #define emberTaskEnableIdling(allow) \
  do { emTaskEnableIdling((allow)); } while (0)

void emTaskEnableIdling(bool allow);

/** @brief Indicates that a task has something to do, so the CPU should not be
 *   idled until emberMarkTaskIdle is next called on this task.
 */
  #define emberMarkTaskActive(taskid) \
  do { emMarkTaskActive((taskid)); } while (0)

void emMarkTaskActive(EmberTaskId taskid);
#else
  #define emberTaskEnableIdling(allow)  do {} while (0)
  #define emberMarkTaskActive(taskid)   do {} while (0)
#endif // EMBER_NO_IDLE_SUPPORT

#endif // __EVENT_H__

// @} END addtogroup
