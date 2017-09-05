/**
 * @file stack-info.h
 * @brief Ember Connect API for accessing and setting stack information.
 * See @ref stack_info for documentation.
 *
 * <!--Copyright 2014 by Silicon Labs. All rights reserved.              *80*-->
 */

/**
 * @addtogroup stack_info
 *
 * See stack-info.h for source code.
 * @{
 */

/** @brief A mask of the tasks that prevent a device from sleeping.
 */
#define EMBER_HIGH_PRIORITY_TASKS \
  (EMBER_OUTGOING_MESSAGES | EMBER_INCOMING_MESSAGES | EMBER_RADIO_IS_ON)

/** @name Handlers*/
//@{

/** @brief A callback invoked when the status of the stack changes.
 *
 * The application is free to begin messaging once it receives the
 * ::EMBER_NETWORK_UP status.
 *
 * @param status  Stack status. One of the following:
 * - ::EMBER_NETWORK_UP
 * - ::EMBER_NETWORK_DOWN
 * - ::EMBER_NO_VALID_BEACONS
 * - ::EMBER_JOIN_SCAN_FAILED
 * - ::EMBER_JOIN_FAILED
 * - ::EMBER_JOIN_DENIED
 * - ::EMBER_JOIN_TIMEOUT
 * - ::EMBER_MAC_SYNC_TIMEOUT
 */
void emberStackStatusHandler(EmberStatus status);

/** @brief This handler is invoked at coordinator or range extender nodes when
 * a new child has joined the device.
 *
 * @param nodeType  The role of the joining device (::EMBER_STAR_RANGE_EXTENDER,
 * ::EMBER_STAR_END_DEVICE or ::EMBER_STAR_SLEEPY_END_DEVICE).
 *
 * @param nodeId    The node ID of the joining device.
 */
void emberChildJoinHandler(EmberNodeType nodeType,
                           EmberNodeId nodeId);

/** @brief This handler is invoked in ISR context whenever a stack-related ISR
 * routine fires.
 */
void emberStackIsrHandler(void);

//@}//END Handlers

/** @name APIs*/
//@{

/** @brief Immediately turns the radio power completely off.
 *
 * After calling this function, you must not call any other stack function
 * except emberStackPowerUp(). This is because all other stack
 * functions require that the radio is powered on for their
 * proper operation.
 */
void emberStackPowerDown(void);

/** @brief Powers up the radio.  Typically called coming out of sleep.
 *
 * For non-sleepy devices, also turns the radio on and leaves it in rx mode.
 */
void emberStackPowerUp(void);

/** @brief Returns the current join status.
 *
 *   Returns a value indicating whether the node is joining, joined to, or
 *   leaving a network.
 *
 * @return An ::EmberNetworkStatus value indicating the current join status.
 */
EmberNetworkStatus emberNetworkState(void);

/** @brief Indicates whether the stack is currently up.
 *
 *   Returns true if the stack is joined to a network and ready to send and
 *   receive messages.  This reflects only the state of the local node; it does
 *   not indicate whether or not other nodes are able to communicate with this
 *   node.
 *
 * @return TRUE if the stack is up, false otherwise.
 */
bool emberStackIsUp(void);

/** @brief Sets the security key.
 *
 * @param key   An ::EmberKeyData value containing the security key to be set.
 *
 * @return An EmberStatus value of ::EMBER_SUCCESS if the key was successfully
 *   set. Otherwise it returns an EmberStatus value of ::EMBER_INVALID_CALL.
 */
EmberStatus emberSetSecurityKey(EmberKeyData *key);

/** @brief Retrieves the stack counter corresponding to the passed counter type.
 *
 * @param counterType   An ::EmberCounterType value indicating the stack counter
 * to be retrieved.
 *
 * @return An EmberStatus value of ::EMBER_SUCCESS if the stack counter was
 * successfully retrieved. An EmberStatus value of ::EMBER_INVALID_CALL if the
 * passed counterType is invalid. An EmberStatus value of
 * ::EMBER_LIBRARY_NOT_PRESENT if the stack counter library is not present.
 */
EmberStatus emberGetCounter(EmberCounterType counterType, uint32_t *count);

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
/** @brief Sets the phy mode to 2.4GHz or SubGHz.
 * This API is only applicable to EM357Pro2+
 *
 * Note: Care should be taken when using this API,
 * Phy should be changed when the current network is down.
 *
 * @param mode  Desired phy mode: 0 is SubGHz, 1 is 2.4GHz.
 *
 * @return An EmberStatus value of ::EMBER_SUCCESS if the phy mode was
 * successfully set. It returns an EmberStatus value of ::EMBER_INVALID_CALL if
 * the stack is up, desired phy mode is already set or architecture is not
 * EM357Pro2+.
 */
