/***************************************************************************//**
 * @file main.c
 * @brief Application specific overrides of weak functions defined as part of
 * the test application.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

#include "em_chip.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_emu.h"

#include "radio/radio.h"
#include "rail.h"
#include "rail_types.h"
#include "rail_config.h"
#include "pa.h"

#include "hal_common.h"
#include "bspconfig.h"

// Memory manager configuration
#define RADIO_RX_PACKET_BUFFER_SIZE 256

// General application memory sizes
#define APP_MAX_PACKET_LENGTH  (RADIO_RX_PACKET_BUFFER_SIZE - 12)   /* sizeof(RAIL_RxPacketInfo_t) == 12) */
#define RAIL_RF_XTAL_FREQ      (38400000)

static int  channel = 0;
static int  currentConfig = 0; //default is first in list
static volatile uint16_t rxBufferSize = 0;
static uint8_t *rxBuffer = NULL;

static bool    radioRFReady = false;
static volatile bool    packetSent = false;
static bool    packetRx   = false;
static uint8_t packetSentCount;
static uint32_t RADIO_cTune;

static uint8_t currentTxMode;
static bool    receiveModeEnabled;
static int32_t txPower;

static RAIL_RxPacketInfo_t *RADIO_rxPacketInfo;
static RAIL_TxPacketInfo_t *RADIO_txPacketInfo;

static const RAIL_Init_t railInitParams = {
  APP_MAX_PACKET_LENGTH,
  RAIL_RF_XTAL_FREQ,
  RAIL_CAL_ALL
};

uint32_t RADIO_changeConfig(int newConfig)
{
  uint32_t status;

  /* Stop TX before changing the config */
  if ( currentTxMode == RADIO_RAIL_TX_MODE_TONE ) {
    RAIL_TxToneStop();
  } else if ( currentTxMode == RADIO_RAIL_TX_MODE_STREAM ) {
    RAIL_TxStreamStop();
  }

  /* Turn off the radio before reconfiguring it */
  RAIL_RfIdle();

  /* Reconfigure radio parameters */
  RAIL_PacketLengthConfigFrameType(frameTypeConfigList[newConfig]);
  status = RAIL_RadioConfig( (void *) configList[newConfig]);
  if ( status != 0 ) {
    return status;
  }

  /* Set the radio to a valid channel */
  channel = RAIL_ChannelConfig(channelConfigs[newConfig]);
  currentConfig = newConfig;

  return status;
}

void RADIO_init(uint32_t xoTune)
{
  RADIO_PAInit_t paInit;
  uint8_t railConfig;

  // Initialize the system clocks and other HAL components
  halInit();

  /* */
  packetSentCount = 0;

  // Initialize PA
  paInit = (RADIO_PAInit_t) RADIO_PA_2P4_INIT;
  RADIO_PA_Init(&paInit);

  // Initialize Radio
  radioRFReady = false;
  RAIL_RfInit(&railInitParams);
  while ( !radioRFReady ) {
  }

  /* Set XO tune value if any */
  if ( xoTune != 0 ) {
    RADIO_cTune = xoTune;
    RAIL_SetTune(xoTune);
  }

  // Configure modem, packet handler
  RADIO_changeConfig(currentConfig);

  // Configure RAIL callbacks with no appended info
  railConfig  = RAIL_RX_CONFIG_INVALID_CRC;
  railConfig |= RAIL_RX_CONFIG_SYNC1_DETECT;
  railConfig |= RAIL_RX_CONFIG_ADDRESS_FILTERED;
  railConfig |= RAIL_RX_CONFIG_BUFFER_OVERFLOW;

  RAIL_RxConfig(railConfig, false);

  currentTxMode      = RADIO_RAIL_TX_MODE_OFF;
  receiveModeEnabled = false;
  txPower            = RAIL_TxPowerGet();

  return;
}

uint8_t RADIO_txCWEnable(bool enable)
{
  if ( enable ) {
    RAIL_TxToneStart(channel);
  } else {
    RAIL_TxToneStop();
  }

  return channel;
}

uint32_t RADIO_getCTune(void)
{
  return RAIL_GetTune();
}

void RADIO_setCTune(uint32_t tune)
{
  RADIO_cTune = tune;
  RAIL_SetTune(RADIO_cTune);
  return;
}

