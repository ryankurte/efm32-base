/***************************************************************************//**
 * @file
 * @brief EzRadio direct reception plug-in managed by the plug-in manager if enabled.
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
#include "ezradio_transmit_plugin.h"

#if defined(EZRADIO_PLUGIN_DIRECT_RECEIVE)

/**************************************************************************//**
 * @brief Starts direct reception with the radio configurations
 *        from the radioHandle parameter.
 *
 *  @param radioHandle Handler of the EzRadio driver instance where channel
 *         configuration is set.
 *
 *  @return
 *    @ref ECODE_EMDRV_EZRADIODRV_OK on success. On failure an appropriate EZRADIODRV
 *    @ref Ecode_t is returned.
 *****************************************************************************/
Ecode_t ezradioStartDirectReceive(EZRADIODRV_Handle_t radioHandle)
{
  if ( radioHandle == NULL ) {
    return ECODE_EMDRV_EZRADIODRV_ILLEGAL_HANDLE;
  }

  /* Read ITs, clear pending ones */
  ezradio_get_int_status(0u, 0u, 0u, NULL);

  /* Start sending packet, channel 0, START immediately, Packet n bytes long, go READY when done */
  ezradio_start_rx(radioHandle->directRx.channel, 0u, 0u,
                   EZRADIO_CMD_START_RX_ARG_NEXT_STATE1_RXTIMEOUT_STATE_ENUM_NOCHANGE,
                   EZRADIO_CMD_START_RX_ARG_NEXT_STATE2_RXVALID_STATE_ENUM_RX,
                   EZRADIO_CMD_START_RX_ARG_NEXT_STATE3_RXINVALID_STATE_ENUM_RX);

  return ECODE_EMDRV_EZRADIODRV_OK;
}

/**************************************************************************//**
 * @brief Stops direct reception.
 *
 *  @return
 *    @ref ECODE_EMDRV_EZRADIODRV_OK on success. On failure an appropriate EZRADIODRV
 *    @ref Ecode_t is returned.
 *****************************************************************************/
Ecode_t ezradioStopDirectReceive(void)
{
  ezradio_change_state(EZRADIO_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);

  return ECODE_EMDRV_EZRADIODRV_OK;
}

#endif //#if defined( EZRADIO_PLUGIN_DIRECT_RECEIVE )
