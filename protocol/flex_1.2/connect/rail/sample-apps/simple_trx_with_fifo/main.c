/***************************************************************************//**
 * @file main.c
 * @brief This application demonstrates the simplest exchange of transmit and receive operation in FIFO mode between two nodes.
 * @copyright Copyright 2017 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include "rail.h"
#include "hal_common.h"
#include "rail_config.h"
#include "em_chip.h"
#include "bsp.h"
#include "retargetserial.h"
#include "gpiointerrupt.h"

#define MAX_FRAME_LENGTH (4093)
#define PACKET_HEADER_LEN (2)

#define LED_TX (0)
#define LED_RX  (1)
#define TX_FIFO_THRESHOLD (40)
#define RX_FIFO_THRESHOLD (100)

const uint8_t channel = 0;
int currentConfig = 0; //default is first in list
uint8_t txPtr[MAX_FRAME_LENGTH];
uint8_t rxPtr[MAX_FRAME_LENGTH];

uint16_t txLength;
uint16_t txWritten;
volatile uint16_t rxReceived;
volatile uint16_t rxExpected;
volatile uint8_t fifoReads;
volatile bool packetTx = false; //go into transfer mode
volatile bool packetRx = true;  //go into receive mode

typedef struct ButtonArray{
  GPIO_Port_TypeDef   port;
  unsigned int        pin;
} ButtonArray_t;

static const ButtonArray_t buttonArray[BSP_NO_OF_BUTTONS] = BSP_GPIO_BUTTONARRAY_INIT;

static const RAIL_Init_t railInitParams = {
  256,
  RADIO_CONFIG_XTAL_FREQUENCY,
  RAIL_CAL_ALL,
};

static const RAIL_CalInit_t calInitParams = {
  RAIL_CAL_ALL,
  irCalConfig,
};

// Prototypes
void changeRadioConfig(int newConfig);
void radioInit();
void gpioCallback(uint8_t pin);
void printBuffer(uint8_t * buffer, const uint16_t bufferLength);
void buttonInit();
void sendPacket(uint16_t packetLength);
void startReceive();

int main(void)
{
  // Initialize the chip
  CHIP_Init();

  // Initialize the system clocks and other HAL components
  halInit();

  // Initialize the BSP
  BSP_Init(BSP_INIT_BCC);

  // Initialize the LEDs on the board
  BSP_LedsInit();

  RETARGET_SerialInit();
  RETARGET_SerialCrLf(1);

  // Initialize gpio for buttons
  buttonInit();

  // Initialize Radio
  radioInit();

  //enable buffer error callbacks
  RAIL_TxConfig(RAIL_TX_CONFIG_BUFFER_UNDERFLOW);
  RAIL_RxConfig(RAIL_RX_CONFIG_BUFFER_OVERFLOW, false);

  //enable fifo mode
  RAIL_DataConfig_t dataConfig = { TX_PACKET_DATA, RX_PACKET_DATA, FIFO_MODE, FIFO_MODE };
  RAIL_DataConfig(&dataConfig);

  //fill the buffer with dummy data
  for (int i = PACKET_HEADER_LEN; i < MAX_FRAME_LENGTH - PACKET_HEADER_LEN; i++) {
    txPtr[i] = i - PACKET_HEADER_LEN;
  }

  printf("BOOTING....\n");

  while (1) {
    if (packetTx) {
      packetTx = false;
      txLength = (rand() + 1) % (MAX_FRAME_LENGTH);
      printf("TX(%d): ", txLength - PACKET_HEADER_LEN);
      printBuffer(txPtr + PACKET_HEADER_LEN, txLength - PACKET_HEADER_LEN);
      printf("\n");
      sendPacket(txLength);
    }
    if (packetRx) {
      packetRx = false;
      startReceive();
    }
  }
}

/******************************************************************************
 * Configuration Utility Functions
 *****************************************************************************/
void sendPacket(uint16_t packetLength)
{
  RAIL_RfIdleExt(RAIL_IDLE_ABORT, true);
  RAIL_ResetFifo(true, false);
  txPtr[0] = (packetLength - PACKET_HEADER_LEN) >> 8;
  txPtr[1] = (packetLength - PACKET_HEADER_LEN) & 0xff;

  RAIL_SetTxFifoThreshold(TX_FIFO_THRESHOLD);
  txWritten = RAIL_WriteTxFifo(txPtr, packetLength);
  RAIL_TxStart(channel, NULL, NULL);
}

void startReceive()
{
  RAIL_RfIdleExt(RAIL_IDLE, true);
  RAIL_ResetFifo(false, true);
  RAIL_SetRxFifoThreshold(RX_FIFO_THRESHOLD); //FIFO size is 512B
  RAIL_EnableRxFifoThreshold();
  rxReceived = 0;
  fifoReads = 0;
  rxExpected = 0;
  RAIL_RxStart(channel);
}

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

void buttonInit()
{
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
}

void radioInit()
{
  RAIL_RfInit(&railInitParams);
  RAIL_RfIdleExt(RAIL_IDLE, true);
  RAIL_CalInit(&calInitParams);
  changeRadioConfig(currentConfig);
}

void gpioCallback(uint8_t pin)
{
  BSP_LedToggle(LED_TX);
  packetTx = true;
}

void printBuffer(uint8_t * buffer, const uint16_t bufferLength)
{
  for (uint16_t i = 0; i < bufferLength; ++i) {
    printf("0x%X, ", buffer[i]);
  }
}

/******************************************************************************
 * RAIL Callback Implementation
 *****************************************************************************/
void RAILCb_TxPacketSent(RAIL_TxPacketInfo_t *txPacketInfo)
{
  packetRx = true;
}

void storeReceivedPackets(uint16_t bytesAvailable)
{
  if (rxExpected == 0) {
    rxReceived = RAIL_ReadRxFifo(rxPtr, bytesAvailable);
    rxExpected = (rxPtr[0] << 8) + rxPtr[1] + PACKET_HEADER_LEN; //received length + length of header
  } else {
    rxReceived += RAIL_ReadRxFifo(rxPtr + rxReceived, rxExpected - rxReceived);
  }
}

void printReceivedPackets()
{
  printf("RX(%d): ", rxReceived - PACKET_HEADER_LEN);
  printBuffer(rxPtr + PACKET_HEADER_LEN, rxReceived - PACKET_HEADER_LEN);
  printf("\n");
}

void RAILCb_TxFifoAlmostEmpty(uint16_t spaceAvailable)
{
  if ( txLength - txWritten > 0) {
    txWritten += RAIL_WriteTxFifo(txPtr + txWritten, txLength - txWritten);
  }
}

void RAILCb_RxFifoAlmostFull(uint16_t bytesAvailable)
{
  storeReceivedPackets(bytesAvailable);
}

void RAILCb_RxPacketReceived(void *rxPacketHandle)
{
  BSP_LedToggle(LED_RX);

  storeReceivedPackets(RAIL_GetRxFifoBytesAvailable());
  printReceivedPackets();
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
    printf("overflow\n");
  } else if (status & RAIL_RX_CONFIG_ADDRESS_FILTERED) {
  }
}

void RAILCb_TxRadioStatus(uint8_t status)
{
}

void RAILCb_TimerExpired()
{
}
