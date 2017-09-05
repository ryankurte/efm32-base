/***************************************************************************//**
 * @file ezsp-host-io.h
 * @brief Header for EZSP host I/O functions
 *
 * See @ref ezsp_util for documentation.
 *
 *******************************************************************************
 * @section License
 * <b>(C) Copyright 2015 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
 * obligation to support this Software. Silicon Labs is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Silicon Labs will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 ******************************************************************************/
#ifndef __EZSP_HOST_IO_H__
#define __EZSP_HOST_IO_H__

/** @addtogroup ezsp_util
 *
 * See ezsp-host-io.h.
 *
 *@{
 */

/** @brief Initializes the serial port for use by EZSP. The port number,
 *  baud rate, stop bits, and flow control method are specifed by the
 *  by the host configuration.
 *
 * @return
 * - ::EZSP_SUCCESS
 * - ::EZSP_HOST_FATAL_ERROR
 */
EzspStatus ezspSerialInit(void);

/** @brief If the serial port is open, discards all I/O data
 *  and closes the port.
 */
void ezspSerialClose(void);

/** @brief Resets the ncp by deasserting and asserting DTR.
 *  This requires a conenction between DTR and nRESET, as there is on the
 *  EM260 breakout board when the on-board USB interface is used.
 */
void ezspResetDtr(void);

/** @brief Custom method for resetting the ncp which must be defined by
 *  the user for their specific hardware and interconect. As shipped, this
 *  function does nothing.
 */
void ezspResetCustom(void);

/** @brief Checks to see if there is space available in the serial
 *  write buffer. If the buffer is full, it is output to the serial port
 *  and it return a "no space indication".
 *
 * @return
 * - ::EZSP_SUCCESS
 * _ ::EZSP_NO_TX_SPACE
 */
EzspStatus ezspSerialWriteAvailable(void);

/** @brief Writes a byte to the serial output buffer.
 *
 * @param byte byte to write
 */
void ezspSerialWriteByte(uint8_t byte);

/** @brief Writes all data the write output buffer to the serial port
 *  and calls fsync(). This is called when a complete frame to be sent to
 *  the ncp has been created.
 */
void ezspSerialWriteFlush(void);

/** @brief Reads a byte from the serial port, if one is available.
 *
 * @param byte pointer to a variable where the byte read will be output
 *
 * @return
 * - ::EZSP_SUCCESS
 * - ::EZSP_NO_RX_DATA
 */
EzspStatus ezspSerialReadByte(uint8_t *byte);

/** @brief Returns number of the bytes available to read from the serial port.
 *
 * @param count pointer to a variable where the byte count will be written
 *
 * @return
 * - ::EZSP_SUCCESS
 * - ::EZSP_NO_RX_DATA
 */
EzspStatus ezspSerialReadAvailable(uint16_t *count);

/** @brief Discards input data from the serial port until there
 *  is none left.
 */
void ezspSerialReadFlush(void);

/** @brief Flushes the ASCII trace output stream.
 */
void ezspDebugFlush(void);

/** @brief Prints ACSII trace information.
 */
#define DEBUG_STREAM  stdout

#ifdef WIN32
  #define ezspDebugPrintf printf
#else
  #define ezspDebugPrintf(...) fprintf(DEBUG_STREAM, __VA_ARGS__)
#endif

#define ezspDebugVfprintf(format, argPointer) \
  vfprintf(DEBUG_STREAM, format, argPointer)

/** @brief Returns the file descriptor associated with the serial port.
 */
int ezspSerialGetFd(void);

/** @brief An enumeration of events that can occur on the serial port.
 */
typedef enum {
  EZSP_SERIAL_PORT_CLOSED = 0,
  EZSP_SERIAL_PORT_OPENED = 1,
} EzspSerialPortEvent;

/** @brief Register a callback that will get called whenever the EZSP serial
 *  file descriptor is opened or closed.
 */
typedef void (*EzspSerialPortCallbackFunction)(EzspSerialPortEvent event, int fileDescriptor);
bool ezspSerialPortRegisterCallback(EzspSerialPortCallbackFunction callback);

/** @brief tests to see if all serial transmit data has actually been shifted
 *  out the host's serial port transmit data pin.
 *  As shipped this is a stub function that must be edited to match the actual
 *  operating system and/or UART hardware.
 * @return  true if all data has been shifted out.
 */
bool ezspSerialOutputIsIdle(void);

#endif //__EZSP_HOST_H__

#if !defined(DOXYGEN_SHOULD_SKIP_THIS)
EzspStatus ezspSetupSerialPort(int* serialPortFdReturn,
                               char* errorStringLocation,
                               int maxErrorLength,
                               bool bootloaderMode);
#endif

/** @brief The classic bad file descriptor.
 */
#define NULL_FILE_DESCRIPTOR  (-1)

/** @} // END addtogroup
 */
