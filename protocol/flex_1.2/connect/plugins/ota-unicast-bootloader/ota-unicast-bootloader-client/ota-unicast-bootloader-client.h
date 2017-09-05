#ifndef _OTA_UNICAST_BOOTLOADER_CLIENT_H_
#define _OTA_UNICAST_BOOTLOADER_CLIENT_H_

#include "ota-unicast-bootloader/ota-unicast-bootloader-protocol.h"
#include "ota-unicast-bootloader/ota-unicast-bootloader-types.h"

/**
 * @addtogroup ota_unicast_bootloader_client
 *
 * See ota-unicast-bootloader-client.h for source code.
 * @{
 */

/*The client shall call the following callbacks (implemented by the application):*/

/** @brief  A callback invoked by the OTA Unicast Client plugin when the client starts
 * receiving a new image. The application can choose to start receiving the
 * image or ignore(refuse) it. If the application chooses to receive the image, other
 * images sent out by other server shall be ignored until the client completes
 * this download.
 *
 * @param   serverId    The node ID of the server that initiated the new image
 * distribution process.
 *
 * @param   imageTag    A 1-byte tag that identifies the incoming image.
 */
bool emberAfPluginOtaUnicastBootloaderClientNewIncomingImageCallback(
  EmberNodeId serverId,
  uint8_t imageTag
  );

/** @brief  A callback invoked by the OTA Unicast Bootloader Client plugin when an image
 * segment that is part of an image that the application elected to download was
 * received.
 *
 * @param   serverId    The node ID of the server that initiated the new image
 * distribution process.
 *
 * @param   startIndex    The index of first byte of the passed segment.
 *
 * @param   endIndex   The index of the last byte of the passed segment.
 *
 * @param   imageTag   A 1-byte tag of the image the passed segment belongs to.
 *
 * @param   imageSegment    An array containing the image segment.
 */
void emberAfPluginOtaUnicastBootloaderClientIncomingImageSegmentCallback(
  EmberNodeId serverId,
  uint32_t startIndex,
  uint32_t endIndex,
  uint8_t imageTag,
  uint8_t *imageSegment
  );

/** @brief  A callback invoked by the OTA Bootloader Client plugin to indicate
 * that an image download has completed.
 *
 * @param status   An ::EmberAfOtaUnicastBootloaderStatus value of:
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS  indicating that the full image
 * corresponding to the passed tag has been received. If this is the case,
 * the client previously handed all the image segments to the application using
 * the ::emberAfPluginOtaUnicastBootloaderClientIncomingImageSegmentCallback()
 * callback.
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_FAILED  indicating that the client failed to
 * fully download the image and the download process was terminated.
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_TIMEOUT indicating that the client timed out
 * waiting for a message from the server.
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_ABORTED indicating that the application
 * aborted the ongoing image download process as result of calling the API
 * ::emberAfPluginOtaUnicastBootloaderClientAbortImageDownload().
 *
 * @param imageTag   A 1-byte tag of the image this callback refers to.
 *
 * @param imageSize  The total size of the downloaded image in bytes. This
 * parameter is meaningful only in case the status parameter is set to
 * ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS.
 */
void emberAfPluginOtaUnicastBootloaderClientImageDownloadCompleteCallback(
  EmberAfOtaUnicastBootloaderStatus status,
  uint8_t imageTag,
  uint32_t imageSize
  );

/** @brief  An API for aborting an ongoing image download process.
 *
 * @param   imageTag    A 1-byte tag that identifies the image the client should
 * no longer download.
 *
 * @param applicationErrorStatus   A 1-byte error code that shall be reported
 * to the server.
 *
 * @return   An ::EmberAfOtaUnicastBootloaderStatus value of:
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS   If the ongoing image download
 * process was successfully aborted.
 * - ::EMBER_OTA_UNICAST_BOOTLOADER_STATUS_INVALID_CALL   If the client was not
 * currently involved in an image download process or it was currently
 * downloading an image with a different tag.
 */
EmberAfOtaUnicastBootloaderStatus emberAfPluginOtaUnicastBootloaderClientAbortImageDownload(uint8_t imageTag,
                                                                                            uint8_t applicationErrorStatus
                                                                                            );

/** @brief  A callback invoked by the OTA Unicast Bootloader Client plugin to indicate
 * that an OTA Unicast Bootloader Server has requested to perform a bootload operation
 * at a certain point in time in the future.
 *
 * @param serverId   The ID of the server the request came from.
 *
 * @param imageTag   A 1-byte tag of the image this callback refers to.
 *
 * @param bootloadDelayMs   The delay in milliseconds after which the client has
 * been requested to perform a bootload operation.
 *
 * @param applicationStatus   A 1-byte status set by the client application that
 * shall be reported to the server.
 *
 * @return TRUE if the application accepted the request of bootloading the
 * specified image at the requested time, FALSE otherwise.
 */
bool emberAfPluginOtaUnicastBootloaderClientIncomingRequestBootloadCallback(
  EmberNodeId serverId,
  uint8_t imageTag,
  uint32_t bootloadDelayMs,
  uint8_t *applicationStatus
  );

/** @} // END addtogroup
 */

#endif // _OTA_UNICAST_BOOTLOADER_CLIENT_H_
