/***************************************************************************//**
 * @file
 * @brief GPIOINT API definition
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

#ifndef GPIOINTERRUPT_H
#define GPIOINTERRUPT_H

#include "em_device.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup GPIOINT
 * @{
 ******************************************************************************/

/*******************************************************************************
 *******************************   TYPEDEFS   **********************************
 ******************************************************************************/

/**
 * @brief
 *  GPIO interrupt callback function pointer.
 * @details
 *   Parameters:
 *   @li intNo - The pin interrupt number the callback function is invoked for.
 */
typedef void (*GPIOINT_IrqCallbackPtr_t)(uint8_t intNo);

/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/
void GPIOINT_Init(void);
void GPIOINT_CallbackRegister(uint8_t intNo, GPIOINT_IrqCallbackPtr_t callbackPtr);
static __INLINE void GPIOINT_CallbackUnRegister(uint8_t intNo);

/***************************************************************************//**
 * @brief
 *   Unregisters user callback for given pin interrupt number.
 *
 * @details
 *   Use this function to unregister a callback.
 *
 * @param[in] intNo
 *   Pin interrupt number for the callback.
 *
 ******************************************************************************/
static __INLINE void GPIOINT_CallbackUnRegister(uint8_t intNo)
{
  GPIOINT_CallbackRegister(intNo, 0);
}

/** @} (end addtogroup GPIOINT */
/** @} (end addtogroup emdrv) */
#ifdef __cplusplus
}
#endif

#endif /* GPIOINTERRUPT_H */
