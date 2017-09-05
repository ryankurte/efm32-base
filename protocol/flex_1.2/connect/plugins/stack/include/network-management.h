/**
 * @file network-management.h
 * See @ref network_management for documentation.
 *
 * <!--Copyright 2014 by Silicon Labs. All rights reserved.              *80*-->
 */

/**
 * @addtogroup network_management
 * @brief Ember Connect API for finding, forming, joining, and leaving Connect
 * networks.
 *
 * See network-management.h for source code.
 * @{
 */

#ifndef __NETWORK_FORMATION_H__
#define __NETWORK_FORMATION_H__

/** @brief The maximum length in bytes of the application beacon payload.
 */
#define EMBER_MAC_MAX_APP_BEACON_PAYLOAD_LENGTH     16

/** @name Handlers*/
//@{

/** @brief This stack handler is invoked if a beacon is received during the
 *  scanning procedure, if this was initiated by the application with the
 *  ::emberStartActiveScan stack APIs.
 *
 * @param panId   The source pan ID of the received beacon.
 *
 * @param nodeId  The source node ID of the received beacon.
 *
 * @param payloadLength  The length in bytes of the application beacon payload
 *   of the received beacon.
 *
 * @param payload   A pointer to the application beacon payload of the received
 *   beacon.
 */
void emberIncomingBeaconHandler(EmberPanId panId,
                                EmberNodeId nodeId,
                                uint8_t payloadLength,
                                uint8_t *payload);

/** @brief This stack handler is invoked after the application has called the
 *  ::emberStartActiveScan stack API to inform the application that the scanning
 *  procedure has completed.
 */
void emberActiveScanCompleteHandler(void);

/** @brief This stack handler is invoked after the application has called the
 *  ::emberStartEnergyScan stack API to inform the application that the energy
 *  scan procedure has completed and to provide statistics.
 *
 *  params mean        The average energy detected.
 *  params min         The minimum energy detected.
 *  params max         The maximum energy detected.
 *  params variance    The variance of the energy detected.
 */
void emberEnergyScanCompleteHandler(int8_t mean,
                                    int8_t min,
                                    int8_t max,
                                    uint16_t variance);

/** @brief This stack handler is invoked after the application has called the
 *  ::emberFrequencyHoppingStartClient stack API to inform the application that
 * the synchronization process with the server has completed.
 * Once the client is synched to a server, it may seamlessly perform the
 * resynchronization process if needed. Sleepy devices in particular
 * periodically perform the resynchronization process. If the client fails a
 * resynchronization process, it will inform the application by invoking the
 * ::emberStackStatusHandler handler with EMBER_MAC_SYNC_TIMEOUT status.
 * When this occurs, the client will be no longer synched to the server.
 *
 *  params status     An ::EmberStatus value indicating whether the
 *   synchronization process with the server was completed successfully or the
 *   reason of failure.
 */
void emberFrequencyHoppingStartClientCompleteHandler(EmberStatus status);

//@}//END Handlers

/** @name APIs*/
//@{

/** @brief Initializes the radio and the Ember stack.
 *
 * Device configuration functions must be called before ::emberInit()
 * is called.
 *
 * @note The application must check the return value of this function. If the
 * initialization fails, normal messaging functions will not be available.
 * Some failure modes are not fatal, but the application must follow certain
 * procedures to permit recovery.
 * Ignoring the return code will result in unpredictable radio and API behavior.
 * (In particular, problems with association will occur.)
 *
 * @return An ::EmberStatus value indicating successful initialization or the
 *   reason for failure.
 */
EmberStatus emberInit(void);

/** @brief A periodic tick routine that should be called:
 * - in the application's main event loop,
 * - as soon as possible after any radio interrupts, and
 * - after ::emberInit().
 */
void emberTick(void);

/** @brief Resume network operation after a reboot.
 *
 * It is required that this be called on boot prior to ANY network operations.
 * This will initialize the networking system and attempt to resume the
 * previous network identity and configuration.  If the node was not previously
 * joined, this routine should still be called.
 *
 * If the node was previously joined to a network it will retain its original
 * type (e.g. coordinator, router, end device, etc.)
 *
 * ::EMBER_NOT_JOINED is returned if the node is not part of a network.
 *
 * @return An ::EmberStatus value that indicates one of the following:
 *   - successful initialization,
 *   - ::EMBER_NOT_JOINED if the node is not part of a network, or
 *   - the reason for failure.
 */
