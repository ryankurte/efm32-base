/***************************************************************************//**
 * @file
 * @brief Si114x types
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

#ifndef SI114X_TYPES
#define SI114X_TYPES

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup Si114x
 * @brief Silicon Labs Si114x Ultraviolet (UV) Index, Gesture, Proximity, and
 *        Ambient Light Sensor I2C driver.
 *
 * @details
 * @{
 ******************************************************************************/

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

/// @cond DOXYGEN_SHOULD_SKIP_THIS
typedef signed char       s8;
typedef signed short      s16;
typedef signed int        s32;
typedef unsigned char     u8;
typedef unsigned short    u16;
typedef unsigned int      u32;

typedef signed char       S8;
typedef signed short      S16;
typedef signed int        S32;
typedef unsigned char     U8;
typedef unsigned short    U16;
typedef unsigned int      U32;

typedef void *            HANDLE;
typedef char *            STRING;
typedef s16               PT_RESULT;
typedef s8                PT_BOOL;
/// @endcond

/**************************************************************************//**
 * @brief Si114x Sample Data Structure
 *
 * @details The SI114X_IRQ_SAMPLE Data is populated within a low-level
 * interrupt handler.
 *
 * **sequence** is not directly from the Si113x/4x, but rather,
 * the low-level interrupt handler can increment a rolling counter
 * whenever a sample is made. **sequence** is provided a convenience
 * to upper software layers.
 *
 * **timestamp** is not from the Si113x/4x directly. The low_level
 * interrupt handler is expected to use any convenient time reference
 * associated with the sample. This will allow the upper layers to
 * have a sense of time.
 *
 * **irqstat**, **vis**, **ir**, **ps1**, **ps2**, **ps3** and **aux**
 * are directly from the Si113x/4x. Beginning with **irqstat**,
 * these members are roughly in the order of I2C Registers 0x21 to 0x2D.
 *
 * @note **ps1**, **ps2** and **ps3** are not used by the Si1132.
 * @note **Si1132** reports UVINDEX in **aux**.
 *****************************************************************************/
typedef struct {
  uint16_t  sequence;         /**< sequence number               */
  uint16_t  timestamp;        /**< 16-bit Timestamp to record    */
  uint8_t   pad;              /**< 8-bit padding                 */
  uint8_t   irqstat;          /**< 8-bit IRQ Status              */
  uint16_t  vis;              /**< VIS Measurement               */
  uint16_t  ir;               /**< IR  Measurement               */
  uint16_t  ps1;              /**< PS1 Measurement               */
  uint16_t  ps2;              /**< PS2 Measurement               */
  uint16_t  ps3;              /**< PS3 Measurement               */
  uint16_t  aux;              /**< AUX Measurement               */
} SI114X_IRQ_SAMPLE;

/// @cond DOXYGEN_SHOULD_SKIP_THIS
#define code
#define xdata

typedef union uu16{
  u16 u16;
  s16 s16;
  u8  u8[2];
  s8  s8[2];
} uu16;

typedef union uu32{
  u32  u32;
  s32  s32;
  uu16 uu16[2];
  u16  u16[2];
  s16  s16[2];
  u8   u8[4];
  s8   s8[4];
} uu32;

typedef char BIT;

#ifndef TRUE
#define TRUE   0xff
#endif

#ifndef FALSE
#define FALSE  0
#endif

#ifndef NULL
#define NULL 0
#endif

/// @endcond

#ifdef __cplusplus
}
#endif

/** @} (end group Si114x) */
/** @} (end group kitdrv) */

#endif
