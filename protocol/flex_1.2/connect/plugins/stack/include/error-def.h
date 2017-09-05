/**
 * @file error-def.h
 * @brief Return-code definitions for Ember Connect stack API functions.
 *
 * See @ref status_codes for documentation.
 *
 * <!--Copyright 2014 by Silicon Laboratories. All rights reserved.      *80*-->
 */

/**
 * @addtogroup status_codes
 *
 * Many Ember Connect API functions return an ::EmberStatus value to indicate
 * the success or failure of the call.
 * Return codes are one byte long.
 * This page documents the possible status codes and their meanings.
 *
 * See error-def.h for source code.
 *
 * See also error.h for information on how the values for the return codes are
 * built up from these definitions.
 * The file error-def.h is separated from error.h because utilities will use
 * this file to parse the return codes.
 *
 * @note Do not include error-def.h directly. It is included by
 * error.h inside an enum typedef, which is in turn included by ember.h.
 *
 * @{
 */

/**
 * @name Generic Messages
 * These messages are system wide.
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The generic "no error" message.
 */
#define EMBER_SUCCESS(0x00)
#else
DEFINE_ERROR(SUCCESS, 0)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The generic "fatal error" message.
 */
#define EMBER_ERR_FATAL(0x01)
#else
DEFINE_ERROR(ERR_FATAL, 0x01)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief An invalid value was passed as an argument to a function.
 */
#define EMBER_BAD_ARGUMENT(0x02)
#else
DEFINE_ERROR(BAD_ARGUMENT, 0x02)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The manufacturing and stack token format in non-volatile memory
 * is different than what the stack expects (returned at initialization).
 */
#define EMBER_EEPROM_MFG_STACK_VERSION_MISMATCH(0x04)
#else
DEFINE_ERROR(EEPROM_MFG_STACK_VERSION_MISMATCH, 0x04)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The API call is not allowed given the current state of the
 * stack.
 */
#define EMBER_INVALID_CALL(0x70)
#else
DEFINE_ERROR(INVALID_CALL, 0x70)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The manufacturing token format in non-volatile memory is
 * different than what the stack expects (returned at initialization).
 */
#define EMBER_EEPROM_MFG_VERSION_MISMATCH(0x06)
#else
DEFINE_ERROR(EEPROM_MFG_VERSION_MISMATCH, 0x06)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The stack token format in non-volatile memory is different
 * than what the stack expects (returned at initialization).
 */
#define EMBER_EEPROM_STACK_VERSION_MISMATCH(0x07)
#else
DEFINE_ERROR(EEPROM_STACK_VERSION_MISMATCH, 0x07)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@} // END Generic Messages

/**
 * @name Packet Buffer Module Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief There are no more buffers.
 */
#define EMBER_NO_BUFFERS(0x18)
#else
DEFINE_ERROR(NO_BUFFERS, 0x18)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@} / END Packet Buffer Module Errors

/**
 * @name Serial Manager Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Specified an invalid baud rate.
 */
#define EMBER_SERIAL_INVALID_BAUD_RATE(0x20)
#else
DEFINE_ERROR(SERIAL_INVALID_BAUD_RATE, 0x20)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Specified an invalid serial port.
 */
#define EMBER_SERIAL_INVALID_PORT(0x21)
#else
DEFINE_ERROR(SERIAL_INVALID_PORT, 0x21)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Tried to send too much data.
 */
#define EMBER_SERIAL_TX_OVERFLOW(0x22)
#else
DEFINE_ERROR(SERIAL_TX_OVERFLOW, 0x22)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief There was not enough space to store a received character
 * and the character was dropped.
 */
#define EMBER_SERIAL_RX_OVERFLOW(0x23)
#else
DEFINE_ERROR(SERIAL_RX_OVERFLOW, 0x23)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Detected a UART framing error.
 */
#define EMBER_SERIAL_RX_FRAME_ERROR(0x24)
#else
DEFINE_ERROR(SERIAL_RX_FRAME_ERROR, 0x24)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Detected a UART parity error.
 */
