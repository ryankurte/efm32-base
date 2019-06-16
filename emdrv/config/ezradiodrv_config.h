/***************************************************************************//**
 * @file
 * @brief EZRADIODRV configuration file.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#ifndef __SILICON_LABS_EZRADIODRV_CONFIG_H__
#define __SILICON_LABS_EZRADIODRV_CONFIG_H__

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup EZRADIODRV
 * @{
 ******************************************************************************/

#warning "This configuration file is deprecated and marked for removal in a later release. Please use the new configuration template file in emdrv\ezradiodrv\config instead."

/// @brief Number of wait-for-CTS retries.
#define EZRADIODRV_COMM_CTS_RETRY 10000

/// @brief Enable poll GPIO1 for CTS.
//#define EZRADIODRV_COMM_USE_GPIO1_FOR_CTS

/// @brief Define to disable packet trace interface.
//#define EZRADIODRV_DISABLE_PTI

/// @brief Define to enable 4-wire SPI communication with the radio.
///        Note: 4-wire mode cannot be used in EZR32 devices.
//#define EZRADIODRV_SPI_4WIRE_MODE

// ONly EZR32 devices support these macros by default, we need to add these for other
// devices. Uncomment & define all the following RF_* macros to support the EzRadio
// device on other EFM32 micros.

//#define RF_USARTRF_LOCATION     1     /**< RF SPI-port (USART) location number. */
//#define RF_USARTRF_CS_PORT      3     /**< Bit banged RF SPI CS GPIO port no.   */
//#define RF_USARTRF_CS_PIN       3     /**< Bit banged RF SPI CS GPIO pin number.*/
//#define RF_USARTRF_CLK_PORT     3     /**< RF SPI CLK GPIO port number.      */
//#define RF_USARTRF_CLK_PIN      2     /**< RF SPI CLK GPIO pin number.       */
//#define RF_USARTRF_MISO_PORT    3     /**< RF SPI MISO GPIO port number.     */
//#define RF_USARTRF_MISO_PIN     1     /**< RF SPI MISO GPIO pin number.      */
//#define RF_USARTRF_MOSI_PORT    3     /**< RF SPI MOSI GPIO port number.     */
//#define RF_USARTRF_MOSI_PIN     0     /**< RF SPI MOSI GPIO pin number.      */
//#define RF_INT_PORT             2     /**< RF interrupt GPIO port number.    */
//#define RF_INT_PIN              0     /**< RF interrupt GPIO pin number.     */
//#define RF_SDN_PORT             2     /**< RF SDN GPIO port number.          */
//#define RF_SDN_PIN              3     /**< RF SDN GPIO pin number.           */

#if !(defined EZRADIODRV_DISABLE_PTI)
//#define RF_GPIO0_PORT           0     /**< RF GPIO0 GPIO port number.        */
//#define RF_GPIO0_PIN            15    /**< RF GPIO0 GPIO pin number.         */
//#define RF_GPIO1_PORT           4     /**< RF GPIO1 GPIO port number.        */
//#define RF_GPIO1_PIN            14    /**< RF GPIO1 GPIO pin number.         */
#endif

/** @} (end addtogroup EZRADIODRV) */
/** @} (end addtogroup emdrv) */

#endif /* __SILICON_LABS_EZRADIODRV_CONFIG_H__ */
