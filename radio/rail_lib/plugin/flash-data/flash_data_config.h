/***************************************************************************//**
 * @file
 * @brief Header file for RAIL Flash Data Configurations
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

#ifndef __FLASHDATACONFIG_PLUGIN_H__
#define __FLASHDATACONFIG_PLUGIN_H__

#include "stdint.h"

#include "em_common.h"
#include "em_device.h"

/**
 * Include the APP configuration header file
 */
#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

/**
 * The page size of flash for the current hardware. For example, the value
 * FLASH_PAGE_SIZE can be used from em_device.h.
 */
#ifndef FLASH_DATA_PAGE_SIZE
#define FLASH_DATA_PAGE_SIZE FLASH_PAGE_SIZE
#endif

/**
 * A randomized 32-bit (word-sized) value stored at the beginning of flash
 * intended for flash usage purposes. Whether or not this value exists in flash
 * indicates if a particular flash location was previously used to save data.
 */
#ifndef FLASH_DATA_PREFIX
#define FLASH_DATA_PREFIX 0xD505DA05UL
#endif

/**
 * The default byte array to be written into flash.
 *
 * Note: In order for an application to use the define FLASH_DATA_BYTE_ARRAY,
 * the application must not use the define FLASH_DATA_BASE_ADDR.
 * Setting FLASH_DATA_BYTE_ARRAY to "" by default still causes a '\0'
 * character to be written to flash, so FD_GetLength() will return 1.
 */
#ifndef FLASH_DATA_BYTE_ARRAY
#define FLASH_DATA_BYTE_ARRAY ""
#endif

#endif // __FLASHDATACONFIG_PLUGIN_H__
