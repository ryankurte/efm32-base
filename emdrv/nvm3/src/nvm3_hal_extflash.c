/***************************************************************************//**
 * @file
 * @brief Non-Volatile Memory Wear-Leveling driver HAL implementation
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

#include <stdbool.h>
#include <string.h>
#include "nvm3.h"
#include "nvm3_hal_extflash.h"
#include "em_system.h"
#include "em_msc.h"
#include "api/btl_interface.h"

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
 * @{
 ******************************************************************************/

/******************************************************************************
 ******************************    MACROS    **********************************
 *****************************************************************************/

#define CACHE_ADR_INVALID       (void *)(-1)
#define CACHE_SIZE              (64)          // MUST be a power of 2

/******************************************************************************
 ***************************   LOCAL VARIABLES   ******************************
 *****************************************************************************/

static BootloaderInformation_t          btlInfo;        ///< Bootloader information
static BootloaderStorageInformation_t   btlStorageInfo; ///< Bootloader storage information

#if CACHE_SIZE > 0
static uint8_t cacheBuf[CACHE_SIZE];
static void *cacheAdr;
#endif

/******************************************************************************
 ***************************   LOCAL FUNCTIONS   ******************************
 *****************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

static Ecode_t btlToNvm3Sta(uint32_t btlSta)
{
  Ecode_t sta;

  switch (btlSta) {
    case BOOTLOADER_OK:
      sta = ECODE_NVM3_OK;
      break;

    default:
      sta = ECODE_NVM3_ERR_NVM_ACCESS;
  }

  return sta;
}

static void btlInfoClr(void)
{
  memset(&btlInfo, 0, sizeof(btlInfo));
  memset(&btlStorageInfo, 0, sizeof(btlStorageInfo));
}

static Ecode_t btlEnableAccess(void)
{
  Ecode_t sta = ECODE_NVM3_OK;
  uint32_t btlSta;

  bootloader_getInfo(&btlInfo);
  if (btlInfo.type == NO_BOOTLOADER) {
    sta = ECODE_NVM3_ERR_NVM_NOT_AVAILABLE;
  }
  if (sta == ECODE_NVM3_OK) {
    btlSta = bootloader_init();
    if (btlSta != BOOTLOADER_OK) {
      sta = ECODE_NVM3_ERR_NVM_NOT_AVAILABLE;
    }
  }

  return sta;
}

static void btlDisableAccess(void)
{
  // This file supports ZIGBEE ONLY!
  // Bootloader deinit is done by the stack.
  // bootloader_deinit();
}

static Ecode_t btlOpen(void)
{
  Ecode_t sta = ECODE_NVM3_OK;

  btlInfoClr();

  sta = btlEnableAccess();
  if (sta == ECODE_NVM3_OK) {
    bootloader_getStorageInfo(&btlStorageInfo);
  }

  return sta;
}

static void btlClose(void)
{
  btlDisableAccess();
}

/** @endcond */

static Ecode_t halExtFlashOpen(nvm3_HalPtr_t nvmAdr, size_t flashSize)
{
  Ecode_t sta;

  sta = btlOpen();
  if (sta == ECODE_NVM3_OK) {
    if (((size_t)nvmAdr + flashSize) > btlStorageInfo.info->partSize) {
      sta = ECODE_NVM3_ERR_ADDRESS_RANGE;
    }
    btlClose();
  }

#if CACHE_SIZE > 0
  cacheAdr = CACHE_ADR_INVALID;
#endif

  return sta;
}

static void halExtFlashClose(void)
{
  // Note: halExtFlashOpen() has already called btlClose()
  //btlClose();
}