#define EMBER_SERIAL_RX_PARITY_ERROR(0x25)
#else
DEFINE_ERROR(SERIAL_RX_PARITY_ERROR, 0x25)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief There is no received data to process.
 */
#define EMBER_SERIAL_RX_EMPTY(0x26)
#else
DEFINE_ERROR(SERIAL_RX_EMPTY, 0x26)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The receive interrupt was not handled in time, and a
 * character was dropped.
 */
#define EMBER_SERIAL_RX_OVERRUN_ERROR(0x27)
#else
DEFINE_ERROR(SERIAL_RX_OVERRUN_ERROR, 0x27)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@}

/**
 * @name MAC Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief No pending data exists for device doing a data poll.
 */
#define EMBER_MAC_NO_DATA(0x31)
#else
DEFINE_ERROR(MAC_NO_DATA, 0x31)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Attempt to scan when we are joined to a network.
 */
#define EMBER_MAC_JOINED_NETWORK(0x32)
#else
DEFINE_ERROR(MAC_JOINED_NETWORK, 0x32)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The frequency hopping client failed the frequency hopping
 * synchronization procedure, it timed out trying to reach the frequency hopping
 * server.
 */
#define EMBER_MAC_SYNC_TIMEOUT(0x33)
#else
DEFINE_ERROR(MAC_SYNC_TIMEOUT, 0x33)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The frequency hopping client failed the frequency hopping
 * synchronization procedure, the server is currently using a different seed.
 */
#define EMBER_MAC_SYNC_WRONG_SEED(0x34)
#else
DEFINE_ERROR(MAC_SYNC_WRONG_SEED, 0x34)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief MAC security operation failed.
 */
#define EMBER_MAC_SECURITY_FAILED(0x35)
#else
DEFINE_ERROR(MAC_SECURITY_FAILED, 0x35)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Failed to scan current channel because we were unable to
 * transmit the relevent MAC command.
 */
#define EMBER_MAC_COMMAND_TRANSMIT_FAILURE(0x36)
#else
DEFINE_ERROR(MAC_COMMAND_TRANSMIT_FAILURE, 0x36)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Transmission failed: the destination node does not appear in the
 * neighbor or child tables.
 */
#define EMBER_MAC_UNKNOWN_DESTINATION(0x37)
#else
DEFINE_ERROR(MAC_UNKNOWN_DESTINATION, 0x37)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Transmission failed: the local node does not support security or a
 * secured transmission has been requested to a child that does not support
 * security.
 */
#define EMBER_MAC_SECURITY_NOT_SUPPORTED(0x38)
#else
DEFINE_ERROR(MAC_SECURITY_NOT_SUPPORTED, 0x38)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The MAC transmit queue is full.
 */
#define EMBER_MAC_TRANSMIT_QUEUE_FULL(0x39)
#else
// Internal
DEFINE_ERROR(MAC_TRANSMIT_QUEUE_FULL, 0x39)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief MAC header FCF error on receive.
 */
#define EMBER_MAC_UNKNOWN_HEADER_TYPE(0x3A)
#else
DEFINE_ERROR(MAC_UNKNOWN_HEADER_TYPE, 0x3A)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief MAC ACK header received.
 */
#define EMBER_MAC_ACK_HEADER_TYPE(0x3B)
#else
DEFINE_ERROR(MAC_ACK_HEADER_TYPE, 0x3B)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The MAC can't complete this task because it is scanning.
 */
#define EMBER_MAC_SCANNING(0x3D)
#else
DEFINE_ERROR(MAC_SCANNING, 0x3D)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The requested operation cannot be completed because the MAC is
 * currently busy performing some high priority task.
 */
#define EMBER_MAC_BUSY(0x3E)
#else
DEFINE_ERROR(MAC_BUSY, 0x3E)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief We expected to receive an ACK following the transmission, but
 * the MAC level ACK was never received.
 */
#define EMBER_MAC_NO_ACK_RECEIVED(0x40)
#else
DEFINE_ERROR(MAC_NO_ACK_RECEIVED, 0x40)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Indirect data message timed out before polled.
 */
