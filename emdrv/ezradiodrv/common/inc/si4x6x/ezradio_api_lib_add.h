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

#ifdef EZRADIO_DRIVER_EXTENDED_SUPPORT
/* Extended driver support functions */

#ifdef EZRADIO_DRIVER_FULL_SUPPORT
/* Full driver support functions */

void ezradio_get_adc_reading(uint8_t adc_en, uint8_t adc_cfg, ezradio_cmd_reply_t *ezradioReply);

void ezradio_ircal(uint8_t searching_step_size, uint8_t searching_rssi_avg, uint8_t rx_chain_setting1, uint8_t rx_chain_setting2);
void ezradio_ircal_manual(uint8_t ircal_amp, uint8_t ircal_ph, ezradio_cmd_reply_t *ezradioReply);

void ezradio_tx_hop(uint8_t inte, uint8_t frac2, uint8_t frac1, uint8_t frac0, uint8_t vco_cnt1, uint8_t vco_cnt0, uint8_t pll_settle_time1, uint8_t pll_settle_time0);
void ezradio_rx_hop(uint8_t inte, uint8_t frac2, uint8_t frac1, uint8_t frac0, uint8_t vco_cnt1, uint8_t vco_cnt0);

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
