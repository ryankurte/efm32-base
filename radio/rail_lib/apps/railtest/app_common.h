/***************************************************************************//**
 * @file
 * @brief This header file defines variables to be shared between the main
 *   test application and customer specific sections.
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

#ifndef __APPS_COMMON_H__
#define __APPS_COMMON_H__

#include "em_gpio.h" // For ButtonArray definition
#include "circular_queue.h"
#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

#include "rail_types.h"
#include "rail_ble.h"
#include "rail_zwave.h"

#include "pa_conversions_efr32.h"

// Set this token to 0 if an external radio config
// (i.e. rail_config.c/h) is not available to the application.
#ifndef RADIO_CONFIG_EXTERNAL_SUPPORT_ENABLED
#define RADIO_CONFIG_EXTERNAL_SUPPORT_ENABLED (1U)
#endif
#if RADIO_CONFIG_EXTERNAL_SUPPORT_ENABLED
#include "rail_config.h"
#endif

/******************************************************************************
 * Macros
 *****************************************************************************/
#define RAILTEST_PRINTF(...) \
  do {                       \
    if (printingEnabled) {   \
      printf(__VA_ARGS__);   \
    }                        \
  } while (false)

/******************************************************************************
 * Constants
 *****************************************************************************/
#define COUNTOF(a) (sizeof(a) / sizeof(a[0]))
#define TX_CONTINUOUS_COUNT (0xFFFFFFFF)

/******************************************************************************
 * Radio Configurations
 *****************************************************************************/
typedef enum RadioConfigType{
  /* Radio Configurations external to RAIL library */
  RADIO_CONFIG_EXTERNAL,            /* see radio config in rail_config.c/h */
  RADIO_CONFIG_EXTERNAL_RMR,        /* see RAM modem reconfiguration feature */
  /* Radio Configurations internal to RAIL library */
  RADIO_CONFIG_INTERNAL_BLE_1MBPS,                /* Mbps */
  RADIO_CONFIG_INTERNAL_BLE_2MBPS,                /* Mbps */
  RADIO_CONFIG_INTERNAL_BLE_CODED_125KBPS,        /* Kbps */
  RADIO_CONFIG_INTERNAL_BLE_CODED_500KBPS,        /* Kbps */
  RADIO_CONFIG_INTERNAL_IEEE802154_2P4GHZ,
  RADIO_CONFIG_INTERNAL_IEEE802154_2P4GHZ_ANTDIV,
  RADIO_CONFIG_INTERNAL_IEEE802154_2P4GHZ_COEX,
  RADIO_CONFIG_INTERNAL_IEEE802154_2P4GHZ_ANTDIV_COEX,
  RADIO_CONFIG_INTERNAL_IEEE802154_GB868_915MHZ,
  RADIO_CONFIG_INTERNAL_IEEE802154_GB868_863MHZ,
  RADIO_CONFIG_INTERNAL_ZWAVE_EU,
  RADIO_CONFIG_INTERNAL_ZWAVE_US,
  RADIO_CONFIG_INTERNAL_ZWAVE_ANZ,
  RADIO_CONFIG_INTERNAL_ZWAVE_HK,
  RADIO_CONFIG_INTERNAL_ZWAVE_MY,
  RADIO_CONFIG_INTERNAL_ZWAVE_IN,
  RADIO_CONFIG_INTERNAL_ZWAVE_JP,
  RADIO_CONFIG_INTERNAL_ZWAVE_RU,
  RADIO_CONFIG_INTERNAL_ZWAVE_IL,
  RADIO_CONFIG_INTERNAL_ZWAVE_KR,
  RADIO_CONFIG_INTERNAL_ZWAVE_CN,
} RadioConfigType_t;

