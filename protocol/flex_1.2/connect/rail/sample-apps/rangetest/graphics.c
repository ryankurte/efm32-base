/***************************************************************************//**
 * @file grpahics.c
 * @brief Graphics for the Range Test Software Example.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rail.h"

#include "bsp.h"
#include "app_common.h"

#include "pushButton.h"
#include "display.h"
#include "graphics.h"
#include "image.h"
#include "menu.h"

#include "rangeTest.h"

/**************************************************************************//**
 * @brief  Function-like macro for writing a string to the screen.
 *
 * @param  STR: input string.
 * @param  Y: starting line of the top of text.
 * @param  OPAQ: Determines whether to show the background or color it with
 *                  the background. color specified by the GLIB_Context_t.
 *                  If OPAQ == true, the background color is used.
 *
 * @return None
 *****************************************************************************/
#define GLIB_DRAW_STRING_CENTER(STR, Y, OPAQ) do                                            \
  {                                                                                         \
    if (!glibTextOverlay) {                                                                 \
      glibRect.xMin = (LS013B7DH03_WIDTH - (strlen(STR) * (pContext->font.fontWidth))) / 2; \
      glibRect.yMin = (Y);                                                                  \
      glibRect.xMax = (LS013B7DH03_WIDTH + (strlen(STR) * (pContext->font.fontWidth))) / 2; \
      glibRect.yMax = (Y) + pContext->font.fontHeight;                                      \
      pContext->foregroundColor = White;                                                    \
      GLIB_drawRectFilled(pContext, &glibRect);                                             \
      pContext->foregroundColor = Black;                                                    \
    }                                                                                       \
    GLIB_drawString(pContext,                                                               \
                    (STR),                                                                  \
                    strlen(STR),                                                            \
                    (LS013B7DH03_WIDTH - (strlen(STR) * pContext->font.fontWidth)) / 2,     \
                    (Y),                                                                    \
                    (OPAQ));                                                                \
  } while (0u)

/**************************************************************************//**
 * @brief  This function-like macro clears text by erasing the line
 *            containing the it.
 *
 * @param  Y: Starting line where to clear from.
 *
 * @return None.
 *****************************************************************************/
#define GLIB_CLEAR_LINE(Y) do                        \
  {                                                  \
    glibRect.xMin = 0;                               \
    glibRect.yMin = (Y);                             \
    glibRect.xMax = LS013B7DH03_WIDTH;               \
    glibRect.yMax = (Y) + pContext->font.fontHeight; \
    pContext->foregroundColor = White;               \
    GLIB_drawRectFilled(pContext, &glibRect);        \
    pContext->foregroundColor = Black;               \
  } while (0u)

// Constants related to the display
#define GRAPHICS_X_SIZE             128u
#define GRAPHICS_FONT_HEIGHT       (pContext->font.fontHeight)
#define GRAPHICS_MAX_X             (pContext->pDisplayGeometry->xSize - 1u)

#define GRAPHICS_CHART_HEIGHT       33u
#define GRAPHICS_CHART_X_OFFSET     22u
#define GRAPHICS_CHART_SIG           2u
#define GRAPHICS_CHART_SIG_0_OFFSET  5u
#define GRAPHICS_CHART_SIG_1_OFFSET  17u

#define GRAPHICS_CHART_SIZE         (GRAPHICS_X_SIZE            \
                                     - (GRAPHICS_CHART_X_OFFSET \
                                        + GRAPHICS_CHART_SIG    \
                                        + 1 /* Line width */    \
                                        + 1 /* End border */))

#define GRAPHICS_TRX_CHART_X        -1
#define GRAPHICS_TRX_CHART_Y        15u
#define GRAPHICS_TRX_TEXT_SPACING    1u
#define GRAPHICS_TRX_LINE_SPACING    3u

#define GRAPHICS_TX_START_ROW       16u
#define GRAPHICS_TX_LINE_SPACING     8u
#define GRAPHICS_TX_TEXT_SPACING     4u

#define GRAPHICS_RX_CHART_X         -1
#define GRAPHICS_RX_CHART_Y         15u
#define GRAPHICS_RX_LINE_SPACING     3u
#define GRAPHICS_RX_TEXT_SPACING     1u

