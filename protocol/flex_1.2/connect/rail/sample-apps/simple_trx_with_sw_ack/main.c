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
#include "pti.h"
#include "pa.h"
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
volatile bool dataTx  = false; //send a data packet
volatile bool ackTx    = false; //send an ack
bool packetRx = true;  //go into receive mode
uint8_t receiveBuffer[MAX_BUFFER_SIZE];

RAIL_TxData_t transmitPayload;

static const uint8_t data[] = {
  0x0F, 0x16, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66,
  0x77, 0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE,
};

static const uint8_t ack[] = {
  0x0F, 0x16, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11,
  0x00, 0x11, 0x00, 0x11, 0x00, 0x11, 0x00, 0x11,
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

#define LED_RX_DATA (0)
#define LED_RX_ACK  (1)

// Prototypes
void changeRadioConfig(int newConfig);
void radioInit();
void configPayload(RAIL_TxData_t *txInfo);
void gpioCallback(uint8_t pin);
bool compareArray(uint8_t *array1, uint8_t *array2, uint8_t length);

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

  // Initilize Radio
  radioInit();

  // Configure RAIL callbacks with no appended info
  RAIL_RxConfig((RAIL_RX_CONFIG_INVALID_CRC
                 | RAIL_RX_CONFIG_SYNC1_DETECT
                 | RAIL_RX_CONFIG_ADDRESS_FILTERED
                 | RAIL_RX_CONFIG_BUFFER_OVERFLOW),
                false);

  while (1) {
    if (dataTx | ackTx) {
      configPayload(&transmitPayload);
      dataTx = false;
      ackTx = false;
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
  if (ackTx) {
    txInfo->dataPtr = (uint8_t *)&ack[0];
    txInfo->dataLength = sizeof(ack);
  } else {
    txInfo->dataPtr = (uint8_t *)&data[0];
    txInfo->dataLength = sizeof(data);
  }
}

void gpioCallback(uint8_t pin)
{
  dataTx = true;
}

// Compares two arrays of equal length up to 255 bytes
bool compareArray(uint8_t *array1, uint8_t *array2, uint8_t length)
{
  for (int i = 0; i < length; i++) {
    if (array1[i] != array2[i]) {
      return false;
    }
  }
  return true;
}

/******************************************************************************
 * RAIL Callback Implementation
 *****************************************************************************/
void RAILCb_TxPacketSent(RAIL_TxPacketInfo_t *txPacketInfo)
{
  packetRx = true;
}

void RAILCb_RxPacketReceived(void *rxPacketHandle)
{
  RAIL_RxPacketInfo_t *rxPacketInfo = (RAIL_RxPacketInfo_t*)rxPacketHandle;

  // Do nothing if we had no memory to capture the packet
  if (rxPacketInfo == NULL) {
    return;
  }

  if ((rxPacketInfo->dataLength == sizeof(data))
      && compareArray(rxPacketInfo->dataPtr, (uint8_t *)data, rxPacketInfo->dataLength)) {
    ackTx = true; //send an ack
    BSP_LedToggle(LED_RX_DATA); //packet match toggle the led
  } else if ((rxPacketInfo->dataLength == sizeof(ack))
             && compareArray(rxPacketInfo->dataPtr, (uint8_t *)ack, rxPacketInfo->dataLength)) {
    BSP_LedToggle(LED_RX_ACK);
    packetRx = true;
  }
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
  }
  if (status & RAIL_TX_CONFIG_CHANNEL_BUSY) {
  }
  packetRx = true;
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
  // Do nothing
}
