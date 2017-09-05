/**
 * @file stack/include/ember-types.h
 * @brief Ember Connect data type definitions.
 *
 * <!--Copyright 2014 by Silicon Laboratories. All rights reserved.      *80*-->
 */

/**
 * @addtogroup ember_types
 *
 * See ember-types.h for source code.
 * @{
 */

#ifndef __EMBER_TYPES_H__
#define __EMBER_TYPES_H__

#include <stdint.h>

#ifndef DOXYGEN_SHOULD_SKIP_THIS
#include "stack/config/ember-configuration-defaults.h"
#endif //DOXYGEN_SHOULD_SKIP_THIS

/**
 * @brief Size of an extended PAN identifier in bytes (8).
 */
#define EXTENDED_PAN_ID_SIZE 8

/**
 * @brief Size of EUI64 (an IEEE address) in bytes (8).
 */
#define EUI64_SIZE 8

/**
 * @brief Size of an encryption key in bytes (16).
 */
#define EMBER_ENCRYPTION_KEY_SIZE 16

/**
 * @brief EUI 64-bit ID (an IEEE address).
 */
typedef uint8_t EmberEUI64[EUI64_SIZE];

/**
 * @brief 802.15.4 node ID.
 */
typedef uint16_t EmberNodeId;

/**
 * @brief 802.15.4 PAN ID.
 */
typedef uint16_t EmberPanId;

#ifdef PHY_EM3XX
/**
 * @brief The maximum 802.15.4 channel number is 26.
 */
#define EMBER_MAX_802_15_4_CHANNEL_NUMBER 26

/**
 * @brief The minimum 802.15.4 channel number is 11.
 */
#define EMBER_MIN_802_15_4_CHANNEL_NUMBER 11

/**
 * @brief There are sixteen 802.15.4 channels.
 */
#define EMBER_NUM_802_15_4_CHANNELS \
  (EMBER_MAX_802_15_4_CHANNEL_NUMBER - EMBER_MIN_802_15_4_CHANNEL_NUMBER + 1)

/**
 * @brief Bitmask to scan all 802.15.4 channels.
 */
#define EMBER_ALL_802_15_4_CHANNELS_MASK 0x07FFF800UL
#endif

/**
 * @brief A distinguished network ID that will never be assigned
 * to any node.  Used to indicate the absence of a node ID.
 */
#define EMBER_NULL_NODE_ID                0xFFFF

/** Broadcast address. */
#define EMBER_BROADCAST_ADDRESS           0xFFFF

/** The coordinator short address. */
#define EMBER_COORDINATOR_ADDRESS         0x0000

/**
 * @brief Defines the possible types of nodes and the roles that a
 * node might play in a network.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberNodeType
#else
typedef uint8_t EmberNodeType;
enum
#endif
{
  /** Device is not joined. */
  EMBER_UNKNOWN_DEVICE = 0,
  /** Star or extended star topology device: will relay messages and can act as
   * a parent to range extender and  end device nodes.
   */
  EMBER_STAR_COORDINATOR = 1,
  /** Star or extended star topology device: will relay messages and can act as
   * a parent to end device nodes.
   */
  EMBER_STAR_RANGE_EXTENDER = 2,
  /** Star or extended star topology device: communicates only with its parent
   * and will not relay messages.
   */
  EMBER_STAR_END_DEVICE = 3,
  /** Star or extended star topology device: an end device whose radio can be
   * turned off to save power. The application must call ::emberPollForData() to
   * receive messages.
   */
  EMBER_STAR_SLEEPY_END_DEVICE = 4,
  /** A device able to send and receive messages from other devices in range on
   * the same PAN ID, with no star topology restrictions. Such device does not
   * relay messages. A node can be started as a direct device by using the
   * ::emberJoinCommissioned() API.
   */
  EMBER_DIRECT_DEVICE = 5,
  /** A device able to send and receive MAC-level messages. A node can be
   * started as a MAC mode device by using the ::emberJoinCommissioned() API.
   */
  EMBER_MAC_MODE_DEVICE = 6,
};

/**
 * @brief Defines the possible join states for a node.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberNetworkStatus
#else
typedef uint8_t EmberNetworkStatus;
enum
#endif
{
  /** The node is not associated with a network in any way. */
  EMBER_NO_NETWORK,
  /** The node is currently attempting to join a network. */
  EMBER_JOINING_NETWORK,
  /** The node is joined to a network. */
  EMBER_JOINED_NETWORK,
};

