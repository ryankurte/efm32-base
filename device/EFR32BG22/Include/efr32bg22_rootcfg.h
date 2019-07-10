/**************************************************************************//**
 * @file
 * @brief EFR32BG22 ROOTCFG register and bit field definitions
 * @version 5.8.0
 ******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories, Inc. www.silabs.com</b>
 ******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 *****************************************************************************/
#ifndef EFR32BG22_ROOTCFG_H
#define EFR32BG22_ROOTCFG_H
#define ROOTCFG_HAS_SET_CLEAR

/**************************************************************************//**
* @addtogroup Parts
* @{
******************************************************************************/
/**************************************************************************//**
 * @defgroup EFR32BG22_ROOTCFG ROOTCFG
 * @{
 * @brief EFR32BG22 ROOTCFG Register Declaration.
 *****************************************************************************/

/** ROOTCFG BOOTREGS Register Group Declaration. */
typedef struct {
  __IOM uint32_t BOOTREG;                            /**< BOOTREG{r}                                         */
} ROOTCFG_BOOTREGS_TypeDef;

/** ROOTCFG Register Declaration. */
typedef struct {
  __IM uint32_t            IPVERSION;           /**< IP VERSION                                         */
  uint32_t                 RESERVED0[1U];       /**< Reserved for future use                      */
  __IOM uint32_t           EN;                  /**< Module Enable Register                             */
  uint32_t                 RESERVED1[61U];      /**< Reserved for future use*/
  __IOM uint32_t           BUSLOCK;             /**< Bus Lock                                           */
  __IOM uint32_t           REGLOCK;             /**< Register Lock                                      */
  __IOM uint32_t           MFRLOCK;             /**< Manufacture Lock                                   */
  __IOM uint32_t           ROOTMODELOCK;        /**< Root Mode Lock                                     */
  uint32_t                 RESERVED2[4U];       /**< Reserved for future use                      */
  __IOM uint32_t           ROOTDBGLOCK;         /**< Root Debug Lock                                    */
  uint32_t                 RESERVED3[7U];       /**< Reserved for future use                      */
  __IOM uint32_t           USERDBGLOCK;         /**< Host Invasive Debug Lock                           */
  __IOM uint32_t           USERNIDLOCK;         /**< Host Non-Invasive Debug Lock                       */
  __IOM uint32_t           USERSPIDLOCK;        /**< Host Secure Invasive Debug Lock                    */
  __IOM uint32_t           USERSPNIDLOCK;       /**< Host Secure Non-Invasive Debug Lock                */
  __IOM uint32_t           USERDBGAPLOCK;       /**< Host Debug Access Port Lock                        */
  uint32_t                 RESERVED4[3U];       /**< Reserved for future use                      */
  __IOM uint32_t           RADIODBGLOCK;        /**< RADIO Debug Lock                                   */
  uint32_t                 RESERVED5[5U];       /**< Reserved for future use                      */
  __IM uint32_t            MFRLOCKVAL0;         /**< Manufacture Lock Value 0                           */
  __IM uint32_t            MFRLOCKVAL1;         /**< Manufacture Lock Value 1                           */
  __IM uint32_t            LOCKSTATUS;          /**< Lock Status                                        */
  __IM uint32_t            UNLOCKSTATUS;        /**< Unlock Status                                      */
  uint32_t                 RESERVED6[30U];      /**< Reserved for future use*/
  __IOM uint32_t           CTRL;                /**< control signals for SE                             */
  __IM uint32_t            STATUS;              /**< Status register for SE                             */
  uint32_t                 RESERVED7[62U];      /**< Reserved for future use*/
  ROOTCFG_BOOTREGS_TypeDef BOOTREGS[4U];        /**<                                                   */
  uint32_t                 RESERVED8[828U];     /**< Reserved for future use*/
  __IM uint32_t            IPVERSION_SET;       /**< IP VERSION                                         */
  uint32_t                 RESERVED9[1U];       /**< Reserved for future use                      */
  __IOM uint32_t           EN_SET;              /**< Module Enable Register                             */
  uint32_t                 RESERVED10[61U];     /**< Reserved for future use*/
  __IOM uint32_t           BUSLOCK_SET;         /**< Bus Lock                                           */
  __IOM uint32_t           REGLOCK_SET;         /**< Register Lock                                      */
  __IOM uint32_t           MFRLOCK_SET;         /**< Manufacture Lock                                   */
  __IOM uint32_t           ROOTMODELOCK_SET;    /**< Root Mode Lock                                     */
  uint32_t                 RESERVED11[4U];      /**< Reserved for future use                      */
  __IOM uint32_t           ROOTDBGLOCK_SET;     /**< Root Debug Lock                                    */
  uint32_t                 RESERVED12[7U];      /**< Reserved for future use                      */
  __IOM uint32_t           USERDBGLOCK_SET;     /**< Host Invasive Debug Lock                           */
  __IOM uint32_t           USERNIDLOCK_SET;     /**< Host Non-Invasive Debug Lock                       */
  __IOM uint32_t           USERSPIDLOCK_SET;    /**< Host Secure Invasive Debug Lock                    */
  __IOM uint32_t           USERSPNIDLOCK_SET;   /**< Host Secure Non-Invasive Debug Lock                */
  __IOM uint32_t           USERDBGAPLOCK_SET;   /**< Host Debug Access Port Lock                        */
  uint32_t                 RESERVED13[3U];      /**< Reserved for future use                      */
  __IOM uint32_t           RADIODBGLOCK_SET;    /**< RADIO Debug Lock                                   */
  uint32_t                 RESERVED14[5U];      /**< Reserved for future use                      */
  __IM uint32_t            MFRLOCKVAL0_SET;     /**< Manufacture Lock Value 0                           */
  __IM uint32_t            MFRLOCKVAL1_SET;     /**< Manufacture Lock Value 1                           */
  __IM uint32_t            LOCKSTATUS_SET;      /**< Lock Status                                        */
  __IM uint32_t            UNLOCKSTATUS_SET;    /**< Unlock Status                                      */
  uint32_t                 RESERVED15[30U];     /**< Reserved for future use*/
  __IOM uint32_t           CTRL_SET;            /**< control signals for SE                             */
  __IM uint32_t            STATUS_SET;          /**< Status register for SE                             */
  uint32_t                 RESERVED16[62U];     /**< Reserved for future use*/
  ROOTCFG_BOOTREGS_TypeDef BOOTREGS_SET[4U];    /**<                                                   */
  uint32_t                 RESERVED17[828U];    /**< Reserved for future use*/
  __IM uint32_t            IPVERSION_CLR;       /**< IP VERSION                                         */
  uint32_t                 RESERVED18[1U];      /**< Reserved for future use                      */
  __IOM uint32_t           EN_CLR;              /**< Module Enable Register                             */
  uint32_t                 RESERVED19[61U];     /**< Reserved for future use*/
  __IOM uint32_t           BUSLOCK_CLR;         /**< Bus Lock                                           */
  __IOM uint32_t           REGLOCK_CLR;         /**< Register Lock                                      */
  __IOM uint32_t           MFRLOCK_CLR;         /**< Manufacture Lock                                   */
  __IOM uint32_t           ROOTMODELOCK_CLR;    /**< Root Mode Lock                                     */
  uint32_t                 RESERVED20[4U];      /**< Reserved for future use                      */
  __IOM uint32_t           ROOTDBGLOCK_CLR;     /**< Root Debug Lock                                    */
  uint32_t                 RESERVED21[7U];      /**< Reserved for future use                      */
  __IOM uint32_t           USERDBGLOCK_CLR;     /**< Host Invasive Debug Lock                           */
  __IOM uint32_t           USERNIDLOCK_CLR;     /**< Host Non-Invasive Debug Lock                       */
  __IOM uint32_t           USERSPIDLOCK_CLR;    /**< Host Secure Invasive Debug Lock                    */
  __IOM uint32_t           USERSPNIDLOCK_CLR;   /**< Host Secure Non-Invasive Debug Lock                */
  __IOM uint32_t           USERDBGAPLOCK_CLR;   /**< Host Debug Access Port Lock                        */
  uint32_t                 RESERVED22[3U];      /**< Reserved for future use                      */
  __IOM uint32_t           RADIODBGLOCK_CLR;    /**< RADIO Debug Lock                                   */
  uint32_t                 RESERVED23[5U];      /**< Reserved for future use                      */
  __IM uint32_t            MFRLOCKVAL0_CLR;     /**< Manufacture Lock Value 0                           */
  __IM uint32_t            MFRLOCKVAL1_CLR;     /**< Manufacture Lock Value 1                           */
  __IM uint32_t            LOCKSTATUS_CLR;      /**< Lock Status                                        */
  __IM uint32_t            UNLOCKSTATUS_CLR;    /**< Unlock Status                                      */
  uint32_t                 RESERVED24[30U];     /**< Reserved for future use*/
  __IOM uint32_t           CTRL_CLR;            /**< control signals for SE                             */
  __IM uint32_t            STATUS_CLR;          /**< Status register for SE                             */
  uint32_t                 RESERVED25[62U];     /**< Reserved for future use*/
  ROOTCFG_BOOTREGS_TypeDef BOOTREGS_CLR[4U];    /**<                                                   */
  uint32_t                 RESERVED26[828U];    /**< Reserved for future use*/
  __IM uint32_t            IPVERSION_TGL;       /**< IP VERSION                                         */
  uint32_t                 RESERVED27[1U];      /**< Reserved for future use                      */
  __IOM uint32_t           EN_TGL;              /**< Module Enable Register                             */
  uint32_t                 RESERVED28[61U];     /**< Reserved for future use*/
  __IOM uint32_t           BUSLOCK_TGL;         /**< Bus Lock                                           */
  __IOM uint32_t           REGLOCK_TGL;         /**< Register Lock                                      */
  __IOM uint32_t           MFRLOCK_TGL;         /**< Manufacture Lock                                   */
  __IOM uint32_t           ROOTMODELOCK_TGL;    /**< Root Mode Lock                                     */
  uint32_t                 RESERVED29[4U];      /**< Reserved for future use                      */
  __IOM uint32_t           ROOTDBGLOCK_TGL;     /**< Root Debug Lock                                    */
  uint32_t                 RESERVED30[7U];      /**< Reserved for future use                      */
  __IOM uint32_t           USERDBGLOCK_TGL;     /**< Host Invasive Debug Lock                           */
  __IOM uint32_t           USERNIDLOCK_TGL;     /**< Host Non-Invasive Debug Lock                       */
  __IOM uint32_t           USERSPIDLOCK_TGL;    /**< Host Secure Invasive Debug Lock                    */
  __IOM uint32_t           USERSPNIDLOCK_TGL;   /**< Host Secure Non-Invasive Debug Lock                */
  __IOM uint32_t           USERDBGAPLOCK_TGL;   /**< Host Debug Access Port Lock                        */
  uint32_t                 RESERVED31[3U];      /**< Reserved for future use                      */
  __IOM uint32_t           RADIODBGLOCK_TGL;    /**< RADIO Debug Lock                                   */
  uint32_t                 RESERVED32[5U];      /**< Reserved for future use                      */
  __IM uint32_t            MFRLOCKVAL0_TGL;     /**< Manufacture Lock Value 0                           */
  __IM uint32_t            MFRLOCKVAL1_TGL;     /**< Manufacture Lock Value 1                           */
  __IM uint32_t            LOCKSTATUS_TGL;      /**< Lock Status                                        */
  __IM uint32_t            UNLOCKSTATUS_TGL;    /**< Unlock Status                                      */
  uint32_t                 RESERVED33[30U];     /**< Reserved for future use*/
  __IOM uint32_t           CTRL_TGL;            /**< control signals for SE                             */
  __IM uint32_t            STATUS_TGL;          /**< Status register for SE                             */
  uint32_t                 RESERVED34[62U];     /**< Reserved for future use*/
  ROOTCFG_BOOTREGS_TypeDef BOOTREGS_TGL[4U];    /**<                                                   */
} ROOTCFG_TypeDef;
/** @} End of group EFR32BG22_ROOTCFG */

