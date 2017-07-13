/***************************************************************************//**
 * @file app_ci.h
 * @brief This is the header file for common RAIL test application commands.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __APP_CI_H__
#define __APP_CI_H__

/******************************************************************************
 * Application commands.
 *****************************************************************************/
#ifdef INTERNAL_COMMAND_HEADER
  #include INTERNAL_COMMAND_HEADER
#endif
#ifndef INTERNAL_COMMANDS
  #define INTERNAL_COMMANDS COMMAND_ENTRY(NULL, NULL, NULL, NULL)
#endif

// Base set of Command Interface commands
// <command> <args> <help text>
// u=uint8, v=uint16, w=uint32, s=int32, b=string, ?=Anything, *=0 or more of previous

#define APP_CI_COMMANDS                                                                                                                                                                                        \
  COMMAND_SEPARATOR("Application Configuration"),                                                                                                                                                              \
  COMMAND_ENTRY("listConfigs", "", listConfigs, "List all available configs and their indices"),                                                                                                               \
  COMMAND_ENTRY("setConfig", "v", setConfig, "[index] Set the radio config to the one at index in the config list."),                                                                                          \
  COMMAND_ENTRY("getConfig", "", getConfig, "Get the index of the current radio config"),                                                                                                                      \
  COMMAND_ENTRY("setRxConfig", "bw", setRxConfig, "[[set|and|or] flags] Modify RAIL_RxConfig"),                                                                                                                \
  COMMAND_ENTRY("printDataRates", "", printDataRates, "Print the data rates of the current PHY"),                                                                                                              \
  COMMAND_ENTRY("resetCounters", "", resetCounters, "Resets the Tx and Rx counters"),                                                                                                                          \
  COMMAND_ENTRY("setPeripheralEnable", "u", setPeripheralEnable, "[enable] Enable(1) or Disable(0) LEDs and LCD peripherals"),                                                                                 \
  COMMAND_ENTRY("setNotifications", "u", setNotifications, "[enable] Enable(1) or Disable(0) status prints that happen asynchronously (rxPacket, txEnd, txError)"),                                            \
  COMMAND_ENTRY("getLogLevels", "", getLogLevels, "Get whether notifications are set or peripherals are enabled"),                                                                                             \
  COMMAND_ENTRY("getVersion", "", getVersion, "Get version information."),                                                                                                                                     \
  COMMAND_ENTRY("getVersionVerbose", "", getVersionVerbose, "Get verbose version information."),                                                                                                               \
  COMMAND_ENTRY("setPtiProtocol", "u", setPtiProtocol, "Get version information."),                                                                                                                            \
  COMMAND_SEPARATOR("Receive and Transmit"),                                                                                                                                                                   \
  COMMAND_ENTRY("rxConfig", "w", rxConfig, "[appendedInfo] Enable(1) or Disable(0) appended info"),                                                                                                            \
  COMMAND_ENTRY("rx", "w", rx, "[enable] Enable(1) or Disable(0) receive mode"),                                                                                                                               \
  COMMAND_ENTRY("rxAt", "wbwbu*", rxAt, "[start mode end mode rxTransEndSched hardEnd] Configure scheduled receive."),                                                                                         \
  COMMAND_ENTRY("setRxOptions", "w", setRxOptions, "[rxOptionsBitField] Configure receive options, based on RAIL_RX_OPTION defines"),                                                                          \
  COMMAND_ENTRY("tx", "w", tx, "[n] Transmit n packets. If n is 0 transmit infinitely"),                                                                                                                       \
  COMMAND_ENTRY("txWithOptions", "w", txWithOptions, "[n] Transmit n packets with tx options. If n is 0 transmit infinitely"),                                                                                 \
  COMMAND_ENTRY("txAt", "wb*", txAtTime, "[time mode] Transmit a packet at the time and mode specified"),                                                                                                      \
  COMMAND_ENTRY("txAfterRx", "w", txAfterRx, "[time] Schedule a TX for a delay in us after each receive. 0 to disable"),                                                                                       \
  COMMAND_ENTRY("configTxOptions", "u", configTxOptions, "[txOptionsBitfield] Configure RAIL_TxOptions_t (LSB = waitForAck)"),                                                                                 \
  COMMAND_ENTRY("abortScheduledTxDuringRx", "u", abortScheduledTxDuringRx, "[bool] If a scheduled Tx occurs during Rx, postpone (0) or abort (1) it"),                                                         \
  COMMAND_ENTRY("setFixedLength", "v", setFixedLength, "[fixedLength] Configure fixed length"),                                                                                                                \
  COMMAND_ENTRY("setchannel", "u", setChannel, "[channel] Set the current radio channel"),                                                                                                                     \
  COMMAND_ENTRY("getchannel", "", getChannel, "Get the current radio channel"),                                                                                                                                \
  COMMAND_ENTRY("setPower", "s", setPower, "[power] Set the current transmit power in deci dBm"),                                                                                                              \
  COMMAND_ENTRY("getPower", "", getPower, "Get the current transmit power in deci dBm"),                                                                                                                       \
  COMMAND_ENTRY("getRssi", "w*", getRssi, "[averageTimeUs] Get RSSI in dBm if the receiver is turned on. Optionally specify in microseconds how long to average RSSI."),                                       \
  COMMAND_ENTRY("sweepPower", "ssw", sweepPower, "[lowPower] [hiPower] [period] Sweep power in square wave fashion. Specify power in deci dBm, period in microseconds."),                                      \
  COMMAND_ENTRY("startAvgRssi", "wu*", startAvgRssi, "[averageTimeUs] [channel] Start AGC RSSI averaging"),                                                                                                    \
  COMMAND_ENTRY("getAvgRssi", "", getAvgRssi, "Get AGC RSSI averaging result."),                                                                                                                               \
  COMMAND_ENTRY("setTxTone", "w", setTxTone, "[enable] Enable(1) or Disable(0) a tone from the radio"),                                                                                                        \
  COMMAND_ENTRY("setTxStream", "w", setTxStream, "[enable] Enable(1) or Disable(0) a PN9 stream from the radio"),                                                                                              \
  COMMAND_ENTRY("status", "", getStatus, "Print the current status counters"),                                                                                                                                 \
  COMMAND_ENTRY("fifoStatus", "", fifoStatus, "Print the current fifo related counters"),                                                                                                                      \
  COMMAND_ENTRY("setTxDelay", "w", setTxDelay, "[delay] Set the inter-packet delay in milliseconds for repeated Tx"),                                                                                          \
  COMMAND_ENTRY("getTxDelay", "", getTxDelay, "Get the inter-packet delay in milliseconds for repeated Tx"),                                                                                                   \
  COMMAND_ENTRY("setTxPayload", "vu*", setTxPayload, "[offset byte0 byte1 ...] Set the packet bytes to be sent"),                                                                                              \
  COMMAND_ENTRY("setTxLength", "v", setTxLength, "[length] Set the number of bytes to transmit for fixed length packets"),                                                                                     \
  COMMAND_ENTRY("printTxPacket", "", printTxPacket, "Print the current Tx data and length"),                                                                                                                   \
  COMMAND_ENTRY("peek", "v*", peekRx, "[number of bytes] [offset] Peek at the start of receive buffer."),                                                                                                      \
  COMMAND_ENTRY("getTime", "", getTime, "Get the current time from the RAIL timebase in microseconds"),                                                                                                        \
  COMMAND_ENTRY("setTime", "w", setTime, "Set the current time in the RAIL timebase in microseconds"),                                                                                                         \
  COMMAND_ENTRY("dataConfig", "bb", dataConfig, "[txMethod rxMethod] Choose between 'pkt' and 'fifo' data methods for RAIL Tx and Rx"),                                                                        \
  COMMAND_ENTRY("setTxFifoThreshold", "v", setTxFifoThreshold, "[txFifoThreshold] Set the Tx Fifo Almost Emtpy threshold"),                                                                                    \
  COMMAND_ENTRY("setRxFifoThreshold", "v", setRxFifoThreshold, "[rxFifoThreshold] Set the Rx Fifo Almost Emtpy threshold"),                                                                                    \
  COMMAND_ENTRY("fifoModeTestOptions", "bb", fifoModeTestOptions, "[txFifoManual rxFifoManual] Manual control over RAILTEST fifo actions"),                                                                    \
  COMMAND_ENTRY("rxFifoManualRead", "bv", rxFifoManualRead, "[appendedInfo bytesToRead] Read bytes out of receive fifo and print"),                                                                            \
  COMMAND_ENTRY("txFifoManualLoad", "", txFifoManualLoad, "Will attempt to load data into the fifo if there is space"),                                                                                        \
  COMMAND_ENTRY("fifoReset", "bb", fifoReset, "[tx rx] Reset the transmit or receive fifo"),                                                                                                                   \
  COMMAND_ENTRY("abortRxPacket", "w", abortRxPacket, "[abortOffset] Delay after sync word before idling radio."),                                                                                              \
  COMMAND_SEPARATOR("Energy Modes and RF Sense"),                                                                                                                                                              \
  COMMAND_ENTRY("sleep", "bw*", sleep, "[EM# [RfSenseUs RfBand]] Sleep in EM# with RFSenseUs on RfBand (0=none,1=2.4GHz,2=SubGHz,3=both) (and UART input)"),                                                   \
  COMMAND_ENTRY("rfsense", "ww", rfSense, "[RfSenseUs RfBand] Start RfSensing with RSenseUs on RfBand"),                                                                                                       \
  COMMAND_SEPARATOR("Address Filtering"),                                                                                                                                                                      \
  COMMAND_ENTRY("configAddressFilter", "wu*", setAddressFilterConfig, "[matchTable offset0 size0 offset1 size1] Configure the addresss filter."),                                                              \
  COMMAND_ENTRY("addressFilterByFrame", "u", addressFilterByFrame, "[validFrames] Set interaction of address filtering and frame type length decoding. Must be called after 'configAddressFilter'"),           \
  COMMAND_ENTRY("setAddressFiltering", "u", setAddressFilter, "[enable] Enable(1) or Disable(0) address filtering."),                                                                                          \
  COMMAND_ENTRY("getAddressFiltering", "", getAddressFilter, "Print the current state of address filtering."),                                                                                                 \
  COMMAND_ENTRY("printAddresses", "", printAddresses, "Print the current address filtering addresses."),                                                                                                       \
  COMMAND_ENTRY("setAddress", "uuu*", setAddress, "[field index value...] Set the address value at (field, index) to value."),                                                                                 \
  COMMAND_ENTRY("setAddressEnable", "uuu", enableAddress, "[field index enable] Enable address filtering for the given address."),                                                                             \
  COMMAND_SEPARATOR("Error Rate Testing"),                                                                                                                                                                     \
  COMMAND_ENTRY("perRx", "ww", startPerMode, "[packets delayUs] Start a Packet Error Rate test. 'perRx 0 0' will disable ongoing test."),                                                                      \
  COMMAND_ENTRY("perStatus", "", getPerStats, "Output the results of the PER test. Also see 'status' command"),                                                                                                \
  COMMAND_ENTRY("setBerConfig", "w", berConfigSet, "[number bytes] Set number of bytes to receive in BER mode; 536870911 = max number of bytes to test; 0 = set max number of bytes to test"),                 \
  COMMAND_ENTRY("berRx", "w", berRx, "[enable] Enable(1) or Disable(0) BER receive mode"),                                                                                                                     \
  COMMAND_ENTRY("berStatus", "", berStatusGet, "Get status of last BER test or of current running test; status information is reset for commands setBerConfig and berRx enable"),                              \
  COMMAND_SEPARATOR("Listen Before Talk (LBT)"),                                                                                                                                                               \
  COMMAND_ENTRY("setLbtMode", "b*", setLbtMode, "[modeStr] Set LBT mode off, csma, lbt"),                                                                                                                      \
  COMMAND_ENTRY("getLbtParams", "", getLbtParams, "Get the current LBT parameters"),                                                                                                                           \
  COMMAND_ENTRY("setLbtParams", "uuusvvw", setLbtParams, "[minBo maxBo tries thresh backoff duration timeout] Set LBT parameters"),                                                                            \
  COMMAND_SEPARATOR("802.15.4 Mode"),                                                                                                                                                                          \
  COMMAND_ENTRY("enable802154", "bvvv", ieee802154Enable, "[defaultState idleTime turnaroundTime ackTimeout] Enable 802.15.4 mode"),                                                                           \
  COMMAND_ENTRY("config2p4GHz802154", "", config2p4Ghz802154, "Configure the radio for 2.4 GHz 802.15.4. This should be called in addition to 'enable802154'."),                                               \
  COMMAND_ENTRY("acceptFrames", "uuuu", ieee802154AcceptFrames, "[command ack data beacon] Enable(1) or Disable(0) 802.15.4 frame acceptance. Default settings for 802.15.4 are 1 0 1 1."),                    \
  COMMAND_ENTRY("setPromiscuousMode", "u", ieee802154SetPromiscuousMode, "[enable] Enable(1) or Disable(0) promiscuous mode"),                                                                                 \
  COMMAND_ENTRY("setPanCoordinator", "u", ieee802154SetPanCoordinator, "[enable] Enable(1) or Disable(0) the node acting as a PAN coordinator"),                                                               \
  COMMAND_ENTRY("setPanId802154", "v", ieee802154SetPanId, "[panId] Set the PAN Id for the given index"),                                                                                                      \
  COMMAND_ENTRY("setShortAddr802154", "v", ieee802154SetShortAddress, "[shortAddr] Set the short address for the given index"),                                                                                \
  COMMAND_ENTRY("setLongAddr802154", "uuuuuuuu", ieee802154SetLongAddress, "[longAddr_0 ... longAddr_7] Set the long address for the given index"),                                                            \
  COMMAND_SEPARATOR("BLE Mode"),                                                                                                                                                                               \
  COMMAND_ENTRY("setBleMode", "u", bleEnable, "[enable] Set BLE mode to enabled or disabled"),                                                                                                                 \
  COMMAND_ENTRY("getBleMode", "", bleStatus, "Get the current BLE mode"),                                                                                                                                      \
  COMMAND_ENTRY("setBleChannelParams", "uw*", bleSetChannelParams, "[logicalChannel accessAddr crcInit disableWhiten] Configure channel parameters related to BLE"),                                           \
  COMMAND_ENTRY("setBleAdvertising", "u", bleAdvertisingConfig, "[advChannel] Configure for BLE advertising on channel 37, 38, or 39"),                                                                        \
  COMMAND_ENTRY("setBle1Mbps", "u*", bleSet1MbpsPhy, "[isViterbi] Switch to the 1Mbps BLE PHY"),                                                                                                               \
  COMMAND_ENTRY("setBle2Mbps", "u*", bleSet2MbpsPhy, "[isViterbi] Switch to the 2Mbps BLE PHY"),                                                                                                               \
  COMMAND_ENTRY("setBleCoding", "u", bleSetCoding, "[coding] Switch to the given RAIL_BLE_Coding_t value"),                                                                                                    \
  COMMAND_SEPARATOR("RAIL Timer"),                                                                                                                                                                             \
  COMMAND_ENTRY("setTimer", "wb", setTimer, "[timeout mode] Set the RAIL timer timeout. You can use either an absolute (abs) or relative (rel) timer mode."),                                                  \
  COMMAND_ENTRY("timerCancel", "", timerCancel, "Cancel the RAIL timer if it's active."),                                                                                                                      \
  COMMAND_ENTRY("printTimerStats", "", printTimerStats, "Print current timer configuration."),                                                                                                                 \
  COMMAND_SEPARATOR("Auto Acking"),                                                                                                                                                                            \
  COMMAND_ENTRY("autoAckConfig", "bvvv", autoAckConfig, "[defaultState idleTime turnaroundTime ackTimeout] Configure and enable auto ack functionality in RAIL. "),                                            \
  COMMAND_ENTRY("autoAckDisable", "", autoAckDisable, "Disable auto ack. Use autoAckConfig to reenable."),                                                                                                     \
  COMMAND_ENTRY("setAckPayload", "vu*", setAckPayload, "[offset byte0 byte1 ...] Set the ack bytes to be sent."),                                                                                              \
  COMMAND_ENTRY("setAckLength", "v", setAckLength, "[length] Set the number of bytes to transmit for ack payloads"),                                                                                           \
  COMMAND_ENTRY("printAckPacket", "", printAckPacket, "Print the current ack data and length"),                                                                                                                \
  COMMAND_ENTRY("getAutoAck", "", getAutoAck, "Print the current state of auto acking."),                                                                                                                      \
  COMMAND_ENTRY("autoAckPause", "uu", autoAckPause, "[RxPause TxPause] Pause(1) or Resume(0) Auto Acking"),                                                                                                    \
  COMMAND_ENTRY("setTxAckOptions", "uu", setTxAckOptions, "[cancelAck useTxBuf] Enable(1) or Disable(0) feature for one receive"),                                                                             \
  COMMAND_SEPARATOR("Diagnostic and Test"),                                                                                                                                                                    \
  COMMAND_ENTRY("getmemw", "ww*", getMemWord, "[address count] Read count 32bit words starting at address"),                                                                                                   \
  COMMAND_ENTRY("setmemw", "ww*", setMemWord, "[address value...] Write as many 32bit values as specified starting at address"),                                                                               \
  COMMAND_ENTRY("setCtune", "v", setCtune, "[ctune] Set the value of CTUNE in the CMU->HFXOSTEADYSTATECTRL register"),                                                                                         \
  COMMAND_ENTRY("getCtune", "", getCtune, "Get the value of CTUNE in the CMU->HFXOSTEADYSTATECTRL register"),                                                                                                  \
  COMMAND_ENTRY("setPaCtune", "uu", setPaCtune, "[txPaCtune] [rxPaCtune] Set the value of PACTUNE for TX and RX mode"),                                                                                        \
  COMMAND_ENTRY("setDebugSignal", "?", setDebugSignal, "Configure chip specific debug output. Use 'setDebugSignal help' for more details."),                                                                   \
  COMMAND_ENTRY("setDebugMode", "w", setDebugMode, "[mode] 1 = Frequency Override. 0 = Disable debug mode"),                                                                                                   \
  COMMAND_ENTRY("freqOverride", "w", setFrequency, "[freq] Change to freq specified in Hz. Requires debug mode to be enabled. Only small frequency deviations from the current configuration are supported."), \
  COMMAND_ENTRY("directMode", "u", setDirectMode, "[enable] Enable(1) or Disable(0) direct mode"),                                                                                                             \
  COMMAND_ENTRY("directTx", "u", setDirectTx, "[enable] Enable(1) or Disable(0) TX in direct mode"),                                                                                                           \
  COMMAND_ENTRY("txCancel", "s", txCancel, "[delay] Set the time in microseconds after which we should cancel a tx (a negative value disables this)"),                                                         \
  COMMAND_ENTRY("getRandom", "vu*", getRandom, "[len hidden] Get len bytes of random data from the radio. Only print them to the screen if hidden is 0 (default)."),                                           \
  COMMAND_ENTRY("setTxUnderflow", "w", setTxUnderflow, "[enable] Enable(1) or Disable(0) TX underflows"),                                                                                                      \
  COMMAND_ENTRY("setRxOverflow", "w", setRxOverflow, "[enable] Enable(1) or Disable(0) RX overflows"),                                                                                                         \
  COMMAND_ENTRY("setCalibrations", "w", setCalibrations, "[enable] Enable(1) or Disable(0) RAIL calibrations"),                                                                                                \
  COMMAND_ENTRY("setTxTransitions", "bb", setTxTransitions, "[txSuccess txError] Set each RAIL TX state transition value to r(x) or i(dle)"),                                                                  \
  COMMAND_ENTRY("setRxTransitions", "bbu", setRxTransitions, "[rxSuccess rxError ignoreErrors] Set each RAIL RX state transition value to t(x), r(x), or i(dle), and set error events bitfield"),              \
  COMMAND_ENTRY("setTimings", "vvvv", setTimings, "[idleToRx txToRx idleToTx rxToTx] Set RAIL state transition timings in microseconds"),                                                                      \
  COMMAND_ENTRY("forceAssert", "w", forceAssert, "[errorCode] Force a RAIL assert with the given error code."),                                                                                                \
  COMMAND_ENTRY("printCallbacks", "w", configPrintCallbacks, "[printCallbacks] Enable printing of callbacks in chronological order."),                                                                         \
  COMMAND_ENTRY("reset", "", resetChip, "Perform a reboot of the chip"),                                                                                                                                       \
  INTERNAL_COMMANDS

