/***************************************************************************//**
 * @file
 * @brief Touch panel driver for DK3650, DK3750 and DK3850 Development Kits
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

#ifndef __TOUCH_H
#define __TOUCH_H

#include "em_device.h"
#include "em_adc.h"
#include "calibrate.h"

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Touch
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */
#define TOUCH_IDLE         0
#define TOUCH_BUSY_PRESSED 1
#define TOUCH_BUSY_CHECK   2
#define TOUCH_BUSY_SCAN    3
/** @endcond */

/** Touch panel position structure */
typedef struct
{ uint16_t x;           /**< x position */
  uint16_t y;           /**< y position */
  uint16_t adcx;        /**< ADC X readings */
  uint16_t adcy;        /**< ADC Y readings */
  uint8_t  pen;         /**< 0=not pressed, 1=touched */
} TOUCH_Pos_TypeDef;

/** Touch configuration structure */
typedef struct
{ uint32_t               frequency;     /**< ADC frequency, min 32kHz, max 13MHz, typical 500kHz */
  uint8_t                ignore;        /**< Ignore move if it has not at least defined pixels */
  ADC_OvsRateSel_TypeDef oversampling;  /**< ADC oversampling used during touch reading */
} TOUCH_Config_TypeDef;

/** Initializes Touch default values */
#define TOUCH_INIT_DEFAULT                                             \
  { 500000,                /* 500kHz sampling frequency */             \
    1,                     /* Ignore moves not greater than 1 pixel */ \
    adcOvsRateSel128,      /* Oversampling 128 times */                \
  }

/** touch panel upcall type definition */
typedef void (TOUCH_Upcall_TypeDef)(TOUCH_Pos_TypeDef *);

void TOUCH_Init(TOUCH_Config_TypeDef *config);
void TOUCH_RegisterUpcall(TOUCH_Upcall_TypeDef *new_upcall);
TOUCH_Pos_TypeDef *TOUCH_GetPos(void);
int TOUCH_CalibrationTable(POINT * displayPtr, POINT * screenPtr);
int TOUCH_IsBusy(void);

#ifdef __cplusplus
}
#endif

/** @} (end group Touch) */
/** @} (end group kitdrv) */

#endif
