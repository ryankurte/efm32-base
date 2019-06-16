/***************************************************************************//**
 * @file
 * @brief NVM3 driver HAL
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef NVM3_HAL_H
#define NVM3_HAL_H

#include "ecode.h"
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#ifdef NVM3_HOST_BUILD
#include "nvm3_hal_host.h"
#else
#include "em_assert.h"
#include "em_common.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup NVM3
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup NVM3Hal
 * @brief NVM3 HAL module
 * @{
 * @details
 * This module provides the interface to the NVM. By having all NVM access
 * functions in a separate file, it is possible to support different hardware
 * by substituting the functions in this module.
 *
 * @note These functions are used by the NVM3 and should not be used by
 * any applications.
 ******************************************************************************/

/******************************************************************************
 ******************************    MACROS    **********************************
 *****************************************************************************/

#define NVM3_HAL_WRITE_SIZE_32    0     ///< Only single writes are allowed
#define NVM3_HAL_WRITE_SIZE_16    1     ///< Two writes are allowed

#define NVM3_HAL_NVM_ACCESS_NONE  0
#define NVM3_HAL_NVM_ACCESS_RD    1
#define NVM3_HAL_NVM_ACCESS_RDWR  2
#define NVM3_HAL_NVM_ACCESS_NOP   3

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

#define nvm3_halOpen(hal, a, b)           hal->open(a, b)
#define nvm3_halClose(hal)                hal->close()
#define nvm3_halGetInfo(hal, a)           hal->getInfo(a)
#define nvm3_halNvmAccess(hal, a)         hal->access(a)
#define nvm3_halReadWords(hal, a, b, c)   hal->readWords(a, b, c)
#define nvm3_halWriteWords(hal, a, b, c)  hal->writeWords(a, b, c)
#define nvm3_halPageErase(hal, a)         hal->pageErase(a)

/// @endcond

/******************************************************************************
 ******************************   TYPEDEFS   **********************************
 *****************************************************************************/

/// @brief Pointer to NVM
typedef void   *nvm3_HalPtr_t;

/// @brief Device NVM capabilities

typedef struct nvm3_HalInfo {
  uint16_t deviceFamily;
  uint8_t writeSize;
  uint8_t memoryMapped;
  size_t pageSize;
  uint64_t systemUnique;
} nvm3_HalInfo_t;

typedef uint8_t nvm3_HalNvmAccessCode_t;

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *  Open the NVM3 HAL for usage.
 *
 * @details
 *   This function must be run at initialization, before any other functions
 *   are called. It is used to call necessary startup routines before the
 *   hardware can be accessed.
 *
 * @param[in] nvmAdr
 *   A pointer to the destination in NVM.
 *
 * @param[in] nvmSize
 *   The total size of the NVM.
 *
 * @return
 *   The result of the open call.
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
typedef Ecode_t (*nvm3_HalOpen_t)(nvm3_HalPtr_t nvmAdr, size_t nvmSize);

/***************************************************************************//**
 * @brief
 *  Close the NVM3 HAL for usage.
 *
 * @details
 *   This function should be called at program termination.
 *   Should be done before any graceful halts.
 ******************************************************************************/
typedef void(*nvm3_HalClose_t)(void);

/***************************************************************************//**
 * @brief
 *   Retrieve device information.
 *
 * @details
 *   This function is used to retrieve information about the device properties,
 *   such as the device family, write size, whether the NVM is memory mapped or
 *   not, and finally the NVM page size.
 *
 * @param[in] deviceInfo
 *   A pointer to a structure that will receive the device information.
 ******************************************************************************/
typedef Ecode_t (*nvm3_HalGetInfo_t)(nvm3_HalInfo_t *info);

/***************************************************************************//**
 * @brief
 *   Control read and write access to the NVM.
 *
 * @details
 *   This function is used to control the access to the NVM. It can be either
 *   read, write, or none.
 *
 * @param[in] access
 *   The requested access.
 ******************************************************************************/
typedef void (*nvm3_HalNvmAccess_t)(nvm3_HalNvmAccessCode_t access);

/***************************************************************************//**
 * @brief
 *   Erase a page in the NVM.
 *
 * @details
 *   This function is used to erase an NVM page.
 *
 * @param[in] nvmAdr
 *   A memory address pointing to the start of the page to erase.
 *
 * @return
 *   The result of the erase operation.
 ******************************************************************************/
typedef Ecode_t (*nvm3_HalPageErase_t)(nvm3_HalPtr_t nvmAdr);

/***************************************************************************//**
 * @brief
 *   Read data from NVM.
 *
 * @details
 *   This function is used to read data from the NVM. It will be a
 *   blocking call, since the thread asking for data to be read cannot continue
 *   without the data.
 *
 * @param[in] nvmAdr
 *   A memory address in NVM where data will be read.
 *
 * @param[in] *pDst
 *   A pointer to the destination buffer.
 *
 * @param[in] cnt
 *   The number of words to read.
 ******************************************************************************/
typedef Ecode_t (*nvm3_HalReadWords_t)(nvm3_HalPtr_t nvmAdr, void *dst, size_t wordCnt);

/***************************************************************************//**
 * @brief
 *   Write data to the NVM.
 *
 * @details
 *   This function is used to write data to the NVM. This is a blocking
 *   function.
 *
 * @param[in] nvmAdr
 *   A memory address in NVM where data will be written.
 *
 * @param[in] *pSrc
 *   A pointer to the source data.
 *
 * @param[in] cnt
 *   The number of words to write.
 *
 * @return
 *   The result of the write operation.
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
typedef Ecode_t (*nvm3_HalWriteWords_t)(nvm3_HalPtr_t nvmAdr, void const *pSrc, size_t cnt);

typedef struct {
  nvm3_HalOpen_t          open;
  nvm3_HalClose_t         close;
  nvm3_HalGetInfo_t       getInfo;
  nvm3_HalNvmAccess_t     access;
  nvm3_HalPageErase_t     pageErase;
  nvm3_HalReadWords_t     readWords;
  nvm3_HalWriteWords_t    writeWords;
} nvm3_HalHandle_t;

/** @} (end addtogroup NVM3Hal) */
/** @} (end addtogroup NVM3) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif /* NVM3_HAL_H */