/* Display device */
DISPLAY_Device_t displayDevice;

/* GLIB context */
GLIB_Context_t glibContext;

const int8_t RssiChartAxis[3u] = { 10, -40, -100 };

bool glibTextOverlay = true;
GLIB_Rectangle_t glibRect;

static uint8_t rssiFinger;
static uint8_t rssiBuffer[GRAPHICS_CHART_SIZE];

/**************************************************************************//**
 * @brief  Initializes the graphics stack.
 * @note   This function will /hang/ if errors occur (usually
 *         caused by faulty displays.
 * @param  None.
 * @return None.
 *****************************************************************************/
void GRAPHICS_Init(void)
{
  EMSTATUS status;

  /* Initialize the DMD module for the DISPLAY device driver. */
  status = DMD_init(0);
  if (DMD_OK != status) {
    while (1u) {
    }
  }
  status = GLIB_contextInit(&glibContext);
  if (GLIB_OK != status) {
    while (1u) {
    }
  }
  glibContext.backgroundColor = White;
  glibContext.foregroundColor = Black;
}

/**************************************************************************//**
 * @brief  Function that draws the splash screen.
 *
 * @param  pContext: Graphical handle to draw on.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawInitScreen(GLIB_Context_t *pContext)
{
  // Heights:
  // Logo: 62 + Title: 8 + Card: 8 + Spacing: 2*5 = 88

  char buff[22u];

  // Draw init screen
  GLIB_clear(pContext);

  GLIB_drawBitmap(pContext,
                  0,
                  10,
                  silabsLogo.img_x,
                  silabsLogo.img_y,
                  (uint8_t *) silabsLogo.image_bits);

  GLIB_setFont(pContext, (GLIB_Font_t *) &GLIB_FontNormal8x8);
  GLIB_DRAW_STRING_CENTER("Range Test", 82, 0);

  GLIB_setFont(pContext, (GLIB_Font_t *)&GLIB_FontNarrow6x8);

  sprintf(buff,
          "EFR32 %03luMHz %+lidBm",
          (channelConfigs[0u]->configs[0].baseFrequency / 1000000ul),
          (RAIL_TxPowerGet() / 10));
  GLIB_DRAW_STRING_CENTER(buff, 105, 0);

  DMD_updateDisplay();
}

/**************************************************************************//**
 * @brief  Function to draw a button icon.
 *
 * @param  location: Button location.
 *                              0 - Left
 *                              1 - Right.
 * @param  type: Button type to display, values from enum menuItemIcon_e.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawButtonIcon(uint8_t location, uint8_t type)
{
  bool bitmap = false;
  char buff[6u];
  uint16_t x, y;

  // Pre-Assume text
  GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNormal8x8);
  x = glibContext.font.fontWidth;
  y = glibContext.font.fontHeight;

  switch (type) {
    case ICON_NONE:
      buff[0u] = '\0';
      x = 0u;         // 0 chars
      break;

    case ICON_SHOW:
      sprintf(buff, "SHOW");
      x = 4u * x;    // SHOW = 4 chars
      break;

    case ICON_START:
      sprintf(buff, "START");
      x = 5u * x;    // START = 5 chars
      break;

    case ICON_STOP:
      sprintf(buff, "STOP");
      x = 4u * x;    // STOP = 4 chars
      break;

    case ICON_BACK:
      sprintf(buff, "BACK");
      x = 4u * x;    // BACK = 4 chars
      break;

    case ICON_GO:
      sprintf(buff, "GO");
      x = 2u * x;    // GO = 2 chars
      break;

    default:
      // Custom icon
      bitmap = true;
      x = icons[type]->img_x;
      y = icons[type]->img_y;
      break;
  }

  if (location) { // RIGHT
    x = (((3 * glibContext.pDisplayGeometry->xSize) / 2u) - x) / 2u;
  } else { // LEFT
    x = ((glibContext.pDisplayGeometry->xSize / 2u) - x) / 2u;
  }
  y = glibContext.pDisplayGeometry->ySize - y - 1u;

  if (bitmap) {
    GLIB_drawBitmap(&glibContext,
                    x,
                    y,
                    icons[type]->img_x,
                    icons[type]->img_y,
                    icons[type]->image_bits);
  } else {
    GLIB_drawString(&glibContext, buff, strlen(buff), x, y, false);
  }
  // Set font back
  GLIB_setFont(&glibContext, (GLIB_Font_t *) &GLIB_FontNarrow6x8);

  // Vertical line that separates them
  GLIB_drawLineV(&glibContext,
                 (glibContext.pDisplayGeometry->xSize / 2u),
                 (glibContext.pDisplayGeometry->ySize - glibContext.font.fontHeight - 2u),
                 glibContext.pDisplayGeometry->ySize);
  // Horizontal line
  GLIB_drawLineH(&glibContext,
                 0u,
                 (glibContext.pDisplayGeometry->ySize - glibContext.font.fontHeight - 2u - 1u),
                 glibContext.pDisplayGeometry->xSize);
}

/**************************************************************************//**
 * @brief  Function to write a title text in the middle.
 *
 * @param  pContext: Graphics handle.
 * @param  pTxt: Pointer to the text to be written.
 * @param  pFont: Pointer to the font to use.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawTitleText(GLIB_Context_t *pContext, char * pTxt, GLIB_Font_t *pFont)
{
  GLIB_setFont(pContext, pFont);
  GLIB_DRAW_STRING_CENTER(pTxt, 2, 0);
}

/**************************************************************************//**
 * @brief  Function to draw the menu points.
 *
 * @param  pContext: Graphics handle.
 * @param  startIdx: Index of the menu item which is drawn first.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawMenuItems(GLIB_Context_t *pContext, uint8_t startIdx)
{
  uint8_t i = startIdx;

  pContext->foregroundColor = Black;
  pContext->backgroundColor = White;

  // Draw menu items
  GLIB_setFont(pContext, (GLIB_Font_t *)&GLIB_FontNarrow6x8);

  while (i < (startIdx + GRAPHICS_MENU_DISP_SIZE)) {
    GLIB_drawString(pContext,
                    menuItemStr(i),
                    strlen(menuItemStr(i)),
                    6u,
                    (16u + (i - startIdx) * 8u),
                    false);

    if (menuIsLastItem(i)) {
      break;
    }
    i++;
  }
}

/**************************************************************************//**
 * @brief  Function to draw the menu.
 *
 * @param  pContext: Graphics handle.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawMenuFrame(GLIB_Context_t *pContext)
{
  pContext->foregroundColor = Black;

  // Title
  glibRect.xMin = glibRect.yMin = 1u;
  glibRect.xMax = pContext->pDisplayGeometry->xSize - 2u;
  glibRect.yMax = pContext->font.fontHeight + 2u;
  GLIB_drawRectFilled(pContext, &glibRect);
  GLIB_drawLineH(pContext, 0u, (glibRect.yMax + 1u), GRAPHICS_MAX_X);
  GLIB_drawLineH(pContext, 0u, (glibRect.yMax + 2u), GRAPHICS_MAX_X);

  // Footer
  glibRect.yMax = pContext->pDisplayGeometry->ySize - 2u;
  glibRect.yMin = glibRect.yMax - pContext->font.fontHeight - 1u;
  GLIB_drawRectFilled(pContext, &glibRect);
  GLIB_drawLineH(pContext, 0u, (glibRect.yMin - 1u), GRAPHICS_MAX_X);
  GLIB_drawLineH(pContext, 0u, (glibRect.yMin - 2u), GRAPHICS_MAX_X);
}

/**************************************************************************//**
 * @brief  Function to clear the RSSI history.
 *
 * @param  None.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_RSSIClear()
{
  uint8_t i;

  // Clear RSSI buffer
  for (i = 0u; i < GRAPHICS_CHART_SIZE; i++) {
    // fill up the buffer with minimal values
    rssiBuffer[i] = (uint8_t)(RssiChartAxis[GRAPHICS_RSSI_MIN_INDEX]);
  }
  // Clear newest pointer
  rssiFinger = 0u;
}

/**************************************************************************//**
 * @brief  This function adds a given RSSI value to the RSSI history buffer.
 *
 * @param  rssi: Current RSSI value to add to the RSSI history.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_RSSIAdd(uint8_t rssi)
{
  rssiFinger++;
  if (GRAPHICS_CHART_SIZE <= rssiFinger) {
    rssiFinger = 0u;
  }
  rssiBuffer[rssiFinger] = rssi;
}

/**************************************************************************//**
 * @brief   Function that returns an element from the given index of the RSSI
 *          history buffer.
 *
 * @param   index: The index of the element we want to get.
 *
 * @returns Actual value of the element.
 *****************************************************************************/
