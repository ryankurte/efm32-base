/***************************************************************************//**
 * @file
 * @brief Energy Modes management driver
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
#ifndef SLEEP_H
#define SLEEP_H

#include <stdint.h>
#include <stdbool.h>

/* Device specific header file(s). */
#include "em_device.h"

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup SLEEP
 * @brief
 *   Sleep Management Driver.
 *
 * @details
 *
 *   @n @section sleepdrv_intro Introduction
 *   This is a sleep management module consisting of sleep.c and sleep.h
 *   source files. The main purpose of the module is to make it easy for an
 *   application to always enter the lowest possible energy mode using a simple
 *   API.
 *
 *   The module works by providing an API for defining "sleep blocks" in the
 *   application code. A "sleep block" will block the MCU from entering a certain
 *   energy mode. A "sleep block" can for instance block EM2 entry because an
 *   EM1 only peripheral is in use. These "sleep blocks" are created by the calls to
 *   @ref SLEEP_SleepBlockBegin() and end with @ref SLEEP_SleepBlockEnd().
 *
 *   When an application wants to enter a low energy mode it can call
 *   @ref SLEEP_Sleep() to enter the lowest possible energy mode. This module
 *   will use the "sleep blocks" to figure out the lowest possible energy mode.
 *
 *   Here is an example of how the sleep driver is initialized and how it can
 *   be used to enter EM2.
 *
 * @code
 *   SLEEP_Init_t sleepConfig = {0};
 *   SLEEP_InitEx(&sleepConfig);
 *   SLEEP_SleepBlockBegin(sleepEM3); // Block EM3 entry
 *   SLEEP_Sleep();
 *   SLEEP_SleepBlockEnd(sleepEM3);
 * @endcode
 *
 *   @n @section sleepdrv_callbacks Sleep and Wakeup Events/Callbacks
 *
 *   This module also provides a way to add application callbacks to notify
 *   the application that the MCU is entering sleep, or waking up from sleep.
 *   These callbacks can be provided to the driver when calling
 *   @ref SLEEP_InitEx().
 *
 *   The sleepCallback function is called before entering sleep and the
 *   wakeupCallback is called after waking up from sleep. The sleepCallback
 *   function has a bool return value. This return value can be used to control
 *   if the MCU should really go to sleep or not. Returning true will make
 *   the MCU enter the selected energy mode, while returning false will force
 *   the sleep driver to return without entering a sleep.
 *
 *   Here is an example of how the sleep and wakeup callbacks are used.
 *
 *   @code
 *   static bool beforeSleep(SLEEP_EnergyMode_t mode)
 *   {
 *     printf("sleep\n");
 *     return true;
 *   }
 *
 *   static void afterWakeup(SLEEP_EnergyMode_t mode)
 *   {
 *     printf("wakeup\n");
 *     (void) mode;
 *   }
 *
 *   void main(void)
 *   {
 *     SLEEP_Init_t sleepConfig = {
 *       .sleepCallback = beforeSleep,
 *       .wakeupCallback = afterWakeup
 *     };
 *     SLEEP_InitEx(&sleepConfig);
 *     SLEEP_SleepBlockBegin(sleepEM3); // Block EM3 entry
 *     while (true) {
 *       SLEEP_Sleep();
 *     }
 *     SLEEP_SleepBlockEnd(sleepEM3);
 *   }
 *   @endcode
 *
 * @{
 ******************************************************************************/

/*******************************************************************************
 *******************************   MACROS   ************************************
 ******************************************************************************/

/**
 * This flag can be returned from the @ref restoreCallback function in order to
 * signal that the sleep driver should continue as normal. */
#define SLEEP_FLAG_NONE              0x0

/**
 * This flag can be returned from the @ref restoreCallback function in order to
 * signal to the sleep driver that HF clocks should not be restored and that
 * the sleep driver should go right back to sleep again. */
#define SLEEP_FLAG_NO_CLOCK_RESTORE  0x1u

/*******************************************************************************
 ****************************   CONFIGURATION   ********************************
 ******************************************************************************/

/** Enable/disable the HW block for protecting accidental setting of low energy
 *  modes. Note that some devices do not support blocking energy mode entry in
 *  hardware, on these devices SLEEP_HW_LOW_ENERGY_BLOCK_ENABLED will have no
 *  effect. */
#ifndef SLEEP_HW_LOW_ENERGY_BLOCK_ENABLED
#define SLEEP_HW_LOW_ENERGY_BLOCK_ENABLED    false
#endif

/** Enable/disable calling wakeup callback after EM4 reset. */
#ifndef SLEEP_EM4_WAKEUP_CALLBACK_ENABLED
#define SLEEP_EM4_WAKEUP_CALLBACK_ENABLED    true
#endif

/** Configure default lowest energy mode that the system can be set to.
 *  Possible values:
 *  @li sleepEM2 - EM2, CPU core is turned off, all HF clocks are turned off,
 *                 LF clocks are on.
 *  @li sleepEM3 - EM3, like EM2 + LF clocks are off, RAM retention, GPIO and ACMP
 *                   interrupt is on. */
#ifndef SLEEP_LOWEST_ENERGY_MODE_DEFAULT
#define SLEEP_LOWEST_ENERGY_MODE_DEFAULT    sleepEM3
#endif

/*******************************************************************************
 ******************************   TYPEDEFS   ***********************************
 ******************************************************************************/

/** Status value used for showing the Energy Mode the device is currently in. */
typedef enum {
  /** Status value for EM0. */
  sleepEM0 = 0,

  /** Status value for EM1. */
  sleepEM1 = 1,

  /** Status value for EM2. */
  sleepEM2 = 2,

  /** Status value for EM3. */
  sleepEM3 = 3,

  /** Status value for EM4. */
  sleepEM4 = 4
} SLEEP_EnergyMode_t;

/** Callback function pointer type. */
typedef void (*SLEEP_CbFuncPtr_t)(SLEEP_EnergyMode_t);

/**
 * Initialization structure for the sleep driver. This includes optional
 * callback functions that can be used by the application to get notified
 * about sleep related events and that can be used to control the MCU
 * behavior when waking up from sleep.
 */
typedef struct {
  /**
   * Pointer to the callback function that is being called before the device
   * is going to sleep. This function is optional, if no sleep callback is
   * needed by the application then this field must be set to NULL.
   *
   * This callback function has a return value that can be used to force the
   * sleep driver to not enter a sleep mode. This can be used in applications
   * where for instance timing of events will make it inefficient to enter
   * sleep at a certain point in time.
   */
  bool (*sleepCallback)(SLEEP_EnergyMode_t emode);

  /**
   * Pointer to the callback function that is being called after wake up. This
   * function is optional, if no wake up callback is needed by the application
   * then this field must be set to NULL.
   */
  void (*wakeupCallback)(SLEEP_EnergyMode_t emode);

  /**
   * Pointer to the callback function that is being called after wake up and
   * before the HF clock is restored. This function can be used by the
   * application to signal that the sleep driver should wakeup normally or if
   * it should skip restoring the HF clock and go back to sleep. Delaying HF
   * clock restore is an advanced functionality that can be used to save
   * power in a system with frequent interrupts.
   *
   * These are the supported flags that can be returned from the callback
   * function.
   *
   * @ref SLEEP_FLAG_NONE
   * @ref SLEEP_FLAG_NO_CLOCK_RESTORE
   */
  uint32_t (*restoreCallback)(SLEEP_EnergyMode_t emode);
} SLEEP_Init_t;

/*******************************************************************************
 ******************************   PROTOTYPES   *********************************
 ******************************************************************************/

void SLEEP_Init(SLEEP_CbFuncPtr_t pSleepCb, SLEEP_CbFuncPtr_t pWakeUpCb);

void SLEEP_InitEx(const SLEEP_Init_t * init);

SLEEP_EnergyMode_t SLEEP_LowestEnergyModeGet(void);

SLEEP_EnergyMode_t SLEEP_Sleep(void);

void SLEEP_ForceSleepInEM4(void);

void SLEEP_SleepBlockBegin(SLEEP_EnergyMode_t eMode);

void SLEEP_SleepBlockEnd(SLEEP_EnergyMode_t eMode);

/** @} (end addtogroup SLEEP) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif
#endif /* SLEEP_H */
