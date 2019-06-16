/***************************************************************************//**
 * @file
 * @brief Non-Volatile Memory Wear-Leveling driver HAL
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

#ifndef __NVMHAL_H
#define __NVMHAL_H

#include "em_device.h"
#include <stdbool.h>
#include "nvm.h"
#include "ecode.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 *****************************   PROTOTYPES   **********************************
 ******************************************************************************/

void NVMHAL_Init(void);
void NVMHAL_DeInit(void);
void NVMHAL_Read(uint8_t *pAddress, void *pObject, uint16_t len);
Ecode_t NVMHAL_Write(uint8_t *pAddress, void const *pObject, uint16_t len);
Ecode_t NVMHAL_PageErase(uint8_t *pAddress);
void NVMHAL_Checksum(uint16_t *checksum, void *pMemory, uint16_t len);

#ifdef __cplusplus
}
#endif

#endif /* __NVMHAL_H */