uint8_t GRAPHICS_RSSIGet(uint8_t index)
{
  if (GRAPHICS_CHART_SIZE <= index) {
    return 0u;
  }

  if (index > rssiFinger) {
    index -= rssiFinger;
    return (rssiBuffer[GRAPHICS_CHART_SIZE - index]);
  }

  return (rssiBuffer[rssiFinger - index]);
}

/**************************************************************************//**
 * @brief  Function to draw the RSSI history chart in RX mode.
 *
 * @param  pContext: Graphics handle.
 * @param  x: Leftmost pixel column to start drawing from.
 * @param  y: Top pixel row of the chart. The zero RSSI value is is 32 pixels
 *            lower.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawRSSIChart(GLIB_Context_t *pContext, int16_t x, int16_t y)
{
  uint8_t i;

  for (i = 1u; i < GRAPHICS_CHART_SIZE; i++) {
    GLIB_drawLineV(pContext,
                   (x + i - 1u),
                   ((y + 32u)
                    - ((int8_t)(GRAPHICS_RSSIGet(GRAPHICS_CHART_SIZE - i))
                       - RssiChartAxis[GRAPHICS_RSSI_MIN_INDEX])  / 4),
                   (y + 32u));
  }
}

/**************************************************************************//**
 * @brief  Function that draws the RSSI history chart frame.
 *
 * @param  pContext: Graphics handle.
 * @param  x: Base location X coordinate. Chart is drawn to the left
 *               relative to this position.
 * @param  y: Base location Y coordinate.
 *
 * @return Next free row.
 *****************************************************************************/