/** @brief Holds network parameters.
 *
 * For information about power settings and radio channels,
 * see the technical specification for the
 * RF communication module in your Developer Kit.
 */
typedef struct {
  /** The network's PAN identifier.*/
  uint16_t  panId;
  /** A power setting, in dBm.*/
  int8_t   radioTxPower;
  /** A radio channel. Be sure to specify a channel supported by the radio. */
  uint8_t   radioChannel;
} EmberNetworkParameters;

/**
 * @brief Message length in bytes.
 */
typedef uint8_t EmberMessageLength;

/**
 * @brief Message options.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberMessageOptions
#else
typedef uint8_t EmberMessageOptions;
enum
#endif
{
  /** No options. */
  EMBER_OPTIONS_NONE                     = 0x00,
  /** The packet should be sent out encrypted. */
  EMBER_OPTIONS_SECURITY_ENABLED         = 0x01,
  /** An acknowledgment should be requested for the outgoing packet. */
  EMBER_OPTIONS_ACK_REQUESTED            = 0x02,
  /** The packet should be sent with high priority. */
  EMBER_OPTIONS_HIGH_PRIORITY            = 0x04,
};

/**
 * @brief An instance of this struct is passed to emberIncomingMessageHandler().
 * It describe the incoming message.
 */
typedef struct {
  /**
   * An ::EmberMessageOptions value indicating the options used for the incoming
   * packet.
   */
  EmberMessageOptions options;
  /**
   * An ::EmberNodeId value indicating source node ID.
   */
  EmberNodeId source;
  /**
   * The endpoint the message is destined to.
   */
  uint8_t endpoint;
  /**
   * The RSSI the packet was received with.
   */
  int8_t rssi;
  /**
   * An ::EmberMessageLength value indicating the length in bytes of the
   * incoming message.
   */
  EmberMessageLength length;
  /**
   * A pointer to the message payload.
   */
  uint8_t *payload;
} EmberIncomingMessage;

/**
 * @brief An instance of this struct is passed to emberMessageSentHandler().
 * It describes the outgoing packet.
 */
typedef struct {
  /**
   * An ::EmberMessageOptions value indicating the options used for transmitting
   * the outgoing message.
   */
  EmberMessageOptions options;
  /**
   * An ::EmberNodeId value indicating the destination short ID.
   */
  EmberNodeId destination;
  /**
   * The endpoint the message is destined to.
   */
  uint8_t endpoint;
  /**
   * A tag value the application can use to match ::emberMessageSend() calls to
   * the corresponding ::emberMessageSentHandler() calls.
   */
  uint8_t tag;
  /**
   * An ::EmberMessageLength value indicating the length in bytes of the
   * incoming message.
   */
  EmberMessageLength length;
  /**
   * A pointer to the message payload.
   */
  uint8_t *payload;
} EmberOutgoingMessage;

/** @brief This data structure contains the key data that is passed
 *   into various other functions. */
typedef struct {
  /** This is the key byte data. */
  uint8_t contents[EMBER_ENCRYPTION_KEY_SIZE];
} EmberKeyData;

/** @brief This function allows the programmer to gain access
 *  to the actual key data bytes of the EmberKeyData struct.
 *
 * @param key  A Pointer to an EmberKeyData structure.
 *
 * @return uint8_t* Returns a pointer to the first byte of the Key data.
 */
#if defined DOXYGEN_SHOULD_SKIP_THIS
uint8_t* emberKeyContents(EmberKeyData* key);
#else
#define emberKeyContents(key) ((key)->contents)
#endif

/**
 * @brief Either marks an event as inactive or specifies the units for the
 * event execution time.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberEventUnits
#else
typedef uint8_t EmberEventUnits;
enum
#endif
{
  /** The event is not scheduled to run. */
  EMBER_EVENT_INACTIVE = 0,
  /** The execution time is in approximate milliseconds.  */
  EMBER_EVENT_MS_TIME,
  /** The execution time is in 'binary' quarter seconds (256 approximate
      milliseconds each). */
  EMBER_EVENT_QS_TIME,
  /** The execution time is in 'binary' minutes (65536 approximate milliseconds
      each). */
  EMBER_EVENT_MINUTE_TIME,
  /** The event is scheduled to run at the earliest opportunity. */
  EMBER_EVENT_ZERO_DELAY
};

/** brief An identifier for a task */
typedef uint8_t EmberTaskId;

//----------------------------------------------------------------
// Events and event queues.

// Forward declarations to make up for C's one-pass type checking.
struct Event_s;
struct EventQueue_s;

