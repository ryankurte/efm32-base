/***************************************************************************//**
 * @file
 * @brief EzRadio auto acknowledge plug-in managed by the plug-in manager if enabled.
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

#ifndef EZRADIO_AUTO_ACK_PLUGIN_H_
#define EZRADIO_AUTO_ACK_PLUGIN_H_

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
 * @addtogroup Auto_ACK_Plugin
 * @brief Auto acknowledge plugin, see @ref ezradiodrv_plugin
 *        for detailed documentation.
 * @{
 ******************************************************************************/

#if defined(EZRADIO_PLUGIN_AUTO_ACK) && defined(EZRADIO_PLUGIN_TRANSMIT) && defined(EZRADIO_PLUGIN_RECEIVE)

/// EzRadio auto acknowledge mode
typedef enum EZRADIODRV_AutoAckMode{
  ezradiodrvAutoAckDisabled                = 0,    ///< Auto acknowledge transmission disabled
  ezradiodrvAutoAckImmediate               = 1,    ///< Immediate acknowledge transmission
  ezradiodrvAutoAckSkipOne                 = 2,    ///< Acknowledge transmission is skipped for one cycle, eg. not to transmit ACK if ACK is received
} EZRADIODRV_AutoAckMode_t;

/// EzRadio packet mode for auto acknowledge plug-in
typedef enum EZRADIODRV_AutoAckPacketMode{
  ezradiodrvAutoAckPktDefault           = 0,    ///< Auto acknowledge uses the same transmission configuration as the actual configuration
  ezradiodrvAutoAckPktCustom            = 1,    ///< Auto acknowledge uses custom packet configuration
} EZRADIODRV_AutoAckPacketMode_t;

/// EzRadio auto acknowledge plug-in instance initialization and handler structure.
typedef struct EZRADIODRV_AutoAckHandle{
  EZRADIODRV_Callback_t   userCallback;         ///< User callback.
  EZRADIODRV_AutoAckMode_t ackMode;             ///< Auto ack mode.
  EZRADIODRV_AutoAckPacketMode_t pktMode;       ///< Packet mode for aut ack.
  EZRADIODRV_PacketLengthConfig_t lenConfig;    ///< Pakcet length config for auto ack.
  uint8_t * pktBuf;                             ///< Pointer to the buffer that holds the auto ack packet.
  uint16_t pktId;                               ///< Auto ack packet ID.
} EZRADIODRV_AutoAckHandle_t;

void ezradioEnableAutoAck(EZRADIODRV_AutoAckHandle_t *autoAckHandle);
void ezradioDisableAutoAck(EZRADIODRV_AutoAckHandle_t *autoAckHandle);
void ezradioSkipAutoAck(EZRADIODRV_AutoAckHandle_t *autoAckHandle);

/// Configuration data for EzRadio auto acknowledge plug-in.
#define EZRADIODRV_AUTO_ACK_PLUGIN_INIT_DEFAULT                        \
  {                                    /* Auto acknowledge          */ \
    NULL,                              /* CallBack                  */ \
    ezradiodrvAutoAckDisabled,         /* Ack mode                  */ \
    ezradiodrvAutoAckPktDefault,       /* Ack packet mode           */ \
    {                                  /* Packet length config      */ \
      ezradiodrvTransmitLenghtDefault, /* Default length mode       */ \
      0,                               /* Packet length             */ \
      RADIO_CONFIG_DATA_FIELD_LENGTH,  /* Field lengths             */ \
    },                                                                 \
    NULL,                              /* Packet buffer             */ \
    0,                                 /* Packet ID                 */ \
  },

#else //#if defined( EZRADIO_PLUGIN_AUTO_ACK ) && defined( EZRADIO_PLUGIN_TRANSMIT ) && defined( EZRADIO_PLUGIN_RECEIVE )

#define EZRADIODRV_AUTO_ACK_PLUGIN_INIT_DEFAULT

#endif //#if defined( EZRADIO_PLUGIN_AUTO_ACK ) && defined( EZRADIO_PLUGIN_TRANSMIT ) && defined( EZRADIO_PLUGIN_RECEIVE )

/** @} (end addtogroup Auto_ACK_Plugin) */
/** @} (end addtogroup Plugin_System) */
/** @} (end addtogroup EZRADIODRV) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif /* EZRADIO_AUTO_ACK_PLUGIN_H_ */
