/***************************************************************************//**
 * @file
 * @brief SPIDRV configuration file.
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
#ifndef __SILICON_LABS_SPIDRV_CONFIG_H__
#define __SILICON_LABS_SPIDRV_CONFIG_H__

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup SPIDRV
 * @{
 ******************************************************************************/

#warning "This configuration file is deprecated and marked for removal in a later release. Please use the new configuration template file in emdrv\spidrv\config instead."

/// SPIDRV configuration option. Use this define to include the slave part of the SPIDRV API.
#define EMDRV_SPIDRV_INCLUDE_SLAVE

/** @} (end addtogroup SPIDRV) */
/** @} (end addtogroup emdrv) */

#endif /* __SILICON_LABS_SPIDRV_CONFIG_H__ */