#define RAIL_EVENT_STRINGS                              \
  {                                                     \
    "RSSI_AVERAGE_DONE",                                \
    "RX_ACK_TIMEOUT",                                   \
    "RX_FIFO_ALMOST_FULL",                              \
    "RX_PACKET_RECEIVED",                               \
    "RX_PREAMBLE_LOST",                                 \
    "RX_PREAMBLE_DETECT",                               \
    "RX_SYNC1_DETECT",                                  \
    "RX_SYNC2_DETECT",                                  \
    "RX_FRAME_ERROR",                                   \
    "RX_FIFO_OVERFLOW",                                 \
    "RX_ADDRESS_FILTERED",                              \
    "RX_TIMEOUT",                                       \
    "RX_SCHEDULED_RX_END",                              \
    "RX_PACKET_ABORTED",                                \
    "RX_FILTER_PASSED",                                 \
    "RX_TIMING_LOST",                                   \
    "RX_TIMING_DETECT",                                 \
    "RX_CHANNEL_HOPPING_COMPLETE/RX_DUTY_CYCLE_RX_END", \
    "IEEE802154_DATA_REQUEST_COMMAND/ZWAVE_BEAM",       \
    "TX_FIFO_ALMOST_EMPTY",                             \
    "TX_PACKET_SENT",                                   \
    "TXACK_PACKET_SENT",                                \
    "TX_ABORTED",                                       \
    "TXACK_ABORTED",                                    \
    "TX_BLOCKED",                                       \
    "TXACK_BLOCKED",                                    \
    "TX_UNDERFLOW",                                     \
    "TXACK_UNDERFLOW",                                  \
    "TX_CHANNEL_CLEAR",                                 \
    "TX_CHANNEL_BUSY",                                  \
    "TX_CCA_RETRY",                                     \
    "TX_START_CCA",                                     \
    "CONFIG_UNSCHEDULED",                               \
    "CONFIG_SCHEDULED",                                 \
    "SCHEDULED_STATUS",                                 \
    "CAL_NEEDED",                                       \
  }

// Default radio configuration used on application boot
#ifndef RADIO_CONFIG_DEFAULT
#define RADIO_CONFIG_DEFAULT (RADIO_CONFIG_EXTERNAL)
#endif

// Since channel hopping is pretty space intensive, put some limitations on it
// 125 32 bit words per channel should be plenty
#define CHANNEL_HOPPING_BUFFER_SIZE 500

/******************************************************************************
 * Variable Export
 *****************************************************************************/
#define PER_PORT (gpioPortC)
#define PER_PIN  (7)

typedef struct PhySwitchToRx{
  bool enable;
  bool disableWhitening;
  RAIL_BLE_Phy_t phy;
  uint16_t physicalChannel;
  uint16_t logicalChannel;
  uint32_t timeDelta;
  uint32_t crcInit;
  uint32_t accessAddress;
} PhySwitchToRx_t;

typedef enum RailTxType {
  TX_TYPE_NORMAL,
  TX_TYPE_CSMA,
  TX_TYPE_LBT
} RailTxType_t;

typedef struct ButtonArray {
  GPIO_Port_TypeDef   port;
  unsigned int        pin;
} ButtonArray_t;

typedef enum RailAppEventType {
  RAIL_EVENT,
  RX_PACKET,
  BEAM_PACKET,
  MULTITIMER,
  AVERAGE_RSSI,
} RailAppEventType_t;

typedef struct ZWaveBeamData {
  /**
   * The channel index in the currently configured channel hopping scheme
   * on which the beam was received.
   */
  uint8_t channelIndex;
  /**
   * The node ID contained in the received beam frame.
   */
  RAIL_ZWAVE_NodeId_t nodeId;
} ZWaveBeamData_t;

typedef struct RxPacketData {
  /**
   * A structure containing the extra information associated with this received
   * packet.
   */
  RAIL_RxPacketDetails_t appendedInfo;
  /**
   * The packet's status
   */
  RAIL_RxPacketStatus_t packetStatus;
  /**
   * The number of bytes that are in the dataPtr array.
   */
  uint16_t dataLength;
  /**
   * The railHandle on which this packet was received.
   */
  RAIL_Handle_t railHandle;
  /**
   * A pointer to a buffer that holds receive packet data bytes.
   */
  uint8_t *dataPtr;
} RxPacketData_t;

typedef struct RailEvent {
  RAIL_Events_t events;
  uint32_t timestamp;
  RAIL_Handle_t handle;
  uint32_t parameter; /**< This field is open to interpretation based on the event type.
                         It may hold information related to the event e.g. status. */
} RailEvent_t;

