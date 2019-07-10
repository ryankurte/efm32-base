/**************************************************************************//**
 * @file
 * @brief EFR32BG22 DCI register and bit field definitions
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
#ifndef EFR32BG22_DCI_H
#define EFR32BG22_DCI_H

/**************************************************************************//**
* @addtogroup Parts
* @{
******************************************************************************/
/**************************************************************************//**
 * @defgroup EFR32BG22_DCI_DBGGRANT DCI_DBGGRANT
 * @{
 * @brief EFR32BG22 DCI_DBGGRANT Register Declaration.
 *****************************************************************************/

/** DCI_DBGGRANT Register Declaration. */
typedef struct {
  __IOM uint32_t ENABLECM;                           /**< Dbg enable Prot CM                                 */
  __IOM uint32_t ENABLESM;                           /**< Dbg enable Prot SM                                 */
  __IOM uint32_t DISABLECM;                          /**< Dbg disable Prot CM                                */
  __IOM uint32_t DISABLESM;                          /**< Dbg disable Prot SM                                */
  __IM uint32_t  DBGGRANT;                           /**< Dbg Grant                                          */
} DCI_DBGGRANT_TypeDef;
/** @} End of group EFR32BG22_DCI_DBGGRANT */

/**************************************************************************//**
 * @addtogroup EFR32BG22_DCI_DBGGRANT
 * @{
 * @defgroup EFR32BG22_DCI_DBGGRANT_BitFields DCI_DBGGRANT Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for DCI ENABLECM */
#define _DCI_ENABLECM_RESETVALUE            0x00000000UL                                /**< Default value for DCI_ENABLECM              */
#define _DCI_ENABLECM_MASK                  0x0000FFFFUL                                /**< Mask for DCI_ENABLECM                       */
#define _DCI_ENABLECM_ENABLECM_SHIFT        0                                           /**< Shift value for DCI_ENABLECM                */
#define _DCI_ENABLECM_ENABLECM_MASK         0xFFFFUL                                    /**< Bit mask for DCI_ENABLECM                   */
#define _DCI_ENABLECM_ENABLECM_DEFAULT      0x00000000UL                                /**< Mode DEFAULT for DCI_ENABLECM               */
#define DCI_ENABLECM_ENABLECM_DEFAULT       (_DCI_ENABLECM_ENABLECM_DEFAULT << 0)       /**< Shifted mode DEFAULT for DCI_ENABLECM       */

/* Bit fields for DCI ENABLESM */
#define _DCI_ENABLESM_RESETVALUE            0x00000000UL                                /**< Default value for DCI_ENABLESM              */
#define _DCI_ENABLESM_MASK                  0x0000FFFFUL                                /**< Mask for DCI_ENABLESM                       */
#define _DCI_ENABLESM_ENABLESM_SHIFT        0                                           /**< Shift value for DCI_ENABLESM                */
#define _DCI_ENABLESM_ENABLESM_MASK         0xFFFFUL                                    /**< Bit mask for DCI_ENABLESM                   */
#define _DCI_ENABLESM_ENABLESM_DEFAULT      0x00000000UL                                /**< Mode DEFAULT for DCI_ENABLESM               */
#define DCI_ENABLESM_ENABLESM_DEFAULT       (_DCI_ENABLESM_ENABLESM_DEFAULT << 0)       /**< Shifted mode DEFAULT for DCI_ENABLESM       */

/* Bit fields for DCI DISABLECM */
#define _DCI_DISABLECM_RESETVALUE           0x00000000UL                                /**< Default value for DCI_DISABLECM             */
#define _DCI_DISABLECM_MASK                 0x0000FFFFUL                                /**< Mask for DCI_DISABLECM                      */
#define _DCI_DISABLECM_DISABLECM_SHIFT      0                                           /**< Shift value for DCI_DISABLECM               */
#define _DCI_DISABLECM_DISABLECM_MASK       0xFFFFUL                                    /**< Bit mask for DCI_DISABLECM                  */
#define _DCI_DISABLECM_DISABLECM_DEFAULT    0x00000000UL                                /**< Mode DEFAULT for DCI_DISABLECM              */
#define DCI_DISABLECM_DISABLECM_DEFAULT     (_DCI_DISABLECM_DISABLECM_DEFAULT << 0)     /**< Shifted mode DEFAULT for DCI_DISABLECM      */