/** @brief The static part of an event.  Each event can be used with only one
 * event queue.
 */

typedef const struct {
  struct EventQueue_s *queue;           // the queue this event goes on
  void (*handler)(struct Event_s *);    // called when the event fires
  void (*marker)(struct Event_s *);     // marking fuction, can be NULL
  const char *name;                     // event name for debugging purposes
} EventActions;

typedef struct Event_s {
  EventActions *actions;                // static data

  // For internal use only, but the 'next' field must be initialized
  // to NULL.
  struct Event_s *next;
  uint32_t timeToExecute;
} Event;

/** @brief An event queue is currently just a list of events ordered by
 * execution time.
 */
typedef struct EventQueue_s {
  Event *isrEvents;
  Event *events;
} EventQueue;

/** @brief Control structure for events.
 *
 * This structure should not be accessed directly.
 * This holds the event status (one of the @e EMBER_EVENT_ values)
 * and the time left before the event fires.
 */
typedef struct {
  /** The event's status, either inactive or the units for timeToExecute. */
  EmberEventUnits status;
  /** The id of the task this event belongs to. */
  EmberTaskId taskid;
  /** How long before the event fires.
   *  Units are always in milliseconds
   */
  uint32_t timeToExecute;
} EmberEventControl;

/** @brief Complete events with a control and a handler procedure.
 *
 * An application typically creates an array of events
 * along with their handlers.
 * The main loop passes the array to ::emberRunEvents() in order to call
 * the handlers of any events whose time has arrived.
 */
typedef PGM struct EmberEventData_S {
  /** The control structure for the event. */
  EmberEventControl *control;
  /** The procedure to call when the event fires. */
  void (*handler)(void);
} EmberEventData;

/** @brief Control structure for tasks.
 *
 * This structure should not be accessed directly.
 */
typedef struct {
  // The time when the next event associated with this task will fire
  uint32_t nextEventTime;
  // The list of events associated with this task
  EmberEventData *events;
  // A flag that indicates the task has something to do other than events
  bool busy;
} EmberTaskControl;

/** @brief Defines tasks that prevent the stack from sleeping.
 */
enum {
  /** There are messages waiting for transmission. */
  EMBER_OUTGOING_MESSAGES = 0x0001,
  /** One or more incoming messages are being processed. */
  EMBER_INCOMING_MESSAGES = 0x0002,
  /** The radio is currently powered on.  On sleepy devices the radio is
   *  turned off when not in use.  On non-sleepy devices, i.e.,
   *  ::EMBER_STAR_COORDINATOR, ::EMBER_STAR_RANGE_EXTENDER,
   *  or ::EMBER_STAR_END_DEVICE, the radio is always on.
   */
  EMBER_RADIO_IS_ON = 0x0004,
  /** The node is currently trying to associate to a Connect network. */
  EMBER_ASSOCIATING = 0x0008,
  /** The node is currently performing a MAC level scanning procedure. */
  EMBER_SCANNING = 0x0010,
};

