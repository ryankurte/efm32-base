/**************************************************************************//**
 * @file
 * @brief EFR32BG22 SYSCFG register and bit field definitions
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
#ifndef EFR32BG22_SYSCFG_H
#define EFR32BG22_SYSCFG_H
#define SYSCFG_HAS_SET_CLEAR

/**************************************************************************//**
* @addtogroup Parts
* @{
******************************************************************************/
/**************************************************************************//**
 * @defgroup EFR32BG22_SYSCFG SYSCFG
 * @{
 * @brief EFR32BG22 SYSCFG Register Declaration.
 *****************************************************************************/

/** SYSCFG Register Declaration. */
typedef struct {
  __IOM uint32_t IF;                            /**< Interrupt Flag Register                            */
  __IOM uint32_t IEN;                           /**< Interrupt Enable Register                          */
  uint32_t       RESERVED0[2U];                 /**< Reserved for future use                      */
  __IOM uint32_t CHIPREVHW;                     /**< Hardwired Chip Rev values                          */
  __IOM uint32_t CHIPREV;                       /**< Configurable Chip Rev Core IF (SE)                 */
  __IOM uint32_t INSTANCEID;                    /**< instancedid                                        */
  __IOM uint32_t CFGSSTCALIB;                   /**< Secure Systic calib                                */
  __IOM uint32_t CFGSSYSTIC;                    /**< Configure Systic                                   */
  uint32_t       RESERVED1[55U];                /**< Reserved for future use*/
  uint32_t       RESERVED2[1U];                 /**< Reserved for future use                      */
  uint32_t       RESERVED3[63U];                /**< Reserved for future use*/
  __IOM uint32_t CTRL;                          /**< Memory System Control Register                     */
  uint32_t       RESERVED4[1U];                 /**< Reserved for future use                      */
  __IOM uint32_t DMEM0RETNCTRL;                 /**< DMEM retention Control Register                    */
  uint32_t       RESERVED5[1U];                 /**< Reserved for future use                      */
  __IM uint32_t  DMEM0ECCADDR;                  /**< DMEM ECC Error Address Register                    */
  __IOM uint32_t DMEM0ECCCTRL;                  /**< DMEM ECC Control Register                          */
  uint32_t       RESERVED6[61U];                /**< Reserved for future use                      */
  __IOM uint32_t RAMBIASCONF;                   /**< RAM Voltage Bias Configuration Reg...              */
  uint32_t       RESERVED7[60U];                /**< Reserved for future use                      */
  __IOM uint32_t RADIORAMRETNCTRL;              /**< RADIO SEQRAM retention Control Register            */
  uint32_t       RESERVED8[1U];                 /**< Reserved for future use                      */
  __IOM uint32_t RADIOECCCTRL;                  /**< RADIO SEQRAM ECC Control Register                  */
  uint32_t       RESERVED9[1U];                 /**< Reserved for future use                      */
  __IM uint32_t  SEQRAMECCADDR;                 /**< SEQRAM ECC Error Address Register                  */
  __IM uint32_t  FRCRAMECCADDR;                 /**< FRCRAM ECC Error Address Register                  */
  uint32_t       RESERVED10[122U];              /**< Reserved for future use*/
  __IOM uint32_t ROOTDATA0;                     /**< User-to-root data                                  */
  __IOM uint32_t ROOTDATA1;                     /**< User-to-root data                                  */
  __IM uint32_t  ROOTLOCKSTATUS;                /**< Lock Status                                        */
  uint32_t       RESERVED11[637U];              /**< Reserved for future use*/
  __IOM uint32_t IF_SET;                        /**< Interrupt Flag Register                            */
  __IOM uint32_t IEN_SET;                       /**< Interrupt Enable Register                          */
  uint32_t       RESERVED12[2U];                /**< Reserved for future use                      */
  __IOM uint32_t CHIPREVHW_SET;                 /**< Hardwired Chip Rev values                          */
  __IOM uint32_t CHIPREV_SET;                   /**< Configurable Chip Rev Core IF (SE)                 */
  __IOM uint32_t INSTANCEID_SET;                /**< instancedid                                        */
  __IOM uint32_t CFGSSTCALIB_SET;               /**< Secure Systic calib                                */
  __IOM uint32_t CFGSSYSTIC_SET;                /**< Configure Systic                                   */
  uint32_t       RESERVED13[55U];               /**< Reserved for future use*/
  uint32_t       RESERVED14[1U];                /**< Reserved for future use                      */
  uint32_t       RESERVED15[63U];               /**< Reserved for future use*/
  __IOM uint32_t CTRL_SET;                      /**< Memory System Control Register                     */
  uint32_t       RESERVED16[1U];                /**< Reserved for future use                      */
  __IOM uint32_t DMEM0RETNCTRL_SET;             /**< DMEM retention Control Register                    */
  uint32_t       RESERVED17[1U];                /**< Reserved for future use                      */
  __IM uint32_t  DMEM0ECCADDR_SET;              /**< DMEM ECC Error Address Register                    */
  __IOM uint32_t DMEM0ECCCTRL_SET;              /**< DMEM ECC Control Register                          */
  uint32_t       RESERVED18[61U];               /**< Reserved for future use                      */
  __IOM uint32_t RAMBIASCONF_SET;               /**< RAM Voltage Bias Configuration Reg...              */
  uint32_t       RESERVED19[60U];               /**< Reserved for future use                      */
  __IOM uint32_t RADIORAMRETNCTRL_SET;          /**< RADIO SEQRAM retention Control Register            */
  uint32_t       RESERVED20[1U];                /**< Reserved for future use                      */
  __IOM uint32_t RADIOECCCTRL_SET;              /**< RADIO SEQRAM ECC Control Register                  */
  uint32_t       RESERVED21[1U];                /**< Reserved for future use                      */
  __IM uint32_t  SEQRAMECCADDR_SET;             /**< SEQRAM ECC Error Address Register                  */
  __IM uint32_t  FRCRAMECCADDR_SET;             /**< FRCRAM ECC Error Address Register                  */
  uint32_t       RESERVED22[122U];              /**< Reserved for future use*/
  __IOM uint32_t ROOTDATA0_SET;                 /**< User-to-root data                                  */
  __IOM uint32_t ROOTDATA1_SET;                 /**< User-to-root data                                  */
  __IM uint32_t  ROOTLOCKSTATUS_SET;            /**< Lock Status                                        */
  uint32_t       RESERVED23[637U];              /**< Reserved for future use*/
  __IOM uint32_t IF_CLR;                        /**< Interrupt Flag Register                            */
  __IOM uint32_t IEN_CLR;                       /**< Interrupt Enable Register                          */
  uint32_t       RESERVED24[2U];                /**< Reserved for future use                      */
  __IOM uint32_t CHIPREVHW_CLR;                 /**< Hardwired Chip Rev values                          */
  __IOM uint32_t CHIPREV_CLR;                   /**< Configurable Chip Rev Core IF (SE)                 */
  __IOM uint32_t INSTANCEID_CLR;                /**< instancedid                                        */
  __IOM uint32_t CFGSSTCALIB_CLR;               /**< Secure Systic calib                                */
  __IOM uint32_t CFGSSYSTIC_CLR;                /**< Configure Systic                                   */
  uint32_t       RESERVED25[55U];               /**< Reserved for future use*/
  uint32_t       RESERVED26[1U];                /**< Reserved for future use                      */
  uint32_t       RESERVED27[63U];               /**< Reserved for future use*/
  __IOM uint32_t CTRL_CLR;                      /**< Memory System Control Register                     */
  uint32_t       RESERVED28[1U];                /**< Reserved for future use                      */
  __IOM uint32_t DMEM0RETNCTRL_CLR;             /**< DMEM retention Control Register                    */
  uint32_t       RESERVED29[1U];                /**< Reserved for future use                      */
  __IM uint32_t  DMEM0ECCADDR_CLR;              /**< DMEM ECC Error Address Register                    */
  __IOM uint32_t DMEM0ECCCTRL_CLR;              /**< DMEM ECC Control Register                          */
  uint32_t       RESERVED30[61U];               /**< Reserved for future use                      */
  __IOM uint32_t RAMBIASCONF_CLR;               /**< RAM Voltage Bias Configuration Reg...              */
  uint32_t       RESERVED31[60U];               /**< Reserved for future use                      */
  __IOM uint32_t RADIORAMRETNCTRL_CLR;          /**< RADIO SEQRAM retention Control Register            */
  uint32_t       RESERVED32[1U];                /**< Reserved for future use                      */
  __IOM uint32_t RADIOECCCTRL_CLR;              /**< RADIO SEQRAM ECC Control Register                  */
  uint32_t       RESERVED33[1U];                /**< Reserved for future use                      */
  __IM uint32_t  SEQRAMECCADDR_CLR;             /**< SEQRAM ECC Error Address Register                  */
  __IM uint32_t  FRCRAMECCADDR_CLR;             /**< FRCRAM ECC Error Address Register                  */
  uint32_t       RESERVED34[122U];              /**< Reserved for future use*/
  __IOM uint32_t ROOTDATA0_CLR;                 /**< User-to-root data                                  */
  __IOM uint32_t ROOTDATA1_CLR;                 /**< User-to-root data                                  */
  __IM uint32_t  ROOTLOCKSTATUS_CLR;            /**< Lock Status                                        */
  uint32_t       RESERVED35[637U];              /**< Reserved for future use*/
  __IOM uint32_t IF_TGL;                        /**< Interrupt Flag Register                            */
  __IOM uint32_t IEN_TGL;                       /**< Interrupt Enable Register                          */
  uint32_t       RESERVED36[2U];                /**< Reserved for future use                      */
  __IOM uint32_t CHIPREVHW_TGL;                 /**< Hardwired Chip Rev values                          */
  __IOM uint32_t CHIPREV_TGL;                   /**< Configurable Chip Rev Core IF (SE)                 */
  __IOM uint32_t INSTANCEID_TGL;                /**< instancedid                                        */
  __IOM uint32_t CFGSSTCALIB_TGL;               /**< Secure Systic calib                                */
  __IOM uint32_t CFGSSYSTIC_TGL;                /**< Configure Systic                                   */
  uint32_t       RESERVED37[55U];               /**< Reserved for future use*/
  uint32_t       RESERVED38[1U];                /**< Reserved for future use                      */
  uint32_t       RESERVED39[63U];               /**< Reserved for future use*/
  __IOM uint32_t CTRL_TGL;                      /**< Memory System Control Register                     */
  uint32_t       RESERVED40[1U];                /**< Reserved for future use                      */
  __IOM uint32_t DMEM0RETNCTRL_TGL;             /**< DMEM retention Control Register                    */
  uint32_t       RESERVED41[1U];                /**< Reserved for future use                      */
  __IM uint32_t  DMEM0ECCADDR_TGL;              /**< DMEM ECC Error Address Register                    */
  __IOM uint32_t DMEM0ECCCTRL_TGL;              /**< DMEM ECC Control Register                          */
  uint32_t       RESERVED42[61U];               /**< Reserved for future use                      */
  __IOM uint32_t RAMBIASCONF_TGL;               /**< RAM Voltage Bias Configuration Reg...              */
  uint32_t       RESERVED43[60U];               /**< Reserved for future use                      */
  __IOM uint32_t RADIORAMRETNCTRL_TGL;          /**< RADIO SEQRAM retention Control Register            */
  uint32_t       RESERVED44[1U];                /**< Reserved for future use                      */
  __IOM uint32_t RADIOECCCTRL_TGL;              /**< RADIO SEQRAM ECC Control Register                  */
  uint32_t       RESERVED45[1U];                /**< Reserved for future use                      */
  __IM uint32_t  SEQRAMECCADDR_TGL;             /**< SEQRAM ECC Error Address Register                  */
  __IM uint32_t  FRCRAMECCADDR_TGL;             /**< FRCRAM ECC Error Address Register                  */
  uint32_t       RESERVED46[122U];              /**< Reserved for future use*/
  __IOM uint32_t ROOTDATA0_TGL;                 /**< User-to-root data                                  */
  __IOM uint32_t ROOTDATA1_TGL;                 /**< User-to-root data                                  */
  __IM uint32_t  ROOTLOCKSTATUS_TGL;            /**< Lock Status                                        */
} SYSCFG_TypeDef;
/** @} End of group EFR32BG22_SYSCFG */

