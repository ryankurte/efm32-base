// Copyright 2015 Silicon Laboratories, Inc.

#ifndef _MAILBOX_CLIENT_H_
#define _MAILBOX_CLIENT_H_

#include "mailbox/mailbox-protocol.h"
#include "mailbox/mailbox-types.h"

/**
 * @addtogroup mailbox_client
 *
 * See mailbox-client.h for source code.
 * @{
 */

/** @brief An API for submitting a data message to a mailbox server. If this
 * API returns an ::EmberAfMailboxStatus value of
 * ::EMBER_MAILBOX_STATUS_SUCCESS, the corresponding asynchronous callback
 * ::emberAfPluginMailboxClientMessageSubmitCallback() will be invoked to
 * indicate whether the message was successfully submitted to the mailbox
 * server or to inform the application of the reason of failure.
 *
 *  @param mailboxServer   The mailbox server short ID.
 *
 *  @param messageDestination   The data message destination short ID.
 *
 *  @param message   A pointer to the message to be sent.
 *
 *  @param messageLength   The length in bytes of the message to be sent.
 *
 *  @param tag   A tag value the application can use to match asynchronous
 *  callbacks.
 *
 *  @param useSecurity   A flag indicating whether the data message should be
 *  sent to the server using security or not.
 *
 *  @return   An ::EmberAfMailboxStatus value of:
 *  - ::EMBER_MAILBOX_STATUS_SUCCESS if the message was successfully passed to
 *    the network layer to be transmitted to the mailbox server.
 *  - ::EMBER_MAILBOX_STATUS_INVALID_CALL if the passed data message is
 *    invalid,
 *  - ::EMBER_MAILBOX_STATUS_INVALID_ADDRESS if the server ID or the
 *    destination ID is an invalid address.
 *  - ::EMBER_MAILBOX_STATUS_MESSAGE_TOO_LONG if the passed message would not
 *    fit in a single mailbox data message.
 *  - ::EMBER_MAILBOX_STATUS_BUSY if the client is still performing a submit
 *    message or a query for message action.
 *  - ::EMBER_MAILBOX_STATUS_STACK_ERROR if the network layer refused the
 *    message (the outgoing queue is currently full).
 */
EmberAfMailboxStatus emberAfPluginMailboxClientMessageSubmit(EmberNodeId mailboxServer,
                                                             EmberNodeId messageDestination,
                                                             uint8_t *message,
                                                             EmberMessageLength messageLength,
                                                             uint8_t tag,
                                                             bool useSecurity);

/** @brief An API for querying a mailbox server for pending messages. If this
 * API returns an ::EmberAfMailboxStatus value of ::EMBER_MAILBOX_STATUS_SUCCESS
 * the corresponding asynchronous callback
 * ::emberAfPluginMailboxClientCheckInboxCallback() shall be invoked either to
 * provide the retrieved message or to indicate the reason of failure.
 *
 *  @param mailboxServer   The mailbox server short ID to be queried.
 *
 *  @param useSecurity    A flag indicating whether the request command should
 *  be sent secured or not. Notice that secured pending messages can only be
 *  retrieved performing a secured query, while unsecured pending messages can
 *  be retrieved using both a unsecured or a secured query.
 *
 *  @return An ::EmberAfMailboxStatus value of:
 *  - ::EMBER_MAILBOX_STATUS_SUCCESS if the query command was successfully
 *    passed to the network layer to be transmitted to the mailbox server.
 *  - ::EMBER_MAILBOX_STATUS_INVALID_ADDRESS if the passed mailbox server short
 *    ID is an invalid address.
 *  - ::EMBER_MAILBOX_STATUS_BUSY if the client is still performing a submit
 *    message or a query for message action.
 *  - ::EMBER_MAILBOX_STATUS_STACK_ERROR if the network layer refused the
 *    command (the outgoing queue is currently full).
 */
EmberAfMailboxStatus emberAfPluginMailboxClientCheckInbox(EmberNodeId mailboxServer,
                                                          bool useSecurity);

