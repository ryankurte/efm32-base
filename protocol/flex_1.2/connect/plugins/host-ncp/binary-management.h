// File: binary-management-protocol.h
//
// Description: Description of the binary management protocol
//
// Copyright 2013 by Silicon Laboratories. All rights reserved.             *80*

// Binary message format:
//
//      byte 1     *    byte 2     *    bytes 3 and beyond
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
// +     command     identifier    | arguments ....
// +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
//
// A 2-byte command identifier, followed by a packed list of its arguments.
// See app/tmsp/tmsp-enum.h for a list of command
// identifiers. See the documentation for emSendBinaryManagementCommand() below
// for a description of possible packed arguments.

/** @brief Send a binary management command.
 *
 * @param identifier The command identifier
 * (see app/tmsp/tmsp-enum.h for a list)
 *
 * @param format The format string of the values starting in the third argument.
 * This string may contain any of the following values:
 *
 * u: one byte, uint8_t
 * s: one byte, int8_t
 * v: two bytes, uint16_t
 * w: four bytes, uint32_t
 * b: const uint8_t * array, followed by an uint8_t length
 * (two arguments are needed)
 *
 * Example: ubv
 * The arguments are: an uint8_t, const uint8_t * array, array length and uint16_t.
 *
 * @param ... The actual values specified in format.
 */
void emSendBinaryManagementCommand(uint16_t identifier,
                                   const char *format,
                                   ...);

void emSendBinaryManagementBlockingCommand(uint16_t identifier,
                                           const char *format,
                                           ...);

uint16_t emFormatBinaryManagementCommand(uint8_t *buffer,
                                         uint16_t bufferSize,
                                         uint16_t identifier,
                                         const char *format,
                                         ...);

// interface to get a command identifier string
const char *emGetBinaryCommandIdentifierString(uint16_t identifier);