/**************************************************************************//**
 * @addtogroup EFR32BG22_SYSCFG
 * @{
 * @defgroup EFR32BG22_SYSCFG_BitFields SYSCFG Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for SYSCFG IF */
#define _SYSCFG_IF_RESETVALUE                              0x00000000UL                             /**< Default value for SYSCFG_IF                 */
#define _SYSCFG_IF_MASK                                    0x3303000FUL                             /**< Mask for SYSCFG_IF                          */
#define SYSCFG_IF_SW0                                      (0x1UL << 0)                             /**< Software Interrupt flag                     */
#define _SYSCFG_IF_SW0_SHIFT                               0                                        /**< Shift value for SYSCFG_SW0                  */
#define _SYSCFG_IF_SW0_MASK                                0x1UL                                    /**< Bit mask for SYSCFG_SW0                     */
#define _SYSCFG_IF_SW0_DEFAULT                             0x00000000UL                             /**< Mode DEFAULT for SYSCFG_IF                  */
#define SYSCFG_IF_SW0_DEFAULT                              (_SYSCFG_IF_SW0_DEFAULT << 0)            /**< Shifted mode DEFAULT for SYSCFG_IF          */
#define SYSCFG_IF_SW1                                      (0x1UL << 1)                             /**< Software Interrupt flag                     */
#define _SYSCFG_IF_SW1_SHIFT                               1                                        /**< Shift value for SYSCFG_SW1                  */
#define _SYSCFG_IF_SW1_MASK                                0x2UL                                    /**< Bit mask for SYSCFG_SW1                     */
#define _SYSCFG_IF_SW1_DEFAULT                             0x00000000UL                             /**< Mode DEFAULT for SYSCFG_IF                  */
#define SYSCFG_IF_SW1_DEFAULT                              (_SYSCFG_IF_SW1_DEFAULT << 1)            /**< Shifted mode DEFAULT for SYSCFG_IF          */
#define SYSCFG_IF_SW2                                      (0x1UL << 2)                             /**< Software Interrupt flag                     */
#define _SYSCFG_IF_SW2_SHIFT                               2                                        /**< Shift value for SYSCFG_SW2                  */
#define _SYSCFG_IF_SW2_MASK                                0x4UL                                    /**< Bit mask for SYSCFG_SW2                     */
#define _SYSCFG_IF_SW2_DEFAULT                             0x00000000UL                             /**< Mode DEFAULT for SYSCFG_IF                  */
#define SYSCFG_IF_SW2_DEFAULT                              (_SYSCFG_IF_SW2_DEFAULT << 2)            /**< Shifted mode DEFAULT for SYSCFG_IF          */
#define SYSCFG_IF_SW3                                      (0x1UL << 3)                             /**< Software Interrupt flag                     */
#define _SYSCFG_IF_SW3_SHIFT                               3                                        /**< Shift value for SYSCFG_SW3                  */
#define _SYSCFG_IF_SW3_MASK                                0x8UL                                    /**< Bit mask for SYSCFG_SW3                     */
#define _SYSCFG_IF_SW3_DEFAULT                             0x00000000UL                             /**< Mode DEFAULT for SYSCFG_IF                  */
#define SYSCFG_IF_SW3_DEFAULT                              (_SYSCFG_IF_SW3_DEFAULT << 3)            /**< Shifted mode DEFAULT for SYSCFG_IF          */
#define SYSCFG_IF_RAMERR1B                                 (0x1UL << 16)                            /**< RAM 1-bit ECC Error Interrupt flag          */
#define _SYSCFG_IF_RAMERR1B_SHIFT                          16                                       /**< Shift value for SYSCFG_RAMERR1B             */
#define _SYSCFG_IF_RAMERR1B_MASK                           0x10000UL                                /**< Bit mask for SYSCFG_RAMERR1B                */
#define _SYSCFG_IF_RAMERR1B_DEFAULT                        0x00000000UL                             /**< Mode DEFAULT for SYSCFG_IF                  */
#define SYSCFG_IF_RAMERR1B_DEFAULT                         (_SYSCFG_IF_RAMERR1B_DEFAULT << 16)      /**< Shifted mode DEFAULT for SYSCFG_IF          */
#define SYSCFG_IF_RAMERR2B                                 (0x1UL << 17)                            /**< RAM 2-bit ECC Error Interrupt flag          */
#define _SYSCFG_IF_RAMERR2B_SHIFT                          17                                       /**< Shift value for SYSCFG_RAMERR2B             */
#define _SYSCFG_IF_RAMERR2B_MASK                           0x20000UL                                /**< Bit mask for SYSCFG_RAMERR2B                */
#define _SYSCFG_IF_RAMERR2B_DEFAULT                        0x00000000UL                             /**< Mode DEFAULT for SYSCFG_IF                  */
#define SYSCFG_IF_RAMERR2B_DEFAULT                         (_SYSCFG_IF_RAMERR2B_DEFAULT << 17)      /**< Shifted mode DEFAULT for SYSCFG_IF          */
#define SYSCFG_IF_SEQRAMERR1B                              (0x1UL << 24)                            /**< SEQRAM 1-bit ECC Error Interrupt flag       */
#define _SYSCFG_IF_SEQRAMERR1B_SHIFT                       24                                       /**< Shift value for SYSCFG_SEQRAMERR1B          */
#define _SYSCFG_IF_SEQRAMERR1B_MASK                        0x1000000UL                              /**< Bit mask for SYSCFG_SEQRAMERR1B             */
#define _SYSCFG_IF_SEQRAMERR1B_DEFAULT                     0x00000000UL                             /**< Mode DEFAULT for SYSCFG_IF                  */
#define SYSCFG_IF_SEQRAMERR1B_DEFAULT                      (_SYSCFG_IF_SEQRAMERR1B_DEFAULT << 24)   /**< Shifted mode DEFAULT for SYSCFG_IF          */
#define SYSCFG_IF_SEQRAMERR2B                              (0x1UL << 25)                            /**< SEQRAM 2-bit ECC Error Interrupt flag       */
#define _SYSCFG_IF_SEQRAMERR2B_SHIFT                       25                                       /**< Shift value for SYSCFG_SEQRAMERR2B          */
#define _SYSCFG_IF_SEQRAMERR2B_MASK                        0x2000000UL                              /**< Bit mask for SYSCFG_SEQRAMERR2B             */
#define _SYSCFG_IF_SEQRAMERR2B_DEFAULT                     0x00000000UL                             /**< Mode DEFAULT for SYSCFG_IF                  */
#define SYSCFG_IF_SEQRAMERR2B_DEFAULT                      (_SYSCFG_IF_SEQRAMERR2B_DEFAULT << 25)   /**< Shifted mode DEFAULT for SYSCFG_IF          */
#define SYSCFG_IF_FRCRAMERR1BIF                            (0x1UL << 28)                            /**< FRCRAM 1-bit ECC Error Interrupt flag       */
#define _SYSCFG_IF_FRCRAMERR1BIF_SHIFT                     28                                       /**< Shift value for SYSCFG_FRCRAMERR1BIF        */
#define _SYSCFG_IF_FRCRAMERR1BIF_MASK                      0x10000000UL                             /**< Bit mask for SYSCFG_FRCRAMERR1BIF           */
#define _SYSCFG_IF_FRCRAMERR1BIF_DEFAULT                   0x00000000UL                             /**< Mode DEFAULT for SYSCFG_IF                  */
#define SYSCFG_IF_FRCRAMERR1BIF_DEFAULT                    (_SYSCFG_IF_FRCRAMERR1BIF_DEFAULT << 28) /**< Shifted mode DEFAULT for SYSCFG_IF          */
#define SYSCFG_IF_FRCRAMERR2BIF                            (0x1UL << 29)                            /**< FRCRAM 2-bit ECC Error Interrupt flag       */
#define _SYSCFG_IF_FRCRAMERR2BIF_SHIFT                     29                                       /**< Shift value for SYSCFG_FRCRAMERR2BIF        */
#define _SYSCFG_IF_FRCRAMERR2BIF_MASK                      0x20000000UL                             /**< Bit mask for SYSCFG_FRCRAMERR2BIF           */
#define _SYSCFG_IF_FRCRAMERR2BIF_DEFAULT                   0x00000000UL                             /**< Mode DEFAULT for SYSCFG_IF                  */
#define SYSCFG_IF_FRCRAMERR2BIF_DEFAULT                    (_SYSCFG_IF_FRCRAMERR2BIF_DEFAULT << 29) /**< Shifted mode DEFAULT for SYSCFG_IF          */

