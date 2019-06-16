/***************************************************************************//**
 * @file
 * @brief PA power conversion curves used by Silicon Labs PA power conversion
 *   functions.
 * @details This file contains the curves needed convert PA power levels to
 *   dBm powers.
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

#ifndef __PA_CURVES_EFR32_H_
#define __PA_CURVES_EFR32_H_

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _SILICON_LABS_32B_SERIES_1
#include "pa_curves_efr32xg1x.h"
#elif defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_3)
#include "pa_curves_efr32xg22.h"
#elif defined (_SILICON_LABS_32B_SERIES_2)
#include "pa_curves_efr32xg21.h"
#else

#error "Unsupported platform!"
#endif

#ifdef __cplusplus
}
#endif

#endif
