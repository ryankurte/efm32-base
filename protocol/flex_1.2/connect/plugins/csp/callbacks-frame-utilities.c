/**************************************************************************//**
 * Copyright 2017 Silicon Laboratories, Inc.
 *
 *****************************************************************************/
//
// Description: Functions for reading and writing command and response frames.

#include PLATFORM_HEADER

#include "stack/include/ember-types.h"
#include "callbacks-frame-utilities.h"
#include "host-ncp/host-buffer-management.h"
#include "csp-fire-callbacks.h"

static Buffer callbacksBuffer = { 0 };
static uint8_t readCallbacksOffset;
static uint8_t writeCallbacksOffset;
static uint8_t size;

void initializeCallbacksBuffers(void)
{
  emMarkBuffer(&callbacksBuffer);
  callbacksBuffer = emAllocateBuffer(CSP_CALLBACKS_BUFFER_SIZE);
  readCallbacksOffset = 0;
  writeCallbacksOffset = 0;
  size = 0;
}

void markCallbacksBuffers(void)
{
  emMarkBuffer(&callbacksBuffer);
}

uint8_t removeUint8_t(void)
{
  uint8_t* currentReadCallbacksOffset = (uint8_t *)emGetBufferPointer(callbacksBuffer);
  currentReadCallbacksOffset += readCallbacksOffset;
  uint8_t value = (uint8_t) *currentReadCallbacksOffset;
  readCallbacksOffset++;
  size--;
  return value;
}

uint8_t *removeUint8_tPointer(uint8_t length)
{
  uint8_t* currentReadCallbacksOffset = (uint8_t *)emGetBufferPointer(callbacksBuffer);
  currentReadCallbacksOffset += readCallbacksOffset;
  uint8_t *result = currentReadCallbacksOffset;
  readCallbacksOffset += length;
  return result;
}

void addUint8_t(uint8_t value)
{
  uint8_t* currentWriteCallbacksOffset = (uint8_t *)emGetBufferPointer(callbacksBuffer);
  currentWriteCallbacksOffset += writeCallbacksOffset;
  *currentWriteCallbacksOffset = value;
  writeCallbacksOffset++;
  size++;
}

void addInt8_t(int8_t value)
{
  uint8_t* currentWriteCallbacksOffset = (uint8_t *)emGetBufferPointer(callbacksBuffer);
  currentWriteCallbacksOffset += writeCallbacksOffset;
  *currentWriteCallbacksOffset = value;
  writeCallbacksOffset++;
  size++;
}

void addUint16_t(uint16_t value)
{
  addUint8_t(LOW_BYTE(value));
  addUint8_t(HIGH_BYTE(value));
}

uint16_t removeUint16_t(void)
{
  uint8_t low = removeUint8_t();
  uint8_t high = removeUint8_t();
  return HIGH_LOW_TO_INT(high, low);
}

void addUint32_t(uint32_t value)
{
  addUint16_t((uint16_t)(value & 0xFFFF));
  addUint16_t((uint16_t)(value >> 16 & 0xFFFF));
}

uint32_t removeUint32_t(void)
{
  uint16_t low = removeUint16_t();
  return (((uint32_t) removeUint16_t()) << 16) + low;
}

void addUint8_tArray(uint8_t length, uint8_t *contents)
{
  uint8_t* currentWriteCallbacksOffset = (uint8_t *)emGetBufferPointer(callbacksBuffer);
  currentWriteCallbacksOffset += writeCallbacksOffset;
  if (length && contents && currentWriteCallbacksOffset) {
    // The first byte is the length
    *currentWriteCallbacksOffset = length;
    // The byte string comes after the length
    MEMMOVE(currentWriteCallbacksOffset + sizeof(length), contents, length);
    writeCallbacksOffset += (length + sizeof(length));
    size += (length + sizeof(length));
  }
}

void addUint16_tArray(uint8_t length, uint16_t *contents)
{
  uint8_t i;
  for (i = 0; i < length; i++) {
    addUint16_t(contents[i]);
  }
}

void removeUint8_tArray(uint8_t* length, uint8_t *contents)
{
  uint8_t* currentReadCallbacksOffset = (uint8_t *)emGetBufferPointer(callbacksBuffer);
  currentReadCallbacksOffset += readCallbacksOffset;
  if (length && contents && currentReadCallbacksOffset) {
    *length = *currentReadCallbacksOffset;
    currentReadCallbacksOffset += sizeof(uint8_t);
    MEMMOVE(contents, currentReadCallbacksOffset, *length);
    readCallbacksOffset += (*length + sizeof(uint8_t));
    size -= (*length + sizeof(uint8_t));
  }
}

void removeUint16_tArray(uint8_t length, uint16_t *contents)
{
  uint8_t i;
  for (i = 0; i < length; i++) {
    contents[i] = removeUint16_t();
  }
}

bool callbacksPending(void)
{
  return (size != 0);
}

void processCallbacks(void)
{
  uint16_t callbackIdentifier = removeUint16_t();

  // Call generated file
  processGeneratedCallbacks(callbackIdentifier);
}

void emAfCallbacksFetchOrAppend(boolean fetch, PGM_P format, ...)
{
  char *c = (char *)format;
  PointerType ptr;
  va_list args;
  va_start(args, format);

  while (*c) {
    ptr = va_arg(args, PointerType);
    switch (*c) {
      case 's': {
        uint8_t *realPointer = (uint8_t *)ptr;
        if (fetch) {
          *realPointer = (uint8_t)removeUint8_t();
        } else {
          addUint8_t((uint8_t)*realPointer);
        }
      }
      break;
      case 'u': {
        uint8_t *realPointer = (uint8_t *)ptr;
        if (fetch) {
          *realPointer = removeUint8_t();
        } else {
          addUint8_t(*realPointer);
        }
      }
      break;
      case 'v': {
        uint16_t *realPointer = (uint16_t *)ptr;
        if (fetch) {
          *realPointer = removeUint16_t();
        } else {
          addUint16_t(*realPointer);
        }
      }
      break;
      case 'w': {
        uint32_t *realPointer = (uint32_t *)ptr;
        if (fetch) {
          *realPointer = removeUint32_t();
        } else {
          addUint32_t((uint32_t)*realPointer);
        }
      }
      break;
      case 'b': {
        uint8_t *realArray = (uint8_t *)ptr;
        uint8_t *lengthPointer = (uint8_t *)va_arg(args, PointerType);
        if (fetch) {
          removeUint8_tArray(lengthPointer, realArray);
        } else {
          addUint8_tArray(*lengthPointer, realArray);
        }
      }
      break;
      case 'p': {
        uint8_t **realPointer = (uint8_t **)ptr;
        uint8_t *lengthPointer = (uint8_t *)va_arg(args, PointerType);
        if (fetch) {
          *realPointer = removeUint8_tPointer(*lengthPointer);
        } else {
          ; // cannot add a pointer
        }
      }
      break;
      default:
        ; // meh
    }
    c++;
  }

  va_end(args);
}
