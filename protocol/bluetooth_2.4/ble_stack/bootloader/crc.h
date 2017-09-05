/** @file hal/micro/crc.h
 * See @ref crc for detailed documentation.
 *
 * <!-- Copyright 2005 by Ember Corporation. All rights reserved.-->
 */

#ifndef __CRC_H__
#define __CRC_H__

#include <stdint.h>

/** @addtogroup crc
 * @brief Functions that provide access to cyclic redundancy code (CRC)
 * calculation. See crc.h for source code.
 *@{
 */

/** @brief Calculates 16-bit cyclic redundancy code (CITT CRC 16).
 *
 * Applies the standard CITT CRC 16 polynomial to a
 * single byte. It should support being called first with an initial
 * value, then repeatedly until all data is processed.
 *
 * @param newByte     The new byte to be run through CRC.
 *
 * @param prevResult  The previous CRC result.
 *
 * @return The new CRC result.
 */
uint16_t halCommonCrc16(uint8_t newByte, uint16_t prevResult);

/** @brief Calculates 32-bit cyclic redundancy code
 *
 * @note On some radios or micros, the CRC
 * for error detection on packet data is calculated in hardware.
 *
 * Applies a CRC32 polynomial to a
 * single byte. It should support being called first with an initial
 * value, then repeatedly until all data is processed.
 *
 * @param newByte       The new byte to be run through CRC.
 *
 * @param prevResult    The previous CRC result.
 *
 * @return The new CRC result.
 */
uint32_t halCommonCrc32(uint8_t newByte, uint32_t prevResult);

// Commonly used initial and expected final CRC32 values
#define INITIAL_CRC             0xFFFFFFFFL
#define CRC32_START             INITIAL_CRC
#define CRC32_END               0xDEBB20E3L  // For CRC32 POLYNOMIAL run LSB-MSB

/**@}  // end of CRC Functions
 */

#endif //__CRC_H__
