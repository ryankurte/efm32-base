/***************************************************************************//**
 * @file ezsp-host-ui.h
 * @brief Header for EZSP Host user interface functions
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
 * See ezsp-host-ui.h.
 *
 *@{
 */

#ifndef __EZSP_HOST_UI_H__
#define __EZSP_HOST_UI_H__

/** @brief Prints usage instructions to stderr.
 *
 * @param name  program name (usually argv[0])
 */
void ezspPrintUsage(char *name);

/** @brief Sets host configuration values from command line options.
 *
 * @param argc  number of command line tokens
 *
 * @param argv  array of pointer to command line tokens
 *
 * @return  true if no errors were detected in the command line
 */
bool ezspProcessCommandOptions(int argc, char *argv[]);

/** @brief Writes a debug trace message, if enabled.
 *
 * @param string  pointer to message string
 *
 * @return
 * - ::EZSP_SUCCESS
 * - ::EZSP_NO_RX_DATA
 */
void ezspTraceEvent(const char *string);

/** @brief  Converts EZSP error code to a string.
 *
 * @param error  error or reset code (from hostError or ncpError)
 *
 * @return  pointer to the string
 */
const uint8_t* ezspErrorString(uint8_t error);

#ifdef EZSP_ASH
  #define BUMP_HOST_COUNTER(mbr) do { ashCount.mbr++; } while (0)
  #define ADD_HOST_COUNTER(op, mbr) do { ashCount.mbr += op; }  while (0)
  #include "ezsp-host/ash/ash-host.h"
  #include "ezsp-host/ash/ash-host-ui.h"
  #define readConfig(x) ashReadConfig(x)
#elif defined(EZSP_USB)
  #include "ezsp-host/usb/usb-host.h"
  #include "ezsp-host/usb/usb-host-ui.h"
  #define readConfig(x) usbReadConfig(x)
  #define BUMP_HOST_COUNTER(mbr)
  #define ADD_HOST_COUNTER(op, mbr)
#elif defined(EZSP_SPI)
  #include "ezsp-host/spi/spi-host.h"
  #include "ezsp-host/spi/spi-host-ui.h"
  #define readConfig(x) spiReadConfig(x)
  #define BUMP_HOST_COUNTER(mbr)
  #define ADD_HOST_COUNTER(op, mbr)
#endif
#endif //__EZSP_HOST_UI_H___

/** @} // END addtogroup
 */
