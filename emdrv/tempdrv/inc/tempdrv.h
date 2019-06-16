/***************************************************************************//**
 * @file
 * @brief TEMPDRV API definition.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef __SILICON_LABS_TEMPDRV_H__
#define __SILICON_LABS_TEMPDRV_H__

#include "em_device.h"
#include "em_emu.h"
#include "ecode.h"
#include "tempdrv_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup TEMPDRV
 * @{
 ******************************************************************************/

#define ECODE_EMDRV_TEMPDRV_OK                (ECODE_OK)                              ///< Success return value.
#define ECODE_EMDRV_TEMPDRV_NO_INIT           (ECODE_EMDRV_TEMPDRV_BASE | 0x00000001) ///< Function requires prior initialization
#define ECODE_EMDRV_TEMPDRV_PARAM_ERROR       (ECODE_EMDRV_TEMPDRV_BASE | 0x00000002) ///< Illegal input parameter.
#define ECODE_EMDRV_TEMPDRV_BAD_LIMIT         (ECODE_EMDRV_TEMPDRV_BASE | 0x00000003) ///< Temperature mismatch with limit.
#define ECODE_EMDRV_TEMPDRV_NO_CALLBACK       (ECODE_EMDRV_TEMPDRV_BASE | 0x00000004) ///< Can't find callback.
#define ECODE_EMDRV_TEMPDRV_NO_SPACE          (ECODE_EMDRV_TEMPDRV_BASE | 0x00000005) ///< No more space to register
#define ECODE_EMDRV_TEMPDRV_TEMP_UNDER        (ECODE_EMDRV_TEMPDRV_BASE | 0x00000006) ///< Requested temperature below measurable range
#define ECODE_EMDRV_TEMPDRV_TEMP_OVER         (ECODE_EMDRV_TEMPDRV_BASE | 0x00000007) ///< Requested temperature above measurable range
#define ECODE_EMDRV_TEMPDRV_DUP_TEMP          (ECODE_EMDRV_TEMPDRV_BASE | 0x00000008) ///< Requested temperature is a duplicate

/**
 * Temperature limit. This is used in the TEMPDRV to specify the
 * direction of temperature change.
 */
typedef enum TEMPDRV_LimitType{
  TEMPDRV_LIMIT_LOW  = 0,   /**< Temperature low. Used to specify that a temperature is decreasing. */
  TEMPDRV_LIMIT_HIGH = 1    /**< Temperature high. Used to specify that a temperature is increasing. */
} TEMPDRV_LimitType_t;

/***************************************************************************//**
 * @brief
 *   TEMPDRV temperature limit callback function.
 *
 * @details
 *   This callback function is called from interrupt context. The callback
 *   function is called when the current temperature is equal to or exceeds
 *   one of the temperature limits that have been registered with the driver.
 *
 * @param[in] temp
 *   The current temperature at the time when the EMU temperature triggers an
 *   interrupt. Note that this is not necessarily the same temperature as was
 *   specified when registering a callback.
 *
 * @param[in] limit
 *   The upper/lower limit reached
 ******************************************************************************/
typedef void (*TEMPDRV_Callback_t)(int8_t temp, TEMPDRV_LimitType_t limit);

/***************************************************************************//**
 * @brief
 *  TEMPDRV IRQ Handler.
 *
 * @details
 *  This IRQ Handler should be called from within the EMU_IRQ_Handler in order
 *  to enable TEMPDRV callbacks. This is included by default with
 *  EMU_CUSTOM_IRQ_HANDLER defined as false.
 ******************************************************************************/
void TEMPDRV_IRQHandler(void);

Ecode_t TEMPDRV_Init(void);

Ecode_t TEMPDRV_DeInit(void);

Ecode_t TEMPDRV_Enable(bool enable);

uint8_t TEMPDRV_GetActiveCallbacks(TEMPDRV_LimitType_t limit);

int8_t TEMPDRV_GetTemp(void);

Ecode_t TEMPDRV_RegisterCallback(int8_t temp,
                                 TEMPDRV_LimitType_t limit,
                                 TEMPDRV_Callback_t callback);

Ecode_t TEMPDRV_UnregisterCallback(TEMPDRV_Callback_t callback);

/** @} (end addtogroup TEMPDRV) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif
#endif // __SILICON_LABS_TEMPDRV_H__
