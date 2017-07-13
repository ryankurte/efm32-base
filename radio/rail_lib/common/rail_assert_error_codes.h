/***************************************************************************//**
 * @file rail_assert_error_codes.h
 * @brief Definition of error codes that occur in rail for use in
      RAILCb_AssertFailed. This file is purely informational and optional -
      it need not be included even if rail_assert libraries are included.

 * @copyright Copyright 2017 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#include "rail_types.h"

/**
 * @addtogroup Assertions
 * @{
 */

#ifndef _RAIL_ASSERT_ERROR_CODES_

#define _RAIL_ASSERT_ERROR_CODES_

/**
 * Enumeration of all possible error codes from RAIL_ASSERT
 */
RAIL_ENUM(RAIL_AssertErrorCodes_t)
{
  RAIL_ASSERT_FAILED_APPENDED_INFO_MISSING,
  RAIL_ASSERT_FAILED_RX_BUFFER_BYTES,
  RAIL_ASSERT_FAILED_RX_BUFFER_ZERO_BYTES_READ,
  RAIL_ASSERT_FAILED_FRC_BLOCKERROR_HANDLED_PACKET_MODE,
  RAIL_ASSERT_FAILED_FRC_BLOCKERROR_HANDLED_FIFO_MODE,
  RAIL_ASSERT_FAILED_SYNTH_DIVCTRL_ENUM_CONVERSION_ERROR, //5
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_RX_BUFFER,
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_RXLEN_BUFFER,
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_TX_BUFFER,
  RAIL_ASSERT_FAILED_UNEXPECTED_STATE_TXACK_BUFFER,
  RAIL_ASSERT_FAILED_TXACK_LENGTH, //10
  RAIL_ASSERT_FAILED_PBUFFER_NOT_DEFINED,
  RAIL_ASSERT_FAILED_INSUFFICIENT_BYTES_IN_RX_PACKET,
  RAIL_ASSERT_FAILED_FRC_PRI_IRQ_NULL,
  RAIL_ASSERT_FAILED_FRC_IRQ_NULL,
  RAIL_ASSERT_FAILED_RAC_RSM_IRQ_NULL, //15
  RAIL_ASSERT_FAILED_RAC_SEQ_IRQ_NULL,
  RAIL_ASSERT_FAILED_AGC_IRQ_NULL,
  RAIL_ASSERT_FAILED_PROTIMER_IRQ_NULL,
  RAIL_ASSERT_FAILED_BUFC_IRQ_NULL,
  RAIL_ASSERT_FAILED_MODEM_IRQ_NULL, //20
  RAIL_ASSERT_FAILED_CLOCK_PRESCALER,
  RAIL_ASSERT_FAILED_RTCC_POST_WAKEUP,
  RAIL_ASSERT_FAILED_SYNTH_VCO_FREQUENCY,
  RAIL_ASSERT_FAILED_RAC_STATE,
  RAIL_ASSERT_FAILED_RETIME_LIMIT, //25
  RAIL_ASSERT_FAILED_NESTED_SEQUENCER_LOCK,
  RAIL_ASSERT_FAILED_RSSI_AVERAGE_DONE,
  RAIL_ASSERT_FAILED_DFL_BITS_SIZE,
  RAIL_ASSERT_FAILED_PROTIMER_RANDOM_SEED,
  RAIL_ASSERT_FAILED_EFR32XG1_REGISTER_SIZE, //30
  RAIL_ASSERT_FAILED_PROTIMER_CHANNEL,
  RAIL_ASSERT_FAILED_TIMER_REQUIRES_WRAP,
  RAIL_ASSERT_FAILED_BASECNTTOP,
  RAIL_ASSERT_FAILED_DEPRECATED_LBTRETRY,
  RAIL_ASSERT_FAILED_RTCC_SYNC_MISSED, //35
  RAIL_ASSERT_FAILED_CLOCK_SOURCE_NOT_READY,
  RAIL_ASSERT_FAILED_TIMINGS_INVALID,
};

/**
 * Use this define to create an array of error messages that map to the codes
 * in \ref RAIL_AssertErrorCodes_t. You can use these to print slightly more
 * detailed error strings related to a particular assert error code if desired.
 * For example, you could implement your assert failed callback as follows to
 * make use of this.
 *
 * @code{.c}
 * void RAILCb_AssertFailed(uint32_t errorCode)
 * {
 *   static const char* railErrorMessages[] = RAIL_ASSERT_ERROR_MESSAGES;
 *   const char *errorMessage = "Unknown";
 *
 *   // If this error code is within the range of known error messages then use
 *   // the appropriate error message.
 *   if (errorCode < (sizeof(railErrorMessages) / sizeof(char*))) {
 *     errorMessage = railErrorMessages[errorCode];
 *   }
 *   printf(errorMessage);
 *
 *   // Reset the chip since an assert is a fatal error
 *   NVIC_SystemReset();
 * }
 * @endcode
 */
#define RAIL_ASSERT_ERROR_MESSAGES {                                \
    "Appended info missing from Rx packet",                         \
    "Payload bytes missing from Rx packet",                         \
    "Error reading back packet payload",                            \
    "Unexpected block error occurred (Packet mode)",                \
    "Unexpected block error occurred (FIFO mode)",                  \
    "Unable to configure radio for IR calibration",                 \
    "Reached unexpected state while handling Rx buffer events",     \
    "Reached unexpected state while handling RXLEN buffer events",  \
    "Reached unexpected state while handling Tx buffer events",     \
    "Reached unexpected state while handling Tx ACK buffer events", \
    "Requested Tx ACK dataLength greater than Tx ACK buffer size",  \
    "No memory to store receive packet",                            \
    "Packet length longer than the receive FIFO size",              \
    "FRC_PRI IRQ callback is NULL",                                 \
    "FRC IRQ callback is NULL",                                     \
    "RAC_RSM IRQ callback is NULL",                                 \
    "RAC_SEQ IRQ callback is NULL",                                 \
    "AGC IRQ callback is NULL",                                     \
    "PROTIMER IRQ callback is NULL",                                \
    "BUFC IRQ callback is NULL",                                    \
    "MODEM IRQ callback is NULL",                                   \
    "Invalid radio clock prescaler",                                \
    "Error synchronizing the RAIL timebase after sleep",            \
    "VCO frequency outside supported range",                        \
    "Radio active while changing channels",                         \
    "Unable to configure DCDC retiming",                            \
    "Nested attempt to lock the sequencer",                         \
    "RSSI averaging enabled without a valid callback",              \
    "Invalid dynamic frame length setting provided (dflBits)",      \
    "Unable to seed radio pseudo random number generator",          \
    "Timeout exceeds EFR32XG1 register size",                       \
    "Invalid timer channel specified",                              \
    "Timer value larger than RAIL timebase",                        \
    "LBT config exceeds EFR32XG1 register size",                    \
    "Deprecated CSMA/LBT retry callback unexpectedly called",       \
    "Could not synchronize RAIL timebase with the RTC",             \
    "Clock source not ready",                                       \
    "Attempted to set RAIL timings to invalid value",               \
}

#endif
/**
 * @}
 */
