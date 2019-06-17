/***************************************************************************//**
 * @file
 * @brief NVM3 definition of the default data structures in external flash.
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
#include "nvm3_hal_extflash.h"

#ifndef NVM3_DEFAULT_CACHE_SIZE
#define NVM3_DEFAULT_CACHE_SIZE  100
#endif
#ifndef NVM3_DEFAULT_NVM_START
#define NVM3_DEFAULT_NVM_START 0
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

nvm3_Handle_t  nvm3_defaultHandleData;
nvm3_Handle_t *nvm3_defaultHandle = &nvm3_defaultHandleData;

static nvm3_CacheEntry_t defaultCache[NVM3_DEFAULT_CACHE_SIZE];

nvm3_Init_t   nvm3_defaultInitData =
{
  (nvm3_HalPtr_t) NVM3_DEFAULT_NVM_START,
  NVM3_DEFAULT_NVM_SIZE,
  defaultCache,
  NVM3_DEFAULT_CACHE_SIZE,
  NVM3_DEFAULT_MAX_OBJECT_SIZE,
  NVM3_DEFAULT_REPACK_HEADROOM,
  &nvm3_halExtFlashHandle,
  NULL
};

nvm3_Init_t   *nvm3_defaultInit = &nvm3_defaultInitData;