/**************************************************************************//**
 * @addtogroup EFR32BG22_ROOTCFG
 * @{
 * @defgroup EFR32BG22_ROOTCFG_BitFields ROOTCFG Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for ROOTCFG IPVERSION */
#define _ROOTCFG_IPVERSION_RESETVALUE                      0x00000000UL                                /**< Default value for ROOTCFG_IPVERSION         */
#define _ROOTCFG_IPVERSION_MASK                            0xFFFFFFFFUL                                /**< Mask for ROOTCFG_IPVERSION                  */
#define _ROOTCFG_IPVERSION_IPVERSION_SHIFT                 0                                           /**< Shift value for ROOTCFG_IPVERSION           */
#define _ROOTCFG_IPVERSION_IPVERSION_MASK                  0xFFFFFFFFUL                                /**< Bit mask for ROOTCFG_IPVERSION              */
#define _ROOTCFG_IPVERSION_IPVERSION_DEFAULT               0x00000000UL                                /**< Mode DEFAULT for ROOTCFG_IPVERSION          */
#define ROOTCFG_IPVERSION_IPVERSION_DEFAULT                (_ROOTCFG_IPVERSION_IPVERSION_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_IPVERSION  */

/* Bit fields for ROOTCFG EN */
#define _ROOTCFG_EN_RESETVALUE                             0x00000000UL                  /**< Default value for ROOTCFG_EN                */
#define _ROOTCFG_EN_MASK                                   0x00000001UL                  /**< Mask for ROOTCFG_EN                         */
#define ROOTCFG_EN_EN                                      (0x1UL << 0)                  /**< ROOTCFG Enable                              */
#define _ROOTCFG_EN_EN_SHIFT                               0                             /**< Shift value for ROOTCFG_EN                  */
#define _ROOTCFG_EN_EN_MASK                                0x1UL                         /**< Bit mask for ROOTCFG_EN                     */
#define _ROOTCFG_EN_EN_DEFAULT                             0x00000000UL                  /**< Mode DEFAULT for ROOTCFG_EN                 */
#define ROOTCFG_EN_EN_DEFAULT                              (_ROOTCFG_EN_EN_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_EN         */

/* Bit fields for ROOTCFG BUSLOCK */
#define _ROOTCFG_BUSLOCK_RESETVALUE                        0x00000000UL                            /**< Default value for ROOTCFG_BUSLOCK           */
#define _ROOTCFG_BUSLOCK_MASK                              0x0000FFFFUL                            /**< Mask for ROOTCFG_BUSLOCK                    */
#define _ROOTCFG_BUSLOCK_LOCKKEY_SHIFT                     0                                       /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_BUSLOCK_LOCKKEY_MASK                      0xFFFFUL                                /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_BUSLOCK_LOCKKEY_DEFAULT                   0x00000000UL                            /**< Mode DEFAULT for ROOTCFG_BUSLOCK            */
#define _ROOTCFG_BUSLOCK_LOCKKEY_LOCK                      0x00000000UL                            /**< Mode LOCK for ROOTCFG_BUSLOCK                */
#define _ROOTCFG_BUSLOCK_LOCKKEY_UNLOCK                    0x00009FFAUL                            /**< Mode UNLOCK for ROOTCFG_BUSLOCK              */
#define ROOTCFG_BUSLOCK_LOCKKEY_DEFAULT                    (_ROOTCFG_BUSLOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_BUSLOCK    */
#define ROOTCFG_BUSLOCK_LOCKKEY_LOCK                       (_ROOTCFG_BUSLOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_BUSLOCK        */
#define ROOTCFG_BUSLOCK_LOCKKEY_UNLOCK                     (_ROOTCFG_BUSLOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_BUSLOCK      */

