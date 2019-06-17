/***************************************************************************//**
 * @file
 * @brief EzRadio transmit plug-in managed by the plug-in manager if enabled.
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

#ifndef EZRADIO_RECEIVE_PLUGIN_H_
#define EZRADIO_RECEIVE_PLUGIN_H_

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
 * @addtogroup Receive_Plugin
 * @brief Receive plugin, see @ref ezradiodrv_plugin
 *        for detailed documentation.
 * @{
 ******************************************************************************/

#if defined(EZRADIO_PLUGIN_RECEIVE)

/// EzRadio receive plug-in instance initialization and handler structure.
typedef struct EZRADIODRV_PacketRxHandle{
  EZRADIODRV_Callback_t userCallback;   ///< User callback.
  uint8_t channel;                      ///< Reception channel.
  uint8_t * pktBuf;                     ///< Pointer to the receive buffer.
} EZRADIODRV_PacketRxHandle_t;

Ecode_t ezradioStartRx(EZRADIODRV_Handle_t radioHandle);

/// Configuration data for EzRadio receive plug-in.
#define EZRADIODRV_RECEIVE_PLUGIN_INIT_DEFAULT                               \
  {                                           /* Packet RX                */ \
    NULL,                                     /* CallBack                 */ \
    RADIO_CONFIGURATION_DATA_CHANNEL_NUMBER,  /* Channel                  */ \
    NULL,                                     /* Packet buffer            */ \
  },

#else //#if defined( EZRADIO_PLUGIN_RECEIVE )

#define EZRADIODRV_RECEIVE_PLUGIN_INIT_DEFAULT

#endif //#if defined( EZRADIO_PLUGIN_RECEIVE )

/** @} (end addtogroup Receive_Plugin) */
/** @} (end addtogroup Plugin_System) */
/** @} (end addtogroup EZRADIODRV) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif /* EZRADIO_RECEIVE_PLUGIN_H_ */