EmberStatus emberNetworkInit(void);

/** @brief This function will start an active scan. ::EMBER_SUCCESS signals that
 * the scan successfully started.  Note that while a scan can be initiated
 * while the node is currently joined to a network, the node will generally
 * be unable to communicate with its PAN during the scan period, so care
 * should be taken when performing scans of any significant duration while
 * presently joined to an existing PAN. Upon receiving a beacon, the
 * ::emberIncomingBeaconHandler stack handler shall be called. At the end of
 * the scanning procedure the ::emberActiveScanCompleteHandler stack handler
 * shall be called.
 *
 * Possible error responses and their meanings:
 * - ::EMBER_INVALID_CALL, the node is currently frequency hopping.
 * - ::EMBER_MAC_SCANNING, we are already scanning.
 * - ::EMBER_PHY_INVALID_CHANNEL, the specified channel is not a valid channel
 *   on the current platform.
 *
 * @param channel  The channel to scan.
 */
EmberStatus emberStartActiveScan(uint8_t channel);

/** @brief This function will kick off an energy scan. ::EMBER_SUCCESS signals
 * that the scan successfully started.  Note that while a scan can be initiated
 * while the node is currently joined to a network, the node will generally
 * be unable to communicate with its PAN during the scan period, so care
 * should be taken when performing scans of any significant duration while
 * presently joined to an existing PAN. At the end of the scanning procedure the
 * ::emberEnergyScanCompleteHandler stack handler shall be called.
 *
 * Possible error responses and their meanings:
 * - ::EMBER_INVALID_CALL, the samples parameter is invalid or the node is
 *   currently frequency hopping.
 * - ::EMBER_MAC_SCANNING, we are already scanning.
 * - ::EMBER_PHY_INVALID_CHANNEL, the specified channel is not a valid channel
 *   on the current platform.
 * - ::EMBER_NO_BUFFERS, the stack doesn't have enough memory at the moment to
 *   perform the requested scan.
 *
 * @param channel  The channel to scan.
 *
 * @param samples   The number of energy samples to be produced. Each sample is
 * performed averaging the detected energy over 8 symbols time (the actual
 * length depends on the selected PHY configuration).
 */
EmberStatus emberStartEnergyScan(uint8_t channel, uint8_t samples);

/** @brief This API allows the application to set the application portion of the
 * beacon payload. This is by default set to the empty string.
 *
 * @param payloadLength  The length in bytes of the application beacon payload
 *   to be set. This value can not exceed
 *   ::EMBER_MAC_MAX_APP_BEACON_PAYLOAD_LENGTH.
 *
 * @param payload   A pointer to the application beacon payload to be set.
 *
 * @return an ::EmberStatus value of ::EMBER_SUCCESS if the application beacon
 * payload was successfully set, otherwise an ::EmberStatus value indicating
 * the reason of failure.
 */
EmberStatus emberSetApplicationBeaconPayload(uint8_t payloadLength,
                                             uint8_t *payload);

/** @brief Forms a new network by becoming the coordinator.
 *
 * @param parameters  An ::EmberNetworkParameters value that specifies the
 *   network parameters of the network to be formed.
 *
 * @return An ::EmberStatus value that indicates either the successful formation
 *   of the new network, or the reason that the network formation failed.
 */
EmberStatus emberFormNetwork(EmberNetworkParameters *parameters);