/* Bit fields for ROOTCFG REGLOCK */
#define _ROOTCFG_REGLOCK_RESETVALUE                        0x00000000UL                            /**< Default value for ROOTCFG_REGLOCK           */
#define _ROOTCFG_REGLOCK_MASK                              0x0000FFFFUL                            /**< Mask for ROOTCFG_REGLOCK                    */
#define _ROOTCFG_REGLOCK_LOCKKEY_SHIFT                     0                                       /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_REGLOCK_LOCKKEY_MASK                      0xFFFFUL                                /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_REGLOCK_LOCKKEY_DEFAULT                   0x00000000UL                            /**< Mode DEFAULT for ROOTCFG_REGLOCK            */
#define _ROOTCFG_REGLOCK_LOCKKEY_LOCK                      0x00000000UL                            /**< Mode LOCK for ROOTCFG_REGLOCK                */
#define _ROOTCFG_REGLOCK_LOCKKEY_UNLOCK                    0x00008FD0UL                            /**< Mode UNLOCK for ROOTCFG_REGLOCK              */
#define ROOTCFG_REGLOCK_LOCKKEY_DEFAULT                    (_ROOTCFG_REGLOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_REGLOCK    */
#define ROOTCFG_REGLOCK_LOCKKEY_LOCK                       (_ROOTCFG_REGLOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_REGLOCK        */
#define ROOTCFG_REGLOCK_LOCKKEY_UNLOCK                     (_ROOTCFG_REGLOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_REGLOCK      */

/* Bit fields for ROOTCFG MFRLOCK */
#define _ROOTCFG_MFRLOCK_RESETVALUE                        0x00000000UL                            /**< Default value for ROOTCFG_MFRLOCK           */
#define _ROOTCFG_MFRLOCK_MASK                              0x0000FFFFUL                            /**< Mask for ROOTCFG_MFRLOCK                    */
#define _ROOTCFG_MFRLOCK_LOCKKEY_SHIFT                     0                                       /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_MFRLOCK_LOCKKEY_MASK                      0xFFFFUL                                /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_MFRLOCK_LOCKKEY_DEFAULT                   0x00000000UL                            /**< Mode DEFAULT for ROOTCFG_MFRLOCK            */
#define _ROOTCFG_MFRLOCK_LOCKKEY_LOCK                      0x00000000UL                            /**< Mode LOCK for ROOTCFG_MFRLOCK                */
#define _ROOTCFG_MFRLOCK_LOCKKEY_UNLOCK                    0x0000F50DUL                            /**< Mode UNLOCK for ROOTCFG_MFRLOCK              */
#define ROOTCFG_MFRLOCK_LOCKKEY_DEFAULT                    (_ROOTCFG_MFRLOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_MFRLOCK    */
#define ROOTCFG_MFRLOCK_LOCKKEY_LOCK                       (_ROOTCFG_MFRLOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_MFRLOCK        */
#define ROOTCFG_MFRLOCK_LOCKKEY_UNLOCK                     (_ROOTCFG_MFRLOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_MFRLOCK      */

/* Bit fields for ROOTCFG ROOTMODELOCK */
#define _ROOTCFG_ROOTMODELOCK_RESETVALUE                   0x00000000UL                                 /**< Default value for ROOTCFG_ROOTMODELOCK      */
#define _ROOTCFG_ROOTMODELOCK_MASK                         0x0000FFFFUL                                 /**< Mask for ROOTCFG_ROOTMODELOCK               */
#define _ROOTCFG_ROOTMODELOCK_LOCKKEY_SHIFT                0                                            /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_ROOTMODELOCK_LOCKKEY_MASK                 0xFFFFUL                                     /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_ROOTMODELOCK_LOCKKEY_DEFAULT              0x00000000UL                                 /**< Mode DEFAULT for ROOTCFG_ROOTMODELOCK       */
#define _ROOTCFG_ROOTMODELOCK_LOCKKEY_LOCK                 0x00000000UL                                 /**< Mode LOCK for ROOTCFG_ROOTMODELOCK           */
#define _ROOTCFG_ROOTMODELOCK_LOCKKEY_UNLOCK               0x00001E98UL                                 /**< Mode UNLOCK for ROOTCFG_ROOTMODELOCK         */
#define ROOTCFG_ROOTMODELOCK_LOCKKEY_DEFAULT               (_ROOTCFG_ROOTMODELOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_ROOTMODELOCK*/
#define ROOTCFG_ROOTMODELOCK_LOCKKEY_LOCK                  (_ROOTCFG_ROOTMODELOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_ROOTMODELOCK   */
#define ROOTCFG_ROOTMODELOCK_LOCKKEY_UNLOCK                (_ROOTCFG_ROOTMODELOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_ROOTMODELOCK */

/* Bit fields for ROOTCFG ROOTDBGLOCK */
#define _ROOTCFG_ROOTDBGLOCK_RESETVALUE                    0x00000000UL                                /**< Default value for ROOTCFG_ROOTDBGLOCK       */
#define _ROOTCFG_ROOTDBGLOCK_MASK                          0x0000FFFFUL                                /**< Mask for ROOTCFG_ROOTDBGLOCK                */
#define _ROOTCFG_ROOTDBGLOCK_LOCKKEY_SHIFT                 0                                           /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_ROOTDBGLOCK_LOCKKEY_MASK                  0xFFFFUL                                    /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_ROOTDBGLOCK_LOCKKEY_DEFAULT               0x00000000UL                                /**< Mode DEFAULT for ROOTCFG_ROOTDBGLOCK        */
#define _ROOTCFG_ROOTDBGLOCK_LOCKKEY_LOCK                  0x00000000UL                                /**< Mode LOCK for ROOTCFG_ROOTDBGLOCK            */
#define _ROOTCFG_ROOTDBGLOCK_LOCKKEY_UNLOCK                0x00007267UL                                /**< Mode UNLOCK for ROOTCFG_ROOTDBGLOCK          */
#define ROOTCFG_ROOTDBGLOCK_LOCKKEY_DEFAULT                (_ROOTCFG_ROOTDBGLOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_ROOTDBGLOCK*/
#define ROOTCFG_ROOTDBGLOCK_LOCKKEY_LOCK                   (_ROOTCFG_ROOTDBGLOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_ROOTDBGLOCK    */
#define ROOTCFG_ROOTDBGLOCK_LOCKKEY_UNLOCK                 (_ROOTCFG_ROOTDBGLOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_ROOTDBGLOCK  */