/* Bit fields for SYSCFG IEN */
#define _SYSCFG_IEN_RESETVALUE                             0x00000000UL                               /**< Default value for SYSCFG_IEN                */
#define _SYSCFG_IEN_MASK                                   0x3303000FUL                               /**< Mask for SYSCFG_IEN                         */
#define SYSCFG_IEN_SW0                                     (0x1UL << 0)                               /**< RAM 1-bit ECC Error Interrupt enable        */
#define _SYSCFG_IEN_SW0_SHIFT                              0                                          /**< Shift value for SYSCFG_SW0                  */
#define _SYSCFG_IEN_SW0_MASK                               0x1UL                                      /**< Bit mask for SYSCFG_SW0                     */
#define _SYSCFG_IEN_SW0_DEFAULT                            0x00000000UL                               /**< Mode DEFAULT for SYSCFG_IEN                 */
#define SYSCFG_IEN_SW0_DEFAULT                             (_SYSCFG_IEN_SW0_DEFAULT << 0)             /**< Shifted mode DEFAULT for SYSCFG_IEN         */
#define SYSCFG_IEN_SW1                                     (0x1UL << 1)                               /**< RAM 1-bit ECC Error Interrupt enable        */
#define _SYSCFG_IEN_SW1_SHIFT                              1                                          /**< Shift value for SYSCFG_SW1                  */
#define _SYSCFG_IEN_SW1_MASK                               0x2UL                                      /**< Bit mask for SYSCFG_SW1                     */
#define _SYSCFG_IEN_SW1_DEFAULT                            0x00000000UL                               /**< Mode DEFAULT for SYSCFG_IEN                 */
#define SYSCFG_IEN_SW1_DEFAULT                             (_SYSCFG_IEN_SW1_DEFAULT << 1)             /**< Shifted mode DEFAULT for SYSCFG_IEN         */
#define SYSCFG_IEN_SW2                                     (0x1UL << 2)                               /**< RAM 1-bit ECC Error Interrupt enable        */
#define _SYSCFG_IEN_SW2_SHIFT                              2                                          /**< Shift value for SYSCFG_SW2                  */
#define _SYSCFG_IEN_SW2_MASK                               0x4UL                                      /**< Bit mask for SYSCFG_SW2                     */
#define _SYSCFG_IEN_SW2_DEFAULT                            0x00000000UL                               /**< Mode DEFAULT for SYSCFG_IEN                 */
#define SYSCFG_IEN_SW2_DEFAULT                             (_SYSCFG_IEN_SW2_DEFAULT << 2)             /**< Shifted mode DEFAULT for SYSCFG_IEN         */
#define SYSCFG_IEN_SW3                                     (0x1UL << 3)                               /**< RAM 1-bit ECC Error Interrupt enable        */
#define _SYSCFG_IEN_SW3_SHIFT                              3                                          /**< Shift value for SYSCFG_SW3                  */
#define _SYSCFG_IEN_SW3_MASK                               0x8UL                                      /**< Bit mask for SYSCFG_SW3                     */
#define _SYSCFG_IEN_SW3_DEFAULT                            0x00000000UL                               /**< Mode DEFAULT for SYSCFG_IEN                 */
#define SYSCFG_IEN_SW3_DEFAULT                             (_SYSCFG_IEN_SW3_DEFAULT << 3)             /**< Shifted mode DEFAULT for SYSCFG_IEN         */
#define SYSCFG_IEN_RAMERR1B                                (0x1UL << 16)                              /**< RAM 1-bit ECC Error Interrupt enable        */
#define _SYSCFG_IEN_RAMERR1B_SHIFT                         16                                         /**< Shift value for SYSCFG_RAMERR1B             */
#define _SYSCFG_IEN_RAMERR1B_MASK                          0x10000UL                                  /**< Bit mask for SYSCFG_RAMERR1B                */
#define _SYSCFG_IEN_RAMERR1B_DEFAULT                       0x00000000UL                               /**< Mode DEFAULT for SYSCFG_IEN                 */
#define SYSCFG_IEN_RAMERR1B_DEFAULT                        (_SYSCFG_IEN_RAMERR1B_DEFAULT << 16)       /**< Shifted mode DEFAULT for SYSCFG_IEN         */
#define SYSCFG_IEN_RAMERR2B                                (0x1UL << 17)                              /**< RAM 2-bit ECC Error Interrupt enable        */
#define _SYSCFG_IEN_RAMERR2B_SHIFT                         17                                         /**< Shift value for SYSCFG_RAMERR2B             */
#define _SYSCFG_IEN_RAMERR2B_MASK                          0x20000UL                                  /**< Bit mask for SYSCFG_RAMERR2B                */
#define _SYSCFG_IEN_RAMERR2B_DEFAULT                       0x00000000UL                               /**< Mode DEFAULT for SYSCFG_IEN                 */
#define SYSCFG_IEN_RAMERR2B_DEFAULT                        (_SYSCFG_IEN_RAMERR2B_DEFAULT << 17)       /**< Shifted mode DEFAULT for SYSCFG_IEN         */
#define SYSCFG_IEN_SEQRAMERR1B                             (0x1UL << 24)                              /**< SEQRAM 1-bit ECC Error Interrupt enable     */
#define _SYSCFG_IEN_SEQRAMERR1B_SHIFT                      24                                         /**< Shift value for SYSCFG_SEQRAMERR1B          */
#define _SYSCFG_IEN_SEQRAMERR1B_MASK                       0x1000000UL                                /**< Bit mask for SYSCFG_SEQRAMERR1B             */
#define _SYSCFG_IEN_SEQRAMERR1B_DEFAULT                    0x00000000UL                               /**< Mode DEFAULT for SYSCFG_IEN                 */
#define SYSCFG_IEN_SEQRAMERR1B_DEFAULT                     (_SYSCFG_IEN_SEQRAMERR1B_DEFAULT << 24)    /**< Shifted mode DEFAULT for SYSCFG_IEN         */
#define SYSCFG_IEN_SEQRAMERR2B                             (0x1UL << 25)                              /**< SEQRAM 2-bit ECC Error Interrupt enable     */
#define _SYSCFG_IEN_SEQRAMERR2B_SHIFT                      25                                         /**< Shift value for SYSCFG_SEQRAMERR2B          */
#define _SYSCFG_IEN_SEQRAMERR2B_MASK                       0x2000000UL                                /**< Bit mask for SYSCFG_SEQRAMERR2B             */
#define _SYSCFG_IEN_SEQRAMERR2B_DEFAULT                    0x00000000UL                               /**< Mode DEFAULT for SYSCFG_IEN                 */
#define SYSCFG_IEN_SEQRAMERR2B_DEFAULT                     (_SYSCFG_IEN_SEQRAMERR2B_DEFAULT << 25)    /**< Shifted mode DEFAULT for SYSCFG_IEN         */
#define SYSCFG_IEN_FRCRAMERR1BIEN                          (0x1UL << 28)                              /**< FRCRAM 1-bit ECC Error Interrupt enable     */
#define _SYSCFG_IEN_FRCRAMERR1BIEN_SHIFT                   28                                         /**< Shift value for SYSCFG_FRCRAMERR1BIEN       */
#define _SYSCFG_IEN_FRCRAMERR1BIEN_MASK                    0x10000000UL                               /**< Bit mask for SYSCFG_FRCRAMERR1BIEN          */
#define _SYSCFG_IEN_FRCRAMERR1BIEN_DEFAULT                 0x00000000UL                               /**< Mode DEFAULT for SYSCFG_IEN                 */
#define SYSCFG_IEN_FRCRAMERR1BIEN_DEFAULT                  (_SYSCFG_IEN_FRCRAMERR1BIEN_DEFAULT << 28) /**< Shifted mode DEFAULT for SYSCFG_IEN         */
#define SYSCFG_IEN_FRCRAMERR2BIEN                          (0x1UL << 29)                              /**< FRCRAM 2-bit ECC Error Interrupt enable     */
#define _SYSCFG_IEN_FRCRAMERR2BIEN_SHIFT                   29                                         /**< Shift value for SYSCFG_FRCRAMERR2BIEN       */
#define _SYSCFG_IEN_FRCRAMERR2BIEN_MASK                    0x20000000UL                               /**< Bit mask for SYSCFG_FRCRAMERR2BIEN          */
#define _SYSCFG_IEN_FRCRAMERR2BIEN_DEFAULT                 0x00000000UL                               /**< Mode DEFAULT for SYSCFG_IEN                 */
#define SYSCFG_IEN_FRCRAMERR2BIEN_DEFAULT                  (_SYSCFG_IEN_FRCRAMERR2BIEN_DEFAULT << 29) /**< Shifted mode DEFAULT for SYSCFG_IEN         */

/* Bit fields for SYSCFG CHIPREVHW */
#define _SYSCFG_CHIPREVHW_RESETVALUE                       0x00000C01UL                            /**< Default value for SYSCFG_CHIPREVHW          */
#define _SYSCFG_CHIPREVHW_MASK                             0xFF0FFFFFUL                            /**< Mask for SYSCFG_CHIPREVHW                   */
#define _SYSCFG_CHIPREVHW_MAJOR_SHIFT                      0                                       /**< Shift value for SYSCFG_MAJOR                */
#define _SYSCFG_CHIPREVHW_MAJOR_MASK                       0x3FUL                                  /**< Bit mask for SYSCFG_MAJOR                   */
#define _SYSCFG_CHIPREVHW_MAJOR_DEFAULT                    0x00000001UL                            /**< Mode DEFAULT for SYSCFG_CHIPREVHW           */
#define SYSCFG_CHIPREVHW_MAJOR_DEFAULT                     (_SYSCFG_CHIPREVHW_MAJOR_DEFAULT << 0)  /**< Shifted mode DEFAULT for SYSCFG_CHIPREVHW   */
#define _SYSCFG_CHIPREVHW_FAMILY_SHIFT                     6                                       /**< Shift value for SYSCFG_FAMILY               */
#define _SYSCFG_CHIPREVHW_FAMILY_MASK                      0xFC0UL                                 /**< Bit mask for SYSCFG_FAMILY                  */
#define _SYSCFG_CHIPREVHW_FAMILY_DEFAULT                   0x00000030UL                            /**< Mode DEFAULT for SYSCFG_CHIPREVHW           */
#define SYSCFG_CHIPREVHW_FAMILY_DEFAULT                    (_SYSCFG_CHIPREVHW_FAMILY_DEFAULT << 6) /**< Shifted mode DEFAULT for SYSCFG_CHIPREVHW   */
#define _SYSCFG_CHIPREVHW_MINOR_SHIFT                      12                                      /**< Shift value for SYSCFG_MINOR                */
#define _SYSCFG_CHIPREVHW_MINOR_MASK                       0xFF000UL                               /**< Bit mask for SYSCFG_MINOR                   */
#define _SYSCFG_CHIPREVHW_MINOR_DEFAULT                    0x00000000UL                            /**< Mode DEFAULT for SYSCFG_CHIPREVHW           */
#define SYSCFG_CHIPREVHW_MINOR_DEFAULT                     (_SYSCFG_CHIPREVHW_MINOR_DEFAULT << 12) /**< Shifted mode DEFAULT for SYSCFG_CHIPREVHW   */

