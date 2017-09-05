/***************************************************************************//**
 * @file pushButton.h
 * @brief Menu Functions of the Range Test Software Example.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef PUSHBUTTON_H_
#define PUSHBUTTON_H_

#include "app_common.h"

/** Macro that defines the long pressed threshold */
#define PB_LONG_COUNT           60  // Cycles to treat as long press
#define PB_LONG_REPEAT_COUNT    8   // Cycles to wait before repeat
#define PB_LONG_REPEAT_DEC_STEP 2
#define PB_LONG_STEP_THRESH     2
#define PB_LONG_STEP_MIN        1

// ----------------------------------------------------------------------------
// Global Enums & Typedefs

/// Pushbutton identification enumeration.
typedef enum {
  PB0,          ///< Button 0
  PB1,          ///< Button 1
  PB_ANY,       ///< Number of buttons
  PB_ANY_SHORT, ///< Unused
} eButtons;

/// States of operation that pushbuttons can have after processing.
typedef enum {
  PB_WAITRELEASE, ///< Initial state prior RELEASED.
  PB_RELEASED,    ///< Pushbutton is not pushed.
  PB_SHORT,       ///< Short push.
  PB_LONG,        ///< Held pushed.
} pbValues_e;

/// container for the pushbutton states.
typedef struct pbState_t{
  pbValues_e pb[PB_ANY]; ///< Array to hold the processed states of buttons.
} pbState_t;

extern const ButtonArray_t buttonArray[BSP_NO_OF_BUTTONS];

// ----------------------------------------------------------------------------
//Global Function Definitions

void pbInit(void);
void pbPoll(void);

///  Fucntion pointers to be used in menus.
pbState_t * pbGetState();
pbValues_e  pbGetButtonState(eButtons);

#if (PB_LONG_STEP_MIN > PB_LONG_REPEAT_COUNT)
#error Wrong values!
#endif

#endif /* PUSHBUTTON_H_ */
