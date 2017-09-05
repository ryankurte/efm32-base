/***************************************************************************//**
 * @file menu.c
 * @brief Menu Functions of the Range Test Software Example.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rail.h"

#include "bsp.h"
#include "app_common.h"

#include "pushButton.h"
#include "graphics.h"
#include "menu.h"
#include "seq.h"

#include "rail_config.h"
#include "rangeTest.h"

// ----------------------------------------------------------------------------
// Function prototypes
bool menuShowInfo(bool);
bool menuSetRFMode(bool);
bool menuSetRFModulation(bool);
bool menuSetRFFrequency(bool);
bool menuSetRFDataRate(bool);
bool menuSetModDevRXBW(bool);
bool menuSetRFPA(bool);
bool menuSetChannel(bool);
bool menuSetPktsNum(bool);
bool menuSetPktsLen(bool);
bool menuSetReTransmit(bool);
bool menuSetIDDest(bool);
bool menuSetIDSrc(bool);
bool menuSetMAWindow(bool);
bool menuSetLogMode(bool);
bool menuRunDemo(bool);

// Menu items
uint8_t menuShowInfoDisplay(char *buff[]);
uint8_t menuSetRFModeDisplay(char *buff[]);
uint8_t menuSetRFModulationDisplay(char *buff[]);
uint8_t menuSetRFFrequencyDisplay(char *buff[]);
uint8_t menuSetRFDataRateDisplay(char *buff[]);
uint8_t menuSetModDevRXBWDisplay(char *buff[]);
uint8_t menuSetRFPADisplay(char *buff[]);
uint8_t menuSetChannelDisplay(char *buff[]);
uint8_t menuSetPktsNumDisplay(char *buff[]);
uint8_t menuSetPktsLenDisplay(char *buff[]);
uint8_t menuSetReTransmitDisplay(char *buff[]);
uint8_t menuSetIDDestDisplay(char *buff[]);
uint8_t menuSetIDSrcDisplay(char *buff[]);
uint8_t menuSetMAWindowDisplay(char *buff[]);
uint8_t menuSetLogModeDisplay(char *buff[]);
uint8_t menuRunDemoDisplay(char *buff[]);

char *  menuPrint(char* item, char* value);

// ----------------------------------------------------------------------------

// ------------------------------------
// Menu related variables
static uint8_t menuIdx;
static uint8_t menuDispStart;
char scrBuffer[21u];

// ------------------------------------

/**************************************************************************//**
 * @brief Array of menu state structure instances.
 * @note  First item must be the first element too.
 *****************************************************************************/
/// Array of menu state structure instances.
static const menuItem_t menuItem[] = {
  { eAppMenu_ShowInfo, eAppMenu_SetRFMode, \
    menuShowInfo, menuShowInfoDisplay, NO_HIDE, 0 },
  { eAppMenu_SetRFMode, eAppMenu_SetRFModulation, \
    menuSetRFMode, menuSetRFModeDisplay, NO_HIDE, 1 },
  { eAppMenu_SetRFModulation, eAppMenu_SetRFFrequency, \
    menuSetRFModulation, menuSetRFModulationDisplay, NO_HIDE, 1 },
  { eAppMenu_SetRFFrequency, eAppMenu_SetRFDataRate, \
    menuSetRFFrequency, menuSetRFFrequencyDisplay, NO_HIDE, 1 },
  { eAppMenu_SetRFDataRate, eAppMenu_SetModDevRXBW, \
    menuSetRFDataRate, menuSetRFDataRateDisplay, NO_HIDE, 1 },
  { eAppMenu_SetModDevRXBW, eAppMenu_SetRFPA, \
    menuSetModDevRXBW, menuSetModDevRXBWDisplay, NO_HIDE, 1 },
  { eAppMenu_SetRFPA, eAppMenu_SetChannel, \
    menuSetRFPA, menuSetRFPADisplay, RX_HIDE, 1 },
  { eAppMenu_SetChannel, eAppMenu_SetPktsNum, \
    menuSetChannel, menuSetChannelDisplay, RX_HIDE, 1 },
  { eAppMenu_SetPktsNum, eAppMenu_SetPktsLen, \
    menuSetPktsNum, menuSetPktsNumDisplay, RX_HIDE, 1 },
  { eAppMenu_SetPktsLen, eAppMenu_SetIDDest, \
    menuSetPktsLen, menuSetPktsLenDisplay, NO_HIDE, 1 },
/*  {eAppMenu_SetReTransmit  , eAppMenu_SetIDDest        ,            \
        menuSetReTransmit    , menuSetReTransmitDisplay  , RX_HIDE, 1 }, */
  { eAppMenu_SetIDDest, eAppMenu_SetIDSrc, \
    menuSetIDDest, menuSetIDDestDisplay, NO_HIDE, 1 },
  { eAppMenu_SetIDSrc, eAppMenu_SetMAWindow, \
    menuSetIDSrc, menuSetIDSrcDisplay, NO_HIDE, 1 },
  { eAppMenu_SetMAWindow, eAppMenu_SetLogMode, \
    menuSetMAWindow, menuSetMAWindowDisplay, TX_HIDE, 1 },
  { eAppMenu_SetLogMode, eAppMenu_RunDemo, \
    menuSetLogMode, menuSetLogModeDisplay, NO_HIDE, 1 },
  { eAppMenu_RunDemo, 0, \
    menuRunDemo, menuRunDemoDisplay, NO_HIDE, 0 },
};

