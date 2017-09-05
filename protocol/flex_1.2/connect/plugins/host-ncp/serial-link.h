// File: serial-link.h
//
// Description: Serial link high-level interface
//
// Copyright 2013 by Ember Corporation. All rights reserved.                *80*

// NCP Layering:
//
// 4: management.c: sending the various management commands
//    management-serial.c: serial imoutput sent as commands
//
// 3: uart-link-ip.c: converting between packets and buffers for data Tx and
//    Rx, plus a fixed FIFO buffer for sending commands.
//
// 2: serial-link.c (part 1): reading and writing data buffers
//
// 1: serial-link.c (part 2): reading and writing commands and parts of buffers
//
// 0: uart-link.c and spi-link.c: low-level serial I/O for blocks of data
//    with an associated type, either DATA or COMMAND.
//
// There are two other files that use serial-link.c:
//  uart-link-host-sim.c: A stub IP stack for simulated hosts.  This
//    implements the management side of uart-link-ip.c.  The data side
//    is at the top of the IP stack, implementing emReallySubmitIpHeader().
//  uart-link-test.c: unit test for sending and receiving blocks of
//    different sizes

// Host Layering:
//
// 2: management.c: sending the various management commands
//
// 1: stack/ip/host/management-client.c: forwarding the management
//    stream over a socket to the interface process.
//

#define MAX_FRAME_LENGTH 200 // For sanity-checking header frame length field

// The next buffer's worth of data message.
void uartLinkAddDataTx(Buffer buffer);

//----------------------------------------------------------------
// Upward callbacks called by serial-link.c.  These have three different
// implementations in:
//  uart-link-host-sim.c
//  uart-link-ip.c
//  uart-link-test.c:

// Tx ----------------

// Rx ----------------

//----------------------------------------------------------------
// serial-link.c housekeeping functions

void uartLinkInit(void);
void uartLinkMarkBuffers(void);

//----------------------------------------------------------------
// Simulation I/O defined in uart-link.c and uart-link-test.c, used in
// uart-link.c and uart-link-ip.c.  This is used to communicate directly
// with the command interpreter on the NCP in simulation tests.

bool simReadByte(uint8_t port, uint8_t *ch);
bool simWriteAvailable(uint8_t port);
void simWriteByte(uint8_t port, uint8_t ch);

void markUartLinkNcpBuffers(void);

void emReallySendManagementCommand(const uint8_t *command, uint16_t length);

void emSerialHandleRemoteReset(void);

// TODO: This needs to be looked at. Thread uses this so it has been put here to avoid warnings.
void emApiCounterHandler(EmberCounterType type, uint16_t increment);
