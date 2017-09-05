/*
 * File: event-queue.h
 * Description: schedulable events
 *
 * Copyright 2013 Silicon Laboratories, Inc.                                *80*
 */

/**
 * @addtogroup event
 *
 * @{

 * An event queue is a queue of events that will be run at some future time(s).
 * The Event type is declared in core/ember-stack.h and has three fields:
 *
   @verbatim
   EventActions *actions;      // see below
   Event *next;                // internal use only, but must be
                             //  initialized to NULL
   uint32_t timeToExecute;       // internal use only
   @endverbatim
 *
 * The EventAction struct also has three fields:
 *
   @verbatim
   struct EventQueue_s *queue; // the queue this event goes on
   void (*handler)(Event *);   // called when the event fires
   void (*marker)(Event *);    // marking fuction, can be NULL
   @endverbatim
 *
 * The marker function is needed if an event refers to a buffer or is
 * itself a buffer; see the dynamic event example below.
 *
 *
 * Sample static event declaration:
 *
   @verbatim
   static void fooEventHandler(Event *event);
   static EventActions fooEventActions = {
   &emStackEventQueue,
   fooEventHandler,
   NULL          // no marking function is needed
   };

   Initialize the event as unscheduled.
   static Event fooEvent = { &fooEventActions, NULL };

   static void fooEventHandler(Event *event)
   {
   ...
   }
   @endverbatim
 *
 * Sample dynamic event:
   @verbatim

   typedef struct {
   Event event;
   PacketHeader someBuffer;
   // plus other values as needed
   } BarEvent;

   static void barEventHandler(BarEvent *event);
   static void barEventMarker(BarEvent *event);

   static EventActions barEventActions = {
   &emStackEventQueue,
   (void (*)(Event *)) barEventHandler,
   (void (*)(Event *)) barEventMarker
   };

   static void barEventHandler(BarEvent *event)
   {
   ...
   };

   static void barEventMarker(BarEvent *event)
   {
   emMarkBuffer(&event->someBuffer);
   }

   static void addBarEvent(Buffer someBuffer, uint32_t delayMs)
   {
   Buffer eventBuffer = emAllocateBuffer(sizeof(BarEvent));
   if (eventBuffer == NULL_BUFFER) {
     return;
   }
   BarEvent *barEvent = (BarEvent *) emGetBufferPointer(eventBuffer);
   barEvent->event.actions = &barEventActions;
   barEvent->event.next = NULL;       // initialize as unscheduled
   barEvent->someBuffer = someBuffer;
   emberEventSetDelayMs((Event *) barEvent, delayMs);
   }
   @endverbatim

 * Cancelling a dynamic event:
   @verbatim

   static bool barEventPredicate(Event *event,
                                  void *castBuffer)
   {
   return (((BarEvent *) event)->someBuffer) == (Buffer) castBuffer;
   }

   static void cancelBarEvent(Buffer someBuffer)
   {
   emberFindEvent(&emStackEventQueue,
                  &barEventActions,
                  barEventPredicate,
                  (void *) someBuffer);
   }
   @endverbatim
 * Delays greater than this will cause the scheduled execution time to
 * wrap around into the past and the event will be executed immediately.
 *
 * Event Queue operations
 */

#define EMBER_MAX_EVENT_DELAY_MS (HALF_MAX_INT32U_VALUE - 1)

/**
 * Marker function for ISR event types.  Events that are scheduled from
 * ISR context must use this as their marker function (and must not
 * required an actual marker function.
 *
 * Events with this in their type may only be scheduled with a zero delay.
 */
void emIsrEventMarker(struct Event_s *);

/**
 * Initialize 'queue'.  This must be called before any use of 'queue'
 * or any of its events.
 */
void emInitializeEventQueue(EventQueue *queue);

/**
 * Unschedule all the events.
 */
void emberCancelAllEvents(EventQueue *queue);

/**
 * Return the number of milliseconds until the next event fires, or -1 if
 * there are no scheduled events.
 */
uint32_t emberMsToNextQueueEvent(EventQueue *queue);

/**
 * Run any and all events which have fired since the last time this queue's
 * events were run.
 */
void emberRunEventQueue(EventQueue *queue);

/**
 * If any scheduled events have mark functions, mark those events, if
 * they are buffers, and call the mark functions.
 */
void emberMarkEventQueue(EventQueue *queue);

/**
 * Returns the first scheduled event that has 'actions' and for which
 * 'predicate' returns true.  The returned event has been cancelled.
 */
#define emberFindEvent(queue, actions, predicate, data) \
  (emFindEvents((queue), (actions), (predicate), (data), false))

/**
 * Returns the scheduled events that have 'actions' and for which
 * 'predicate' returns true.  If 'predicate' is NULL then all events
 * that have 'actions' are returned.  The returned events are linked
 * through the 'next' field and have been cancelled.
 *
 * WARNING: the 'next' field of the returned events must be set to
 * NULL before the event is passed to any event operation.
 */

#define emberFindAllEvents(queue, actions, predicate, data) \
  (emFindEvents((queue), (actions), (predicate), (data), true))

typedef bool (*EventPredicate)(Event *, void *);

Event *emFindEvents(EventQueue *queue,
                    EventActions *actions,
                    EventPredicate predicate,
                    void *data,
                    bool all);

/**
 * Event operations
 */

/**
 * Schedule 'event' to run after 'delay' milliseconds.  Delays greater than
 * EMBER_MAX_EVENT_DELAY_MS will be reduced to EMBER_MAX_EVENT_DELAY_MS.
 */
void emberEventSetDelayMs(Event *event, uint32_t delay);

/**
 * Schedule 'event' to run immediately.
 */
#define emberEventSetActive(event) emberEventSetDelayMs((event), 0)

/**
 * Cancel 'event'.
 */
void emberEventSetInactive(Event *event);

/**
 * Return true if the event is scheduled to run.
 */
bool emberEventIsScheduled(Event *event);

/**
 * Return the number of milliseconds before 'event' runs, or -1 if 'event'
 * is not scheduled to run.
 */
uint32_t emberEventGetRemainingMs(Event *event);

/**
 * @}
 */
