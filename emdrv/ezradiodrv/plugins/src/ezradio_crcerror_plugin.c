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

#include <stddef.h>
#include "em_device.h"

#include "ezradio_cmd.h"
#include "ezradio_prop.h"
#include "ezradio_api_lib.h"
#include "ezradio_api_lib_add.h"
#include "ezradio_plugin_manager.h"
#include "ezradio_receive_plugin.h"

#if defined(EZRADIO_PLUGIN_CRC_ERROR)

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN
//  Note: These are internal routines used by ezradio plugin manager.

/**************************************************************************//**
 * @brief EZRadioDrv CRC error plug-in handler routine.
 *
 *  @param radioHandle EzRadio driver instance handler.
 *  @param radioReplyHandle EZRadio communication reply handler.
 *
 *  @return
 *    @ref ECODE_EMDRV_EZRADIODRV_OK on success. On failure an appropriate EZRADIODRV
 *    @ref Ecode_t is returned.
 *****************************************************************************/
Ecode_t ezradioHandleCrcErrorPlugin(EZRADIODRV_Handle_t radioHandle, EZRADIODRV_ReplyHandle_t radioReplyHandle)
{
  if ( radioHandle == NULL ) {
    return ECODE_EMDRV_EZRADIODRV_ILLEGAL_HANDLE;
  }

  /* Check if CRC IT is received */
  if ( radioReplyHandle->GET_INT_STATUS.PH_PEND & EZRADIO_CMD_GET_INT_STATUS_REP_PH_PEND_CRC_ERROR_PEND_BIT) {
    /* Reset FIFO */
    ezradio_change_state(EZRADIO_CMD_CHANGE_STATE_ARG_NEXT_STATE1_NEW_STATE_ENUM_READY);
    ezradio_fifo_info(EZRADIO_CMD_FIFO_INFO_ARG_FIFO_RX_BIT, NULL);

    if ( radioHandle->packetCrcError.userCallback != NULL ) {
      radioHandle->packetCrcError.userCallback(radioHandle, ECODE_EMDRV_EZRADIODRV_OK);
    }
  }

  return ECODE_EMDRV_EZRADIODRV_OK;
}

/// @endcond

#endif //#if defined( EZRADIO_PLUGIN_CRC_ERROR )