/** @brief Causes the stack to associate with the network using the
 * specified network parameters. It can take several seconds for the stack to
 * associate with the local network. Do not send messages until a call to the
 * ::emberStackStatusHandler() callback informs you that the stack is up.
 *
 * @param nodeType    Specification of the role that this node will have in
 *   the network. This role can be ::EMBER_STAR_RANGE_EXTENDER,
 *   ::EMBER_STAR_END_DEVICE or ::EMBER_STAR_SLEEPY_END_DEVICE. If the node
 *   is frequency hopping, the role can not be ::EMBER_STAR_RANGE_EXTENDER.
 *
 * @param nodeId    An ::EmberNodeId value indicating the short ID the node
 * intends to use for addressing purposes. If this value is ::EMBER_NULL_NODE_ID
 * the network coordinator will allocate a new short address. Addresses should
 * be allocated by the coordinator unless there is a specific need to join a
 * network with a specific ID. If a specific ID is used, uniqueness should be
 * guaranteed across the entire network by the application, via some out of band
 * means.
 *
 * @param parameters  An ::EmberNetworkParameters value that specifies the
 * network parameters of the network with which the node should associate.
 *
 * @return An ::EmberStatus value that indicates either:
 * - that the association process began successfully, or
 * - the reason for failure.
 */
EmberStatus emberJoinNetworkExtended(EmberNodeType nodeType,
                                     EmberNodeId nodeId,
                                     EmberNetworkParameters *parameters);

#if defined(DOXYGEN_SHOULD_SKIP_THIS) || defined(UNIX_HOST)
/** @brief Causes the stack to associate with the network using the
 * specified network parameters. The network ID shall be assigned by the network
 * coordinator.
 * It can take several seconds for the stack to associate with the local
 * network. Do not send messages until a call to the ::emberStackStatusHandler()
 * callback informs you that the stack is up.
 *
 * @param nodeType    Specification of the role that this node will have in
 *   the network. This role can be ::EMBER_STAR_RANGE_EXTENDER,
 *   ::EMBER_STAR_END_DEVICE or ::EMBER_STAR_SLEEPY_END_DEVICE. If the node
 *   is frequency hopping, the role can not be ::EMBER_STAR_RANGE_EXTENDER.
 *
 * @param parameters  An ::EmberNetworkParameters value that specifies the
 *   network parameters of the network with which the node should associate.
 *
 * @return An ::EmberStatus value that indicates either:
 *   - that the association process began successfully, or
 *   - the reason for failure.
 */
EmberStatus emberJoinNetwork(EmberNodeType nodeType,
                             EmberNetworkParameters *parameters);
#else
#define emberJoinNetwork(nodeType, parameters) \
  (emberJoinNetworkExtended((nodeType), EMBER_NULL_NODE_ID, (parameters)))
#endif // DOXYGEN_SHOULD_SKIP_THIS

/** @brief Tells the stack to allow other nodes to join the network
 * with this node as their parent.  Joining is initially disabled by default.
 * This function may only be called after the node is part of a network
 * and the stack is up.
 *
 * @param duration  A value of 0x00 disables joining. A value of 0xFF enables
 *   joining indefinitely.  Any other value enables joining for that number of
 *   seconds.
 *
 * @return an ::EmberStatus value of ::EMBER_SUCCESS if the permit joining
 *   was successfully set, otherwise an ::EmberStatus value indicating the
 *   reason of failure.
 */
EmberStatus emberPermitJoining(uint8_t duration);

/** @brief Causes the stack to go up with the passed network parameters without
 * performing any over-the-air message exchange.
 *
 * @param nodeType    Specifies the role that this node will have in
 *   the network. The only device types allowed in the commissioning API are
 * ::EMBER_DIRECT_DEVICE and ::EMBER_MAC_MODE_DEVICE.
 *
 * @param nodeId  An ::EmberNodeId value that specifies the short ID the node
 * shall have. The passed node ID must be a valid short address
 * (any value other than ::EMBER_NULL_NODE_ID or ::EMBER_BROADCAST_ADDRESS).
 *
 * @param parameters  An ::EmberNetworkParameters value that specifies the
 *   network parameters of the network the node should participate in.
 *
 * @return An ::EmberStatus value that indicates either:
 *   - that the node successfully commissioned the passed network parameters
 *   - the reason for failure.
 */
EmberStatus emberJoinCommissioned(EmberNodeType nodeType,
                                  EmberNodeId nodeId,
                                  EmberNetworkParameters *parameters);

/** @brief The maximum number of entries the auxiliary address filtering table
 * can hold.
 */
#define EMBER_MAX_AUXILIARY_ADDRESS_FILTERING_TABLE_LENGTH   4

