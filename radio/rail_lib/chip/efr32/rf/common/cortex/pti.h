/***************************************************************************//**
 * @file pti.h
 * @brief This header file contains information for working with the packet
 * trace APIs.
 * @copyright Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __RADIO_PTI_H
#define __RADIO_PTI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#include "em_gpio.h"

// Once this is a RAIL API this code can be removed as rail-types.h does this
#ifndef RAIL_ENUM
#ifdef DOXYGEN_SHOULD_SKIP_THIS
/// The RAIL library does not use actual enums because the ARM EABI leaves their
/// size ambiguous. This ambiguity causes problems if the application is built
/// with different flags than the library. To work around this we use uint8_t
/// typedefs in compiled code for all enums. For documentation purposes this is
/// converted to an actual enum since it's much easier to read in Doxygen.
#define RAIL_ENUM(name) enum name
#else
/// Define used for the actual RAIL library which sets each enum to a uint8_t
/// typedef and creates a named enum structure for the enumeration values.
#define RAIL_ENUM(name) typedef uint8_t name; enum name##_enum
// For debugging use the following define to turn this back into a proper enum
// #define RAIL_ENUM(name) typedef enum name##_enum name; enum name##_enum
#endif
#endif

/***************************************************************************//**
 * @addtogroup Chip_Specific
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup EFR32xG1x_PTI
 * @{
 * @brief EFR32 Packet Trace Interface (PTI) setup and configuration
 ******************************************************************************/

/********************************  TYPEDEFS   *********************************/

/** Channel type enumeration. */
RAIL_ENUM(RADIO_PTIMode_t) {
  /** SPI mode. */
  RADIO_PTI_MODE_SPI = 0U,
  /** UART mode. */
  RADIO_PTI_MODE_UART = 1U,
  /** 9bit UART mode. */
  RADIO_PTI_MODE_UART_ONEWIRE = 2U,
  /** Turn PTI off entirely */
  RADIO_PTI_MODE_DISABLED = 3U,
};

/**
 * @struct RADIO_PTIInit_t
 * @brief Configuration structure for the packet trace interface (PTI)
 *
 * This structure requires information about the pins to use as well as the
 * route locations for those pins. To find the route locations consult your
 * datasheet for the appropriate values on your particular part.
 */
typedef struct RADIO_PTIInit {
  /** Packet Trace mode (UART or SPI) */
  RADIO_PTIMode_t mode;

  /** Output baudrate for PTI in Hz */
  uint32_t baud;

  /** Data output location for pin/port (FRC_DOUT) */
  uint8_t doutLoc;
  /** Data output GPIO port */
  GPIO_Port_TypeDef doutPort;
  /** Data output GPIO pin */
  uint8_t doutPin;

  /** Data clock location for pin/port. Only used in SPI mode (FRC_DCLK) */
  uint8_t dclkLoc;
  /** Data clock GPIO port. Only used in SPI mode */
  GPIO_Port_TypeDef dclkPort;
  /** Data clock GPIO pin. Only used in SPI mode */
  uint8_t dclkPin;

  /** Data frame location for pin/port (FRC_DFRAME) */
  uint8_t dframeLoc;
  /** Data frame GPIO port */
  GPIO_Port_TypeDef dframePort;
  /** Data frame GPIO pin */
  uint8_t dframePin;
} RADIO_PTIInit_t;

/*************************  FUNCTION PROTOTYPES   *****************************/

/**
 * Initialize the PTI interface
 *
 * @param ptiInit The structure that defines what pins and modes to use for
 * packet trace.
 *
 * This API will initialize the packet trace interface. It allows you to
 * configure what mode and pins to use for packet trace output. You must call
 * this API either before RAIL initialization or before an explicit call to
 * \ref RADIO_PTI_Enable() to properly initialize PTI.
 */
void RADIO_PTI_Init(RADIO_PTIInit_t *ptiInit);

/**
 * Enabled the PTI interface
 *
 * This API will turn on the packet trace interface (PTI). By default this is
 * turned on already during init time. Note that you must call \ref
 * RADIO_PTI_Init() with a valid initialization structure before calling this
 * API or PTI will not actually turn on.
 */
void RADIO_PTI_Enable(void);

/**
 * Disable the PTI interface
 *
 * This API will turn off the packet trace interface (PTI). By default this is
 * enabled while initializing RAIL. You may also turn this off by calling \ref
 * RADIO_PTI_Init() with a mode of \ref RADIO_PTI_MODE_DISABLED.
 */
void RADIO_PTI_Disable(void);

/** @} (end addtogroup EFR32xG1x_PTI) */
/** @} (end addtogroup Chip_Specific) */

#ifdef __cplusplus
}
#endif

#endif //__RADIO_PTI_H
