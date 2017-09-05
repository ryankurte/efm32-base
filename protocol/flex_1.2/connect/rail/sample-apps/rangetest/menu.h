/***************************************************************************//**
 * @file menu.h
 * @brief Menu functions for the Range Test Software Example.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef MENU_H_
#define MENU_H_

// ----------------------------------------------------------------------------
//  Global structure & enumeration definitions

/**************************************************************************//**
 * @brief This structure contains information of a menu item.
 *****************************************************************************/

/// Type for defining the menu items of the main menu and associated data.
typedef struct {
  uint8_t   menuItem;        ///> Kind of the current menu item (enum value).
  uint8_t   menuNextItem;    ///> Kind of the item following the current item.
  bool    (*action)(bool);   ///> Handle triggers
  uint8_t (*repr)(char **);  ///> Display function
  uint8_t   menuHidden;      ///> The mode in which this menu item is hidden.
  bool      menuLong;        ///> True this item handles long button pushes.
} menuItem_t;

/**************************************************************************//**
 * @brief This enumeration contains menu item definitions.
 * @note Be sure not to have 0 for any of the values.
 *****************************************************************************/

/// Enumeration of all the menu items available in the main menu.
typedef enum {
  eAppMenu_ShowInfo         = 0x01, ///> Show Info menu.
  eAppMenu_SetRFMode,               ///> RX or TX Mode.
  eAppMenu_SetRFModulation,         ///> Radio Mulation Mode (FSK,2FSK etc.).
  eAppMenu_SetRFFrequency,          ///> Modulation Frequency (e.g 868 MHz).
  eAppMenu_SetRFDataRate,           ///> Data Rate (e.g. 500 kbps).
  eAppMenu_SetModDevRXBW,           ///> Deviation (e.g. 125 kHz).
  eAppMenu_SetRFPA,                 ///> Antenna Output Power (e.g. +10dBm)
  eAppMenu_SetChannel,              ///> Channel number (e.g. 0..19)
  eAppMenu_SetPktsNum,              ///> Transmitted packet count (e.g. 500).
  eAppMenu_SetPktsLen,              ///> Transmitted payload length (bytes).
  eAppMenu_SetReTransmit,           ///> Unused.
  eAppMenu_SetIDDest,               ///> Set ID of the device to send to.
  eAppMenu_SetIDSrc,                ///> Set ID of device.
  eAppMenu_SetRSSIMode,             ///> Unused.
  eAppMenu_SetMAWindow,             ///> Set moving average window size.
  eAppMenu_SetLogMode,              ///> Turn UART logging on or off.
  eAppMenu_RunDemo,                 ///> Start Range test.
} menuItemIndex_e;

/// Enum to use to tell which menu items are visible in which Range Test modes.
typedef enum {
  NO_HIDE,                          ///> Menu item visible in all modes.
  RX_HIDE,                          ///> Item hidden in RX mode.
  TX_HIDE,                          ///> Item hidden in TX mode.
  TRX_HIDE,                         ///> Unused.
} menuHideInfo_e;

/// Displayable Icon types in an enum.
typedef enum {
  ICON_DOWN,                        ///> Down arrow for menu navigation.
  ICON_PLUS,                        ///> Plus sign if increment is allowed.
  ICON_GO,                          ///> Unused.
  ICON_SHOW,                        ///> 'SHOW' is printed.
  ICON_START,                       ///> 'START' is printed.
  ICON_STOP,                        ///> 'STOP' is printed.
  ICON_BACK,                        ///> 'BACK' is printed.
  ICON_NONE,                        ///> Nothing is printed or drawn.
} menuItemIcon_e;

/// RSSI options. Unused.
typedef enum {
  RSSI_DBM,
  RSSI_ICON,
  RSSI_RAW,
  NUMOF_RSSI_OPTIONS
} rssiMode_e;

// ----------------------------------------------------------------------------
//  Function declarations

void menuInit(void);
bool menuIsFirstItem(uint8_t state);
bool menuIsLastItem(uint8_t state);
char* menuItemStr(uint8_t item);
uint8_t menuItemIcon(uint8_t item);
bool initHandleInput(pbState_t *pInp);
bool menuHandleInput(pbState_t *pInp);
char* menuPrintAligned(char* item, char* value, uint8_t col);
bool menuItemHidden(uint8_t item, uint8_t mode);
uint8_t menuGetActIdx();
uint8_t menuGetDispStartIdx();
bool runTRXHandleInput(pbState_t *pInp);

#endif /* MENU_H_ */
