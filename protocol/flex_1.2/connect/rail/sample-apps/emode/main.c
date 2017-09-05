/***************************************************************************//**
 * @file main.c
 * @brief This application allows the user to use Button 1 to move between
 * sleep modes (EModes) supported by the EFR32 and use Button 0 to transmit packets
 * using the RAIL library from any other mode. When Tx is done the device will
 * automatically go back into its previous appState or energyMode. The current
 * appState or energy mode is printed on the LCD screen.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rail.h"
#include "rail_types.h"

#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_emu.h"
#include "em_core.h"

#include "pti.h"
#include "pa.h"
#include "rtcdriver.h"
#include "bsp.h"
#include "gpiointerrupt.h"
#include "graphics.h"

#include "rail_config.h"

#include "hal_common.h"

// Display buffer size
#ifndef APP_DISPLAY_BUFFER_SIZE
#define APP_DISPLAY_BUFFER_SIZE 64
#endif

// Memory manager configuration
#define MAX_BUFFER_SIZE  256

// General application memory sizes
#define APP_MAX_PACKET_LENGTH  (MAX_BUFFER_SIZE - 12) /* sizeof(RAIL_RxPacketInfo_t) == 12) */

// buttons
typedef struct ButtonArray{
  GPIO_Port_TypeDef   port;
  unsigned int        pin;
} ButtonArray_t;
static const ButtonArray_t buttonArray[BSP_NO_OF_BUTTONS] = BSP_GPIO_BUTTONARRAY_INIT;

// Error codes
typedef enum {
  INVALID_APP_STATE,
  FAILED_TO_SET_RADIO_CONFIG
} ErrorCode;

char *errorCodeString[] =
{
  "Invalid App State         ",
  "Failed to set radio config"
};

// App states
typedef enum {
  PACKET_TX,      // Transmit a packet
  PACKET_TX_WAIT, // Wait for the RAIL library to respond
  PACKET_RX,      // Go into receive mode
  PACKET_RX_WAIT, // Device is in receive mode
  SLEEP_MODE_1,   // Go into eMode 1
  SLEEP_MODE_2,   // Go into eMode 2
  SLEEP_MODE_3   // Go into eMode 3
} AppState;
AppState appState = PACKET_RX; // Start off in receive mode
AppState previousAppState = PACKET_RX;
AppState preTxAppState = PACKET_RX;

char *appStateString[] =
{
  "Packet Tx        ",
  "Packet Tx wait...",
  "Packet Rx        ",
  "Packet Rx wait...",
  "Sleep eMode 1    ",
  "Sleep eMode 2    ",
  "Sleep eMode 3    "
};

// global variables
int channel = 0; // the channel used by the application
int currentConfig = 0; //default is first in list
bool button0Pressed = false;
bool button1Pressed = false;
int packetReceived = 0;
int receiveBuffer[MAX_BUFFER_SIZE]; // buffer used for packet reception

// The application payload sent during a tx
RAIL_TxData_t transmitPayload;

// Application payload data sent OTA with each transmission
static const uint8_t data[] = {
  0x0F, 0x16, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
  0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
};

// Parameters used in the initialization of rail
static const RAIL_Init_t railInitParams = {
  APP_MAX_PACKET_LENGTH,
  RADIO_CONFIG_XTAL_FREQUENCY,
  RAIL_CAL_ALL
};

// Function prototypes
static void changeRadioConfig(int newConfig);
static void lcdPrintError(int errorCode);
static void generatePayload(RAIL_TxData_t *txInfo);
void gpioCallback(uint8_t pin);
void rtcDriverCallback(RTCDRV_TimerID_t id);
static void serviceButtonPresses(void);
static void switchAppState(void);
static void updateDisplay(void);

/******************************************************************************
 * App main
 *****************************************************************************/
int main(void)
{
  // Initialize the chip
  CHIP_Init();

  // Initialize the system clocks and other HAL components
  halInit();

  CMU_ClockEnable(cmuClock_GPIO, true);

  // Initialize the BSP
  BSP_Init(BSP_INIT_BCC);

  // Enable the buttons on the board
  for (int i = 0; i < BSP_NO_OF_BUTTONS; i++) {
    GPIO_PinModeSet(buttonArray[i].port, buttonArray[i].pin, gpioModeInputPull, 1);
  }

  // Initialize the LCD display
  GRAPHICS_Init();

  // Button Interrupt Config
  GPIOINT_Init();
  GPIOINT_CallbackRegister(buttonArray[0].pin, gpioCallback);
  GPIOINT_CallbackRegister(buttonArray[1].pin, gpioCallback);
  GPIO_IntConfig(buttonArray[0].port, buttonArray[0].pin, false, true, true);
  GPIO_IntConfig(buttonArray[1].port, buttonArray[1].pin, false, true, true);

  // Initialize Radio
  RAIL_RfInit(&railInitParams);

  // Configure modem, packet handler
  changeRadioConfig(currentConfig);

  // Configure RAIL callbacks with no appended info
  RAIL_RxConfig((RAIL_RX_CONFIG_INVALID_CRC
                 | RAIL_RX_CONFIG_SYNC1_DETECT
                 | RAIL_RX_CONFIG_ADDRESS_FILTERED
                 | RAIL_RX_CONFIG_BUFFER_OVERFLOW),
                false);

  generatePayload(&transmitPayload);

  while (1) {
    switchAppState();
  }
}