/**************************************************************************//**
 * @brief  Trigger function that shows the info screen upon a key press
 *         when the Show Information menu item is selected.
 *
 * @param  init: Request resetting.
 *
 * @return Always true.
 *****************************************************************************/
bool menuShowInfo(bool init)
{
  // Nothing to do with init
  if (!init) {
    seqSet(SEQ_INIT);
  }
  return true;
}

/**************************************************************************//**
 * @brief  This function selects between available radio modes.
 *
 * @param  init: User (re)init request.
 *
 * @return Always true.
 *****************************************************************************/
bool menuSetRFMode(bool init)
{
  rangeTest.radioMode++;
  if ((rangeTest.radioMode >= NUMOF_RADIO_MODES) || (init)) {
    rangeTest.radioMode = RADIO_MODE_RX;  // 0u
  }
  return true;
}

/**************************************************************************//**
 * @brief  This function selects (cycles) between destination IDs.
 *
 * @param  init: User (re)init request.
 *
 * @return Always true.
 *****************************************************************************/
bool menuSetIDDest(bool init)
{
  rangeTest.destID++;
  if ((rangeTest.destID > 32u) || (init)) {
    rangeTest.destID = 0u;
  }
  return true;
}

/**************************************************************************//**
 * @brief  This function selects (cycles) between source IDs.
 *
 * @param  init: User (re)init request.
 *
 * @return Always true.
 *****************************************************************************/
bool menuSetIDSrc(bool init)
{
  rangeTest.srcID++;
  if ((rangeTest.srcID > 32u) || (init)) {
    rangeTest.srcID = 0u;
  }
  return true;
}

/**************************************************************************//**
 * @brief  This function selects (cycles) between trasmitted packet count
 *         options.
 *
 * @param  init: User (re)init request.
 *
 * @return Always true.
 *****************************************************************************/
bool menuSetPktsNum(bool init)
{
  static uint16_t pktsNum[8u] =
  { 500u, 1000u, 2500u, 5000u, 10000u, 25000u, 50000u, RANGETEST_TX_REPEAT };
  static uint8_t i = 1u;

  if (!init) {
    i++;
    if (i >= sizeof(pktsNum) / sizeof(*pktsNum)) {
      i = 0u;
    }
  }
  rangeTest.pktsReq = pktsNum[i];

  return true;
}

/**************************************************************************//**
 * @brief  This function selects (cycles) between packet lengths.
 *
 * @param  init: User (re)init request.
 *
 * @return Always true.
 *****************************************************************************/
