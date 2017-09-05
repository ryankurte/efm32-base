#ifndef _OTA_BOOTLOADER_SERVER_H_
#define _OTA_BOOTLOADER_SERVER_H_

#include "ota-bootloader/ota-bootloader-protocol.h"
#include "ota-bootloader/ota-bootloader-types.h"

/**
 * @addtogroup ota_bootloader_server
 *
 * See ota-bootloader-server.h for source code.
 * @{
 */

/**
 * @brief The number of consecutive stack message submission errors or stack
 * related errors such as CCA failures after which the plugin will give up.
 */
#define EMBER_AF_PLUGIN_OTA_BOOTLOADER_SERVER_MAX_STACK_ERRORS                 8

/**
 * @brief The number of consecutive unicast attempts after which a target shall
 * be declared unreachable. Legal values for this are in the [0,7] range.
 */
#define EMBER_AF_PLUGIN_OTA_BOOTLOADER_SERVER_MAX_UNICAST_ERRORS               4

/**
 * @brief The time in milliseconds after which the server gives up waiting for
 * a response from a client.
 */
#define EMBER_AF_PLUGIN_OTA_BOOTLOADER_SERVER_RESPONSE_TIMEOUT_MS            250

/**
 * @brief The maximum number of image broadcast rounds the server shall perform
 * before declaring an image distribution process failed.
 */
#define EMBER_AF_PLUGIN_OTA_BOOTLOADER_SERVER_MAX_BROADCAST_ROUNDS             5

//------------------------------------------------------------------------------
// APIs

/** @brief  An API for initiating the image distribution process.
 * The application can use the ::emberAfPluginBootloaderServerGetTargetStatus()
 * API at any time during the image distribution process to check the status of
 * each target.
 *
 * @param imageSize   The image size in bytes to be distributed.
 *
 * @param imageTag    A 1-byte tag that will be embedded in the server-to-client
 * over-the-air messages. The application can use the image tag for versioning
 * purposes and/or for distinguishing between different image types.
 *
 * @param targetList  An array of ::EmberNodeId indicating the node IDs of
 * the target devices.
 *
 * @param targetListLength   The length of the passed target list.
 *
 * @return An ::EmberAfOtaBootloaderStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_STATUS_SUCCESS if the image distribution is
 * successfully initiated. If this is the case, the
 * ::emberAfPluginOtaBootloaderServerDistributionCompletedCallback() callback
 * shall be invoked when the distribution process has terminated.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_INVALID_CALL if some of the passed parameters
 * are invalid.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_BUSY if the server is already performing
 * another image distribution or some other over-the-air process.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_NO_BUFFERS if the server currently has no
 * enough RAM to store the passed target list.
 */
EmberAfOtaBootloaderStatus emberAfPluginOtaBootloaderServerInitiateImageDistribution(uint32_t imageSize,
                                                                                     uint8_t imageTag,
                                                                                     EmberNodeId *targetList,
                                                                                     uint16_t targetListLength);

/** @brief  An API for initiating the process of requesting the status of a set
 * of target devices.
 * The application can use the ::emberAfPluginBootloaderServerGetTargetStatus()
 * API at any time during the target status request process to check the status
 * of each target.
 *
 * @param targetList  An array of ::EmberNodeId indicating the node IDs of
 * the target devices that shall be queried for their status.
 *
 * @param targetListLength   The length of the passed target list.
 *
 * @return An ::EmberAfOtaBootloaderStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_STATUS_SUCCESS if the plugin successfully initiated
 * the process of requesting the status of a set of targets.
 * If this is the case, the corresponding callback
 * ::emberAfPluginBootloaderServerRequestTargetsStatusCompleteCallback() shall
 * be invoked when the request process to all targets has completed.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_INVALID_CALL if some of the passed parameters
 * are invalid.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_BUSY if the server is currently involved in
 * another over-the-air process.
 */
EmberAfOtaBootloaderStatus emberAfPluginBootloaderServerInitiateRequestTargetsStatus(EmberNodeId *targetList,
                                                                                     uint16_t targetListLength,
                                                                                     uint8_t applicationServerStatus);