typedef struct Multitimer {
  RAIL_Time_t currentTime;
  RAIL_Time_t expirationTime;
  uint32_t index;
} Multitimer_t;

typedef struct AverageRssi {
  int16_t rssi;
} AverageRssi_t;

typedef struct RailAppEvent {
  RailAppEventType_t type;
  union {
    RxPacketData_t rxPacket;
    ZWaveBeamData_t beamPacket;
    RailEvent_t railEvent;
    Multitimer_t multitimer;
    AverageRssi_t rssi;
  };
} RailAppEvent_t;

typedef struct Stats{
  uint32_t samples;
  int32_t min;
  int32_t max;
  float mean;
  float varianceTimesSamples;
} Stats_t;

typedef struct Counters{
  // Counts all successful user transmits
  // "user" in this and following variable names refers to
  // a transmit that a user initiated, i.e. not an ack
  uint32_t userTx;
  // Counts all successful ack transmits
  uint32_t ackTx;
  uint32_t userTxAborted;
  uint32_t ackTxAborted;
  uint32_t userTxBlocked;
  uint32_t ackTxBlocked;
  uint32_t userTxUnderflow;
  uint32_t ackTxUnderflow;
  uint32_t ackTxFpSet;
  uint32_t ackTxFpFail;
  uint32_t ackTxFpAddrFail;

  // Channel busy doesn't differentiate
  // between ack/user packets
  uint32_t txChannelBusy;

  uint32_t receive;
  uint32_t syncDetect;
  uint32_t preambleLost;
  uint32_t preambleDetect;
  uint32_t frameError;
  uint32_t rxOfEvent;
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
  uint32_t radioConfigChanged;
  Stats_t rssi;
} Counters_t;

extern bool printingEnabled;
extern PhySwitchToRx_t phySwitchToRx;
extern Counters_t counters;
extern int currentConfig;
extern bool receiveModeEnabled;
extern RAIL_RadioState_t rxSuccessTransition;
extern bool transmitting;
extern bool txParameterChanged;
extern uint16_t channel;
extern uint8_t configIndex;
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
extern uint32_t rxOverflowDelay;
extern uint32_t dataReqLatencyUs;
extern bool afterRxCancelAck;
extern bool afterRxUseTxBufferForAck;
extern bool newTxError;
extern uint32_t failPackets;
extern RAIL_Events_t enablePrintEvents;
extern bool printRxErrorPackets;
extern RAIL_VerifyConfig_t configVerify;
extern uint32_t internalTransmitCounter;
extern const char buildDateTime[];
extern bool ieee802154EnhAckEnabled;
extern uint8_t ieee802154PhrLen; // 15.4 PHY Header Length (1 or 2 bytes)

#ifdef RAIL_IC_SIM_BUILD
#define PERIPHERAL_ENABLE (0x00)
#define ASYNC_RESPONSE (0x00)
#else
#define PERIPHERAL_ENABLE (0x01)
#define ASYNC_RESPONSE (0x02)
#endif
extern uint8_t logLevel;
extern uint8_t txData[APP_MAX_PACKET_LENGTH];
extern uint16_t txDataLen;

extern uint8_t ackData[RAIL_AUTOACK_MAX_LENGTH];
extern uint8_t ackDataLen;

extern RailTxType_t txType;
extern RAIL_LbtConfig_t *lbtConfig;
extern RAIL_CsmaConfig_t *csmaConfig;

// Variables used in script_ci.c
extern char *script;
extern uint16_t scriptMarker;
extern uint16_t scriptLength;
extern uint32_t suspension;
extern uint32_t suspensionStartTime;

// Structure that holds txOptions
extern RAIL_TxOptions_t txOptions;

// Structure that holds (default) rxOptions
extern RAIL_RxOptions_t rxOptions;

// Data Management
extern Queue_t railAppEventQueue;
extern volatile uint32_t eventsMissed;
extern RAIL_DataConfig_t railDataConfig;

// Fifo mode Test bits
extern bool rxFifoManual;
extern bool txFifoManual;

// RAIL instance handle
extern RAIL_Handle_t railHandle;

// Indicator of whether or not to print tx acks as they happens
extern bool printTxAck;

// Indicator of last power level requested for use
extern uint8_t lastSetTxPowerLevel;

