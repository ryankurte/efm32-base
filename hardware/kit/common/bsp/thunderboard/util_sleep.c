/***************************************************************************//**
 * @file
 * @brief Utility Functions for the Thunderboard Sense
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

#include "thunderboard/util.h"
#include "rtcdriver.h"
#include "em_emu.h"

/**************************************************************************//**
* @addtogroup TBSense_BSP
* @{
******************************************************************************/

/***************************************************************************//**
 * @defgroup Util Utility Functions
 * @{
 * @brief Utility functions
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/* Local variables */
static volatile bool    rtcComplete;/**< Flag to show if the RTC has expired  */
static RTCDRV_TimerID_t rtcId;      /**< The ID of the currently used RTC     */

static void rtcCb(RTCDRV_TimerID_t id, void *user);
void SysTick_Handler(void);

/** @endcond */

/***************************************************************************//**
 * @brief
 *    Sets up the RTC timer used for sleep functions.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t UTIL_sleepInit(void)
{
  uint32_t stat;

  /* Allocate RTC timer and start driver */
  stat = RTCDRV_Init();
  if ( stat != ECODE_EMDRV_RTCDRV_OK ) {
    return stat;
  }

  stat = RTCDRV_AllocateTimer(&rtcId);

  return stat;
}

/***************************************************************************//**
 * @brief
 *    Delays number of milliseconds in sleep mode (EM2) using the RTC
 *
 * @param ms
 *    Number of milliseconds to sleep
 *
 * @return
 *    None
 ******************************************************************************/
void UTIL_sleep(uint32_t ms)
{
  rtcComplete = false;
  RTCDRV_StartTimer(rtcId, rtcdrvTimerTypeOneshot, ms, rtcCb, 0);

  while ( !rtcComplete )
    EMU_EnterEM2(true);

  return;
}

/***************************************************************************//**
 * @brief
 *    Delays number of milliseconds in sleep mode (EM2) using the RTC but
 *    but returns if an event wakes up the MCU
 *
 * @param timeout
 *    Timeout, the maximum number of milliseconds to sleep
 *
 * @return
 *    The number of milliseconds remained from the timeout
 ******************************************************************************/
uint32_t UTIL_waitForEvent(uint32_t timeout)
{
  uint32_t remaining;

  rtcComplete = false;
  RTCDRV_StartTimer(rtcId, rtcdrvTimerTypeOneshot, timeout, rtcCb, 0);

  EMU_EnterEM2(true);

  if ( rtcComplete ) {
    remaining = 0;
  } else {
    RTCDRV_TimeRemaining(rtcId, &remaining);
  }

  return remaining;
}

/***************************************************************************//**
 * @brief
 *    RTC callback function, called when the timer expired
 *
 * @param[in] id
 *    The id of the timer
 *
 * @param[in] user
 *    Extra parameter for user application (not used)
 *
 * @return
 *    None
 ******************************************************************************/
static void rtcCb(RTCDRV_TimerID_t id, void *user)
{
  rtcComplete = true;
}

/** @} */
/** @} {end addtogroup TBSense_BSP} */