#define EMBER_MAC_INDIRECT_TIMEOUT(0x41)
#else
DEFINE_ERROR(MAC_INDIRECT_TIMEOUT, 0x41)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Transmission failed: the indirect message was purged because the
 * destination child has been removed or updated.
 */
#define EMBER_MAC_INDIRECT_MESSAGE_PURGED(0x42)
#else
DEFINE_ERROR(MAC_INDIRECT_MESSAGE_PURGED, 0x42)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@}

/**
 * @name  Simulated EEPROM Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The Simulated EEPROM is telling the application that there
 * is at least one flash page to be erased.  The GREEN status means the
 * current page has not filled above the ::ERASE_CRITICAL_THRESHOLD.
 *
 * The application should call the function ::halSimEepromErasePage() when it can
 * to erase a page.
 */
#define EMBER_SIM_EEPROM_ERASE_PAGE_GREEN(0x43)
#else
DEFINE_ERROR(SIM_EEPROM_ERASE_PAGE_GREEN, 0x43)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The Simulated EEPROM is telling the application that there
 * is at least one flash page to be erased.  The RED status means the
 * current page has filled above the ::ERASE_CRITICAL_THRESHOLD.
 *
 * Due to the shrinking availability of write space, there is a danger of
 * data loss.  The application must call the function ::halSimEepromErasePage()
 * as soon as possible to erase a page.
 */
#define EMBER_SIM_EEPROM_ERASE_PAGE_RED(0x44)
#else
DEFINE_ERROR(SIM_EEPROM_ERASE_PAGE_RED, 0x44)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The Simulated EEPROM has run out of room to write any new data
 * and the data trying to be set has been lost.  This error code is the
 * result of ignoring the ::SIM_EEPROM_ERASE_PAGE_RED error code.
 *
 * The application must call the function ::halSimEepromErasePage() to make room for
 * any further calls to set a token.
 */
#define EMBER_SIM_EEPROM_FULL(0x45)
#else
DEFINE_ERROR(SIM_EEPROM_FULL, 0x45)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//  Errors 46 and 47 are now defined below in the
//    flash error block (was attempting to prevent renumbering)

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Attempt 1 to initialize the Simulated EEPROM has failed.
 *
 * This failure means the information already stored in Flash (or a lack
 * thereof), is fatally incompatible with the token information compiled
 * into the code image being run.
 */
#define EMBER_SIM_EEPROM_INIT_1_FAILED(0x48)
#else
DEFINE_ERROR(SIM_EEPROM_INIT_1_FAILED, 0x48)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Attempt 2 to initialize the Simulated EEPROM has failed.
 *
 * This failure means Attempt 1 failed, and the token system failed to
 * properly reload default tokens and reset the Simulated EEPROM.
 */
#define EMBER_SIM_EEPROM_INIT_2_FAILED(0x49)
#else
DEFINE_ERROR(SIM_EEPROM_INIT_2_FAILED, 0x49)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Attempt 3 to initialize the Simulated EEPROM has failed.
 *
 * This failure means one or both of the tokens ::TOKEN_MFG_NVDATA_VERSION or
 * ::TOKEN_STACK_NVDATA_VERSION were incorrect and the token system failed to
 * properly reload default tokens and reset the Simulated EEPROM.
 */
#define EMBER_SIM_EEPROM_INIT_3_FAILED(0x4A)
#else
DEFINE_ERROR(SIM_EEPROM_INIT_3_FAILED, 0x4A)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The Simulated EEPROM is repairing itself.
 *
 * While there's nothing for an app to do when the SimEE is going to
 * repair itself (SimEE has to be fully functional for the rest of the
 * system to work), alert the application to the fact that repairing
 * is occurring.  There are debugging scenarios where an app might want
 * to know that repairing is happening; such as monitoring frequency.
 * @note  Common situations will trigger an expected repair, such as
 *        using an erased chip or changing token definitions.
 */
#define EMBER_SIM_EEPROM_REPAIRING(0x4D)
#else
DEFINE_ERROR(SIM_EEPROM_REPAIRING, 0x4D)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@}

