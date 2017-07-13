/***************************************************************************//**
 * @file app_common.h
 * @brief This header file defines variables to be shared between the main
 * test application and customer specific sections.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __APPS_COMMON_H__
#define __APPS_COMMON_H__

#include "em_gpio.h" // For ButtonArray definition
#include "circular_queue.h"
#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

#include "rail_types.h"

/******************************************************************************
 * Constants
 *****************************************************************************/
#define COUNTOF(a) (sizeof(a) / sizeof(a[0]))
#define TX_CONTINUOUS_COUNT (0xFFFFFFFF)

/******************************************************************************
 * Variable Export
 *****************************************************************************/
#define PER_PORT (gpioPortC)
#define PER_PIN  (7)

typedef struct ButtonArray{
  GPIO_Port_TypeDef   port;
  unsigned int        pin;
} ButtonArray_t;

typedef struct Stats{
  uint32_t samples;
  int32_t min;
  int32_t max;
  float mean;
  float varianceTimesSamples;
} Stats_t;

typedef struct Counters{
  uint32_t transmit;
  uint32_t receive;
  uint32_t syncDetect;
  uint32_t preambleDetect;
  uint32_t frameError;
  uint32_t rxOfEvent;
  uint32_t rxUfEvent;
  uint32_t txOfEvent;
  uint32_t txUfEvent;
  uint32_t txAbort;
  uint32_t txChannelBusy;
  uint32_t addrFilterEvent;
  uint32_t rxFail;
  uint32_t calibrations;
  uint32_t noRxBuffer;
  uint32_t rfSensedEvent;
  uint32_t perTriggers;
  uint32_t ackTimeout;
  uint32_t lbtSuccess;
  uint32_t lbtRetry;
  uint32_t lbtStartCca;
  uint32_t txFifoAlmostEmpty;
  uint32_t rxFifoAlmostFull;
  uint32_t timingLost;
  uint32_t timingDetect;
  Stats_t rssi;
} Counters_t;

extern Counters_t counters;
extern int currentConfig;
extern bool receiveModeEnabled;
extern bool transmitting;
extern bool txParameterChanged;
extern uint8_t channel;
extern uint32_t continuousTransferPeriod;
extern int32_t txCount;
extern uint32_t txAfterRxDelay;
extern int32_t txCancelDelay;
extern RAIL_ChannelConfigEntry_t channels[];
extern const RAIL_ChannelConfig_t channelConfig;
extern bool redrawDisplay;
extern bool skipCalibrations;
extern bool schRxStopOnRxEvent;
extern volatile bool serEvent;
extern uint32_t perCount;
extern uint32_t perDelay;
extern bool afterRxCancelAck;
extern bool afterRxUseTxBufferForAck;
extern bool newTxError;
extern uint32_t failPackets;
extern uint32_t enablePrintCallbacks;

extern uint32_t internalTransmitCounter;

#define PERIPHERAL_ENABLE (0x01)
#define ASYNC_RESPONSE (0x02)
extern uint8_t logLevel;
extern uint8_t txData[APP_MAX_PACKET_LENGTH];
extern RAIL_TxData_t transmitPayload;

extern uint8_t ackData[RAIL_AUTOACK_MAX_LENGTH];
extern RAIL_AutoAckData_t ackPayload;

extern RAIL_PreTxOp_t txPreTxOp;
extern void* txPreTxOpArgs;

// Structure that holds txOptions
extern RAIL_TxOptions_t txOptions;
// If this pointer is not NULL, call RAIL_TxStartWithOptions
extern RAIL_TxOptions_t *txOptionsPtr;

// Data Management
extern Queue_t  rxPacketQueue;
extern RAIL_DataConfig_t railDataConfig;

// Fifo mode Test bits
extern bool rxFifoManual;
extern bool txFifoManual;

extern uint32_t railRxConfig;
/**
 * @enum AppMode
 * @brief Enumeration of RailTest transmit states.
 */
