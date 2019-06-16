/***************************************************************************//**
 * @file
 * @brief Source file for RAIL HAL common functionality
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

#include "hal_common.h"
#include "rail_types.h"

#include "em_common.h"
#include "em_cmu.h"

SL_WEAK void halInitChipSpecific(void)
{
  // Do nothing, but allow each chip to override this if necessary
}

void halInit(void)
{
  // Call the chip specific hal init
  halInitChipSpecific();
}

void halCommonMemMove(void *dest, const void *src, uint16_t bytes)
{
  uint8_t *d = (uint8_t *)dest;
  uint8_t *s = (uint8_t *)src;

  if (d > s) {
    d += bytes - 1;
    s += bytes - 1;
    #ifndef _HAL_SMALL_MEMUTILS_
    while (bytes >= 4) {
      bytes -= 4;
      *d-- = *s--;
      *d-- = *s--;
      *d-- = *s--;
      *d-- = *s--;
    }
    #endif // _HAL_SMALL_MEMUTILS_
    for (; bytes; bytes--) {
      *d-- = *s--;
    }
  } else {
    #ifndef _HAL_SMALL_MEMUTILS_
    while (bytes >= 4) {
      bytes -= 4;
      *d++ = *s++;
      *d++ = *s++;
      *d++ = *s++;
      *d++ = *s++;
    }
    #endif // _HAL_SMALL_MEMUTILS_
    for (; bytes; bytes--) {
      *d++ = *s++;
    }
  }
}