/* Bit fields for SYSCFG CHIPREV */
#define _SYSCFG_CHIPREV_RESETVALUE                         0x00000000UL                          /**< Default value for SYSCFG_CHIPREV            */
#define _SYSCFG_CHIPREV_MASK                               0x000FFFFFUL                          /**< Mask for SYSCFG_CHIPREV                     */
#define _SYSCFG_CHIPREV_MAJOR_SHIFT                        0                                     /**< Shift value for SYSCFG_MAJOR                */
#define _SYSCFG_CHIPREV_MAJOR_MASK                         0x3FUL                                /**< Bit mask for SYSCFG_MAJOR                   */
#define _SYSCFG_CHIPREV_MAJOR_DEFAULT                      0x00000000UL                          /**< Mode DEFAULT for SYSCFG_CHIPREV             */
#define SYSCFG_CHIPREV_MAJOR_DEFAULT                       (_SYSCFG_CHIPREV_MAJOR_DEFAULT << 0)  /**< Shifted mode DEFAULT for SYSCFG_CHIPREV     */
#define _SYSCFG_CHIPREV_FAMILY_SHIFT                       6                                     /**< Shift value for SYSCFG_FAMILY               */
#define _SYSCFG_CHIPREV_FAMILY_MASK                        0xFC0UL                               /**< Bit mask for SYSCFG_FAMILY                  */
#define _SYSCFG_CHIPREV_FAMILY_DEFAULT                     0x00000000UL                          /**< Mode DEFAULT for SYSCFG_CHIPREV             */
#define SYSCFG_CHIPREV_FAMILY_DEFAULT                      (_SYSCFG_CHIPREV_FAMILY_DEFAULT << 6) /**< Shifted mode DEFAULT for SYSCFG_CHIPREV     */
#define _SYSCFG_CHIPREV_MINOR_SHIFT                        12                                    /**< Shift value for SYSCFG_MINOR                */
#define _SYSCFG_CHIPREV_MINOR_MASK                         0xFF000UL                             /**< Bit mask for SYSCFG_MINOR                   */
#define _SYSCFG_CHIPREV_MINOR_DEFAULT                      0x00000000UL                          /**< Mode DEFAULT for SYSCFG_CHIPREV             */
#define SYSCFG_CHIPREV_MINOR_DEFAULT                       (_SYSCFG_CHIPREV_MINOR_DEFAULT << 12) /**< Shifted mode DEFAULT for SYSCFG_CHIPREV     */

/* Bit fields for SYSCFG INSTANCEID */
#define _SYSCFG_INSTANCEID_RESETVALUE                      0x00000000UL                                 /**< Default value for SYSCFG_INSTANCEID         */
#define _SYSCFG_INSTANCEID_MASK                            0x0000000FUL                                 /**< Mask for SYSCFG_INSTANCEID                  */
#define _SYSCFG_INSTANCEID_INSTANCEID_SHIFT                0                                            /**< Shift value for SYSCFG_INSTANCEID           */
#define _SYSCFG_INSTANCEID_INSTANCEID_MASK                 0xFUL                                        /**< Bit mask for SYSCFG_INSTANCEID              */
#define _SYSCFG_INSTANCEID_INSTANCEID_DEFAULT              0x00000000UL                                 /**< Mode DEFAULT for SYSCFG_INSTANCEID          */
#define SYSCFG_INSTANCEID_INSTANCEID_DEFAULT               (_SYSCFG_INSTANCEID_INSTANCEID_DEFAULT << 0) /**< Shifted mode DEFAULT for SYSCFG_INSTANCEID  */

/* Bit fields for SYSCFG CFGSSTCALIB */
#define _SYSCFG_CFGSSTCALIB_RESETVALUE                     0x01004A37UL                              /**< Default value for SYSCFG_CFGSSTCALIB        */
#define _SYSCFG_CFGSSTCALIB_MASK                           0x03FFFFFFUL                              /**< Mask for SYSCFG_CFGSSTCALIB                 */
#define _SYSCFG_CFGSSTCALIB_TENMS_SHIFT                    0                                         /**< Shift value for SYSCFG_TENMS                */
#define _SYSCFG_CFGSSTCALIB_TENMS_MASK                     0xFFFFFFUL                                /**< Bit mask for SYSCFG_TENMS                   */
#define _SYSCFG_CFGSSTCALIB_TENMS_DEFAULT                  0x00004A37UL                              /**< Mode DEFAULT for SYSCFG_CFGSSTCALIB         */
#define SYSCFG_CFGSSTCALIB_TENMS_DEFAULT                   (_SYSCFG_CFGSSTCALIB_TENMS_DEFAULT << 0)  /**< Shifted mode DEFAULT for SYSCFG_CFGSSTCALIB */
#define SYSCFG_CFGSSTCALIB_SKEW                            (0x1UL << 24)                             /**< Secure Systic calib[24]                     */
#define _SYSCFG_CFGSSTCALIB_SKEW_SHIFT                     24                                        /**< Shift value for SYSCFG_SKEW                 */
#define _SYSCFG_CFGSSTCALIB_SKEW_MASK                      0x1000000UL                               /**< Bit mask for SYSCFG_SKEW                    */
#define _SYSCFG_CFGSSTCALIB_SKEW_DEFAULT                   0x00000001UL                              /**< Mode DEFAULT for SYSCFG_CFGSSTCALIB         */
#define SYSCFG_CFGSSTCALIB_SKEW_DEFAULT                    (_SYSCFG_CFGSSTCALIB_SKEW_DEFAULT << 24)  /**< Shifted mode DEFAULT for SYSCFG_CFGSSTCALIB */
#define SYSCFG_CFGSSTCALIB_NOREF                           (0x1UL << 25)                             /**< Secure Systic calib[25]                     */
#define _SYSCFG_CFGSSTCALIB_NOREF_SHIFT                    25                                        /**< Shift value for SYSCFG_NOREF                */
#define _SYSCFG_CFGSSTCALIB_NOREF_MASK                     0x2000000UL                               /**< Bit mask for SYSCFG_NOREF                   */
#define _SYSCFG_CFGSSTCALIB_NOREF_DEFAULT                  0x00000000UL                              /**< Mode DEFAULT for SYSCFG_CFGSSTCALIB         */
#define _SYSCFG_CFGSSTCALIB_NOREF_REF                      0x00000000UL                              /**< Mode REF for SYSCFG_CFGSSTCALIB              */
#define _SYSCFG_CFGSSTCALIB_NOREF_NOREF                    0x00000001UL                              /**< Mode NOREF for SYSCFG_CFGSSTCALIB            */
#define SYSCFG_CFGSSTCALIB_NOREF_DEFAULT                   (_SYSCFG_CFGSSTCALIB_NOREF_DEFAULT << 25) /**< Shifted mode DEFAULT for SYSCFG_CFGSSTCALIB */
#define SYSCFG_CFGSSTCALIB_NOREF_REF                       (_SYSCFG_CFGSSTCALIB_NOREF_REF << 25)     /**< Shifted mode REF for SYSCFG_CFGSSTCALIB      */
#define SYSCFG_CFGSSTCALIB_NOREF_NOREF                     (_SYSCFG_CFGSSTCALIB_NOREF_NOREF << 25)   /**< Shifted mode NOREF for SYSCFG_CFGSSTCALIB    */

/* Bit fields for SYSCFG CFGSSYSTIC */
#define _SYSCFG_CFGSSYSTIC_RESETVALUE                      0x00000000UL                                      /**< Default value for SYSCFG_CFGSSYSTIC         */
#define _SYSCFG_CFGSSYSTIC_MASK                            0x00000001UL                                      /**< Mask for SYSCFG_CFGSSYSTIC                  */
#define SYSCFG_CFGSSYSTIC_SSYSTICEXTCLKEN                  (0x1UL << 0)                                      /**< Secure Systic calib[23:0]                   */
#define _SYSCFG_CFGSSYSTIC_SSYSTICEXTCLKEN_SHIFT           0                                                 /**< Shift value for SYSCFG_SSYSTICEXTCLKEN      */
#define _SYSCFG_CFGSSYSTIC_SSYSTICEXTCLKEN_MASK            0x1UL                                             /**< Bit mask for SYSCFG_SSYSTICEXTCLKEN         */
#define _SYSCFG_CFGSSYSTIC_SSYSTICEXTCLKEN_DEFAULT         0x00000000UL                                      /**< Mode DEFAULT for SYSCFG_CFGSSYSTIC          */
#define SYSCFG_CFGSSYSTIC_SSYSTICEXTCLKEN_DEFAULT          (_SYSCFG_CFGSSYSTIC_SSYSTICEXTCLKEN_DEFAULT << 0) /**< Shifted mode DEFAULT for SYSCFG_CFGSSYSTIC  */

