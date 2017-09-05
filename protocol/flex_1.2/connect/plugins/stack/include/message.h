/**
 * @file message.h
 * @brief Ember Connect APIs and handlers for sending and receiving messages.
 * See @ref message for documentation.
 *
 * <!--Copyright 2015 by Silicon Labs. All rights reserved.              *80*-->
 */

/**
 * @addtogroup message
 * @brief Ember Connect APIs sending and receiving messages.
 *
 * See message.h for source code.
 * @{
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

/** @brief The maximum length in bytes of the application payload for an
 * unsecured message.
 */
#define EMBER_MAX_UNSECURED_APPLICATION_PAYLOAD_LENGTH     107

/** @brief The maximum length in bytes of the application payload for a
 * secured message.
 */
#define EMBER_MAX_SECURED_APPLICATION_PAYLOAD_LENGTH       98

/** @brief The maximum length in bytes of the MAC payload for an unsecured
 * message. Notice that this define is only meaningful for MAC mode devices
 * (see ::EMBER_MAC_MODE_DEVICE).
 */
#define EMBER_MAX_UNSECURED_MAC_PAYLOAD_LENGTH             112

/** @brief The maximum length in bytes of the MAC payload for a secured message.
 * Notice that this define is only meaningful for MAC mode devices (see
 * ::EMBER_MAC_MODE_DEVICE).
 */
#define EMBER_MAX_SECURED_MAC_PAYLOAD_LENGTH               103

/** @brief The maximum allowed endpoint value.
 */
#define EMBER_MAX_ENDPOINT                                 0xF

/** @name Handlers*/
//@{

/** @brief A callback invoked when the stack has completed sending a message.
 *
 * @param status   An ::EmberStatus value of:
 * - ::EMBER_SUCCESS if an ACK was received from the destination or no ACK was
 * requested.
 * - ::EMBER_MAC_NO_ACK_RECEIVED if an ACK was requested and no ACK was
 * received.
 * - ::EMBER_MAC_INDIRECT_TIMEOUT is the destination is a sleepy node and the
 * packet timed-out before the sleepy node sent a data request.
 * - ::EMBER_MAC_INDIRECT_MESSAGE_PURGED is the destination is a sleepy node
 *  and it was removed from the child table while the packet was stored in the
 *  indirect queue.
 * - ::EMBER_PHY_TX_CCA_FAIL if the node failed all the clear channel assessment
 * attempts.
 * - ::EMBER_PHY_TX_INCOMPLETE if the transmission was not completed correctly.
 *
 * @param message  An ::EmberOutgoingMessage describing the outgoing packet.
 */
void emberMessageSentHandler(EmberStatus status, EmberOutgoingMessage *message);

/** @brief A callback invoked when a packet has been received.
 *
 * @param message  An ::EmberIncomingMessage describing the incoming packet.
 */
void emberIncomingMessageHandler(EmberIncomingMessage *message);

//@}//END Handlers

/** @name APIs*/
//@{

/** @brief Sends a message to the passed destination short ID.
 *
 * @param destination The destination node short ID.
 *
 * @param endpoint        The destination endpoint of the outgoing message. This
 *                        value can not exceed ::EMBER_MAX_ENDPOINT.
 *
 * @param messageTag      A value chosen by the application. This value will be
 *                        passed in the corresponding
 *                        ::emberMessageSentHandler() call.
 *
 * @param messageLength The size in bytes of the message payload. Please refer
 * to ::EMBER_MAX_UNSECURED_APPLICATION_PAYLOAD_LENGTH or
 * ::EMBER_MAX_SECURED_APPLICATION_PAYLOAD_LENGTH for the maximum message length
 * allowed (::EMBER_MAC_MODE_DEVICE devices should instead refer to
 * ::EMBER_MAX_UNSECURED_MAC_PAYLOAD_LENGTH and
 * ::EMBER_MAX_SECURED_MAC_PAYLOAD_LENGTH).
 *
 * @param message A pointer to an array of bytes containing the message payload.
 *
 * @param options  Specifies the ::EmberMessageOptions for the outgoing message.
 *
 * @return an ::EmberStatus value of:
 * - ::EMBER_SUCCESS if the message was accepted by the stack. If a success
 * status is returned, the ::emberMessageSentHandler() callback will be invoked
 * by the stack to indicate whether the message was successfully delivered or
 * the reason of failure.
 * - ::EMBER_INVALID_CALL if the node is not joined to a network, the packet
 * length is 0, the passed TX options indicates some feature feature that is not
 * supported or the passed endpoint exceeds ::EMBER_MAX_ENDPOINT.
 * - ::EMBER_MESSAGE_TOO_LONG if the message does not fit in a single frame;
 * - ::EMBER_PHY_TX_BUSY if the message cannot be sent since the node does not
 * support MAC queuing and the radio is currently busy.
 * - ::EMBER_MAC_TRANSMIT_QUEUE_FULL if the outgoing MAC queue is currently
 * full.
 * - ::EMBER_NO_BUFFERS if the stack could not allocate enough RAM to store the
 * submitted message.
 * - ::EMBER_MAC_UNKNOWN_DESTINATION if the node is part of a star network and
 * the destination node does not appear in the node's routing table.
 * - ::EMBER_MAC_SECURITY_NOT_SUPPORTED if the message was requested to be sent
 * out secured and either the local node does not support security or the
 * destination node is known to not support security.
 * - ::EMBER_MAC_BUSY if the message was not accepted because the MAC is
 * currently performing some critical operation.
 */
EmberStatus emberMessageSend(EmberNodeId destination,
                             uint8_t endpoint,
                             uint8_t messageTag,
                             EmberMessageLength messageLength,
                             uint8_t *message,
                             EmberMessageOptions options);

/** @brief Sends a data request command to the parent node.
 *
 * @return and ::EmberStatus value of ::EMBER_SUCCESS if the data poll was sent
 * out successfully, a value of ::EMBER_INVALID_CALL if the node is not joined
 * to a network, or the node is not an end device, a value of ::EMBER_MAC_BUSY
 * if the MAC is currently performing some critical operation.
 */
EmberStatus emberPollForData(void);

//@}//END APIs

#endif //__MESSAGE_H__

/** @} // END addtogroup
 */
