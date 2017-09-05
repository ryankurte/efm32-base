/**************************************************************************//**
* @file flash.h
* @brief Bootloader flash writing functions.
* @version x.xx
******************************************************************************
* @section License
* <b>Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com</b>
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

#ifndef FLASH_H
#define FLASH_H

#ifdef __ICCARM__
#define RAMFUNC __ramfunc
#else
/*GCC ramfuncs are handled in linker*/
#define RAMFUNC __attribute__ ((section(".ram")))
#endif
/*
 * Flash programming hardware interface
 *
 */

/* Helper functions */
RAMFUNC void FLASH_writeWord(uint32_t address, uint32_t data);
RAMFUNC void FLASH_writeBlock(void *block_start,
                              uint32_t count,
                              uint8_t const *buffer);
RAMFUNC void FLASH_eraseOneBlock(uint32_t blockStart);
void FLASH_init(void);
void FLASH_CalcPageSize(void);

extern uint32_t flashPageSize;
#endif