/**
 * @name  Flash Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief A fatal error has occurred while trying to write data to the
 * Flash.  The target memory attempting to be programmed is already programmed.
 * The flash write routines were asked to flip a bit from a 0 to 1, which is
 * physically impossible and the write was therefore inhibited.  The data in
 * the flash cannot be trusted after this error.
 */
#define EMBER_ERR_FLASH_WRITE_INHIBITED(0x46)
#else
DEFINE_ERROR(ERR_FLASH_WRITE_INHIBITED, 0x46)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief A fatal error has occurred while trying to write data to the
 * Flash and the write verification has failed.  The data in the flash
 * cannot be trusted after this error, and it is possible this error is the
 * result of exceeding the life cycles of the flash.
 */
#define EMBER_ERR_FLASH_VERIFY_FAILED(0x47)
#else
DEFINE_ERROR(ERR_FLASH_VERIFY_FAILED, 0x47)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @description A fatal error has occurred while trying to write data to the
 * flash, possibly due to write protection or an invalid address.  The data in
 * the flash cannot be trusted after this error, and it is possible this error
 * is the result of exceeding the life cycles of the flash.
 */
#define EMBER_ERR_FLASH_PROG_FAIL(0x4B)
#else
DEFINE_ERROR(ERR_FLASH_PROG_FAIL, 0x4B)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @description A fatal error has occurred while trying to erase flash, possibly
 * due to write protection.  The data in the flash cannot be trusted after
 * this error, and it is possible this error is the result of exceeding the
 * life cycles of the flash.
 */
#define EMBER_ERR_FLASH_ERASE_FAIL(0x4C)
#else
DEFINE_ERROR(ERR_FLASH_ERASE_FAIL, 0x4C)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@}

/**
 * @name  Bootloader Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The bootloader received an invalid message (failed attempt
 * to go into bootloader).
 */
#define EMBER_ERR_BOOTLOADER_TRAP_TABLE_BAD(0x58)
#else
DEFINE_ERROR(ERR_BOOTLOADER_TRAP_TABLE_BAD, 0x58)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Bootloader received an invalid message (failed attempt to go
 * into bootloader).
 */
#define EMBER_ERR_BOOTLOADER_TRAP_UNKNOWN(0x59)
#else
DEFINE_ERROR(ERR_BOOTLOADER_TRAP_UNKNOWN, 0x59)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The bootloader cannot complete the bootload operation because
 * either an image was not found or the image exceeded memory bounds.
 */
#define EMBER_ERR_BOOTLOADER_NO_IMAGE(0x05A)
#else
DEFINE_ERROR(ERR_BOOTLOADER_NO_IMAGE, 0x5A)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@}

/**
 * @name  Transport Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The message to be transmitted is too big to fit into a
 * single over-the-air packet.
 */
#define EMBER_MESSAGE_TOO_LONG(0x74)
#else
DEFINE_ERROR(MESSAGE_TOO_LONG, 0x74)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@}

/**
 * @name  HAL Module Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Conversion is complete.
 */
#define EMBER_ADC_CONVERSION_DONE(0x80)
#else
DEFINE_ERROR(ADC_CONVERSION_DONE, 0x80)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Conversion cannot be done because a request is being
 * processed.
 */
#define EMBER_ADC_CONVERSION_BUSY(0x81)
#else
DEFINE_ERROR(ADC_CONVERSION_BUSY, 0x81)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Conversion is deferred until the current request has been
 * processed.
 */
#define EMBER_ADC_CONVERSION_DEFERRED(0x82)
#else
DEFINE_ERROR(ADC_CONVERSION_DEFERRED, 0x82)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief No results are pending.
 */
#define EMBER_ADC_NO_CONVERSION_PENDING(0x84)
#else
DEFINE_ERROR(ADC_NO_CONVERSION_PENDING, 0x84)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief Sleeping (for a duration) has been abnormally interrupted
 * and exited prematurely.
 */
#define EMBER_SLEEP_INTERRUPTED(0x85)
#else
DEFINE_ERROR(SLEEP_INTERRUPTED, 0x85)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@}