/******************************************************************************
 * Externed Prototypes
 *****************************************************************************/
// Command callback function prototypes
// trx_ci
void rxConfig(int argc, char **argv);
void rx(int argc, char **argv);
void rxAt(int argc, char **argv);
void setRxOptions(int argc, char **argv);
void tx(int argc, char **argv);
void txWithOptions(int argc, char **argv);
void configTxOptions(int argc, char **argv);
void abortScheduledTxDuringRx(int argc, char **argv);
void txAtTime(int argc, char **argv);
void txAfterRx(int argc, char **argv);
void setTxTone(int argc, char **argv);
void setTxStream(int argc, char **argv);
void setDirectMode(int argc, char **argv);
void setDirectTx(int argc, char **argv);
void sleep(int argc, char **argv);
void rfSense(int argc, char **argv);

//lbt_ci
void setLbtMode(int argc, char **argv);
void getLbtParams(int argc, char **argv);
void setLbtParams(int argc, char **argv);

//packet_ci
void setTxPayload(int argc, char **argv);
void setTxLength(int argc, char **argv);
void printTxPacket(int argc, char **argv);
void peekRx(int argc, char **argv);

void setAckPayload(int argc, char **argv);
void setAckLength(int argc, char **argv);
void printAckPacket(int argc, char **argv);

