// File: ezsp-protocol.h
//
// Description: Definitions useful for creating and decoding EZSP frames. The
// frame format is defined in the EM260 datasheet and looks like this:
//   <sequence:1>
//   <frame control:1>
//   <frame ID:1>
//   <parameters:0-120>
//
// Copyright 2007-2010 by Ember Corporation. All rights reserved.           *80*

#ifndef __EZSP_PROTOCOL_H__
#define __EZSP_PROTOCOL_H__

//------------------------------------------------------------------------------
// Protocol Definitions

#define EZSP_PROTOCOL_VERSION 0x04

#define EZSP_MIN_FRAME_LENGTH     3
#define EZSP_MAX_FRAME_LENGTH     200
#define EZSP_SEQUENCE_INDEX       0
#define EZSP_FRAME_CONTROL_INDEX  1
#define EZSP_FRAME_ID_INDEX       2
#define EZSP_PARAMETERS_INDEX     3

#define EZSP_STACK_TYPE_MESH 0x02

#include "ezsp-enum.h"

//------------------------------------------------------------------------------
// Frame Control Definitions

// The high bit of the frame control byte indicates the direction of the
// message. Commands are sent from the Host to the EM260. Responses are sent
// from the EM260 to the Host.
#define EZSP_FRAME_CONTROL_DIRECTION_MASK 0x80
#define EZSP_FRAME_CONTROL_COMMAND        0x00
#define EZSP_FRAME_CONTROL_RESPONSE       0x80

// Bits 5 and 6 of the frame control byte carry the network index the ezsp
// message is related to. The NCP upon processing an incoming EZSP command,
// temporary switches the current network to the one indicated in the EZSP
// frame control.
#define EZSP_FRAME_CONTROL_NETWORK_INDEX_MASK     0x60
#define EZSP_FRAME_CONTROL_NETWORK_INDEX_OFFSET   5

// Command Frame Control Fields

// The EM260 enters the sleep mode specified by the command frame control once
// it has sent its response.
#define EZSP_FRAME_CONTROL_SLEEP_MODE_MASK 0x03
#define EZSP_FRAME_CONTROL_IDLE            0x00 // Processor idle.
#define EZSP_FRAME_CONTROL_DEEP_SLEEP      0x01 // Wake on interrupt or timer.
#define EZSP_FRAME_CONTROL_POWER_DOWN      0x02 // Wake on interrupt only.
#define EZSP_FRAME_CONTROL_RESERVED_SLEEP  0x03 // Reserved

// Response Frame Control Fields

// The overflow flag in the response frame control indicates to the Host that
// one or more callbacks occurred since the previous response and there was not
// enough memory available to report them to the Host.
#define EZSP_FRAME_CONTROL_OVERFLOW_MASK  0x01
#define EZSP_FRAME_CONTROL_NO_OVERFLOW    0x00
#define EZSP_FRAME_CONTROL_OVERFLOW       0x01

// The truncated flag in the response frame control indicates to the Host that
// the response has been truncated. This will happen if there is not enough
// memory available to complete the response or if the response would have
// exceeded the maximum EZSP frame length.
#define EZSP_FRAME_CONTROL_TRUNCATED_MASK 0x02
#define EZSP_FRAME_CONTROL_NOT_TRUNCATED  0x00
#define EZSP_FRAME_CONTROL_TRUNCATED      0x02

// The pending callbacks flag in the response frame control byte indicates to
// the Host that there is at least one callback ready to be read. This flag is
// clear if the response to a callback command read the last pending callback.
#define EZSP_FRAME_CONTROL_PENDING_CB_MASK 0x04
#define EZSP_FRAME_CONTROL_PENDING_CB     0x04
#define EZSP_FRAME_CONTROL_NO_PENDING_CB  0x00

// The synchronous callback flag in the response frame control byte indicates
// this ezsp frame is the response to an ezspCallback().
#define EZSP_FRAME_CONTROL_SYNCH_CB_MASK  0x08
#define EZSP_FRAME_CONTROL_SYNCH_CB       0x08
#define EZSP_FRAME_CONTROL_NOT_SYNCH_CB   0x00

// The asynchronous callback flag in the response frame control byte indicates
// this ezsp frame is a callback sent asynchronously by the ncp. This flag may
// be set only in the uart version when EZSP_VALUE_UART_SYNCH_CALLBACKS is 0.
#define EZSP_FRAME_CONTROL_ASYNCH_CB_MASK 0x10
#define EZSP_FRAME_CONTROL_ASYNCH_CB      0x10
#define EZSP_FRAME_CONTROL_NOT_ASYNCH_CB  0x00

#endif // __EZSP_PROTOCOL_H__
