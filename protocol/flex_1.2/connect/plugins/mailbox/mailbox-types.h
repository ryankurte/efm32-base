// Copyright 2015 Silicon Laboratories, Inc.

#ifndef _MAILBOX_TYPES_H_
#define _MAILBOX_TYPES_H_

/**
 * @addtogroup mailbox_common
 * @{
 */

/**
 * @brief Mailbox return status codes.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberAfMailboxStatus
#else
typedef uint8_t EmberAfMailboxStatus;
enum
#endif
{
  EMBER_MAILBOX_STATUS_SUCCESS             = 0x00,
  EMBER_MAILBOX_STATUS_INVALID_CALL        = 0x01,
  EMBER_MAILBOX_STATUS_BUSY                = 0x02,
  EMBER_MAILBOX_STATUS_STACK_ERROR         = 0x03,
  EMBER_MAILBOX_STATUS_INVALID_ADDRESS     = 0x04,
  EMBER_MAILBOX_STATUS_MESSAGE_TOO_LONG    = 0x05,
  EMBER_MAILBOX_STATUS_MESSAGE_TABLE_FULL  = 0x06,
  EMBER_MAILBOX_STATUS_MESSAGE_NO_BUFFERS  = 0x07,
  EMBER_MAILBOX_STATUS_MESSAGE_NO_RESPONSE = 0x08,
  EMBER_MAILBOX_STATUS_MESSAGE_TIMED_OUT   = 0x09,
  EMBER_MAILBOX_STATUS_MESSAGE_NO_DATA     = 0x0A,
};

/** @} // END addtogroup
 */

#endif // _MAILBOX_TYPES_H_