/* Bit fields for DCI DISABLESM */
#define _DCI_DISABLESM_RESETVALUE           0x00000000UL                                /**< Default value for DCI_DISABLESM             */
#define _DCI_DISABLESM_MASK                 0x0000FFFFUL                                /**< Mask for DCI_DISABLESM                      */
#define _DCI_DISABLESM_DISABLESM_SHIFT      0                                           /**< Shift value for DCI_DISABLESM               */
#define _DCI_DISABLESM_DISABLESM_MASK       0xFFFFUL                                    /**< Bit mask for DCI_DISABLESM                  */
#define _DCI_DISABLESM_DISABLESM_DEFAULT    0x00000000UL                                /**< Mode DEFAULT for DCI_DISABLESM              */
#define DCI_DISABLESM_DISABLESM_DEFAULT     (_DCI_DISABLESM_DISABLESM_DEFAULT << 0)     /**< Shifted mode DEFAULT for DCI_DISABLESM      */

/* Bit fields for DCI DBGGRANT */
#define _DCI_DBGGRANT_RESETVALUE            0x00000000UL                                /**< Default value for DCI_DBGGRANT              */
#define _DCI_DBGGRANT_MASK                  0x0000FFFFUL                                /**< Mask for DCI_DBGGRANT                       */
#define _DCI_DBGGRANT_DBGGRANT_SHIFT        0                                           /**< Shift value for DCI_DBGGRANT                */
#define _DCI_DBGGRANT_DBGGRANT_MASK         0xFFFFUL                                    /**< Bit mask for DCI_DBGGRANT                   */
#define _DCI_DBGGRANT_DBGGRANT_DEFAULT      0x00000000UL                                /**< Mode DEFAULT for DCI_DBGGRANT               */
#define DCI_DBGGRANT_DBGGRANT_DEFAULT       (_DCI_DBGGRANT_DBGGRANT_DEFAULT << 0)       /**< Shifted mode DEFAULT for DCI_DBGGRANT       */

/** @} End of group EFR32BG22_DCI_DBGGRANT_BitFields */
/** @} End of group EFR32BG22_DCI_DBGGRANT */
/**************************************************************************//**
 * @defgroup EFR32BG22_DCI_MESSAGING DCI_MESSAGING
 * @{
 * @brief EFR32BG22 DCI_MESSAGING Register Declaration.
 *****************************************************************************/

/** DCI_MESSAGING Register Declaration. */
typedef struct {
  __IM uint32_t  RXDATA;                             /**< Rx Data                                            */
  __IOM uint32_t TXDATA;                             /**< Tx Data                                            */
  __IOM uint32_t CLRRXIRQ;                           /**< Clear Rx IRQ                                       */
  __IOM uint32_t CLRTXIRQ;                           /**< Clear Tx IRQ                                       */
  uint32_t       RESERVED0[2U];                      /**< Reserved for future use                      */
  __IM uint32_t  DATAAVAILABLE;                      /**< Data Available                                     */
  __IM uint32_t  SENDINGDATA;                        /**< Sending Data                                       */
  uint32_t       RESERVED1[2U];                      /**< Reserved for future use                      */
  __IM uint32_t  RXSTATE;                            /**< Rx State                                           */
  __IM uint32_t  TXSTATE;                            /**< Tx State                                           */
} DCI_MESSAGING_TypeDef;
/** @} End of group EFR32BG22_DCI_MESSAGING */

/**************************************************************************//**
 * @addtogroup EFR32BG22_DCI_MESSAGING
 * @{
 * @defgroup EFR32BG22_DCI_MESSAGING_BitFields DCI_MESSAGING Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for DCI RXDATA */
