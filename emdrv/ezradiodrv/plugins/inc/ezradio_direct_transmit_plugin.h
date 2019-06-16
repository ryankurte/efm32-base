/***************************************************************************//**
 * @file
 * @brief EzRadio direct transmission plug-in managed by the plug-in manager if enabled.
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

#ifndef EZRADIO_DIRECT_TRANSMIT_PLUGIN_H_
#define EZRADIO_DIRECT_TRANSMIT_PLUGIN_H_

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup EZRADIODRV
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Plugin_System
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Direct_Transmit_Plugin
 * @brief Direct transmit plugin, see @ref ezradiodrv_plugin
 *        for detailed documentation.
 * @{
 ******************************************************************************/

#if defined(EZRADIO_PLUGIN_DIRECT_TRANSMIT)

/// EzRadio direct tx plug-in instance initialization and handler structure.
typedef struct EZRADIODRV_DirectTxHandle{
  uint8_t channel;      ///< Direct transmit channel.
} EZRADIODRV_DirectTxHandle_t;

Ecode_t ezradioStartDirectTransmit(EZRADIODRV_Handle_t radioHandle);
Ecode_t ezradioStopDirectTransmit(void);

/// Configuration data for EzRadio direct tx plug-in.
#define EZRADIODRV_DIRECT_TRANSMIT_PLUGIN_INIT_DEFAULT                 \
  {                             /* PN9                              */ \
    0,                          /* Channel                          */ \
  },

#else //#if defined( EZRADIO_PLUGIN_DIRECT_TRANSMIT )
#define EZRADIODRV_DIRECT_TRANSMIT_PLUGIN_INIT_DEFAULT
#endif  //#if defined( EZRADIO_PLUGIN_DIRECT_TRANSMIT )

/** @} (end addtogroup Direct_Transmit_Plugin) */
/** @} (end addtogroup Plugin_System) */
/** @} (end addtogroup EZRADIODRV) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif /* EZRADIO_DIRECT_TRANSMIT_PLUGIN_H_ */