/* Bit fields for ROOTCFG USERDBGLOCK */
#define _ROOTCFG_USERDBGLOCK_RESETVALUE                    0x00000000UL                                /**< Default value for ROOTCFG_USERDBGLOCK       */
#define _ROOTCFG_USERDBGLOCK_MASK                          0x0000FFFFUL                                /**< Mask for ROOTCFG_USERDBGLOCK                */
#define _ROOTCFG_USERDBGLOCK_LOCKKEY_SHIFT                 0                                           /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_USERDBGLOCK_LOCKKEY_MASK                  0xFFFFUL                                    /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_USERDBGLOCK_LOCKKEY_DEFAULT               0x00000000UL                                /**< Mode DEFAULT for ROOTCFG_USERDBGLOCK        */
#define _ROOTCFG_USERDBGLOCK_LOCKKEY_LOCK                  0x00000000UL                                /**< Mode LOCK for ROOTCFG_USERDBGLOCK            */
#define _ROOTCFG_USERDBGLOCK_LOCKKEY_UNLOCK                0x0000076FUL                                /**< Mode UNLOCK for ROOTCFG_USERDBGLOCK          */
#define ROOTCFG_USERDBGLOCK_LOCKKEY_DEFAULT                (_ROOTCFG_USERDBGLOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_USERDBGLOCK*/
#define ROOTCFG_USERDBGLOCK_LOCKKEY_LOCK                   (_ROOTCFG_USERDBGLOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_USERDBGLOCK    */
#define ROOTCFG_USERDBGLOCK_LOCKKEY_UNLOCK                 (_ROOTCFG_USERDBGLOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_USERDBGLOCK  */

/* Bit fields for ROOTCFG USERNIDLOCK */
#define _ROOTCFG_USERNIDLOCK_RESETVALUE                    0x00000000UL                                /**< Default value for ROOTCFG_USERNIDLOCK       */
#define _ROOTCFG_USERNIDLOCK_MASK                          0x0000FFFFUL                                /**< Mask for ROOTCFG_USERNIDLOCK                */
#define _ROOTCFG_USERNIDLOCK_LOCKKEY_SHIFT                 0                                           /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_USERNIDLOCK_LOCKKEY_MASK                  0xFFFFUL                                    /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_USERNIDLOCK_LOCKKEY_DEFAULT               0x00000000UL                                /**< Mode DEFAULT for ROOTCFG_USERNIDLOCK        */
#define _ROOTCFG_USERNIDLOCK_LOCKKEY_LOCK                  0x00000000UL                                /**< Mode LOCK for ROOTCFG_USERNIDLOCK            */
#define _ROOTCFG_USERNIDLOCK_LOCKKEY_UNLOCK                0x00004C3DUL                                /**< Mode UNLOCK for ROOTCFG_USERNIDLOCK          */
#define ROOTCFG_USERNIDLOCK_LOCKKEY_DEFAULT                (_ROOTCFG_USERNIDLOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_USERNIDLOCK*/
#define ROOTCFG_USERNIDLOCK_LOCKKEY_LOCK                   (_ROOTCFG_USERNIDLOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_USERNIDLOCK    */
#define ROOTCFG_USERNIDLOCK_LOCKKEY_UNLOCK                 (_ROOTCFG_USERNIDLOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_USERNIDLOCK  */

/* Bit fields for ROOTCFG USERSPIDLOCK */
#define _ROOTCFG_USERSPIDLOCK_RESETVALUE                   0x00000000UL                                 /**< Default value for ROOTCFG_USERSPIDLOCK      */
#define _ROOTCFG_USERSPIDLOCK_MASK                         0x0000FFFFUL                                 /**< Mask for ROOTCFG_USERSPIDLOCK               */
#define _ROOTCFG_USERSPIDLOCK_LOCKKEY_SHIFT                0                                            /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_USERSPIDLOCK_LOCKKEY_MASK                 0xFFFFUL                                     /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_USERSPIDLOCK_LOCKKEY_DEFAULT              0x00000000UL                                 /**< Mode DEFAULT for ROOTCFG_USERSPIDLOCK       */
#define _ROOTCFG_USERSPIDLOCK_LOCKKEY_LOCK                 0x00000000UL                                 /**< Mode LOCK for ROOTCFG_USERSPIDLOCK           */
#define _ROOTCFG_USERSPIDLOCK_LOCKKEY_UNLOCK               0x00004C3DUL                                 /**< Mode UNLOCK for ROOTCFG_USERSPIDLOCK         */
#define ROOTCFG_USERSPIDLOCK_LOCKKEY_DEFAULT               (_ROOTCFG_USERSPIDLOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_USERSPIDLOCK*/
#define ROOTCFG_USERSPIDLOCK_LOCKKEY_LOCK                  (_ROOTCFG_USERSPIDLOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_USERSPIDLOCK   */
#define ROOTCFG_USERSPIDLOCK_LOCKKEY_UNLOCK                (_ROOTCFG_USERSPIDLOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_USERSPIDLOCK */

/* Bit fields for ROOTCFG USERSPNIDLOCK */
#define _ROOTCFG_USERSPNIDLOCK_RESETVALUE                  0x00000000UL                                  /**< Default value for ROOTCFG_USERSPNIDLOCK     */
#define _ROOTCFG_USERSPNIDLOCK_MASK                        0x0000FFFFUL                                  /**< Mask for ROOTCFG_USERSPNIDLOCK              */
#define _ROOTCFG_USERSPNIDLOCK_LOCKKEY_SHIFT               0                                             /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_USERSPNIDLOCK_LOCKKEY_MASK                0xFFFFUL                                      /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_USERSPNIDLOCK_LOCKKEY_DEFAULT             0x00000000UL                                  /**< Mode DEFAULT for ROOTCFG_USERSPNIDLOCK      */
#define _ROOTCFG_USERSPNIDLOCK_LOCKKEY_LOCK                0x00000000UL                                  /**< Mode LOCK for ROOTCFG_USERSPNIDLOCK          */
#define _ROOTCFG_USERSPNIDLOCK_LOCKKEY_UNLOCK              0x00004C3DUL                                  /**< Mode UNLOCK for ROOTCFG_USERSPNIDLOCK        */
#define ROOTCFG_USERSPNIDLOCK_LOCKKEY_DEFAULT              (_ROOTCFG_USERSPNIDLOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_USERSPNIDLOCK*/
#define ROOTCFG_USERSPNIDLOCK_LOCKKEY_LOCK                 (_ROOTCFG_USERSPNIDLOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_USERSPNIDLOCK  */
#define ROOTCFG_USERSPNIDLOCK_LOCKKEY_UNLOCK               (_ROOTCFG_USERSPNIDLOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_USERSPNIDLOCK*/

/* Bit fields for ROOTCFG USERDBGAPLOCK */
#define _ROOTCFG_USERDBGAPLOCK_RESETVALUE                  0x00000000UL                                  /**< Default value for ROOTCFG_USERDBGAPLOCK     */
#define _ROOTCFG_USERDBGAPLOCK_MASK                        0x0000FFFFUL                                  /**< Mask for ROOTCFG_USERDBGAPLOCK              */
#define _ROOTCFG_USERDBGAPLOCK_LOCKKEY_SHIFT               0                                             /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_USERDBGAPLOCK_LOCKKEY_MASK                0xFFFFUL                                      /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_USERDBGAPLOCK_LOCKKEY_DEFAULT             0x00000000UL                                  /**< Mode DEFAULT for ROOTCFG_USERDBGAPLOCK      */
#define _ROOTCFG_USERDBGAPLOCK_LOCKKEY_LOCK                0x00000000UL                                  /**< Mode LOCK for ROOTCFG_USERDBGAPLOCK          */
#define _ROOTCFG_USERDBGAPLOCK_LOCKKEY_UNLOCK              0x000045D3UL                                  /**< Mode UNLOCK for ROOTCFG_USERDBGAPLOCK        */
#define ROOTCFG_USERDBGAPLOCK_LOCKKEY_DEFAULT              (_ROOTCFG_USERDBGAPLOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_USERDBGAPLOCK*/
#define ROOTCFG_USERDBGAPLOCK_LOCKKEY_LOCK                 (_ROOTCFG_USERDBGAPLOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_USERDBGAPLOCK  */
#define ROOTCFG_USERDBGAPLOCK_LOCKKEY_UNLOCK               (_ROOTCFG_USERDBGAPLOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_USERDBGAPLOCK*/