/******************************************************************************
 * App specific functions
 *****************************************************************************/

/**************************************************************************//**
 * @brief  Change the radio configuration at runtime.
 *
 * @param[in] newConfig The array index of the radio configuration to use. this
 * is chosen from a list of radio configurations defined in rail_config.h
 *   0 = Bluetooth_LE_Test
 *   1 = IEEE802154_2p4GHz
 *   2 = RAIL
 *****************************************************************************/
static void changeRadioConfig(int newConfig)
{
  // Turn off the radio before reconfiguring it
  RAIL_RfIdleExt(RAIL_IDLE, true);

  // Reconfigure the radio parameters
  RAIL_PacketLengthConfigFrameType(frameTypeConfigList[newConfig]);
  if (RAIL_RadioConfig((void*)configList[newConfig])) {
    lcdPrintError(FAILED_TO_SET_RADIO_CONFIG);
    while (1) ;
  }

  // Set us to a valid channel for this config and force an update in the main
  // loop to restart whatever action was going on
  RAIL_ChannelConfig(channelConfigs[newConfig]);
  currentConfig = newConfig;
}

/**************************************************************************//**
 * @brief   A function for managing what is shown on the LCD display in an
 *          error state.
 *****************************************************************************/
static void lcdPrintError(int errorCode)
{
  char textBuf[APP_DISPLAY_BUFFER_SIZE];
  // Clear what's currently on screen
  GRAPHICS_Clear();
  // Add the demo output strings
  GRAPHICS_AppendString("RAIL eMode Demo");
  GRAPHICS_AppendString("");
  snprintf(textBuf, APP_DISPLAY_BUFFER_SIZE, "ERROR: %d", errorCode);
  GRAPHICS_AppendString(textBuf);
  GRAPHICS_AppendString("");
  GRAPHICS_AppendString(errorCodeString[errorCode]);
  // Force a redraw
  GRAPHICS_Update();
}

/**************************************************************************//**
 * @brief   A utility function for populating a RAIL application payload.
 *
 * @param[in] txInfo  The instance of RAIL_TxData_t to be used in tranmission
 * of the message. This is currently simply populated with the data array
 * defined above.
 *****************************************************************************/
static void generatePayload(RAIL_TxData_t *txInfo)
{
  //Normal code
  txInfo->dataPtr = (uint8_t *)&data[0];
  txInfo->dataLength = sizeof(data);
}

/**************************************************************************//**
 * @brief   A callback to handle a button press on the WSTK
 *
 * @param[in] pin  The pin used to identify the button that has been pressed.
 *****************************************************************************/
void gpioCallback(uint8_t pin)
{
  // button press on 0 causes a transmit from any other state. When tx
  // is complete we'll go back into our previous appState
  if (pin == buttonArray[0].pin) {
    button0Pressed = true;
  }
  // button 1 takes us into our next appState IF we are not
  // in a protected appState, like preparing to transmit or receive
  else if (pin == buttonArray[1].pin) {
    button1Pressed = true;
  }
}

/**************************************************************************//**
 * @brief  A function for handling button presses outside of interrupt
 *         context.
 *****************************************************************************/
static void serviceButtonPresses(void)
{
  CORE_DECLARE_IRQ_STATE;
  CORE_ENTER_CRITICAL(); // Disable interrupts so we can't be whacked while processing
  // button press on 0 causes a transmit from any other state. When tx
  // is complete we'll go back into our previous appState
  if (button0Pressed) {
    button0Pressed = false;
    preTxAppState = appState;
    appState = PACKET_TX;
  }
  // button 1 takes us into our next appState IF we are not
  // in a protected appState, like preparing to transmit or receive
  else if (button1Pressed) {
    button1Pressed = false;
    // protect PACKET_RX, PACKET_TX and PACKET_TX_WAIT since they are
    // all transitional states
    if ((appState == PACKET_TX)
        || (appState == PACKET_TX_WAIT)
        || (appState == PACKET_RX)) {
      // oops nothing hapened we were busy, could print an error here
    } else if (appState == PACKET_RX_WAIT) {
      appState = SLEEP_MODE_1;
    } else if (appState == SLEEP_MODE_1) {
      appState = SLEEP_MODE_2;
    } else if (appState == SLEEP_MODE_2) {
      appState = SLEEP_MODE_3;
    } else if (appState == SLEEP_MODE_3) {
      appState = PACKET_RX;
    } else {
      lcdPrintError(INVALID_APP_STATE);
      while (1) ; //should never get here
    }
  }
  CORE_EXIT_CRITICAL(); // Re-enable interrupts when we are done
}

