/***************************************************************************//**
 * @file
 * @brief This file contains the EZRadio communication layer.
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

#ifndef _EZRADIO_COMM_H_
#define _EZRADIO_COMM_H_

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
 * @addtogroup COMM_Layer
 * @brief EzRadio Communication Layer
 * @{
 ******************************************************************************/

#if defined(EZRADIODRV_SPI_4WIRE_MODE)
#define EZRADIODRV_MAX_CTS_BUFF_SIZE 257
#endif

extern uint8_t ezradio_comm_CtsWentHigh;

uint8_t ezradio_comm_GetResp(uint8_t byteCount, uint8_t* pData);
void ezradio_comm_SendCmd(uint8_t byteCount, uint8_t* pData);
void ezradio_comm_ReadData(uint8_t cmd, uint8_t pollCts, uint8_t byteCount, uint8_t* pData);
void ezradio_comm_WriteData(uint8_t cmd, uint8_t pollCts, uint8_t byteCount, uint8_t* pData);

uint8_t ezradio_comm_PollCTS(void);
uint8_t ezradio_comm_SendCmdGetResp(uint8_t cmdByteCount, uint8_t* pCmdData,
                                    uint8_t respByteCount, uint8_t* pRespData);
void ezradio_comm_ClearCTS(void);

/** @} (end addtogroup Comm_Layer) */
/** @} (end addtogroup EZRADIODRV) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#endif //_EZRADIO_COMM_H_
