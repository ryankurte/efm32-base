/***************************************************************************//**
 * @file app_hal.c
 * @brief This file handles the hardware interactions for RAILtest
 * @copyright Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include <stdio.h>

#include "rail.h"

#include "em_cmu.h"
#include "em_gpio.h"

#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

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
#endif

// Configuration defines
#ifndef BUTTON_HOLD_MS
#define BUTTON_HOLD_MS (1000UL)
#endif
#ifndef APP_DISPLAY_BUFFER_SIZE
#define APP_DISPLAY_BUFFER_SIZE 64
#endif

volatile bool serEvent = false;
bool redrawDisplay = true;

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

  CMU_ClockEnable(cmuClock_GPIO, true);

  // Initialize the BSP
  BSP_Init(BSP_INIT_BCC);

  // Initialize the LEDs on the board
  initLeds();

  // Initialize the LCD display
  initGraphics();

  // Initialize the USART and map LF to CRLF
  RETARGET_SerialInit();
  RETARGET_SerialCrLf(1);

  // For PER test
  GPIO_PinModeSet(PER_PORT, PER_PIN, gpioModePushPull, 1);

  GPIOINT_Init();
  GPIOINT_CallbackRegister(RETARGET_RXPIN, gpioCallback); // for 'sleep'

  // Enable the buttons on the board
  initButtons();
}

void PeripheralDisable(void)
{
  LedsDisable();
  disableGraphics();
}

void PeripheralEnable(void)
{
  enableGraphics();
  redrawDisplay = true;
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
             counters.transmit % 100000);
    GRAPHICS_AppendString(textBuf);
    snprintf(textBuf, APP_DISPLAY_BUFFER_SIZE, "Channel: %d", channel);
    GRAPHICS_AppendString(textBuf);
    GRAPHICS_AppendString("");
    GRAPHICS_AppendString("   Tx     Rx");

    // Draw Tx/Rx triangles if the timeout hasn't occurred
    GRAPHICS_InsertTriangle(20, 80, 32, true,
                            ((int8_t)(counters.transmit % 10)) * -10);
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
#ifdef BSP_GPIO_LEDS

void initLeds(void)
{
  BSP_LedsInit();
}

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

void initLeds(void)
{
}
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

void initButtons(void)
{
  for (int i = 0; i < BSP_NO_OF_BUTTONS; i++) {
    GPIO_PinModeSet(buttonArray[i].port, buttonArray[i].pin, gpioModeInputPull, 1);
  }

  // Button Interrupt Config

  GPIOINT_CallbackRegister(buttonArray[0].pin, gpioCallback);
  GPIOINT_CallbackRegister(buttonArray[1].pin, gpioCallback);

  GPIO_IntConfig(buttonArray[0].port, buttonArray[0].pin, true, true, true);
  GPIO_IntConfig(buttonArray[1].port, buttonArray[1].pin, true, true, true);
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
    RAIL_RfIdle();
  }

  // Check if next channel exists
  if (RAIL_ChannelExists(channel + 1) == RAIL_STATUS_NO_ERROR) {
    channel++;
  } else {
    // Find initial channel
    channel = 0;
    while (RAIL_ChannelExists(channel) != RAIL_STATUS_NO_ERROR) {
      channel++;
    }
  }

  // Wait for RxStart to succeed
  while (receiveModeEnabled && RAIL_RxStart(channel)) {
    RAIL_RfIdle();
  }

  redrawDisplay = true;
}

void gpioCallback(uint8_t pin)
{
  #define GET_TIME_IN_MS() (RAIL_GetTime() / 1000)

  static uint32_t gpio0TimeCapture;
  static uint32_t gpio1TimeCapture;
  if (pin == RETARGET_RXPIN) {
    serEvent = true;
  }
  if (pin == buttonArray[0].pin) {
    if (GPIO_PinInGet(buttonArray[0].port, buttonArray[0].pin) == 0) {
      // Negative Edge
      gpio0TimeCapture = GET_TIME_IN_MS();
    } else {
      // Positive Edge
      if (elapsedTimeInt32u(gpio0TimeCapture, GET_TIME_IN_MS())
          > BUTTON_HOLD_MS) {
        gpio0LongPress();
      } else {
        gpio0ShortPress();
      }
    }
  } else if (pin == buttonArray[1].pin) {
    if (GPIO_PinInGet(buttonArray[1].port, buttonArray[1].pin) == 0) {
      // Negative Edge
      gpio1TimeCapture = GET_TIME_IN_MS();
    } else {
      // Positive Edge
      if (elapsedTimeInt32u(gpio1TimeCapture, GET_TIME_IN_MS())
          > BUTTON_HOLD_MS) {
        gpio1LongPress();
      } else {
        gpio1ShortPress();
      }
    }
  }
}

#else

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