/**************************************************************************//**
 * @brief   Do what we need to based on our applications current state
 *****************************************************************************/
static void switchAppState(void)
{
  serviceButtonPresses(); //check to see if appState should be modified
  if (appState != previousAppState) {
    previousAppState = appState;
    updateDisplay(); // Only update the display on change
  }
  switch (appState) {
    case PACKET_TX:
      RAIL_RfIdleExt(RAIL_IDLE, true);
      RAIL_TxDataLoad(&transmitPayload);
      RAIL_TxStart(channel, NULL, NULL);
      appState = PACKET_TX_WAIT;
      break;
    case PACKET_TX_WAIT:
      // we have sent a packet and are waiting for info from RAIL
      // just wait for our response, when it comes we'll go back to
      // our entry state.
      break;
    case PACKET_RX:
      RAIL_RfIdleExt(RAIL_IDLE, true);
      RAIL_RxStart(channel);
      appState = PACKET_RX_WAIT;
      break;
    case PACKET_RX_WAIT:
      // we are in receive mode, just wait for something to happen
      // either we'll receive a packet or we'll be pushed into the next state
      break;
    case SLEEP_MODE_1:
      RAIL_RfIdleExt(RAIL_IDLE, true); // Idle the radio so it doesn't keep us out of sleep
      EMU_EnterEM1();
      break;
    case SLEEP_MODE_2:
      RAIL_RfIdleExt(RAIL_IDLE, true); // Idle the radio so it doesn't keep us out of sleep
      EMU_EnterEM2(true); //need to re-enable clocks for RAIL
      break;
    case SLEEP_MODE_3:
      RAIL_RfIdleExt(RAIL_IDLE, true); // Idle the radio so it doesn't keep us out of sleep
      EMU_EnterEM3(true); //need to re-enable clocks for RAIL
      break;
    default:
    {
      lcdPrintError(INVALID_APP_STATE);
      while (1) ; //should never get here
    }
  }
}

/**************************************************************************//**
 * @brief   A function for managing what is shown on the LCD display under
 *          normal conditions.
 *****************************************************************************/
static void updateDisplay(void)
{
  char textBuf[APP_DISPLAY_BUFFER_SIZE];

  // Clear what's currently on screen
  GRAPHICS_Clear();

  // Add the demo output strings
  GRAPHICS_AppendString("RAIL eMode Demo");
  GRAPHICS_AppendString("");
  snprintf(textBuf, APP_DISPLAY_BUFFER_SIZE, "App State: %d", (int)appState);
  GRAPHICS_AppendString(textBuf);
  GRAPHICS_AppendString("");
  GRAPHICS_AppendString(appStateString[appState]);
  GRAPHICS_AppendString("");
  snprintf(textBuf, APP_DISPLAY_BUFFER_SIZE, "Packet Rx: %d", (int)packetReceived);
  GRAPHICS_AppendString(textBuf);

  // Force a redraw
  GRAPHICS_Update();
}

/******************************************************************************
 * RAIL Callback Implementation
 *****************************************************************************/
void RAILCb_TxPacketSent(RAIL_TxPacketInfo_t *txPacketInfo)
{
  if (preTxAppState == PACKET_RX_WAIT) {
    appState = PACKET_RX; //RX will pop us back into RX_WAIT
  } else {
    appState = preTxAppState;
  }
}

void RAILCb_RxPacketReceived(void *rxPacketHandle)
{
  packetReceived++; // increment the counter
  appState = PACKET_RX; // packet received go back into receive mode
}

void RAILCb_RxRadioStatus(uint8_t status)
{
  if (status & RAIL_RX_CONFIG_INVALID_CRC) {
  }
  if (status & RAIL_RX_CONFIG_SYNC1_DETECT) {
  }
  if (status & RAIL_RX_CONFIG_PREAMBLE_DETECT) {
  }
  if (status & RAIL_RX_CONFIG_BUFFER_OVERFLOW) {
  } else if (status & RAIL_RX_CONFIG_ADDRESS_FILTERED) {
  }
}

void RAILCb_TxRadioStatus(uint8_t status)
{
  if (status & RAIL_TX_CONFIG_BUFFER_UNDERFLOW) {
    if (preTxAppState == PACKET_RX_WAIT) {
      appState = PACKET_RX; //RX will pop us back into RX_WAIT
    } else {
      appState = preTxAppState;
    }
  }
  if (status & RAIL_TX_CONFIG_CHANNEL_BUSY) {
  }
}

void *RAILCb_AllocateMemory(uint32_t size)
{
  return receiveBuffer;
}

void *RAILCb_BeginWriteMemory(void *handle,
                              uint32_t offset,
                              uint32_t *available)
{
  return ((uint8_t*)handle) + offset;
}

void RAILCb_EndWriteMemory(void *handle, uint32_t offset, uint32_t size)
{
  // Do nothing
}

void RAILCb_FreeMemory(void *ptr)
{
}
