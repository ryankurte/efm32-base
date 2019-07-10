/***************************************************************************//**
 * @file
 * @brief Board support package device initialization
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
#ifndef BSP_INIT_H
#define BSP_INIT_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup BSP
 * @{
 * @addtogroup BSP_INIT Device Initialization
 * @brief Device Initialization using HAL configuration
 * @details
 *  The BSP Device Initialization APIs provide functionality to initialize the
 *  device for a specific application and board based on a HAL configuration
 *  header file. The BSP Device Initialization module provides APIs that
 *  perform
 *
 *  - EFM32/EZR32/EFR32 errata workaround
 *  - DC-to-DC converter initialization on devices with DCDC
 *  - Clock initialization for crystal oscillators and clock selection for HF
 *    and LF clock trees
 *
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @brief Initialize the device using HAL config settings
 *
 * @details Perform errata workarounds, DC-to-DC converter initialization and
 *          clock initialization and selection
 ******************************************************************************/
void BSP_initDevice(void);

/***************************************************************************//**
 * @brief Configure and initialize the DC-to-DC converter
 *
 * @details Initialize the DC-to-DC converter if the board is wired for DCDC
 *          operation. Power down the DC-to-DC converter if the board is not
 *          wired for DCDC operation.
 ******************************************************************************/
void BSP_initDcdc(void);

/***************************************************************************//**
 * @brief Configure EMU energy modes
 *
 * @details Configure EM2/3.
 ******************************************************************************/
void BSP_initEmu(void);

/***************************************************************************//**
 * @brief Initialize crystal oscillators and configure clock trees
 *
 * @details Initialize HFXO and LFXO crystal oscillators if present.
 *          Select clock sources for HF and LF clock trees.
 ******************************************************************************/
void BSP_initClocks(void);

/***************************************************************************//**
 * @brief Initialize board based on HAL configuration
 *
 * @details Initialize peripherals to communicate with board components
 ******************************************************************************/
void BSP_initBoard(void);

/** @} endgroup BSP_INIT */
/** @} endgroup BSP */

#ifdef __cplusplus
}
#endif

#endif // BSP_INIT_H