/* Bit fields for SYSCFG CTRL */
#define _SYSCFG_CTRL_RESETVALUE                            0x00000021UL                                 /**< Default value for SYSCFG_CTRL               */
#define _SYSCFG_CTRL_MASK                                  0x00000021UL                                 /**< Mask for SYSCFG_CTRL                        */
#define SYSCFG_CTRL_ADDRFAULTEN                            (0x1UL << 0)                                 /**< Invalid Address Bus Fault Response Enabl    */
#define _SYSCFG_CTRL_ADDRFAULTEN_SHIFT                     0                                            /**< Shift value for SYSCFG_ADDRFAULTEN          */
#define _SYSCFG_CTRL_ADDRFAULTEN_MASK                      0x1UL                                        /**< Bit mask for SYSCFG_ADDRFAULTEN             */
#define _SYSCFG_CTRL_ADDRFAULTEN_DEFAULT                   0x00000001UL                                 /**< Mode DEFAULT for SYSCFG_CTRL                */
#define SYSCFG_CTRL_ADDRFAULTEN_DEFAULT                    (_SYSCFG_CTRL_ADDRFAULTEN_DEFAULT << 0)      /**< Shifted mode DEFAULT for SYSCFG_CTRL        */
#define SYSCFG_CTRL_RAMECCERRFAULTEN                       (0x1UL << 5)                                 /**< Two bit ECC error bus fault response ena    */
#define _SYSCFG_CTRL_RAMECCERRFAULTEN_SHIFT                5                                            /**< Shift value for SYSCFG_RAMECCERRFAULTEN     */
#define _SYSCFG_CTRL_RAMECCERRFAULTEN_MASK                 0x20UL                                       /**< Bit mask for SYSCFG_RAMECCERRFAULTEN        */
#define _SYSCFG_CTRL_RAMECCERRFAULTEN_DEFAULT              0x00000001UL                                 /**< Mode DEFAULT for SYSCFG_CTRL                */
#define SYSCFG_CTRL_RAMECCERRFAULTEN_DEFAULT               (_SYSCFG_CTRL_RAMECCERRFAULTEN_DEFAULT << 5) /**< Shifted mode DEFAULT for SYSCFG_CTRL        */

/* Bit fields for SYSCFG DMEM0RETNCTRL */
#define _SYSCFG_DMEM0RETNCTRL_RESETVALUE                   0x00000000UL                                     /**< Default value for SYSCFG_DMEM0RETNCTRL      */
#define _SYSCFG_DMEM0RETNCTRL_MASK                         0x00000003UL                                     /**< Mask for SYSCFG_DMEM0RETNCTRL               */
#define _SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_SHIFT            0                                                /**< Shift value for SYSCFG_RAMRETNCTRL          */
#define _SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_MASK             0x3UL                                            /**< Bit mask for SYSCFG_RAMRETNCTRL             */
#define _SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_DEFAULT          0x00000000UL                                     /**< Mode DEFAULT for SYSCFG_DMEM0RETNCTRL       */
#define _SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_ALLON            0x00000000UL                                     /**< Mode ALLON for SYSCFG_DMEM0RETNCTRL          */
#define _SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_BLK0             0x00000001UL                                     /**< Mode BLK0 for SYSCFG_DMEM0RETNCTRL           */
#define _SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_BLK1             0x00000002UL                                     /**< Mode BLK1 for SYSCFG_DMEM0RETNCTRL           */
#define SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_DEFAULT           (_SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_DEFAULT << 0) /**< Shifted mode DEFAULT for SYSCFG_DMEM0RETNCTRL*/
#define SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_ALLON             (_SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_ALLON << 0)   /**< Shifted mode ALLON for SYSCFG_DMEM0RETNCTRL  */
#define SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_BLK0              (_SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_BLK0 << 0)    /**< Shifted mode BLK0 for SYSCFG_DMEM0RETNCTRL   */
#define SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_BLK1              (_SYSCFG_DMEM0RETNCTRL_RAMRETNCTRL_BLK1 << 0)    /**< Shifted mode BLK1 for SYSCFG_DMEM0RETNCTRL   */

/* Bit fields for SYSCFG DMEM0ECCADDR */
#define _SYSCFG_DMEM0ECCADDR_RESETVALUE                    0x00000000UL                                     /**< Default value for SYSCFG_DMEM0ECCADDR       */
#define _SYSCFG_DMEM0ECCADDR_MASK                          0xFFFFFFFFUL                                     /**< Mask for SYSCFG_DMEM0ECCADDR                */
#define _SYSCFG_DMEM0ECCADDR_DMEM0ECCADDR_SHIFT            0                                                /**< Shift value for SYSCFG_DMEM0ECCADDR         */
#define _SYSCFG_DMEM0ECCADDR_DMEM0ECCADDR_MASK             0xFFFFFFFFUL                                     /**< Bit mask for SYSCFG_DMEM0ECCADDR            */
#define _SYSCFG_DMEM0ECCADDR_DMEM0ECCADDR_DEFAULT          0x00000000UL                                     /**< Mode DEFAULT for SYSCFG_DMEM0ECCADDR        */
#define SYSCFG_DMEM0ECCADDR_DMEM0ECCADDR_DEFAULT           (_SYSCFG_DMEM0ECCADDR_DMEM0ECCADDR_DEFAULT << 0) /**< Shifted mode DEFAULT for SYSCFG_DMEM0ECCADDR*/

/* Bit fields for SYSCFG DMEM0ECCCTRL */
#define _SYSCFG_DMEM0ECCCTRL_RESETVALUE                    0x00000000UL                                   /**< Default value for SYSCFG_DMEM0ECCCTRL       */
#define _SYSCFG_DMEM0ECCCTRL_MASK                          0x00000003UL                                   /**< Mask for SYSCFG_DMEM0ECCCTRL                */
#define SYSCFG_DMEM0ECCCTRL_RAMECCEN                       (0x1UL << 0)                                   /**< RAM ECC Enable                              */
#define _SYSCFG_DMEM0ECCCTRL_RAMECCEN_SHIFT                0                                              /**< Shift value for SYSCFG_RAMECCEN             */
#define _SYSCFG_DMEM0ECCCTRL_RAMECCEN_MASK                 0x1UL                                          /**< Bit mask for SYSCFG_RAMECCEN                */
#define _SYSCFG_DMEM0ECCCTRL_RAMECCEN_DEFAULT              0x00000000UL                                   /**< Mode DEFAULT for SYSCFG_DMEM0ECCCTRL        */
#define SYSCFG_DMEM0ECCCTRL_RAMECCEN_DEFAULT               (_SYSCFG_DMEM0ECCCTRL_RAMECCEN_DEFAULT << 0)   /**< Shifted mode DEFAULT for SYSCFG_DMEM0ECCCTRL*/
#define SYSCFG_DMEM0ECCCTRL_RAMECCEWEN                     (0x1UL << 1)                                   /**< RAM ECC Error Writeback Enable              */
#define _SYSCFG_DMEM0ECCCTRL_RAMECCEWEN_SHIFT              1                                              /**< Shift value for SYSCFG_RAMECCEWEN           */
#define _SYSCFG_DMEM0ECCCTRL_RAMECCEWEN_MASK               0x2UL                                          /**< Bit mask for SYSCFG_RAMECCEWEN              */
#define _SYSCFG_DMEM0ECCCTRL_RAMECCEWEN_DEFAULT            0x00000000UL                                   /**< Mode DEFAULT for SYSCFG_DMEM0ECCCTRL        */
#define SYSCFG_DMEM0ECCCTRL_RAMECCEWEN_DEFAULT             (_SYSCFG_DMEM0ECCCTRL_RAMECCEWEN_DEFAULT << 1) /**< Shifted mode DEFAULT for SYSCFG_DMEM0ECCCTRL*/

/* Bit fields for SYSCFG RAMBIASCONF */
#define _SYSCFG_RAMBIASCONF_RESETVALUE                     0x00000002UL                                   /**< Default value for SYSCFG_RAMBIASCONF        */
#define _SYSCFG_RAMBIASCONF_MASK                           0x0000000FUL                                   /**< Mask for SYSCFG_RAMBIASCONF                 */
#define _SYSCFG_RAMBIASCONF_RAMBIASCTRL_SHIFT              0                                              /**< Shift value for SYSCFG_RAMBIASCTRL          */
#define _SYSCFG_RAMBIASCONF_RAMBIASCTRL_MASK               0xFUL                                          /**< Bit mask for SYSCFG_RAMBIASCTRL             */
#define _SYSCFG_RAMBIASCONF_RAMBIASCTRL_DEFAULT            0x00000002UL                                   /**< Mode DEFAULT for SYSCFG_RAMBIASCONF         */
#define _SYSCFG_RAMBIASCONF_RAMBIASCTRL_No                 0x00000000UL                                   /**< Mode No for SYSCFG_RAMBIASCONF               */
#define _SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB100             0x00000001UL                                   /**< Mode VSB100 for SYSCFG_RAMBIASCONF           */
#define _SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB200             0x00000002UL                                   /**< Mode VSB200 for SYSCFG_RAMBIASCONF           */
#define _SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB300             0x00000004UL                                   /**< Mode VSB300 for SYSCFG_RAMBIASCONF           */
#define _SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB400             0x00000008UL                                   /**< Mode VSB400 for SYSCFG_RAMBIASCONF           */
#define SYSCFG_RAMBIASCONF_RAMBIASCTRL_DEFAULT             (_SYSCFG_RAMBIASCONF_RAMBIASCTRL_DEFAULT << 0) /**< Shifted mode DEFAULT for SYSCFG_RAMBIASCONF */
#define SYSCFG_RAMBIASCONF_RAMBIASCTRL_No                  (_SYSCFG_RAMBIASCONF_RAMBIASCTRL_No << 0)      /**< Shifted mode No for SYSCFG_RAMBIASCONF       */
#define SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB100              (_SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB100 << 0)  /**< Shifted mode VSB100 for SYSCFG_RAMBIASCONF   */
#define SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB200              (_SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB200 << 0)  /**< Shifted mode VSB200 for SYSCFG_RAMBIASCONF   */
#define SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB300              (_SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB300 << 0)  /**< Shifted mode VSB300 for SYSCFG_RAMBIASCONF   */
#define SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB400              (_SYSCFG_RAMBIASCONF_RAMBIASCTRL_VSB400 << 0)  /**< Shifted mode VSB400 for SYSCFG_RAMBIASCONF   */

