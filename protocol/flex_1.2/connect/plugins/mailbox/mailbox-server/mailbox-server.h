// Copyright 2015 Silicon Laboratories, Inc.

#ifndef _MAILBOX_SERVER_H_
#define _MAILBOX_SERVER_H_

#include "mailbox/mailbox-protocol.h"
#include "mailbox/mailbox-types.h"

/**
 * @addtogroup mailbox_server
 *
 * See mailbox-server.h for source code.
 * @{
 */

/** @brief Add a message to the mailbox queue. The message will be stored in the
 * internal queue until the destination node queries the mailbox server node for
 * messages or upon timeout.
 *
 *  @param destination   An ::EmberNodeId value indicating the address of the
 *  message's destination node.
 *
 *  @param message    A pointer to the message to be enqueued. Notice that if
 *  the allocation memory plugin option is enabled, the packet content will be
 *  copied in the internal plugin data structures. Otherwise the plugin will
 *  simply store the pointer: in this case the application is responsible to
 *  ensure that this pointer is valid until the message is delivered to the
 *  final destination or upon timeout.
 *
 *  @param messageLength    The length in bytes of the message to be enqueued.
 *
 *  @param tag    A tag value for the passed message.
 *
 *  @param useSecurity   Indicates whether the message should be sent secured
 *  or not.
 *
 *  @return An ::EmberAfMailboxStatus value of:
 *  - ::EMBER_MAILBOX_STATUS_SUCCESS if the message was successfully added to
 *    the packet queue.
 *  - ::EMBER_MAILBOX_STATUS_INVALID_CALL if the passed message is invalid.
 *  - ::EMBER_MAILBOX_STATUS_INVALID_ADDRESS if the passed destination address
 *    is invalid.
 *  - ::EMBER_MAILBOX_STATUS_MESSAGE_TOO_LONG if the payload size of the passed
 *    message exceeds the maximum allowable payload for the passed transmission
 *    options.
 *  - ::EMBER_MAILBOX_STATUS_MESSAGE_TABLE_FULL if the packet table is already
 *    full.
 *  - ::EMBER_MAILBOX_STATUS_MESSAGE_NO_BUFFERS if not enough memory buffers are
 *    available for storing the message content.
 */
EmberAfMailboxStatus emberAfPluginMailboxServerAddMessage(EmberNodeId destination,
                                                          uint8_t *message,
                                                          EmberMessageLength messageLength,
                                                          uint8_t tag,
                                                          bool useSecurity);

/** @brief Mailbox Server Message Delivered Callback
 *
 * This callback is invoked at the server when a message submitted locally by
 * the server was successfully delivered or timed-out.
 *
 * @param status    An ::EmberAfMailboxStatus value of:
 * - ::EMBER_MAILBOX_STATUS_SUCCESS indicates that the message was successfully
 *   delivered to the final destination.
 * - ::EMBER_MAILBOX_STATUS_MESSAGE_TIMED_OUT indicates that the message
 *   timed-out and was removed from the server queue.
 *
 * @param messageDestination   The message destination.
 *
 * @param tag   The message tag.
 */
void emberAfPluginMailboxServerMessageDeliveredCallback(EmberAfMailboxStatus status,
                                                        EmberNodeId messageDestination,
                                                        uint8_t tag);

/** @} // END addtogroup
 */

#endif //_MAILBOX_SERVER_H_