bool menuSetPktsLen(bool init)
{
  rangeTest.payload++;
  if ((rangeTest.payload > RANGETEST_PAYLOAD_LEN_MAX) || (init)) {
    rangeTest.payload = RANGETEST_PAYLOAD_LEN_MIN;
  }
  return true;
}

/**************************************************************************//**
 * @brief  This function selects between number of retransmit counts.
 * @note   This function is not used.
 *
 * @param  init: User (re)init request.
 *
 * @return Always true.
 *****************************************************************************/
bool menuSetReTransmit(bool init)
{
  rangeTest.retransmitCnt++;
  if ((rangeTest.retransmitCnt > 10u) || (init)) {
    rangeTest.retransmitCnt = 0u;
  }
  return true;
}

/**************************************************************************//**
 * @brief  This function selects (cycles) between moving average window
 *         sizes.
 *
 * @param  init: User (re)init request.
 *
 * @return Always true.
 *****************************************************************************/
bool menuSetMAWindow(bool init)
{
  if ((rangeTest.maSize == 128u) || (init)) {
    rangeTest.maSize = 32u;
  } else {
    rangeTest.maSize <<= 1u;
  }
  return true;
}

/**************************************************************************//**
 * @brief  This function selects if logging is enabled or not.
 *
 * @param  init: User (re)init request.
 *
 * @return Always true.
 *****************************************************************************/
bool menuSetLogMode(bool init)
{
  rangeTest.log = !rangeTest.log;

  if (init) {
    rangeTest.log = false;
  }
  return true;
}

/**************************************************************************//**
 * @brief  This function executes the Range Test in the selected mode.
 *
 * @param  init: User (re)init request.
 *
 * @return False when init requested, true if not.
 *****************************************************************************/
bool menuRunDemo(bool init)
{
  // Nothing to init here
  if (init) {
    return false;
  }
  // (Re-)Init
  rangeTestInit();

  // Update packet field configuration with the payload length.
  FrameLen = rangeTest.payload;
  RAIL_PacketLengthConfigFrameType(&rangeTestFrameConf);

  GRAPHICS_RSSIClear();

  // Start Demo
  seqSet(SEQ_TRX);

  return true;
}

/**************************************************************************//**
 * @brief  This function would be used to set the RF modulation from the
 *         menu.
 * @note   This function is not implemented. RF modulation is fixed as
 *         it was generated in design time.
 *
 * @param  init: User (re)init request.
 *
 * @return Always false
 *****************************************************************************/
bool menuSetRFModulation(bool init)
{
  return false;
}

/**************************************************************************//**
 * @brief  This function would be used to select from radio frequencies.
 * @note   This function is not implemented (read-only).
 *
 * @param  init: User (re)init request.
 *
 * @return Always false.
 *****************************************************************************/
bool menuSetRFFrequency(bool init)
{
  return false;
}

/**************************************************************************//**
 * @brief  This function would select between data rates.
 * @note   This function is not implemented (read-only).
 *
 * @param  init: User (re)init request.
 *
 * @return Always false.
 *****************************************************************************/
bool menuSetRFDataRate(bool init)
{
  return false;
}

/**************************************************************************//**
 * @brief  This function would set the deviation.
 * @note   This function is not implemented (read-only).
 *
 * @param  init: User (re)init request.
 *
 * @return Always false.
 *****************************************************************************/
bool menuSetModDevRXBW(bool init)
{
  return false;
}

/**************************************************************************//**
 * @brief  This function sets the output power.
 *
 * @param  init: User (re)init request.
 *
 * @return True if value was changed, false if not.
 *****************************************************************************/
bool menuSetRFPA(bool init)
{
  if (init) {
    // Init the rangetest power field to the current power level
    // to sync demanded and current value.
    rangeTest.txPower = (int16_t)RAIL_TxPowerGet();
  } else {
    rangeTest.txPower += RAIL_TX_POWER_INC;
    // Limit and cycle requested power.
    if ( rangeTest.txPower > RAIL_TX_POWER_MAX) {
      rangeTest.txPower = RAIL_TX_POWER_MIN;
    }
    // Return value ignored here
    (void)RAIL_TxPowerSet((int32_t)rangeTest.txPower);
  }
  return true;
}

