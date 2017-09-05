/***************************************************************************//**
 * @file graphics.h
 * @brief Graphics for the Range Test Software Example.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef GRAPHICS_H_
#define GRAPHICS_H_

#include "dmd.h"
#include "glib.h"

// ----------------------------------------------------------------------------
// Function-like macros used in other modules

/**************************************************************************//**
 * @brief     This function-like macro clears the display by calling the GLIB
 *            clear function with the currently active graphics handler.
 *
 * @param     None.
 *
 * @return    None.
 *****************************************************************************/
#define GRAPHICS_Clear()  do  \
  {                           \
    GLIB_clear(&glibContext); \
  } while (0u)

/**************************************************************************//**
 * @brief     This function-like macro updates the display by calling the DMD
 *            display update function.
 *
 * @param     None.
 *
 * @return    None.
 *****************************************************************************/
#define GRAPHICS_Update() do \
  {                          \
    DMD_updateDisplay();     \
  } while (0u)

// Display-specific constants
#define GRAPHICS_MENU_DISP_SIZE        (12u)

// Indices for the minimum and maximum values (last and first index,
// respectively).
#define GRAPHICS_RSSI_MIN_INDEX 2u
#define GRAPHICS_RSSI_MAX_INDEX 0u

// ----------------------------------------------------------------------------
// Global references

/// Global graphic handle for the display.
extern GLIB_Context_t glibContext;

/// Array containing the displayed RSSI axis values. The last and first elements
/// are used for limiting the value displayed.
extern const int8_t RssiChartAxis[3u];

// ----------------------------------------------------------------------------
// Global declarations

void GRAPHICS_Init(void);
void GRAPHICS_DrawInitScreen(GLIB_Context_t *pContext);
void GRAPHICS_DrawButtonIcon(uint8_t location, uint8_t type);
void GRAPHICS_DrawTitleText(GLIB_Context_t *pContext, char * pTxt, GLIB_Font_t *pFont);
void GRAPHICS_DrawMenuItems(GLIB_Context_t *pContext, uint8_t startIdx);
void GRAPHICS_DrawItemPointer(GLIB_Context_t *pContext, uint8_t row);
void GRAPHICS_DrawMenu(GLIB_Context_t *pContext);
void GRAPHICS_DrawTRX(GLIB_Context_t *pContext);
void GRAPHICS_DrawDemo(GLIB_Context_t *pContext);
void GRAPHICS_RSSIClear();
void GRAPHICS_RSSIAdd(uint8_t rssi);
uint8_t GRAPHICS_RSSIGet(uint8_t index);

#endif /* GRAPHICS_H_ */
