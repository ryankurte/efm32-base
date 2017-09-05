/**************************************************************************//**
* @file main.c
* @brief Bootloader main functions.
* @version x.xx
******************************************************************************
* @section License
* <b>Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
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
#ifndef MAIN_H
#define MAIN_H

#include <stdbool.h>
#include <stdint.h>
#include "config.h"

#define SET_PINMODEL(PORT, PIN, MODE, OUT) { GPIO->P[PORT].DOUT |= (OUT) << (PIN); GPIO->P[PORT].MODEL = (GPIO->P[PORT].MODEL & ~(0xF << (PIN * 4))) | (MODE << (PIN * 4)); }
#define BOOTLOADER_VERSION 0x00000005
#define MAIN_APP ((uint32_t*)BOOTLOADER_SIZE)

bool check_image_crc(const uint8_t *address);

#endif