void setFixedLength(int argc, char **argv);
void dataConfig(int argc, char **argv);
void fifoReset (int argc, char **argv);
void fifoModeTestOptions(int argc, char **argv);
void rxFifoManualRead(int argc, char **argv);
void txFifoManualLoad(int argc, char **argv);
void abortRxPacket(int argc, char **argv);

//parameter_ci
void setChannel(int argc, char **argv);
void getChannel(int argc, char **argv);
void setDirectMode(int argc, char **argv);
void setDirectTx(int argc, char **argv);
void getMemWord(int argc, char **argv);
void setMemWord(int argc, char **argv);

void getPower(int argc, char **argv);
void setPower(int argc, char **argv);
void getTxDelay(int argc, char **argv);
void setTxDelay(int argc, char **argv);
void setTxTransitions(int argc, char **argv);
void setRxTransitions(int argc, char **argv);
void setTimings(int argc, char **argv);

void setConfig(int argc, char **argv);
void getConfig(int argc, char **argv);
void listConfigs(int argc, char **argv);

void setCtune(int argc, char **argv);
void getCtune(int argc, char **argv);

void setPaCtune(int argc, char **argv);

void setTxFifoThreshold(int argc, char **argv);
void setRxFifoThreshold(int argc, char **argv);

void setRxConfig(int argc, char **argv);

