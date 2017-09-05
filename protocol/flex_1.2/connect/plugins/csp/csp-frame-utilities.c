// File: csp-frame-utilities.h
//
// Description: Functions for reading and writing command and response frames.
//
// Copyright 2015 by Ember Corporation. All rights reserved.                *80*

#include PLATFORM_HEADER

#include "stack/include/ember-types.h"
#include "csp-frame-utilities.h"
#ifdef UNIX_HOST
#include "host-ncp/host-buffer-management.h"
#else
#include "stack/framework/buffer-management.h"
#endif

static uint8_t cspBuffer[CSP_BUFFER_SIZE];
static uint8_t *readPointer;
static uint8_t *writePointer;

bool waitingForResponse = false;

void initializeCspBuffers(void)
{
  readPointer = cspBuffer;
  writePointer = cspBuffer;
}

uint8_t fetchUint8_t(void)
{
  uint8_t value = (uint8_t) *readPointer;
  readPointer++;
  return value;
}

uint8_t *fetchUint8_tPointer(uint8_t length)
{
  uint8_t *result = readPointer;
  readPointer += length;
  return result;
}

void appendUint8_t(uint8_t value)
{
  *writePointer = value;
  writePointer++;
}

void appendInt8_t(int8_t value)
{
  *writePointer = value;
  writePointer++;
}

void appendUint16_t(uint16_t value)
{
  appendUint8_t(LOW_BYTE(value));
  appendUint8_t(HIGH_BYTE(value));
}

uint16_t fetchUint16_t(void)
{
  uint8_t low = fetchUint8_t();
  uint8_t high = fetchUint8_t();
  return HIGH_LOW_TO_INT(high, low);
}

void appendUint32_t(uint32_t value)
{
  appendUint16_t((uint16_t)(value & 0xFFFF));
  appendUint16_t((uint16_t)(value >> 16 & 0xFFFF));
}

uint32_t fetchUint32_t(void)
{
  uint16_t low = fetchUint16_t();
  return (((uint32_t) fetchUint16_t()) << 16) + low;
}

void appendUint8_tArray(uint8_t length, uint8_t *contents)
{
  MEMMOVE(writePointer, contents, length);
  writePointer += length;
}

void appendUint16_tArray(uint8_t length, uint16_t *contents)
{
  uint8_t i;
  for (i = 0; i < length; i++) {
    appendUint16_t(contents[i]);
  }
}

void fetchUint8_tArray(uint8_t length, uint8_t *contents)
{
  MEMMOVE(contents, readPointer, length);
  readPointer += length;
}

void fetchUint16_tArray(uint8_t length, uint16_t *contents)
{
  uint8_t i;
  for (i = 0; i < length; i++) {
    contents[i] = fetchUint16_t();
  }
}

void emAfFetchOrAppend(boolean fetch, PGM_P format, ...)
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
          *realPointer = (uint8_t)fetchUint8_t();
        } else {
          appendUint8_t((uint8_t)*realPointer);
        }
      }
      break;
      case 'u': {
        uint8_t *realPointer = (uint8_t *)ptr;
        if (fetch) {
          *realPointer = fetchUint8_t();
        } else {
          appendUint8_t(*realPointer);
        }
      }
      break;
      case 'v': {
        uint16_t *realPointer = (uint16_t *)ptr;
        if (fetch) {
          *realPointer = fetchUint16_t();
        } else {
          appendUint16_t(*realPointer);
        }
      }
      break;
      case 'w': {
        uint32_t *realPointer = (uint32_t *)ptr;
        if (fetch) {
          *realPointer = fetchUint32_t();
        } else {
          appendUint32_t((uint32_t)*realPointer);
        }
      }
      break;
      case 'b': {
        uint8_t *realArray = (uint8_t *)ptr;
        uint8_t *lengthPointer = (uint8_t *)va_arg(args, PointerType);
        if (fetch) {
          fetchUint8_tArray(*lengthPointer, realArray);
        } else {
          appendUint8_tArray(*lengthPointer, realArray);
        }
      }
      break;
      case 'p': {
        uint8_t **realPointer = (uint8_t **)ptr;
        uint8_t *lengthPointer = (uint8_t *)va_arg(args, PointerType);
        if (fetch) {
          *realPointer = fetchUint8_tPointer(*lengthPointer);
        } else {
          ; // cannot append a pointer
        }
      }
      break;
      default:
        ; // meh
    }
    c++;
  }

  if (!fetch) {
    waitingForResponse = false;
  }

  va_end(args);
}