/* Bit fields for ROOTCFG RADIODBGLOCK */
#define _ROOTCFG_RADIODBGLOCK_RESETVALUE                   0x00000000UL                                 /**< Default value for ROOTCFG_RADIODBGLOCK      */
#define _ROOTCFG_RADIODBGLOCK_MASK                         0x0000FFFFUL                                 /**< Mask for ROOTCFG_RADIODBGLOCK               */
#define _ROOTCFG_RADIODBGLOCK_LOCKKEY_SHIFT                0                                            /**< Shift value for ROOTCFG_LOCKKEY             */
#define _ROOTCFG_RADIODBGLOCK_LOCKKEY_MASK                 0xFFFFUL                                     /**< Bit mask for ROOTCFG_LOCKKEY                */
#define _ROOTCFG_RADIODBGLOCK_LOCKKEY_DEFAULT              0x00000000UL                                 /**< Mode DEFAULT for ROOTCFG_RADIODBGLOCK       */
#define _ROOTCFG_RADIODBGLOCK_LOCKKEY_LOCK                 0x00000000UL                                 /**< Mode LOCK for ROOTCFG_RADIODBGLOCK           */
#define _ROOTCFG_RADIODBGLOCK_LOCKKEY_UNLOCK               0x00001B5CUL                                 /**< Mode UNLOCK for ROOTCFG_RADIODBGLOCK         */
#define ROOTCFG_RADIODBGLOCK_LOCKKEY_DEFAULT               (_ROOTCFG_RADIODBGLOCK_LOCKKEY_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_RADIODBGLOCK*/
#define ROOTCFG_RADIODBGLOCK_LOCKKEY_LOCK                  (_ROOTCFG_RADIODBGLOCK_LOCKKEY_LOCK << 0)    /**< Shifted mode LOCK for ROOTCFG_RADIODBGLOCK   */
#define ROOTCFG_RADIODBGLOCK_LOCKKEY_UNLOCK                (_ROOTCFG_RADIODBGLOCK_LOCKKEY_UNLOCK << 0)  /**< Shifted mode UNLOCK for ROOTCFG_RADIODBGLOCK */

/* Bit fields for ROOTCFG MFRLOCKVAL0 */
#define _ROOTCFG_MFRLOCKVAL0_RESETVALUE                    0xC1332579UL                            /**< Default value for ROOTCFG_MFRLOCKVAL0       */
#define _ROOTCFG_MFRLOCKVAL0_MASK                          0xFFFFFFFFUL                            /**< Mask for ROOTCFG_MFRLOCKVAL0                */
#define _ROOTCFG_MFRLOCKVAL0_VAL_SHIFT                     0                                       /**< Shift value for ROOTCFG_VAL                 */
#define _ROOTCFG_MFRLOCKVAL0_VAL_MASK                      0xFFFFFFFFUL                            /**< Bit mask for ROOTCFG_VAL                    */
#define _ROOTCFG_MFRLOCKVAL0_VAL_DEFAULT                   0xC1332579UL                            /**< Mode DEFAULT for ROOTCFG_MFRLOCKVAL0        */
#define ROOTCFG_MFRLOCKVAL0_VAL_DEFAULT                    (_ROOTCFG_MFRLOCKVAL0_VAL_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_MFRLOCKVAL0*/

/* Bit fields for ROOTCFG MFRLOCKVAL1 */
#define _ROOTCFG_MFRLOCKVAL1_RESETVALUE                    0x29A0EC34UL                            /**< Default value for ROOTCFG_MFRLOCKVAL1       */
#define _ROOTCFG_MFRLOCKVAL1_MASK                          0xFFFFFFFFUL                            /**< Mask for ROOTCFG_MFRLOCKVAL1                */
#define _ROOTCFG_MFRLOCKVAL1_VAL_SHIFT                     0                                       /**< Shift value for ROOTCFG_VAL                 */
#define _ROOTCFG_MFRLOCKVAL1_VAL_MASK                      0xFFFFFFFFUL                            /**< Bit mask for ROOTCFG_VAL                    */
#define _ROOTCFG_MFRLOCKVAL1_VAL_DEFAULT                   0x29A0EC34UL                            /**< Mode DEFAULT for ROOTCFG_MFRLOCKVAL1        */
#define ROOTCFG_MFRLOCKVAL1_VAL_DEFAULT                    (_ROOTCFG_MFRLOCKVAL1_VAL_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_MFRLOCKVAL1*/

