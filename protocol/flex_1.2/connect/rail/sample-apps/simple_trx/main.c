/***************************************************************************//**
 * @file main.c
 * @brief Application specific overrides of weak functions defined as part of
 * the test application.
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
#include "bsp.h"
#include "gpiointerrupt.h"
#include "rail_config.h"
#include "hal_common.h"

// Memory manager configuration
#define MAX_BUFFER_SIZE  256

// General application memory sizes
#define APP_MAX_PACKET_LENGTH  (MAX_BUFFER_SIZE - 12) /* sizeof(RAIL_RxPacketInfo_t) == 12) */

typedef struct ButtonArray{
  GPIO_Port_TypeDef   port;
  unsigned int        pin;
} ButtonArray_t;

static const ButtonArray_t buttonArray[BSP_NO_OF_BUTTONS] = BSP_GPIO_BUTTONARRAY_INIT;

uint8_t channel = 0;
int currentConfig = 0; //default is first in list
volatile bool packetTx = false; //go into transfer mode
volatile bool packetRx = true;  //go into receive mode
uint8_t receiveBuffer[MAX_BUFFER_SIZE];

RAIL_TxData_t transmitPayload;

static const uint8_t data[] = {
  0x0F, 0x16, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
  0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
};

static const RAIL_Init_t railInitParams = {
  APP_MAX_PACKET_LENGTH,
  RADIO_CONFIG_XTAL_FREQUENCY,
  RAIL_CAL_ALL,
};

static const RAIL_CalInit_t calInitParams = {
  RAIL_CAL_ALL,
  irCalConfig,
};

#define LED_TX (0)
#define LED_RX (1)

// Prototypes
void changeRadioConfig(int newConfig);
void radioInit();
void configPayload(RAIL_TxData_t *txInfo);
void gpioCallback(uint8_t pin);

int main(void)
{
  // Initialize the chip
  CHIP_Init();

  // Initialize the system clocks and other HAL components
  halInit();

  CMU_ClockEnable(cmuClock_GPIO, true);

  // Initialize the BSP
  BSP_Init(BSP_INIT_BCC);

  // Initialize the LEDs on the board
  BSP_LedsInit();

  // Enable the buttons on the board
  for (int i = 0; i < BSP_NO_OF_BUTTONS; i++) {
    GPIO_PinModeSet(buttonArray[i].port, buttonArray[i].pin, gpioModeInputPull, 1);
  }

  // Button Interrupt Config
  GPIOINT_Init();
  GPIOINT_CallbackRegister(buttonArray[0].pin, gpioCallback);
  GPIOINT_CallbackRegister(buttonArray[1].pin, gpioCallback);
  GPIO_IntConfig(buttonArray[0].port, buttonArray[0].pin, false, true, true);
  GPIO_IntConfig(buttonArray[1].port, buttonArray[1].pin, false, true, true);

  // Initialize Radio
  radioInit();

  // Configure RAIL callbacks with no appended info
  RAIL_RxConfig((RAIL_RX_CONFIG_INVALID_CRC
                 | RAIL_RX_CONFIG_SYNC1_DETECT
                 | RAIL_RX_CONFIG_ADDRESS_FILTERED
                 | RAIL_RX_CONFIG_BUFFER_OVERFLOW),
                false);

  // Configure TX payload
  configPayload(&transmitPayload);

  while (1) {
    if (packetTx) {
      packetTx = false;
      RAIL_RfIdleExt(RAIL_IDLE, true);
      RAIL_TxDataLoad(&transmitPayload);
      RAIL_TxStart(channel, NULL, NULL);
    }
    if (packetRx) {
      packetRx = false;
      RAIL_RfIdleExt(RAIL_IDLE, true);
      RAIL_RxStart(channel);
    }
  }
}

/******************************************************************************
 * Configuration Utility Functions
 *****************************************************************************/
void changeRadioConfig(int newConfig)
{
  // Turn off the radio before reconfiguring it
  RAIL_RfIdleExt(RAIL_IDLE, true);

  // Reconfigure the radio parameters
  RAIL_PacketLengthConfigFrameType(frameTypeConfigList[newConfig]);
  if (RAIL_RadioConfig((void*)configList[newConfig])) {
    while (1) ;
  }

  // Set us to a valid channel for this config and force an update in the main
  // loop to restart whatever action was going on
  RAIL_ChannelConfig(channelConfigs[newConfig]);
  currentConfig = newConfig;
}

void radioInit()
{
  RAIL_RfInit(&railInitParams);
  RAIL_RfIdleExt(RAIL_IDLE, true);
  RAIL_CalInit(&calInitParams);
  changeRadioConfig(currentConfig);
}

void configPayload(RAIL_TxData_t *txInfo)
{
  txInfo->dataPtr = (uint8_t *)&data[0];
  txInfo->dataLength = sizeof(data);
}

void gpioCallback(uint8_t pin)
{
  packetTx = true;
}

/******************************************************************************
 * RAIL Callback Implementation
 *****************************************************************************/
void RAILCb_TxPacketSent(RAIL_TxPacketInfo_t *txPacketInfo)
{
  BSP_LedToggle(LED_TX);
  packetRx = true;
}

void RAILCb_RxPacketReceived(void *rxPacketHandle)
{
  BSP_LedToggle(LED_RX);
  packetRx = true;
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
    packetRx = true;
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

void RAILCb_RadioStateChanged(uint8_t state)
{
}

void RAILCb_RfReady()
{
}

void RAILCb_TimerExpired()
{
}

void RAILCb_CalNeeded()
{
}
