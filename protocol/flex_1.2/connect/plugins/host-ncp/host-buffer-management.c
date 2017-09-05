/*
 * File: host-buffer-management.c
 * Description: Buffer allocation and management routines for Host.
 * Author(s): Nathan Smith
 *
 * Copyright 2015 by Silicon Laboratories. All rights reserved.             *80*
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

typedef uint16_t Buffer;
#define NULL_BUFFER 0x0000

#include "host-buffer-management.h"

typedef struct {
  // its location
  Buffer location;

  // its data
  uint8_t *data;

  // its data length
  uint16_t length;

  // are we marked?
  bool marked;

  // for lists
  Buffer payloadLink;
  Buffer queueLink;

  // used during buffer reclamation
  Buffer newContentsLocation;
} BufferContents;

static BufferContents *bufferContents = NULL;
uint16_t bufferContentsSize = 0;
uint16_t bufferContentsIndex = 0;

// used while merging
static BufferContents *newContents = NULL;
static uint16_t newContentsSize = 0;

typedef enum {
  NO_PHASE,
  MARK_LIVE,
  UPDATE
} Phase;

Phase tracePhase = NO_PHASE;

void printBufferContents(void)
{
  uint32_t i;

  for (i = 0; i < bufferContentsIndex; i++) {
    BufferContents *contents = &bufferContents[i];
    printf("[%u] location %u | "
           "length %u | "
           "marked %u | "
           "new location %u | "
           "payload link %u | "
           "queue link %u\n",
           i,
           contents->location,
           contents->length,
           contents->marked,
           contents->newContentsLocation,
           contents->payloadLink,
           contents->queueLink);
  }
}

void emInitializeBuffers(void)
{
  bufferContentsSize = 0;
  bufferContents = NULL;
}

Buffer emReallyAllocateBuffer(uint16_t length, bool unused)
{
  return emAllocateBuffer(length);
}

Buffer emAllocateBuffer(uint16_t length)
{
  if (bufferContentsIndex >= bufferContentsSize) {
    if (bufferContentsSize == 0) {
      bufferContentsSize = 10;
    }

    bufferContents = realloc(bufferContents,
                             sizeof(BufferContents) * bufferContentsSize * 2);

    if (bufferContents == NULL) {
      return NULL_BUFFER;
    }

    bufferContentsSize *= 2;
  }

  BufferContents contents;
  memset(&contents, 0, sizeof(BufferContents));
  contents.location = bufferContentsIndex + 1;
  contents.data = malloc(length);
  contents.length = length;
  contents.payloadLink = NULL_BUFFER;
  contents.queueLink = NULL_BUFFER;

  if (contents.data == NULL) {
    return NULL_BUFFER;
  }

  memcpy(&bufferContents[bufferContentsIndex],
         &contents,
         sizeof(BufferContents));

  bufferContentsIndex++;
  return (bufferContentsIndex);
}

uint8_t *emGetBufferPointer(Buffer buffer)
{
  if (buffer == NULL_BUFFER) {
    return NULL;
  }

  assert(buffer - 1 < bufferContentsIndex);
  return bufferContents[buffer - 1].data;
}

uint16_t emGetBufferLength(Buffer buffer)
{
  assert(buffer - 1 < bufferContentsIndex);
  return bufferContents[buffer - 1].length;
}

static uint32_t bufferRun = 0;

void emReclaimUnusedBuffers(const BufferMarker *markers)
{
  // mark everything to false
  size_t i;
  bufferRun++;

  for (i = 0; i < bufferContentsIndex; i++) {
    bufferContents[i].marked = false;
  }

  if (markers != NULL) {
    tracePhase = MARK_LIVE;

    for (i = 0; markers[i] != NULL; i++) {
      markers[i]();
    }
  }

  // how big is it now?
  uint16_t newSize = 0;

  for (i = 0; i < bufferContentsIndex; i++) {
    if (bufferContents[i].marked) {
      newSize++;
    }
  }

  // squish the possibly sparsely allocated bufferContents array
  // if newSize == bufferContentsIndex, there's no need to do anything
  if (newSize == 0) {
    size_t i;

    for (i = 0; i < bufferContentsIndex; i++) {
      assert(bufferContents[i].length > 0);
      free(bufferContents[i].data);
    }

    free(bufferContents);
    bufferContents = NULL;
    bufferContentsIndex = 0;
    bufferContentsSize = 0;
  } else if (newSize < bufferContentsIndex) {
    newContentsSize = newSize;
    newContents = malloc(sizeof(BufferContents) * newContentsSize);
    memset(newContents, 0, sizeof(BufferContents) * newContentsSize);

    if (newContents == NULL) {
      // what to do?
      assert(false);
    }

    size_t j;

    for (i = 0, j = 0; i < bufferContentsIndex; i++) {
      if (bufferContents[i].marked) {
        memcpy(&newContents[j], &bufferContents[i], sizeof(BufferContents));
        newContents[j].location = j + 1;
        bufferContents[i].newContentsLocation = j;
        j++;
      } else {
        assert(bufferContents[i].length > 0);
        free(bufferContents[i].data);
      }
    }

    // update the positions
    if (markers != NULL) {
      tracePhase = UPDATE;

      for (i = 0; markers[i] != NULL; i++) {
        markers[i]();
      }
    }

    free(bufferContents);
    bufferContents = newContents;
    bufferContentsSize = newContentsSize;
    bufferContentsIndex = newContentsSize;

    newContents = NULL;
    newContentsSize = 0;
  }
}

void emMarkBuffer(Buffer *root)
{
  if (*root == NULL_BUFFER) {
    return;
  }

  assert(*root - 1 < bufferContentsIndex);

  BufferContents *contents = &bufferContents[*root - 1];

  if (contents->queueLink != NULL_BUFFER) {
    emMarkBuffer(&contents->queueLink);
  }

  if (contents->payloadLink != NULL_BUFFER) {
    emMarkBuffer(&contents->payloadLink);
  }

  if (tracePhase == MARK_LIVE) {
    contents->marked = true;
  } else if (tracePhase == UPDATE) {
    *root = contents->newContentsLocation + 1;
  }
}

void *emBufferMalloc(uint16_t size)
{
  Buffer buffer = emAllocateBuffer(size);
  return (buffer != NULL_BUFFER
          ? emGetBufferPointer(buffer)
          : NULL);
}

// this is slow
void emMarkBufferPointer(uint8_t **pointer)
{
  uint16_t i;

  for (i = 0; i < bufferContentsIndex; i++) {
    if (bufferContents[i].data == *pointer) {
      if (tracePhase == MARK_LIVE) {
        bufferContents[i].marked = true;
      } else if (tracePhase == UPDATE) {
        assert(newContents != NULL);
        *pointer = newContents[bufferContents[i].newContentsLocation].data;
        assert(*pointer != NULL);
      }
    }
  }
}

Buffer emGetBufferLink(Buffer buffer, uint8_t i)
{
  assert(buffer - 1 < bufferContentsIndex);

  if (i == QUEUE_LINK) {
    return bufferContents[buffer - 1].queueLink;
  } else if (i == PAYLOAD_LINK) {
    return bufferContents[buffer - 1].payloadLink;
  }

  assert(false);
  return NULL_BUFFER;
}

void emSetBufferLink(Buffer buffer, uint8_t i, Buffer newLink)
{
  assert(buffer - 1 < bufferContentsIndex);

  if (i == QUEUE_LINK) {
    bufferContents[buffer - 1].queueLink = newLink;
  } else if (i == PAYLOAD_LINK) {
    bufferContents[buffer - 1].payloadLink = newLink;
  } else {
    assert(false);
  }
}

Buffer emReallyFillBuffer(const uint8_t *contents, uint16_t length, bool async)
{
  Buffer result = emAllocateBuffer(length);

  if (result != NULL_BUFFER) {
    uint8_t *bytes = emGetBufferPointer(result);
    memcpy(bytes, contents, length);
  }

  return result;
}

Buffer emFillStringBuffer(const uint8_t *contents)
{
  return (contents == NULL
          ? NULL_BUFFER
          : emFillBuffer(contents, strlen((char const *)contents) + 1));
}

void simPrintBytes(char *prefix, uint8_t *bytes, uint16_t count)
{
  uint16_t i;

  fprintf(stderr, "%s", prefix);
  for (i = 0; i < count; i++) {
    fprintf(stderr, " %02X", bytes[i]);
  }
  fprintf(stderr, "\n");
}

#define USE_STUB_emBufferUsage
#define USE_STUB_emEndBufferUsage

//------------------------------------------------------------------------------
// These are wrappers for the subset of functionality we currently expose to the
// application layer.

Buffer emberAllocateBuffer(uint16_t length)
{
  return emAllocateBuffer(length);
}

void emberMarkBuffer(Buffer *root)
{
  emMarkBuffer(root);
}

uint8_t *emberGetBufferPointer(Buffer buffer)
{
  return emGetBufferPointer(buffer);
}

uint16_t emberGetBufferLength(Buffer buffer)
{
  return emGetBufferLength(buffer);
}
