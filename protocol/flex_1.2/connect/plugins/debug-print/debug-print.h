// Copyright 2014 Silicon Laboratories, Inc.

/**
 * @addtogroup debug_print
 *
 * See debug-print.h for source code.
 * @{
 */

/** @brief Indicates if a printing area is enabled. */
bool emberAfPrintEnabled(uint16_t area);

/** @brief Enables a printing area. */
void emberAfPrintOn(uint16_t userArea);

/** @brief Disables a printing area. */
void emberAfPrintOff(uint16_t userArea);

/** @brief Enables all printing areas. */
void emberAfPrintAllOn(void);

/** @brief Disables all printing areas. */
void emberAfPrintAllOff(void);

/** @brief Prints the status of the printing areas. */
void emberAfPrintStatus(void);

/** @brief Prints a formatted message. */
void emberAfPrint(uint16_t area, PGM_P formatString, ...);

/** @brief Prints a formatted message followed by a newline. */
void emberAfPrintln(uint16_t area, PGM_P formatString, ...);

/** @brief Prints a buffer as a series of bytes in hexidecimal format. */
void emberAfPrintBuffer(uint16_t area,
                        const uint8_t *buffer,
                        uint16_t bufferLen,
                        bool withSpace);

/** @brief Print an EUI64 (IEEE address) in big-endian format. */
void emberAfPrintBigEndianEui64(const EmberEUI64 eui64);

/** @brief Print an EUI64 (IEEE address) in little-endian format. */
void emberAfPrintLittleEndianEui64(const EmberEUI64 eui64);

/** @brief Prints a 16-byte key. */
void emberAfPrintKey(const uint8_t *key);

/** @brief Waits for all data currently queued to be transmitted. */
void emberAfFlush(uint16_t area);

extern uint16_t emberAfPrintActiveArea;

/** @} END addtogroup */
