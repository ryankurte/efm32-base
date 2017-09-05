// Copyright 2016 Silicon Laboratories, Inc.

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "stack/include/ember.h"
#include "hal/hal.h"
#include "debug-print/debug-print.h"
#include "ota-bootloader-test-common/ota-bootloader-test-common.h"

#include EMBER_AF_API_BOOTLOADER_INTERFACE
#include EMBER_AF_API_COMMAND_INTERPRETER2

// The image tag the client shall accept.
uint8_t otaBootloaderTestImageTag = DEFAULT_IMAGE_TAG;

#ifdef EMBER_AF_PLUGIN_OTA_BOOTLOADER_TEST_COMMON_INCLUDE_COMMON_EBL_FILE

#include "bl-test-application-ebl.h"

bool emberAfPluginBootloaderInterfaceWriteEbl()
{
  uint8_t result;
  uint32_t address = 0;
  uint8_t len;
  uint32_t remainingLength;
  const HalEepromInformationType *eepromInfo;

  result = halAppBootloaderInit();

  if (result == EEPROM_SUCCESS) {
    emberAfCorePrintln("Flash initialized");
  } else {
    emberAfCorePrintln("Flash not initialized: 0x%x", result);
    return false;
  }

  eepromInfo = halAppBootloaderInfo();

  if (eepromInfo != NULL) {
    emberAfCorePrintln("Flash type: %s", eepromInfo->partDescription);
  } else {
    emberAfCorePrintln("Flash type unknown");
    return false;
  }

  if (__wire_replacement_ebl_len > eepromInfo->partSize) {
    emberAfCorePrintln("Flash part size is too small!");
    return false;
  }

  emberAfCorePrint("Erasing flash");

  if ( !emberAfPluginBootloaderInterfaceChipErase() ) {
    emberAfCorePrintln("\r\nFlash erase failed");
  } else {
    emberAfCorePrintln("\r\nFlash erase complete");
    emberAfCorePrint("Start writing test image");
  }

  remainingLength = __wire_replacement_ebl_len;

  while (remainingLength) {
    len = (remainingLength > 255) ? 255 : remainingLength;

    if ( !emberAfPluginBootloaderInterfaceWrite(address, len, (uint8_t *)(__wire_replacement_ebl + address)  ) ) {
      return false;
    }

    address += len;
    remainingLength -= len;

    emberAfCorePrintln("Writing flash address: 0x%4x", address);
  }

  emberAfCorePrintln("Flash write completed!");

  return true;
}
#endif // EMBER_AF_PLUGIN_OTA_BOOTLOADER_TEST_COMMON_INCLUDE_COMMON_EBL_FILE

//------------------------------------------------------------------------------
// CLI commands.

void bootloaderGetVersionCommand(void)
{
  uint16_t blVersion;

  emberAfPluginBootloaderInterfaceGetVersion(&blVersion);

  emberAfCorePrintln("bootloader version: %d", blVersion);
}

void bootloaderInitCommand(void)
{
  if (emberAfPluginBootloaderInterfaceInit()) {
    emberAfCorePrintln("bootloader init succeeded!");
  } else {
    emberAfCorePrintln("bootloader init failed! wrong chip?");
  }
}

void bootloaderSleepCommand(void)
{
  emberAfPluginBootloaderInterfaceSleep();
  emberAfCorePrintln("sleep bootloader and flash part");
}

void bootloaderFlashEraseCommand(void)
{
  emberAfCorePrintln("flash erase started");
  emberAfPluginBootloaderInterfaceChipErase();
}

void bootloaderFlashEraseSlotCommand(void)
{
  uint32_t slot = emberUnsignedCommandArgument(0);

  emberAfCorePrintln("flash erasing slot %d started", slot);

  if ( emberAfPluginBootloaderInterfaceChipEraseSlot(slot) ) {
    emberAfCorePrintln("flash erase successful!");
  } else {
    emberAfCorePrintln("flash erase failed!");
  }
}

void bootloaderFlashReadCommand(void)
{
  uint32_t address = emberUnsignedCommandArgument(0);
  uint8_t length = emberUnsignedCommandArgument(1);
  uint8_t buff[255];
  uint8_t i;

  if (emberAfPluginBootloaderInterfaceRead(address, length, buff)) {
    emberAfCorePrintln("flash read succeeded!");
    emberAfCorePrintln("address: %d, length: %d, data:", address, length);
    for (i = 0; i < length; i++) {
      emberAfCorePrint("0x%x ", buff[i]);
    }
    emberAfCorePrintln("");
  } else {
    emberAfCorePrintln("flash read failed!");
  }
}

void bootloaderFlashWriteCommand(void)
{
  uint32_t address = emberUnsignedCommandArgument(0);
  uint8_t length;
  uint8_t *dataBuff = emberStringCommandArgument(1, &length);

  if (emberAfPluginBootloaderInterfaceWrite(address, length, dataBuff)) {
    emberAfCorePrintln("flash write succeeded!");
  } else {
    emberAfCorePrintln("flash write failed!");
  }
}

void bootloaderWriteTestEblCommand(void)
{
#ifdef EMBER_AF_PLUGIN_OTA_BOOTLOADER_TEST_COMMON_INCLUDE_COMMON_EBL_FILE
  if (emberAfPluginBootloaderInterfaceWriteEbl()) {
    emberAfCorePrintln("bootloader: writing EBL file succeeded!");
  } else {
    emberAfCorePrintln("bootloader: writing EBL file failed!");
  }
#else
  emberAfCorePrintln("Test ebl image not enabled in plugin options!");
#endif
}

void bootloaderValidateImage(void)
{
  if (!emberAfPluginBootloaderInterfaceValidateImage()) {
    emberAfCorePrintln("Image is invalid!");
  } else {
    emberAfCorePrintln("Image is valid!");
  }
}

void bootloaderFlashImage(void)
{
  if (!emberAfPluginBootloaderInterfaceIsBootloaderInitialized()) {
    if (!emberAfPluginBootloaderInterfaceInit()) {
      emberAfCorePrintln("bootloader init failed");
      return;
    }
  }

  emberAfPluginBootloaderInterfaceBootload();

  // If we get here bootload process failed.
  emberAfCorePrintln("bootload failed!");
}

void otaBootloaderSetImageTag(void)
{
  otaBootloaderTestImageTag = emberUnsignedCommandArgument(0);
  emberAfCorePrintln("image tag set");
}
