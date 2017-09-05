#ifndef __RADIO_H_
#define __RADIO_H_

#include <stdint.h>
#include <stdbool.h>

#ifndef RADIO_BLE
#include "rail.h"

#define RADIO_RAIL_TX_MODE_OFF          0
#define RADIO_RAIL_TX_MODE_TONE         1
#define RADIO_RAIL_TX_MODE_STREAM       2

void     RADIO_init            (uint32_t xoTune);
int      RADIO_receivePacket   (uint8_t buffer[], uint16_t bufferSize, uint16_t timeout,
                                RAIL_RxPacketInfo_t **rxPacket);
int      RADIO_sendPacket      (uint8_t buffer[], uint16_t bufferSize, uint16_t txPayloadSize, uint16_t timeout,
                                RAIL_TxPacketInfo_t **txPacketInfo);
#endif

uint32_t RADIO_getCTune        (void);
void     RADIO_setCTune        (uint32_t);
uint8_t  RADIO_txCWEnable      (bool enable);

uint32_t RADIO_getReg          (uint32_t addr);
void     RADIO_setReg          (uint32_t addr, uint32_t value);

uint32_t RADIO_changeConfig    (int newConfig);
int      RADIO_getConfig       (void);

void     RADIO_setRxMode       (bool enable);
bool     RADIO_getRxMode       (void);

void     RADIO_setTxMode       (uint8_t mode);
uint8_t  RADIO_getTxMode       (void);

uint32_t RADIO_setChannel      (uint16_t newChannel);
uint16_t RADIO_getChannel      (void);

void     RADIO_setPower        (int power);
int      RADIO_getPower        (void);

#endif