static Ecode_t halExtFlashGetInfo(nvm3_HalInfo_t *halInfo)
{
  SYSTEM_ChipRevision_TypeDef chipRev;
  Ecode_t sta;

  SYSTEM_ChipRevisionGet(&chipRev);
  halInfo->deviceFamily = chipRev.family;
  halInfo->memoryMapped = 0;
  sta = btlOpen();
  if (sta == ECODE_NVM3_OK) {
    halInfo->pageSize = btlStorageInfo.info->pageSize;
    btlClose();
  }
  // Force the write size to 32-bit.
  halInfo->writeSize = NVM3_HAL_WRITE_SIZE_32;
  halInfo->systemUnique = SYSTEM_GetUnique();

  return sta;
}

static void nvm3_halExtFlashAccess(nvm3_HalNvmAccessCode_t access)
{
  switch (access) {
    case NVM3_HAL_NVM_ACCESS_NONE:
      btlDisableAccess();
      break;

    case NVM3_HAL_NVM_ACCESS_RD:
    case NVM3_HAL_NVM_ACCESS_RDWR:
      btlEnableAccess();
      break;

    default:
      break;
  }
}

Ecode_t halExtFlashReadWords(nvm3_HalPtr_t nvmAdr, void *dst, size_t wordCnt)
{
  uint32_t btlSta;
#if CACHE_SIZE > 0
  Ecode_t sta = ECODE_NVM3_OK;
  void *pageAdr;
  size_t len;
  size_t ofs;

  while (wordCnt > 0) {
    pageAdr = (void *)((size_t)nvmAdr & ~(CACHE_SIZE - 1));
    ofs = ((size_t)nvmAdr - (size_t)pageAdr);
    len = (wordCnt * 4) > (CACHE_SIZE - ofs) ? (CACHE_SIZE - ofs) : wordCnt * 4;
    if (cacheAdr != pageAdr) {
      btlSta = bootloader_readRawStorage((uint32_t)pageAdr, cacheBuf, CACHE_SIZE);
      sta = btlToNvm3Sta(btlSta);
      cacheAdr = (sta == ECODE_NVM3_OK) ? pageAdr : CACHE_ADR_INVALID;
      if (sta != ECODE_NVM3_OK) {
        return sta;
      }
    }
    memcpy(dst, cacheBuf + ofs, len);
    wordCnt -= (len / 4);
    nvmAdr = (nvm3_HalPtr_t)((size_t)nvmAdr + len);
    dst = (void *)((size_t)dst + len);
  }
  return sta;
#else
  btlSta = bootloader_readRawStorage((uint32_t)nvmAdr, dst, wordCnt * 4);
  return btlToNvm3Sta(btlSta);
#endif
}

static Ecode_t halExtFlashWriteWords(nvm3_HalPtr_t nvmAdr, void const *src, size_t wordCnt)
{
  uint32_t btlSta;

#if CACHE_SIZE > 0
  cacheAdr = CACHE_ADR_INVALID;
#endif

  btlSta = bootloader_writeRawStorage((uint32_t)nvmAdr, (uint8_t *)src, wordCnt * 4);
  return btlToNvm3Sta(btlSta);
}

static Ecode_t halExtFlashPageErase(nvm3_HalPtr_t nvmAdr)
{
  uint32_t btlSta;

#if CACHE_SIZE > 0
  cacheAdr = CACHE_ADR_INVALID;
#endif

  btlSta = bootloader_eraseRawStorage((uint32_t)nvmAdr, btlStorageInfo.info->pageSize);
  return btlToNvm3Sta(btlSta);
}

/*******************************************************************************
 ***************************   GLOBAL VARIABLES   ******************************
 ******************************************************************************/

const nvm3_HalHandle_t nvm3_halExtFlashHandle = {
  .open = halExtFlashOpen,
  .close = halExtFlashClose,
  .getInfo = halExtFlashGetInfo,
  .access = nvm3_halExtFlashAccess,
  .pageErase = halExtFlashPageErase,
  .readWords = halExtFlashReadWords,
  .writeWords = halExtFlashWriteWords,
};

/** @} (end addtogroup NVM3Hal) */
/** @} (end addtogroup NVM3) */
/** @} (end addtogroup emdrv) */
