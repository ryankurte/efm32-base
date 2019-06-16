/***************************************************************************//**
 * @file
 * @brief RAILTEST transmit and receive
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

typedef struct BerStatus{
  uint32_t bytesTotal; /**< Number of bytes to receive */
  uint32_t bytesTested; /**< Number of bytes currently tested */
  uint32_t bitErrors; /**< Number of bits errors detected */
  int8_t   rssi; /**< Current RSSI value during pattern acquisition */
} BerStatus_t;

// Variables for TX
extern RAIL_Events_t lastTxStatus;
extern RAIL_Events_t lastTxAckStatus;
extern uint32_t sentAckPackets;
extern RAIL_TxPacketDetails_t previousTxAppendedInfo;
extern RAIL_TxPacketDetails_t previousTxAckAppendedInfo;

// Variables for RX
extern uint32_t abortRxDelay;
extern RAIL_ScheduleTxConfig_t nextPacketTxTime;

// Variables for BER testing
extern BerStatus_t berStats;
extern bool berTestModeEnabled;

void configRxLengthSetting(uint16_t rxLength);

void loadTxData(uint8_t *data, uint16_t dataLen);
void setNextPacketTime(RAIL_ScheduleTxConfig_t *scheduledTxOptions);