typedef enum AppMode{
  NONE = 0,           /**< RAILtest is not doing anything special */
  TX_STREAM = 1,      /**< Send a stream of pseudo-random bits */
  TX_TONE = 2,        /**< Send a tone at the carrier frequency */
  TX_CONTINUOUS = 3,  /**< Send an unending stream of packets*/
  DIRECT = 4,         /**< Send data to and from a GPIO, without any packet handling */
  TX_N_PACKETS = 5,   /**< Send a specific number of packets */
  TX_SCHEDULED = 6,   /**< Send one packet scheduled in the future */
  SCHTX_AFTER_RX = 7, /**< Schedule a TX for a fixed delay after receiving a packet */
  RX_OVERFLOW = 8,    /**< Cause overflow on receive */
  TX_UNDERFLOW = 9,   /**< Cause underflows on the next TX sequence */
  TX_CANCEL = 10,     /**< Cancel a single packet transmit to force an error callback */
  RF_SENSE = 11,      /**< Sense RF energy to wake the radio */
  PER = 12,           /**< Packet Error Rate test mode */
  BER = 13,           /**< Bit Error Rate test mode */
  RX_SCHEDULED = 14,  /**< Enable receive at a time scheduled in the future */
} AppMode_t;

typedef struct CallbackData{
  uint32_t        callbackId;
  uint32_t        timestamp;
} CallbackData_t;

/**
 * @enum RailtestCallbacks
 * @brief Enumeration of callbacks used in railtest.
 *
 * These values serve as an index for the function name
 * strings in RailCbNames. Therefore, the number of enum
 * values here should match the number of elements in that
 * array.
 */
typedef enum RailtestCallbacks{
  RAILCB_RX_RADIO_STATUS_EXT,
  RAILCB_TX_RADIO_STATUS,
  RAILCB_TX_PACKET_SENT,
  RAILCB_RX_PACKET_RECEIVED,
  RAILCB_TX_FIFO_ALMOST_EMPTY,
  RAILCB_RX_FIFO_ALMOST_FULL,
  RAILCB_RF_READY,
  RAILCB_CAL_NEEDED,
  RAILCB_RADIO_STATE_CHANGED,
  RAILCB_TIMER_EXPIRED,
  RAILCB_RX_ACK_TIMEOUT,
  RAILCB_IEEE802154_DATA_REQUEST_COMMAND,
  RAILCB_RSSI_AVERAGE_DONE,
  RAILCB_ASSERT_FAILED
} RailtestCallbacks_t;

AppMode_t previousAppMode(void);
AppMode_t currentAppMode(void);
void enableAppMode(AppMode_t appMode, bool enable, char *command);
bool enableAppModeSync(AppMode_t appMode, bool enable, char *command);
void setNextAppMode(AppMode_t appMode, char *command);
void changeAppModeIfPending();
char *appModeNames(AppMode_t appMode);
bool inAppMode(AppMode_t appMode, char *command);
bool inRadioState(RAIL_RadioState_t state, char *command);

void updateStats(int32_t newValue, Stats_t *stats);
void rfSensedCheck(void);
void updateDisplay(void);

void changeChannel(uint32_t i);
void pendPacketTx(void);
void pendFinishTxSequence(void);
void setNextPacketTime(uint32_t time, bool isAbs);
void radioTransmit(uint32_t iterations, char *command);
void configureTxAfterRx(uint32_t delay, bool enable, char *command);
void scheduleNextTx(void);
void changeRadioConfig(int newConfig);
void printPacket(char *cmdName,
                 uint8_t *data,
                 uint16_t dataLength,
                 RAIL_RxPacketInfo_t *packetInfo);

void appHalInit(void);
void initLeds(void);
void initButtons(void);
void initGraphics(void);
void LedSet(int led);
void LedToggle(int led);
void PeripheralDisable(void);
void LedsDisable(void);
void disableGraphics(void);
void PeripheralEnable(void);
void enableGraphics(void);
void usDelay(uint32_t microseconds);
void serialWaitForTxIdle(void);
void enqueueCallback(uint32_t callbackId);
#endif // __APPS_COMMON_H__