/** @brief  An API for initiating the process of requesting a set of target
 * devices to initiate the bootload of a received image some time in the future.
 * The application can use the ::emberAfPluginBootloaderServerGetTargetStatus()
 * API at any time during the bootload request process to check the status of
 * each target.
 *
 * @param bootloadDelayMs   The delay in milliseconds after which all the
 * targets should be perform an image bootload.
 *
 * @param imageTag  A 1-byte tag that identifies the image to be bootloaded at
 * the target devices.
 *
 * @param targetList  An array of ::EmberNodeId indicating the node IDs of
 * the target devices that shall be requested to bootload an image.
 *
 * @param targetListLength   The length of the passed target list.
 *
 * @return An ::EmberAfOtaBootloaderStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_STATUS_SUCCESS if the plugin successfully initiated
 * the process of requesting a set of targets to initiate a bootload.
 * If this is the case, the corresponding callback
 * ::emberAfPluginBootloaderServerRequestTargetsBootloadCompleteCallback() shall
 * be invoked when the request process to all targets has completed.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_INVALID_CALL if some of the passed parameters
 * are invalid.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_BUSY if the server is currently involved in
 * another over-the-air process.
 */
EmberAfOtaBootloaderStatus emberAfPluginBootloaderServerInitiateRequestTargetsBootload(uint32_t bootloadDelayMs,
                                                                                       uint8_t imageTag,
                                                                                       EmberNodeId *targetList,
                                                                                       uint16_t targetListLength);

/** @brief  An API for retrieving the status of an individual target in the
 * distribution list.
 *
 * @param targetId   The node ID of the target device whose status is being
 * requested.
 *
 * @param applicationTargetStatus   The application status reported by the
 * client side application. This parameter is valid only for certain return
 * status codes (see return status documentation).
 *
 * @return An ::EmberAfOtaBootloaderTargetStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_INVALID if the passed node ID does not
 * appear in the current server target list of the current ongoing process or if
 * there is no current ongoing process.
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_UNREACHABLE if the target has not
 * responded to any of the server's unicast messages.
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_COMPLETED if the server
 * is currently performing an image distribution process and the target has
 * confirmed that it has received the full image.
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_ONGOING if the server is
 * currently performing an image distribution process and the target has
 * partially received the image and distribution is continuing.
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_REFUSED if the target has
 * refused the current image.
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_FAILED if the server is
 * currently performing an image distribution process and the target reported
 * that an error was encountered.
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_ABORTED if the server is
 * currently performing an image distribution process and the target decided to
 * abort the image download process. In this case, the client also reports an
 * application status (thus the applicationTargetStatus parameter is valid).
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_STATUS_REQUEST_COMPLETED if the server
 * is currently performing a target status request process and the target has
 * responded to the server's inquiry. In this case, the client also reports an
 * application status (thus the applicationTargetStatus parameter is valid).
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_STATUS_REQUEST_ONGOING if the server
 * is currently performing a target status request process and the target has
 * not been queried by the server yet.
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_BOOTLOAD_REQUEST_ACCEPTED is the
 * server is currently performing a bootload request process and the target has
 * accepted to perform the requested image bootload.
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_BOOTLOAD_REQUEST_REFUSED is the
 * server is currently performing a bootload request process and the target has
 * refused to perform the requested image bootload.
 * - ::EMBER_OTA_BOOTLOADER_TARGET_STATUS_BOOTLOAD_REQUEST_ONGOING is the
 * server is currently performing a bootload request process and the target has
 * not been reached by the server yet.
 */
EmberAfOtaBootloaderTargetStatus emberAfPluginBootloaderServerGetTargetStatus(EmberNodeId targetId,
                                                                              uint8_t *applicationTargetStatus);

/** @brief  An API for aborting the ongoing process (image distribution,
 * status request or bootload request). Notice that aborting a bootload request
 * process would likely result in some targets performing the bootload while
 * some others won't.
 *
 * @return An ::EmberAfOtaBootloaderStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_STATUS_SUCCESS if the current ongoing process was
 * successfully aborted.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_INVALID_CALL if the server is not currently
 * involved in any process.
 */
