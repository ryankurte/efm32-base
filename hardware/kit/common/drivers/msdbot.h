/***************************************************************************//**
 * @file
 * @brief Definitions for the Bulk Only Transport protocol of
 *        USB Mass Storage devices.
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

#ifndef __MSDBOT_H
#define __MSDBOT_H

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

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#define CBW_LEN          31
#define CSW_LEN          13

#define BOT_DIR_IN       0x80                 /* bmCBWFlags value for bulk IN cmds.  */
#define BOT_DIR_OUT      0x00                 /* bmCBWFlags value for bulk OUT cmds. */

#define CSW_SIGNATURE    0x53425355           /* Ascii USBS */
#define CBW_SIGNATURE    0x43425355           /* Ascii USBC */

/** @endcond */

/**************************************************************************//**
 * @brief Bulk Only Transport (BOT) Command Block Wrapper (CBW) typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct {
  uint32_t dCBWSignature;           /**< The CBW signature (ascii USBC). */
  uint32_t dCBWTag;                 /**< The CBW tag.                    */
  uint32_t dCBWDataTransferLength;  /**< BOT transfer length.            */
  union {
    struct {
      uint8_t Reserved1 : 6;        /**< Reserved, expect 0.             */
      uint8_t Obsolete  : 1;        /**< Obsolete, expect 0.             */
      uint8_t Direction : 1;        /**< BOT transfer directon, 1=BOT IN */
    };
    uint8_t bmCBWFlags;             /**< CBW Flags.                      */
  };
  union {
    struct {
      uint8_t Lun       : 4;        /**< Device Logical Unit Number.     */
      uint8_t Reserved2 : 4;        /**< Reserved, expect 0.             */
    };
    uint8_t bCBWLUN;
  };
  union {
    struct {
      uint8_t CBLength  : 5;        /**< SCSI CDB length.                */
      uint8_t Reserved3 : 3;        /**< Reserved, expect 0.             */
    };
    uint8_t bCBWCBLength;           /**< SCSI CDB length.                */
  };
  uint8_t CBWCB[16];                /**< SCSI CDB.                       */
} SL_ATTRIBUTE_PACKED MSDBOT_CBW_TypeDef;
SL_PACK_END()

/**************************************************************************//**
 * @brief Bulk Only Transport (BOT) Command Status Wrapper (CSW) typedef.
 *****************************************************************************/
SL_PACK_START(1)
typedef struct {
  uint32_t dCSWSignature;           /**< The CSW signature (ascii USBS). */
  uint32_t dCSWTag;                 /**< The CSW tag.                    */
  uint32_t dCSWDataResidue;         /**< BOT transfer residue count.     */
  uint8_t  bCSWStatus;              /**< BOT command execution result.   */
} SL_ATTRIBUTE_PACKED MSDBOT_CSW_TypeDef;
SL_PACK_END()

/** @brief MSDBOT status enumerator. */
typedef enum {
  MSDBOT_STATUS_OK  = 0,            /**< MSDBOT status, no error.        */
  MSDBOT_INIT_ERROR = -1,           /**< MSDBOT initialization failure.  */
  MSDBOT_CMD_FAILED = -2,           /**< MSDBOT command failure.         */
  MSDBOT_XFER_ERROR = -3,           /**< MSDBOT transfer error.          */
} MSDBOT_Status_TypeDef;

/*** MSDBOT Function prototypes ***/

#if defined(USB_HOST)

int MSDBOT_Init(USBH_Ep_TypeDef *out, USBH_Ep_TypeDef *in);
int MSDBOT_Xfer(void* cbw, void* data);

#endif

#ifdef __cplusplus
}
#endif

/** @} (end group Msd) */
/** @} (end group kitdrv) */

#endif /* __MSDBOT_H */