/* Bit fields for SYSCFG RADIORAMRETNCTRL */
#define _SYSCFG_RADIORAMRETNCTRL_RESETVALUE                0x00000000UL                                           /**< Default value for SYSCFG_RADIORAMRETNCTRL   */
#define _SYSCFG_RADIORAMRETNCTRL_MASK                      0x00000103UL                                           /**< Mask for SYSCFG_RADIORAMRETNCTRL            */
#define _SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_SHIFT      0                                                      /**< Shift value for SYSCFG_SEQRAMRETNCTRL       */
#define _SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_MASK       0x3UL                                                  /**< Bit mask for SYSCFG_SEQRAMRETNCTRL          */
#define _SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_DEFAULT    0x00000000UL                                           /**< Mode DEFAULT for SYSCFG_RADIORAMRETNCTRL    */
#define _SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_ALLON      0x00000000UL                                           /**< Mode ALLON for SYSCFG_RADIORAMRETNCTRL       */
#define _SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_BLK0       0x00000001UL                                           /**< Mode BLK0 for SYSCFG_RADIORAMRETNCTRL        */
#define _SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_BLK1       0x00000002UL                                           /**< Mode BLK1 for SYSCFG_RADIORAMRETNCTRL        */
#define _SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_ALLOFF     0x00000003UL                                           /**< Mode ALLOFF for SYSCFG_RADIORAMRETNCTRL      */
#define SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_DEFAULT     (_SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_DEFAULT << 0) /**< Shifted mode DEFAULT for SYSCFG_RADIORAMRETNCTRL*/
#define SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_ALLON       (_SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_ALLON << 0)   /**< Shifted mode ALLON for SYSCFG_RADIORAMRETNCTRL*/
#define SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_BLK0        (_SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_BLK0 << 0)    /**< Shifted mode BLK0 for SYSCFG_RADIORAMRETNCTRL*/
#define SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_BLK1        (_SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_BLK1 << 0)    /**< Shifted mode BLK1 for SYSCFG_RADIORAMRETNCTRL*/
#define SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_ALLOFF      (_SYSCFG_RADIORAMRETNCTRL_SEQRAMRETNCTRL_ALLOFF << 0)  /**< Shifted mode ALLOFF for SYSCFG_RADIORAMRETNCTRL*/
#define SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL             (0x1UL << 8)                                           /**< FRCRAM Memory Shutdown Control Register     */
#define _SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_SHIFT      8                                                      /**< Shift value for SYSCFG_FRCRAMRETNCTRL       */
#define _SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_MASK       0x100UL                                                /**< Bit mask for SYSCFG_FRCRAMRETNCTRL          */
#define _SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_DEFAULT    0x00000000UL                                           /**< Mode DEFAULT for SYSCFG_RADIORAMRETNCTRL    */
#define _SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_ALLON      0x00000000UL                                           /**< Mode ALLON for SYSCFG_RADIORAMRETNCTRL       */
#define _SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_ALLOFF     0x00000001UL                                           /**< Mode ALLOFF for SYSCFG_RADIORAMRETNCTRL      */
#define SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_DEFAULT     (_SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_DEFAULT << 8) /**< Shifted mode DEFAULT for SYSCFG_RADIORAMRETNCTRL*/
#define SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_ALLON       (_SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_ALLON << 8)   /**< Shifted mode ALLON for SYSCFG_RADIORAMRETNCTRL*/
#define SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_ALLOFF      (_SYSCFG_RADIORAMRETNCTRL_FRCRAMRETNCTRL_ALLOFF << 8)  /**< Shifted mode ALLOFF for SYSCFG_RADIORAMRETNCTRL*/

/* Bit fields for SYSCFG RADIOECCCTRL */
#define _SYSCFG_RADIOECCCTRL_RESETVALUE                    0x00000000UL                                      /**< Default value for SYSCFG_RADIOECCCTRL       */
#define _SYSCFG_RADIOECCCTRL_MASK                          0x00000303UL                                      /**< Mask for SYSCFG_RADIOECCCTRL                */
#define SYSCFG_RADIOECCCTRL_SEQRAMECCEN                    (0x1UL << 0)                                      /**< SEQRAM ECC Enable                           */
#define _SYSCFG_RADIOECCCTRL_SEQRAMECCEN_SHIFT             0                                                 /**< Shift value for SYSCFG_SEQRAMECCEN          */
#define _SYSCFG_RADIOECCCTRL_SEQRAMECCEN_MASK              0x1UL                                             /**< Bit mask for SYSCFG_SEQRAMECCEN             */
#define _SYSCFG_RADIOECCCTRL_SEQRAMECCEN_DEFAULT           0x00000000UL                                      /**< Mode DEFAULT for SYSCFG_RADIOECCCTRL        */
#define SYSCFG_RADIOECCCTRL_SEQRAMECCEN_DEFAULT            (_SYSCFG_RADIOECCCTRL_SEQRAMECCEN_DEFAULT << 0)   /**< Shifted mode DEFAULT for SYSCFG_RADIOECCCTRL*/
#define SYSCFG_RADIOECCCTRL_SEQRAMECCEWEN                  (0x1UL << 1)                                      /**< SEQRAM ECC Error Writeback Enable           */
#define _SYSCFG_RADIOECCCTRL_SEQRAMECCEWEN_SHIFT           1                                                 /**< Shift value for SYSCFG_SEQRAMECCEWEN        */
#define _SYSCFG_RADIOECCCTRL_SEQRAMECCEWEN_MASK            0x2UL                                             /**< Bit mask for SYSCFG_SEQRAMECCEWEN           */
#define _SYSCFG_RADIOECCCTRL_SEQRAMECCEWEN_DEFAULT         0x00000000UL                                      /**< Mode DEFAULT for SYSCFG_RADIOECCCTRL        */
#define SYSCFG_RADIOECCCTRL_SEQRAMECCEWEN_DEFAULT          (_SYSCFG_RADIOECCCTRL_SEQRAMECCEWEN_DEFAULT << 1) /**< Shifted mode DEFAULT for SYSCFG_RADIOECCCTRL*/
#define SYSCFG_RADIOECCCTRL_FRCRAMECCEN                    (0x1UL << 8)                                      /**< FRCRAM ECC Enable                           */
#define _SYSCFG_RADIOECCCTRL_FRCRAMECCEN_SHIFT             8                                                 /**< Shift value for SYSCFG_FRCRAMECCEN          */
#define _SYSCFG_RADIOECCCTRL_FRCRAMECCEN_MASK              0x100UL                                           /**< Bit mask for SYSCFG_FRCRAMECCEN             */
#define _SYSCFG_RADIOECCCTRL_FRCRAMECCEN_DEFAULT           0x00000000UL                                      /**< Mode DEFAULT for SYSCFG_RADIOECCCTRL        */
#define SYSCFG_RADIOECCCTRL_FRCRAMECCEN_DEFAULT            (_SYSCFG_RADIOECCCTRL_FRCRAMECCEN_DEFAULT << 8)   /**< Shifted mode DEFAULT for SYSCFG_RADIOECCCTRL*/
#define SYSCFG_RADIOECCCTRL_FRCRAMECCEWEN                  (0x1UL << 9)                                      /**< FRCRAM ECC Error Writeback Enable           */
#define _SYSCFG_RADIOECCCTRL_FRCRAMECCEWEN_SHIFT           9                                                 /**< Shift value for SYSCFG_FRCRAMECCEWEN        */
#define _SYSCFG_RADIOECCCTRL_FRCRAMECCEWEN_MASK            0x200UL                                           /**< Bit mask for SYSCFG_FRCRAMECCEWEN           */
#define _SYSCFG_RADIOECCCTRL_FRCRAMECCEWEN_DEFAULT         0x00000000UL                                      /**< Mode DEFAULT for SYSCFG_RADIOECCCTRL        */
#define SYSCFG_RADIOECCCTRL_FRCRAMECCEWEN_DEFAULT          (_SYSCFG_RADIOECCCTRL_FRCRAMECCEWEN_DEFAULT << 9) /**< Shifted mode DEFAULT for SYSCFG_RADIOECCCTRL*/

/* Bit fields for SYSCFG SEQRAMECCADDR */
#define _SYSCFG_SEQRAMECCADDR_RESETVALUE                   0x00000000UL                                       /**< Default value for SYSCFG_SEQRAMECCADDR      */
#define _SYSCFG_SEQRAMECCADDR_MASK                         0xFFFFFFFFUL                                       /**< Mask for SYSCFG_SEQRAMECCADDR               */
#define _SYSCFG_SEQRAMECCADDR_SEQRAMECCADDR_SHIFT          0                                                  /**< Shift value for SYSCFG_SEQRAMECCADDR        */
#define _SYSCFG_SEQRAMECCADDR_SEQRAMECCADDR_MASK           0xFFFFFFFFUL                                       /**< Bit mask for SYSCFG_SEQRAMECCADDR           */
#define _SYSCFG_SEQRAMECCADDR_SEQRAMECCADDR_DEFAULT        0x00000000UL                                       /**< Mode DEFAULT for SYSCFG_SEQRAMECCADDR       */
#define SYSCFG_SEQRAMECCADDR_SEQRAMECCADDR_DEFAULT         (_SYSCFG_SEQRAMECCADDR_SEQRAMECCADDR_DEFAULT << 0) /**< Shifted mode DEFAULT for SYSCFG_SEQRAMECCADDR*/

/* Bit fields for SYSCFG FRCRAMECCADDR */
#define _SYSCFG_FRCRAMECCADDR_RESETVALUE                   0x00000000UL                                       /**< Default value for SYSCFG_FRCRAMECCADDR      */
#define _SYSCFG_FRCRAMECCADDR_MASK                         0xFFFFFFFFUL                                       /**< Mask for SYSCFG_FRCRAMECCADDR               */
#define _SYSCFG_FRCRAMECCADDR_FRCRAMECCADDR_SHIFT          0                                                  /**< Shift value for SYSCFG_FRCRAMECCADDR        */
#define _SYSCFG_FRCRAMECCADDR_FRCRAMECCADDR_MASK           0xFFFFFFFFUL                                       /**< Bit mask for SYSCFG_FRCRAMECCADDR           */
#define _SYSCFG_FRCRAMECCADDR_FRCRAMECCADDR_DEFAULT        0x00000000UL                                       /**< Mode DEFAULT for SYSCFG_FRCRAMECCADDR       */
#define SYSCFG_FRCRAMECCADDR_FRCRAMECCADDR_DEFAULT         (_SYSCFG_FRCRAMECCADDR_FRCRAMECCADDR_DEFAULT << 0) /**< Shifted mode DEFAULT for SYSCFG_FRCRAMECCADDR*/

