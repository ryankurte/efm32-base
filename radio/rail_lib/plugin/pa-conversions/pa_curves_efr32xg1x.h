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

#ifndef __PA_CURVES_EFR32XG1X_H_
#define __PA_CURVES_EFR32XG1X_H_

#ifdef __cplusplus
extern "C" {
#endif

#define RAIL_PA_CURVES_LP_VALUES 7
#define RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS 8

#define RAIL_PA_CURVES_2P4_HP_VBAT_MAX_POWER      200
#define RAIL_PA_CURVES_2P4_HP_VBAT_MIN_POWER      -260
#define RAIL_PA_CURVES_2P4_HP_VBAT_CURVES \
  { { 252, 2776, -300026 },               \
    { 136, 1335, -73192 },                \
    { 83, 772, -7179 },                   \
    { 50, 441, 17309 },                   \
    { 41, 343, 22520 },                   \
    { 22, 209, 22360 },                   \
    { 14, 124, 18896 },                   \
    { 8, 75, 14632 } }

#define RAIL_PA_CURVES_2P4_HP_DCDC_MAX_POWER      140
#define RAIL_PA_CURVES_2P4_HP_DCDC_MIN_POWER      -260
#define RAIL_PA_CURVES_2P4_HP_DCDC_CURVES \
  { { -1, 0, 0 },                         \
    { 252, 4306, -391604 },               \
    { 117, 1435, -52495 },                \
    { 62, 610, 13579 },                   \
    { 37, 331, 24456 },                   \
    { 23, 224, 23902 },                   \
    { 14, 140, 20330 },                   \
    { 9, 81, 15607 } }

#define RAIL_PA_CURVES_SG_VBAT_MAX_POWER          200
#define RAIL_PA_CURVES_SG_VBAT_MIN_POWER          -260
#define RAIL_PA_CURVES_SG_VBAT_CURVES \
  { { 248, 2924, -336264 },           \
    { 130, 1265, -71090 },            \
    { 77, 735, -8632 },               \
    { 47, 451, 13197 },               \
    { 30, 281, 19428 },               \
    { 18, 172, 19088 },               \
    { 12, 108, 16305 },               \
    { 7, 67, 12870 } }

#define RAIL_PA_CURVES_SG_DCDC_MAX_POWER          150
#define RAIL_PA_CURVES_SG_DCDC_MIN_POWER          -260
#define RAIL_PA_CURVES_SG_DCDC_CURVES \
  { { 248, 9069, -1171644 },          \
    { 199, 3826, -378994 },           \
    { 92, 932, -22748 },              \
    { 50, 470, 13485 },               \
    { 31, 304, 19712 },               \
    { 18, 192, 19146 },               \
    { 11, 110, 15607 },               \
    { 6, 56, 11146 } }

#define RAIL_PA_CURVES_2P4_LP  \
  {                            \
    -125, /*! Power Level 1 */ \
    -65,  /*! Power Level 2 */ \
    -30,  /*! Power Level 3 */ \
    -15,  /*! Power Level 4 */ \
    -7,   /*! Power Level 5 */ \
    -2,   /*! Power Level 6 */ \
    0     /*! Power Level 7 */ \
  }

/**
 * Macros to ease in the delcarations of the various power amplifier units
 * conversion curves.
 */
#define RAIL_DECLARE_PA_2P4_HP_VBAT                                     \
  static const RAIL_TxPowerCurveSegment_t                               \
  twoPointFourHpVbatCurves[RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS] \
    = RAIL_PA_CURVES_2P4_HP_VBAT_CURVES;

#define RAIL_DECLARE_PA_2P4_HP_DCDC                                     \
  static const RAIL_TxPowerCurveSegment_t                               \
  twoPointFourHpDcdcCurves[RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS] \
    = RAIL_PA_CURVES_2P4_HP_DCDC_CURVES;

#define RAIL_DECLARE_PA_SG_VBAT                                 \
  static const RAIL_TxPowerCurveSegment_t                       \
  subgigVbatCurves[RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS] \
    = RAIL_PA_CURVES_SG_VBAT_CURVES;

#define RAIL_DECLARE_PA_SG_DCDC                                 \
  static const RAIL_TxPowerCurveSegment_t                       \
  subgigDcdcCurves[RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS] \
    = RAIL_PA_CURVES_SG_DCDC_CURVES;

// Macro to declare the variables needed to initialize
// RAIL_TxPowerCurvesConfigAlt_t for use in RAIL_InitTxPowerCurvesAlt,
// assuming battery powered operation
#define RAIL_DECLARE_TX_POWER_VBAT_CURVES_ALT                      \
  static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataHp = {     \
    RAIL_PA_CURVES_2P4_HP_VBAT_MAX_POWER,                          \
    RAIL_PA_CURVES_2P4_HP_VBAT_MIN_POWER,                          \
    RAIL_PA_CURVES_2P4_HP_VBAT_CURVES                              \
  };                                                               \
  static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataSg = {     \
    RAIL_PA_CURVES_SG_VBAT_MAX_POWER,                              \
    RAIL_PA_CURVES_SG_VBAT_MIN_POWER,                              \
    RAIL_PA_CURVES_SG_VBAT_CURVES                                  \
  };                                                               \
  static const int16_t RAIL_curves24Lp[RAIL_PA_CURVES_LP_VALUES] = \
    RAIL_PA_CURVES_2P4_LP;
// *INDENT-OFF*

// Macro to declare the variables needed to initialize
// RAIL_TxPowerCurvesConfigAlt_t for use in RAIL_InitTxPowerCurvesAlt,
// assuming DCDC powered operation
#define RAIL_DECLARE_TX_POWER_DCDC_CURVES_ALT                                  \
  static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataHp = {                 \
    RAIL_PA_CURVES_2P4_HP_DCDC_MAX_POWER,                                      \
    RAIL_PA_CURVES_2P4_HP_DCDC_MIN_POWER,                                      \
    RAIL_PA_CURVES_2P4_HP_DCDC_CURVES,                                         \
  };                                                                           \
  static const RAIL_TxPowerCurveAlt_t RAIL_piecewiseDataSg = {                 \
    RAIL_PA_CURVES_SG_DCDC_MAX_POWER,                                          \
    RAIL_PA_CURVES_SG_DCDC_MIN_POWER,                                          \
    RAIL_PA_CURVES_SG_DCDC_CURVES                                              \
  };                                                                           \
  static const int16_t RAIL_curves24Lp[RAIL_PA_CURVES_LP_VALUES] =             \
    RAIL_PA_CURVES_2P4_LP;
// *INDENT-OFF*

// Macro to declare the variables needed to initialize
// RAIL_TxPowerCurvesConfig_t for use in RAIL_InitTxPowerCurves,
// assuming battery powered operation
#define RAIL_DECLARE_TX_POWER_VBAT_CURVES(piecewiseSegments, curvesSg,         \
                                          curves24Hp, curves24Lp)              \
  static const uint8_t (piecewiseSegments) =                                   \
    RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS;                               \
                                                                               \
  RAIL_DECLARE_PA_2P4_HP_VBAT;                                                 \
  RAIL_DECLARE_PA_SG_VBAT;                                                     \
                                                                               \
  static const RAIL_TxPowerCurves_t curvesSg[1] = {                            \
    { RAIL_PA_CURVES_SG_VBAT_MAX_POWER,                                        \
      RAIL_PA_CURVES_SG_VBAT_MIN_POWER,                                        \
      subgigVbatCurves }                                                       \
  };                                                                           \
  static const RAIL_TxPowerCurves_t curves24Hp[1] = {                          \
    { RAIL_PA_CURVES_2P4_HP_VBAT_MAX_POWER,                                    \
      RAIL_PA_CURVES_2P4_HP_VBAT_MIN_POWER,                                    \
      twoPointFourHpVbatCurves }                                               \
  };                                                                           \
  static const int16_t curves24Lp[RAIL_PA_CURVES_LP_VALUES] =                  \
    RAIL_PA_CURVES_2P4_LP;