/* Bit fields for ROOTCFG LOCKSTATUS */
#define _ROOTCFG_LOCKSTATUS_RESETVALUE                     0x011F0107UL                                      /**< Default value for ROOTCFG_LOCKSTATUS        */
#define _ROOTCFG_LOCKSTATUS_MASK                           0x011F0117UL                                      /**< Mask for ROOTCFG_LOCKSTATUS                 */
#define ROOTCFG_LOCKSTATUS_BUSLOCK                         (0x1UL << 0)                                      /**< Bus Lock                                    */
#define _ROOTCFG_LOCKSTATUS_BUSLOCK_SHIFT                  0                                                 /**< Shift value for ROOTCFG_BUSLOCK             */
#define _ROOTCFG_LOCKSTATUS_BUSLOCK_MASK                   0x1UL                                             /**< Bit mask for ROOTCFG_BUSLOCK                */
#define _ROOTCFG_LOCKSTATUS_BUSLOCK_DEFAULT                0x00000001UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_BUSLOCK_DEFAULT                 (_ROOTCFG_LOCKSTATUS_BUSLOCK_DEFAULT << 0)        /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */
#define ROOTCFG_LOCKSTATUS_REGLOCK                         (0x1UL << 1)                                      /**< Register Lock                               */
#define _ROOTCFG_LOCKSTATUS_REGLOCK_SHIFT                  1                                                 /**< Shift value for ROOTCFG_REGLOCK             */
#define _ROOTCFG_LOCKSTATUS_REGLOCK_MASK                   0x2UL                                             /**< Bit mask for ROOTCFG_REGLOCK                */
#define _ROOTCFG_LOCKSTATUS_REGLOCK_DEFAULT                0x00000001UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_REGLOCK_DEFAULT                 (_ROOTCFG_LOCKSTATUS_REGLOCK_DEFAULT << 1)        /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */
#define ROOTCFG_LOCKSTATUS_MFRLOCK                         (0x1UL << 2)                                      /**< Manufacture Lock                            */
#define _ROOTCFG_LOCKSTATUS_MFRLOCK_SHIFT                  2                                                 /**< Shift value for ROOTCFG_MFRLOCK             */
#define _ROOTCFG_LOCKSTATUS_MFRLOCK_MASK                   0x4UL                                             /**< Bit mask for ROOTCFG_MFRLOCK                */
#define _ROOTCFG_LOCKSTATUS_MFRLOCK_DEFAULT                0x00000001UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_MFRLOCK_DEFAULT                 (_ROOTCFG_LOCKSTATUS_MFRLOCK_DEFAULT << 2)        /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */
#define ROOTCFG_LOCKSTATUS_ROOTMODELOCK                    (0x1UL << 4)                                      /**< Root Mode Lock                              */
#define _ROOTCFG_LOCKSTATUS_ROOTMODELOCK_SHIFT             4                                                 /**< Shift value for ROOTCFG_ROOTMODELOCK        */
#define _ROOTCFG_LOCKSTATUS_ROOTMODELOCK_MASK              0x10UL                                            /**< Bit mask for ROOTCFG_ROOTMODELOCK           */
#define _ROOTCFG_LOCKSTATUS_ROOTMODELOCK_DEFAULT           0x00000000UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_ROOTMODELOCK_DEFAULT            (_ROOTCFG_LOCKSTATUS_ROOTMODELOCK_DEFAULT << 4)   /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */
#define ROOTCFG_LOCKSTATUS_ROOTDBGLOCK                     (0x1UL << 8)                                      /**< Root Debug Lock                             */
#define _ROOTCFG_LOCKSTATUS_ROOTDBGLOCK_SHIFT              8                                                 /**< Shift value for ROOTCFG_ROOTDBGLOCK         */
#define _ROOTCFG_LOCKSTATUS_ROOTDBGLOCK_MASK               0x100UL                                           /**< Bit mask for ROOTCFG_ROOTDBGLOCK            */
#define _ROOTCFG_LOCKSTATUS_ROOTDBGLOCK_DEFAULT            0x00000001UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_ROOTDBGLOCK_DEFAULT             (_ROOTCFG_LOCKSTATUS_ROOTDBGLOCK_DEFAULT << 8)    /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */
#define ROOTCFG_LOCKSTATUS_USERDBGLOCK                     (0x1UL << 16)                                     /**< User Invasive Debug Lock                    */
#define _ROOTCFG_LOCKSTATUS_USERDBGLOCK_SHIFT              16                                                /**< Shift value for ROOTCFG_USERDBGLOCK         */
#define _ROOTCFG_LOCKSTATUS_USERDBGLOCK_MASK               0x10000UL                                         /**< Bit mask for ROOTCFG_USERDBGLOCK            */
#define _ROOTCFG_LOCKSTATUS_USERDBGLOCK_DEFAULT            0x00000001UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_USERDBGLOCK_DEFAULT             (_ROOTCFG_LOCKSTATUS_USERDBGLOCK_DEFAULT << 16)   /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */
#define ROOTCFG_LOCKSTATUS_USERNIDLOCK                     (0x1UL << 17)                                     /**< User Non-invasive Debug Lock                */
#define _ROOTCFG_LOCKSTATUS_USERNIDLOCK_SHIFT              17                                                /**< Shift value for ROOTCFG_USERNIDLOCK         */
#define _ROOTCFG_LOCKSTATUS_USERNIDLOCK_MASK               0x20000UL                                         /**< Bit mask for ROOTCFG_USERNIDLOCK            */
#define _ROOTCFG_LOCKSTATUS_USERNIDLOCK_DEFAULT            0x00000001UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_USERNIDLOCK_DEFAULT             (_ROOTCFG_LOCKSTATUS_USERNIDLOCK_DEFAULT << 17)   /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */
#define ROOTCFG_LOCKSTATUS_USERSPIDLOCK                    (0x1UL << 18)                                     /**< User Secure Invasive Debug Lock             */
#define _ROOTCFG_LOCKSTATUS_USERSPIDLOCK_SHIFT             18                                                /**< Shift value for ROOTCFG_USERSPIDLOCK        */
#define _ROOTCFG_LOCKSTATUS_USERSPIDLOCK_MASK              0x40000UL                                         /**< Bit mask for ROOTCFG_USERSPIDLOCK           */
#define _ROOTCFG_LOCKSTATUS_USERSPIDLOCK_DEFAULT           0x00000001UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_USERSPIDLOCK_DEFAULT            (_ROOTCFG_LOCKSTATUS_USERSPIDLOCK_DEFAULT << 18)  /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */
#define ROOTCFG_LOCKSTATUS_USERSPNIDLOCK                   (0x1UL << 19)                                     /**< User Secure Non-invasive Debug Lock         */
#define _ROOTCFG_LOCKSTATUS_USERSPNIDLOCK_SHIFT            19                                                /**< Shift value for ROOTCFG_USERSPNIDLOCK       */
#define _ROOTCFG_LOCKSTATUS_USERSPNIDLOCK_MASK             0x80000UL                                         /**< Bit mask for ROOTCFG_USERSPNIDLOCK          */
#define _ROOTCFG_LOCKSTATUS_USERSPNIDLOCK_DEFAULT          0x00000001UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_USERSPNIDLOCK_DEFAULT           (_ROOTCFG_LOCKSTATUS_USERSPNIDLOCK_DEFAULT << 19) /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */
#define ROOTCFG_LOCKSTATUS_USERDBGAPLOCK                   (0x1UL << 20)                                     /**< User Debug Access Port Lock                 */
#define _ROOTCFG_LOCKSTATUS_USERDBGAPLOCK_SHIFT            20                                                /**< Shift value for ROOTCFG_USERDBGAPLOCK       */
#define _ROOTCFG_LOCKSTATUS_USERDBGAPLOCK_MASK             0x100000UL                                        /**< Bit mask for ROOTCFG_USERDBGAPLOCK          */
#define _ROOTCFG_LOCKSTATUS_USERDBGAPLOCK_DEFAULT          0x00000001UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_USERDBGAPLOCK_DEFAULT           (_ROOTCFG_LOCKSTATUS_USERDBGAPLOCK_DEFAULT << 20) /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */
#define ROOTCFG_LOCKSTATUS_RADIODBGLOCK                    (0x1UL << 24)                                     /**< Radio Debug Lock                            */
#define _ROOTCFG_LOCKSTATUS_RADIODBGLOCK_SHIFT             24                                                /**< Shift value for ROOTCFG_RADIODBGLOCK        */
#define _ROOTCFG_LOCKSTATUS_RADIODBGLOCK_MASK              0x1000000UL                                       /**< Bit mask for ROOTCFG_RADIODBGLOCK           */
#define _ROOTCFG_LOCKSTATUS_RADIODBGLOCK_DEFAULT           0x00000001UL                                      /**< Mode DEFAULT for ROOTCFG_LOCKSTATUS         */
#define ROOTCFG_LOCKSTATUS_RADIODBGLOCK_DEFAULT            (_ROOTCFG_LOCKSTATUS_RADIODBGLOCK_DEFAULT << 24)  /**< Shifted mode DEFAULT for ROOTCFG_LOCKSTATUS */

