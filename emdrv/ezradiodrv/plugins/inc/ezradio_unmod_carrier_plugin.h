/***************************************************************************//**
 * @file
 * @brief EzRadio unmodulated carrier plug-in managed by the plug-in manager if enabled.
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

#ifndef EZRADIO_UNMOD_CARRIER_PLUGIN_H_
#define EZRADIO_UNMOD_CARRIER_PLUGIN_H_

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
 * @addtogroup Unmod_Carrier_Plugin
 * @brief Unmodulated Carrier plugin, see @ref ezradiodrv_plugin
 *        for detailed documentation.
 * @{
 ******************************************************************************/

#if defined(EZRADIO_PLUGIN_UNMOD_CARRIER)

/// EzRadio unmodulated carrier plug-in instance initialization and handler structure.
typedef struct EZRADIODRV_UnmodCarrierHandle{
  uint8_t channel;      ///< Unmodulated carrier transmit channel.
} EZRADIODRV_UnmodCarrierHandle_t;

Ecode_t ezradioStartUnmodulatedCarrier(EZRADIODRV_Handle_t radioHandle);
Ecode_t ezradioStopUnmodulatedCarrier(void);

/// Configuration data for EzRadio unmodulated carrier plug-in.
#define EZRADIODRV_UNMOD_CARRIER_PLUGIN_INIT_DEFAULT                   \
  {                             /* Unmodulated Carrier              */ \
    0,                          /* Channel                          */ \
  },

#else //#if defined( EZRADIO_PLUGIN_UNMOD_CARRIER )
#define EZRADIODRV_UNMOD_CARRIER_PLUGIN_INIT_DEFAULT
#endif  //#if defined( EZRADIO_PLUGIN_UNMOD_CARRIER )

/** @} (end addtogroup Unmod_Carrier_Plugin) */
/** @} (end addtogroup Plugin_System) */
/** @} (end addtogroup EZRADIODRV) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif /* EZRADIO_UNMOD_CARRIER_PLUGIN_H_ */