EmberStatus emberSetPhyMode(uint8_t mode);

/** @brief Gets the current phy mode.
 * This API is only applicable to EM357Pro2+
 *
 * @return Current phy mode: 0 is SubGHz, 1 is 2.4GHz. ::EMBER_INVALID_CALL is
 * returned if architecture is not EM357Pro2+.
 */
uint8_t emberGetPhyMode(void);
#endif

/** @brief Sets the channel to use for sending and receiving messages on the
 * current logical network. For a list of available radio channels, see the
 * technical specification for the RF communication module in your Developer
 * Kit.
 *
 * Note: Care should be taken when using this API.
 *       All devices on a network must use the same channel.
 *       Each call triggers a token write.
 *       Excessive usage might cause flash to wear-out.
 *
 * @param channel  Desired radio channel.
 *
 * @return An ::EmberStatus value of:
 * - ::EMBER_SUCCESS if the stack accepted the channel change.
 * - ::EMBER_INVALID_CALL if the node is currently performing frequency hopping.
 * - ::EMBER_PHY_INVALID_CHANNEL if the passed channel is invalid.
 * - ::EMBER_MAC_BUSY if the MAC is currently performing some high priority
 *   task.
 */
EmberStatus emberSetRadioChannel(uint8_t channel);

/** @brief Gets the radio channel to which a node is set on the current
 * logical network. The possible return values depend on the radio in use.
 * For a list of available radio channels, see the technical specification
 * for the RF communication module in your Developer Kit.
 *
 * @return Current radio channel.
 */
uint8_t emberGetRadioChannel(void);

/** @brief Sets the radio output power at which a node is to operate for the
 * current logical network. Ember radios have discrete power settings. For a
 * list of available power settings, see the technical specification for the RF
 * communication module in your Developer Kit.
 * Note: Care should be taken when using this API on a running network,
 * as it will directly impact the established link qualities neighboring nodes
 * have with the node on which it is called.  This can lead to disruption of
 * existing routes and erratic network behavior.
 * Note: If the requested power level is not available on a given radio, this
 * function will use the next higher available power level.
 *
 * @param power  Desired radio output power, in dBm.
 *
 * @return An ::EmberStatus value indicating the success or failure of the
 * command.  Failure indicates that the requested power level is out of range.
 */
EmberStatus emberSetRadioPower(int8_t power);

/** @brief Gets the radio output power of the current logical network at which
 * a node is operating. Ember radios have discrete power settings. For a list of
 * available power settings, see the technical specification for the RF
 * communication module in your Developer Kit.
 *
 * @return Current radio output power, in dBm.
 */
int8_t emberGetRadioPower(void);

/** @brief This API allows the application to turn the radio on/off. This API is
 * intended to be used with direct devices only.
 *
 * @param radioOn  If this parameter is true, the radio shall be turned on,
 * otherwise it shall be turned off.
 *
 * @return An ::EmberStatus value indicating the success or failure of the
 * command.  Failure indicates that the node type is a type other than
 * ::EMBER_DIRECT_DEVICE or ::EMBER_MAC_MODE_DEVICE.
 */
EmberStatus emberSetRadioPowerMode(bool radioOn);

/** @brief Sets the MAC layer transmission parameters.
 *
 * @param checkCca   If this is set to TRUE, the node will perform a clear
 *                   channel assessment prior to transmit a packet. If the
 *                   channel is not clear, a random backoff shall be performed.
 *                   If this is set to FALSE, no clear channel assessment is
 *                   performed and the packet shall be transmitted right away.
 *                   This parameter is set by default to TRUE.
 *
 * @param maxCcaAttempts   The maximum number of clear channel assessment
 *                         attempts that shall be performed prior to fail to
 *                         transmit a packet with ::EMBER_PHY_TX_CCA_FAIL
 *                         status. This parameter is set by default to 4.
 *                         Note: this is meaningful only if the checkCca
 *                         parameter is set to TRUE.
 *
 * @param minBackoffExp   The backoff exponent to be used if the initial channel
 *                        clear assessment fails. This parameter is set by
 *                        default to 3.
 *                        Note: this is meaningful only if the checkCca
 *                        parameter is set to TRUE.
 *
 * @param maxBackoffExp   The backoff exponent to be used if the final channel
 *                        clear assessment fails. This parameter is set by
 *                        default to 5.
 *                        Note: this is meaningful only if the checkCca
 *                        parameter is set to TRUE.
 *
 * @param maxRetries      The number of transmission retries that shall be
 *                        performed in case no acknowledgement was received.
 *                        This parameter is set by default to 3 (which means
 *                        that a total of 4 transmission attempts shall per
 *                        performed).
 *
 * @return An ::EmberStatus value indicating whether the MAC parameters were
 * successfully set or the reason of failure.
 */