/** @brief Mailbox Client Message Submit Callback
 *
 * A callback that is invoked after a successful
 * ::emberAfPluginMailboxClientMessageSubmit() API call.
 *
 *  @param status   An ::EmberAfMailboxStatus value of:
 *  - ::EMBER_MAILBOX_STATUS_SUCCESS if the data message was accepted by the
 *  mailbox server.
 *  - ::EMBER_MAILBOX_STATUS_STACK_ERROR if the message couldn't be delivered
 *  to the mailbox server.
 *  - ::EMBER_MAILBOX_STATUS_MESSAGE_NO_RESPONSE if the client timed-out
 *  waiting for a response from the server.
 *  - ::EMBER_MAILBOX_STATUS_MESSAGE_TABLE_FULL if the mailbox server table is
 *  currently full.
 *  - ::EMBER_MAILBOX_STATUS_MESSAGE_NO_BUFFERS if the server could not
 *  allocate enough memory to store the message.
 *
 *  @param mailboxServer   The mailbox server short ID.
 *
 *  @param messageDestination   The message destination short ID.
 *
 *  @param tag   The tag value passed in the
 *  ::emberAfPluginMailboxClientMessageSubmit() API.
 */
void emberAfPluginMailboxClientMessageSubmitCallback(EmberAfMailboxStatus status,
                                                     EmberNodeId mailboxServer,
                                                     EmberNodeId messageDestination,
                                                     uint8_t tag);

/** @brief Mailbox Client Message Delivered Callback
 *
 * A callback that might be invoked if the a message that was submitted
 * to a mailbox server has reached its final destination or it timed-out.
 * Notice that this callback is not guaranteed to be called, if the status
 * message from the server is lost, this callback won't be called.
 *
 * @param status    An ::EmberAfMailboxStatus value of:
 * - ::EMBER_MAILBOX_STATUS_SUCCESS indicates that the message was successfully
 *   delivered to the final destination.
 * - ::EMBER_MAILBOX_STATUS_MESSAGE_TIMED_OUT indicates that the message
 *   timed-out and was removed from the server queue.
 *
 * @param mailboxServer   The mailbox server short ID the message was submitted
 * to.
 *
 * @param messageDestination   The message destination.
 *
 * @param tag   The message tag.
 */
void emberAfPluginMailboxClientMessageDeliveredCallback(EmberAfMailboxStatus status,
                                                        EmberNodeId mailboxServer,
                                                        EmberNodeId messageDestination,
                                                        uint8_t tag);

/** @brief Mailbox Client Check Inbox Callback
 *
 * This callback is invoked asynchronously after a successful call to the
 * ::emberAfPluginMailboxClientCheckInbox() API. If a message was retrieved
 * from the mailbox server, this callback passes it to the application,
 * otherwise it indicates to the application the reason of failure.
 *
 *  @param status    An ::EmberAfMailboxStatus value of:
 * - ::EMBER_MAILBOX_STATUS_SUCCESS if a message was retrieved from the mailbox
 * server.
 * - ::EMBER_MAILBOX_STATUS_MESSAGE_NO_DATA if the server has currently no
 * message for this mailbox client.
 * - ::EMBER_MAILBOX_STATUS_MESSAGE_NO_RESPONSE if the client timed-out waiting
 * for a query response from the mailbox server.
 * - ::EMBER_MAILBOX_STATUS_STACK_ERROR if the stack failed to deliver the
 * query message to the mailbox server.
 *
 *  @param messageSource   The source node address of the retrieved message.
 *  Notice that this parameter is meaningful only if the status parameter has
 *  an ::EmberAfMailboxStatus value of ::EMBER_MAILBOX_STATUS_SUCCESS.
 *
 *  @param message   A pointer to the retrieved message payload. Notice that
 *  this parameter is meaningful only if the status parameter has an
 *  ::EmberAfMailboxStatus value of ::EMBER_MAILBOX_STATUS_SUCCESS.
 *
 *  @param messageLength   The length in bytes of the retrived message payload.
 *  Notice that this parameter is meaningful only if the status parameter has
 *  an ::EmberAfMailboxStatus value of ::EMBER_MAILBOX_STATUS_SUCCESS.
 *
 *  @param tag   The tag value used by the source mailbox client.
 *  Notice that this parameter is meaningful only if the status parameter has
 *  an ::EmberAfMailboxStatus value of ::EMBER_MAILBOX_STATUS_SUCCESS.
 *
 *  @param moreMessages   This flag indicates whether the mailbox server has
 *  more pending messages for this mailbox client. Notice that this parameter
 *  is meaningful only if the status parameter has an ::EmberAfMailboxStatus
 *  value of ::EMBER_MAILBOX_STATUS_SUCCESS.
 */
void emberAfPluginMailboxClientCheckInboxCallback(EmberAfMailboxStatus status,
                                                  EmberNodeId mailboxServer,
                                                  EmberNodeId messageSource,
                                                  uint8_t *message,
                                                  EmberMessageLength messageLength,
                                                  uint8_t tag,
                                                  bool moreMessages);

/** @} // END addtogroup
 */

#endif // _MAILBOX_CLIENT_H_