#define _DCI_RXDATA_RESETVALUE                      0x00000000UL                        /**< Default value for DCI_RXDATA                */
#define _DCI_RXDATA_MASK                            0xFFFFFFFFUL                        /**< Mask for DCI_RXDATA                         */
#define _DCI_RXDATA_RXDATA_SHIFT                    0                                   /**< Shift value for DCI_RXDATA                  */
#define _DCI_RXDATA_RXDATA_MASK                     0xFFFFFFFFUL                        /**< Bit mask for DCI_RXDATA                     */
#define _DCI_RXDATA_RXDATA_DEFAULT                  0x00000000UL                        /**< Mode DEFAULT for DCI_RXDATA                 */
#define DCI_RXDATA_RXDATA_DEFAULT                   (_DCI_RXDATA_RXDATA_DEFAULT << 0)   /**< Shifted mode DEFAULT for DCI_RXDATA         */

/* Bit fields for DCI TXDATA */
#define _DCI_TXDATA_RESETVALUE                      0x00000000UL                        /**< Default value for DCI_TXDATA                */
#define _DCI_TXDATA_MASK                            0xFFFFFFFFUL                        /**< Mask for DCI_TXDATA                         */
#define _DCI_TXDATA_TXDATA_SHIFT                    0                                   /**< Shift value for DCI_TXDATA                  */
#define _DCI_TXDATA_TXDATA_MASK                     0xFFFFFFFFUL                        /**< Bit mask for DCI_TXDATA                     */
#define _DCI_TXDATA_TXDATA_DEFAULT                  0x00000000UL                        /**< Mode DEFAULT for DCI_TXDATA                 */
#define DCI_TXDATA_TXDATA_DEFAULT                   (_DCI_TXDATA_TXDATA_DEFAULT << 0)   /**< Shifted mode DEFAULT for DCI_TXDATA         */

/* Bit fields for DCI CLRRXIRQ */
#define _DCI_CLRRXIRQ_RESETVALUE                    0x00000000UL                          /**< Default value for DCI_CLRRXIRQ              */
#define _DCI_CLRRXIRQ_MASK                          0xFFFFFFFFUL                          /**< Mask for DCI_CLRRXIRQ                       */
#define _DCI_CLRRXIRQ_CLRRXIRQ_SHIFT                0                                     /**< Shift value for DCI_CLRRXIRQ                */
#define _DCI_CLRRXIRQ_CLRRXIRQ_MASK                 0xFFFFFFFFUL                          /**< Bit mask for DCI_CLRRXIRQ                   */
#define _DCI_CLRRXIRQ_CLRRXIRQ_DEFAULT              0x00000000UL                          /**< Mode DEFAULT for DCI_CLRRXIRQ               */
#define DCI_CLRRXIRQ_CLRRXIRQ_DEFAULT               (_DCI_CLRRXIRQ_CLRRXIRQ_DEFAULT << 0) /**< Shifted mode DEFAULT for DCI_CLRRXIRQ       */

/* Bit fields for DCI CLRTXIRQ */
#define _DCI_CLRTXIRQ_RESETVALUE                    0x00000000UL                          /**< Default value for DCI_CLRTXIRQ              */
#define _DCI_CLRTXIRQ_MASK                          0xFFFFFFFFUL                          /**< Mask for DCI_CLRTXIRQ                       */
#define _DCI_CLRTXIRQ_CLRTXIRQ_SHIFT                0                                     /**< Shift value for DCI_CLRTXIRQ                */
#define _DCI_CLRTXIRQ_CLRTXIRQ_MASK                 0xFFFFFFFFUL                          /**< Bit mask for DCI_CLRTXIRQ                   */
#define _DCI_CLRTXIRQ_CLRTXIRQ_DEFAULT              0x00000000UL                          /**< Mode DEFAULT for DCI_CLRTXIRQ               */
#define DCI_CLRTXIRQ_CLRTXIRQ_DEFAULT               (_DCI_CLRTXIRQ_CLRTXIRQ_DEFAULT << 0) /**< Shifted mode DEFAULT for DCI_CLRTXIRQ       */

