// ****************************************************************************
// * bootload-xmodem.h
// *
// * Routines for sending data via xmodem
// *
// * Copyright 2015 Silicon Laboratories, Inc.
// *****************************************************************************

// Initialize xmodem state machine for a new transfer
// If startImmediately is set, will not wait for an initial 'C' character
//   to be received before sending the first block of data
void emInitXmodemState(bool startImmediately);

// Send a chunk of data via xmodem.  Arbitrary lengths of data may be passed,
//   it will be broken up into appropriate sized chunks for transmission. Xmodem
//   itself transfers data in 128 byte chunks
// Note: This function will block for the duration of time required to send
//   the data that is passed in.
// The "finished" flag should be set when called with the final chunk to be
//   transferred to terminate the transfer properly
bool emSendXmodemData(const uint8_t *data, uint16_t length, bool finished);
