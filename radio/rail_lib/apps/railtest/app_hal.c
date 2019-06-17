/***************************************************************************//**
 * @file
 * @brief This file handles the hardware interactions for RAILtest
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

#include <stdio.h>

#include "rail.h"

#include "em_cmu.h"
#include "em_gpio.h"
#include "em_usart.h"

#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

#include "hal-config.h"
#include "bsp.h"
#include "retargetserial.h"
#include "gpiointerrupt.h"
#ifdef EMBER_AF_PLUGIN_LCD_GRAPHICS
#include "graphics.h"
#endif
#include "hal_common.h"

#include "app_common.h"

#ifdef BSP_GPIO_BUTTONS
// App Structures
static const ButtonArray_t buttonArray[BSP_NO_OF_BUTTONS] = BSP_GPIO_BUTTONARRAY_INIT;
// Configuration defines
#ifndef BUTTON_HOLD_MS
#define BUTTON_HOLD_MS (1000UL)
#endif
#endif

#ifndef APP_DISPLAY_BUFFER_SIZE
#define APP_DISPLAY_BUFFER_SIZE 64
#endif

volatile bool serEvent = false;
bool redrawDisplay = true;
// Holds Enable/Disable status of the buttons on the board
static bool initButtonStatus = false;

static void initAntennaDiversity(void);

/******************************************************************************
 * GPIO Callback Declaration
 *****************************************************************************/

void gpioCallback(uint8_t pin);

/******************************************************************************
 * Application HAL Initialization
 *****************************************************************************/
void appHalInit(void)
{
  // Initialize the system clocks and other HAL components
  halInit();

#if !defined(RAIL_IC_SIM_BUILD)
  // Initialize the LCD display
  initGraphics();

  // Initialize the USART and map LF to CRLF
  RETARGET_SerialCrLf(1);

  // For PER test
  GPIO_PinModeSet(PER_PORT, PER_PIN, gpioModePushPull, 1);

  GPIOINT_Init();
  GPIOINT_CallbackRegister(RETARGET_RXPIN, gpioCallback); // for 'sleep'

  // Enable the buttons on the board
  initButtons();

  initAntennaDiversity();
#endif
}

void PeripheralDisable(void)
{
  LedsDisable();
  disableGraphics();
  // Disable the buttons on the board
  deinitButtons();
}

void PeripheralEnable(void)
{
  enableGraphics();
  redrawDisplay = true;
  // Enable the buttons on the board
  initButtons();
}

/**
 * Delay for the specified number of microseconds by busy waiting.
 * @param microseconds Time to delay for in microseconds.
 */
void usDelay(uint32_t microseconds)
{
  uint32_t start = RAIL_GetTime();
  while ((RAIL_GetTime() - start) < microseconds) {
  }
}

void serialWaitForTxIdle(void)
{
  // This is grody and not very serial-independent... but we need to
  // wait for the serial output to have completely cleared the UART
  // before sleeping, and this beats a wishful-thinking worst-case
  // timed delay.
  while ((USART_StatusGet(RETARGET_UART) & USART_STATUS_TXIDLE) == 0) {
  }
}

/******************************************************************************
 * Configurable Peripherals
 *****************************************************************************/

// Graphics
#ifdef EMBER_AF_PLUGIN_LCD_GRAPHICS

void disableGraphics(void)
{
  GRAPHICS_Clear();
  GRAPHICS_Update();
  GRAPHICS_Sleep();

#if defined(HAL_VCOM_ENABLE) && defined(BSP_VCOM_ENABLE_PORT)
  // Some boards use the same pin for VCOM and the display so re-init that pin
  // here just to be safe.
  GPIO_PinModeSet(BSP_VCOM_ENABLE_PORT, BSP_VCOM_ENABLE_PIN, gpioModePushPull, 1);
#endif
}

void enableGraphics(void)
{
  GRAPHICS_Wakeup();
}

