/** @file ember-configuration.c
 * @brief User-configurable stack memory allocation and convenience stubs
 * for little-used callbacks.
 *
 *
 * \b Note: Application developers should \b not modify any portion
 * of this file. Doing so may lead to mysterious bugs. Allocations should be
 * adjusted only with macros in a custom CONFIGURATION_HEADER.
 *
 * <!--Copyright 2014 by Silicon Laboratories. All rights reserved.      *80*-->
 */

#include PLATFORM_HEADER

#include "stack/include/ember.h"
#include "stack/include/error.h"

#include "stack/config/ember-configuration-defaults.h"

PGM uint8_t emTaskCount = EMBER_TASK_COUNT;
EmberTaskControl emTasks[EMBER_TASK_COUNT];

// Configurable stack parameters.
uint8_t emberMacAckTimeoutMS = EMBER_MAC_ACK_TIMEOUT_MS;
int8_t emRadioCcaThreshold = EMBER_RADIO_CCA_THRESHOLD;
uint16_t emberFrequencyHoppingSeed = EMBER_FREQUENCY_HOPPING_SEED;
uint16_t emberFrequencyHoppingStartChannel = EMBER_FREQUENCY_HOPPING_START_CHANNEL;
uint16_t emberFrequencyHoppingEndChannel = EMBER_FREQUENCY_HOPPING_END_CHANNEL;
uint16_t emberFrequencyHoppingChannelDurationMs = EMBER_FREQUENCY_HOPPING_CHANNEL_DURATION_MS;
uint16_t emberFrequencyHoppingChannelGuardDurationMs = EMBER_FREQUENCY_HOPPING_CHANNEL_GUARD_DURATION_MS;
uint16_t emberFrequencyHoppingServerFreqInfoBroadcastPeriodS = EMBER_FREQUENCY_HOPPING_SERVER_FREQ_INFO_BROADCAST_PERIOD_S;
uint16_t emberFrequencyHoppingClientResyncPeriodS = EMBER_FREQUENCY_HOPPING_CLIENT_RESYNC_PERIOD_S;

#if defined(CORTEXM3)
// This declaration is performed only to ensure that the size of the memory heap
// is *at least* EMBER_HEAP_SIZE (if not, the linker would return an error).
// The buffer management code uses the full heap memory segment.
VAR_AT_SEGMENT(NO_STRIPPING uint16_t heapMemory[(EMBER_HEAP_SIZE) / 2], __EMHEAP__);
#else
uint16_t heapMemory[(EMBER_HEAP_SIZE) / 2];
const uint32_t heapMemorySize = EMBER_HEAP_SIZE;
#endif

#if (EMBER_CHILD_TABLE_SIZE > 0)
uint32_t emberChildTimeoutSec = EMBER_CHILD_TIMEOUT_SEC;
uint8_t emberChildTableSize = EMBER_CHILD_TABLE_SIZE;
static EmberChildEntry childTable[EMBER_CHILD_TABLE_SIZE];
EmberChildEntry *emChildTable = childTable;
#endif // EMBER_CHILD_TABLE_SIZE > 0

#if (EMBER_MAC_OUTGOING_QUEUE_SIZE > 0)
uint8_t emberMacOutgoingQueueSize = EMBER_MAC_OUTGOING_QUEUE_SIZE;
static EmOutgoingPacket macOutgoingQueue[EMBER_MAC_OUTGOING_QUEUE_SIZE];
EmOutgoingPacket *emMacOutgoingQueue = macOutgoingQueue;
#endif // EMBER_MAC_OUTGOING_QUEUE_SIZE > 0

#if (EMBER_INDIRECT_QUEUE_SIZE > 0)
uint8_t emberIndirectPacketQueueSize = EMBER_INDIRECT_QUEUE_SIZE;
static EmOutgoingPacket indirectPacketQueue[EMBER_INDIRECT_QUEUE_SIZE];
static uint16_t indirectQueueTimeouts[EMBER_INDIRECT_QUEUE_SIZE];
EmOutgoingPacket *emIndirectPacketQueue = indirectPacketQueue;
uint16_t *emIndirectQueueTimeouts = indirectQueueTimeouts;
uint16_t emberMacIndirectTimeoutMs = EMBER_INDIRECT_TRANSMISSION_TIMEOUT_MS;
#endif // EMBER_INDIRECT_QUEUE_SIZE > 0
