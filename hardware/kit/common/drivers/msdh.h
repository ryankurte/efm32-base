/***************************************************************************//**
 * @file
 * @brief Host side implementation of Mass Storage class Device (MSD) interface.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef __MSDH_H
#define __MSDH_H

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Msd
 * @{
 ******************************************************************************/

#ifdef __cplusplus
extern "C" {
#endif

/*** MSDH Function prototypes ***/

bool MSDH_Init(uint8_t *usbDeviceInfo, int usbDeviceInfoSize);
bool MSDH_GetSectorCount(uint32_t *sectorCount);
bool MSDH_GetSectorSize(uint16_t *sectorSize);
bool MSDH_GetBlockSize(uint32_t *blockSize);
bool MSDH_ReadSectors(uint32_t lba, uint16_t sectors, void *data);
bool MSDH_WriteSectors(uint32_t lba, uint16_t sectors, const void *data);

#ifdef __cplusplus
}
#endif

/** @} (end group Msd) */
/** @} (end group kitdrv) */

#endif /* __MSDH_H */