/** @brief Sets an entry in the auxiliary address filtering table at a given
 * address. Nodes of ::EMBER_DIRECT_DEVICE device type can receive incoming
 * messages destined to any of the node IDs in the auxiliary address filtering
 * table (while also receiving  messages destined to actual node ID). If the
 * passed node ID is ::EMBER_NULL_NODE_ID, the entry shall be cleared.
 *
 * @param nodeId    An ::EmberNodeId value to be added to the auxiliary address
 * filtering table at the passed entry index.
 *
 * @param entryIndex  The index of the auxiliary address filtering table entry
 * to be set.
 *
 * @return An ::EmberStatus value of ::EMBER_SUCCESS if auxiliary address
 * filtering table entry was successfully set. An ::EmberStatus value of
 * ::EMBER_INVALID_CALL if the passed entry index is invalid.
 */
EmberStatus emberSetAuxiliaryAddressFilteringEntry(EmberNodeId nodeId,
                                                   uint8_t entryIndex);

/** @brief Retrieves the content of the auxiliary address filtering
 *  table at a given address.
 *
 * @param entryIndex  The index in the auxiliary address filtering table entry
 * to be retrieved.
 *
 * @return An ::EmberNodeId value of ::EMBER_NULL_NODE_ID if the passed entry
 * index is invalid or if the passed entry index refers to an unused entry.
 * Otherwise it returns the content of the auxiliary address filtering table
 * entry corresponding to be passed entry index.
 */
EmberNodeId emberGetAuxiliaryAddressFilteringEntry(uint8_t entryIndex);

/** @brief Forgets the current network and reverts to a network status of
 * ::EMBER_NO_NETWORK.
 */
void emberResetNetworkState(void);

/** @brief An API to start operating as frequency hopping server. This API can
 * only be invoked when the node is joined to a network.
 *
 * @return An ::EmberStatus value of ::EMBER_SUCCESS if the node successfully
 * initiated frequency hopping server operations. An ::EmberStatus value of
 * ::EMBER_INVALID_CALL if the node is not currently joined to a network or if
 * the node is already performing frequency hopping.
 */
EmberStatus emberFrequencyHoppingStartServer(void);

/** @brief An API to start operating as frequency hopping client and synchronize
 * with the specified server. This API can be invoked on nodes that are already
 * joined to a network and nodes that are not joined to a network yet.
 * If the node is already performing frequency hopping, this API shall return
 * ::EMBER_INVALID_CALL.
 * If this API returns ::EMBER_SUCCESS, the
 * ::emberFrequencyHoppingStartClientCompleteHandler will be invoked
 * asynchronously to inform the application whether the node successfully
 * synchronized with the specified server or to inform the application of the
 * reason of failure.
 * Once the client is synched to a server, it may seamlessly perform the
 * resynchronization process if needed. Sleepy devices in particular
 * periodically perform the resynchronization process. If the client fails a
 * resynchronization process, it will inform the application by invoking the
 * ::emberStackStatusHandler handler with EMBER_MAC_SYNC_TIMEOUT status.
 * When this occurs, the client will be no longer synched to the server.
 * The application may elect to attempt a new synchronization process by
 * invoking this API again.
 *
 * @param serverNodeId   An ::EmberNodeId value indicating the node ID of the
 * server to synchronize with.
 *
 * @param serverPanId    An ::EmberPanId value indicating the PAN ID of the
 * server to synchronize with. Note that this parameter is meaningful only if
 * the node is not currently joined to any network.
 *
 * @return An ::EmberStatus value of ::EMBER_SUCCESS indicating that the node
 * successfully initiated the synchronization process with the server, otherwise
 * an ::EmberStatus value indicating the reason of failure.
 */
EmberStatus emberFrequencyHoppingStartClient(EmberNodeId serverNodeId,
                                             EmberPanId serverPanId);

/** @brief An API to stop frequency hopping. This API can only be invoked when
 * the node is frequency hopping.
 *
 * @return An ::EmberStatus value of ::EMBER_SUCCESS indicating that the node
 * successfully stopped frequency hopping. An ::EmberStatus value of
 * ::EMBER_INVALID_CALL if the node is not currently frequency hopping.
 */
EmberStatus emberFrequencyHoppingStop(void);
//@}//END APIs

#endif //__NETWORK_FORMATION_H__

/** @} // END addtogroup
 */
