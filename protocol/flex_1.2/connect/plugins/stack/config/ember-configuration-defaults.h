/** @file ember-configuration-defaults.h
 * @brief User-configurable stack memory allocation defaults
 *
 * @note Application developers should \b not modify any portion
 * of this file. Doing so may cause mysterious bugs. Allocations should be
 * adjusted only by defining the appropriate macros in the application's
 * CONFIGURATION_HEADER.
 *
 * See @ref configuration for documentation.
 * <!--Copyright 2014 by Silicon Laboratories. All rights reserved.      *80*-->
 */

/**
 * @addtogroup configuration
 *
 * All configurations have defaults, therefore many applications may not need
 * to do anything special.  However, you can override these defaults
 * by creating a CONFIGURATION_HEADER and within this header,
 * defining the appropriate macro to a different size.
 *
 * See ember-configuration-defaults.h for source code.

 * @{
 */

#ifndef __EMBER_CONFIGURATION_DEFAULTS_H__
#define __EMBER_CONFIGURATION_DEFAULTS_H__

#ifdef CONFIGURATION_HEADER
  #include CONFIGURATION_HEADER
#endif

/** @brief The number of event tasks that can be tracked for the purpose of
 *  processor idling.
 */
#ifndef EMBER_TASK_COUNT
 #define EMBER_TASK_COUNT (3)
#endif

/** @brief The size of the Ember heap. This is set to default to 0. If the
 * application includes the optional stack features that require dynamic memory
 * allocation, this should adjusted accordingly.
 */
#ifndef EMBER_HEAP_SIZE
  #define EMBER_HEAP_SIZE                           2000
#endif

/** @brief The maximum number of children that a node may have. This is set to
 * default to 0. If the application includes the optional parent support stack
 * feature, this should be adjusted accordingly.
 */
#ifndef EMBER_CHILD_TABLE_SIZE
  #define EMBER_CHILD_TABLE_SIZE                    0
#endif

#define EMBER_CHILD_TABLE_TOKEN_SIZE                EMBER_CHILD_TABLE_SIZE

/** @brief Every child should exchange regularly some sort of traffic with the
 * parent. Eventually, if traffic is not exchanged for a prolonged period of
 * time, the parent will remove the child from the child table.
 * Range extenders periodically exchange network-level commands with the
 * coordinator. End devices and sleepy end devices can use ::emberPollForData()
 * as keep alive mechanism.
 */
#ifndef EMBER_CHILD_TIMEOUT_SEC
  #define EMBER_CHILD_TIMEOUT_SEC                   3600
#endif

/** @brief The maximum number of pending indirect packets. This is set to
 * default to 0. If the application includes the optional parent support stack
 * feature, this should be adjusted accordingly.
 */
#ifndef EMBER_INDIRECT_QUEUE_SIZE
  #define EMBER_INDIRECT_QUEUE_SIZE                 0
#endif

/** @brief The maximum number of packets in the outgoing MAC queue. This is set
 * to default to 0. If the application includes the optional MAC queue stack
 * feature, this should be adjusted accordingly.
 */
#ifndef EMBER_MAC_OUTGOING_QUEUE_SIZE
  #define EMBER_MAC_OUTGOING_QUEUE_SIZE             0
#endif

/** @brief The maximum amount of time (in milliseconds) that the MAC
 * will hold a message for indirect transmission to a child.
 *
 * The default is 8000 milliseconds (8 sec).
 * The maximum value is 30 seconds (30000 milliseconds).larger values
 * will cause rollover confusion.
 */
#ifndef EMBER_INDIRECT_TRANSMISSION_TIMEOUT_MS
  #define EMBER_INDIRECT_TRANSMISSION_TIMEOUT_MS 8000
#endif
#define EMBER_MAX_INDIRECT_TRANSMISSION_TIMEOUT_MS 30000
#if (EMBER_INDIRECT_TRANSMISSION_TIMEOUT_MS \
     > EMBER_MAX_INDIRECT_TRANSMISSION_TIMEOUT_MS)
  #error "Indirect transmission timeout too large."
