/***************************************************************************//**
 * @file pushButton.c
 * @brief Menu Functions of the Range Test Software Example.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rail_types.h"

#include "spidrv.h"
#include "em_device.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_emu.h"
#include "em_gpio.h"
#include "gpiointerrupt.h"

#include "bsp.h"
#include "app_common.h"

#include "pushButton.h"

#include "bspconfig.h"
#include "graphics.h"
#include "menu.h"
#include "seq.h"

#include "rangeTest.h"

// ----------------------------------------------------------------------------
// Types

/// Pushbutton state structure.
typedef struct pbIntState_t_TAG{
  uint8_t    repeatCnt;   ///< Counter to repeat state if button is held.
  uint16_t   stateCnt;    ///< How long (poll#) is the button pushed.
  pbValues_e actPBState;  ///< Actual unprocessed button state.
  pbValues_e pbState;     ///< Processed button state (pushed short, long etc.).
} pbIntState_t;

// ----------------------------------------------------------------------------
// Defines

// Defines for pushbutton states
#define PRESSED   0u
#define RELEASED  1u

// ----------------------------------------------------------------------------
// Global symbols

// Storage of pushbutton logical levels
const ButtonArray_t buttonArray[BSP_NO_OF_BUTTONS] = BSP_GPIO_BUTTONARRAY_INIT;

// ----------------------------------------------------------------------------
// Local variables

static pbState_t    pbState;
static pbIntState_t pbStates[PB_ANY];

// ----------------------------------------------------------------------------
// Function definitions

/**************************************************************************//**
 * @brief    Function to initialize the pusbutton states.
 *
 * @return  None.
 *****************************************************************************/
void pbInit()
{
  uint8_t i;

  for (i = 0u; i < PB_ANY; i++) {
    pbStates[i].actPBState = PB_WAITRELEASE;
    pbStates[i].pbState = PB_RELEASED;
    pbStates[i].repeatCnt = 0u;
    pbStates[i].stateCnt = 0u;
  }
}

/**************************************************************************//**
 * @brief  Returns if a button on the given pin is pushed or released.
 *
 * @param  pin: The pin of the button which is checked.
 *
 * @return Returns the state of the pin or RELEASED.
 *****************************************************************************/
bool pbGet(uint8_t pin)
{
  if (pin < BSP_NO_OF_BUTTONS) {
    return (GPIO_PinInGet(buttonArray[pin].port, buttonArray[pin].pin));
  }

  return RELEASED;
}

/**************************************************************************//**
 * @brief  Pushbutton polling function.
 *
 * @note   Must be called periodically.
 *
 * @return None.
 *****************************************************************************/
void pbPoll()
{
  bool lActState;
  uint8_t i;

  static uint8_t lPBRepCntTh = PB_LONG_REPEAT_COUNT;
  static uint8_t lPBRepThreshCnt = 0u;

  for (i = 0u; i < PB_ANY; i++) {
    lActState = pbGet(i);

    switch (pbStates[i].actPBState) {
      case PB_WAITRELEASE:
        if (RELEASED == lActState) {
          pbStates[i].actPBState = PB_RELEASED;
        }
        break;

      case PB_RELEASED:
        if (lActState == PRESSED) {
          pbStates[i].pbState = pbStates[i].actPBState \
                                  = PB_SHORT;
          pbStates[i].stateCnt = 0u;
          lPBRepCntTh = PB_LONG_REPEAT_COUNT;
        }
        break;

      case PB_SHORT:
        if (lActState == RELEASED) {
          pbStates[i].actPBState = PB_RELEASED;
        } else {
          pbStates[i].stateCnt++;
          if (pbStates[i].stateCnt > PB_LONG_COUNT) {
            pbStates[i].pbState = pbStates[i].actPBState \
                                    = PB_LONG;
          }
        }
        break;

      case PB_LONG:
        if (lActState == RELEASED) {
          pbStates[i].actPBState = PB_RELEASED;
        } else {
          pbStates[i].repeatCnt++;
          if (pbStates[i].repeatCnt > lPBRepCntTh) {
            lPBRepThreshCnt++;
            if (lPBRepThreshCnt > PB_LONG_STEP_THRESH) {
              lPBRepThreshCnt = 0u;
              lPBRepCntTh = (lPBRepCntTh > PB_LONG_STEP_MIN)
                            ? (lPBRepCntTh - PB_LONG_REPEAT_DEC_STEP)
                            : (PB_LONG_STEP_MIN);
            }
            pbStates[i].pbState = PB_LONG;
            pbStates[i].repeatCnt = 0u;
          }
        }
        break;

      default:
        pbStates[i].actPBState = PB_RELEASED;
        break;
    }
  }
}

/**************************************************************************//**
 * @brief  Returns the actual state of the selected PushButton.
 *
 * @param  buttonNr: The pushbutton ID which shall be identified
 *
 * @return pbValues_e: State of the pushbutton queried.
 *****************************************************************************/
pbValues_e pbGetButtonState(eButtons buttonNr)
{
  pbValues_e retVal = PB_RELEASED;

  if (buttonNr < PB_ANY) {
    retVal = pbStates[buttonNr].pbState;
    pbStates[buttonNr].pbState = PB_RELEASED;
  } else {
    uint8_t i;
    uint8_t retTemp = 0u;

    retTemp = 0u;
    for (i = 0u; i < PB_ANY; i++) {
      if (buttonNr == PB_ANY) {
        retTemp += !!(pbStates[i].pbState > PB_RELEASED);
      } else {
        retTemp += !!(pbStates[i].pbState == PB_SHORT);
      }
    }
    retVal = (retTemp) ? (PB_SHORT) : (PB_RELEASED);
  }

  return retVal;
}

/**************************************************************************//**
 * @brief  Function to access the pushbutton state variable.
 *
 * @return Pointer to the pushbuttons' state variable stored in this module.
 *****************************************************************************/
pbState_t * pbGetState()
{
  uint8_t i;

  for (i = 0u; i < PB_ANY; i++) {
    pbState.pb[i] = pbGetButtonState((eButtons)i);
  }

  return &pbState;
}
