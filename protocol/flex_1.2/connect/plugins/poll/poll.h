// Copyright 2015 Silicon Laboratories, Inc.

/**
 * @addtogroup poll
 *
 * See poll.h for source code.
 * @{
 */

/** @brief Set the short poll interval.
 *
 *  @param intervalQS The short poll interval in quarter seconds to be set.
 */
void emberAfPluginPollSetShortPollInterval(uint8_t intervalQS);

/** @brief Set the long poll interval.
 *
 *  @param intervalS The long poll interval in seconds to be set.
 */
void emberAfPluginPollSetLongPollInterval(uint16_t intervalS);

/** @brief Enable/disable short polling.
 *
 *  @param enable If this parameter is TRUE, short polling shall be enabled,
 *  otherwise the node shall switch back to long polling.
 */
void emberAfPluginPollEnableShortPolling(bool enable);

/** @} // END addtogroup
 */
