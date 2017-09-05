/*
 * File: buffer-management.h
 * Description: Buffer allocation and management routines.
 * Author(s): Richard Kelsey
 *
 * Copyright 2009 by Ember Corporation. All rights reserved.                *80*
 */

// These buffers are contiguous blocks of bytes.  Buffers are allocated
// linearly from a single chunk of memory.  When space gets short any
// buffers still in use can be moved to the beginning of buffer memory,
// which consolidates any unused space.
//
// 'Buffer' values are actually offsets from the beginning of buffer
// memory.  Using uint8_ts would require additional memory for the
// mapping between the uint8_ts and memory locations.
//
// In fact, Buffers are offsets from one before the beginning of buffer
// memory.  The first buffer is thus 0x0001.  This allows us to use 0x0000
// as the null buffer, just as C uses zero for the null pointer.
//
// In general the buffer functions are not safe for use in ISRs.
// The one exception is the PHY->MAC queue, which is handled specially.
// It is safe for an ISR to allocate a new buffer and put it on the
// PHY->MAC queue.

#ifndef __BUFFER_MANAGEMENT_H__
#define __BUFFER_MANAGEMENT_H__

Buffer emReallyAllocateBuffer(uint16_t dataSizeInBytes, bool async);

#define emAllocateBuffer(dataSizeInBytes) \
  emReallyAllocateBuffer(dataSizeInBytes, false)

uint8_t *emGetBufferPointer(Buffer buffer);

uint16_t emGetBufferLength(Buffer buffer);

// 'contents' may be NULL, in which case NULL_BUFFER is returned.
Buffer emFillStringBuffer(const uint8_t *contents);

#define emFillBuffer(contents, length) \
  emReallyFillBuffer(contents, length, false)

Buffer emReallyFillBuffer(const uint8_t *contents, uint16_t length, bool async);

uint16_t emBufferBytesRemaining(void);

// Every buffer has two links to other buffers.  For packets, these
// are used to organize buffers into queues and to associate header
// and payload buffers.

Buffer emGetBufferLink(Buffer buffer, uint8_t i);
void emSetBufferLink(Buffer buffer, uint8_t i, Buffer newLink);

#define QUEUE_LINK   0
#define PAYLOAD_LINK 1

#define emGetQueueLink(buffer)   (emGetBufferLink((buffer), QUEUE_LINK))
#define emGetPayloadLink(buffer) (emGetBufferLink((buffer), PAYLOAD_LINK))

#define emSetQueueLink(buffer, new) \
  (emSetBufferLink((buffer), QUEUE_LINK, (new)))
#define emSetPayloadLink(buffer, new) \
  (emSetBufferLink((buffer), PAYLOAD_LINK, (new)))

// Bookkeeping.
void emInitializeBuffers(void);
bool emPointsIntoHeap(void *pointer);

void emResetBufferTracking(void);
uint16_t emGetTrackedBufferBytes(void);
void emResetTraceTracker(void);
uint16_t emGetTracedBytes(void);

bool emIsValidBuffer(Buffer buffer);

// Buffer space can be reserved to prevent it from being allocated
// asynchronously (typically by the radio receive ISR).  Reserving
// buffer space splits the available memory into two parts, with
// synchronous allocation getting the reserved amount and asynchronous
// allocation getting anything left over.  Neither is allowed to
// overrun its allotment.
//
// Reserving an insufficient amount of memory is a bug.  An assert
// fires if memory has been reserved and a synchronous allocation asks
// for more memory than is available in the reserved portion.
//
// NULL_BUFFER is returned if an asynchronous allocation cannot be
// satisified or if there is no reservation in place and a synchronous
// allocation cannot be satisfied.

#define emAllocateAsyncBuffer(dataSizeInBytes) \
  emReallyAllocateBuffer(dataSizeInBytes, true)

Buffer emAllocateIndirectBuffer(uint8_t *contents,
                                void    *freePtr,
                                uint16_t length);

bool emSetReservedBufferSpace(uint16_t dataSizeInBytes);
void emEndBufferSpaceReservation(void);

//----------------------------------------------------------------

// Truncates the buffer.  New length cannot be longer than the old length.

void emSetBufferLength(Buffer buffer, uint16_t length);

// Ditto, but remove bytes from the front of the buffer rather than the end.
void emSetBufferLengthFromEnd(Buffer buffer, uint16_t length);

// Reclaims all buffers not reachable from 'roots', the PHY->MAC
// queue, by one of marker functions.  The marker functions should
// call their argument on all known buffer references.
//
// The idea is that single top level references, such as a queue,
// can be put in 'roots', whereas buffers stored in data structures,
// such as the retry table, are handled by marker functions.  See
// emMarkRetryBuffers() in stack/routing/util/retry.c, for example.
//
// Surviving buffers moved to the beginning of the heap, which
// amalgamates the free memory into a single contiguous block.  This
// should only be called when there are no references to buffers outside
// of 'roots', the PHY->MAC queue, and the locations passed to
// the marker functions.
//
// IMPORTANT: BufferMarker routines should not reference buffers after
// they have been marked.  The buffers may be in an inconsistent state
// until the reclaimation has completed.
//
// Right:
//   void myBufferMarker(void)
//   {
//     ... myBuffer ...
//     emMarkBuffer(&myBuffer);
//   }
//
// Wrong:
//   void myBufferMarker(void)
//   {
//     emMarkBuffer(&myBuffer);
//     ... myBuffer ...
//   }
//

