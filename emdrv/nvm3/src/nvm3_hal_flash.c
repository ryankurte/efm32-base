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
#include "nvm3_hal_flash.h"
#include "em_system.h"
#include "em_msc.h"

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

#define CHECK_DATA  1           ///< Macro defining if data should be checked

/******************************************************************************
 ***************************   LOCAL VARIABLES   ******************************
 *****************************************************************************/

/******************************************************************************
 ***************************   LOCAL FUNCTIONS   ******************************
 *****************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/***************************************************************************//**
 * @brief
 *   Convert return type.
 *
 * @details
 *   This function converts between the return type of the emlib and the
 *   NVM3 API.
 *
 * @param[in] result
 *   Operation result.
 *
 * @return
 *   Returns remapped status code.
 ******************************************************************************/
static Ecode_t convertMscStatusToNvm3Status(MSC_Status_TypeDef result)
{
  Ecode_t ret;

  switch (result) {
    case mscReturnOk:
      ret = ECODE_NVM3_OK;
      break;
    case mscReturnInvalidAddr:
      ret = ECODE_NVM3_ERR_INT_ADDR_INVALID;
      break;
    default:
      ret = ECODE_NVM3_ERR_INT_EMULATOR;
      break;
  }

  return ret;
}

// Check if the page is erased.
static bool isErased(void *adr, size_t len)
{
  size_t i;
  size_t cnt;
  uint32_t *dat = adr;

  cnt = len / sizeof(uint32_t);
  for (i = 0U; i < cnt; i++) {
    if (*dat != 0xFFFFFFFFUL) {
      return false;
    }
    dat++;
  }

  return true;
}

/** @endcond */

static Ecode_t nvm3_halFlashOpen(nvm3_HalPtr_t nvmAdr, size_t flashSize)
{
  (void)nvmAdr;
  (void)flashSize;
  MSC_Init();

  return ECODE_NVM3_OK;
}

static void nvm3_halFlashClose(void)
{
  MSC_Deinit();
}

static Ecode_t nvm3_halFlashGetInfo(nvm3_HalInfo_t *halInfo)
{
  SYSTEM_ChipRevision_TypeDef chipRev;

  SYSTEM_ChipRevisionGet(&chipRev);
  halInfo->deviceFamily = chipRev.family;
  halInfo->memoryMapped = 1;
#if defined(_SILICON_LABS_32B_SERIES_2)
  halInfo->writeSize = NVM3_HAL_WRITE_SIZE_32;
#else
  halInfo->writeSize = NVM3_HAL_WRITE_SIZE_16;
#endif
  halInfo->pageSize = SYSTEM_GetFlashPageSize();
  halInfo->systemUnique = SYSTEM_GetUnique();

  return ECODE_NVM3_OK;
}

static void nvm3_halFlashAccess(nvm3_HalNvmAccessCode_t access)
{
  (void)access;
}

Ecode_t nvm3_halFlashReadWords(nvm3_HalPtr_t nvmAdr, void *dst, size_t wordCnt)
{
  uint32_t *pSrc = (uint32_t *)nvmAdr;
  uint32_t *pDst = dst;

  while (wordCnt > 0U) {
    *pDst++ = *pSrc++;
    wordCnt--;
  }

  return ECODE_NVM3_OK;
}

static Ecode_t nvm3_halFlashWriteWords(nvm3_HalPtr_t nvmAdr, void const *src, size_t wordCnt)
{
  const uint32_t *pSrc = src;
  uint32_t *pDst = (uint32_t *)nvmAdr;
  MSC_Status_TypeDef mscSta;
  Ecode_t halSta;
  size_t byteCnt;

  byteCnt = wordCnt * sizeof(uint32_t);
  mscSta = MSC_WriteWord(pDst, pSrc, byteCnt);
  halSta = convertMscStatusToNvm3Status(mscSta);

#if CHECK_DATA
  if (halSta == ECODE_NVM3_OK) {
    if (memcmp(pDst, pSrc, byteCnt) != 0) {
      halSta = ECODE_NVM3_ERR_WRITE_FAILED;
    }
  }
#endif

  return halSta;
}

static Ecode_t nvm3_halFlashPageErase(nvm3_HalPtr_t nvmAdr)
{
  MSC_Status_TypeDef mscSta;
  Ecode_t halSta;

  mscSta = MSC_ErasePage((uint32_t *)nvmAdr);
  halSta = convertMscStatusToNvm3Status(mscSta);

#if CHECK_DATA
  if (halSta == ECODE_NVM3_OK) {
    if (!isErased(nvmAdr, SYSTEM_GetFlashPageSize())) {
      halSta = ECODE_NVM3_ERR_ERASE_FAILED;
    }
  }
#endif

  return halSta;
}

/*******************************************************************************
 ***************************   GLOBAL VARIABLES   ******************************
 ******************************************************************************/

const nvm3_HalHandle_t nvm3_halFlashHandle = {
  .open = nvm3_halFlashOpen,
  .close = nvm3_halFlashClose,
  .getInfo = nvm3_halFlashGetInfo,
  .access = nvm3_halFlashAccess,
  .pageErase = nvm3_halFlashPageErase,
  .readWords = nvm3_halFlashReadWords,
  .writeWords = nvm3_halFlashWriteWords,
};

/** @} (end addtogroup NVM3Hal) */
/** @} (end addtogroup NVM3) */
/** @} (end addtogroup emdrv) */