EmberAfOtaBootloaderStatus emberAfPluginOtaBootloaderServerAbortCurrentProcess(void);

//------------------------------------------------------------------------------
// Callbacks

/** @brief  A callback invoked by the OTA Bootloader Server plugin during an
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
 * OTA Server plugin shall abort the ongoing distribution process.
 */
bool emberAfPluginOtaBootloaderServerGetImageSegmentCallback(uint32_t startIndex,
                                                             uint32_t endIndex,
                                                             uint8_t imageTag,
                                                             uint8_t *imageSegment);

/** @brief  A callback invoked by the OTA Bootloader Server plugin when the
 * image distribution process is terminated. The application can use the
 * ::emberAfPluginBootloaderServerGetTargetStatus() API to retrieve the status
 * reported by each target device.
 *
 * @param   status    An ::EmberAfOtaBootloaderStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_STATUS_SUCCESS if all the targets have confirmed
 * that the full image was has received, except for those that have been
 * declared "unreachable".
 * - ::EMBER_OTA_BOOTLOADER_STATUS_DATA_UNDERFLOW if the application failed to
 * supply the requested image segments.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_STACK_ERROR if the server encountered
 * multiple consecutive transmission errors. The Server shall give up the image
 * distribution process if
 * ::EMBER_AF_PLUGIN_OTA_BOOTLOADER_SERVER_MAX_STACK_ERRORS consecutive
 * transmission errors are encountered.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_FAILED if the distribution
 * process terminated prematurely because all targets have been declared
 * unreachable.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_TIMEOUT if the server performed all the
 * allowable broadcast rounds and there are still missing segments at one or
 * more targets. The maximum allowable rounds is defined by
 * ::EMBER_AF_PLUGIN_OTA_BOOTLOADER_SERVER_MAX_BROADCAST_ROUNDS.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_ABORTED if the application aborted the
 * current image distribution process.
 */
void emberAfPluginOtaBootloaderServerImageDistributionCompleteCallback(EmberAfOtaBootloaderStatus status);

/** @brief  A callback invoked by the OTA Bootloader Server plugin when bootload
 * request process has completed. Within this callback, the application should
 * use the ::emberAfPluginBoootloaderServerGetTargetStatus() API to retrieve the
 * status and application status reported by each target.
 *
 * @param   status    An ::EmberAfOtaBootloaderStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_STATUS_SUCCESS if all the targets have been queried
 * for their status. Notice that some targets might have been declared
 * unreachable.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_STACK_ERROR if the server encountered
 * multiple consecutive transmission errors. The Server shall give up the
 * targets status request process if
 * ::EMBER_AF_PLUGIN_OTA_BOOTLOADER_SERVER_MAX_STACK_ERRORS consecutive
 * transmission errors are encountered.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_ABORTED if the application aborted the
 * current targets status request process.
 */
void emberAfPluginBootloaderServerRequestTargetsStatusCompleteCallback(EmberAfOtaBootloaderStatus status);

/** @brief  A callback invoked by the OTA Bootloader Server plugin when a
 * bootload request process has completed. Within this callback, the application
 * should use the ::emberAfPluginBoootloaderServerGetTargetStatus() API to
 * retrieve the status and the application status reported by each target.
 *
 * @param   status    An ::EmberAfOtaBootloaderStatus value of:
 * - ::EMBER_OTA_BOOTLOADER_STATUS_SUCCESS if all the targets have been
 * requested to perform a bootload. Notice that some targets might have been
 * declared unreachable.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_STACK_ERROR if the server encountered
 * multiple consecutive transmission errors. The Server shall give up the
 * bootload request process if
 * ::EMBER_AF_PLUGIN_OTA_BOOTLOADER_SERVER_MAX_STACK_ERRORS consecutive
 * transmission errors are encountered.
 * - ::EMBER_OTA_BOOTLOADER_STATUS_ABORTED if the application aborted the
 * current bootload request process.
 */
void emberAfPluginBootloaderServerRequestTargetsBootloadCompleteCallback(EmberAfOtaBootloaderStatus status);

/** @} // END addtogroup
 */

#endif // _OTA_BOOTLOADER_SERVER_H_
