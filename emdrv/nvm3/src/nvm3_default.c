/***************************************************************************//**
 * @file
 * @brief NVM3 definition of the default data structures.
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

#include "nvm3.h"
#include "nvm3_hal_flash.h"

#ifndef NVM3_DEFAULT_CACHE_SIZE
#define NVM3_DEFAULT_CACHE_SIZE  100
#endif
#ifndef NVM3_DEFAULT_NVM_SIZE
#define NVM3_DEFAULT_NVM_SIZE  36864
#endif
#ifndef NVM3_DEFAULT_MAX_OBJECT_SIZE
#define NVM3_DEFAULT_MAX_OBJECT_SIZE  NVM3_MAX_OBJECT_SIZE
#endif
#ifndef NVM3_DEFAULT_REPACK_HEADROOM
#define NVM3_DEFAULT_REPACK_HEADROOM  0
#endif

#ifndef NVM3_BASE

#if defined (__ICCARM__)

#ifndef __NVM3__
#define __NVM3__ "SIMEE"
#endif

__root uint8_t nvm3Storage[NVM3_DEFAULT_NVM_SIZE] @ __NVM3__;
#define NVM3_BASE (nvm3Storage)

#elif defined (__GNUC__)

#ifndef __NVM3__
#define __NVM3__ ".simee"
#endif

__attribute__((used)) uint8_t nvm3Storage[NVM3_DEFAULT_NVM_SIZE] __attribute__ ((section(__NVM3__)));
/* If linker does not provide __nvm3Base symbol, then use nvm3Storage*/
extern char __nvm3Base __attribute__((alias("nvm3Storage")));
#define NVM3_BASE (&__nvm3Base)

#else
#error "Unsupported toolchain"
#endif

#endif //NVM3_BASE

uint8_t       *nvm3Address = (uint8_t *)NVM3_BASE;
nvm3_Handle_t  nvm3_defaultHandleData;
nvm3_Handle_t *nvm3_defaultHandle = &nvm3_defaultHandleData;

static nvm3_CacheEntry_t defaultCache[NVM3_DEFAULT_CACHE_SIZE];

nvm3_Init_t    nvm3_defaultInitData =
{
  (nvm3_HalPtr_t)NVM3_BASE,
  NVM3_DEFAULT_NVM_SIZE,
  defaultCache,
  NVM3_DEFAULT_CACHE_SIZE,
  NVM3_DEFAULT_MAX_OBJECT_SIZE,
  NVM3_DEFAULT_REPACK_HEADROOM,
  &nvm3_halFlashHandle,
#ifdef NVM3_SUPPORT_ENCRYPTION
  NULL,
#endif
};

nvm3_Init_t   *nvm3_defaultInit = &nvm3_defaultInitData;
