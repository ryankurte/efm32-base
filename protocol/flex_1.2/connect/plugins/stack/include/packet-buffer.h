/**
 * @file packet-buffer.h
 *
 * This file contains a subset of the actual packet-buffer.h header, which is
 * not publicly released. This is a work-around for the serial plugin in
 * base/hal/plugin/serial that requires the definitions below.
 * Applications MUST NOT use these functions below nor stack code should include
 * this file. The interface available to applications is defined in
 * message-buffer.h.
 */

// None of these should endup in doxygen
#ifndef DOXYGEN_SHOULD_SKIP_THIS

extern EmberMessageBuffer *emPacketBufferLinks;

#define LOG_PACKET_BUFFER_SIZE 5
#define PACKET_BUFFER_SIZE (1 << LOG_PACKET_BUFFER_SIZE)

#define emberAllocateStackBuffer() (emberAllocateLinkedBuffers(1))
#define emberStackBufferLink(buffer) \
  (emPacketBufferLinks[(buffer)])
#define emberLinkedBufferContents(buffer) emberMessageBufferContents(buffer)

EmberMessageBuffer emberAllocateLinkedBuffers(uint8_t count);
EmberStatus emberAppendToLinkedBuffers(EmberMessageBuffer buffer,
                                       uint8_t *contents,
                                       uint8_t length);
uint16_t emberMessageBufferLength(EmberMessageBuffer buffer);
void emberHoldMessageBuffer(EmberMessageBuffer buffer);
void emberReleaseMessageBuffer(EmberMessageBuffer buffer);
uint8_t *emberMessageBufferContents(EmberMessageBuffer buffer);
EmberStatus emberAppendPgmStringToLinkedBuffers(EmberMessageBuffer buffer, PGM_P suffix);
EmberMessageBuffer emberFillLinkedBuffers(uint8_t *contents, uint8_t length);

#endif // !DOXYGEN_SHOULD_SKIP_THIS