/**
 * @name  PHY Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The transmit hardware buffer underflowed.
 */
#define EMBER_PHY_TX_UNDERFLOW(0x88)
#else
DEFINE_ERROR(PHY_TX_UNDERFLOW, 0x88)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The transmit hardware did not finish transmitting a packet.
 */
#define EMBER_PHY_TX_INCOMPLETE(0x89)
#else
DEFINE_ERROR(PHY_TX_INCOMPLETE, 0x89)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief An unsupported channel setting was specified.
 */
#define EMBER_PHY_INVALID_CHANNEL(0x8A)
#else
DEFINE_ERROR(PHY_INVALID_CHANNEL, 0x8A)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief An unsupported power setting was specified.
 */
#define EMBER_PHY_INVALID_POWER(0x8B)
#else
DEFINE_ERROR(PHY_INVALID_POWER, 0x8B)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The requested operation cannot be completed because the radio
 * is currently busy, either transmitting a packet or performing calibration.
 */
#define EMBER_PHY_TX_BUSY(0x8C)
#else
DEFINE_ERROR(PHY_TX_BUSY, 0x8C)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The transmit attempt failed because all CCA attempts indicated that
 * the channel was busy.
 */
#define EMBER_PHY_TX_CCA_FAIL(0x8D)
#else
DEFINE_ERROR(PHY_TX_CCA_FAIL, 0x8D)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The expected ACK was received after the last transmission.
 */
#define EMBER_PHY_ACK_RECEIVED(0x8F)
#else
DEFINE_ERROR(PHY_ACK_RECEIVED, 0x8F)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@}

/**
 * @name  Return Codes Passed to emberStackStatusHandler()
 * See also ::emberStackStatusHandler().
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The stack software has completed initialization and is ready
 * to send and receive packets over the air.
 */
#define EMBER_NETWORK_UP(0x90)
#else
DEFINE_ERROR(NETWORK_UP, 0x90)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The network is not operating.
 */
#define EMBER_NETWORK_DOWN(0x91)
#else
DEFINE_ERROR(NETWORK_DOWN, 0x91)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The node failed to initiate the scanning process during the joining
 * process.
 */
#define EMBER_JOIN_SCAN_FAILED(0x93)
#else
DEFINE_ERROR(JOIN_SCAN_FAILED, 0x93)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief An attempt to join a network failed.
 */
#define EMBER_JOIN_FAILED(0x94)
#else
DEFINE_ERROR(JOIN_FAILED, 0x94)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief An attempt to join a network was rejected.
 */
#define EMBER_JOIN_DENIED(0x95)
#else
DEFINE_ERROR(JOIN_DENIED, 0x95)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The node timed out waiting for a response during the joining process.
 */
#define EMBER_JOIN_TIMEOUT(0x96)
#else
DEFINE_ERROR(JOIN_TIMEOUT, 0x96)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/** @brief An attempt to join or rejoin the network failed because
 *  no valid beacons could be heard by the joining node.
 */
#define EMBER_NO_VALID_BEACONS(0xAB)
#else
DEFINE_ERROR(NO_VALID_BEACONS, 0xAB)
#endif

//@}

/**
 * @name  Security Errors
 */
#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The passed key data is not valid.  A key of all zeros or
 *   all F's are reserved values and cannot be used.
 */
#define EMBER_KEY_INVALID(0xB2)
#else
DEFINE_ERROR(KEY_INVALID, 0xB2)
#endif // DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/** @brief The security data provided was not valid, or an
 *    integrity check failed.
 */
#define EMBER_SECURITY_DATA_INVALID(0xBD)
#else
DEFINE_ERROR(SECURITY_DATA_INVALID, 0xBD)
#endif

//@}

/**
 * @name  Miscellaneous Network Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The node has not joined a network.
 */
#define EMBER_NOT_JOINED(0x93)
#else
DEFINE_ERROR(NOT_JOINED, 0x93)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@}

/**
 * @name  Miscellaneous Utility Errors
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief An index was passed into the function that was larger
 * than the valid range.
 */