// Update any LCD text that should be changed
void updateDisplay(void)
{
  if (redrawDisplay && (logLevel & PERIPHERAL_ENABLE)) {
    redrawDisplay = false;
    char textBuf[APP_DISPLAY_BUFFER_SIZE];

    // Clear what's currently on screen
    GRAPHICS_Clear();

    // Add the demo output strings
    GRAPHICS_AppendString("\n"APP_DEMO_STRING_INIT "\n");
    GRAPHICS_AppendString("");
    snprintf(textBuf, APP_DISPLAY_BUFFER_SIZE, "Rx Count: %05lu",
             counters.receive % 100000);
    GRAPHICS_AppendString(textBuf);
    snprintf(textBuf, APP_DISPLAY_BUFFER_SIZE, "Tx Count: %05lu",
             (counters.userTx + counters.ackTx) % 100000);
    GRAPHICS_AppendString(textBuf);
    snprintf(textBuf, APP_DISPLAY_BUFFER_SIZE, "Channel: %d", channel);
    GRAPHICS_AppendString(textBuf);
    GRAPHICS_AppendString("");
    GRAPHICS_AppendString("   Tx     Rx");

    // Draw Tx/Rx triangles if the timeout hasn't occurred
    GRAPHICS_InsertTriangle(20, 80, 32, true,
                            ((int8_t)((counters.userTx + counters.ackTx) % 10)) * -10);
    GRAPHICS_InsertTriangle(76, 80, 32, false, (counters.receive % 10) * 10);

    // Force a redraw
    GRAPHICS_Update();
  }
}

void initGraphics(void)
{
  GRAPHICS_Init();
}

#else

void updateDisplay(void)
{
}
void disableGraphics(void)
{
}
void enableGraphics(void)
{
}
void initGraphics(void)
{
}

#endif //EMBER_AF_PLUGIN_LCD_GRAPHICS

// LED's
#if defined(BSP_GPIO_LEDS)

void LedSet(int led)
{
  if (logLevel & PERIPHERAL_ENABLE) {
    BSP_LedSet(led);
  }
}

void LedToggle(int led)
{
  if (logLevel & PERIPHERAL_ENABLE) {
    BSP_LedToggle(led);
  }
}

void LedsDisable(void)
{
  BSP_LedClear(0);
  BSP_LedClear(1);
}

#else

void LedSet(int led)
{
}
void LedToggle(int led)
{
}
void LedsDisable(void)
{
}

#endif

// Buttons
#ifdef BSP_GPIO_BUTTONS

#if (BSP_NO_OF_BUTTONS >= 2)
#define APP_BUTTONS (2U)
#else
#define APP_BUTTONS ((uint8_t)BSP_NO_OF_BUTTONS)
#endif

void deinitButtons(void)
{
  // Only deinitialize the buttons if not already deinitialized
  if (initButtonStatus) {
    initButtonStatus = false;
    for (uint8_t i = 0; i < APP_BUTTONS; i++) {
      // Just turn off the callbacks.  That should be enough so we can repurpose this pin.
      GPIO_IntDisable(1 << buttonArray[i].pin);
      GPIO_ExtIntConfig(buttonArray[i].port,
                        buttonArray[i].pin,
                        buttonArray[i].pin,
                        false,
                        false,
                        false);
    }
  }
}

void initButtons(void)
{
  // Only initialize the buttons if not already initialized
  if (!initButtonStatus) {
    initButtonStatus = true;
    for (uint8_t i = 0; i < (uint8_t)BSP_NO_OF_BUTTONS; i++) {
      GPIO_PinModeSet(buttonArray[i].port, buttonArray[i].pin, gpioModeInputPull, 1);
      if (i < APP_BUTTONS) {
        // Button Interrupt Config
        GPIOINT_CallbackRegister(buttonArray[i].pin, gpioCallback);
        GPIO_ExtIntConfig(buttonArray[i].port,
                          buttonArray[i].pin,
                          buttonArray[i].pin,
                          true,
                          true,
                          true);
      }
    }
  }
}

void gpio0LongPress(void)
{
  radioTransmit(0, NULL);
}

void gpio0ShortPress(void)
{
  radioTransmit(1, NULL);
}

void gpio1LongPress(void)
{
}

void gpio1ShortPress(void)
{
  if (!inAppMode(NONE, NULL) || inRadioState(RAIL_RF_STATE_TX, NULL)) {
    return;
  }

  if (inRadioState(RAIL_RF_STATE_RX, NULL)) {
    RAIL_Idle(railHandle, RAIL_IDLE_ABORT, false);
  }

  // Check if next channel exists
  if (RAIL_IsValidChannel(railHandle, channel + 1)
      == RAIL_STATUS_NO_ERROR) {
    channel++;
  } else {
    // Find initial channel
    channel = 0;
    while (RAIL_IsValidChannel(railHandle, channel)
           != RAIL_STATUS_NO_ERROR) {
      channel++;
    }
  }

  changeChannel(channel);
}