#endif

/** @brief The period in seconds a range extender sends an update command to the
 * coordinator containing the list of its children.
 */
#ifndef EMBER_NWK_RANGE_EXTENDER_UPDATE_PERIOD_SEC
  #define EMBER_NWK_RANGE_EXTENDER_UPDATE_PERIOD_SEC     60
#endif

/** @brief The ACK timeout in milliseconds. This can be fine-tuned to reduce
 * energy consumption at sleepy devices. This parameter depends on the data rate
 * of the PHY configuration being used.
 */
#ifndef EMBER_MAC_ACK_TIMEOUT_MS
#define EMBER_MAC_ACK_TIMEOUT_MS 40
#endif // EMBER_MAC_ACK_TIMEOUT_MS

/** @brief The CCA threshold used at the MAC layer.
 */
#ifndef EMBER_RADIO_CCA_THRESHOLD
#define EMBER_RADIO_CCA_THRESHOLD -65
#endif // EMBER_RADIO_CCA_THRESHOLD

/** @brief The frequency hopping channel sequence generation seed.
 */
#ifndef EMBER_FREQUENCY_HOPPING_SEED
#define EMBER_FREQUENCY_HOPPING_SEED 0
#endif // EMBER_FREQUENCY_HOPPING_SEED

/** @brief The lowest channel on the frequency hopping list.
 */
#ifndef EMBER_FREQUENCY_HOPPING_START_CHANNEL
#define EMBER_FREQUENCY_HOPPING_START_CHANNEL 0
#endif // EMBER_FREQUENCY_HOPPING_START_CHANNEL

/** @brief The highest channel on the frequency hopping list.
 */
#ifndef EMBER_FREQUENCY_HOPPING_END_CHANNEL
#define EMBER_FREQUENCY_HOPPING_END_CHANNEL 24
#endif // EMBER_FREQUENCY_HOPPING_END_CHANNEL

/** @brief The time in milliseconds to stay on each channel for frequency
 *  hopping.
 */
#ifndef EMBER_FREQUENCY_HOPPING_CHANNEL_DURATION_MS
#define EMBER_FREQUENCY_HOPPING_CHANNEL_DURATION_MS 400
#endif // EMBER_FREQUENCY_HOPPING_CHANNEL_DURATION_MS

/** @brief The time in milliseconds to guard each channel while frequency
 *  hopping.No mac activity will be allowed when entering or exiting the slot.
 */
#ifndef EMBER_FREQUENCY_HOPPING_CHANNEL_GUARD_DURATION_MS
#define EMBER_FREQUENCY_HOPPING_CHANNEL_GUARD_DURATION_MS 20
#endif // EMBER_FREQUENCY_HOPPING_CHANNEL_GUARD_DURATION_MS

/** @brief The duration in second after which the server should broadcast
 * its Frequency Hopping information so that the clients can realign.
 */
#ifndef EMBER_FREQUENCY_HOPPING_SERVER_FREQ_INFO_BROADCAST_PERIOD_S
#define EMBER_FREQUENCY_HOPPING_SERVER_FREQ_INFO_BROADCAST_PERIOD_S 15
#endif // EMBER_FREQUENCY_HOPPING_SERVER_FREQ_INFO_BROADCAST_PERIOD_S

/** @brief The duration in seconds after which the client should resync
 * with the server if the last resync happened more than this duration ago.
 */
#ifndef EMBER_FREQUENCY_HOPPING_CLIENT_RESYNC_PERIOD_S
#define EMBER_FREQUENCY_HOPPING_CLIENT_RESYNC_PERIOD_S 60
#endif // EMBER_FREQUENCY_HOPPING_CLIENT_RESYNC_PERIOD_S

/** @} END addtogroup */

#endif //__EMBER_CONFIGURATION_DEFAULTS_H__