uint8_t GRAPHICS_DrawRSSIFrame(GLIB_Context_t *pContext, int16_t x, int16_t y)
{
  char buff[5u];

  // Draw RSSI chart frame
  GLIB_drawLineV(pContext,
                 (x + GRAPHICS_CHART_X_OFFSET + GRAPHICS_CHART_SIG),
                 y,
                 (y + GRAPHICS_CHART_HEIGHT));
  GLIB_drawLineH(pContext,
                 (x + GRAPHICS_CHART_X_OFFSET),
                 (y + GRAPHICS_CHART_SIG_0_OFFSET),
                 (x + GRAPHICS_CHART_X_OFFSET + GRAPHICS_CHART_SIG));
  GLIB_drawLineH(pContext,
                 (x + GRAPHICS_CHART_X_OFFSET),
                 (y + GRAPHICS_CHART_SIG_1_OFFSET),
                 (x + GRAPHICS_CHART_X_OFFSET + GRAPHICS_CHART_SIG));
  GLIB_drawLineH(pContext,
                 (x + GRAPHICS_CHART_X_OFFSET),
                 (y + GRAPHICS_CHART_HEIGHT),
                 GRAPHICS_MAX_X);

  sprintf(buff, "% 3d", RssiChartAxis[0u]);
  GLIB_drawString(pContext, buff, strlen(buff), x, (y + GRAPHICS_CHART_SIG_0_OFFSET - 3u), false);
  sprintf(buff, "% 3d", RssiChartAxis[1u]);
  GLIB_drawString(pContext, buff, strlen(buff), x, (y + GRAPHICS_CHART_SIG_1_OFFSET - 3u), false);
  sprintf(buff, "% 3d", RssiChartAxis[2u]);
  GLIB_drawString(pContext, buff, strlen(buff), x, (y + 3u * GRAPHICS_FONT_HEIGHT), false);

  return (y + GRAPHICS_CHART_HEIGHT); // End row
}

