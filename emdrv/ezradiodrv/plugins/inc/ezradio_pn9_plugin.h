/***************************************************************************//**
 * @file
 * @brief EzRadio PN9 plug-in managed by the plug-in manager if enabled.
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

#ifndef EZRADIO_PN9_PLUGIN_H_
#define EZRADIO_PN9_PLUGIN_H_

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
 * @addtogroup PN9_Plugin
 * @brief Pseudo random transmit plugin, see @ref ezradiodrv_plugin
 *        for detailed documentation.
 * @{
 ******************************************************************************/

#if defined(EZRADIO_PLUGIN_PN9)

/// EzRadio PN9 plug-in instance initialization and handler structure.
typedef struct EZRADIODRV_Pn9Handle{
  uint8_t channel;      ///< PN9 transmit channel.
} EZRADIODRV_Pn9Handle_t;

Ecode_t ezradioStartPn9(EZRADIODRV_Handle_t radioHandle);
Ecode_t ezradioStopPn9(void);

/// Configuration data for EzRadio PN9 plug-in.
#define EZRADIODRV_PN9_PLUGIN_INIT_DEFAULT                             \
  {                             /* PN9                              */ \
    0,                          /* Channel                          */ \
  },

#else //#if defined( EZRADIO_PLUGIN_PN9 )
#define EZRADIODRV_PN9_PLUGIN_INIT_DEFAULT
#endif  //#if defined( EZRADIO_PLUGIN_PN9 )

/** @} (end addtogroup PN9_Plugin) */
/** @} (end addtogroup Plugin_System) */
/** @} (end addtogroup EZRADIODRV) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif /* EZRADIO_PN9_PLUGIN_H_ */
