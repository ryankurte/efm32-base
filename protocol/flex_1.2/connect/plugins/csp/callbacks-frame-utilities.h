/**************************************************************************//**
 * Copyright 2017 Silicon Laboratories, Inc.
 *
 *****************************************************************************/
//
// Description: Functions for reading and writing callbacks frames.

#ifndef __CSP_CALLBACKS_FRAME_UTILITIES_H__
#define __CSP_CALLBACKS_FRAME_UTILITIES_H__

#define CSP_CALLBACKS_BUFFER_SIZE 1000

void initializeCallbacksBuffers(void);
void markCallbacksBuffers(void);

uint8_t removeUint8_t(void);
uint8_t *removeUint8_tPointer(uint8_t length);
void addInt8_t(int8_t value);
void addUint8_t(uint8_t value);
uint16_t removeUint16_t(void);
void addUint16_t(uint16_t value);

void addUint32_t(uint32_t value);
uint32_t removeUint32_t(void);

void addUint8_tArray(uint8_t length, uint8_t *contents);
void removeUint8_tArray(uint8_t* length, uint8_t *contents);

void addUint16_tArray(uint8_t length, uint16_t *contents);
void removeUint16_tArray(uint8_t length, uint16_t *contents);

bool callbacksPending(void);
void processCallbacks(void);

// Format the same as in command-interpreter2.h, with the addition of
// a 'p' for a pointer.
// Make sure to pass references to all of the variables! And if an array is
// passed, the length needs to come directly after it. This length should
// be a reference as well.
void emAfCallbacksFetchOrAppend(boolean remove, PGM_P format, ...);

#endif // __CSP_CALLBACKS_FRAME_UTILITIES_H__