/* Bit fields for SYSCFG ROOTDATA0 */
#define _SYSCFG_ROOTDATA0_RESETVALUE                       0x00000000UL                          /**< Default value for SYSCFG_ROOTDATA0          */
#define _SYSCFG_ROOTDATA0_MASK                             0xFFFFFFFFUL                          /**< Mask for SYSCFG_ROOTDATA0                   */
#define _SYSCFG_ROOTDATA0_DATA_SHIFT                       0                                     /**< Shift value for SYSCFG_DATA                 */
#define _SYSCFG_ROOTDATA0_DATA_MASK                        0xFFFFFFFFUL                          /**< Bit mask for SYSCFG_DATA                    */
#define _SYSCFG_ROOTDATA0_DATA_DEFAULT                     0x00000000UL                          /**< Mode DEFAULT for SYSCFG_ROOTDATA0           */
#define SYSCFG_ROOTDATA0_DATA_DEFAULT                      (_SYSCFG_ROOTDATA0_DATA_DEFAULT << 0) /**< Shifted mode DEFAULT for SYSCFG_ROOTDATA0   */

/* Bit fields for SYSCFG ROOTDATA1 */
#define _SYSCFG_ROOTDATA1_RESETVALUE                       0x00000000UL                          /**< Default value for SYSCFG_ROOTDATA1          */
#define _SYSCFG_ROOTDATA1_MASK                             0xFFFFFFFFUL                          /**< Mask for SYSCFG_ROOTDATA1                   */
#define _SYSCFG_ROOTDATA1_DATA_SHIFT                       0                                     /**< Shift value for SYSCFG_DATA                 */
#define _SYSCFG_ROOTDATA1_DATA_MASK                        0xFFFFFFFFUL                          /**< Bit mask for SYSCFG_DATA                    */
#define _SYSCFG_ROOTDATA1_DATA_DEFAULT                     0x00000000UL                          /**< Mode DEFAULT for SYSCFG_ROOTDATA1           */
#define SYSCFG_ROOTDATA1_DATA_DEFAULT                      (_SYSCFG_ROOTDATA1_DATA_DEFAULT << 0) /**< Shifted mode DEFAULT for SYSCFG_ROOTDATA1   */

/* Bit fields for SYSCFG ROOTLOCKSTATUS */
#define _SYSCFG_ROOTLOCKSTATUS_RESETVALUE                  0x011F0107UL                                         /**< Default value for SYSCFG_ROOTLOCKSTATUS     */
#define _SYSCFG_ROOTLOCKSTATUS_MASK                        0x011F0117UL                                         /**< Mask for SYSCFG_ROOTLOCKSTATUS              */
#define SYSCFG_ROOTLOCKSTATUS_BUSLOCK                      (0x1UL << 0)                                         /**< Bus Lock                                    */
#define _SYSCFG_ROOTLOCKSTATUS_BUSLOCK_SHIFT               0                                                    /**< Shift value for SYSCFG_BUSLOCK              */
#define _SYSCFG_ROOTLOCKSTATUS_BUSLOCK_MASK                0x1UL                                                /**< Bit mask for SYSCFG_BUSLOCK                 */
#define _SYSCFG_ROOTLOCKSTATUS_BUSLOCK_DEFAULT             0x00000001UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_BUSLOCK_DEFAULT              (_SYSCFG_ROOTLOCKSTATUS_BUSLOCK_DEFAULT << 0)        /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/
#define SYSCFG_ROOTLOCKSTATUS_REGLOCK                      (0x1UL << 1)                                         /**< Register Lock                               */
#define _SYSCFG_ROOTLOCKSTATUS_REGLOCK_SHIFT               1                                                    /**< Shift value for SYSCFG_REGLOCK              */
#define _SYSCFG_ROOTLOCKSTATUS_REGLOCK_MASK                0x2UL                                                /**< Bit mask for SYSCFG_REGLOCK                 */
#define _SYSCFG_ROOTLOCKSTATUS_REGLOCK_DEFAULT             0x00000001UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_REGLOCK_DEFAULT              (_SYSCFG_ROOTLOCKSTATUS_REGLOCK_DEFAULT << 1)        /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/
#define SYSCFG_ROOTLOCKSTATUS_MFRLOCK                      (0x1UL << 2)                                         /**< Manufacture Lock                            */
#define _SYSCFG_ROOTLOCKSTATUS_MFRLOCK_SHIFT               2                                                    /**< Shift value for SYSCFG_MFRLOCK              */
#define _SYSCFG_ROOTLOCKSTATUS_MFRLOCK_MASK                0x4UL                                                /**< Bit mask for SYSCFG_MFRLOCK                 */
#define _SYSCFG_ROOTLOCKSTATUS_MFRLOCK_DEFAULT             0x00000001UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_MFRLOCK_DEFAULT              (_SYSCFG_ROOTLOCKSTATUS_MFRLOCK_DEFAULT << 2)        /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/
#define SYSCFG_ROOTLOCKSTATUS_ROOTMODELOCK                 (0x1UL << 4)                                         /**< Root Mode Lock                              */
#define _SYSCFG_ROOTLOCKSTATUS_ROOTMODELOCK_SHIFT          4                                                    /**< Shift value for SYSCFG_ROOTMODELOCK         */
#define _SYSCFG_ROOTLOCKSTATUS_ROOTMODELOCK_MASK           0x10UL                                               /**< Bit mask for SYSCFG_ROOTMODELOCK            */
#define _SYSCFG_ROOTLOCKSTATUS_ROOTMODELOCK_DEFAULT        0x00000000UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_ROOTMODELOCK_DEFAULT         (_SYSCFG_ROOTLOCKSTATUS_ROOTMODELOCK_DEFAULT << 4)   /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/
#define SYSCFG_ROOTLOCKSTATUS_ROOTDBGLOCK                  (0x1UL << 8)                                         /**< Root Debug Lock                             */
#define _SYSCFG_ROOTLOCKSTATUS_ROOTDBGLOCK_SHIFT           8                                                    /**< Shift value for SYSCFG_ROOTDBGLOCK          */
#define _SYSCFG_ROOTLOCKSTATUS_ROOTDBGLOCK_MASK            0x100UL                                              /**< Bit mask for SYSCFG_ROOTDBGLOCK             */
#define _SYSCFG_ROOTLOCKSTATUS_ROOTDBGLOCK_DEFAULT         0x00000001UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_ROOTDBGLOCK_DEFAULT          (_SYSCFG_ROOTLOCKSTATUS_ROOTDBGLOCK_DEFAULT << 8)    /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/
#define SYSCFG_ROOTLOCKSTATUS_USERDBGLOCK                  (0x1UL << 16)                                        /**< User Invasive Debug Lock                    */
#define _SYSCFG_ROOTLOCKSTATUS_USERDBGLOCK_SHIFT           16                                                   /**< Shift value for SYSCFG_USERDBGLOCK          */
#define _SYSCFG_ROOTLOCKSTATUS_USERDBGLOCK_MASK            0x10000UL                                            /**< Bit mask for SYSCFG_USERDBGLOCK             */
#define _SYSCFG_ROOTLOCKSTATUS_USERDBGLOCK_DEFAULT         0x00000001UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_USERDBGLOCK_DEFAULT          (_SYSCFG_ROOTLOCKSTATUS_USERDBGLOCK_DEFAULT << 16)   /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/
#define SYSCFG_ROOTLOCKSTATUS_USERNIDLOCK                  (0x1UL << 17)                                        /**< User Non-invasive Debug Lock                */
#define _SYSCFG_ROOTLOCKSTATUS_USERNIDLOCK_SHIFT           17                                                   /**< Shift value for SYSCFG_USERNIDLOCK          */
#define _SYSCFG_ROOTLOCKSTATUS_USERNIDLOCK_MASK            0x20000UL                                            /**< Bit mask for SYSCFG_USERNIDLOCK             */
#define _SYSCFG_ROOTLOCKSTATUS_USERNIDLOCK_DEFAULT         0x00000001UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_USERNIDLOCK_DEFAULT          (_SYSCFG_ROOTLOCKSTATUS_USERNIDLOCK_DEFAULT << 17)   /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/
#define SYSCFG_ROOTLOCKSTATUS_USERSPIDLOCK                 (0x1UL << 18)                                        /**< User Secure Invasive Debug Lock             */
#define _SYSCFG_ROOTLOCKSTATUS_USERSPIDLOCK_SHIFT          18                                                   /**< Shift value for SYSCFG_USERSPIDLOCK         */
#define _SYSCFG_ROOTLOCKSTATUS_USERSPIDLOCK_MASK           0x40000UL                                            /**< Bit mask for SYSCFG_USERSPIDLOCK            */
#define _SYSCFG_ROOTLOCKSTATUS_USERSPIDLOCK_DEFAULT        0x00000001UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_USERSPIDLOCK_DEFAULT         (_SYSCFG_ROOTLOCKSTATUS_USERSPIDLOCK_DEFAULT << 18)  /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/
#define SYSCFG_ROOTLOCKSTATUS_USERSPNIDLOCK                (0x1UL << 19)                                        /**< User Secure Non-invasive Debug Lock         */
#define _SYSCFG_ROOTLOCKSTATUS_USERSPNIDLOCK_SHIFT         19                                                   /**< Shift value for SYSCFG_USERSPNIDLOCK        */
#define _SYSCFG_ROOTLOCKSTATUS_USERSPNIDLOCK_MASK          0x80000UL                                            /**< Bit mask for SYSCFG_USERSPNIDLOCK           */
#define _SYSCFG_ROOTLOCKSTATUS_USERSPNIDLOCK_DEFAULT       0x00000001UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_USERSPNIDLOCK_DEFAULT        (_SYSCFG_ROOTLOCKSTATUS_USERSPNIDLOCK_DEFAULT << 19) /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/
#define SYSCFG_ROOTLOCKSTATUS_USERDBGAPLOCK                (0x1UL << 20)                                        /**< User Debug Access Port Lock                 */
#define _SYSCFG_ROOTLOCKSTATUS_USERDBGAPLOCK_SHIFT         20                                                   /**< Shift value for SYSCFG_USERDBGAPLOCK        */
#define _SYSCFG_ROOTLOCKSTATUS_USERDBGAPLOCK_MASK          0x100000UL                                           /**< Bit mask for SYSCFG_USERDBGAPLOCK           */
#define _SYSCFG_ROOTLOCKSTATUS_USERDBGAPLOCK_DEFAULT       0x00000001UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_USERDBGAPLOCK_DEFAULT        (_SYSCFG_ROOTLOCKSTATUS_USERDBGAPLOCK_DEFAULT << 20) /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/
#define SYSCFG_ROOTLOCKSTATUS_RADIODBGLOCK                 (0x1UL << 24)                                        /**< Radio Debug Lock                            */
#define _SYSCFG_ROOTLOCKSTATUS_RADIODBGLOCK_SHIFT          24                                                   /**< Shift value for SYSCFG_RADIODBGLOCK         */
#define _SYSCFG_ROOTLOCKSTATUS_RADIODBGLOCK_MASK           0x1000000UL                                          /**< Bit mask for SYSCFG_RADIODBGLOCK            */
#define _SYSCFG_ROOTLOCKSTATUS_RADIODBGLOCK_DEFAULT        0x00000001UL                                         /**< Mode DEFAULT for SYSCFG_ROOTLOCKSTATUS      */
#define SYSCFG_ROOTLOCKSTATUS_RADIODBGLOCK_DEFAULT         (_SYSCFG_ROOTLOCKSTATUS_RADIODBGLOCK_DEFAULT << 24)  /**< Shifted mode DEFAULT for SYSCFG_ROOTLOCKSTATUS*/

