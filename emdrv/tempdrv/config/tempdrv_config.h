/***************************************************************************//**
 * @file
 * @brief TEMPDRV configuration file.
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
#ifndef __SILICON_LABS_TEMPDRV_CONFIG_H__
#define __SILICON_LABS_TEMPDRV_CONFIG_H__

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup TEMPDRV
 * @{
 ******************************************************************************/

// Callback table depth (for high and low callbacks each)
#ifndef TEMPDRV_CUSTOM_CALLBACK_DEPTH
#define TEMPDRV_CUSTOM_CALLBACK_DEPTH 5
#endif

// Allow temperature sensor to wake the device up from EM4
#ifndef TEMPDRV_EM4WAKEUP
#define TEMPDRV_EM4WAKEUP false
#endif

// Allow TEMPDRV to define the EMU_IRQ_Handler. Enable if EMU_IRQ_Handler is
// defined elsewhere.
#ifndef EMU_CUSTOM_IRQ_HANDLER
#define EMU_CUSTOM_IRQ_HANDLER false
#endif

/** @} (end addtogroup TEMPDRV) */
/** @} (end addtogroup emdrv) */

#endif /* __SILICON_LABS_TEMPDRV_CONFIG_H__ */
