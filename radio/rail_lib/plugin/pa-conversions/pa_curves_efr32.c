/***************************************************************************//**
 * @file
 * @brief Default PA power conversion structures with curves calibrated by the
 *   RAIL team.
 * @details This file contains the curves that convert PA power levels to dBm
 *   powers.
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

// This entire file should never be used on FCC pre-certified modules
#ifndef _SILICON_LABS_MODULE

#include "em_device.h"
#include "em_common.h"
#include "pa_conversions_efr32.h"

#if defined(_SILICON_LABS_32B_SERIES_1)

static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataHpVbat = {
  RAIL_PA_CURVES_2P4_HP_VBAT_MAX_POWER,
  RAIL_PA_CURVES_2P4_HP_VBAT_MIN_POWER,
  RAIL_PA_CURVES_2P4_HP_VBAT_CURVES
};
static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataSgVbat = {
  RAIL_PA_CURVES_SG_VBAT_MAX_POWER,
  RAIL_PA_CURVES_SG_VBAT_MIN_POWER,
  RAIL_PA_CURVES_SG_VBAT_CURVES
};
static const int16_t RAIL_curves24LpVbat[RAIL_PA_CURVES_LP_VALUES] =
  RAIL_PA_CURVES_2P4_LP;

// This macro is defined when Silicon Labs builds this into the library as WEAK
// to ensure it can be overriden by customer versions of these functions. It
// should *not* be defined in a customer build.
#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
const RAIL_TxPowerCurvesConfigAlt_t RAIL_TxPowerCurvesVbat = {
  .curves = {
    {
      .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,
      .segments = RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS,
      .min = RAIL_TX_POWER_LEVEL_HP_MIN,
      .max = RAIL_TX_POWER_LEVEL_HP_MAX,
      .conversion = { .powerCurve = &RAIL_piecewiseDataHpVbat },
    },
    {
      .algorithm = RAIL_PA_ALGORITHM_MAPPING_TABLE,
      .segments = 0U,
      .min = RAIL_TX_POWER_LEVEL_LP_MIN,
      .max = RAIL_TX_POWER_LEVEL_LP_MAX,
      .conversion = { .mappingTable = &RAIL_curves24LpVbat[0] },
    },
    {
      .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,
      .segments = RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS,
      .min = RAIL_TX_POWER_LEVEL_SUBGIG_MIN,
      .max = RAIL_TX_POWER_LEVEL_SUBGIG_MAX,
      .conversion = { .powerCurve = &RAIL_piecewiseDataSgVbat },
    },
  },
};

static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataHpDcdc = {
  RAIL_PA_CURVES_2P4_HP_DCDC_MAX_POWER,
  RAIL_PA_CURVES_2P4_HP_DCDC_MIN_POWER,
  RAIL_PA_CURVES_2P4_HP_DCDC_CURVES
};
static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataSgDcdc = {
  RAIL_PA_CURVES_SG_DCDC_MAX_POWER,
  RAIL_PA_CURVES_SG_DCDC_MIN_POWER,
  RAIL_PA_CURVES_SG_DCDC_CURVES
};
static const int16_t RAIL_curves24LpDcdc[RAIL_PA_CURVES_LP_VALUES] =
  RAIL_PA_CURVES_2P4_LP;

#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
const RAIL_TxPowerCurvesConfigAlt_t RAIL_TxPowerCurvesDcdc = {
  .curves = {
    {
      .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,
      .segments = RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS,
      .min = RAIL_TX_POWER_LEVEL_HP_MIN,
      .max = RAIL_TX_POWER_LEVEL_HP_MAX,
      .conversion = { .powerCurve = &RAIL_piecewiseDataHpDcdc },
    },
    {
      .algorithm = RAIL_PA_ALGORITHM_MAPPING_TABLE,
      .segments = 0U,
      .min = RAIL_TX_POWER_LEVEL_LP_MIN,
      .max = RAIL_TX_POWER_LEVEL_LP_MAX,
      .conversion = { .mappingTable = &RAIL_curves24LpDcdc[0] },
    },
    {
      .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,
      .segments = RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS,
      .min = RAIL_TX_POWER_LEVEL_SUBGIG_MIN,
      .max = RAIL_TX_POWER_LEVEL_SUBGIG_MAX,
      .conversion = { .powerCurve = &RAIL_piecewiseDataSgDcdc },
    },
  },
};

#elif defined(_SILICON_LABS_32B_SERIES_2_CONFIG_2) || defined(_SILICON_LABS_32B_SERIES_2_CONFIG_3)

static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataHpVbat = {
  RAIL_PA_CURVES_2P4_HP_VBAT_MAX_POWER,
  RAIL_PA_CURVES_2P4_HP_VBAT_MIN_POWER,
  RAIL_PA_CURVES_2P4_HP_VBAT_CURVES,
};
static const int16_t RAIL_curves24Lp[RAIL_PA_CURVES_LP_VALUES] =
  RAIL_PA_CURVES_2P4_LP_VBAT_CURVES;

#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
const RAIL_TxPowerCurvesConfigAlt_t RAIL_TxPowerCurvesVbat = {
  .curves = {
    {
      .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,
      .segments = RAIL_PA_CURVES_PIECEWISE_SEGMENTS,
      .min = RAIL_TX_POWER_LEVEL_HP_MIN,
      .max = RAIL_TX_POWER_LEVEL_HP_MAX,
      .conversion = { .powerCurve = &RAIL_piecewiseDataHpVbat },
    },
    {                                                        \
      .algorithm = RAIL_PA_ALGORITHM_MAPPING_TABLE,          \
      .segments = 0U,                                        \
      .min = RAIL_TX_POWER_LEVEL_LP_MIN,                     \
      .max = RAIL_TX_POWER_LEVEL_LP_MAX,                     \
      .conversion = { .mappingTable = &RAIL_curves24Lp[0] }, \
    },
  }
};

#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
const RAIL_TxPowerCurvesConfigAlt_t RAIL_TxPowerCurvesDcdc = {
  .curves = {
    {
      .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,
      .segments = RAIL_PA_CURVES_PIECEWISE_SEGMENTS,
      .min = RAIL_TX_POWER_LEVEL_HP_MIN,
      .max = RAIL_TX_POWER_LEVEL_HP_MAX,
      .conversion = { .powerCurve = &RAIL_piecewiseDataHpVbat },
    },
    {                                                        \
      .algorithm = RAIL_PA_ALGORITHM_MAPPING_TABLE,          \
      .segments = 0U,                                        \
      .min = RAIL_TX_POWER_LEVEL_LP_MIN,                     \
      .max = RAIL_TX_POWER_LEVEL_LP_MAX,                     \
      .conversion = { .mappingTable = &RAIL_curves24Lp[0] }, \
    },
  }
};

#elif defined(_SILICON_LABS_32B_SERIES_2)

static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataHpVbat = {
  RAIL_PA_CURVES_2P4_HP_VBAT_MAX_POWER,
  RAIL_PA_CURVES_2P4_HP_VBAT_MIN_POWER,
  RAIL_PA_CURVES_2P4_HP_VBAT_CURVES,
};
static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataMpVbat = {
  RAIL_PA_CURVES_2P4_MP_VBAT_MAX_POWER,
  RAIL_PA_CURVES_2P4_MP_VBAT_MIN_POWER,
  RAIL_PA_CURVES_2P4_MP_VBAT_CURVES,
};
static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataLpVbat = {
  RAIL_PA_CURVES_2P4_LP_VBAT_MAX_POWER,
  RAIL_PA_CURVES_2P4_LP_VBAT_MIN_POWER,
  RAIL_PA_CURVES_2P4_LP,
};

#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
const RAIL_TxPowerCurvesConfigAlt_t RAIL_TxPowerCurvesVbat = {
  .curves = {
    {
      .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,
      .segments = RAIL_PA_CURVES_PIECEWISE_SEGMENTS,
      .min = RAIL_TX_POWER_LEVEL_HP_MIN,
      .max = RAIL_TX_POWER_LEVEL_HP_MAX,
      .conversion = { .powerCurve = &RAIL_piecewiseDataHpVbat },
    },
    {
      .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,
      .segments = RAIL_PA_CURVES_PIECEWISE_SEGMENTS,
      .min = RAIL_TX_POWER_LEVEL_MP_MIN,
      .max = RAIL_TX_POWER_LEVEL_MP_MAX,
      .conversion = { .powerCurve = &RAIL_piecewiseDataMpVbat },
    },
    {
      .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,
      .segments = RAIL_PA_CURVES_PIECEWISE_SEGMENTS,
      .min = RAIL_TX_POWER_LEVEL_LP_MIN,
      .max = RAIL_TX_POWER_LEVEL_LP_MAX,
      .conversion = { .powerCurve = &RAIL_piecewiseDataLpVbat },
    },
  }
};

#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
const RAIL_TxPowerCurvesConfigAlt_t RAIL_TxPowerCurvesDcdc;

#else
#error "Unsupported platform!"
#endif

#endif //_SILICON_LABS_MODULE
