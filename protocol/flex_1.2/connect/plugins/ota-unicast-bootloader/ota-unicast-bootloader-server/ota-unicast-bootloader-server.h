#ifndef _OTA_UNICAST_BOOTLOADER_SERVER_H_
#define _OTA_UNICAST_BOOTLOADER_SERVER_H_

#include "ota-unicast-bootloader/ota-unicast-bootloader-protocol.h"
#include "ota-unicast-bootloader/ota-unicast-bootloader-types.h"

/**
 * @addtogroup ota_unicast_bootloader_server
 *
 * See ota-unicast-bootloader-server.h for source code.
 * @{
 */

/**
 * @brief The number of consecutive stack message submission errors or stack
 * related errors such as CCA failures after which the plugin will give up.
 */
#define EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_SERVER_MAX_STACK_ERRORS                 8

/**
 * @brief The number of consecutive unicast attempts after which a target shall
 * be declared unreachable. Legal values for this are in the [0,7] range.
 */
#define EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_SERVER_MAX_UNICAST_ERRORS               4

/**
 * @brief The time in milliseconds after which the server gives up waiting for
 * a response from a client.
 */
#define EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_SERVER_RESPONSE_TIMEOUT_MS            250

/**
 * @brief The maximum number of image broadcast rounds the server shall perform
 * before declaring an image distribution process failed.
 */
#define EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_SERVER_MAX_BROADCAST_ROUNDS             5

//------------------------------------------------------------------------------
// APIs

/** @brief  An API for initiating the image distribution process.
 *
 * @param targetId   The target’s short ID of the image distribution process.
 *
 * @param imageSize   The image size in bytes to be distributed.
 *
 * @param imageTag    A 1-byte tag that will be embedded in the server-to-client
 * over-the-air messages. The application can use the image tag for versioning
 * purposes and/or for distinguishing between different image types.
 *
 * @return An ::EmberAfOtaUnicastBootloaderStatus value of:
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_BUSY if an image distriution is
 * already in progress
 * - ::EMBER_OTA_BOOTLOADER_STATUS_INVALID_CALL if the given target or
 * the image size is invalid
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS if the image distribution
 * was successfully initiated.
 */
EmberAfOtaUnicastBootloaderStatus emberAfPluginOtaUnicastBootloaderServerInitiateImageDistribution(
  EmberNodeId targetId,
  uint32_t imageSize,
  uint8_t imageTag
  );

/** @brief  A callback invoked by the OTA Unicast Bootloader Server plugin during an
 * image distribution process to retrieve a contiguous segment of the image
 * being distributed.
 *
 * @param   startIndex    The index of first byte the application should copy
 * into the passed array.
 *
 * @param   endIndex      The index of the last byte the application should copy
 * into the passed array.
 *
 * @param   imageTag    A 1-byte tag of the image for which a segment is being
 * requested.
 *
 * @param   imageSegment    An array of (endIndex - startIndex + 1) length to
 * which the application should copy the requested image segment.
 *
 * @return A boolean indicating whether the application successfully copied the
 * requested bytes into the passed array. If the application returns FALSE, the
 * OTA Unicast Server plugin shall abort the ongoing distribution process.
 */
bool emberAfPluginOtaUnicastBootloaderServerGetImageSegmentCallback(
  uint32_t startIndex,
  uint32_t endIndex,
  uint8_t imageTag,
  uint8_t *imageSegment
  );

/** @brief  A callback invoked by the OTA Bootloader Unicast Server plugin when the
 * image distribution process is terminated.
 *
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS if the target has confirmed
 * that the full image has been received.
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_DATA_UNDERFLOW if the application failed to
 * supply the requested image segments.
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_STACK_ERROR if the server encountered
 * multiple consecutive transmission errors. The Server shall give up the image
 * distribution process if
 * ::EMBER_AF_PLUGIN_OTA_BOOTLOADER_SERVER_MAX_STACK_ERRORS consecutive
 * transmission errors are encountered.
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_FAILED if the distribution
 * process terminated prematurely because the target can't be reached.
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_TIMEOUT if the target has not responded
 * within EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_SERVER_RESPONSE_TIMEOUT_MS
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_ABORTED if the application aborted the
 * current image distribution process.
 */
void emberAfPluginOtaUnicastBootloaderServerImageDistributionCompleteCallback(
  EmberAfOtaUnicastBootloaderStatus status
  );

/** @brief  An API for initiating the process of requesting a target
 * device to initiate the bootload of a received image some time in the future.
 *
 * @param bootloadDelayMs   The delay in milliseconds after which all the
 * targets should be perform an image bootload.
 *
 * @param imageTag  A 1-byte tag that identifies the image to be bootloaded at
 * the target device.
 *
 * @param targetId
 *
 * @return An ::EmberAfOtaUnicastBootloaderStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_STATUS_SUCCESS if the plugin successfully initiated
 * the process of requesting a target to initiate a bootload.
 * If this is the case, the corresponding callback
 * ::emberAfPluginBootloaderServerRequestTargetBootloadCompleteCallback() shall
 * be invoked when the request process to all targets has completed.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_INVALID_CALL if some of the passed parameters
 * are invalid.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_BUSY if the server is currently involved in
 * another over-the-air process.
 */
EmberAfOtaUnicastBootloaderStatus emberAfPluginBootloaderServerInitiateRequestTargetBootload(
  uint32_t bootloadDelayMs,
  uint8_t imageTag,
  EmberNodeId targetId
  );

/** @brief  An API for aborting the ongoing process (image distribution,
 * status request or bootload request).
 *
 * @return An ::EmberAfOtaUnicastBootloaderStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_STATUS_SUCCESS if the current ongoing process was
 * successfully aborted.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_INVALID_CALL if the server is not currently
 * involved in any process.
 */
EmberAfOtaUnicastBootloaderStatus emberAfPluginOtaUnicastBootloaderServerAbortCurrentProcess(
  void
  );

/** @brief  A callback invoked by the OTA Unicast Bootloader Server plugin when a
 * bootload request process has completed.
 *
 * @param   status    An ::EmberAfOtaUnicastBootloaderStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_STATUS_SUCCESS if all the targets have been
 * requested to perform a bootload. Notice that some targets might have been
 * declared unreachable.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_STACK_ERROR if the server encountered
 * multiple consecutive transmission errors. The Server shall give up the
 * bootload request process if
 * ::EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_SERVER_MAX_STACK_ERRORS consecutive
 * transmission errors are encountered.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_ABORTED if the application aborted the
 * current bootload request process.
 */
void emberAfPluginOtaUnicastBootloaderServerRequestTargetBootloadCompleteCallback(
  EmberAfOtaUnicastBootloaderStatus status
  );

/** @} // END addtogroup
 */

#endif // _OTA_UNICAST_BOOTLOADER_SERVER_H_