/**************************************************************************//**
 * @brief  This function selects (cycles) which channel to use for TX/RX.
 *
 * @param  init: User (re)init request.
 *
 * @return Always true.
 *****************************************************************************/
bool menuSetChannel(bool init)
{
  rangeTest.channel++;
  if ((rangeTest.channel >= channelConfigs[0]->configs[0u].channelNumberEnd)
      || (init)) {
    // Initialize the channel to the first one in channel config
    rangeTest.channel = channelConfigs[0]->configs[0u].channelNumberStart;
  }

  return true;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu
 *         to show some information about the Range Test Example.
 *
 * @param  buff[]: Buffer that is to be filled out with the string
 *                 that will be printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu-
 *****************************************************************************/
uint8_t menuShowInfoDisplay(char *buff[])
{
  if (buff) {
    //static /*const?*/ char *txt = "Show Information";
    *buff = "Show Information";
  }
  return ICON_SHOW;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu
 *         to set the Range Test mode (RX or TX role).
 *
 * @param  buff[]: Buffer that is to be filled out with the string
 *                 that will be printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu.
 *****************************************************************************/
uint8_t menuSetRFModeDisplay(char *buff[])
{
  if (buff) {
    *buff = menuPrint("Mode:",
                      (rangeTest.radioMode == RADIO_MODE_TX) ? ("TX") \
                      : ((rangeTest.radioMode == RADIO_MODE_RX) ? ("RX") : ("TRX")));
  }
  return ICON_PLUS;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu
 *         to set the RF modulation type (e.g. FSK2).
 *
 * @param  buff[]: Buffer that is to be filled out with the string
 *                 that will be printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu.
 *         ICON_NONE means that this menu item is read-only.
 *****************************************************************************/
uint8_t menuSetRFModulationDisplay(char *buff[])
{
  if (buff) {
    *buff = menuPrint("Modulation:", (char *) rangeTestConf.radioModulation);
  }
  return ICON_NONE;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu
 *         to shows the frequency.
 *
 * @param  buff[]: Buffer that is to be filled out with the string
 *                 that will be printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu
 *         ICON_NONE means that this menu item is read-only.
 *****************************************************************************/
uint8_t menuSetRFFrequencyDisplay(char *buff[])
{
  char pVal[11u];

  if (buff) {
    // Check if frequency has fractional part in MHz scale
    if (channelConfigs[0]->configs[0u].baseFrequency % 1000000u) {
      sprintf(pVal,
              "%u.%02uMHz",
              (uint16_t) (channelConfigs[0]->configs[0u].baseFrequency / 1000000u),
              (uint16_t) ((channelConfigs[0]->configs[0u].baseFrequency % 1000000u) / 10000u));
    } else {
      sprintf(pVal,
              "%uMHz",
              (uint16_t) (channelConfigs[0]->configs[0u].baseFrequency / 1000000u));
    }

    *buff = menuPrint("Frequency:", pVal);
  }

  return ICON_NONE;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu  that shows
 *         the data rate.
 *
 * @param  buff[]: Buffer that is to be filled out with the string
 *                 that will be printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu
 *         ICON_NONE means that this menu item is read-only.
 *****************************************************************************/
uint8_t menuSetRFDataRateDisplay(char *buff[])
{
  if (buff) {
    *buff = menuPrint("Data Rate:", (char *) rangeTestConf.radioDataRate);
  }
  return ICON_NONE;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu
 *         that shows the deviation.
 *
 * @param  buff[]: Buffer that is to be filled out with the string
 *                 that will be printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu
 *         ICON_NONE means that this menu item is read-only.
 *****************************************************************************/
uint8_t menuSetModDevRXBWDisplay(char *buff[])
{
  if (buff) {
    *buff = menuPrint("Deviation:", (char *) rangeTestConf.radioDeviation);
  }
  return ICON_NONE;
}

uint8_t menuSetRFPADisplay(char *buff[])
{
  // Enough to hold a sign and two digits,
  // one decimal point and one decimal digist and dBm */
  char pVal[15u];
  // Temporary power variable for actual RAIL output power.
  int16_t power;
  // Temporary power variable for the requested power.
  int16_t reqpower;

  if (buff) {
    power = (int16_t)(RAIL_TxPowerGet());
    reqpower = rangeTest.txPower;
    sprintf(pVal,
            "%+i.%d/%+i.%ddBm",
            (reqpower / 10),
            (((reqpower > 0) ? (reqpower) : (-reqpower)) % 10),
            (power / 10),
            (((power > 0) ? (power) : (-power)) % 10) );
    *buff = menuPrint("Power:", pVal);
  }
  return ICON_PLUS;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu
 *         that sets the channel.
 *
 * @param  buff[]: Buffer that is to be filled out with the string
 *                 that will be printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu.
 *****************************************************************************/
uint8_t menuSetChannelDisplay(char *buff[])
{
  char pVal[4u];

  if (buff) {
    sprintf(pVal, "%u", rangeTest.channel);
    *buff = menuPrint("Channel number:", pVal);
  }

  return ICON_PLUS;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu
 *         that shows/sets the number of packets to be transmitted.
 *
 * @param  buff[]: Buffer that is to be filled out with the string
 *                 that will be printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu.
 *****************************************************************************/
uint8_t menuSetPktsNumDisplay(char *buff[])
{
  char pVal[6u];

  if (buff) {
    if (rangeTest.pktsReq != RANGETEST_TX_REPEAT) {
      sprintf(pVal, "%u", rangeTest.pktsReq);
    }
    *buff = menuPrint("Packet Count:",
                      (rangeTest.pktsReq != RANGETEST_TX_REPEAT) ? (pVal) : ("Repeat"));
  }
  ;

  return ICON_PLUS;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu
 *         that shows/sets the channel.
 *
 * @param  Buffer that is to be filled out with the string that will be printed
 *         in th menu.
 *
 * @return Icon type to be used by the caller for this menu.
 *****************************************************************************/
uint8_t menuSetPktsLenDisplay(char *buff[])
{
  char pVal[3u];

  if (buff) {
    sprintf(pVal, "%u", rangeTest.payload);
    *buff = menuPrint("Payload length:", pVal);
  }

  return ICON_PLUS;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu
 *         that shows the re-transmission count.
 * @note   This function is not used.
 *
 * @param  buff[]: Buffer that is to be filled out with the string that will be
 *                 printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu.
 *****************************************************************************/
uint8_t menuSetReTransmitDisplay(char *buff[])
{
  char pVal[3u];

  if (buff) {
    sprintf(pVal, "%u", rangeTest.retransmitCnt);
    *buff = menuPrint("Re-Transmit:",
                      (!rangeTest.retransmitCnt) ? ("Off") : (pVal) );
  }

  return ICON_PLUS;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu to sets/shows
 *         the destination ID.
 *
 * @param  buff[]: Buffer that is to be filled out with the string that will be
 *                 printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu.
 *****************************************************************************/
uint8_t menuSetIDDestDisplay(char *buff[])
{
  char pVal[4u];

  if (buff) {
    sprintf(pVal, "%u", rangeTest.destID);
    *buff = menuPrint("Remote ID:", pVal);
  }

  return ICON_PLUS;
}

/**************************************************************************//**
 * @brief      This function sets the printed menu text for the menu
 *             to sets/shows the source ID.
 *
 * @param[out] buff[]: Buffer that is to be filled out with the string
 *                           that will be printed in th menu
 *
 * @return     Icon type to be used by the caller for this menu
 *****************************************************************************/
uint8_t menuSetIDSrcDisplay(char *buff[])
{
  char pVal[4u];

  if (buff) {
    sprintf(pVal, "%u", rangeTest.srcID);
    *buff = menuPrint("Self ID:", pVal);
  }

  return ICON_PLUS;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu to sets/shows
 *         the size of the moving average window.
 *
 * @param  buff[]: Buffer that is to be filled out with the string that will be
 *                 printed in th menu
 *
 * @return Icon type to be used by the caller for this menu
 *****************************************************************************/
uint8_t menuSetMAWindowDisplay(char *buff[])
{
  char pVal[4u];

  if (buff) {
    sprintf(pVal, "%u", rangeTest.maSize);
    *buff = menuPrint("MA Window size:", pVal);
  }

  return ICON_PLUS;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu
 *         to shows/sets .
 *
 * @param  buff[]: Buffer that is to be filled out with the string that will be
 *                 printed in th menu.
 *
 * @return Icon type to be used by the caller for this menu.
 *****************************************************************************/
uint8_t menuSetLogModeDisplay(char *buff[])
{
  if (buff) {
    *buff = menuPrint("UART log enable:",
                      (rangeTest.log) ? ("Yes") : ("No"));
  }
  return ICON_PLUS;
}

/**************************************************************************//**
 * @brief  This function sets the printed menu text for the menu for executing
 *         the Range Test.
 *
 * @param  buff[]: Buffer that is to be filled out with the string that will be
 *                 printed in th menu
 *
 * @return Icon type to be used by the caller for this menu.
 *****************************************************************************/
uint8_t menuRunDemoDisplay(char *buff[])
{
  if (buff) {
    *buff = "Start Range Test";
  }
  return ICON_START;
}

/**************************************************************************//**
 * @brief  Checks isf the given item is the first element.
 *
 * @param  item: Menu item which is to be cheked if first or not.
 *
 * @return True if this is the first item.
 *****************************************************************************/
bool menuIsFirstItem(uint8_t item)
{
  return (menuItem[0u].menuItem == item);
}

/**************************************************************************//**
 * @brief  Checks isf the given item is the last element.
 *
 * @param  index: Menu item which is to be cheked if last or not.
 *
 * @return True if this is the last item.
 *****************************************************************************/
bool menuIsLastItem(uint8_t index)
{
  if (menuItem[index].menuNextItem == 0) {
    return true;
  }
  return false;
}

/**************************************************************************//**
 * @brief  Handles the list display for menu states that have string values.
 *         Extra aligment is done to pad out the value to  given
 *
 * @param  item: Item string.
 * @param  value: Value string.
 * @param  col: cClumn to align the value to.
 *
 * @return Pointer to (global) string buffer.
 *****************************************************************************/
char* menuPrintAligned(char* item, char* value, uint8_t col)
{
  uint8_t ii = 0u;

  while (*item) {
    scrBuffer[ii] = *item;
    item++;
    ii++;
  }

  while (ii < (col - strlen(value))) {
    scrBuffer[ii] = ' ';
    ii++;
  }

  while (*value) {
    scrBuffer[ii] = *value;
    value++;
    ii++;
  }
  scrBuffer[ii] = 0u;

  return scrBuffer;
}

/**************************************************************************//**
 * @brief  Handles the list display for menu states that have string values.
 *
 * @param  item: Item string.
 * @param  value: Value string.
 *
 * @return Pointer to (global) string buffer.
 *****************************************************************************/
char* menuPrint(char* item, char* value)
{
  return (menuPrintAligned(item, value, 20u));
}

/**************************************************************************//**
 * @brief  Initilization of the menu on the display by going through the actions
 *         of all menu items.
 *
 * @param  None.
 *
 * @return None.
 *****************************************************************************/
void menuInit()
{
  // Go through menu item handlers and let them init themselves

  uint8_t i = 0u;

  while (true) {
    i++;
    menuItem[i].action(true);

    if (menuIsLastItem(i)) {
      break;
    }
  }
}

/**************************************************************************//**
 * @brief  Handles the list display for menu states that have string values.
 *
 * @param  item: Item string.
 *
 * @return Pointer to (global) string buffer.
 *****************************************************************************/
char *menuItemStr(uint8_t item)
{
  char *ret;

  menuItem[item].repr(&ret);
  return ret;
}

/**************************************************************************//**
 * @brief  Triggers the display function of the selected item but it just
 *         determines the type of action icon used for this menu.
 *
 * @param  item: The current menu item index.
 *
 * @return The icon of the chosen item.
 *****************************************************************************/
uint8_t menuItemIcon(uint8_t item)
{
  return menuItem[item].repr(NULL);
}

/**************************************************************************//**
 * @brief  Function that checks if the menu item of a given index is visible
 *         in the menu on the display or not.
 *
 * @param  item: index of the menu item.
 * @param  mode: enum value (menuHideInfo_e) that determines in which mode
 *         should the menu item be visible.
 *
 * @return Pointer to (global) string buffer.
 *****************************************************************************/
bool menuItemHidden(uint8_t item, uint8_t mode)
{
  return (menuItem[item].menuHidden == mode);
}

/**************************************************************************//**
 * @brief  Returns the index of the currently selected menu item.
 *
 * @param  None.
 *
 * @return The index of the currently selected menu item.
 *****************************************************************************/
uint8_t menuGetActIdx()
{
  return menuIdx;
}

/**************************************************************************//**
 * @brief  Returns the menu index that is the top visible menu line.
 *
 * @param  None.
 *
 * @return Pointer to (global) string buffer.
 *****************************************************************************/
uint8_t menuGetDispStartIdx()
{
  return menuDispStart;
}

/**************************************************************************//**
 * @brief  Input handler for the init screen.
 *
 * @param  pInp: State of the pushbuttons .
 *
 * @return Returns true if the button status demands a display update.
 *****************************************************************************/
bool initHandleInput(pbState_t *pInp)
{
  static uint16_t delay = 500u;

  while ((PB_SHORT != pInp->pb[PB0]) && (PB_SHORT != pInp->pb[PB1])) {
    // Timeout only valid for first time init screen
    if (delay) {
      --delay;
      if (!delay) {
        break;
      }
    }
    return false;
  }
  seqSet(SEQ_MENU);

  return true;
}

/**************************************************************************//**
 * @brief  This function handles the button presses and move the menu menu and
 *         menu highlight or carry out the operation associated with the menu.
 *
 * @param  pInp: Pointer to the pushbutton states.
 *
 * @return True if buttin press was registered.
 *****************************************************************************/
bool menuHandleInput(pbState_t *pInp)
{
  bool retVal = false;

  if (pInp->pb[PB1] > PB_RELEASED) {
    // Increase menu index or wrap around
    if (menuIsLastItem(menuIdx)) {
      menuIdx = 0u;
      menuDispStart = 0u;
    } else {
      menuIdx++;
      // Scroll down if cursor past halfway and bottom option not yet visible
      if ( (menuIdx >= (GRAPHICS_MENU_DISP_SIZE / 2u))
           && ((COUNTOF(menuItem) - menuIdx) > (GRAPHICS_MENU_DISP_SIZE / 2u)) ) {
        menuDispStart++;
      }
    }

    retVal = true;
  }

  if (pInp->pb[PB0] > PB_RELEASED) {
    if (pInp->pb[PB0] > PB_SHORT && (!menuItem[menuIdx].menuLong)) {
      return false;
    }
    // Call trigger handler
    retVal = menuItem[menuIdx].action(false);
  }

  return retVal;
}

/**************************************************************************//**
 * @brief  Handles button presses in the TRX mode.
 *
 * @param  pInp: Pointer to the pushbutton states.
 *
 * @return True if one button was pushed.
 *****************************************************************************/
bool runTRXHandleInput(pbState_t *pInp)
{
  if (pInp->pb[PB0] > PB_RELEASED) {
    rangeTest.isRunning = !rangeTest.isRunning;

    // If just started reset counters
    if (rangeTest.isRunning) {
      rangeTest.pktsSent = 0u;
    }

    return true;
  }

  if (pInp->pb[PB1] == PB_SHORT) {
    // Idle Raido
    RAIL_RfIdleExt(RAIL_IDLE, true);

    seqSet(SEQ_MENU);
    return true;
  }

  return false;
}