/** @} End of group EFR32BG22_SYSCFG_BitFields */
/** @} End of group EFR32BG22_SYSCFG */
/**************************************************************************//**
 * @defgroup EFR32BG22_SYSCFG_CFGNS SYSCFG_CFGNS
 * @{
 * @brief EFR32BG22 SYSCFG_CFGNS Register Declaration.
 *****************************************************************************/

/** SYSCFG_CFGNS Register Declaration. */
typedef struct {
  uint32_t       RESERVED0[7U];                 /**< Reserved for future use                      */
  __IOM uint32_t CFGNSTCALIB;                   /**< Non Secure Systic calib                            */
  uint32_t       RESERVED1[376U];               /**< Reserved for future use*/
  __IOM uint32_t ROOTNSDATA0;                   /**< User-to-root data                                  */
  __IOM uint32_t ROOTNSDATA1;                   /**< User-to-root data                                  */
  uint32_t       RESERVED2[638U];               /**< Reserved for future use*/
  uint32_t       RESERVED3[7U];                 /**< Reserved for future use                      */
  __IOM uint32_t CFGNSTCALIB_SET;               /**< Non Secure Systic calib                            */
  uint32_t       RESERVED4[376U];               /**< Reserved for future use*/
  __IOM uint32_t ROOTNSDATA0_SET;               /**< User-to-root data                                  */
  __IOM uint32_t ROOTNSDATA1_SET;               /**< User-to-root data                                  */
  uint32_t       RESERVED5[638U];               /**< Reserved for future use*/
  uint32_t       RESERVED6[7U];                 /**< Reserved for future use                      */
  __IOM uint32_t CFGNSTCALIB_CLR;               /**< Non Secure Systic calib                            */
  uint32_t       RESERVED7[376U];               /**< Reserved for future use*/
  __IOM uint32_t ROOTNSDATA0_CLR;               /**< User-to-root data                                  */
  __IOM uint32_t ROOTNSDATA1_CLR;               /**< User-to-root data                                  */
  uint32_t       RESERVED8[638U];               /**< Reserved for future use*/
  uint32_t       RESERVED9[7U];                 /**< Reserved for future use                      */
  __IOM uint32_t CFGNSTCALIB_TGL;               /**< Non Secure Systic calib                            */
  uint32_t       RESERVED10[376U];              /**< Reserved for future use*/
  __IOM uint32_t ROOTNSDATA0_TGL;               /**< User-to-root data                                  */
  __IOM uint32_t ROOTNSDATA1_TGL;               /**< User-to-root data                                  */
} SYSCFG_CFGNS_TypeDef;
/** @} End of group EFR32BG22_SYSCFG_CFGNS */

/**************************************************************************//**
 * @addtogroup EFR32BG22_SYSCFG_CFGNS
 * @{
 * @defgroup EFR32BG22_SYSCFG_CFGNS_BitFields SYSCFG_CFGNS Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for SYSCFG CFGNSTCALIB */
#define _SYSCFG_CFGNSTCALIB_RESETVALUE       0x01004A37UL                               /**< Default value for SYSCFG_CFGNSTCALIB        */
#define _SYSCFG_CFGNSTCALIB_MASK             0x03FFFFFFUL                               /**< Mask for SYSCFG_CFGNSTCALIB                 */
#define _SYSCFG_CFGNSTCALIB_TENMS_SHIFT      0                                          /**< Shift value for SYSCFG_TENMS                */
#define _SYSCFG_CFGNSTCALIB_TENMS_MASK       0xFFFFFFUL                                 /**< Bit mask for SYSCFG_TENMS                   */
#define _SYSCFG_CFGNSTCALIB_TENMS_DEFAULT    0x00004A37UL                               /**< Mode DEFAULT for SYSCFG_CFGNSTCALIB         */
#define SYSCFG_CFGNSTCALIB_TENMS_DEFAULT     (_SYSCFG_CFGNSTCALIB_TENMS_DEFAULT << 0)   /**< Shifted mode DEFAULT for SYSCFG_CFGNSTCALIB */
#define SYSCFG_CFGNSTCALIB_SKEW              (0x1UL << 24)                              /**< Non Secure Systic calib[24]                 */
#define _SYSCFG_CFGNSTCALIB_SKEW_SHIFT       24                                         /**< Shift value for SYSCFG_SKEW                 */
#define _SYSCFG_CFGNSTCALIB_SKEW_MASK        0x1000000UL                                /**< Bit mask for SYSCFG_SKEW                    */
#define _SYSCFG_CFGNSTCALIB_SKEW_DEFAULT     0x00000001UL                               /**< Mode DEFAULT for SYSCFG_CFGNSTCALIB         */
#define SYSCFG_CFGNSTCALIB_SKEW_DEFAULT      (_SYSCFG_CFGNSTCALIB_SKEW_DEFAULT << 24)   /**< Shifted mode DEFAULT for SYSCFG_CFGNSTCALIB */
#define SYSCFG_CFGNSTCALIB_NOREF             (0x1UL << 25)                              /**< Non Secure Systic calib[25]                 */
#define _SYSCFG_CFGNSTCALIB_NOREF_SHIFT      25                                         /**< Shift value for SYSCFG_NOREF                */
#define _SYSCFG_CFGNSTCALIB_NOREF_MASK       0x2000000UL                                /**< Bit mask for SYSCFG_NOREF                   */
#define _SYSCFG_CFGNSTCALIB_NOREF_DEFAULT    0x00000000UL                               /**< Mode DEFAULT for SYSCFG_CFGNSTCALIB         */
#define _SYSCFG_CFGNSTCALIB_NOREF_REF        0x00000000UL                               /**< Mode REF for SYSCFG_CFGNSTCALIB              */
#define _SYSCFG_CFGNSTCALIB_NOREF_NOREF      0x00000001UL                               /**< Mode NOREF for SYSCFG_CFGNSTCALIB            */
#define SYSCFG_CFGNSTCALIB_NOREF_DEFAULT     (_SYSCFG_CFGNSTCALIB_NOREF_DEFAULT << 25)  /**< Shifted mode DEFAULT for SYSCFG_CFGNSTCALIB */
#define SYSCFG_CFGNSTCALIB_NOREF_REF         (_SYSCFG_CFGNSTCALIB_NOREF_REF << 25)      /**< Shifted mode REF for SYSCFG_CFGNSTCALIB      */
#define SYSCFG_CFGNSTCALIB_NOREF_NOREF       (_SYSCFG_CFGNSTCALIB_NOREF_NOREF << 25)    /**< Shifted mode NOREF for SYSCFG_CFGNSTCALIB    */

/* Bit fields for SYSCFG ROOTNSDATA0 */
#define _SYSCFG_ROOTNSDATA0_RESETVALUE       0x00000000UL                               /**< Default value for SYSCFG_ROOTNSDATA0        */
#define _SYSCFG_ROOTNSDATA0_MASK             0xFFFFFFFFUL                               /**< Mask for SYSCFG_ROOTNSDATA0                 */
#define _SYSCFG_ROOTNSDATA0_DATA_SHIFT       0                                          /**< Shift value for SYSCFG_DATA                 */
#define _SYSCFG_ROOTNSDATA0_DATA_MASK        0xFFFFFFFFUL                               /**< Bit mask for SYSCFG_DATA                    */
#define _SYSCFG_ROOTNSDATA0_DATA_DEFAULT     0x00000000UL                               /**< Mode DEFAULT for SYSCFG_ROOTNSDATA0         */
#define SYSCFG_ROOTNSDATA0_DATA_DEFAULT      (_SYSCFG_ROOTNSDATA0_DATA_DEFAULT << 0)    /**< Shifted mode DEFAULT for SYSCFG_ROOTNSDATA0 */

/* Bit fields for SYSCFG ROOTNSDATA1 */
#define _SYSCFG_ROOTNSDATA1_RESETVALUE       0x00000000UL                               /**< Default value for SYSCFG_ROOTNSDATA1        */
#define _SYSCFG_ROOTNSDATA1_MASK             0xFFFFFFFFUL                               /**< Mask for SYSCFG_ROOTNSDATA1                 */
#define _SYSCFG_ROOTNSDATA1_DATA_SHIFT       0                                          /**< Shift value for SYSCFG_DATA                 */
#define _SYSCFG_ROOTNSDATA1_DATA_MASK        0xFFFFFFFFUL                               /**< Bit mask for SYSCFG_DATA                    */
#define _SYSCFG_ROOTNSDATA1_DATA_DEFAULT     0x00000000UL                               /**< Mode DEFAULT for SYSCFG_ROOTNSDATA1         */
#define SYSCFG_ROOTNSDATA1_DATA_DEFAULT      (_SYSCFG_ROOTNSDATA1_DATA_DEFAULT << 0)    /**< Shifted mode DEFAULT for SYSCFG_ROOTNSDATA1 */

/** @} End of group EFR32BG22_SYSCFG_CFGNS_BitFields */
/** @} End of group EFR32BG22_SYSCFG_CFGNS */
/** @} End of group Parts */
#endif /* EFR32BG22_SYSCFG_H */
