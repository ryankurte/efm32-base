/***************************************************************************//**
 * @file seq.c
 * @brief Program sequences for the Range Test Software Example.
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
#include "platformconfig.h"
#include "graphics.h"
#include "menu.h"
#include "seq.h"

#include "rangeTest.h"

// ----------------------------------------------------------------------------
// Variables

/// Sequence handlers arranged in a state machine-like variable.
/// Rows are the different modes (e.g. init, menu, Range Test running).
/// Fields are the different types of functions to handle in that mode.
seqHandler_t seqHdlrs[] =
{
  { initHandleInput, GRAPHICS_DrawInitScreen, NULL },         // Init screen
  { menuHandleInput, GRAPHICS_DrawMenu, NULL },               // Menu
  { runTRXHandleInput, GRAPHICS_DrawDemo, runDemo },          // Range Test
};

/// Current operation mode of the sequence handler.
static uint8_t seqHandlerIdx;

// ----------------------------------------------------------------------------
// Function implementations

/**************************************************************************//**
 * @brief     Initializes the sequence of the Range Test program execution.
 *
 * @param     None.
 *
 * @return    None.
 *****************************************************************************/
void seqInit()
{
  // Draw init screen
  seqHandlerIdx = 0u;
  seqHdlrs[0u].display(&glibContext);
}

/**************************************************************************//**
 * @brief     Sequence execution of the Range Test based on the sequence
 *            handler state machine.
 *
 * @param     None.
 *
 * @return    None.
 *****************************************************************************/
void seqRun()
{
  bool state = false;

  if (NULL != seqHdlrs[seqHandlerIdx].input) {
    state = seqHdlrs[seqHandlerIdx].input(pbGetState());
  }

  if (NULL != seqHdlrs[seqHandlerIdx].runner) {
    state |= seqHdlrs[seqHandlerIdx].runner();
  }

  if (true == state) {
    GRAPHICS_Clear();
    seqHdlrs[seqHandlerIdx].display(&glibContext);
    GRAPHICS_Update();
  }
}

/**************************************************************************//**
 * @brief     Changes the mode of operation into to another one.
 *
 * @param     newIdx: The next mode to change.
 *
 * @return    None.
 *****************************************************************************/
void seqSet(uint8_t newIdx)
{
  if ( (sizeof(seqHdlrs) / sizeof(*seqHdlrs) ) > newIdx) {
    seqHandlerIdx = newIdx;
  }
}
