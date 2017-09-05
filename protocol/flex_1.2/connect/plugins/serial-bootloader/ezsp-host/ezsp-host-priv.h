/***************************************************************************//**
 * @file ezsp-host-priv.h
 * @brief Private header for EZSP Host functions
 *
 * This file should be included only by ezsp-host-ui.c and ash-host.c.
 *
 * See @ref ezsp_util for documentation.
 *
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
/** @addtogroup ezsp_util
 *
 * See ezsp-host-priv.h.
 *
 *@{
 */

#ifndef __EZSP_HOST_PRIV_H__
#define __EZSP_HOST_PRIV_H__

// Defined in ezsp-host-ui.c
void ezspPrintElapsedTime(void);
void ezspTraceEzspFrameId(const char *message, uint8_t *ezspFrame);
void ezspTraceEzspVerbose(char *format, ...);

#ifdef EZSP_ASH
  #include "ezsp-host/ash/ash-host-priv.h"
#endif

#endif //__EZSP_HOST_PRIV_H___

/** @} // END addtogroup
 */