/**
 * @brief Defines the events reported to the application
 * by the ::emberCounterHandler().
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberCounterType
#else
typedef uint8_t EmberCounterType;
enum
#endif
{
  /** Every packet that comes in over the radio (except mac acks). */
  EMBER_COUNTER_PHY_IN_PACKETS,

  /** Every packet that goes out over the radio (except mac acks). */
  EMBER_COUNTER_PHY_OUT_PACKETS,

  /** Incoming mac unicasts. */
  EMBER_COUNTER_MAC_IN_UNICAST,

  /** Incoming mac broadcasts. */
  EMBER_COUNTER_MAC_IN_BROADCAST,

  /** Outgoing mac unicasts that do not require an ack. */
  EMBER_COUNTER_MAC_OUT_UNICAST_NO_ACK,

  /** Outgoing mac unicasts that require an ack for which we received an ack,
   * possibly after retrying. */
  EMBER_COUNTER_MAC_OUT_UNICAST_ACK_SUCCESS,

  /** Outgoing unicasts for which we never received an ack even
   * after retrying. */
  EMBER_COUNTER_MAC_OUT_UNICAST_ACK_FAIL,

  /** Outgoing mac packets which were never transmitted because
   * clear channel assessment always returned busy. */
  EMBER_COUNTER_MAC_OUT_UNICAST_CCA_FAIL,

  /** Outgoing unicast retries.  This does not count the initial
   * transmission.  Note a single mac transmission can result in
   * multiple retries. */
  EMBER_COUNTER_MAC_OUT_UNICAST_RETRY,

  EMBER_COUNTER_MAC_OUT_BROADCAST,

  EMBER_COUNTER_MAC_OUT_BROADCAST_CCA_FAIL,

  /** Dropped incoming MAC packets (out of memory) */
  EMBER_COUNTER_MAC_DROP_IN_MEMORY,

  /** Dropped incoming MAC packets (invalid frame counter) */
  EMBER_COUNTER_MAC_DROP_IN_FRAME_COUNTER,

  /** Dropped incoming MAC packets (can't decrypt) */
  EMBER_COUNTER_MAC_DROP_IN_DECRYPT,

  /** Outgoing nwk forwarded packets */
  EMBER_COUNTER_NWK_OUT_FORWARDING,

  /** Incoming nwk data frames correctly processed */
  EMBER_COUNTER_NWK_IN_SUCCESS,

  /** Dropped incoming nwk packets (wrong source node) */
  EMBER_COUNTER_NWK_DROP_IN_WRONG_SOURCE,

  /** Dropped incoming nwk packets (can't forward) */
  EMBER_COUNTER_NWK_DROP_IN_FORWARDING,

  /** UART in and out data */
  EMBER_COUNTER_UART_IN_DATA,
  EMBER_COUNTER_UART_IN_MANAGEMENT,
  EMBER_COUNTER_UART_IN_FAIL,
  EMBER_COUNTER_UART_OUT_DATA,
  EMBER_COUNTER_UART_OUT_MANAGEMENT,
  EMBER_COUNTER_UART_OUT_FAIL,

  // Counters for non-packet events below.
  EMBER_COUNTER_ROUTE_2_HOP_LOOP,
  EMBER_COUNTER_BUFFER_ALLOCATION_FAIL,

  /** ASHv3 */
  EMBER_ASH_V3_ACK_SENT,
  EMBER_ASH_V3_ACK_RECEIVED,
  EMBER_ASH_V3_NACK_SENT,
  EMBER_ASH_V3_NACK_RECEIVED,
  EMBER_ASH_V3_RESEND,
  EMBER_ASH_V3_BYTES_SENT,
  EMBER_ASH_V3_TOTAL_BYTES_RECEIVED,
  EMBER_ASH_V3_VALID_BYTES_RECEIVED,
  EMBER_ASH_V3_PAYLOAD_BYTES_SENT,

  /** A placeholder giving the number of Ember counter types. */
  EMBER_COUNTER_TYPE_COUNT,
};

/**
 * @brief  Buffers used by the memory buffer system.
 */
typedef uint16_t EmberBuffer;

/**
 * @brief  Return type for Ember functions.
 */
#ifndef __EMBERSTATUS_TYPE__
#define __EMBERSTATUS_TYPE__
typedef uint8_t EmberStatus;
#endif //__EMBERSTATUS_TYPE__

//------------------------------------------------------------------------------
// INTERNAL TYPES AND DEFINES
//------------------------------------------------------------------------------

#ifndef DOXYGEN_SHOULD_SKIP_THIS

typedef uint16_t Buffer;
typedef uint16_t EmberMessageBuffer;

#define NULL_BUFFER 0x0000

typedef struct {
  EmberNodeId source;
  EmberNodeId destination;
  uint8_t endpoint;
  uint8_t tag;
  Buffer payload;
  EmberMessageOptions txOptions;
} EmOutgoingPacket;

enum {
  /* RX status codes */
  EMBER_PACKET_TRACE_RX_STATUS_SUCCESS      = 0x00,
  EMBER_PACKET_TRACE_RX_STATUS_CRC_FAILED   = 0x01,
  EMBER_PACKET_TRACE_RX_STATUS_MAC_FILTERED = 0x02,
  /* TX status codes */
  EMBER_PACKET_TRACE_TX_STATUS_SUCCESS      = 0x80
};

typedef struct {
  uint8_t status;
  int8_t rssi;
  uint8_t channel;
  EmberMessageLength packetLength;
  uint8_t *packet;
} EmberMessageTrace;

typedef struct {
  uint32_t lastSeen;
  uint32_t frameCounter;
  EmberNodeId shortId;
  EmberEUI64 longId;
  uint8_t flags;
} EmberChildEntry;

typedef uint8_t EmberLibraryStatus;

#endif //DOXYGEN_SHOULD_SKIP_THIS

#endif // __EMBER_TYPES_H__

/** @} // END addtogroup
 */
