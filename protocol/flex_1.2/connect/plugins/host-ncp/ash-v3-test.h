// File: ash-v3-test.h
//
// Description: Functions to test the Host and NCP setup.
//
// Copyright 2016 by Silicon Laboratories. All rights reserved.             *80*

/**
 * Test command. Echo data to the NCP.
 */
void emberEcho(uint8_t *data, uint8_t length);
void emberEchoReturn(uint8_t *data, uint8_t length);

/**
 * Test command. Software flow control.
 */
void emberStartXOnXOffTest(void);