/* Bit fields for ROOTCFG UNLOCKSTATUS */
#define _ROOTCFG_UNLOCKSTATUS_RESETVALUE                   0x00000010UL                                          /**< Default value for ROOTCFG_UNLOCKSTATUS      */
#define _ROOTCFG_UNLOCKSTATUS_MASK                         0x011F0117UL                                          /**< Mask for ROOTCFG_UNLOCKSTATUS               */
#define ROOTCFG_UNLOCKSTATUS_BUSUNLOCK                     (0x1UL << 0)                                          /**< Bus Unlock                                  */
#define _ROOTCFG_UNLOCKSTATUS_BUSUNLOCK_SHIFT              0                                                     /**< Shift value for ROOTCFG_BUSUNLOCK           */
#define _ROOTCFG_UNLOCKSTATUS_BUSUNLOCK_MASK               0x1UL                                                 /**< Bit mask for ROOTCFG_BUSUNLOCK              */
#define _ROOTCFG_UNLOCKSTATUS_BUSUNLOCK_DEFAULT            0x00000000UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_BUSUNLOCK_DEFAULT             (_ROOTCFG_UNLOCKSTATUS_BUSUNLOCK_DEFAULT << 0)        /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/
#define ROOTCFG_UNLOCKSTATUS_REGUNLOCK                     (0x1UL << 1)                                          /**< Register Unlock                             */
#define _ROOTCFG_UNLOCKSTATUS_REGUNLOCK_SHIFT              1                                                     /**< Shift value for ROOTCFG_REGUNLOCK           */
#define _ROOTCFG_UNLOCKSTATUS_REGUNLOCK_MASK               0x2UL                                                 /**< Bit mask for ROOTCFG_REGUNLOCK              */
#define _ROOTCFG_UNLOCKSTATUS_REGUNLOCK_DEFAULT            0x00000000UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_REGUNLOCK_DEFAULT             (_ROOTCFG_UNLOCKSTATUS_REGUNLOCK_DEFAULT << 1)        /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/
#define ROOTCFG_UNLOCKSTATUS_MFRUNLOCK                     (0x1UL << 2)                                          /**< Manufacture Unlock                          */
#define _ROOTCFG_UNLOCKSTATUS_MFRUNLOCK_SHIFT              2                                                     /**< Shift value for ROOTCFG_MFRUNLOCK           */
#define _ROOTCFG_UNLOCKSTATUS_MFRUNLOCK_MASK               0x4UL                                                 /**< Bit mask for ROOTCFG_MFRUNLOCK              */
#define _ROOTCFG_UNLOCKSTATUS_MFRUNLOCK_DEFAULT            0x00000000UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_MFRUNLOCK_DEFAULT             (_ROOTCFG_UNLOCKSTATUS_MFRUNLOCK_DEFAULT << 2)        /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/
#define ROOTCFG_UNLOCKSTATUS_ROOTMODEUNLOCK                (0x1UL << 4)                                          /**< Root Mode Unlock                            */
#define _ROOTCFG_UNLOCKSTATUS_ROOTMODEUNLOCK_SHIFT         4                                                     /**< Shift value for ROOTCFG_ROOTMODEUNLOCK      */
#define _ROOTCFG_UNLOCKSTATUS_ROOTMODEUNLOCK_MASK          0x10UL                                                /**< Bit mask for ROOTCFG_ROOTMODEUNLOCK         */
#define _ROOTCFG_UNLOCKSTATUS_ROOTMODEUNLOCK_DEFAULT       0x00000001UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_ROOTMODEUNLOCK_DEFAULT        (_ROOTCFG_UNLOCKSTATUS_ROOTMODEUNLOCK_DEFAULT << 4)   /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/
#define ROOTCFG_UNLOCKSTATUS_ROOTDBGUNLOCK                 (0x1UL << 8)                                          /**< Root Debug Unlock                           */
#define _ROOTCFG_UNLOCKSTATUS_ROOTDBGUNLOCK_SHIFT          8                                                     /**< Shift value for ROOTCFG_ROOTDBGUNLOCK       */
#define _ROOTCFG_UNLOCKSTATUS_ROOTDBGUNLOCK_MASK           0x100UL                                               /**< Bit mask for ROOTCFG_ROOTDBGUNLOCK          */
#define _ROOTCFG_UNLOCKSTATUS_ROOTDBGUNLOCK_DEFAULT        0x00000000UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_ROOTDBGUNLOCK_DEFAULT         (_ROOTCFG_UNLOCKSTATUS_ROOTDBGUNLOCK_DEFAULT << 8)    /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/
#define ROOTCFG_UNLOCKSTATUS_USERDBGUNLOCK                 (0x1UL << 16)                                         /**< User Invasive Debug Unlock                  */
#define _ROOTCFG_UNLOCKSTATUS_USERDBGUNLOCK_SHIFT          16                                                    /**< Shift value for ROOTCFG_USERDBGUNLOCK       */
#define _ROOTCFG_UNLOCKSTATUS_USERDBGUNLOCK_MASK           0x10000UL                                             /**< Bit mask for ROOTCFG_USERDBGUNLOCK          */
#define _ROOTCFG_UNLOCKSTATUS_USERDBGUNLOCK_DEFAULT        0x00000000UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_USERDBGUNLOCK_DEFAULT         (_ROOTCFG_UNLOCKSTATUS_USERDBGUNLOCK_DEFAULT << 16)   /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/
#define ROOTCFG_UNLOCKSTATUS_USERNIDUNLOCK                 (0x1UL << 17)                                         /**< User Non-invasive Debug Unlock              */
#define _ROOTCFG_UNLOCKSTATUS_USERNIDUNLOCK_SHIFT          17                                                    /**< Shift value for ROOTCFG_USERNIDUNLOCK       */
#define _ROOTCFG_UNLOCKSTATUS_USERNIDUNLOCK_MASK           0x20000UL                                             /**< Bit mask for ROOTCFG_USERNIDUNLOCK          */
#define _ROOTCFG_UNLOCKSTATUS_USERNIDUNLOCK_DEFAULT        0x00000000UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_USERNIDUNLOCK_DEFAULT         (_ROOTCFG_UNLOCKSTATUS_USERNIDUNLOCK_DEFAULT << 17)   /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/
#define ROOTCFG_UNLOCKSTATUS_USERSPIDUNLOCK                (0x1UL << 18)                                         /**< User Secure Invasive Debug Unlock           */
#define _ROOTCFG_UNLOCKSTATUS_USERSPIDUNLOCK_SHIFT         18                                                    /**< Shift value for ROOTCFG_USERSPIDUNLOCK      */
#define _ROOTCFG_UNLOCKSTATUS_USERSPIDUNLOCK_MASK          0x40000UL                                             /**< Bit mask for ROOTCFG_USERSPIDUNLOCK         */
#define _ROOTCFG_UNLOCKSTATUS_USERSPIDUNLOCK_DEFAULT       0x00000000UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_USERSPIDUNLOCK_DEFAULT        (_ROOTCFG_UNLOCKSTATUS_USERSPIDUNLOCK_DEFAULT << 18)  /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/
#define ROOTCFG_UNLOCKSTATUS_USERSPNIDUNLOCK               (0x1UL << 19)                                         /**< User Secure Non-invasive Debug Unlock       */
#define _ROOTCFG_UNLOCKSTATUS_USERSPNIDUNLOCK_SHIFT        19                                                    /**< Shift value for ROOTCFG_USERSPNIDUNLOCK     */
#define _ROOTCFG_UNLOCKSTATUS_USERSPNIDUNLOCK_MASK         0x80000UL                                             /**< Bit mask for ROOTCFG_USERSPNIDUNLOCK        */
#define _ROOTCFG_UNLOCKSTATUS_USERSPNIDUNLOCK_DEFAULT      0x00000000UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_USERSPNIDUNLOCK_DEFAULT       (_ROOTCFG_UNLOCKSTATUS_USERSPNIDUNLOCK_DEFAULT << 19) /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/
#define ROOTCFG_UNLOCKSTATUS_USERDBGAPUNLOCK               (0x1UL << 20)                                         /**< User Secure Debug Access Port Unlock        */
#define _ROOTCFG_UNLOCKSTATUS_USERDBGAPUNLOCK_SHIFT        20                                                    /**< Shift value for ROOTCFG_USERDBGAPUNLOCK     */
#define _ROOTCFG_UNLOCKSTATUS_USERDBGAPUNLOCK_MASK         0x100000UL                                            /**< Bit mask for ROOTCFG_USERDBGAPUNLOCK        */
#define _ROOTCFG_UNLOCKSTATUS_USERDBGAPUNLOCK_DEFAULT      0x00000000UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_USERDBGAPUNLOCK_DEFAULT       (_ROOTCFG_UNLOCKSTATUS_USERDBGAPUNLOCK_DEFAULT << 20) /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/
#define ROOTCFG_UNLOCKSTATUS_RADIODBGUNLOCK                (0x1UL << 24)                                         /**< Radio Debug Unlock                          */
#define _ROOTCFG_UNLOCKSTATUS_RADIODBGUNLOCK_SHIFT         24                                                    /**< Shift value for ROOTCFG_RADIODBGUNLOCK      */
#define _ROOTCFG_UNLOCKSTATUS_RADIODBGUNLOCK_MASK          0x1000000UL                                           /**< Bit mask for ROOTCFG_RADIODBGUNLOCK         */
#define _ROOTCFG_UNLOCKSTATUS_RADIODBGUNLOCK_DEFAULT       0x00000000UL                                          /**< Mode DEFAULT for ROOTCFG_UNLOCKSTATUS       */
#define ROOTCFG_UNLOCKSTATUS_RADIODBGUNLOCK_DEFAULT        (_ROOTCFG_UNLOCKSTATUS_RADIODBGUNLOCK_DEFAULT << 24)  /**< Shifted mode DEFAULT for ROOTCFG_UNLOCKSTATUS*/

