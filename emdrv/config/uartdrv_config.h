/***************************************************************************//**
 * @file
 * @brief UARTDRV configuration file.
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
#ifndef __SILICON_LABS_UARTDRV_CONFIG_H__
#define __SILICON_LABS_UARTDRV_CONFIG_H__

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup UARTDRV
 * @{
 ******************************************************************************/

#warning "This configuration file is deprecated and marked for removal in a later release. Please use the new configuration template file in emdrv\uartdrv\config instead."

/// Maximum concurrent receive operations
#if !defined(EMDRV_UARTDRV_MAX_CONCURRENT_RX_BUFS)
#define EMDRV_UARTDRV_MAX_CONCURRENT_RX_BUFS    6
#endif

/// Maximum concurrent transmit operations
#if !defined(EMDRV_UARTDRV_MAX_CONCURRENT_TX_BUFS)
#define EMDRV_UARTDRV_MAX_CONCURRENT_TX_BUFS    6
#endif

/// Set to 1 to include flow control support
#if !defined(EMDRV_UARTDRV_FLOW_CONTROL_ENABLE)
#define EMDRV_UARTDRV_FLOW_CONTROL_ENABLE       1
#endif

/// Maximum number of driver instances. This maximum applies only when EMDRV_UARTDRV_FLOW_CONTROL_ENABLE = 1.
#if !defined(EMDRV_UARTDRV_MAX_DRIVER_INSTANCES)
#define EMDRV_UARTDRV_MAX_DRIVER_INSTANCES      4
#endif

/// UART software flow control code: request peer to start TX
#if !defined(UARTDRV_FC_SW_XON)
#define UARTDRV_FC_SW_XON                       0x11
#endif

/// UART software flow control code: request peer to stop TX
#if !defined(UARTDRV_FC_SW_XOFF)
#define UARTDRV_FC_SW_XOFF                      0x13
#endif

/** @} (end addtogroup UARTDRV) */
/** @} (end addtogroup emdrv) */

#endif /* __SILICON_LABS_UARTDRV_CONFIG_H__ */
