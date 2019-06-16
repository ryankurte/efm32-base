/***************************************************************************//**
 * @file
 * @brief EzRadio CRC error plug-in managed by the plug-in manager if enabled.
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

#ifndef EZRADIO_CRCERROR_PLUGIN_H_
#define EZRADIO_CRCERROR_PLUGIN_H_

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
 * @addtogroup CRC_Error_Plugin
 * @brief CRC error plugin, see @ref ezradiodrv_plugin
 *        for detailed documentation.
 * @{
 ******************************************************************************/

#if defined(EZRADIO_PLUGIN_CRC_ERROR)

/// EzRadio CRC error packet receive plug-in instance initialization and handler structure.
typedef struct EZRADIODRV_PacketCrcErrorHandle{
  EZRADIODRV_Callback_t userCallback;   ///< User callback.
} EZRADIODRV_PacketCrcErrorHandle_t;

/// Configuration data for EzRadio CRC error plug-in.
#define EZRADIODRV_CRC_ERROR_PLUGIN_INIT_DEFAULT                       \
  {                             /* Packet CRC Error                 */ \
    NULL,                       /* CallBack                         */ \
  },

#else //#if defined( EZRADIO_PLUGIN_CRC_ERROR )

#define EZRADIODRV_CRC_ERROR_PLUGIN_INIT_DEFAULT

#endif //#if defined( EZRADIO_PLUGIN_CRC_ERROR )

/** @} (end addtogroup CRC_Error_Plugin) */
/** @} (end addtogroup Plugin_System) */
/** @} (end addtogroup EZRADIODRV) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif /* EZRADIO_CRCERROR_PLUGIN_H_ */