// For 2.4GHz protocols, the preferred PA to use
extern RAIL_TxPowerMode_t default2p4Pa;

// PA Settings to use (mode will be default2p4Pa)
extern RAIL_TxPowerConfig_t txPowerConfig;

// Strings representing the possible PA selections
extern const char* paStrings[];

// LQI offset variable
extern int16_t lqiOffset;

// Verify config in RAILCb_RadioConfigChanged
extern bool verifyConfigEnabled;

// Antenna Diversity Configuration
extern RAIL_AntennaConfig_t halAntennaConfig;

// Variable containing current RSSI
extern float averageRssi;

// Channel Hopping configuration structures
extern uint32_t* channelHoppingBuffer;

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
  TX_CANCEL = 10,     /**< Cancel a single packet transmit to force an error event */
  RF_SENSE = 11,      /**< Sense RF energy to wake the radio */
  PER = 12,           /**< Packet Error Rate test mode */
  BER = 13,           /**< Bit Error Rate test mode */
  RX_SCHEDULED = 14,  /**< Enable receive at a time scheduled in the future */
} AppMode_t;

/**
 * @enum RailTxType
 * @brief Enumeration of the types of tx available in RAIL
 *
 * These are used to decide which type of tx to do, based on
 * what's been configured in railtest. Scheduled is not included
 * as railtest handles it somewhat separately.
 */

void RAILCb_TimerExpired(RAIL_Handle_t railHandle);
AppMode_t previousAppMode(void);
AppMode_t currentAppMode(void);
void enableAppMode(AppMode_t appMode, bool enable, char *command);
bool enableAppModeSync(AppMode_t appMode, bool enable, char *command);
void setNextAppMode(AppMode_t appMode, char *command);
void changeAppModeIfPending();
const char *appModeNames(AppMode_t appMode);
bool inAppMode(AppMode_t appMode, char *command);
bool inRadioState(RAIL_RadioState_t state, char *command);
bool parseTimeModeFromString(char *str, RAIL_TimeMode_t *mode);
const char *configuredRxAntenna(RAIL_RxOptions_t rxOptions);

void updateStats(int32_t newValue, Stats_t *stats);
void rfSensedCheck(void);
void updateDisplay(void);

void changeChannel(uint32_t i);
void pendPacketTx(void);
void pendFinishTxSequence(void);
void pendFinishTxAckSequence(void);
void radioTransmit(uint32_t iterations, char *command);
void configureTxAfterRx(uint32_t delay, bool enable, char *command);
void scheduleNextTx(void);
void printPacket(char *cmdName,
                 uint8_t *data,
                 uint16_t dataLength,
                 RxPacketData_t *packetInfo);

void appHalInit(void);
void deinitButtons(void);
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
void enqueueEvents(RAIL_Events_t events);
void rxFifoPrep(void);
void printRailEvents(RailEvent_t *railEvent);
void printRailAppEvents(void);
RAIL_Status_t chooseTxType(void);
const char *getRfStateName(RAIL_RadioState_t state);
const char *getStatusMessage(RAIL_Status_t status);
void runFlashScript(void);
uint16_t applyDefaultRadioConfig(RAIL_Handle_t railHandle,
                                 RadioConfigType_t configSelection);

void RAILCb_TxPacketSent(RAIL_Handle_t railHandle, bool isAck);
void RAILCb_RxPacketAborted(RAIL_Handle_t railHandle);
void RAILCb_RxPacketReceived(RAIL_Handle_t railHandle);
void RAILCb_TxFifoAlmostEmpty(RAIL_Handle_t railHandle);
void RAILCb_RxFifoAlmostFull(RAIL_Handle_t railHandle);
void RAILCb_AssertFailed(RAIL_Handle_t railHandle, uint32_t errorCode);
void RAILCb_RadioConfigChanged(RAIL_Handle_t railHandle,
                               const RAIL_ChannelConfigEntry_t * entry);
void RAILCb_IEEE802154_DataRequestCommand(RAIL_Handle_t railHandle);
void RAILCb_ZWAVE_BeamFrame(RAIL_Handle_t railHandle);

#endif // __APPS_COMMON_H__