int RADIO_receivePacket(uint8_t buffer[], uint16_t bufferSize, uint16_t timeout,
                        RAIL_RxPacketInfo_t **rxPacket)
{
  int ret = 0;

  volatile uint16_t loopCount;

  /* Setup */
  packetRx      = false;
  rxBuffer      = &buffer[0];
  rxBufferSize  = bufferSize;
  RADIO_rxPacketInfo  = NULL;
  RADIO_txPacketInfo  = NULL;

  /* Start receiver */
  RAIL_RfIdle();
  RAIL_RxStart(channel);

  /* Wait for packet to arrive */
  loopCount = 0;
  while ( (packetRx == false) && (loopCount < timeout) ) {
    loopCount++;
    EMU_EnterEM1();
  }
  RAIL_RfIdle();

  if ( packetRx ) {
    ret = 1;
    *rxPacket = RADIO_rxPacketInfo;
  }

  return ret;
}

int RADIO_sendPacket(uint8_t buffer[], uint16_t bufferSize, uint16_t txPayloadSize, uint16_t timeout,
                     RAIL_TxPacketInfo_t **txPacketInfo)
{
  int ret;
  uint8_t radioStat;
  uint32_t loopCount;
  uint32_t loopLimit;
  RAIL_TxData_t txPacket;

  packetSent = false;
  ret        = 0;
  loopCount  = 0;
  loopLimit  = 0;

  loopLimit = timeout * 1000;

  /* Packet format and prep */
  txPacket.dataPtr    = &buffer[0];
  txPacket.dataLength = txPayloadSize;

  /* Send packet */
  RAIL_RfIdle();
  radioStat = RAIL_TxDataLoad(&txPacket);
  if ( radioStat != 0 ) {
    printf("TxDataLoad() failed! stat = %02X\r\n", radioStat);
  }
  radioStat = RAIL_TxStart(channel, NULL, NULL);
  if ( radioStat != 0 ) {
    printf("TxStart() failed! stat = %02X\r\n", radioStat);
  }

  /* Wait for packet to show up */
  while ( (packetSent != true) && (loopCount < loopLimit) ) {
    loopCount++;
  }
  packetSent = false;
  RAIL_RfIdle();

  if ( loopCount < loopLimit ) {
    *txPacketInfo = RADIO_txPacketInfo;
    ret = 1;
  }

  return ret;
}

uint32_t RADIO_getReg(uint32_t addr)
{
  volatile uint32_t *reg = (volatile uint32_t *)addr;

  return *reg;
}

void RADIO_setReg(uint32_t addr, uint32_t value)
{
  volatile uint32_t *reg = (volatile uint32_t *)addr;

  *reg = value;

  return;
}

void RADIO_setRxMode(bool enable)
{
  if ( currentTxMode == RADIO_RAIL_TX_MODE_OFF ) {
    RAIL_RfIdle();
    if ( enable ) {
      RAIL_RxStart(channel);
    }
  }

  receiveModeEnabled = enable;
}

bool RADIO_getRxMode(void)
{
  return receiveModeEnabled;
}

uint32_t RADIO_setChannel(uint16_t newChannel)
{
  uint32_t status;

  status = RAIL_ChannelExists(newChannel);

  if ( status == RAIL_STATUS_NO_ERROR ) {
    channel = newChannel;

    RAIL_RfIdle();

    if ( currentTxMode == RADIO_RAIL_TX_MODE_TONE ) {
      RAIL_TxToneStart(channel);
    } else if ( currentTxMode == RADIO_RAIL_TX_MODE_STREAM ) {
      RAIL_TxStreamStart(channel, PN9_STREAM);
    } else if ( receiveModeEnabled ) {
      RAIL_RxStart(channel);
    }
  }
  return status;
}

uint16_t RADIO_getChannel(void)
{
  return channel;
}

void RADIO_setTxMode(uint8_t mode)
{
  if ( currentTxMode == RADIO_RAIL_TX_MODE_OFF ) {
    RAIL_RfIdle();
    if ( mode == RADIO_RAIL_TX_MODE_TONE ) {
      RAIL_TxToneStart(channel);
      currentTxMode = mode;
    }
  } else if ( mode == RADIO_RAIL_TX_MODE_OFF ) {
    if ( currentTxMode == RADIO_RAIL_TX_MODE_TONE ) {
      RAIL_TxToneStop();
      currentTxMode = mode;
    }
  }
}

uint8_t RADIO_getTxMode(void)
{
  return currentTxMode;
}

int RADIO_getConfig(void)
{
  return currentConfig;
}

