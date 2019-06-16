/***************************************************************************//**
 * @file
 * @brief PA power conversion functions provided to the customer as source for
 *   highest level of customization.
 * @details This file contains the curves and logic that convert PA power
 *   levels to dBm powers.
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

#include "em_device.h"
#include "em_assert.h"
#include "em_cmu.h"
#include "em_common.h"

#include "pa_conversions_efr32.h"
#include "rail.h"
#include "rail_assert_error_codes.h"
#include "rail_chip_specific.h"

static RAIL_TxPowerCurvesConfigAlt_t powerCurvesState;

// This macro is defined when Silicon Labs builds this into the library as WEAK
// to ensure it can be overriden by customer versions of these functions. It
// should *not* be defined in a customer build.
#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
const RAIL_TxPowerCurves_t *RAIL_GetTxPowerCurve(RAIL_TxPowerMode_t mode)
{
  static RAIL_TxPowerCurves_t powerCurves;

  // Check for an invalid Tx power mode
  if (mode >= RAIL_TX_POWER_MODE_NONE) {
    return NULL;
  }

  RAIL_TxPowerCurveAlt_t const *curve =
    powerCurvesState.curves[mode].conversion.powerCurve;

  // Check for an invalid power curve
  if (curve == NULL) {
    return NULL;
  }

  powerCurves.maxPower = curve->maxPower;
  powerCurves.minPower = curve->minPower;
  powerCurves.powerParams = &curve->powerParams[0];
  return &powerCurves;
}

// This function will not be supported for any parts after efr32xg1x
#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
RAIL_Status_t RAIL_InitTxPowerCurves(const RAIL_TxPowerCurvesConfig_t *config)
{
#ifdef _SILICON_LABS_32B_SERIES_1
  // First PA is 2.4 GHz high power, using a piecewise fit
  RAIL_PaDescriptor_t *current =
    &powerCurvesState.curves[RAIL_TX_POWER_MODE_2P4_HP];
  current->algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR;
  current->segments = config->piecewiseSegments;
  current->min = RAIL_TX_POWER_LEVEL_HP_MIN;
  current->max = RAIL_TX_POWER_LEVEL_HP_MAX;
  static RAIL_TxPowerCurveAlt_t txPower2p4 = {
    .minPower = 0U,
    .maxPower = 0U,
    .powerParams = { // The current max number of piecewise segments is 8
      { 0U, 0U, 0U }, { 0U, 0U, 0U }, { 0U, 0U, 0U }, { 0U, 0U, 0U },
      { 0U, 0U, 0U }, { 0U, 0U, 0U }, { 0U, 0U, 0U }, { 0U, 0U, 0U },
    }
  };
  txPower2p4.maxPower = config->txPowerSgCurves->maxPower;
  txPower2p4.minPower = config->txPowerSgCurves->minPower;
  memcpy(&txPower2p4.powerParams[0],
         config->txPowerSgCurves->powerParams,
         config->piecewiseSegments * sizeof(RAIL_TxPowerCurveSegment_t));
  current->conversion.powerCurve = &txPower2p4;

  // Second PA is 2.4 GHz low power, using a mapping table
  current = &powerCurvesState.curves[RAIL_TX_POWER_MODE_2P4_LP];
  current->algorithm = RAIL_PA_ALGORITHM_MAPPING_TABLE;
  current->segments = 0U;
  current->min = RAIL_TX_POWER_LEVEL_LP_MIN;
  current->max = RAIL_TX_POWER_LEVEL_LP_MAX;
  current->conversion.mappingTable = config->txPower24LpCurves;

  // Third and final PA is Sub-GHz, using a piecewise fit
  current = &powerCurvesState.curves[RAIL_TX_POWER_MODE_SUBGIG];
  current->algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR;
  current->segments = config->piecewiseSegments;
  current->min = RAIL_TX_POWER_LEVEL_SUBGIG_MIN;
  current->max = RAIL_TX_POWER_LEVEL_SUBGIG_MAX;
  static RAIL_TxPowerCurveAlt_t txPowerSubGig = {
    .minPower = 0U,
    .maxPower = 0U,
    .powerParams = { // The current max number of piecewise segments is 8
      { 0U, 0U, 0U }, { 0U, 0U, 0U }, { 0U, 0U, 0U }, { 0U, 0U, 0U },
      { 0U, 0U, 0U }, { 0U, 0U, 0U }, { 0U, 0U, 0U }, { 0U, 0U, 0U },
    }
  };
  txPowerSubGig.maxPower = config->txPowerSgCurves->maxPower;
  txPowerSubGig.minPower = config->txPowerSgCurves->minPower;
  memcpy(&txPowerSubGig.powerParams[0],
         config->txPowerSgCurves->powerParams,
         config->piecewiseSegments * sizeof(RAIL_TxPowerCurveSegment_t));
  current->conversion.powerCurve = &txPowerSubGig;

  return RAIL_STATUS_NO_ERROR;
#else
  (void) config;
  return RAIL_STATUS_INVALID_CALL;
#endif
}

#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
RAIL_Status_t RAIL_InitTxPowerCurvesAlt(const RAIL_TxPowerCurvesConfigAlt_t *config)
{
  RAIL_VerifyTxPowerCurves(config);
  powerCurvesState = *config;

  return RAIL_STATUS_NO_ERROR;
}

#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
RAIL_TxPowerLevel_t RAIL_ConvertDbmToRaw(RAIL_Handle_t railHandle,
                                         RAIL_TxPowerMode_t mode,
                                         RAIL_TxPower_t power)
{
  uint32_t powerLevel;
  int16_t powerIndex;

  (void)railHandle;
  // This function is called internally from the RAIL library,
  // so if the user never calls RAIL_InitTxPowerCurves - even
  // if they never intend to use dBm values in their code -
  // they'll always hit the assert below. Give the user a way
  // to not have to call RAIL_InitTxPowerCurves if they don't
  // care about dBm values by picking a dBm value that returns the
  // highest RAIL_TxPowerLevel_t possible. In other words, when
  // a channel dBm limitation greater than or equal to \ref RAIL_TX_POWER_MAX
  // is converted to raw units, the max RAIL_TxPowerLevel_t will be
  // returned. When compared to the current power level of the PA,
  // it will always be greater, indicating that no power coercion
  // is necessary to comply with channel limitations.
  if (power >= RAIL_TX_POWER_MAX) {
    return 255;
  }

  // Check for an invalid Tx power mode
  if (mode >= RAIL_TX_POWER_MODE_NONE) {
    return 0;
  }

  RAIL_PaDescriptor_t const *modeInfo = &powerCurvesState.curves[mode];

  // If we're in low power mode, just use the simple lookup table
  if (modeInfo->algorithm == RAIL_PA_ALGORITHM_MAPPING_TABLE) {
    // Loop through the lookup table to find the closest power level
    // without going over.
    for (powerIndex = (int16_t)(modeInfo->max - 1U);
         (powerIndex != 0) && (power < modeInfo->conversion.mappingTable[powerIndex]);
         powerIndex--) {
      // Searching...
    }
    return powerIndex + 1;
  }

  // Here we know we're using the piecewise linear conversion
  RAIL_TxPowerCurveSegment_t const *powerParams;
  RAIL_TxPowerCurveAlt_t const *paParams = modeInfo->conversion.powerCurve;

  // Check for valid paParams before using them
  if (paParams == NULL) {
    return 0;
  }

  // Cap the power based on the PA settings.
  if (power > paParams->maxPower) {
    power = paParams->maxPower;
  } else if (power < paParams->minPower) {
    power = paParams->minPower;
  } else {
  }
  // Map the power value to a 0 - 7 powerIndex value
  // 0 -> 20dBm - 16dBm; 1 -> 16dBm - 12dBm, etc...
  // These are just starting points to give the code
  // a rough idea of which segment to use, based on
  // how they were fit. Adjustments are made later on
  // if this turns out to be incorrect.
  powerIndex = ((power - 200) / 40);
  powerIndex = -powerIndex;
  if (powerIndex > (int16_t)(modeInfo->segments - 1U)) {
    powerIndex = (int16_t)(modeInfo->segments - 1U);
  }

  do {
    // Select the correct piecewise segment to use for conversion.
    powerParams = &paParams->powerParams[powerIndex];

    // powerLevel can only go down to 0.
    if (powerParams->intercept + powerParams->slope * power < 0) {
      powerLevel = 0;
    } else {
      powerLevel = powerParams->intercept + powerParams->slope * power;
    }

    // Add 500 to do rounding correctly, as opposed to just rounding towards 0
    powerLevel = ((powerLevel + 500U) / 1000U);

    // In case it turns out the resultant power level was too low and we have
    // to recalculate with the next curve...
    powerIndex++;
  } while ((powerIndex < (int16_t)modeInfo->segments)
           && (powerLevel <= paParams->powerParams[powerIndex].maxPowerLevel));

  // We already know that powerIndex is at most modeInfo->segments
  if (powerLevel > paParams->powerParams[powerIndex - 1].maxPowerLevel) {
    powerLevel = paParams->powerParams[powerIndex - 1].maxPowerLevel;
  }

  // Although 0 is a legitimate power on non-2.4 LP PA's and can be set via
  // "RAIL_SetTxPower(railHandle, 0)" it is MUCH lower than power
  // level 1 (approximately -50 dBm). Including it in the piecewise
  // linear fit would skew the curve substantially, so we exclude it
  // from the conversion.
  if (powerLevel == 0U) {
    powerLevel = 1;
  }
  return powerLevel;
}

#ifdef RAIL_PA_CONVERSIONS_WEAK
SL_WEAK
#endif
RAIL_TxPower_t RAIL_ConvertRawToDbm(RAIL_Handle_t railHandle,
                                    RAIL_TxPowerMode_t mode,
                                    RAIL_TxPowerLevel_t powerLevel)
{
  (void)railHandle;

  // Check for an invalid Tx power mode
  if (mode >= RAIL_TX_POWER_MODE_NONE) {
    return RAIL_TX_POWER_MIN;
  }

  RAIL_PaDescriptor_t const *modeInfo = &powerCurvesState.curves[mode];

  if (modeInfo->algorithm == RAIL_PA_ALGORITHM_MAPPING_TABLE) {
    // Limit the max power level
    if (powerLevel > modeInfo->max) {
      powerLevel = modeInfo->max;
    }

    // We 1-index low power PA power levels, but of course arrays are 0 indexed
    if (powerLevel != 0U) {
      powerLevel--;
    }

    return modeInfo->conversion.mappingTable[powerLevel];
  } else {
    // Although 0 is a legitimate power on non-2.4 LP PA's and can be set via
    // "RAIL_SetTxPower(railHandle, 0)" it is MUCH lower than power
    // level 1 (approximately -50 dBm). Including it in the piecewise
    // linear fit would skew the curve substantially, so we exclude it
    // from the conversion.
    if (powerLevel == 0U) {
      return -500;
    }

    int32_t power;

    RAIL_TxPowerCurveAlt_t const *powerCurve = modeInfo->conversion.powerCurve;
    // Check for a valid powerCurve pointer before using it
    if (powerCurve == NULL) {
      return RAIL_TX_POWER_MIN;
    }

    RAIL_TxPowerCurveSegment_t const *powerParams = powerCurve->powerParams;
    // Check for a valid powerParams pointer before using it
    if (powerParams == NULL) {
      return RAIL_TX_POWER_MIN;
    }

    // Hard code the extremes (i.e. don't use the curve fit) in order
    // to make it clear that we are reaching the extent of the chip's
    // capabilities
    if (powerLevel <= 1U) {
      return powerCurve->minPower;
    } else if (powerLevel >= modeInfo->max) {
      return powerCurve->maxPower;
    } else {
    }

    // Figure out which parameter to use based on the power level
    uint8_t x;
    uint8_t upperBound = modeInfo->segments - 1U;
    for (x = 0; x < upperBound; x++) {
      if (powerParams[x + 1U].maxPowerLevel < powerLevel) {
        break;
      }
    }

    power = ((1000 * (int32_t)(powerLevel)) - powerParams[x].intercept);
    power = ((power + (powerParams[x].slope / 2)) / powerParams[x].slope);

    if (power > powerCurve->maxPower) {
      return powerCurve->maxPower;
    } else if (power < powerCurve->minPower) {
      return powerCurve->minPower;
    } else {
      return power;
    }
  }
}
