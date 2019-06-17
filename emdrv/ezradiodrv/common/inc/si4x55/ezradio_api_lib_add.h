/***************************************************************************//**
 * @file
 * @brief This file contains the additional API library for the listed members of
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

#ifndef _EZRADIO_API_LIB_ADD_H_
#define _EZRADIO_API_LIB_ADD_H_

#include "ezradio_api_lib.h"

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
 * @addtogroup API_Layer
 * @{
 ******************************************************************************/

EZRADIO_ConfigRet_t ezradio_configuration_init(const uint8_t* pSetPropCmd);

void ezradio_write_ezconfig_array(uint8_t numBytes, uint8_t* pEzConfigArray);
void ezradio_ezconfig_check(uint16_t checksum, ezradio_cmd_reply_t *ezradioReply);

#ifdef EZRADIO_DRIVER_EXTENDED_SUPPORT
/* Extended driver support functions */

#ifdef EZRADIO_DRIVER_FULL_SUPPORT
/* Full driver support functions */

void ezradio_get_adc_reading(uint8_t adc_en, uint8_t adc_cfg, ezradio_cmd_reply_t *ezradioReply);

#endif /* EZRADIO_DRIVER_FULL_SUPPORT */
#endif /* EZRADIO_DRIVER_EXTENDED_SUPPORT */

/** @} (end addtogroup API_Layer) */
/** @} (end addtogroup EZRADIODRV) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#else //_EZRADIO_API_LIB_ADD_H_
#error Additional EZRadio API library is already defined.
#endif //_EZRADIO_API_LIB_ADD_H_