void RADIO_setPower(int power)
{
  txPower = RAIL_TxPowerSet(power);

  RAIL_RfIdle();
  if ( currentTxMode == RADIO_RAIL_TX_MODE_TONE ) {
    RAIL_TxToneStart(channel);
  } else if ( currentTxMode == RADIO_RAIL_TX_MODE_STREAM ) {
    RAIL_TxStreamStart(channel, PN9_STREAM);
  } else if ( receiveModeEnabled ) {
    RAIL_RxStart(channel);
  }
}

int RADIO_getPower(void)
{
  return txPower;
}

/******************************************************************************
 * RAIL Callback Implementation
 *****************************************************************************/
void RAILCb_RfReady(void)
{
  radioRFReady = true;
  return;
}

uint8_t radioStateChangedState = 0;
void RAILCb_RadioStateChanged(uint8_t state)
{
  radioStateChangedState = state;
  return;
}

void RAILCb_TxPacketSent(RAIL_TxPacketInfo_t *txPacketInfo)
{
  RADIO_txPacketInfo = txPacketInfo;
  packetSent = true;

  return;
}

void RAILCb_RxPacketReceived(void *rxPacketHandle)
{
  RADIO_rxPacketInfo = (RAIL_RxPacketInfo_t *) rxPacketHandle;
  packetRx = true;

  return;
}

volatile uint8_t rxStatus;
void RAILCb_RxRadioStatus(uint8_t status)
{
  rxStatus = 0;

  if (status & RAIL_RX_CONFIG_INVALID_CRC) {
    rxStatus |= status;
  }
  if (status & RAIL_RX_CONFIG_SYNC1_DETECT) {
    rxStatus |= status;
  }
  if (status & RAIL_RX_CONFIG_PREAMBLE_DETECT) {
    rxStatus |= status;
  }
  if (status & RAIL_RX_CONFIG_BUFFER_OVERFLOW) {
    rxStatus |= status;
  } else if (status & RAIL_RX_CONFIG_ADDRESS_FILTERED) {
    rxStatus |= status;
  }

  return;
}

volatile uint8_t txStatus;
void RAILCb_TxRadioStatus(uint8_t status)
{
  txStatus = 0;

  if (status & RAIL_TX_CONFIG_BUFFER_UNDERFLOW) {
    txStatus |= status;
  }
  if (status & RAIL_TX_CONFIG_CHANNEL_BUSY) {
    txStatus |= status;
  }

  return;
}
/*
   static volatile uint32_t lastAllocSize;
   void *RAILCb_AllocateMemory( uint32_t size )
   {
   lastAllocSize = size;
   return (void *)rxBuffer;
   }

   void RAILCb_FreeMemory(void *ptr)
   {
   return;
   }
 */

/******************************************************************************
 * RAIL Memory Management
 *****************************************************************************/
/*
   We use the RX buffer used with receivePacket at any given time.
 */
void *RAILCb_AllocateMemory(uint32_t size)
{
  void *ptr = NULL;

  if (size > rxBufferSize) {
    return NULL;
  } else {
    ptr = rxBuffer;
  }

  return ptr;
}

void RAILCb_FreeMemory(void *ptr)
{
  // Do nothing
}

void *RAILCb_BeginWriteMemory(void *handle,
                              uint32_t offset,
                              uint32_t *available)
{
  if ( handle != rxBuffer ) {
    *available = 0;
    return NULL;
  }

  *available = rxBufferSize - offset;

  return (void*)((uint32_t)handle + offset);
}

void RAILCb_EndWriteMemory(void *handle, uint32_t offset, uint32_t size)
{
  // Do nothing
}

void RAILCb_TimerExpired(void)
{
  return;
}

void RAILCb_CalNeeded()
{
  // Only calibrate the radio when not currently transmitting
  // or in a transmit mode
  if (currentTxMode == RADIO_RAIL_TX_MODE_OFF) {
    RAIL_CalMask_t pendingCals = RAIL_CalPendingGet();

    // Disable the radio if we have to do an offline calibration
    if (pendingCals & RAIL_CAL_OFFLINE) {
      RAIL_RfIdle();
    }

    // Perform the necessary calibrations and don't save the results
    RAIL_CalStart(NULL, pendingCals, false);

    // Turn the radio back on if we disabled it above
    if ((pendingCals & RAIL_CAL_OFFLINE) && receiveModeEnabled) {
      RAIL_RxStart(channel);
    }
  }
}