/* Bit fields for ROOTCFG CTRL */
#define _ROOTCFG_CTRL_RESETVALUE                           0x00000000UL                                           /**< Default value for ROOTCFG_CTRL              */
#define _ROOTCFG_CTRL_MASK                                 0x00000058UL                                           /**< Mask for ROOTCFG_CTRL                       */
#define _ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_SHIFT          3                                                      /**< Shift value for ROOTCFG_EFUSEPOWERRAMPGPIOSEL*/
#define _ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_MASK           0x18UL                                                 /**< Bit mask for ROOTCFG_EFUSEPOWERRAMPGPIOSEL  */
#define _ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_DEFAULT        0x00000000UL                                           /**< Mode DEFAULT for ROOTCFG_CTRL               */
#define _ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_GPIO_9_PC0     0x00000000UL                                           /**< Mode GPIO_9_PC0 for ROOTCFG_CTRL             */
#define _ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_GPIP_11_PC2    0x00000001UL                                           /**< Mode GPIP_11_PC2 for ROOTCFG_CTRL            */
#define _ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_GPIO_13_PC4    0x00000002UL                                           /**< Mode GPIO_13_PC4 for ROOTCFG_CTRL            */
#define ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_DEFAULT         (_ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_DEFAULT << 3)     /**< Shifted mode DEFAULT for ROOTCFG_CTRL       */
#define ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_GPIO_9_PC0      (_ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_GPIO_9_PC0 << 3)  /**< Shifted mode GPIO_9_PC0 for ROOTCFG_CTRL     */
#define ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_GPIP_11_PC2     (_ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_GPIP_11_PC2 << 3) /**< Shifted mode GPIP_11_PC2 for ROOTCFG_CTRL    */
#define ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_GPIO_13_PC4     (_ROOTCFG_CTRL_EFUSEPOWERRAMPGPIOSEL_GPIO_13_PC4 << 3) /**< Shifted mode GPIO_13_PC4 for ROOTCFG_CTRL    */
#define ROOTCFG_CTRL_EFUSEDIS                              (0x1UL << 6)                                           /**< gate off efuse clock                        */
#define _ROOTCFG_CTRL_EFUSEDIS_SHIFT                       6                                                      /**< Shift value for ROOTCFG_EFUSEDIS            */
#define _ROOTCFG_CTRL_EFUSEDIS_MASK                        0x40UL                                                 /**< Bit mask for ROOTCFG_EFUSEDIS               */
#define _ROOTCFG_CTRL_EFUSEDIS_DEFAULT                     0x00000000UL                                           /**< Mode DEFAULT for ROOTCFG_CTRL               */
#define ROOTCFG_CTRL_EFUSEDIS_DEFAULT                      (_ROOTCFG_CTRL_EFUSEDIS_DEFAULT << 6)                  /**< Shifted mode DEFAULT for ROOTCFG_CTRL       */

/* Bit fields for ROOTCFG STATUS */
#define _ROOTCFG_STATUS_RESETVALUE                         0x00000000UL                                 /**< Default value for ROOTCFG_STATUS            */
#define _ROOTCFG_STATUS_MASK                               0x00000030UL                                 /**< Mask for ROOTCFG_STATUS                     */
#define ROOTCFG_STATUS_MFRACCESS                           (0x1UL << 4)                                 /**< Manufacturer access                         */
#define _ROOTCFG_STATUS_MFRACCESS_SHIFT                    4                                            /**< Shift value for ROOTCFG_MFRACCESS           */
#define _ROOTCFG_STATUS_MFRACCESS_MASK                     0x10UL                                       /**< Bit mask for ROOTCFG_MFRACCESS              */
#define _ROOTCFG_STATUS_MFRACCESS_DEFAULT                  0x00000000UL                                 /**< Mode DEFAULT for ROOTCFG_STATUS             */
#define ROOTCFG_STATUS_MFRACCESS_DEFAULT                   (_ROOTCFG_STATUS_MFRACCESS_DEFAULT << 4)     /**< Shifted mode DEFAULT for ROOTCFG_STATUS     */
#define ROOTCFG_STATUS_EFUSEUNLOCKED                       (0x1UL << 5)                                 /**< eFuse Unlocked                              */
#define _ROOTCFG_STATUS_EFUSEUNLOCKED_SHIFT                5                                            /**< Shift value for ROOTCFG_EFUSEUNLOCKED       */
#define _ROOTCFG_STATUS_EFUSEUNLOCKED_MASK                 0x20UL                                       /**< Bit mask for ROOTCFG_EFUSEUNLOCKED          */
#define _ROOTCFG_STATUS_EFUSEUNLOCKED_DEFAULT              0x00000000UL                                 /**< Mode DEFAULT for ROOTCFG_STATUS             */
#define ROOTCFG_STATUS_EFUSEUNLOCKED_DEFAULT               (_ROOTCFG_STATUS_EFUSEUNLOCKED_DEFAULT << 5) /**< Shifted mode DEFAULT for ROOTCFG_STATUS     */

/* Bit fields for ROOTCFG BOOTREG */
#define _ROOTCFG_BOOTREG_RESETVALUE                        0x00000000UL                            /**< Default value for ROOTCFG_BOOTREG           */
#define _ROOTCFG_BOOTREG_MASK                              0xFFFFFFFFUL                            /**< Mask for ROOTCFG_BOOTREG                    */
#define _ROOTCFG_BOOTREG_BOOTREG_SHIFT                     0                                       /**< Shift value for ROOTCFG_BOOTREG             */
#define _ROOTCFG_BOOTREG_BOOTREG_MASK                      0xFFFFFFFFUL                            /**< Bit mask for ROOTCFG_BOOTREG                */
#define _ROOTCFG_BOOTREG_BOOTREG_DEFAULT                   0x00000000UL                            /**< Mode DEFAULT for ROOTCFG_BOOTREG            */
#define ROOTCFG_BOOTREG_BOOTREG_DEFAULT                    (_ROOTCFG_BOOTREG_BOOTREG_DEFAULT << 0) /**< Shifted mode DEFAULT for ROOTCFG_BOOTREG    */

/** @} End of group EFR32BG22_ROOTCFG_BitFields */
/** @} End of group EFR32BG22_ROOTCFG */
/** @} End of group Parts */
#endif /* EFR32BG22_ROOTCFG_H */