EmberStatus emberSetMacParams(bool checkCca,
                              uint8_t maxCcaAttempts,
                              uint8_t minBackoffExp,
                              uint8_t maxBackoffExp,
                              uint8_t maxRetries);

/** @brief Returns a bitmask indicating the stack's current tasks.
 *
 *  The mask ::EMBER_HIGH_PRIORITY_TASKS defines which tasks are high
 *  priority.  Devices should not sleep if any high priority tasks are active.
 *  Active tasks that are not high priority are waiting for
 *  messages to arrive from other devices.  If there are active tasks,
 *  but no high priority ones, the device may sleep but should periodically
 *  wake up and call ::emberPollForData() in order to receive messages.  Parents
 *  will hold messages for ::EMBER_INDIRECT_TRANSMISSION_TIMEOUT_MS milliseconds
 *  before discarding them.
 *
 * @return A bitmask of the stack's active tasks.
 */
uint16_t emberCurrentStackTasks(void);

/** @brief Indicates whether the stack is currently in a state where
 * there are no high priority tasks and may sleep.
 *
 * There may be tasks expecting incoming messages, in which case the device
 * should periodically wake up and call ::emberPollForData() in order to receive
 * messages. This function can only be called when the node type is
 * ::EMBER_STAR_SLEEPY_END_DEVICE.
 *
 * @return TRUE if the application may sleep but the stack may be expecting
 * incoming messages.
 */
#if defined(DOXYGEN_SHOULD_SKIP_THIS) || defined(UNIX_HOST)
bool emberOkToNap(void);
#else
#define emberOkToNap() \
  (!(emberCurrentStackTasks() & EMBER_HIGH_PRIORITY_TASKS))
#endif

/** @brief Indicates whether the stack currently has any tasks pending.
 *
 * If there are no pending tasks then ::emberTick() does not need to be called
 * until the next time a stack API function is called. This function can only be
 * called when the node type is ::EMBER_STAR_SLEEPY_END_DEVICE.
 *
 * @return TRUE if the application may sleep for as long as it wishes.
 */
#if defined(DOXYGEN_SHOULD_SKIP_THIS) || defined(UNIX_HOST)
bool emberOkToHibernate(void);
#else
#define emberOkToHibernate() (!emberCurrentStackTasks())
#endif

#if defined(DOXYGEN_SHOULD_SKIP_THIS) || defined(UNIX_HOST)
/** @brief Returns the EUI64 ID of the local node.
 *
 * @return The 64-bit ID.
 */
#ifdef UNIX_HOST
uint8_t* emberGetEui64(void);
#else
EmberEUI64 emberGetEui64(void);
#endif

/** @brief Determines whether \c eui64 is the local node's EUI64 ID.
 *
 * @param eui64  An EUI64 ID.
 *
 * @return TRUE if \c eui64 is the local node's ID, otherwise FALSE.
 */
bool emberIsLocalEui64(EmberEUI64 eui64);

/** @brief Returns the 16-bit node ID of local node on the current logical
 * network.
 *
 * @return The 16-bit ID.
 */
EmberNodeId emberGetNodeId(void);

/** @brief Returns the local node's PAN ID of the current logical network.
 *
 * @return A PAN ID.
 */
EmberPanId emberGetPanId(void);

/** @brief Returns the parent's node ID.
 *
 * @return The parent's node ID.
 */
EmberNodeId emberGetParentId(void);

/** @brief Returns an ::EmberNodeType value indicating the type of the node.
 *
 * @return A PAN ID.
 */
EmberNodeType emberGetNodeType(void);

#else   // Doxygen ignores the following
extern EmberNodeId emLocalNodeId;
#define emberGetNodeId() (emLocalNodeId)
extern EmberPanId emLocalPanId;
#define emberGetPanId() (emLocalPanId)
extern EmberNodeId emParentId;
#define emberGetParentId() (emParentId)
extern EmberNodeType emNodeType;
#define emberGetNodeType() (emNodeType)
extern EmberEUI64 emLocalEui64;
#define emberGetEui64() (emLocalEui64)
#define emberIsLocalEui64(eui64) \
  (MEMCOMPARE((eui64), emLocalEui64, EUI64_SIZE) == 0)

#endif // DOXYGEN_SHOULD_SKIP_THIS

//@}//END APIs

/** @} // END addtogroup
 */
