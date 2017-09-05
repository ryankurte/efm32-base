// Copyright 2016 Silicon Laboratories, Inc.

#ifndef _OTA_BOOTLOADER_TEST_COMMON_H_
#define _OTA_BOOTLOADER_TEST_COMMON_H_

// Application error status codes.
#define APPLICATION_STATUS_FLASH_INIT_FAILED     0xA0
#define APPLICATION_STATUS_FLASH_WRITE_FAILED    0xA1
#define APPLICATION_STATUS_WRONG_IMAGE_TAG       0xA2

// Arbitrary image tag default value.
#define DEFAULT_IMAGE_TAG                        0x89

// Common OTA bootloader Image Tag.
extern uint8_t otaBootloaderTestImageTag;

#ifdef EMBER_AF_PLUGIN_OTA_BOOTLOADER_TEST_COMMON_INCLUDE_COMMON_EBL_FILE
bool emberAfPluginBootloaderInterfaceWriteEbl(void);
#endif // EMBER_AF_PLUGIN_OTA_BOOTLOADER_TEST_COMMON_INCLUDE_COMMON_EBL_FILE

#endif // _OTA_BOOTLOADER_TEST_COMMON_H_
