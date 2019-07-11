/***************************************************************************//**
 * @file
 * @brief Board support package for Touch Display Kit
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

#ifndef __BSP_TDK_H__
#define __BSP_TDK_H__

void  BSP_initEbiFlash          (void);
void  BSP_initEbiSram           (void);
void  BSP_initEbiTftDirectDrive (void);

void *BSP_getTftSRAMDirectDriveBaseAddress (void);
void  BSP_setTftSRAMDirectDriveBaseAddress (void *baseAddressIn);

#endif /* __BSP_TDK_H__ */