//info_ci
void getStatus(int argc, char **argv);
void fifoStatus(int argc, char **argv);
void getVersion(int argc, char **argv);
void getVersionVerbose(int argc, char **argv);
void setPtiProtocol(int argc, char **argv);
void getRssi(int argc, char **argv);
void sweepPower(int argc, char **argv);
void startAvgRssi(int argc, char **argv);
void getAvgRssi(int argc, char **argv);
void resetCounters(int argc, char **argv);
void getTime(int argc, char **argv);
void setTime(int argc, char **argv);

//debug_ci
void setFrequency(int argc, char **argv);
void setDebugMode(int argc, char **argv);
void getMemWord(int argc, char **argv);
void setMemWord(int argc, char **argv);
void setTxUnderflow(int argc, char **argv);
void setRxOverflow(int argc, char **argv);
void setCalibrations(int argc, char **argv);
void getLogLevels(int argc, char **argv);
void setPeripheralEnable(int argc, char **argv);
void setNotifications(int argc, char **argv);
void resetChip(int argc, char **argv);
void printDataRates(int argc, char **argv);
void txCancel(int argc, char **argv);
void getRandom(int argc, char **argv);
void setDebugSignal(int argc, char **argv);
void forceAssert(int argc, char**argv);
void configPrintCallbacks(int argc, char**argv);

