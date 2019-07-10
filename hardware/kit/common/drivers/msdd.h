/***************************************************************************//**
 * @file
 * @brief Mass Storage class Device (MSD) driver.
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
#ifndef __MSDD_H
#define __MSDD_H

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

#define MEDIA_BUFSIZ    4096      /**< Intermediate media storage buffer size */

/**************************************************************************//**
 * @brief Status info for one BOT CBW -> Data I/O -> CSW cycle.
 *****************************************************************************/
typedef struct {
  bool        valid;     /**< True if the CBW is valid.    */
  uint8_t     direction; /**< Set if BOT direction is IN.  */
  uint8_t     *pData;    /**< Media data pointer.          */
  uint32_t    lba;       /**< SCSI Read/Write lba address. */
  uint32_t    xferLen;   /**< BOT transfer length.         */
  uint32_t    maxBurst;  /**< Max length of one transfer.  */
  enum { XFER_MEMORYMAPPED = 0, XFER_INDIRECT } xferType;
  /**< MSD media access type.       */
} MSDD_CmdStatus_TypeDef;

/*** MSDD Device Driver Function prototypes ***/

bool MSDD_Handler(void);
void MSDD_Init(int activityLedPort, uint32_t activityLedPin);
int  MSDD_SetupCmd(const USB_Setup_TypeDef *setup);
void MSDD_StateChangeEvent(USBD_State_TypeDef oldState,
                           USBD_State_TypeDef newState);

#ifdef __cplusplus
}
#endif

/** @} (end group Msd) */
/** @} (end group kitdrv) */

#endif /* __MSDD_H */
