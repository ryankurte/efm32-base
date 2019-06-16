/***************************************************************************//**
 * @file
 * @brief Source file for RAIL Flash Data functionality
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#include <string.h>

#include "rail_types.h"
#include "rail.h"

#include "flash_data_config.h"
#include "flash_data.h"

#include "em_msc.h"

#define FLASH_DEFAULT_VALUE 0xFFFFFFFFUL

// The entire size of FD_Structure must be the size of a flash page.
#define FLASH_DATA_BYTE_ARRAY_LENGTH ((uint32_t)FLASH_DATA_PAGE_SIZE \
                                      - sizeof(uint32_t)             \
                                      - sizeof(uint32_t))

// This structure is mapped to flash.
typedef struct FD_Structure {
  uint32_t prefix;
  uint32_t length;
  uint8_t byteArray[FLASH_DATA_BYTE_ARRAY_LENGTH];
} FD_Structure_t;

/**
 * FLASH_DATA_BASE_ADDR: The base address in flash available for saving
 * application data.
 *
 * Note: USERDATA_BASE can be used from em_device.h, but know that this
 * location may already be used by other software.
 */
#ifndef FLASH_DATA_BASE_ADDR
// Align this array to a flash page boundary.
SL_ALIGN(FLASH_DATA_PAGE_SIZE)
const FD_Structure_t flash_data_structure
SL_ATTRIBUTE_ALIGN(FLASH_DATA_PAGE_SIZE) =
{
  FLASH_DATA_PREFIX,
  sizeof(FLASH_DATA_BYTE_ARRAY),
  FLASH_DATA_BYTE_ARRAY
};
#define FLASH_DATA_BASE_ADDR (&flash_data_structure)
#endif

/**
 * Flash layout (with a uint32_t pointer pointing to the base flash address):
 * ptr[0] = (uint32_t)FLASH_DATA_PREFIX
 * ptr[1] = (uint32_t)byte_array_length
 * ptr[2] = first word of byte array (i.e. first 4 bytes of the byte array)
 * ...
 * ptr[byte_array_length_rounded / 4] = remaining bytes of the byte array
 *
 * Note: When writing to flash, byte_array_length is rounded up to the nearest
 * multiple of 4 (e.g. byte_array_length_rounded) in order to only write
 * uint32_t word sizes to flash.
 * For example, if byte_array_length = 8, 4 uint32_t flash locations will be used:
 *   ptr[0] = (uint32_t)FLASH_DATA_PREFIX
 *   ptr[1] = 0x00000008 // length = 8 bytes
 *   ptr[2] = 0xXXXXXXXX // bytes 0-3 of the byte array
 *   ptr[3] = 0xXXXXXXXX // bytes 4-7 of the byte array
 * For example, if byte_array_length = 9, 5 uint32_t flash locations will be used:
 *   ptr[0] = (uint32_t)FLASH_DATA_PREFIX
 *   ptr[1] = 0x00000009 // length = 9 bytes
 *   ptr[2] = 0xXXXXXXXX // bytes 0-3 of the byte array
 *   ptr[3] = 0xXXXXXXXX // bytes 4-7 of the byte array
 *   ptr[4] = 0x------XX // byte 8 of the byte array + 3 irrelevant bytes
 */
volatile FD_Structure_t *pFlash = (FD_Structure_t *)(FLASH_DATA_BASE_ADDR);
static const uint32_t flashPrefix = FLASH_DATA_PREFIX;

RAIL_Status_t FD_ReadData(uint8_t **data, uint32_t *len)
{
  if (NULL == data) {
    return RAIL_STATUS_INVALID_PARAMETER; // invalid input pointer
  }
  if (FD_GetLength() == 0UL) {
    return RAIL_STATUS_INVALID_CALL; // no application data exists
  }
  // Return valid data length and address (ignoring flashPrefix).
  if (NULL != len) {
    *len = pFlash->length;
  }
  *data = (uint8_t *)(&(pFlash->byteArray[0]));
  return RAIL_STATUS_NO_ERROR;
}

uint32_t FD_GetLength(void)
{
  uint32_t result = 0UL;
  if (pFlash->prefix == flashPrefix
      && pFlash->length != FLASH_DEFAULT_VALUE) {
    result = pFlash->length;
  }
  return result;
}

uint32_t FD_GetMaxLength(void)
{
  return FLASH_DATA_BYTE_ARRAY_LENGTH;
}

RAIL_Status_t FD_WriteData(uint8_t *data, uint32_t len)
{
  RAIL_Status_t status = RAIL_STATUS_INVALID_PARAMETER;
  MSC_Status_TypeDef mscStatus;

  // Ensure that the data to be saved fits in the flash page size,
  // minus size of prefix (FLASH_DATA_PREFIX),
  // minus size of data length (len),
  // minus 0-3 bytes for uint32_t word alignment.
  if ((NULL == data)
      || (len > (uint32_t)FLASH_DATA_PAGE_SIZE
          - sizeof(flashPrefix) - sizeof(len) - (len % 4))) {
    return RAIL_STATUS_INVALID_PARAMETER; // data too large
  }

  // Erase flash page,
  // write data prefix (FLASH_DATA_PREFIX) to flash,
  // write data length (len) to flash,
  // and write the data (data) to flash.
  MSC_Init();
  mscStatus = MSC_ErasePage((uint32_t *)pFlash);
  if (mscReturnOk == mscStatus) {
    mscStatus = MSC_WriteWord((uint32_t *)&(pFlash->prefix),
                              &flashPrefix,
                              sizeof(flashPrefix));
    if (mscReturnOk == mscStatus) {
      mscStatus = MSC_WriteWord((uint32_t *)&(pFlash->length),
                                &len,
                                sizeof(len));
      if (mscReturnOk == mscStatus) {
        // Ensure 4-byte alignment; round up.
        uint32_t lenAligned = (len + 0x00000003UL) & 0xFFFFFFFCUL;
        mscStatus = MSC_WriteWord((uint32_t *)(&(pFlash->byteArray[0])),
                                  data,
                                  lenAligned);
        if (mscReturnOk == mscStatus) {
          status = RAIL_STATUS_NO_ERROR;
        }
      }
    }
  }
  MSC_Deinit();

  // Clear the prefix in case it was written without the data itself.
  if (RAIL_STATUS_NO_ERROR != status) {
    FD_ClearData();
  }

  return status;
}

RAIL_Status_t FD_ClearData(void)
{
  RAIL_Status_t status = RAIL_STATUS_INVALID_CALL; // no valid data in flash
  MSC_Status_TypeDef mscStatus;

  MSC_Init();
  mscStatus = MSC_ErasePage((uint32_t *)pFlash);
  if (mscReturnOk == mscStatus) {
    status = RAIL_STATUS_NO_ERROR;
  }
  MSC_Deinit();

  return status;
}