typedef void (*Marker)(Buffer *buffer);
typedef void (*BufferMarker)(void);

void emMarkBuffer(Buffer *root);
bool emMarkAmalgamateQueue(Buffer *queue);

void emReclaimUnusedBuffers(const BufferMarker *markers);

void emReclaimUnusedBuffersAndAmalgamate(const BufferMarker *markers,
                                         uint8_t* scratchpad,
                                         uint16_t scratchpadSize);

void emPrintBuffers(uint8_t port, const BufferMarker *markers);

// A utility for marking a buffer via a pointer to its contents.
void emMarkBufferPointer(void **pointerLoc);

// Applications that use buffers must mark them by defining this function.
// The stack uses this when reclaiming unused buffers.
void emberMarkApplicationBuffers(void);

uint16_t emBufferChainByteLength(Buffer buffer, uint8_t link);
#define emTotalPayloadLength(buffer) \
  (emBufferChainByteLength((buffer), 1))

// Buffers referenced by ISRs must be protected in order to avoid
// problems during reclamations.  A protected buffer may be referenced
// from ISRs, but only via the location passed to
// emInterruptProtectBuffer().  The link fields of a protected object
// may not be read or written during an ISR.

void emInterruptProtectBuffer(Buffer *bufferLocation);
void emClearInterruptProtectBuffer(Buffer buffer);

// The PHY->MAC queue is special in that it can be added to in ISR context.

void emPhyToMacQueueAdd(Buffer newTail);
Buffer emPhyToMacQueueRemoveHead(void);
bool emPhyToMacQueueIsEmpty(void);
void emEmptyPhyToMacQueue(void);
void emPhyToMacQueueItemAddedCallback(void);

// Allow the wakeup code to save and restore the heap pointer.  This is
// much faster than emReclaimUnusedBuffers() but requires that the caller
// knows that there are no references anywhere to newly allocated buffers.
uint16_t emSaveHeapState(void);
void emRestoreHeapState(uint16_t state);

// This handler is called when freeing memory allocated with
// emberAllocateMemoryForPacketHandler(). You will be passed the reference that was
// supplied when allocating the memory. If you passed NULL as the objectRef
// value then this callback is not called.
void emberFreeMemoryForPacketHandler(void *objectRef);

// This function can be used to hook an external memory allocator into the
// stack. It will be called when we need to allocate large packets. If a value
// other than NULL is returned that pointer will be used to store packet data.
// You must also fill in the objectRef parameter with whatever reference you
// would like passed to the emberFreeMemoryForPacketHandler() when we're done
// with this memory. If you set the objectRef to NULL then the free handler will
// not be called.
void *emberAllocateMemoryForPacketHandler(uint32_t size, void **objectRef);

// This function can be used to get the object reference for a buffer allocated
// using one of the callbacks above. If you did not specify an object reference
// or this is a regular ember buffer then NULL will be returned.
void* emberGetObjectRefFromBuffer(Buffer b);

//----------------------------------------------------------------
// Utilities used by buffer-malloc.c

Buffer emBufferPointerToBuffer(uint16_t *bufferPointer);
Buffer emFollowingBuffer(Buffer buffer);
void emMergeBuffers(Buffer first, Buffer second);
Buffer emSplitBuffer(Buffer buffer, uint16_t newLength);

//----------------------------------------------------------------
// Macros for the MessageBuffer interface.

#define emberMessageBufferContents emGetBufferPointer
#define emberMessageBufferLength emGetBufferLength
#define emberSetMessageBufferLength emSetBufferLength

#define emPacketHeaderPayload(header) emGetPayloadLink(header)

#define emSetPacketHeaderPayload(header, payload) \
  emSetPayloadLink((header), (payload))

#define emberFillLinkedBuffers(contents, length) \
  emReallyFillBuffer((contents), (length), false)

#define emberFillLinkedAsyncBuffers(contents, length) \
  emReallyFillBuffer((contents), (length), true)

#define emMessageBufferQueueAdd emBufferQueueAdd
#define emMessageBufferQueueRemoveHead emBufferQueueRemoveHead
#define emMessageBufferQueueRemove emBufferQueueRemove
#define emMessageBufferQueueIsEmpty emBufferQueueIsEmpty

#undef EMBER_NULL_MESSAGE_BUFFER
#define EMBER_NULL_MESSAGE_BUFFER NULL_BUFFER

#define bufferUse(tag)    do { emBufferUsage(tag); } while (0)
#define endBufferUse(tag) do { emEndBufferUsage(); } while (0)
void emBufferUsage(const char *tag);
void emEndBufferUsage(void);

//----------------------------------------------------------------
// A linked list of buffers, ordered by location in memory.
// Keeping them ordered makes it easier to merge adjacent free
// buffer together.
//
// This must be set to NULL_BUFFER before calling
// emReclaimUnusedBuffers().

extern uint16_t heapMemory[];
extern const uint32_t heapMemorySize;

#endif