#define EMBER_INDEX_OUT_OF_RANGE(0xB1)
#else
DEFINE_ERROR(INDEX_OUT_OF_RANGE, 0xB1)
#endif

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief There are no empty entries left in the table.
 */
#define EMBER_TABLE_FULL(0xB4)
#else
DEFINE_ERROR(TABLE_FULL, 0xB4)
#endif //DOXYGEN_SHOULD_SKIP_THIS

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The requested table entry has been erased and contains
 *   no valid data.
 */
#define EMBER_TABLE_ENTRY_ERASED(0xB6)
#else
DEFINE_ERROR(TABLE_ENTRY_ERASED, 0xB6)
#endif

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The requested function cannot be executed because
 *   the library that contains the necessary functionality is not present.
 */
#define EMBER_LIBRARY_NOT_PRESENT(0xB5)
#else
DEFINE_ERROR(LIBRARY_NOT_PRESENT, 0xB5)
#endif

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief The stack accepted the command and is currently processing
 * the request.  The results will be returned via an appropriate handler.
 */
#define EMBER_OPERATION_IN_PROGRESS(0xBA)
#else
DEFINE_ERROR(OPERATION_IN_PROGRESS, 0xBA)
#endif

//@}

/**
 * @name  Application Errors
 * These error codes are available for application use.
 */
//@{

#ifdef DOXYGEN_SHOULD_SKIP_THIS
/**
 * @brief This error is reserved for customer application use.
 *  This will never be returned from any portion of the network stack or HAL.
 */
#define EMBER_APPLICATION_ERROR_0(0xF0)
#define EMBER_APPLICATION_ERROR_1(0xF1)
#define EMBER_APPLICATION_ERROR_2(0xF2)
#define EMBER_APPLICATION_ERROR_3(0xF3)
#define EMBER_APPLICATION_ERROR_4(0xF4)
#define EMBER_APPLICATION_ERROR_5(0xF5)
#define EMBER_APPLICATION_ERROR_6(0xF6)
#define EMBER_APPLICATION_ERROR_7(0xF7)
#define EMBER_APPLICATION_ERROR_8(0xF8)
#define EMBER_APPLICATION_ERROR_9(0xF9)
#define EMBER_APPLICATION_ERROR_10(0xFA)
#define EMBER_APPLICATION_ERROR_11(0xFB)
#define EMBER_APPLICATION_ERROR_12(0xFC)
#define EMBER_APPLICATION_ERROR_13(0xFD)
#define EMBER_APPLICATION_ERROR_14(0xFE)
#define EMBER_APPLICATION_ERROR_15(0xFF)
#else
DEFINE_ERROR(APPLICATION_ERROR_0, 0xF0)
DEFINE_ERROR(APPLICATION_ERROR_1, 0xF1)
DEFINE_ERROR(APPLICATION_ERROR_2, 0xF2)
DEFINE_ERROR(APPLICATION_ERROR_3, 0xF3)
DEFINE_ERROR(APPLICATION_ERROR_4, 0xF4)
DEFINE_ERROR(APPLICATION_ERROR_5, 0xF5)
DEFINE_ERROR(APPLICATION_ERROR_6, 0xF6)
DEFINE_ERROR(APPLICATION_ERROR_7, 0xF7)
DEFINE_ERROR(APPLICATION_ERROR_8, 0xF8)
DEFINE_ERROR(APPLICATION_ERROR_9, 0xF9)
DEFINE_ERROR(APPLICATION_ERROR_10, 0xFA)
DEFINE_ERROR(APPLICATION_ERROR_11, 0xFB)
DEFINE_ERROR(APPLICATION_ERROR_12, 0xFC)
DEFINE_ERROR(APPLICATION_ERROR_13, 0xFD)
DEFINE_ERROR(APPLICATION_ERROR_14, 0xFE)
DEFINE_ERROR(APPLICATION_ERROR_15, 0xFF)
#endif //DOXYGEN_SHOULD_SKIP_THIS

//@} // END name group

/** @} END addtogroup */
