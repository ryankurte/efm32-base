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

#include <stddef.h>
#include "em_device.h"

#include "ezradio_cmd.h"
#include "ezradio_prop.h"
#include "ezradio_api_lib.h"
#include "ezradio_api_lib_add.h"
#include "ezradio_plugin_manager.h"
#include "ezradio_transmit_plugin.h"

#if defined(EZRADIO_PLUGIN_PN9)

/**************************************************************************//**
 * @brief Starts PN9 transmission with the radio configurations
 *        from the radioHandle parameter.
 *
 *  @param radioHandle Handler of the EzRadio driver instance where channel
 *         configuration is set.
 *
 *  @return
 *    @ref ECODE_EMDRV_EZRADIODRV_OK on success. On failure an appropriate EZRADIODRV
 *    @ref Ecode_t is returned.
 *****************************************************************************/
Ecode_t ezradioStartPn9(EZRADIODRV_Handle_t radioHandle)
{
  ezradio_cmd_reply_t ezradioReply;

  if ( radioHandle == NULL ) {
    return ECODE_EMDRV_EZRADIODRV_ILLEGAL_HANDLE;
  }

  /* Request and check radio device state */
  ezradio_request_device_state(&ezradioReply);

  if (ezradioReply.REQUEST_DEVICE_STATE.CURR_STATE == EZRADIO_CMD_REQUEST_DEVICE_STATE_REP_CURR_STATE_MAIN_STATE_ENUM_TX) {
    return ECODE_EMDRV_EZRADIODRV_TRANSMIT_FAILED;
  }

  /* Read ITs, clear pending ones */
  ezradio_get_int_status(0u, 0u, 0u, NULL);

  /* Start sending packet, channel 0, START immediately, Packet n bytes long, go READY when done */
  ezradio_start_tx(radioHandle->pn9.channel, 0u, 0u);

  return ECODE_EMDRV_EZRADIODRV_OK;
}

/**************************************************************************//**
 * @brief Stops PN9 carrier transmission.
 *
 *  @return
 *    @ref ECODE_EMDRV_EZRADIODRV_OK on success. On failure an appropriate EZRADIODRV
 *    @ref Ecode_t is returned.
 *****************************************************************************/
Ecode_t ezradioStopPn9(void)
{
  ezradio_change_state(EZRADIO_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);

  return ECODE_EMDRV_EZRADIODRV_OK;
}

#endif //#if defined( EZRADIO_PLUGIN_PN9 )
