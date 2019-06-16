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

#include <stddef.h>
#include "em_device.h"

#include "ezradio_cmd.h"
#include "ezradio_prop.h"
#include "ezradio_api_lib.h"
#include "ezradio_api_lib_add.h"
#include "ezradio_plugin_manager.h"

#if defined(EZRADIO_PLUGIN_AUTO_ACK) && defined(EZRADIO_PLUGIN_TRANSMIT) && defined(EZRADIO_PLUGIN_RECEIVE)

/**************************************************************************//**
 * @brief Enables auto acknowledge sending feature.
 *
 *  @param autoAckHandle Auto acknowledge plug-in handler.
 *
 *  @return
 *    @ref ECODE_EMDRV_EZRADIODRV_OK on success. On failure an appropriate EZRADIODRV
 *    @ref Ecode_t is returned.
 *****************************************************************************/
void ezradioEnableAutoAck(EZRADIODRV_AutoAckHandle_t *autoAckHandle)
{
  autoAckHandle->ackMode = ezradiodrvAutoAckImmediate;
}

/**************************************************************************//**
 * @brief Disables auto acknowledge sending feature.
 *
 *  @param autoAckHandle Auto acknowledge plug-in handler.
 *
 *  @return
 *    @ref ECODE_EMDRV_EZRADIODRV_OK on success. On failure an appropriate EZRADIODRV
 *    @ref Ecode_t is returned.
 *****************************************************************************/
void ezradioDisableAutoAck(EZRADIODRV_AutoAckHandle_t *autoAckHandle)
{
  autoAckHandle->ackMode = ezradiodrvAutoAckDisabled;
}

/**************************************************************************//**
 * @brief Configures auto acknowledge sending feature so that there will be
 *        no acknowledge transmitted to the next received packet. This is
 *        usefull is both nodes of a link uses auto acknowledgement.
 *
 *  @param autoAckHandle Auto acknowledge plug-in handler.
 *
 *  @return
 *    @ref ECODE_EMDRV_EZRADIODRV_OK on success. On failure an appropriate EZRADIODRV
 *    @ref Ecode_t is returned.
 *****************************************************************************/
void ezradioSkipAutoAck(EZRADIODRV_AutoAckHandle_t *autoAckHandle)
{
  autoAckHandle->ackMode = ezradiodrvAutoAckSkipOne;
}

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
//  Note: These are internal routines used by ezradio plugin manager.

/**************************************************************************//**
 * @brief EZRadioDrv auto acknowledge plug-in handler routine.
 *
 *  @param radioHandle EzRadio driver instance handler.
 *  @param radioReplyHandle EZRadio communication reply handler.
 *
 *  @return
 *    @ref ECODE_EMDRV_EZRADIODRV_OK on success. On failure an appropriate EZRADIODRV
 *    @ref Ecode_t is returned.
 *****************************************************************************/
Ecode_t ezradioHandleAutoAckPlugin(EZRADIODRV_Handle_t radioHandle, EZRADIODRV_ReplyHandle_t radioReplyHandle)
{
  //Silent warning.
  (void)radioReplyHandle;

  if ( radioHandle == NULL ) {
    return ECODE_EMDRV_EZRADIODRV_ILLEGAL_HANDLE;
  }

  if ( radioHandle->autoAck.userCallback != NULL ) {
    radioHandle->autoAck.userCallback(radioHandle, ECODE_EMDRV_EZRADIODRV_OK);
  }

  /* Set packet type back to normal */
  radioHandle->packetTx.pktType = ezradiodrvPacketTypeNormal;

  return ECODE_EMDRV_EZRADIODRV_OK;
}

/**************************************************************************//**
 * @brief Transmit auto acknowledge packet according the the configuration
 *        in the radioHandle parameter.
 *
 *  @param[in] radioHandle Handler of the EzRadio driver instance where packet
 *             buffer, callback and channel configurations are set.
 *
 *  @return
 *    @ref ECODE_EMDRV_EZRADIODRV_OK on success. On failure an appropriate EZRADIODRV
 *    @ref Ecode_t is returned.
 *****************************************************************************/
Ecode_t ezradioTransmitAutoAck(EZRADIODRV_Handle_t radioHandle)
{
  if ( radioHandle == NULL ) {
    return ECODE_EMDRV_EZRADIODRV_ILLEGAL_HANDLE;
  }

  /* Set packet type to auto ack */
  radioHandle->packetTx.pktType = ezradiodrvPacketTypeAutoAck;

  /* Start default or smart transmission depending on the auto ack packet mode configuration */
  if (radioHandle->autoAck.pktMode == ezradiodrvAutoAckPktDefault) {
    ezradioStartTransmitDefault(radioHandle, radioHandle->autoAck.pktBuf);
  } else {
    ezradioStartTransmitSmart(radioHandle, radioHandle->autoAck.lenConfig, radioHandle->autoAck.pktBuf);
  }

  return ECODE_EMDRV_EZRADIODRV_OK;
}

/// @endcond

#endif //#if defined( EZRADIO_PLUGIN_AUTO_ACK ) && defined( EZRADIO_PLUGIN_TRANSMIT ) && defined( EZRADIO_PLUGIN_RECEIVE )