void gpioCallback(uint8_t pin)
{
  #define GET_TIME_IN_MS() (RAIL_GetTime() / 1000)

  static uint32_t gpioTimeCapture[APP_BUTTONS];
  // Hold true if a Negative Edge is encountered for the button press
  static bool gpioNegEdge[APP_BUTTONS];

  void(*gpioLongPress_arr[])(void) = { gpio0LongPress, gpio1LongPress };
  void(*gpioShortPress_arr[])(void) = { gpio0ShortPress, gpio1ShortPress };

  if (pin == RETARGET_RXPIN) {
    serEvent = true;
  }
  for (uint8_t i = 0; i < APP_BUTTONS; i++) {
    if (pin == buttonArray[i].pin) {
      if (GPIO_PinInGet(buttonArray[i].port, buttonArray[i].pin) == 0) {
        // Negative Edge
        gpioTimeCapture[i] = GET_TIME_IN_MS();
        gpioNegEdge[i] = true;
      } else {
        // Positive Edge with a preceeding Negative Edge
        if (gpioNegEdge[i]) {
          gpioNegEdge[i] = false;
          if ((elapsedTimeInt32u(gpioTimeCapture[i], GET_TIME_IN_MS())
               > BUTTON_HOLD_MS)) {
            (*gpioLongPress_arr[i])();
          } else {
            (*gpioShortPress_arr[i])();
          }
        }
      }
    }
  }
}

#else

void deinitButtons(void)
{
}
void initButtons(void)
{
}
void gpio0LongPress(void)
{
}
void gpio0ShortPress(void)
{
}
void gpio1LongPress(void)
{
}
void gpio1ShortPress(void)
{
}

void gpioCallback(uint8_t pin)
{
  if (pin == RETARGET_RXPIN) {
    serEvent = true;
  }
}

#endif //BSP_GPIO_BUTTONS

#if     HAL_ANTDIV_ENABLE

// From base hal/plugin/antenna/antenna.h since they're not in hal-config
#define HAL_ANTENNA_MODE_DISABLED  0 /**< Don't alter antenna selection */
#define HAL_ANTENNA_MODE_ENABLE1   1 /**< Use antenna 1 */
#define HAL_ANTENNA_MODE_ENABLE2   2 /**< Use antenna 2 */
#define HAL_ANTENNA_MODE_DIVERSITY 3 /**< Choose antenna 1 or 2 dynamically */

// Establish Tx default mode
#ifdef  HAL_ANTDIV_TX_MODE
  #define ANTENNA_TX_DEFAULT_MODE HAL_ANTDIV_TX_MODE
#else//!HAL_ANTDIV_TX_MODE
  #define ANTENNA_TX_DEFAULT_MODE HAL_ANTENNA_MODE_DIVERSITY
#endif//HAL_ANTDIV_TX_MODE

// Establish Rx default mode
#ifdef  HAL_ANTDIV_RX_MODE
  #define ANTENNA_RX_DEFAULT_MODE HAL_ANTDIV_RX_MODE
#else//!HAL_ANTDIV_RX_MODE
  #define ANTENNA_RX_DEFAULT_MODE HAL_ANTENNA_MODE_DISABLED
#endif//HAL_ANTDIV_RX_MODE

static void initAntennaDiversity(void)
{
 #if (HAL_ANTDIV_TX_MODE != HAL_ANTENNA_MODE_DISABLED)
  txOptions = ((((RAIL_TxOptions_t)HAL_ANTDIV_TX_MODE)
                << RAIL_TX_OPTION_ANTENNA0_SHIFT)
               & (RAIL_TX_OPTION_ANTENNA0 | RAIL_TX_OPTION_ANTENNA1));
 #endif
 #if (HAL_ANTDIV_RX_MODE != HAL_ANTENNA_MODE_DISABLED)
  rxOptions = ((((RAIL_RxOptions_t)HAL_ANTDIV_RX_MODE)
                << RAIL_RX_OPTION_ANTENNA0_SHIFT)
               & (RAIL_RX_OPTION_ANTENNA0 | RAIL_RX_OPTION_ANTENNA1));
 #endif
}

#else//!HAL_ANTDIV_ENABLE

static void initAntennaDiversity(void)
{
}

#endif//HAL_ANTDIV_ENABLE
