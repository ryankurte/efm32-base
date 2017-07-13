/***************************************************************************//**
 * @file app_trx.h
 * @brief RAILTEST transmit and receive
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/
extern bool receivingPacket;
extern uint8_t  lastTxStatus;
extern uint32_t previousTxTime;
extern bool berTestModeEnabled;
extern uint32_t abortRxDelay;

void configRxLengthSetting(uint16_t rxLength);

void loadTxData(RAIL_TxData_t *transmitData);

void berGetStats(RAIL_BerStatus_t *status);
void berResetStats(uint32_t numBytes);
