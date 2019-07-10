/***************************************************************************//**
 * @file
 * @brief Inertial Measurement Unit DCM matrix related routines
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

#ifndef IMU_H
#define IMU_H

#include <stdint.h>

#include "em_gpio.h"

#include "i2cspm.h"

#include "thunderboard/board_pic_regs.h"
#include "thunderboard/board_rgbled_profiles.h"

/***************************************************************************//**
 * @addtogroup IMU
 * @{
 ******************************************************************************/

/**************************************************************************//**
* @name Error Codes
* @{
******************************************************************************/
#define IMU_OK                         0  /**< No errors */
/**@}*/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#define IMU_PI                     3.14159265358979323            /**< The value of the Pi                  */
#define IMU_DEG_TO_RAD_FACTOR      ( (2.0 * IMU_PI) / 360.0)      /**< Degrees to radians conversion factor */
#define IMU_RAD_TO_DEG_FACTOR      (360.0 / (2.0 * IMU_PI) )      /**< Radians to degrees conversion factor */
#define IMU_DEG_TO_RAD(ang)        (ang * IMU_DEG_TO_RAD_FACTOR)  /**< Degrees to radians converter macro   */
#define IMU_RAD_TO_DEG(ang)        (ang * IMU_RAD_TO_DEG_FACTOR)  /**< Radians to degrees converter macro   */

#define IMU_MAX_ACCEL_FOR_ANGLE    0.9848  /**< Maximum acceleration value for a given angle                */
/** @endcond */

/**************************************************************************//**
* @name State Definitions
* @{
******************************************************************************/
#define IMU_STATE_DISABLED         0x00    /**< The IMU is disabled                                         */
#define IMU_STATE_READY            0x01    /**< The IMU is fully configured and ready to take measurements  */
#define IMU_STATE_INITIALIZING     0x02    /**< The IMU is being initialized                                */
#define IMU_STATE_CALIBRATING      0x03    /**< The IMU is being calibrated                                 */
/**@}*/

/***************************************************************************//**
 * @brief
 *    Structure to store the sensor fusion data
 ******************************************************************************/
typedef struct _IMU_SensorFusion{
  /* Direction Cosine Matrix */
  float     dcm[3][3];           /**< Direction Cosine Matrix                       */

  /* Accelerometer filtering */
  float     aVector[3];          /**< Acceleration vector                           */
  float     aAccumulator[3];     /**< Accumulator for acceleration vector           */
  uint32_t  aAccumulatorCount;   /**< Number of vectors stored in the accumulator   */
  float     aSampleRate;         /**< Acceleration measurement sample rate          */

  /* Gyro */
  float     gVector[3];          /**< Gyro vector                                   */
  float     gSampleRate;         /**< Gyroscope measurement sample rate             */
  float     gDeltaTime;          /**< Time between gyro samples                     */
  float     gDeltaTimeScale;     /**< Rotation between gyro samples                 */

  /* Orientation */
  float     angleCorrection[3];  /**< Angle correction vector                       */
  float     orientation[3];      /**< Orientation vector                            */
} IMU_SensorFusion;

/******************************************************************************/
/** Module functions                                                         **/
/******************************************************************************/
uint32_t IMU_init                       (void);
uint32_t IMU_deInit                     (void);
uint8_t  IMU_getState                   (void);
void     IMU_update                     (void);
void     IMU_reset                      (void);

void     IMU_accelerationGet            (int16_t avec[3]);
void     IMU_orientationGet             (int16_t ovec[3]);
void     IMU_gyroGet                    (int16_t gvec[3]);
uint32_t IMU_gyroCalibrate              (void);
void     IMU_getGyroCorrectionAngles    (float acorr[3]);

/*****************************************************************************/
/** Vector and Matrix math                                                  **/
/*****************************************************************************/
void     IMU_normalizeAngle             (float *a);
void     IMU_matrixMultiply             (float c[3][3], float a[3][3], float b[3][3]);
void     IMU_vectorNormalizeAngle       (float v[3]);
void     IMU_vectorZero                 (float v[3]);
void     IMU_vectorScale                (float v[3], float scale);
void     IMU_vectorScalarMultiplication (float r[3], float v[3], float scale);
void     IMU_vectorAdd                  (float r[3], float a[3], float b[3]);
void     IMU_vectorSubtract             (float r[3], float a[3], float b[3]);
float    IMU_vectorDotProduct           (float a[3], float b[3]);
void     IMU_vectorCrossProduct         (float r[3], float a[3], float b[3]);

/*****************************************************************************/
/** Direction Cosine Matrix functions                                       **/
/*****************************************************************************/
void     IMU_dcmReset                   (float dcm[3][3]);
void     IMU_dcmResetZ                  (float dcm[3][3]);
void     IMU_dcmNormalize               (float dcm[3][3]);
void     IMU_dcmRotate                  (float dcm[3][3], float ang[3]);
void     IMU_dcmGetAngles               (float dcm[3][3], float ang[3]);

/*****************************************************************************/
/** IMU general use                                                         **/
/*****************************************************************************/
void     IMU_config                     (float sampleRate);
bool     IMU_isDataReady                (void);
bool     IMU_isDataReadyFlag            (void);
void     IMU_clearDataReadyFlag         (void);

void     IMU_getAccelerometerData       (float avec[3]);
void     IMU_getGyroData                (float gvec[3]);

/*****************************************************************************/
/** Sensor Fusion functions                                                 **/
/*****************************************************************************/
void     IMU_fuseAccelerometerSetSampleRate (IMU_SensorFusion *f, float rate);
void     IMU_fuseAccelerometerUpdateFilter  (IMU_SensorFusion *f, float avec[3]);

void     IMU_fuseGyroSetSampleRate          (IMU_SensorFusion *f, float rate);
void     IMU_fuseGyroUpdate                 (IMU_SensorFusion *f, float gvec[3]);
void     IMU_fuseGyroCorrectionClear        (IMU_SensorFusion *f);
void     IMU_fuseGyroCorrection             (IMU_SensorFusion *f,
                                             bool accValid, bool dirValid,
                                             float dirZ);

void     IMU_fuseNew                        (IMU_SensorFusion *f);
void     IMU_fuseReset                      (IMU_SensorFusion *f);
void     IMU_fuseUpdate                     (IMU_SensorFusion *f);

/** @} */
/** @} */

#endif // IMU_H
