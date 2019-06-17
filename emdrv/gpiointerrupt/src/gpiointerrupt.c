/***************************************************************************//**
 * @file
 * @brief GPIOINT API implementation
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

#include "em_gpio.h"
#include "em_core.h"
#include "gpiointerrupt.h"
#include "em_assert.h"
#include "em_common.h"

/*******************************************************************************
 ********************************   MACROS   ***********************************
 ******************************************************************************/

/*******************************************************************************
 *******************************   STRUCTS   ***********************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

typedef struct {
  /* Pin interrupt number in range of 0 to 15 */
  uint32_t intNo;

  /* Pointer to the callback function */
  GPIOINT_IrqCallbackPtr_t callback;
} GPIOINT_CallbackDesc_t;

/*******************************************************************************
 ********************************   GLOBALS   **********************************
 ******************************************************************************/

/* Array of user callbacks. One for each pin interrupt number. */
static GPIOINT_IrqCallbackPtr_t gpioCallbacks[16] = { 0 };

/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/
static void GPIOINT_IRQDispatcher(uint32_t iflags);

/** @endcond */

/*******************************************************************************
 ***************************   GLOBAL FUNCTIONS   ******************************
 ******************************************************************************/

/***************************************************************************//**
 * @brief
 *   Initialization of GPIOINT module.
 *
 ******************************************************************************/
void GPIOINT_Init(void)
{
  NVIC_ClearPendingIRQ(GPIO_ODD_IRQn);
  NVIC_EnableIRQ(GPIO_ODD_IRQn);
  NVIC_ClearPendingIRQ(GPIO_EVEN_IRQn);
  NVIC_EnableIRQ(GPIO_EVEN_IRQn);
}

/***************************************************************************//**
 * @brief
 *   Registers user callback for given pin interrupt number.
 *
 * @details
 *   Use this function to register a callback which shall be called upon
 *   interrupt generated for a given pin interrupt number.
 *   Interrupt itself must be configured externally. Function overwrites previously
 *   registered callback.
 *
 * @param[in] pin
 *   Pin number for the callback.
 * @param[in] callbackPtr
 *   A pointer to callback function.
 ******************************************************************************/
void GPIOINT_CallbackRegister(uint8_t intNo, GPIOINT_IrqCallbackPtr_t callbackPtr)
{
  CORE_ATOMIC_SECTION(
    /* Dispatcher is used */
    gpioCallbacks[intNo] = callbackPtr;
    )
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/***************************************************************************//**
 * @brief
 *   Function calls users callback for registered pin interrupts.
 *
 * @details
 *   This function is called when GPIO interrupts are handled by the dispatcher.
 *   Function gets even or odd interrupt flags and calls user callback
 *   registered for that pin. Function iterates on flags starting from MSB.
 *
 * @param iflags
 *  Interrupt flags which shall be handled by the dispatcher.
 *
 ******************************************************************************/
static void GPIOINT_IRQDispatcher(uint32_t iflags)
{
  uint32_t irqIdx;
  GPIOINT_IrqCallbackPtr_t callback;

  /* check for all flags set in IF register */
  while (iflags != 0U) {
    irqIdx = SL_CTZ(iflags);

    /* clear flag*/
    iflags &= ~(1 << irqIdx);

    callback = gpioCallbacks[irqIdx];
    if (callback) {
      /* call user callback */
      callback(irqIdx);
    }
  }
}

/***************************************************************************//**
 * @brief
 *   GPIO EVEN interrupt handler. Interrupt handler clears all IF even flags and
 *   call the dispatcher passing the flags which triggered the interrupt.
 *
 ******************************************************************************/
void GPIO_EVEN_IRQHandler(void)
{
  uint32_t iflags;

  /* Get all even interrupts. */
  iflags = GPIO_IntGetEnabled() & 0x00005555;

  /* Clean only even interrupts. */
  GPIO_IntClear(iflags);

  GPIOINT_IRQDispatcher(iflags);
}

/***************************************************************************//**
 * @brief
 *   GPIO ODD interrupt handler. Interrupt handler clears all IF odd flags and
 *   call the dispatcher passing the flags which triggered the interrupt.
 *
 ******************************************************************************/
void GPIO_ODD_IRQHandler(void)
{
  uint32_t iflags;

  /* Get all odd interrupts. */
  iflags = GPIO_IntGetEnabled() & 0x0000AAAA;

  /* Clean only odd interrupts. */
  GPIO_IntClear(iflags);

  GPIOINT_IRQDispatcher(iflags);
}

/** @endcond */

/** @} (end addtogroup GPIOINT */
/** @} (end addtogroup emdrv) */

/* *INDENT-OFF* */
/******** THE REST OF THE FILE IS DOCUMENTATION ONLY !**********************//**
 * @addtogroup emdrv
 * @{
 * @addtogroup GPIOINT
 * @brief GPIOINT General Purpose Input/Output Interrupt dispatcher
 * @{

   @details
   The source files for the GPIO interrupt dispatcher library resides in the
   emdrv/gpiointerrupt folder, and are named gpiointerrupt.c and gpiointerrupt.h.

   @li @ref gpioint_intro
   @li @ref gpioint_api
   @li @ref gpioint_example

   @n @section gpioint_intro Introduction
 * EFM32/EZR32/EFR32 has two GPIO interrupts lines, Odd and Even. If more
 * than two interrupts are used then interrupt routine must dispatch from a callback
 * register. This module provides small dispatcher for both GPIO interrupts enabling
 * handling of up to 16 GPIO pin interrupts.
 *
 * It is up to the user to configure and enable interrupt on given pin. This can be done
 * using the GPIO library (emlib). This module handles the dispatch register and clearing of
 * interrupt flags.
 *
 * In order to use this dispatcher, it has to be initialized first by
 * calling GPIOINT_Init(). Then each pin interrupt number must be configured by first
 * registering the callback function for given interrupt number and then configure and
 * enabling the interrupt number in the GPIO module.

   @n @section gpioint_api The API
   This section contain brief descriptions of the functions in the API. You will
   find detailed information on parameters by clicking on the hyperlinked function names.

   Your application code must include one header file: @em gpiointerrupt.h.

   @ref GPIOINT_Init() @n
    This functions initializes the dispatcher register. Typically
    @htmlonly GPIOINT_Init() @endhtmlonly is called once in your startup code.

   @ref GPIOINT_CallbackRegister() @n
    Register a callback function on a pin interrupt number.

   @ref GPIOINT_CallbackUnRegister() @n
    Un-register a callback function on a pin interrupt number.

   @n @section gpioint_example Example
   @verbatim

#include "em_gpio.h"
#include "em_int.h"
#include "gpiointerrupt.h"

int main(void)
{
  CHIP_Init();

  // Enable clock for GPIO module, initialize GPIOINT
  CMU_ClockEnable(cmuClock_GPIO, true);
  GPIOINT_Init();

  // Register callback functions and enable interrupts
  GPIOINT_CallbackRegister(1, gpioCallback1);
  GPIOINT_CallbackRegister(3, gpioCallback3);
  GPIOINT_CallbackRegister(8, gpioCallback8);
  GPIO_IntEnable(1<<1 | 1<<3 | 1<<8);

  while(true);
}

   @endverbatim

 * @} end group GPIOINT *******************************************************
 * @} end group emdrv ****************************************************/