/**************************************************************************//**
 * @brief  Function to write the settings are static, in RX and TX modes.
 *
 * @param  pContext: Graphics handle.
 * @param  row: First row of the static values to be displayed.
 *
 * @return The next row to write to.
 *****************************************************************************/
uint8_t GRAPHICS_DrawConstInfo(GLIB_Context_t *pContext, uint8_t row)
{
  char *ptr;
  char buff[21u];
  uint8_t origRow = row;

  if (RADIO_MODE_TX == rangeTest.radioMode) {
    // Variables for the requested and actual radio output power
    int16_t power;
    int16_t reqpower;

    // Check if frequency has fractional part in MHz scale
    if ( 0u < (channelConfigs[0]->configs[0u].baseFrequency % 1000000ul)) {
      sprintf(buff,
              "%u.%02uMHz",
              (uint16_t) (channelConfigs[0]->configs[0u].baseFrequency / 1000000ul),
              (uint16_t) ((channelConfigs[0]->configs[0u].baseFrequency % 1000000ul) / 10000u));
    } else {
      sprintf(buff,
              "%uMHz",
              (uint16_t) (channelConfigs[0]->configs[0u].baseFrequency / 1000000ul));
    }

    ptr = menuPrintAligned("Frequency:", buff, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    ptr = menuPrintAligned("Data Rate:", RADIO_CONFIG_BITRATE, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    ptr = menuPrintAligned("Modulation:", RADIO_CONFIG_MODULATION_TYPE, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    ptr = menuPrintAligned("Deviation:", RADIO_CONFIG_DEVIATION, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    sprintf(buff, "%ukHz", (uint16_t) (channelConfigs[0]->configs[0u].channelSpacing / 1000u));
    ptr = menuPrintAligned("Ch. spacing:", buff, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    sprintf(buff, "%u", rangeTest.channel);
    ptr = menuPrintAligned("Ch. number:", buff, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    power = (int16_t)(RAIL_TxPowerGet());
    reqpower = rangeTest.txPower;
    // Since power is coming in 0.1 dBm units, integer and fraction digits are split
    sprintf(buff,
            "%+i.%d/%+i.%ddBm",
            (reqpower / 10),
            (((reqpower > 0) ? (reqpower) : (-reqpower)) % 10),
            (power / 10),
            (((power > 0) ? (power) : (-power)) % 10) );
    ptr = menuPrintAligned("Power:", buff, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    sprintf(buff, "%u", rangeTest.srcID);
    ptr = menuPrintAligned("Source ID:", buff, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    sprintf(buff, "%u", rangeTest.destID);
    ptr = menuPrintAligned("Remote ID:", buff, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;
  } else { //if (RADIO_MODE_TX == rangeTest.radioMode)
           // RADIO_MDOE_RX

    // Buffering volatile fields
    uint8_t  destID = rangeTest.destID;
    uint8_t  srcID = rangeTest.srcID;

    if (RADIO_MODE_RX == rangeTest.radioMode) {
      // Check if frequency has fractional part in MHz scale
      if (0u < channelConfigs[0]->configs[0u].baseFrequency % 1000000ul) {
        sprintf(buff,
                "F:%uM%02u DR:%8s",
                (uint16_t) (channelConfigs[0]->configs[0u].baseFrequency / 1000000ul),
                (uint16_t) ((channelConfigs[0]->configs[0u].baseFrequency % 1000000ul) / 10000u),
                RADIO_CONFIG_BITRATE);
      } else {
        sprintf(buff,
                "F:%uMHz DR:%8s",
                (uint16_t) (channelConfigs[0]->configs[0u].baseFrequency / 1000000ul),
                RADIO_CONFIG_BITRATE);
      }

      GLIB_drawString(pContext, buff, strlen(buff), 1u, row, false);
      row += GRAPHICS_FONT_HEIGHT;
    }
    sprintf(buff,
            "Fch:%4ukHz  Ch#: %3u",
            (uint16_t) (channelConfigs[0]->configs[0u].channelSpacing / 1000u),
            rangeTest.channel);
    GLIB_drawString(pContext, buff, strlen(buff), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    if (RADIO_MODE_TRX == rangeTest.radioMode) {
      sprintf(buff, "Fdev:%s P:%+lidBm", RADIO_CONFIG_DEVIATION, (RAIL_TxPowerGet() / 10));
    } else {
      sprintf(buff, "Fdev:%8s M:%5s", RADIO_CONFIG_DEVIATION, RADIO_CONFIG_MODULATION_TYPE);
    }
    GLIB_drawString(pContext, buff, strlen(buff), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    sprintf(buff, "src: %3u     dst: %3u", srcID, destID);
    GLIB_drawString(pContext, buff, strlen(buff), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;
  }

  return (row - origRow);
}

/**************************************************************************//**
 * @brief  This function writes the dynamically changing values in RX and
 *         TX modes.
 *
 * @param  pContext: Graphics handle.
 * @param  row: Starting row for displaying the values.
 *
 * @return The next row available to write to.
 *****************************************************************************/
uint8_t GRAPHICS_DrawVarInfo(GLIB_Context_t *pContext, uint8_t row)
{
  char *ptr;
  char buff[21u];
  uint8_t origRow = row;

  // volatile fields used on multiple places
  uint8_t  radioMode = rangeTest.radioMode;
  uint16_t pktsSent = rangeTest.pktsSent;

  if (RADIO_MODE_TX == radioMode) {
    sprintf(buff, "%5u", pktsSent);
    ptr = menuPrintAligned("Transmitted:", buff, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;
  } else { // RADIO_MODE_TX
           //buffering volatile data fields used only here
    uint16_t pktsCnt = rangeTest.pktsCnt;
    uint16_t pktsRcvd = rangeTest.pktsRcvd;
    uint8_t  rssiLatch = rangeTest.rssiLatch;
    float    PER = rangeTest.PER;
    float    MA = rangeTest.MA;

    if (RANGETEST_PACKET_COUNT_INVALID == pktsRcvd) {
      sprintf(buff, "---.- dBm");
    } else {
      sprintf(buff, "% 3d dBm", (int8_t)rssiLatch);
    }
    ptr = menuPrintAligned("RSSI:", buff, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    if (RADIO_MODE_TRX == radioMode) {
      sprintf(buff, "%5u", pktsSent);
      ptr = menuPrintAligned("Tx:", buff, 21u);
      GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
      row += GRAPHICS_FONT_HEIGHT;
    }

    if (RANGETEST_PACKET_COUNT_INVALID == pktsRcvd) {
      // No packet received yet.
      sprintf(buff, "  -- / -- ");
    } else {
      sprintf(buff, "%5u/%5u", pktsRcvd, pktsCnt);
    }

    ptr = menuPrintAligned("Rx:", buff, 21u);
    GLIB_drawString(pContext, ptr, strlen(ptr), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;

    if (RANGETEST_PACKET_COUNT_INVALID == pktsRcvd) {
      sprintf(buff, "MA: --.-%%  PER: --.-%%");
    } else {
      // This all mess is added because formatting
      // not seem working for floats like %3.1f...
      // Otherwise printing won't seem fancy
      char tmpStr[10u];

      if (MA < 10.0f) {
        sprintf(tmpStr, "  %1.1f", MA);
      } else if (rangeTest.MA < 100.0f) {
        sprintf(tmpStr, " %2.1f", MA);
      } else {
        sprintf(tmpStr, "%3.1f", MA);
      }
      if (PER < 10.0f) {
        sprintf(buff, "MA:%s%%  PER:  %1.1f%%", tmpStr, PER);
      } else if (PER < 100.0f) {
        sprintf(buff, "MA:%s%%  PER: %2.1f%%", tmpStr, PER);
      } else {
        sprintf(buff, "MA:%s%%  PER:%3.1f%%", tmpStr, PER);
      }
    }
    GLIB_drawString(pContext, buff, strlen(buff), 1u, row, false);
    row += GRAPHICS_FONT_HEIGHT;
  }

  return (row - origRow);
}

/**************************************************************************//**
 * @brief  Function to write the menu highlight.
 *
 * @param  pContext: Graphics handle.
 * @param  row: The row on the display the is to be highlighted.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawItemPointer(GLIB_Context_t *pContext, uint8_t row)
{
  GLIB_drawLineH(pContext, 1u, ((row * 8u) + 16u), 1u);
  GLIB_drawLineH(pContext, 1u, ((row * 8u) + 16u + 1u), 2u);
  GLIB_drawLineH(pContext, 1u, ((row * 8u) + 16u + 2u), 3u);
  GLIB_drawLineH(pContext, 1u, ((row * 8u) + 16u + 3u), 4u);
  GLIB_drawLineH(pContext, 1u, ((row * 8u) + 16u + 4u), 3u);
  GLIB_drawLineH(pContext, 1u, ((row * 8u) + 16u + 5u), 2u);
  GLIB_drawLineH(pContext, 1u, ((row * 8u) + 16u + 6u), 1u);
  //GLIB_drawChar(pContext, '>', 1u, (row * 8u) + 16u, false);
}

/**************************************************************************//**
 * @brief  Function to display the menu.
 *
 * @param  pContext: Graphics handle.

 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawMenu(GLIB_Context_t *pContext)
{
  GRAPHICS_DrawMenuFrame(pContext);

  pContext->foregroundColor = White;
  GRAPHICS_DrawTitleText(pContext, "CONFIGURATION", (GLIB_Font_t *) &GLIB_FontNormal8x8);

  GLIB_drawLineH(pContext, 0u, (GRAPHICS_FONT_HEIGHT + 2u), GRAPHICS_MAX_X);

  GRAPHICS_DrawButtonIcon(0u, ICON_DOWN);
  GRAPHICS_DrawButtonIcon(1u, menuItemIcon(menuGetActIdx()));

  GRAPHICS_DrawMenuItems(pContext, menuGetDispStartIdx());
  GRAPHICS_DrawItemPointer(pContext, menuGetActIdx() - menuGetDispStartIdx());
}

/**************************************************************************//**
 * @brief  Displays the Range Test in RX mode.
 *
 * @param  pContext: Graphics handle.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawRX(GLIB_Context_t *pContext)
{
  uint8_t row;

  GRAPHICS_DrawMenuFrame(pContext);

  pContext->foregroundColor = White;
  GRAPHICS_DrawTitleText(pContext,
                         "Range Test",
                         (GLIB_Font_t *) &GLIB_FontNormal8x8);
  GLIB_drawLineH(pContext, 0u, (GRAPHICS_FONT_HEIGHT + 2u), GRAPHICS_MAX_X);

  GRAPHICS_DrawButtonIcon(0u, ICON_BACK);

  pContext->foregroundColor = Black;
  row = GRAPHICS_DrawRSSIFrame(pContext, GRAPHICS_RX_CHART_X, GRAPHICS_RX_CHART_Y);

  GRAPHICS_DrawRSSIChart(pContext,
                         (GRAPHICS_RX_CHART_X
                          + GRAPHICS_CHART_X_OFFSET
                          + GRAPHICS_CHART_SIG
                          + 1u),
                         GRAPHICS_RX_CHART_Y);

  row += GRAPHICS_RX_LINE_SPACING;
  GLIB_drawLineH(pContext, 0u, row, GRAPHICS_MAX_X);
  row += GRAPHICS_RX_LINE_SPACING;

  row += GRAPHICS_DrawConstInfo(pContext, row);

  row += GRAPHICS_RX_TEXT_SPACING;
  GLIB_drawLineH(pContext, 0u, row, GRAPHICS_MAX_X);
  row += GRAPHICS_RX_LINE_SPACING;

  GRAPHICS_DrawVarInfo(pContext, row);
}

/**************************************************************************//**
 * @brief  Displays the Range Test in TX mode.
 *
 * @param  pContext: Graphics handle.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawTX(GLIB_Context_t *pContext)
{
  uint8_t row;

  GRAPHICS_DrawMenuFrame(pContext);

  pContext->foregroundColor = White;
  GRAPHICS_DrawTitleText(pContext,
                         "Range Test",
                         (GLIB_Font_t *) &GLIB_FontNormal8x8);
  GLIB_drawLineH(pContext, 0u, (GRAPHICS_FONT_HEIGHT + 2u), GRAPHICS_MAX_X);

  GRAPHICS_DrawButtonIcon(1u, ( (rangeTest.isRunning) ? (ICON_STOP) : (ICON_START) ));
  GRAPHICS_DrawButtonIcon(0u, ICON_BACK);

  pContext->foregroundColor = Black;
  row = GRAPHICS_TX_START_ROW;
  row += GRAPHICS_DrawConstInfo(pContext, row);

  row += GRAPHICS_TX_TEXT_SPACING;
  GLIB_drawLineH(pContext, 0u, row, GRAPHICS_MAX_X);
  row += GRAPHICS_TX_LINE_SPACING;

  GRAPHICS_DrawVarInfo(pContext, row);
}

/**************************************************************************//**
 * @brief  Displays the Range Test in TRX mode.
 *
 * @param  pContext: Graphics handle.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawTRX(GLIB_Context_t *pContext)
{
  char buf[21u];
  uint8_t row = 0u;

  GRAPHICS_DrawMenuFrame(pContext);

  pContext->foregroundColor = White;

  //Check if frequency has fractional part in MHz scale
  if (channelConfigs[0]->configs[0u].baseFrequency % 1000000ul) {
    sprintf(buf,
            "%uM%02u %s %s",
            (uint16_t) (channelConfigs[0]->configs[0u].baseFrequency / 1000000ul),
            (uint16_t) ((channelConfigs[0]->configs[0u].baseFrequency % 1000000ul) / 10000u),
            RADIO_CONFIG_MODULATION_TYPE,
            RADIO_CONFIG_BITRATE);
  } else {
    sprintf(buf,
            "%uMHz %s %s",
            (uint16_t) (channelConfigs[0]->configs[0u].baseFrequency / 1000000ul),
            RADIO_CONFIG_MODULATION_TYPE,
            RADIO_CONFIG_BITRATE);
  }

  GRAPHICS_DrawTitleText(pContext, buf, (GLIB_Font_t *) &GLIB_FontNarrow6x8);
  GLIB_drawLineH(pContext, 0u, GRAPHICS_FONT_HEIGHT + 2u, GRAPHICS_MAX_X);

  GRAPHICS_DrawButtonIcon(1u, ( (rangeTest.isRunning) ? (ICON_STOP) : (ICON_START) ));
  GRAPHICS_DrawButtonIcon(0u, ICON_BACK);

  pContext->foregroundColor = Black;
  row += GRAPHICS_DrawRSSIFrame(pContext, GRAPHICS_TRX_CHART_X, GRAPHICS_TRX_CHART_Y);
  GRAPHICS_DrawRSSIChart(pContext,
                         (GRAPHICS_TRX_CHART_X
                          + GRAPHICS_CHART_X_OFFSET
                          + GRAPHICS_CHART_SIG
                          + 1u),
                         GRAPHICS_TRX_CHART_Y);

  // Horizontal separator
  row += GRAPHICS_TRX_LINE_SPACING;
  GLIB_drawLineH(pContext, 0u, row, GRAPHICS_MAX_X);
  row += GRAPHICS_TRX_LINE_SPACING;

  row += GRAPHICS_DrawConstInfo(pContext, row);

  // Horizontal separator
  row += GRAPHICS_TRX_TEXT_SPACING;
  GLIB_drawLineH(pContext, 0u, row, GRAPHICS_MAX_X);
  row += GRAPHICS_TRX_LINE_SPACING;

  GRAPHICS_DrawVarInfo(pContext, row);
}

/**************************************************************************//**
 * @brief  Displays the selected Range Test mode (RX, TX).
 *
 * @param  pContext: Graphics handle.
 *
 * @return None.
 *****************************************************************************/
void GRAPHICS_DrawDemo(GLIB_Context_t *pContext)
{
  switch (rangeTest.radioMode) {
    case RADIO_MODE_RX:
      GRAPHICS_DrawRX(pContext);
      break;

    case RADIO_MODE_TX:
      GRAPHICS_DrawTX(pContext);
      break;

    case RADIO_MODE_TRX:
      GRAPHICS_DrawTRX(pContext);
      break;

    default:
      break;
  }
}

int rtcIntCallbackRegister(void (*pFunction)(void*),
                           void* argument,
                           unsigned int frequency)
{
  return 0;
}
