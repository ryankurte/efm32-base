// Copyright 2016 Silicon Laboratories, Inc.

#ifndef _OTA_UNICAST_BOOTLOADER_TYPES_H_
#define _OTA_UNICAST_BOOTLOADER_TYPES_H_

/**
 * @addtogroup ota_unicast_bootloader_common
 * @{
 */

/**
 * @brief Ota Unicast Bootloader return status codes.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberAfOtaUnicastBootloaderStatus
#else
typedef uint8_t EmberAfOtaUnicastBootloaderStatus;
enum
#endif
{
  EMBER_OTA_UNICAST_BOOTLOADER_STATUS_SUCCESS                        = 0x00,
  EMBER_OTA_UNICAST_BOOTLOADER_STATUS_INVALID_CALL                   = 0x01,
  EMBER_OTA_UNICAST_BOOTLOADER_STATUS_BUSY                           = 0x02,
  EMBER_OTA_UNICAST_BOOTLOADER_STATUS_DATA_UNDERFLOW                 = 0x03,
  EMBER_OTA_UNICAST_BOOTLOADER_STATUS_STACK_ERROR                    = 0x04,
  EMBER_OTA_UNICAST_BOOTLOADER_STATUS_TIMEOUT                        = 0x05,
  EMBER_OTA_UNICAST_BOOTLOADER_STATUS_FAILED                         = 0x06,
  EMBER_OTA_UNICAST_BOOTLOADER_STATUS_ABORTED                        = 0x07,
  EMBER_OTA_UNICAST_BOOTLOADER_STATUS_REFUSED                        = 0x08,
  EMBER_OTA_UNICAST_BOOTLOADER_STATUS_UNREACHABLE                    = 0x09,
};

/**
 * @brief Ota Unicast Bootloader target status codes.
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
enum EmberAfOtaUnicastBootloaderTargetStatus
#else
typedef uint8_t EmberAfOtaUnicastBootloaderTargetStatus;
enum
#endif
{
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_INVALID                        = 0x00,
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_UNREACHABLE                    = 0x01,
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_COMPLETED         = 0x02,
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_ONGOING           = 0x03,
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_FAILED            = 0x04,
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_REFUSED           = 0x05,
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_ABORTED           = 0x06,
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_DISTRIBUTION_OUT_OF_SEQ        = 0x07,
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_BOOTLOAD_REQUEST_ACCEPTED      = 0x08,
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_BOOTLOAD_REQUEST_ONGOING       = 0x09,
  EMBER_OTA_UNICAST_BOOTLOADER_TARGET_STATUS_BOOTLOAD_REQUEST_REFUSED       = 0x0A,
};

/**
 * @brief A value indicating that client application did not set the application
 * level target status in any of the client callbacks.
 */
#define EMBER_AF_PLUGIN_OTA_UNICAST_BOOTLOADER_INVALID_APPLICATION_TARGET_STATUS    0xFF

/** @} // END addtogroup
 */

#endif // _OTA_UNICAST_BOOTLOADER_TYPES_H_