//address_filter_ci
void setAddressFilterConfig(int argc, char **argv);
void addressFilterByFrame(int argc, char **argv);
void setAddressFilter(int argc, char **argv);
void getAddressFilter(int argc, char **argv);
void printAddresses(int argc, char **argv);
void setAddress(int argc, char **argv);
void enableAddress(int argc, char **argv);

//error_rate_ci
void startPerMode(int argc, char **argv);
void getPerStats(int argc, char **argv);
void berConfigSet(int argc, char **argv);
void berRx(int argc, char **argv);
void berStatusGet(int argc, char **argv);

// 154_rx_ci
void ieee802154Enable(int argc, char **argv);
void config2p4Ghz802154(int argc, char **argv);
void ieee802154AcceptFrames(int argc, char **argv);
void ieee802154SetPromiscuousMode(int argc, char **argv);
void ieee802154SetPanCoordinator(int argc, char **argv);
void ieee802154SetPanId(int argc, char **argv);
void ieee802154SetShortAddress(int argc, char **argv);
void ieee802154SetLongAddress(int argc, char **argv);

// ble_ci
void bleEnable(int argc, char **argv);
void bleStatus(int argc, char **argv);
void bleSetChannelParams(int argc, char **argv);
void bleAdvertisingConfig(int argc, char **argv);
void bleSet1MbpsPhy(int argc, char **argv);
void bleSet2MbpsPhy(int argc, char **argv);
void bleSetCoding(int argc, char **argv);

//timer_ci
void setTimer(int argc, char **argv);
void printTimerStats(int argc, char **argv);
void timerCancel(int argc, char** argv);

//autoack_ci
void autoAckConfig(int argc, char **argv);
void autoAckDisable(int argc, char **argv);
void getAutoAck(int argc, char **argv);
void autoAckPause(int argc, char **argv);
void setTxAckOptions(int argc, char **argv);

#endif // __APP_CI_H__