// *INDENT-OFF*

// Macro to declare the variables needed to initialize
// RAIL_TxPowerCurvesConfig_t for use in RAIL_InitTxPowerCurves,
// assuming DCDC powered operation
#define RAIL_DECLARE_TX_POWER_DCDC_CURVES(piecewiseSegments, curvesSg,         \
                                          curves24Hp, curves24Lp)              \
  static const uint8_t piecewiseSegments =                                     \
    RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS;                               \
                                                                               \
  RAIL_DECLARE_PA_2P4_HP_DCDC;                                                 \
  RAIL_DECLARE_PA_SG_DCDC;                                                     \
                                                                               \
  static const RAIL_TxPowerCurves_t curvesSg[1] = {                            \
    { RAIL_PA_CURVES_SG_DCDC_MAX_POWER,                                        \
      RAIL_PA_CURVES_SG_DCDC_MIN_POWER,                                        \
      subgigDcdcCurves }                                                       \
  };                                                                           \
  static const RAIL_TxPowerCurves_t curves24Hp[1] = {                          \
    { RAIL_PA_CURVES_2P4_HP_DCDC_MAX_POWER,                                    \
      RAIL_PA_CURVES_2P4_HP_DCDC_MIN_POWER,                                    \
      twoPointFourHpDcdcCurves }                                               \
  };                                                                           \
  static const int16_t curves24Lp[RAIL_PA_CURVES_LP_VALUES] =                  \
    RAIL_PA_CURVES_2P4_LP;
// *INDENT-OFF*

#define RAIL_DECLARE_TX_POWER_CURVES_CONFIG_ALT                                \
  {                                                                            \
    .curves = {                                                                \
      {                                                                        \
        .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,                       \
        .segments = RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS,               \
        .min = RAIL_TX_POWER_LEVEL_HP_MIN,                                     \
        .max = RAIL_TX_POWER_LEVEL_HP_MAX,                                     \
        .conversion = { .powerCurve = &RAIL_piecewiseDataHp },                 \
      },                                                                       \
      {                                                                        \
        .algorithm = RAIL_PA_ALGORITHM_MAPPING_TABLE,                          \
        .segments = 0U,                                                        \
        .min = RAIL_TX_POWER_LEVEL_LP_MIN,                                     \
        .max = RAIL_TX_POWER_LEVEL_LP_MAX,                                     \
        .conversion = { .mappingTable = &RAIL_curves24Lp[0] },                 \
      },                                                                       \
      {                                                                        \
        .algorithm = RAIL_PA_ALGORITHM_PIECEWISE_LINEAR,                       \
        .segments = RAIL_PA_CURVES_2P4_HP_SG_PIECEWISE_SEGMENTS,               \
        .min = RAIL_TX_POWER_LEVEL_SUBGIG_MIN,                                 \
        .max = RAIL_TX_POWER_LEVEL_SUBGIG_MAX,                                 \
        .conversion = { .powerCurve = &RAIL_piecewiseDataSg },                 \
      },                                                                       \
    }                                                                          \
  }
// *INDENT-OFF*

#ifdef __cplusplus
}
#endif

#endif
