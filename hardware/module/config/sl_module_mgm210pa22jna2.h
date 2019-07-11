/***************************************************************************//**
 * Module Configuration Header
 * Copyright 2018 Silicon Laboratories, Inc.
 *
 * # License
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.@n
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.@n
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Laboratories, Inc.
 * has no obligation to support this Software. Silicon Laboratories, Inc. is
 * providing the Software "AS IS", with no express or implied warranties of any
 * kind, including, but not limited to, any implied warranties of
 * merchantability or fitness for any particular purpose or warranties against
 * infringement of any proprietary rights of a third party.
 *
 * Silicon Laboratories, Inc. will not be liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this Software.
 ******************************************************************************/
 
#ifndef SL_MODULE_MGM210PA22JNA2_H
#define SL_MODULE_MGM210PA22JNA2_H

#include "em_cmu.h"
#include "em_emu.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef BSP_CLK_HFXO_CTUNE_TOKEN
#define BSP_CLK_HFXO_CTUNE_TOKEN	1U 
#endif //BSP_CLK_HFXO_CTUNE_TOKEN
#define BSP_CLK_HFXO_FREQ 			38400000UL
#define BSP_CLK_HFXO_INIT 			CMU_HFXOINIT_DEFAULT
#define BSP_CLK_HFXO_PRESENT 		1U

#ifndef BSP_PA_VOLTAGE
#define BSP_PA_VOLTAGE 			3300U
#endif //BSP_PA_VOLTAGE

#ifdef __cplusplus
}
#endif

#endif // SL_MODULE_MGM210PA22JNA2_H
