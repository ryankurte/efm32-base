/**************************************************************************//**
 * @file
 * @brief CMSIS Cortex-M Peripheral Access Layer for Silicon Laboratories
 *        microcontroller devices
 *
 * This is a convenience header file for defining the part number on the
 * build command line, instead of specifying the part specific header file.
 *
 * @verbatim
 * Example: Add "-DEFM32G890F128" to your build options, to define part
 *          Add "#include "em_device.h" to your source files

 *
 * @endverbatim
 *
 * @version 5.8.0
 ******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories, Inc. www.silabs.com</b>
 ******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *****************************************************************************/

#ifndef EM_DEVICE_H
#define EM_DEVICE_H
#if defined(MGM210L022JIF1)
#include "mgm210l022jif1.h"

#elif defined(MGM210L022JIF2)
#include "mgm210l022jif2.h"

#elif defined(MGM210L022JNF1)
#include "mgm210l022jnf1.h"

#elif defined(MGM210L022JNF2)
#include "mgm210l022jnf2.h"

#elif defined(MGM210LA22JIF1)
#include "mgm210la22jif1.h"

#elif defined(MGM210LA22JIF2)
#include "mgm210la22jif2.h"

#elif defined(MGM210LA22JNF1)
#include "mgm210la22jnf1.h"

#elif defined(MGM210LA22JNF2)
#include "mgm210la22jnf2.h"

#elif defined(MGM210P022JIA1)
#include "mgm210p022jia1.h"

#elif defined(MGM210P022JIA2)
#include "mgm210p022jia2.h"

#elif defined(MGM210P022JNA2)
#include "mgm210p022jna2.h"

#elif defined(MGM210P032JIA1)
#include "mgm210p032jia1.h"

#elif defined(MGM210P032JIA2)
#include "mgm210p032jia2.h"

#elif defined(MGM210P032JNA2)
#include "mgm210p032jna2.h"

#elif defined(MGM210PA22JIA1)
#include "mgm210pa22jia1.h"

#elif defined(MGM210PA22JIA2)
#include "mgm210pa22jia2.h"

#elif defined(MGM210PA22JNA2)
#include "mgm210pa22jna2.h"

#elif defined(MGM210PA32JIA1)
#include "mgm210pa32jia1.h"

#elif defined(MGM210PA32JIA2)
#include "mgm210pa32jia2.h"

#elif defined(MGM210PA32JNA2)
#include "mgm210pa32jna2.h"

#else
#error "em_device.h: PART NUMBER undefined"
#endif
#endif /* EM_DEVICE_H */