/* Bit fields for DCI DATAAVAILABLE */
#define _DCI_DATAAVAILABLE_RESETVALUE               0x00000000UL                                    /**< Default value for DCI_DATAAVAILABLE         */
#define _DCI_DATAAVAILABLE_MASK                     0x00000001UL                                    /**< Mask for DCI_DATAAVAILABLE                  */
#define DCI_DATAAVAILABLE_DATAAVAILABLE             (0x1UL << 0)                                    /**< New BitField                                */
#define _DCI_DATAAVAILABLE_DATAAVAILABLE_SHIFT      0                                               /**< Shift value for DCI_DATAAVAILABLE           */
#define _DCI_DATAAVAILABLE_DATAAVAILABLE_MASK       0x1UL                                           /**< Bit mask for DCI_DATAAVAILABLE              */
#define _DCI_DATAAVAILABLE_DATAAVAILABLE_DEFAULT    0x00000000UL                                    /**< Mode DEFAULT for DCI_DATAAVAILABLE          */
#define DCI_DATAAVAILABLE_DATAAVAILABLE_DEFAULT     (_DCI_DATAAVAILABLE_DATAAVAILABLE_DEFAULT << 0) /**< Shifted mode DEFAULT for DCI_DATAAVAILABLE  */

/* Bit fields for DCI SENDINGDATA */
#define _DCI_SENDINGDATA_RESETVALUE                 0x00000000UL                                /**< Default value for DCI_SENDINGDATA           */
#define _DCI_SENDINGDATA_MASK                       0x00000001UL                                /**< Mask for DCI_SENDINGDATA                    */
#define DCI_SENDINGDATA_SENDINGDATA                 (0x1UL << 0)                                /**< SENDINGDATA                                 */
#define _DCI_SENDINGDATA_SENDINGDATA_SHIFT          0                                           /**< Shift value for DCI_SENDINGDATA             */
#define _DCI_SENDINGDATA_SENDINGDATA_MASK           0x1UL                                       /**< Bit mask for DCI_SENDINGDATA                */
#define _DCI_SENDINGDATA_SENDINGDATA_DEFAULT        0x00000000UL                                /**< Mode DEFAULT for DCI_SENDINGDATA            */
#define DCI_SENDINGDATA_SENDINGDATA_DEFAULT         (_DCI_SENDINGDATA_SENDINGDATA_DEFAULT << 0) /**< Shifted mode DEFAULT for DCI_SENDINGDATA    */

/* Bit fields for DCI RXSTATE */
#define _DCI_RXSTATE_RESETVALUE                     0x00000000UL                        /**< Default value for DCI_RXSTATE               */
#define _DCI_RXSTATE_MASK                           0x00000007UL                        /**< Mask for DCI_RXSTATE                        */
#define _DCI_RXSTATE_RXSTATE_SHIFT                  0                                   /**< Shift value for DCI_RXSTATE                 */
#define _DCI_RXSTATE_RXSTATE_MASK                   0x7UL                               /**< Bit mask for DCI_RXSTATE                    */
#define _DCI_RXSTATE_RXSTATE_DEFAULT                0x00000000UL                        /**< Mode DEFAULT for DCI_RXSTATE                */
#define DCI_RXSTATE_RXSTATE_DEFAULT                 (_DCI_RXSTATE_RXSTATE_DEFAULT << 0) /**< Shifted mode DEFAULT for DCI_RXSTATE        */

/* Bit fields for DCI TXSTATE */
#define _DCI_TXSTATE_RESETVALUE                     0x00000000UL                        /**< Default value for DCI_TXSTATE               */
#define _DCI_TXSTATE_MASK                           0x00000007UL                        /**< Mask for DCI_TXSTATE                        */
#define _DCI_TXSTATE_TXSTATE_SHIFT                  0                                   /**< Shift value for DCI_TXSTATE                 */
#define _DCI_TXSTATE_TXSTATE_MASK                   0x7UL                               /**< Bit mask for DCI_TXSTATE                    */
#define _DCI_TXSTATE_TXSTATE_DEFAULT                0x00000000UL                        /**< Mode DEFAULT for DCI_TXSTATE                */
#define DCI_TXSTATE_TXSTATE_DEFAULT                 (_DCI_TXSTATE_TXSTATE_DEFAULT << 0) /**< Shifted mode DEFAULT for DCI_TXSTATE        */

/** @} End of group EFR32BG22_DCI_MESSAGING_BitFields */
/** @} End of group EFR32BG22_DCI_MESSAGING */
/** @} End of group Parts */
#endif /* EFR32BG22_DCI_H */
