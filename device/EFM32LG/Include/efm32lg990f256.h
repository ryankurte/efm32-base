/**************************************************************************//**
 * @file efm32lg990f256.h
 * @brief CMSIS Cortex-M Peripheral Access Layer Header File
 *        for EFM32LG990F256
 * @version 3.20.7
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Laboratories, Inc. http://www.silabs.com</b>
 ******************************************************************************
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software.@n
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.@n
 * 3. This notice may not be removed or altered from any source distribution.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Laboratories, Inc.
 * has no obligation to support this Software. Silicon Laboratories, Inc. is
 * providing the Software "AS IS", with no express or implied warranties of any
 * kind, including, but not limited to, any implied warranties of
 * merchantability or fitness for any particular purpose or warranties against
 * infringement of any proprietary rights of a third party.
 *
 * Silicon Laboratories, Inc. will not be liable for any consequential,
 * incidental, or special damages, or any other relief, or for any claim by
 * any third party, arising from your use of this Software.
 *
 *****************************************************************************/

#ifndef __EFM32LG990F256_H
#define __EFM32LG990F256_H

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
 * @addtogroup Parts
 * @{
 *****************************************************************************/

/**************************************************************************//**
 * @defgroup EFM32LG990F256 EFM32LG990F256
 * @{
 *****************************************************************************/

/** Interrupt Number Definition */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers *******************************************/
  NonMaskableInt_IRQn   = -14,              /*!< 2 Non Maskable Interrupt                 */
  HardFault_IRQn        = -13,              /*!< 3 Cortex-M3 Hard Fault Interrupt         */
  MemoryManagement_IRQn = -12,              /*!< 4 Cortex-M3 Memory Management Interrupt  */
  BusFault_IRQn         = -11,              /*!< 5 Cortex-M3 Bus Fault Interrupt          */
  UsageFault_IRQn       = -10,              /*!< 6 Cortex-M3 Usage Fault Interrupt        */
  SVCall_IRQn           = -5,               /*!< 11 Cortex-M3 SV Call Interrupt           */
  DebugMonitor_IRQn     = -4,               /*!< 12 Cortex-M3 Debug Monitor Interrupt     */
  PendSV_IRQn           = -2,               /*!< 14 Cortex-M3 Pend SV Interrupt           */
  SysTick_IRQn          = -1,               /*!< 15 Cortex-M3 System Tick Interrupt       */

/******  EFM32LG Peripheral Interrupt Numbers *********************************************/

  DMA_IRQn              = 0,  /*!< 16+0 EFM32 DMA Interrupt */
  GPIO_EVEN_IRQn        = 1,  /*!< 16+1 EFM32 GPIO_EVEN Interrupt */
  TIMER0_IRQn           = 2,  /*!< 16+2 EFM32 TIMER0 Interrupt */
  USART0_RX_IRQn        = 3,  /*!< 16+3 EFM32 USART0_RX Interrupt */
  USART0_TX_IRQn        = 4,  /*!< 16+4 EFM32 USART0_TX Interrupt */
  USB_IRQn              = 5,  /*!< 16+5 EFM32 USB Interrupt */
  ACMP0_IRQn            = 6,  /*!< 16+6 EFM32 ACMP0 Interrupt */
  ADC0_IRQn             = 7,  /*!< 16+7 EFM32 ADC0 Interrupt */
  DAC0_IRQn             = 8,  /*!< 16+8 EFM32 DAC0 Interrupt */
  I2C0_IRQn             = 9,  /*!< 16+9 EFM32 I2C0 Interrupt */
  I2C1_IRQn             = 10, /*!< 16+10 EFM32 I2C1 Interrupt */
  GPIO_ODD_IRQn         = 11, /*!< 16+11 EFM32 GPIO_ODD Interrupt */
  TIMER1_IRQn           = 12, /*!< 16+12 EFM32 TIMER1 Interrupt */
  TIMER2_IRQn           = 13, /*!< 16+13 EFM32 TIMER2 Interrupt */
  TIMER3_IRQn           = 14, /*!< 16+14 EFM32 TIMER3 Interrupt */
  USART1_RX_IRQn        = 15, /*!< 16+15 EFM32 USART1_RX Interrupt */
  USART1_TX_IRQn        = 16, /*!< 16+16 EFM32 USART1_TX Interrupt */
  LESENSE_IRQn          = 17, /*!< 16+17 EFM32 LESENSE Interrupt */
  USART2_RX_IRQn        = 18, /*!< 16+18 EFM32 USART2_RX Interrupt */
  USART2_TX_IRQn        = 19, /*!< 16+19 EFM32 USART2_TX Interrupt */
  UART0_RX_IRQn         = 20, /*!< 16+20 EFM32 UART0_RX Interrupt */
  UART0_TX_IRQn         = 21, /*!< 16+21 EFM32 UART0_TX Interrupt */
  UART1_RX_IRQn         = 22, /*!< 16+22 EFM32 UART1_RX Interrupt */
  UART1_TX_IRQn         = 23, /*!< 16+23 EFM32 UART1_TX Interrupt */
  LEUART0_IRQn          = 24, /*!< 16+24 EFM32 LEUART0 Interrupt */
  LEUART1_IRQn          = 25, /*!< 16+25 EFM32 LEUART1 Interrupt */
  LETIMER0_IRQn         = 26, /*!< 16+26 EFM32 LETIMER0 Interrupt */
  PCNT0_IRQn            = 27, /*!< 16+27 EFM32 PCNT0 Interrupt */
  PCNT1_IRQn            = 28, /*!< 16+28 EFM32 PCNT1 Interrupt */
  PCNT2_IRQn            = 29, /*!< 16+29 EFM32 PCNT2 Interrupt */
  RTC_IRQn              = 30, /*!< 16+30 EFM32 RTC Interrupt */
  BURTC_IRQn            = 31, /*!< 16+31 EFM32 BURTC Interrupt */
  CMU_IRQn              = 32, /*!< 16+32 EFM32 CMU Interrupt */
  VCMP_IRQn             = 33, /*!< 16+33 EFM32 VCMP Interrupt */
  LCD_IRQn              = 34, /*!< 16+34 EFM32 LCD Interrupt */
  MSC_IRQn              = 35, /*!< 16+35 EFM32 MSC Interrupt */
  AES_IRQn              = 36, /*!< 16+36 EFM32 AES Interrupt */
  EBI_IRQn              = 37, /*!< 16+37 EFM32 EBI Interrupt */
  EMU_IRQn              = 38, /*!< 16+38 EFM32 EMU Interrupt */
} IRQn_Type;

/**************************************************************************//**
 * @defgroup EFM32LG990F256_Core EFM32LG990F256 Core
 * @{
 * @brief Processor and Core Peripheral Section
 *****************************************************************************/
#define __MPU_PRESENT             1 /**< Presence of MPU  */
#define __NVIC_PRIO_BITS          3 /**< NVIC interrupt priority bits */
#define __Vendor_SysTickConfig    0 /**< Is 1 if different SysTick counter is used */

/** @} End of group EFM32LG990F256_Core */

/**************************************************************************//**
* @defgroup EFM32LG990F256_Part EFM32LG990F256 Part
* @{
******************************************************************************/

/** Part family */
#define _EFM32_GIANT_FAMILY    1 /**< Giant/Leopard Gecko EFM32LG/GG MCU Family */
#define _EFM_DEVICE              /**< Silicon Labs EFM-type microcontroller */

/* If part number is not defined as compiler option, define it */
#if !defined(EFM32LG990F256)
#define EFM32LG990F256    1 /**< Giant/Leopard Gecko Part  */
#endif

/** Configure part number */
#define PART_NUMBER          "EFM32LG990F256" /**< Part Number */

/** Memory Base addresses and limits */
#define FLASH_MEM_BASE       ((uint32_t) 0x0UL)        /**< FLASH base address  */
#define FLASH_MEM_SIZE       ((uint32_t) 0x10000000UL) /**< FLASH available address space  */
#define FLASH_MEM_END        ((uint32_t) 0xFFFFFFFUL)  /**< FLASH end address  */
#define FLASH_MEM_BITS       ((uint32_t) 0x28UL)       /**< FLASH used bits  */
#define AES_MEM_BASE         ((uint32_t) 0x400E0000UL) /**< AES base address  */
#define AES_MEM_SIZE         ((uint32_t) 0x400UL)      /**< AES available address space  */
#define AES_MEM_END          ((uint32_t) 0x400E03FFUL) /**< AES end address  */
#define AES_MEM_BITS         ((uint32_t) 0x10UL)       /**< AES used bits  */
#define USBC_MEM_BASE        ((uint32_t) 0x40100000UL) /**< USBC base address  */
#define USBC_MEM_SIZE        ((uint32_t) 0x40000UL)    /**< USBC available address space  */
#define USBC_MEM_END         ((uint32_t) 0x4013FFFFUL) /**< USBC end address  */
#define USBC_MEM_BITS        ((uint32_t) 0x18UL)       /**< USBC used bits  */
#define EBI_CODE_MEM_BASE    ((uint32_t) 0x12000000UL) /**< EBI_CODE base address  */
#define EBI_CODE_MEM_SIZE    ((uint32_t) 0xE000000UL)  /**< EBI_CODE available address space  */
#define EBI_CODE_MEM_END     ((uint32_t) 0x1FFFFFFFUL) /**< EBI_CODE end address  */
#define EBI_CODE_MEM_BITS    ((uint32_t) 0x28UL)       /**< EBI_CODE used bits  */
#define PER_MEM_BASE         ((uint32_t) 0x40000000UL) /**< PER base address  */
#define PER_MEM_SIZE         ((uint32_t) 0xE0000UL)    /**< PER available address space  */
#define PER_MEM_END          ((uint32_t) 0x400DFFFFUL) /**< PER end address  */
#define PER_MEM_BITS         ((uint32_t) 0x20UL)       /**< PER used bits  */
#define RAM_MEM_BASE         ((uint32_t) 0x20000000UL) /**< RAM base address  */
#define RAM_MEM_SIZE         ((uint32_t) 0x40000UL)    /**< RAM available address space  */
#define RAM_MEM_END          ((uint32_t) 0x2003FFFFUL) /**< RAM end address  */
#define RAM_MEM_BITS         ((uint32_t) 0x18UL)       /**< RAM used bits  */
#define RAM_CODE_MEM_BASE    ((uint32_t) 0x10000000UL) /**< RAM_CODE base address  */
#define RAM_CODE_MEM_SIZE    ((uint32_t) 0x20000UL)    /**< RAM_CODE available address space  */
#define RAM_CODE_MEM_END     ((uint32_t) 0x1001FFFFUL) /**< RAM_CODE end address  */
#define RAM_CODE_MEM_BITS    ((uint32_t) 0x17UL)       /**< RAM_CODE used bits  */
#define EBI_MEM_BASE         ((uint32_t) 0x80000000UL) /**< EBI base address  */
#define EBI_MEM_SIZE         ((uint32_t) 0x40000000UL) /**< EBI available address space  */
#define EBI_MEM_END          ((uint32_t) 0xBFFFFFFFUL) /**< EBI end address  */
#define EBI_MEM_BITS         ((uint32_t) 0x30UL)       /**< EBI used bits  */

/** Bit banding area */
#define BITBAND_PER_BASE     ((uint32_t) 0x42000000UL) /**< Peripheral Address Space bit-band area */
#define BITBAND_RAM_BASE     ((uint32_t) 0x22000000UL) /**< SRAM Address Space bit-band area */

/** Flash and SRAM limits for EFM32LG990F256 */
#define FLASH_BASE           (0x00000000UL) /**< Flash Base Address */
#define FLASH_SIZE           (0x00040000UL) /**< Available Flash Memory */
#define FLASH_PAGE_SIZE      2048           /**< Flash Memory page size */
#define SRAM_BASE            (0x20000000UL) /**< SRAM Base Address */
#define SRAM_SIZE            (0x00008000UL) /**< Available SRAM Memory */
#define __CM3_REV            0x201          /**< Cortex-M3 Core revision r2p1 */
#define PRS_CHAN_COUNT       12             /**< Number of PRS channels */
#define DMA_CHAN_COUNT       12             /**< Number of DMA channels */

/** AF channels connect the different on-chip peripherals with the af-mux */
#define AFCHAN_MAX           163
#define AFCHANLOC_MAX        7
/** Analog AF channels */
#define AFACHAN_MAX          53

/* Part number capabilities */

#define USART_PRESENT         /**< USART is available in this part */
#define USART_COUNT         3 /**< 3 USARTs available  */
#define UART_PRESENT          /**< UART is available in this part */
#define UART_COUNT          2 /**< 2 UARTs available  */
#define TIMER_PRESENT         /**< TIMER is available in this part */
#define TIMER_COUNT         4 /**< 4 TIMERs available  */
#define ACMP_PRESENT          /**< ACMP is available in this part */
#define ACMP_COUNT          2 /**< 2 ACMPs available  */
#define LEUART_PRESENT        /**< LEUART is available in this part */
#define LEUART_COUNT        2 /**< 2 LEUARTs available  */
#define LETIMER_PRESENT       /**< LETIMER is available in this part */
#define LETIMER_COUNT       1 /**< 1 LETIMERs available  */
#define PCNT_PRESENT          /**< PCNT is available in this part */
#define PCNT_COUNT          3 /**< 3 PCNTs available  */
#define I2C_PRESENT           /**< I2C is available in this part */
#define I2C_COUNT           2 /**< 2 I2Cs available  */
#define ADC_PRESENT           /**< ADC is available in this part */
#define ADC_COUNT           1 /**< 1 ADCs available  */
#define DAC_PRESENT           /**< DAC is available in this part */
#define DAC_COUNT           1 /**< 1 DACs available  */
#define DMA_PRESENT
#define DMA_COUNT           1
#define AES_PRESENT
#define AES_COUNT           1
#define USBC_PRESENT
#define USBC_COUNT          1
#define USB_PRESENT
#define USB_COUNT           1
#define LE_PRESENT
#define LE_COUNT            1
#define MSC_PRESENT
#define MSC_COUNT           1
#define EMU_PRESENT
#define EMU_COUNT           1
#define RMU_PRESENT
#define RMU_COUNT           1
#define CMU_PRESENT
#define CMU_COUNT           1
#define LESENSE_PRESENT
#define LESENSE_COUNT       1
#define EBI_PRESENT
#define EBI_COUNT           1
#define RTC_PRESENT
#define RTC_COUNT           1
#define GPIO_PRESENT
#define GPIO_COUNT          1
#define VCMP_PRESENT
#define VCMP_COUNT          1
#define PRS_PRESENT
#define PRS_COUNT           1
#define OPAMP_PRESENT
#define OPAMP_COUNT         1
#define BU_PRESENT
#define BU_COUNT            1
#define LCD_PRESENT
#define LCD_COUNT           1
#define BURTC_PRESENT
#define BURTC_COUNT         1
#define HFXTAL_PRESENT
#define HFXTAL_COUNT        1
#define LFXTAL_PRESENT
#define LFXTAL_COUNT        1
#define WDOG_PRESENT
#define WDOG_COUNT          1
#define DBG_PRESENT
#define DBG_COUNT           1
#define ETM_PRESENT
#define ETM_COUNT           1
#define BOOTLOADER_PRESENT
#define BOOTLOADER_COUNT    1
#define ANALOG_PRESENT
#define ANALOG_COUNT        1

#include "core_cm3.h"       /* Cortex-M3 processor and core peripherals */
#include "system_efm32lg.h" /* System Header */

/** @} End of group EFM32LG990F256_Part */

/**************************************************************************//**
 * @defgroup EFM32LG990F256_Peripheral_TypeDefs EFM32LG990F256 Peripheral TypeDefs
 * @{
 * @brief Device Specific Peripheral Register Structures
 *****************************************************************************/

#include "efm32lg_dma_ch.h"
#include "efm32lg_dma.h"
#include "efm32lg_aes.h"
#include "efm32lg_usb_hc.h"
#include "efm32lg_usb_diep.h"
#include "efm32lg_usb_doep.h"
#include "efm32lg_usb.h"
#include "efm32lg_msc.h"

/**************************************************************************//**
 * @defgroup EFM32LG990F256_EMU EFM32LG990F256 EMU
 * @{
 * @brief EFM32LG990F256_EMU Register Declaration
 *****************************************************************************/
typedef struct
{
  __IO uint32_t CTRL;          /**< Control Register  */

  uint32_t      RESERVED0[1];  /**< Reserved for future use **/
  __IO uint32_t LOCK;          /**< Configuration Lock Register  */

  uint32_t      RESERVED1[6];  /**< Reserved for future use **/
  __IO uint32_t AUXCTRL;       /**< Auxiliary Control Register  */

  uint32_t      RESERVED2[1];  /**< Reserved for future use **/
  __IO uint32_t EM4CONF;       /**< Energy mode 4 configuration register  */
  __IO uint32_t BUCTRL;        /**< Backup Power configuration register  */
  __IO uint32_t PWRCONF;       /**< Power connection configuration register  */
  __IO uint32_t BUINACT;       /**< Backup mode inactive configuration register  */
  __IO uint32_t BUACT;         /**< Backup mode active configuration register  */
  __I uint32_t  STATUS;        /**< Status register  */
  __IO uint32_t ROUTE;         /**< I/O Routing Register  */
  __I uint32_t  IF;            /**< Interrupt Flag Register  */
  __IO uint32_t IFS;           /**< Interrupt Flag Set Register  */
  __IO uint32_t IFC;           /**< Interrupt Flag Clear Register  */
  __IO uint32_t IEN;           /**< Interrupt Enable Register  */
  __IO uint32_t BUBODBUVINCAL; /**< BU_VIN Backup BOD calibration  */
  __IO uint32_t BUBODUNREGCAL; /**< Unregulated power Backup BOD calibration  */
} EMU_TypeDef;                 /** @} */

#include "efm32lg_rmu.h"
#include "efm32lg_cmu.h"
#include "efm32lg_lesense_st.h"
#include "efm32lg_lesense_buf.h"
#include "efm32lg_lesense_ch.h"
#include "efm32lg_lesense.h"
#include "efm32lg_ebi.h"

/**************************************************************************//**
 * @defgroup EFM32LG990F256_USART EFM32LG990F256 USART
 * @{
 * @brief EFM32LG990F256_USART Register Declaration
 *****************************************************************************/
typedef struct
{
  __IO uint32_t CTRL;       /**< Control Register  */
  __IO uint32_t FRAME;      /**< USART Frame Format Register  */
  __IO uint32_t TRIGCTRL;   /**< USART Trigger Control register  */
  __IO uint32_t CMD;        /**< Command Register  */
  __I uint32_t  STATUS;     /**< USART Status Register  */
  __IO uint32_t CLKDIV;     /**< Clock Control Register  */
  __I uint32_t  RXDATAX;    /**< RX Buffer Data Extended Register  */
  __I uint32_t  RXDATA;     /**< RX Buffer Data Register  */
  __I uint32_t  RXDOUBLEX;  /**< RX Buffer Double Data Extended Register  */
  __I uint32_t  RXDOUBLE;   /**< RX FIFO Double Data Register  */
  __I uint32_t  RXDATAXP;   /**< RX Buffer Data Extended Peek Register  */
  __I uint32_t  RXDOUBLEXP; /**< RX Buffer Double Data Extended Peek Register  */
  __IO uint32_t TXDATAX;    /**< TX Buffer Data Extended Register  */
  __IO uint32_t TXDATA;     /**< TX Buffer Data Register  */
  __IO uint32_t TXDOUBLEX;  /**< TX Buffer Double Data Extended Register  */
  __IO uint32_t TXDOUBLE;   /**< TX Buffer Double Data Register  */
  __I uint32_t  IF;         /**< Interrupt Flag Register  */
  __IO uint32_t IFS;        /**< Interrupt Flag Set Register  */
  __IO uint32_t IFC;        /**< Interrupt Flag Clear Register  */
  __IO uint32_t IEN;        /**< Interrupt Enable Register  */
  __IO uint32_t IRCTRL;     /**< IrDA Control Register  */
  __IO uint32_t ROUTE;      /**< I/O Routing Register  */
  __IO uint32_t INPUT;      /**< USART Input Register  */
  __IO uint32_t I2SCTRL;    /**< I2S Control Register  */
} USART_TypeDef;            /** @} */

#include "efm32lg_timer_cc.h"
#include "efm32lg_timer.h"
#include "efm32lg_acmp.h"
#include "efm32lg_leuart.h"
#include "efm32lg_rtc.h"
#include "efm32lg_letimer.h"
#include "efm32lg_pcnt.h"
#include "efm32lg_i2c.h"
#include "efm32lg_gpio_p.h"
#include "efm32lg_gpio.h"
#include "efm32lg_vcmp.h"
#include "efm32lg_prs_ch.h"
#include "efm32lg_prs.h"
#include "efm32lg_adc.h"
#include "efm32lg_dac.h"
#include "efm32lg_lcd.h"
#include "efm32lg_burtc_ret.h"
#include "efm32lg_burtc.h"
#include "efm32lg_wdog.h"
#include "efm32lg_etm.h"
#include "efm32lg_dma_descriptor.h"
#include "efm32lg_devinfo.h"
#include "efm32lg_romtable.h"
#include "efm32lg_calibrate.h"

/** @} End of group EFM32LG990F256_Peripheral_TypeDefs */

/**************************************************************************//**
 * @defgroup EFM32LG990F256_Peripheral_Base EFM32LG990F256 Peripheral Memory Map
 * @{
 *****************************************************************************/

#define DMA_BASE          (0x400C2000UL) /**< DMA base address  */
#define AES_BASE          (0x400E0000UL) /**< AES base address  */
#define USB_BASE          (0x400C4000UL) /**< USB base address  */
#define MSC_BASE          (0x400C0000UL) /**< MSC base address  */
#define EMU_BASE          (0x400C6000UL) /**< EMU base address  */
#define RMU_BASE          (0x400CA000UL) /**< RMU base address  */
#define CMU_BASE          (0x400C8000UL) /**< CMU base address  */
#define LESENSE_BASE      (0x4008C000UL) /**< LESENSE base address  */
#define EBI_BASE          (0x40008000UL) /**< EBI base address  */
#define USART0_BASE       (0x4000C000UL) /**< USART0 base address  */
#define USART1_BASE       (0x4000C400UL) /**< USART1 base address  */
#define USART2_BASE       (0x4000C800UL) /**< USART2 base address  */
#define UART0_BASE        (0x4000E000UL) /**< UART0 base address  */
#define UART1_BASE        (0x4000E400UL) /**< UART1 base address  */
#define TIMER0_BASE       (0x40010000UL) /**< TIMER0 base address  */
#define TIMER1_BASE       (0x40010400UL) /**< TIMER1 base address  */
#define TIMER2_BASE       (0x40010800UL) /**< TIMER2 base address  */
#define TIMER3_BASE       (0x40010C00UL) /**< TIMER3 base address  */
#define ACMP0_BASE        (0x40001000UL) /**< ACMP0 base address  */
#define ACMP1_BASE        (0x40001400UL) /**< ACMP1 base address  */
#define LEUART0_BASE      (0x40084000UL) /**< LEUART0 base address  */
#define LEUART1_BASE      (0x40084400UL) /**< LEUART1 base address  */
#define RTC_BASE          (0x40080000UL) /**< RTC base address  */
#define LETIMER0_BASE     (0x40082000UL) /**< LETIMER0 base address  */
#define PCNT0_BASE        (0x40086000UL) /**< PCNT0 base address  */
#define PCNT1_BASE        (0x40086400UL) /**< PCNT1 base address  */
#define PCNT2_BASE        (0x40086800UL) /**< PCNT2 base address  */
#define I2C0_BASE         (0x4000A000UL) /**< I2C0 base address  */
#define I2C1_BASE         (0x4000A400UL) /**< I2C1 base address  */
#define GPIO_BASE         (0x40006000UL) /**< GPIO base address  */
#define VCMP_BASE         (0x40000000UL) /**< VCMP base address  */
#define PRS_BASE          (0x400CC000UL) /**< PRS base address  */
#define ADC0_BASE         (0x40002000UL) /**< ADC0 base address  */
#define DAC0_BASE         (0x40004000UL) /**< DAC0 base address  */
#define LCD_BASE          (0x4008A000UL) /**< LCD base address  */
#define BURTC_BASE        (0x40081000UL) /**< BURTC base address  */
#define WDOG_BASE         (0x40088000UL) /**< WDOG base address  */
#define ETM_BASE          (0xE0041000UL) /**< ETM base address  */
#define CALIBRATE_BASE    (0x0FE08000UL) /**< CALIBRATE base address */
#define DEVINFO_BASE      (0x0FE081B0UL) /**< DEVINFO base address */
#define ROMTABLE_BASE     (0xE00FFFD0UL) /**< ROMTABLE base address */
#define LOCKBITS_BASE     (0x0FE04000UL) /**< Lock-bits page base address */
#define USERDATA_BASE     (0x0FE00000UL) /**< User data page base address */

/** @} End of group EFM32LG990F256_Peripheral_Base */

/**************************************************************************//**
 * @defgroup EFM32LG990F256_Peripheral_Declaration  EFM32LG990F256 Peripheral Declarations
 * @{
 *****************************************************************************/

#define DMA          ((DMA_TypeDef *) DMA_BASE)             /**< DMA base pointer */
#define AES          ((AES_TypeDef *) AES_BASE)             /**< AES base pointer */
#define USB          ((USB_TypeDef *) USB_BASE)             /**< USB base pointer */
#define MSC          ((MSC_TypeDef *) MSC_BASE)             /**< MSC base pointer */
#define EMU          ((EMU_TypeDef *) EMU_BASE)             /**< EMU base pointer */
#define RMU          ((RMU_TypeDef *) RMU_BASE)             /**< RMU base pointer */
#define CMU          ((CMU_TypeDef *) CMU_BASE)             /**< CMU base pointer */
#define LESENSE      ((LESENSE_TypeDef *) LESENSE_BASE)     /**< LESENSE base pointer */
#define EBI          ((EBI_TypeDef *) EBI_BASE)             /**< EBI base pointer */
#define USART0       ((USART_TypeDef *) USART0_BASE)        /**< USART0 base pointer */
#define USART1       ((USART_TypeDef *) USART1_BASE)        /**< USART1 base pointer */
#define USART2       ((USART_TypeDef *) USART2_BASE)        /**< USART2 base pointer */
#define UART0        ((USART_TypeDef *) UART0_BASE)         /**< UART0 base pointer */
#define UART1        ((USART_TypeDef *) UART1_BASE)         /**< UART1 base pointer */
#define TIMER0       ((TIMER_TypeDef *) TIMER0_BASE)        /**< TIMER0 base pointer */
#define TIMER1       ((TIMER_TypeDef *) TIMER1_BASE)        /**< TIMER1 base pointer */
#define TIMER2       ((TIMER_TypeDef *) TIMER2_BASE)        /**< TIMER2 base pointer */
#define TIMER3       ((TIMER_TypeDef *) TIMER3_BASE)        /**< TIMER3 base pointer */
#define ACMP0        ((ACMP_TypeDef *) ACMP0_BASE)          /**< ACMP0 base pointer */
#define ACMP1        ((ACMP_TypeDef *) ACMP1_BASE)          /**< ACMP1 base pointer */
#define LEUART0      ((LEUART_TypeDef *) LEUART0_BASE)      /**< LEUART0 base pointer */
#define LEUART1      ((LEUART_TypeDef *) LEUART1_BASE)      /**< LEUART1 base pointer */
#define RTC          ((RTC_TypeDef *) RTC_BASE)             /**< RTC base pointer */
#define LETIMER0     ((LETIMER_TypeDef *) LETIMER0_BASE)    /**< LETIMER0 base pointer */
#define PCNT0        ((PCNT_TypeDef *) PCNT0_BASE)          /**< PCNT0 base pointer */
#define PCNT1        ((PCNT_TypeDef *) PCNT1_BASE)          /**< PCNT1 base pointer */
#define PCNT2        ((PCNT_TypeDef *) PCNT2_BASE)          /**< PCNT2 base pointer */
#define I2C0         ((I2C_TypeDef *) I2C0_BASE)            /**< I2C0 base pointer */
#define I2C1         ((I2C_TypeDef *) I2C1_BASE)            /**< I2C1 base pointer */
#define GPIO         ((GPIO_TypeDef *) GPIO_BASE)           /**< GPIO base pointer */
#define VCMP         ((VCMP_TypeDef *) VCMP_BASE)           /**< VCMP base pointer */
#define PRS          ((PRS_TypeDef *) PRS_BASE)             /**< PRS base pointer */
#define ADC0         ((ADC_TypeDef *) ADC0_BASE)            /**< ADC0 base pointer */
#define DAC0         ((DAC_TypeDef *) DAC0_BASE)            /**< DAC0 base pointer */
#define LCD          ((LCD_TypeDef *) LCD_BASE)             /**< LCD base pointer */
#define BURTC        ((BURTC_TypeDef *) BURTC_BASE)         /**< BURTC base pointer */
#define WDOG         ((WDOG_TypeDef *) WDOG_BASE)           /**< WDOG base pointer */
#define ETM          ((ETM_TypeDef *) ETM_BASE)             /**< ETM base pointer */
#define CALIBRATE    ((CALIBRATE_TypeDef *) CALIBRATE_BASE) /**< CALIBRATE base pointer */
#define DEVINFO      ((DEVINFO_TypeDef *) DEVINFO_BASE)     /**< DEVINFO base pointer */
#define ROMTABLE     ((ROMTABLE_TypeDef *) ROMTABLE_BASE)   /**< ROMTABLE base pointer */

/** @} End of group EFM32LG990F256_Peripheral_Declaration */

/**************************************************************************//**
 * @defgroup EFM32LG990F256_BitFields EFM32LG990F256 Bit Fields
 * @{
 *****************************************************************************/

#include "efm32lg_prs_signals.h"
#include "efm32lg_dmareq.h"
#include "efm32lg_dmactrl.h"

/**************************************************************************//**
 * @defgroup EFM32LG990F256_USART_BitFields  EFM32LG990F256_USART Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for USART CTRL */
#define _USART_CTRL_RESETVALUE                0x00000000UL                             /**< Default value for USART_CTRL */
#define _USART_CTRL_MASK                      0x7DFFFF7FUL                             /**< Mask for USART_CTRL */
#define USART_CTRL_SYNC                       (0x1UL << 0)                             /**< USART Synchronous Mode */
#define _USART_CTRL_SYNC_SHIFT                0                                        /**< Shift value for USART_SYNC */
#define _USART_CTRL_SYNC_MASK                 0x1UL                                    /**< Bit mask for USART_SYNC */
#define _USART_CTRL_SYNC_DEFAULT              0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_SYNC_DEFAULT               (_USART_CTRL_SYNC_DEFAULT << 0)          /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_LOOPBK                     (0x1UL << 1)                             /**< Loopback Enable */
#define _USART_CTRL_LOOPBK_SHIFT              1                                        /**< Shift value for USART_LOOPBK */
#define _USART_CTRL_LOOPBK_MASK               0x2UL                                    /**< Bit mask for USART_LOOPBK */
#define _USART_CTRL_LOOPBK_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_LOOPBK_DEFAULT             (_USART_CTRL_LOOPBK_DEFAULT << 1)        /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_CCEN                       (0x1UL << 2)                             /**< Collision Check Enable */
#define _USART_CTRL_CCEN_SHIFT                2                                        /**< Shift value for USART_CCEN */
#define _USART_CTRL_CCEN_MASK                 0x4UL                                    /**< Bit mask for USART_CCEN */
#define _USART_CTRL_CCEN_DEFAULT              0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_CCEN_DEFAULT               (_USART_CTRL_CCEN_DEFAULT << 2)          /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_MPM                        (0x1UL << 3)                             /**< Multi-Processor Mode */
#define _USART_CTRL_MPM_SHIFT                 3                                        /**< Shift value for USART_MPM */
#define _USART_CTRL_MPM_MASK                  0x8UL                                    /**< Bit mask for USART_MPM */
#define _USART_CTRL_MPM_DEFAULT               0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_MPM_DEFAULT                (_USART_CTRL_MPM_DEFAULT << 3)           /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_MPAB                       (0x1UL << 4)                             /**< Multi-Processor Address-Bit */
#define _USART_CTRL_MPAB_SHIFT                4                                        /**< Shift value for USART_MPAB */
#define _USART_CTRL_MPAB_MASK                 0x10UL                                   /**< Bit mask for USART_MPAB */
#define _USART_CTRL_MPAB_DEFAULT              0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_MPAB_DEFAULT               (_USART_CTRL_MPAB_DEFAULT << 4)          /**< Shifted mode DEFAULT for USART_CTRL */
#define _USART_CTRL_OVS_SHIFT                 5                                        /**< Shift value for USART_OVS */
#define _USART_CTRL_OVS_MASK                  0x60UL                                   /**< Bit mask for USART_OVS */
#define _USART_CTRL_OVS_DEFAULT               0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define _USART_CTRL_OVS_X16                   0x00000000UL                             /**< Mode X16 for USART_CTRL */
#define _USART_CTRL_OVS_X8                    0x00000001UL                             /**< Mode X8 for USART_CTRL */
#define _USART_CTRL_OVS_X6                    0x00000002UL                             /**< Mode X6 for USART_CTRL */
#define _USART_CTRL_OVS_X4                    0x00000003UL                             /**< Mode X4 for USART_CTRL */
#define USART_CTRL_OVS_DEFAULT                (_USART_CTRL_OVS_DEFAULT << 5)           /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_OVS_X16                    (_USART_CTRL_OVS_X16 << 5)               /**< Shifted mode X16 for USART_CTRL */
#define USART_CTRL_OVS_X8                     (_USART_CTRL_OVS_X8 << 5)                /**< Shifted mode X8 for USART_CTRL */
#define USART_CTRL_OVS_X6                     (_USART_CTRL_OVS_X6 << 5)                /**< Shifted mode X6 for USART_CTRL */
#define USART_CTRL_OVS_X4                     (_USART_CTRL_OVS_X4 << 5)                /**< Shifted mode X4 for USART_CTRL */
#define USART_CTRL_CLKPOL                     (0x1UL << 8)                             /**< Clock Polarity */
#define _USART_CTRL_CLKPOL_SHIFT              8                                        /**< Shift value for USART_CLKPOL */
#define _USART_CTRL_CLKPOL_MASK               0x100UL                                  /**< Bit mask for USART_CLKPOL */
#define _USART_CTRL_CLKPOL_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define _USART_CTRL_CLKPOL_IDLELOW            0x00000000UL                             /**< Mode IDLELOW for USART_CTRL */
#define _USART_CTRL_CLKPOL_IDLEHIGH           0x00000001UL                             /**< Mode IDLEHIGH for USART_CTRL */
#define USART_CTRL_CLKPOL_DEFAULT             (_USART_CTRL_CLKPOL_DEFAULT << 8)        /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_CLKPOL_IDLELOW             (_USART_CTRL_CLKPOL_IDLELOW << 8)        /**< Shifted mode IDLELOW for USART_CTRL */
#define USART_CTRL_CLKPOL_IDLEHIGH            (_USART_CTRL_CLKPOL_IDLEHIGH << 8)       /**< Shifted mode IDLEHIGH for USART_CTRL */
#define USART_CTRL_CLKPHA                     (0x1UL << 9)                             /**< Clock Edge For Setup/Sample */
#define _USART_CTRL_CLKPHA_SHIFT              9                                        /**< Shift value for USART_CLKPHA */
#define _USART_CTRL_CLKPHA_MASK               0x200UL                                  /**< Bit mask for USART_CLKPHA */
#define _USART_CTRL_CLKPHA_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define _USART_CTRL_CLKPHA_SAMPLELEADING      0x00000000UL                             /**< Mode SAMPLELEADING for USART_CTRL */
#define _USART_CTRL_CLKPHA_SAMPLETRAILING     0x00000001UL                             /**< Mode SAMPLETRAILING for USART_CTRL */
#define USART_CTRL_CLKPHA_DEFAULT             (_USART_CTRL_CLKPHA_DEFAULT << 9)        /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_CLKPHA_SAMPLELEADING       (_USART_CTRL_CLKPHA_SAMPLELEADING << 9)  /**< Shifted mode SAMPLELEADING for USART_CTRL */
#define USART_CTRL_CLKPHA_SAMPLETRAILING      (_USART_CTRL_CLKPHA_SAMPLETRAILING << 9) /**< Shifted mode SAMPLETRAILING for USART_CTRL */
#define USART_CTRL_MSBF                       (0x1UL << 10)                            /**< Most Significant Bit First */
#define _USART_CTRL_MSBF_SHIFT                10                                       /**< Shift value for USART_MSBF */
#define _USART_CTRL_MSBF_MASK                 0x400UL                                  /**< Bit mask for USART_MSBF */
#define _USART_CTRL_MSBF_DEFAULT              0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_MSBF_DEFAULT               (_USART_CTRL_MSBF_DEFAULT << 10)         /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_CSMA                       (0x1UL << 11)                            /**< Action On Slave-Select In Master Mode */
#define _USART_CTRL_CSMA_SHIFT                11                                       /**< Shift value for USART_CSMA */
#define _USART_CTRL_CSMA_MASK                 0x800UL                                  /**< Bit mask for USART_CSMA */
#define _USART_CTRL_CSMA_DEFAULT              0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define _USART_CTRL_CSMA_NOACTION             0x00000000UL                             /**< Mode NOACTION for USART_CTRL */
#define _USART_CTRL_CSMA_GOTOSLAVEMODE        0x00000001UL                             /**< Mode GOTOSLAVEMODE for USART_CTRL */
#define USART_CTRL_CSMA_DEFAULT               (_USART_CTRL_CSMA_DEFAULT << 11)         /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_CSMA_NOACTION              (_USART_CTRL_CSMA_NOACTION << 11)        /**< Shifted mode NOACTION for USART_CTRL */
#define USART_CTRL_CSMA_GOTOSLAVEMODE         (_USART_CTRL_CSMA_GOTOSLAVEMODE << 11)   /**< Shifted mode GOTOSLAVEMODE for USART_CTRL */
#define USART_CTRL_TXBIL                      (0x1UL << 12)                            /**< TX Buffer Interrupt Level */
#define _USART_CTRL_TXBIL_SHIFT               12                                       /**< Shift value for USART_TXBIL */
#define _USART_CTRL_TXBIL_MASK                0x1000UL                                 /**< Bit mask for USART_TXBIL */
#define _USART_CTRL_TXBIL_DEFAULT             0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define _USART_CTRL_TXBIL_EMPTY               0x00000000UL                             /**< Mode EMPTY for USART_CTRL */
#define _USART_CTRL_TXBIL_HALFFULL            0x00000001UL                             /**< Mode HALFFULL for USART_CTRL */
#define USART_CTRL_TXBIL_DEFAULT              (_USART_CTRL_TXBIL_DEFAULT << 12)        /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_TXBIL_EMPTY                (_USART_CTRL_TXBIL_EMPTY << 12)          /**< Shifted mode EMPTY for USART_CTRL */
#define USART_CTRL_TXBIL_HALFFULL             (_USART_CTRL_TXBIL_HALFFULL << 12)       /**< Shifted mode HALFFULL for USART_CTRL */
#define USART_CTRL_RXINV                      (0x1UL << 13)                            /**< Receiver Input Invert */
#define _USART_CTRL_RXINV_SHIFT               13                                       /**< Shift value for USART_RXINV */
#define _USART_CTRL_RXINV_MASK                0x2000UL                                 /**< Bit mask for USART_RXINV */
#define _USART_CTRL_RXINV_DEFAULT             0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_RXINV_DEFAULT              (_USART_CTRL_RXINV_DEFAULT << 13)        /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_TXINV                      (0x1UL << 14)                            /**< Transmitter output Invert */
#define _USART_CTRL_TXINV_SHIFT               14                                       /**< Shift value for USART_TXINV */
#define _USART_CTRL_TXINV_MASK                0x4000UL                                 /**< Bit mask for USART_TXINV */
#define _USART_CTRL_TXINV_DEFAULT             0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_TXINV_DEFAULT              (_USART_CTRL_TXINV_DEFAULT << 14)        /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_CSINV                      (0x1UL << 15)                            /**< Chip Select Invert */
#define _USART_CTRL_CSINV_SHIFT               15                                       /**< Shift value for USART_CSINV */
#define _USART_CTRL_CSINV_MASK                0x8000UL                                 /**< Bit mask for USART_CSINV */
#define _USART_CTRL_CSINV_DEFAULT             0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_CSINV_DEFAULT              (_USART_CTRL_CSINV_DEFAULT << 15)        /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_AUTOCS                     (0x1UL << 16)                            /**< Automatic Chip Select */
#define _USART_CTRL_AUTOCS_SHIFT              16                                       /**< Shift value for USART_AUTOCS */
#define _USART_CTRL_AUTOCS_MASK               0x10000UL                                /**< Bit mask for USART_AUTOCS */
#define _USART_CTRL_AUTOCS_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_AUTOCS_DEFAULT             (_USART_CTRL_AUTOCS_DEFAULT << 16)       /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_AUTOTRI                    (0x1UL << 17)                            /**< Automatic TX Tristate */
#define _USART_CTRL_AUTOTRI_SHIFT             17                                       /**< Shift value for USART_AUTOTRI */
#define _USART_CTRL_AUTOTRI_MASK              0x20000UL                                /**< Bit mask for USART_AUTOTRI */
#define _USART_CTRL_AUTOTRI_DEFAULT           0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_AUTOTRI_DEFAULT            (_USART_CTRL_AUTOTRI_DEFAULT << 17)      /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_SCMODE                     (0x1UL << 18)                            /**< SmartCard Mode */
#define _USART_CTRL_SCMODE_SHIFT              18                                       /**< Shift value for USART_SCMODE */
#define _USART_CTRL_SCMODE_MASK               0x40000UL                                /**< Bit mask for USART_SCMODE */
#define _USART_CTRL_SCMODE_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_SCMODE_DEFAULT             (_USART_CTRL_SCMODE_DEFAULT << 18)       /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_SCRETRANS                  (0x1UL << 19)                            /**< SmartCard Retransmit */
#define _USART_CTRL_SCRETRANS_SHIFT           19                                       /**< Shift value for USART_SCRETRANS */
#define _USART_CTRL_SCRETRANS_MASK            0x80000UL                                /**< Bit mask for USART_SCRETRANS */
#define _USART_CTRL_SCRETRANS_DEFAULT         0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_SCRETRANS_DEFAULT          (_USART_CTRL_SCRETRANS_DEFAULT << 19)    /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_SKIPPERRF                  (0x1UL << 20)                            /**< Skip Parity Error Frames */
#define _USART_CTRL_SKIPPERRF_SHIFT           20                                       /**< Shift value for USART_SKIPPERRF */
#define _USART_CTRL_SKIPPERRF_MASK            0x100000UL                               /**< Bit mask for USART_SKIPPERRF */
#define _USART_CTRL_SKIPPERRF_DEFAULT         0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_SKIPPERRF_DEFAULT          (_USART_CTRL_SKIPPERRF_DEFAULT << 20)    /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_BIT8DV                     (0x1UL << 21)                            /**< Bit 8 Default Value */
#define _USART_CTRL_BIT8DV_SHIFT              21                                       /**< Shift value for USART_BIT8DV */
#define _USART_CTRL_BIT8DV_MASK               0x200000UL                               /**< Bit mask for USART_BIT8DV */
#define _USART_CTRL_BIT8DV_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_BIT8DV_DEFAULT             (_USART_CTRL_BIT8DV_DEFAULT << 21)       /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_ERRSDMA                    (0x1UL << 22)                            /**< Halt DMA On Error */
#define _USART_CTRL_ERRSDMA_SHIFT             22                                       /**< Shift value for USART_ERRSDMA */
#define _USART_CTRL_ERRSDMA_MASK              0x400000UL                               /**< Bit mask for USART_ERRSDMA */
#define _USART_CTRL_ERRSDMA_DEFAULT           0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_ERRSDMA_DEFAULT            (_USART_CTRL_ERRSDMA_DEFAULT << 22)      /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_ERRSRX                     (0x1UL << 23)                            /**< Disable RX On Error */
#define _USART_CTRL_ERRSRX_SHIFT              23                                       /**< Shift value for USART_ERRSRX */
#define _USART_CTRL_ERRSRX_MASK               0x800000UL                               /**< Bit mask for USART_ERRSRX */
#define _USART_CTRL_ERRSRX_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_ERRSRX_DEFAULT             (_USART_CTRL_ERRSRX_DEFAULT << 23)       /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_ERRSTX                     (0x1UL << 24)                            /**< Disable TX On Error */
#define _USART_CTRL_ERRSTX_SHIFT              24                                       /**< Shift value for USART_ERRSTX */
#define _USART_CTRL_ERRSTX_MASK               0x1000000UL                              /**< Bit mask for USART_ERRSTX */
#define _USART_CTRL_ERRSTX_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_ERRSTX_DEFAULT             (_USART_CTRL_ERRSTX_DEFAULT << 24)       /**< Shifted mode DEFAULT for USART_CTRL */
#define _USART_CTRL_TXDELAY_SHIFT             26                                       /**< Shift value for USART_TXDELAY */
#define _USART_CTRL_TXDELAY_MASK              0xC000000UL                              /**< Bit mask for USART_TXDELAY */
#define _USART_CTRL_TXDELAY_DEFAULT           0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define _USART_CTRL_TXDELAY_NONE              0x00000000UL                             /**< Mode NONE for USART_CTRL */
#define _USART_CTRL_TXDELAY_SINGLE            0x00000001UL                             /**< Mode SINGLE for USART_CTRL */
#define _USART_CTRL_TXDELAY_DOUBLE            0x00000002UL                             /**< Mode DOUBLE for USART_CTRL */
#define _USART_CTRL_TXDELAY_TRIPLE            0x00000003UL                             /**< Mode TRIPLE for USART_CTRL */
#define USART_CTRL_TXDELAY_DEFAULT            (_USART_CTRL_TXDELAY_DEFAULT << 26)      /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_TXDELAY_NONE               (_USART_CTRL_TXDELAY_NONE << 26)         /**< Shifted mode NONE for USART_CTRL */
#define USART_CTRL_TXDELAY_SINGLE             (_USART_CTRL_TXDELAY_SINGLE << 26)       /**< Shifted mode SINGLE for USART_CTRL */
#define USART_CTRL_TXDELAY_DOUBLE             (_USART_CTRL_TXDELAY_DOUBLE << 26)       /**< Shifted mode DOUBLE for USART_CTRL */
#define USART_CTRL_TXDELAY_TRIPLE             (_USART_CTRL_TXDELAY_TRIPLE << 26)       /**< Shifted mode TRIPLE for USART_CTRL */
#define USART_CTRL_BYTESWAP                   (0x1UL << 28)                            /**< Byteswap In Double Accesses */
#define _USART_CTRL_BYTESWAP_SHIFT            28                                       /**< Shift value for USART_BYTESWAP */
#define _USART_CTRL_BYTESWAP_MASK             0x10000000UL                             /**< Bit mask for USART_BYTESWAP */
#define _USART_CTRL_BYTESWAP_DEFAULT          0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_BYTESWAP_DEFAULT           (_USART_CTRL_BYTESWAP_DEFAULT << 28)     /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_AUTOTX                     (0x1UL << 29)                            /**< Always Transmit When RX Not Full */
#define _USART_CTRL_AUTOTX_SHIFT              29                                       /**< Shift value for USART_AUTOTX */
#define _USART_CTRL_AUTOTX_MASK               0x20000000UL                             /**< Bit mask for USART_AUTOTX */
#define _USART_CTRL_AUTOTX_DEFAULT            0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_AUTOTX_DEFAULT             (_USART_CTRL_AUTOTX_DEFAULT << 29)       /**< Shifted mode DEFAULT for USART_CTRL */
#define USART_CTRL_MVDIS                      (0x1UL << 30)                            /**< Majority Vote Disable */
#define _USART_CTRL_MVDIS_SHIFT               30                                       /**< Shift value for USART_MVDIS */
#define _USART_CTRL_MVDIS_MASK                0x40000000UL                             /**< Bit mask for USART_MVDIS */
#define _USART_CTRL_MVDIS_DEFAULT             0x00000000UL                             /**< Mode DEFAULT for USART_CTRL */
#define USART_CTRL_MVDIS_DEFAULT              (_USART_CTRL_MVDIS_DEFAULT << 30)        /**< Shifted mode DEFAULT for USART_CTRL */

/* Bit fields for USART FRAME */
#define _USART_FRAME_RESETVALUE               0x00001005UL                              /**< Default value for USART_FRAME */
#define _USART_FRAME_MASK                     0x0000330FUL                              /**< Mask for USART_FRAME */
#define _USART_FRAME_DATABITS_SHIFT           0                                         /**< Shift value for USART_DATABITS */
#define _USART_FRAME_DATABITS_MASK            0xFUL                                     /**< Bit mask for USART_DATABITS */
#define _USART_FRAME_DATABITS_FOUR            0x00000001UL                              /**< Mode FOUR for USART_FRAME */
#define _USART_FRAME_DATABITS_FIVE            0x00000002UL                              /**< Mode FIVE for USART_FRAME */
#define _USART_FRAME_DATABITS_SIX             0x00000003UL                              /**< Mode SIX for USART_FRAME */
#define _USART_FRAME_DATABITS_SEVEN           0x00000004UL                              /**< Mode SEVEN for USART_FRAME */
#define _USART_FRAME_DATABITS_DEFAULT         0x00000005UL                              /**< Mode DEFAULT for USART_FRAME */
#define _USART_FRAME_DATABITS_EIGHT           0x00000005UL                              /**< Mode EIGHT for USART_FRAME */
#define _USART_FRAME_DATABITS_NINE            0x00000006UL                              /**< Mode NINE for USART_FRAME */
#define _USART_FRAME_DATABITS_TEN             0x00000007UL                              /**< Mode TEN for USART_FRAME */
#define _USART_FRAME_DATABITS_ELEVEN          0x00000008UL                              /**< Mode ELEVEN for USART_FRAME */
#define _USART_FRAME_DATABITS_TWELVE          0x00000009UL                              /**< Mode TWELVE for USART_FRAME */
#define _USART_FRAME_DATABITS_THIRTEEN        0x0000000AUL                              /**< Mode THIRTEEN for USART_FRAME */
#define _USART_FRAME_DATABITS_FOURTEEN        0x0000000BUL                              /**< Mode FOURTEEN for USART_FRAME */
#define _USART_FRAME_DATABITS_FIFTEEN         0x0000000CUL                              /**< Mode FIFTEEN for USART_FRAME */
#define _USART_FRAME_DATABITS_SIXTEEN         0x0000000DUL                              /**< Mode SIXTEEN for USART_FRAME */
#define USART_FRAME_DATABITS_FOUR             (_USART_FRAME_DATABITS_FOUR << 0)         /**< Shifted mode FOUR for USART_FRAME */
#define USART_FRAME_DATABITS_FIVE             (_USART_FRAME_DATABITS_FIVE << 0)         /**< Shifted mode FIVE for USART_FRAME */
#define USART_FRAME_DATABITS_SIX              (_USART_FRAME_DATABITS_SIX << 0)          /**< Shifted mode SIX for USART_FRAME */
#define USART_FRAME_DATABITS_SEVEN            (_USART_FRAME_DATABITS_SEVEN << 0)        /**< Shifted mode SEVEN for USART_FRAME */
#define USART_FRAME_DATABITS_DEFAULT          (_USART_FRAME_DATABITS_DEFAULT << 0)      /**< Shifted mode DEFAULT for USART_FRAME */
#define USART_FRAME_DATABITS_EIGHT            (_USART_FRAME_DATABITS_EIGHT << 0)        /**< Shifted mode EIGHT for USART_FRAME */
#define USART_FRAME_DATABITS_NINE             (_USART_FRAME_DATABITS_NINE << 0)         /**< Shifted mode NINE for USART_FRAME */
#define USART_FRAME_DATABITS_TEN              (_USART_FRAME_DATABITS_TEN << 0)          /**< Shifted mode TEN for USART_FRAME */
#define USART_FRAME_DATABITS_ELEVEN           (_USART_FRAME_DATABITS_ELEVEN << 0)       /**< Shifted mode ELEVEN for USART_FRAME */
#define USART_FRAME_DATABITS_TWELVE           (_USART_FRAME_DATABITS_TWELVE << 0)       /**< Shifted mode TWELVE for USART_FRAME */
#define USART_FRAME_DATABITS_THIRTEEN         (_USART_FRAME_DATABITS_THIRTEEN << 0)     /**< Shifted mode THIRTEEN for USART_FRAME */
#define USART_FRAME_DATABITS_FOURTEEN         (_USART_FRAME_DATABITS_FOURTEEN << 0)     /**< Shifted mode FOURTEEN for USART_FRAME */
#define USART_FRAME_DATABITS_FIFTEEN          (_USART_FRAME_DATABITS_FIFTEEN << 0)      /**< Shifted mode FIFTEEN for USART_FRAME */
#define USART_FRAME_DATABITS_SIXTEEN          (_USART_FRAME_DATABITS_SIXTEEN << 0)      /**< Shifted mode SIXTEEN for USART_FRAME */
#define _USART_FRAME_PARITY_SHIFT             8                                         /**< Shift value for USART_PARITY */
#define _USART_FRAME_PARITY_MASK              0x300UL                                   /**< Bit mask for USART_PARITY */
#define _USART_FRAME_PARITY_DEFAULT           0x00000000UL                              /**< Mode DEFAULT for USART_FRAME */
#define _USART_FRAME_PARITY_NONE              0x00000000UL                              /**< Mode NONE for USART_FRAME */
#define _USART_FRAME_PARITY_EVEN              0x00000002UL                              /**< Mode EVEN for USART_FRAME */
#define _USART_FRAME_PARITY_ODD               0x00000003UL                              /**< Mode ODD for USART_FRAME */
#define USART_FRAME_PARITY_DEFAULT            (_USART_FRAME_PARITY_DEFAULT << 8)        /**< Shifted mode DEFAULT for USART_FRAME */
#define USART_FRAME_PARITY_NONE               (_USART_FRAME_PARITY_NONE << 8)           /**< Shifted mode NONE for USART_FRAME */
#define USART_FRAME_PARITY_EVEN               (_USART_FRAME_PARITY_EVEN << 8)           /**< Shifted mode EVEN for USART_FRAME */
#define USART_FRAME_PARITY_ODD                (_USART_FRAME_PARITY_ODD << 8)            /**< Shifted mode ODD for USART_FRAME */
#define _USART_FRAME_STOPBITS_SHIFT           12                                        /**< Shift value for USART_STOPBITS */
#define _USART_FRAME_STOPBITS_MASK            0x3000UL                                  /**< Bit mask for USART_STOPBITS */
#define _USART_FRAME_STOPBITS_HALF            0x00000000UL                              /**< Mode HALF for USART_FRAME */
#define _USART_FRAME_STOPBITS_DEFAULT         0x00000001UL                              /**< Mode DEFAULT for USART_FRAME */
#define _USART_FRAME_STOPBITS_ONE             0x00000001UL                              /**< Mode ONE for USART_FRAME */
#define _USART_FRAME_STOPBITS_ONEANDAHALF     0x00000002UL                              /**< Mode ONEANDAHALF for USART_FRAME */
#define _USART_FRAME_STOPBITS_TWO             0x00000003UL                              /**< Mode TWO for USART_FRAME */
#define USART_FRAME_STOPBITS_HALF             (_USART_FRAME_STOPBITS_HALF << 12)        /**< Shifted mode HALF for USART_FRAME */
#define USART_FRAME_STOPBITS_DEFAULT          (_USART_FRAME_STOPBITS_DEFAULT << 12)     /**< Shifted mode DEFAULT for USART_FRAME */
#define USART_FRAME_STOPBITS_ONE              (_USART_FRAME_STOPBITS_ONE << 12)         /**< Shifted mode ONE for USART_FRAME */
#define USART_FRAME_STOPBITS_ONEANDAHALF      (_USART_FRAME_STOPBITS_ONEANDAHALF << 12) /**< Shifted mode ONEANDAHALF for USART_FRAME */
#define USART_FRAME_STOPBITS_TWO              (_USART_FRAME_STOPBITS_TWO << 12)         /**< Shifted mode TWO for USART_FRAME */

/* Bit fields for USART TRIGCTRL */
#define _USART_TRIGCTRL_RESETVALUE            0x00000000UL                             /**< Default value for USART_TRIGCTRL */
#define _USART_TRIGCTRL_MASK                  0x00000077UL                             /**< Mask for USART_TRIGCTRL */
#define _USART_TRIGCTRL_TSEL_SHIFT            0                                        /**< Shift value for USART_TSEL */
#define _USART_TRIGCTRL_TSEL_MASK             0x7UL                                    /**< Bit mask for USART_TSEL */
#define _USART_TRIGCTRL_TSEL_DEFAULT          0x00000000UL                             /**< Mode DEFAULT for USART_TRIGCTRL */
#define _USART_TRIGCTRL_TSEL_PRSCH0           0x00000000UL                             /**< Mode PRSCH0 for USART_TRIGCTRL */
#define _USART_TRIGCTRL_TSEL_PRSCH1           0x00000001UL                             /**< Mode PRSCH1 for USART_TRIGCTRL */
#define _USART_TRIGCTRL_TSEL_PRSCH2           0x00000002UL                             /**< Mode PRSCH2 for USART_TRIGCTRL */
#define _USART_TRIGCTRL_TSEL_PRSCH3           0x00000003UL                             /**< Mode PRSCH3 for USART_TRIGCTRL */
#define _USART_TRIGCTRL_TSEL_PRSCH4           0x00000004UL                             /**< Mode PRSCH4 for USART_TRIGCTRL */
#define _USART_TRIGCTRL_TSEL_PRSCH5           0x00000005UL                             /**< Mode PRSCH5 for USART_TRIGCTRL */
#define _USART_TRIGCTRL_TSEL_PRSCH6           0x00000006UL                             /**< Mode PRSCH6 for USART_TRIGCTRL */
#define _USART_TRIGCTRL_TSEL_PRSCH7           0x00000007UL                             /**< Mode PRSCH7 for USART_TRIGCTRL */
#define USART_TRIGCTRL_TSEL_DEFAULT           (_USART_TRIGCTRL_TSEL_DEFAULT << 0)      /**< Shifted mode DEFAULT for USART_TRIGCTRL */
#define USART_TRIGCTRL_TSEL_PRSCH0            (_USART_TRIGCTRL_TSEL_PRSCH0 << 0)       /**< Shifted mode PRSCH0 for USART_TRIGCTRL */
#define USART_TRIGCTRL_TSEL_PRSCH1            (_USART_TRIGCTRL_TSEL_PRSCH1 << 0)       /**< Shifted mode PRSCH1 for USART_TRIGCTRL */
#define USART_TRIGCTRL_TSEL_PRSCH2            (_USART_TRIGCTRL_TSEL_PRSCH2 << 0)       /**< Shifted mode PRSCH2 for USART_TRIGCTRL */
#define USART_TRIGCTRL_TSEL_PRSCH3            (_USART_TRIGCTRL_TSEL_PRSCH3 << 0)       /**< Shifted mode PRSCH3 for USART_TRIGCTRL */
#define USART_TRIGCTRL_TSEL_PRSCH4            (_USART_TRIGCTRL_TSEL_PRSCH4 << 0)       /**< Shifted mode PRSCH4 for USART_TRIGCTRL */
#define USART_TRIGCTRL_TSEL_PRSCH5            (_USART_TRIGCTRL_TSEL_PRSCH5 << 0)       /**< Shifted mode PRSCH5 for USART_TRIGCTRL */
#define USART_TRIGCTRL_TSEL_PRSCH6            (_USART_TRIGCTRL_TSEL_PRSCH6 << 0)       /**< Shifted mode PRSCH6 for USART_TRIGCTRL */
#define USART_TRIGCTRL_TSEL_PRSCH7            (_USART_TRIGCTRL_TSEL_PRSCH7 << 0)       /**< Shifted mode PRSCH7 for USART_TRIGCTRL */
#define USART_TRIGCTRL_RXTEN                  (0x1UL << 4)                             /**< Receive Trigger Enable */
#define _USART_TRIGCTRL_RXTEN_SHIFT           4                                        /**< Shift value for USART_RXTEN */
#define _USART_TRIGCTRL_RXTEN_MASK            0x10UL                                   /**< Bit mask for USART_RXTEN */
#define _USART_TRIGCTRL_RXTEN_DEFAULT         0x00000000UL                             /**< Mode DEFAULT for USART_TRIGCTRL */
#define USART_TRIGCTRL_RXTEN_DEFAULT          (_USART_TRIGCTRL_RXTEN_DEFAULT << 4)     /**< Shifted mode DEFAULT for USART_TRIGCTRL */
#define USART_TRIGCTRL_TXTEN                  (0x1UL << 5)                             /**< Transmit Trigger Enable */
#define _USART_TRIGCTRL_TXTEN_SHIFT           5                                        /**< Shift value for USART_TXTEN */
#define _USART_TRIGCTRL_TXTEN_MASK            0x20UL                                   /**< Bit mask for USART_TXTEN */
#define _USART_TRIGCTRL_TXTEN_DEFAULT         0x00000000UL                             /**< Mode DEFAULT for USART_TRIGCTRL */
#define USART_TRIGCTRL_TXTEN_DEFAULT          (_USART_TRIGCTRL_TXTEN_DEFAULT << 5)     /**< Shifted mode DEFAULT for USART_TRIGCTRL */
#define USART_TRIGCTRL_AUTOTXTEN              (0x1UL << 6)                             /**< AUTOTX Trigger Enable */
#define _USART_TRIGCTRL_AUTOTXTEN_SHIFT       6                                        /**< Shift value for USART_AUTOTXTEN */
#define _USART_TRIGCTRL_AUTOTXTEN_MASK        0x40UL                                   /**< Bit mask for USART_AUTOTXTEN */
#define _USART_TRIGCTRL_AUTOTXTEN_DEFAULT     0x00000000UL                             /**< Mode DEFAULT for USART_TRIGCTRL */
#define USART_TRIGCTRL_AUTOTXTEN_DEFAULT      (_USART_TRIGCTRL_AUTOTXTEN_DEFAULT << 6) /**< Shifted mode DEFAULT for USART_TRIGCTRL */

/* Bit fields for USART CMD */
#define _USART_CMD_RESETVALUE                 0x00000000UL                         /**< Default value for USART_CMD */
#define _USART_CMD_MASK                       0x00000FFFUL                         /**< Mask for USART_CMD */
#define USART_CMD_RXEN                        (0x1UL << 0)                         /**< Receiver Enable */
#define _USART_CMD_RXEN_SHIFT                 0                                    /**< Shift value for USART_RXEN */
#define _USART_CMD_RXEN_MASK                  0x1UL                                /**< Bit mask for USART_RXEN */
#define _USART_CMD_RXEN_DEFAULT               0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_RXEN_DEFAULT                (_USART_CMD_RXEN_DEFAULT << 0)       /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_RXDIS                       (0x1UL << 1)                         /**< Receiver Disable */
#define _USART_CMD_RXDIS_SHIFT                1                                    /**< Shift value for USART_RXDIS */
#define _USART_CMD_RXDIS_MASK                 0x2UL                                /**< Bit mask for USART_RXDIS */
#define _USART_CMD_RXDIS_DEFAULT              0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_RXDIS_DEFAULT               (_USART_CMD_RXDIS_DEFAULT << 1)      /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_TXEN                        (0x1UL << 2)                         /**< Transmitter Enable */
#define _USART_CMD_TXEN_SHIFT                 2                                    /**< Shift value for USART_TXEN */
#define _USART_CMD_TXEN_MASK                  0x4UL                                /**< Bit mask for USART_TXEN */
#define _USART_CMD_TXEN_DEFAULT               0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_TXEN_DEFAULT                (_USART_CMD_TXEN_DEFAULT << 2)       /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_TXDIS                       (0x1UL << 3)                         /**< Transmitter Disable */
#define _USART_CMD_TXDIS_SHIFT                3                                    /**< Shift value for USART_TXDIS */
#define _USART_CMD_TXDIS_MASK                 0x8UL                                /**< Bit mask for USART_TXDIS */
#define _USART_CMD_TXDIS_DEFAULT              0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_TXDIS_DEFAULT               (_USART_CMD_TXDIS_DEFAULT << 3)      /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_MASTEREN                    (0x1UL << 4)                         /**< Master Enable */
#define _USART_CMD_MASTEREN_SHIFT             4                                    /**< Shift value for USART_MASTEREN */
#define _USART_CMD_MASTEREN_MASK              0x10UL                               /**< Bit mask for USART_MASTEREN */
#define _USART_CMD_MASTEREN_DEFAULT           0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_MASTEREN_DEFAULT            (_USART_CMD_MASTEREN_DEFAULT << 4)   /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_MASTERDIS                   (0x1UL << 5)                         /**< Master Disable */
#define _USART_CMD_MASTERDIS_SHIFT            5                                    /**< Shift value for USART_MASTERDIS */
#define _USART_CMD_MASTERDIS_MASK             0x20UL                               /**< Bit mask for USART_MASTERDIS */
#define _USART_CMD_MASTERDIS_DEFAULT          0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_MASTERDIS_DEFAULT           (_USART_CMD_MASTERDIS_DEFAULT << 5)  /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_RXBLOCKEN                   (0x1UL << 6)                         /**< Receiver Block Enable */
#define _USART_CMD_RXBLOCKEN_SHIFT            6                                    /**< Shift value for USART_RXBLOCKEN */
#define _USART_CMD_RXBLOCKEN_MASK             0x40UL                               /**< Bit mask for USART_RXBLOCKEN */
#define _USART_CMD_RXBLOCKEN_DEFAULT          0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_RXBLOCKEN_DEFAULT           (_USART_CMD_RXBLOCKEN_DEFAULT << 6)  /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_RXBLOCKDIS                  (0x1UL << 7)                         /**< Receiver Block Disable */
#define _USART_CMD_RXBLOCKDIS_SHIFT           7                                    /**< Shift value for USART_RXBLOCKDIS */
#define _USART_CMD_RXBLOCKDIS_MASK            0x80UL                               /**< Bit mask for USART_RXBLOCKDIS */
#define _USART_CMD_RXBLOCKDIS_DEFAULT         0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_RXBLOCKDIS_DEFAULT          (_USART_CMD_RXBLOCKDIS_DEFAULT << 7) /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_TXTRIEN                     (0x1UL << 8)                         /**< Transmitter Tristate Enable */
#define _USART_CMD_TXTRIEN_SHIFT              8                                    /**< Shift value for USART_TXTRIEN */
#define _USART_CMD_TXTRIEN_MASK               0x100UL                              /**< Bit mask for USART_TXTRIEN */
#define _USART_CMD_TXTRIEN_DEFAULT            0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_TXTRIEN_DEFAULT             (_USART_CMD_TXTRIEN_DEFAULT << 8)    /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_TXTRIDIS                    (0x1UL << 9)                         /**< Transmitter Tristate Disable */
#define _USART_CMD_TXTRIDIS_SHIFT             9                                    /**< Shift value for USART_TXTRIDIS */
#define _USART_CMD_TXTRIDIS_MASK              0x200UL                              /**< Bit mask for USART_TXTRIDIS */
#define _USART_CMD_TXTRIDIS_DEFAULT           0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_TXTRIDIS_DEFAULT            (_USART_CMD_TXTRIDIS_DEFAULT << 9)   /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_CLEARTX                     (0x1UL << 10)                        /**< Clear TX */
#define _USART_CMD_CLEARTX_SHIFT              10                                   /**< Shift value for USART_CLEARTX */
#define _USART_CMD_CLEARTX_MASK               0x400UL                              /**< Bit mask for USART_CLEARTX */
#define _USART_CMD_CLEARTX_DEFAULT            0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_CLEARTX_DEFAULT             (_USART_CMD_CLEARTX_DEFAULT << 10)   /**< Shifted mode DEFAULT for USART_CMD */
#define USART_CMD_CLEARRX                     (0x1UL << 11)                        /**< Clear RX */
#define _USART_CMD_CLEARRX_SHIFT              11                                   /**< Shift value for USART_CLEARRX */
#define _USART_CMD_CLEARRX_MASK               0x800UL                              /**< Bit mask for USART_CLEARRX */
#define _USART_CMD_CLEARRX_DEFAULT            0x00000000UL                         /**< Mode DEFAULT for USART_CMD */
#define USART_CMD_CLEARRX_DEFAULT             (_USART_CMD_CLEARRX_DEFAULT << 11)   /**< Shifted mode DEFAULT for USART_CMD */

/* Bit fields for USART STATUS */
#define _USART_STATUS_RESETVALUE              0x00000040UL                               /**< Default value for USART_STATUS */
#define _USART_STATUS_MASK                    0x00001FFFUL                               /**< Mask for USART_STATUS */
#define USART_STATUS_RXENS                    (0x1UL << 0)                               /**< Receiver Enable Status */
#define _USART_STATUS_RXENS_SHIFT             0                                          /**< Shift value for USART_RXENS */
#define _USART_STATUS_RXENS_MASK              0x1UL                                      /**< Bit mask for USART_RXENS */
#define _USART_STATUS_RXENS_DEFAULT           0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXENS_DEFAULT            (_USART_STATUS_RXENS_DEFAULT << 0)         /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXENS                    (0x1UL << 1)                               /**< Transmitter Enable Status */
#define _USART_STATUS_TXENS_SHIFT             1                                          /**< Shift value for USART_TXENS */
#define _USART_STATUS_TXENS_MASK              0x2UL                                      /**< Bit mask for USART_TXENS */
#define _USART_STATUS_TXENS_DEFAULT           0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXENS_DEFAULT            (_USART_STATUS_TXENS_DEFAULT << 1)         /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_MASTER                   (0x1UL << 2)                               /**< SPI Master Mode */
#define _USART_STATUS_MASTER_SHIFT            2                                          /**< Shift value for USART_MASTER */
#define _USART_STATUS_MASTER_MASK             0x4UL                                      /**< Bit mask for USART_MASTER */
#define _USART_STATUS_MASTER_DEFAULT          0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_MASTER_DEFAULT           (_USART_STATUS_MASTER_DEFAULT << 2)        /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXBLOCK                  (0x1UL << 3)                               /**< Block Incoming Data */
#define _USART_STATUS_RXBLOCK_SHIFT           3                                          /**< Shift value for USART_RXBLOCK */
#define _USART_STATUS_RXBLOCK_MASK            0x8UL                                      /**< Bit mask for USART_RXBLOCK */
#define _USART_STATUS_RXBLOCK_DEFAULT         0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXBLOCK_DEFAULT          (_USART_STATUS_RXBLOCK_DEFAULT << 3)       /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXTRI                    (0x1UL << 4)                               /**< Transmitter Tristated */
#define _USART_STATUS_TXTRI_SHIFT             4                                          /**< Shift value for USART_TXTRI */
#define _USART_STATUS_TXTRI_MASK              0x10UL                                     /**< Bit mask for USART_TXTRI */
#define _USART_STATUS_TXTRI_DEFAULT           0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXTRI_DEFAULT            (_USART_STATUS_TXTRI_DEFAULT << 4)         /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXC                      (0x1UL << 5)                               /**< TX Complete */
#define _USART_STATUS_TXC_SHIFT               5                                          /**< Shift value for USART_TXC */
#define _USART_STATUS_TXC_MASK                0x20UL                                     /**< Bit mask for USART_TXC */
#define _USART_STATUS_TXC_DEFAULT             0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXC_DEFAULT              (_USART_STATUS_TXC_DEFAULT << 5)           /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXBL                     (0x1UL << 6)                               /**< TX Buffer Level */
#define _USART_STATUS_TXBL_SHIFT              6                                          /**< Shift value for USART_TXBL */
#define _USART_STATUS_TXBL_MASK               0x40UL                                     /**< Bit mask for USART_TXBL */
#define _USART_STATUS_TXBL_DEFAULT            0x00000001UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXBL_DEFAULT             (_USART_STATUS_TXBL_DEFAULT << 6)          /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXDATAV                  (0x1UL << 7)                               /**< RX Data Valid */
#define _USART_STATUS_RXDATAV_SHIFT           7                                          /**< Shift value for USART_RXDATAV */
#define _USART_STATUS_RXDATAV_MASK            0x80UL                                     /**< Bit mask for USART_RXDATAV */
#define _USART_STATUS_RXDATAV_DEFAULT         0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXDATAV_DEFAULT          (_USART_STATUS_RXDATAV_DEFAULT << 7)       /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXFULL                   (0x1UL << 8)                               /**< RX FIFO Full */
#define _USART_STATUS_RXFULL_SHIFT            8                                          /**< Shift value for USART_RXFULL */
#define _USART_STATUS_RXFULL_MASK             0x100UL                                    /**< Bit mask for USART_RXFULL */
#define _USART_STATUS_RXFULL_DEFAULT          0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXFULL_DEFAULT           (_USART_STATUS_RXFULL_DEFAULT << 8)        /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXBDRIGHT                (0x1UL << 9)                               /**< TX Buffer Expects Double Right Data */
#define _USART_STATUS_TXBDRIGHT_SHIFT         9                                          /**< Shift value for USART_TXBDRIGHT */
#define _USART_STATUS_TXBDRIGHT_MASK          0x200UL                                    /**< Bit mask for USART_TXBDRIGHT */
#define _USART_STATUS_TXBDRIGHT_DEFAULT       0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXBDRIGHT_DEFAULT        (_USART_STATUS_TXBDRIGHT_DEFAULT << 9)     /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXBSRIGHT                (0x1UL << 10)                              /**< TX Buffer Expects Single Right Data */
#define _USART_STATUS_TXBSRIGHT_SHIFT         10                                         /**< Shift value for USART_TXBSRIGHT */
#define _USART_STATUS_TXBSRIGHT_MASK          0x400UL                                    /**< Bit mask for USART_TXBSRIGHT */
#define _USART_STATUS_TXBSRIGHT_DEFAULT       0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_TXBSRIGHT_DEFAULT        (_USART_STATUS_TXBSRIGHT_DEFAULT << 10)    /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXDATAVRIGHT             (0x1UL << 11)                              /**< RX Data Right */
#define _USART_STATUS_RXDATAVRIGHT_SHIFT      11                                         /**< Shift value for USART_RXDATAVRIGHT */
#define _USART_STATUS_RXDATAVRIGHT_MASK       0x800UL                                    /**< Bit mask for USART_RXDATAVRIGHT */
#define _USART_STATUS_RXDATAVRIGHT_DEFAULT    0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXDATAVRIGHT_DEFAULT     (_USART_STATUS_RXDATAVRIGHT_DEFAULT << 11) /**< Shifted mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXFULLRIGHT              (0x1UL << 12)                              /**< RX Full of Right Data */
#define _USART_STATUS_RXFULLRIGHT_SHIFT       12                                         /**< Shift value for USART_RXFULLRIGHT */
#define _USART_STATUS_RXFULLRIGHT_MASK        0x1000UL                                   /**< Bit mask for USART_RXFULLRIGHT */
#define _USART_STATUS_RXFULLRIGHT_DEFAULT     0x00000000UL                               /**< Mode DEFAULT for USART_STATUS */
#define USART_STATUS_RXFULLRIGHT_DEFAULT      (_USART_STATUS_RXFULLRIGHT_DEFAULT << 12)  /**< Shifted mode DEFAULT for USART_STATUS */

/* Bit fields for USART CLKDIV */
#define _USART_CLKDIV_RESETVALUE              0x00000000UL                     /**< Default value for USART_CLKDIV */
#define _USART_CLKDIV_MASK                    0x001FFFC0UL                     /**< Mask for USART_CLKDIV */
#define _USART_CLKDIV_DIV_SHIFT               6                                /**< Shift value for USART_DIV */
#define _USART_CLKDIV_DIV_MASK                0x1FFFC0UL                       /**< Bit mask for USART_DIV */
#define _USART_CLKDIV_DIV_DEFAULT             0x00000000UL                     /**< Mode DEFAULT for USART_CLKDIV */
#define USART_CLKDIV_DIV_DEFAULT              (_USART_CLKDIV_DIV_DEFAULT << 6) /**< Shifted mode DEFAULT for USART_CLKDIV */

/* Bit fields for USART RXDATAX */
#define _USART_RXDATAX_RESETVALUE             0x00000000UL                         /**< Default value for USART_RXDATAX */
#define _USART_RXDATAX_MASK                   0x0000C1FFUL                         /**< Mask for USART_RXDATAX */
#define _USART_RXDATAX_RXDATA_SHIFT           0                                    /**< Shift value for USART_RXDATA */
#define _USART_RXDATAX_RXDATA_MASK            0x1FFUL                              /**< Bit mask for USART_RXDATA */
#define _USART_RXDATAX_RXDATA_DEFAULT         0x00000000UL                         /**< Mode DEFAULT for USART_RXDATAX */
#define USART_RXDATAX_RXDATA_DEFAULT          (_USART_RXDATAX_RXDATA_DEFAULT << 0) /**< Shifted mode DEFAULT for USART_RXDATAX */
#define USART_RXDATAX_PERR                    (0x1UL << 14)                        /**< Data Parity Error */
#define _USART_RXDATAX_PERR_SHIFT             14                                   /**< Shift value for USART_PERR */
#define _USART_RXDATAX_PERR_MASK              0x4000UL                             /**< Bit mask for USART_PERR */
#define _USART_RXDATAX_PERR_DEFAULT           0x00000000UL                         /**< Mode DEFAULT for USART_RXDATAX */
#define USART_RXDATAX_PERR_DEFAULT            (_USART_RXDATAX_PERR_DEFAULT << 14)  /**< Shifted mode DEFAULT for USART_RXDATAX */
#define USART_RXDATAX_FERR                    (0x1UL << 15)                        /**< Data Framing Error */
#define _USART_RXDATAX_FERR_SHIFT             15                                   /**< Shift value for USART_FERR */
#define _USART_RXDATAX_FERR_MASK              0x8000UL                             /**< Bit mask for USART_FERR */
#define _USART_RXDATAX_FERR_DEFAULT           0x00000000UL                         /**< Mode DEFAULT for USART_RXDATAX */
#define USART_RXDATAX_FERR_DEFAULT            (_USART_RXDATAX_FERR_DEFAULT << 15)  /**< Shifted mode DEFAULT for USART_RXDATAX */

/* Bit fields for USART RXDATA */
#define _USART_RXDATA_RESETVALUE              0x00000000UL                        /**< Default value for USART_RXDATA */
#define _USART_RXDATA_MASK                    0x000000FFUL                        /**< Mask for USART_RXDATA */
#define _USART_RXDATA_RXDATA_SHIFT            0                                   /**< Shift value for USART_RXDATA */
#define _USART_RXDATA_RXDATA_MASK             0xFFUL                              /**< Bit mask for USART_RXDATA */
#define _USART_RXDATA_RXDATA_DEFAULT          0x00000000UL                        /**< Mode DEFAULT for USART_RXDATA */
#define USART_RXDATA_RXDATA_DEFAULT           (_USART_RXDATA_RXDATA_DEFAULT << 0) /**< Shifted mode DEFAULT for USART_RXDATA */

/* Bit fields for USART RXDOUBLEX */
#define _USART_RXDOUBLEX_RESETVALUE           0x00000000UL                             /**< Default value for USART_RXDOUBLEX */
#define _USART_RXDOUBLEX_MASK                 0xC1FFC1FFUL                             /**< Mask for USART_RXDOUBLEX */
#define _USART_RXDOUBLEX_RXDATA0_SHIFT        0                                        /**< Shift value for USART_RXDATA0 */
#define _USART_RXDOUBLEX_RXDATA0_MASK         0x1FFUL                                  /**< Bit mask for USART_RXDATA0 */
#define _USART_RXDOUBLEX_RXDATA0_DEFAULT      0x00000000UL                             /**< Mode DEFAULT for USART_RXDOUBLEX */
#define USART_RXDOUBLEX_RXDATA0_DEFAULT       (_USART_RXDOUBLEX_RXDATA0_DEFAULT << 0)  /**< Shifted mode DEFAULT for USART_RXDOUBLEX */
#define USART_RXDOUBLEX_PERR0                 (0x1UL << 14)                            /**< Data Parity Error 0 */
#define _USART_RXDOUBLEX_PERR0_SHIFT          14                                       /**< Shift value for USART_PERR0 */
#define _USART_RXDOUBLEX_PERR0_MASK           0x4000UL                                 /**< Bit mask for USART_PERR0 */
#define _USART_RXDOUBLEX_PERR0_DEFAULT        0x00000000UL                             /**< Mode DEFAULT for USART_RXDOUBLEX */
#define USART_RXDOUBLEX_PERR0_DEFAULT         (_USART_RXDOUBLEX_PERR0_DEFAULT << 14)   /**< Shifted mode DEFAULT for USART_RXDOUBLEX */
#define USART_RXDOUBLEX_FERR0                 (0x1UL << 15)                            /**< Data Framing Error 0 */
#define _USART_RXDOUBLEX_FERR0_SHIFT          15                                       /**< Shift value for USART_FERR0 */
#define _USART_RXDOUBLEX_FERR0_MASK           0x8000UL                                 /**< Bit mask for USART_FERR0 */
#define _USART_RXDOUBLEX_FERR0_DEFAULT        0x00000000UL                             /**< Mode DEFAULT for USART_RXDOUBLEX */
#define USART_RXDOUBLEX_FERR0_DEFAULT         (_USART_RXDOUBLEX_FERR0_DEFAULT << 15)   /**< Shifted mode DEFAULT for USART_RXDOUBLEX */
#define _USART_RXDOUBLEX_RXDATA1_SHIFT        16                                       /**< Shift value for USART_RXDATA1 */
#define _USART_RXDOUBLEX_RXDATA1_MASK         0x1FF0000UL                              /**< Bit mask for USART_RXDATA1 */
#define _USART_RXDOUBLEX_RXDATA1_DEFAULT      0x00000000UL                             /**< Mode DEFAULT for USART_RXDOUBLEX */
#define USART_RXDOUBLEX_RXDATA1_DEFAULT       (_USART_RXDOUBLEX_RXDATA1_DEFAULT << 16) /**< Shifted mode DEFAULT for USART_RXDOUBLEX */
#define USART_RXDOUBLEX_PERR1                 (0x1UL << 30)                            /**< Data Parity Error 1 */
#define _USART_RXDOUBLEX_PERR1_SHIFT          30                                       /**< Shift value for USART_PERR1 */
#define _USART_RXDOUBLEX_PERR1_MASK           0x40000000UL                             /**< Bit mask for USART_PERR1 */
#define _USART_RXDOUBLEX_PERR1_DEFAULT        0x00000000UL                             /**< Mode DEFAULT for USART_RXDOUBLEX */
#define USART_RXDOUBLEX_PERR1_DEFAULT         (_USART_RXDOUBLEX_PERR1_DEFAULT << 30)   /**< Shifted mode DEFAULT for USART_RXDOUBLEX */
#define USART_RXDOUBLEX_FERR1                 (0x1UL << 31)                            /**< Data Framing Error 1 */
#define _USART_RXDOUBLEX_FERR1_SHIFT          31                                       /**< Shift value for USART_FERR1 */
#define _USART_RXDOUBLEX_FERR1_MASK           0x80000000UL                             /**< Bit mask for USART_FERR1 */
#define _USART_RXDOUBLEX_FERR1_DEFAULT        0x00000000UL                             /**< Mode DEFAULT for USART_RXDOUBLEX */
#define USART_RXDOUBLEX_FERR1_DEFAULT         (_USART_RXDOUBLEX_FERR1_DEFAULT << 31)   /**< Shifted mode DEFAULT for USART_RXDOUBLEX */

/* Bit fields for USART RXDOUBLE */
#define _USART_RXDOUBLE_RESETVALUE            0x00000000UL                           /**< Default value for USART_RXDOUBLE */
#define _USART_RXDOUBLE_MASK                  0x0000FFFFUL                           /**< Mask for USART_RXDOUBLE */
#define _USART_RXDOUBLE_RXDATA0_SHIFT         0                                      /**< Shift value for USART_RXDATA0 */
#define _USART_RXDOUBLE_RXDATA0_MASK          0xFFUL                                 /**< Bit mask for USART_RXDATA0 */
#define _USART_RXDOUBLE_RXDATA0_DEFAULT       0x00000000UL                           /**< Mode DEFAULT for USART_RXDOUBLE */
#define USART_RXDOUBLE_RXDATA0_DEFAULT        (_USART_RXDOUBLE_RXDATA0_DEFAULT << 0) /**< Shifted mode DEFAULT for USART_RXDOUBLE */
#define _USART_RXDOUBLE_RXDATA1_SHIFT         8                                      /**< Shift value for USART_RXDATA1 */
#define _USART_RXDOUBLE_RXDATA1_MASK          0xFF00UL                               /**< Bit mask for USART_RXDATA1 */
#define _USART_RXDOUBLE_RXDATA1_DEFAULT       0x00000000UL                           /**< Mode DEFAULT for USART_RXDOUBLE */
#define USART_RXDOUBLE_RXDATA1_DEFAULT        (_USART_RXDOUBLE_RXDATA1_DEFAULT << 8) /**< Shifted mode DEFAULT for USART_RXDOUBLE */

/* Bit fields for USART RXDATAXP */
#define _USART_RXDATAXP_RESETVALUE            0x00000000UL                           /**< Default value for USART_RXDATAXP */
#define _USART_RXDATAXP_MASK                  0x0000C1FFUL                           /**< Mask for USART_RXDATAXP */
#define _USART_RXDATAXP_RXDATAP_SHIFT         0                                      /**< Shift value for USART_RXDATAP */
#define _USART_RXDATAXP_RXDATAP_MASK          0x1FFUL                                /**< Bit mask for USART_RXDATAP */
#define _USART_RXDATAXP_RXDATAP_DEFAULT       0x00000000UL                           /**< Mode DEFAULT for USART_RXDATAXP */
#define USART_RXDATAXP_RXDATAP_DEFAULT        (_USART_RXDATAXP_RXDATAP_DEFAULT << 0) /**< Shifted mode DEFAULT for USART_RXDATAXP */
#define USART_RXDATAXP_PERRP                  (0x1UL << 14)                          /**< Data Parity Error Peek */
#define _USART_RXDATAXP_PERRP_SHIFT           14                                     /**< Shift value for USART_PERRP */
#define _USART_RXDATAXP_PERRP_MASK            0x4000UL                               /**< Bit mask for USART_PERRP */
#define _USART_RXDATAXP_PERRP_DEFAULT         0x00000000UL                           /**< Mode DEFAULT for USART_RXDATAXP */
#define USART_RXDATAXP_PERRP_DEFAULT          (_USART_RXDATAXP_PERRP_DEFAULT << 14)  /**< Shifted mode DEFAULT for USART_RXDATAXP */
#define USART_RXDATAXP_FERRP                  (0x1UL << 15)                          /**< Data Framing Error Peek */
#define _USART_RXDATAXP_FERRP_SHIFT           15                                     /**< Shift value for USART_FERRP */
#define _USART_RXDATAXP_FERRP_MASK            0x8000UL                               /**< Bit mask for USART_FERRP */
#define _USART_RXDATAXP_FERRP_DEFAULT         0x00000000UL                           /**< Mode DEFAULT for USART_RXDATAXP */
#define USART_RXDATAXP_FERRP_DEFAULT          (_USART_RXDATAXP_FERRP_DEFAULT << 15)  /**< Shifted mode DEFAULT for USART_RXDATAXP */

/* Bit fields for USART RXDOUBLEXP */
#define _USART_RXDOUBLEXP_RESETVALUE          0x00000000UL                               /**< Default value for USART_RXDOUBLEXP */
#define _USART_RXDOUBLEXP_MASK                0xC1FFC1FFUL                               /**< Mask for USART_RXDOUBLEXP */
#define _USART_RXDOUBLEXP_RXDATAP0_SHIFT      0                                          /**< Shift value for USART_RXDATAP0 */
#define _USART_RXDOUBLEXP_RXDATAP0_MASK       0x1FFUL                                    /**< Bit mask for USART_RXDATAP0 */
#define _USART_RXDOUBLEXP_RXDATAP0_DEFAULT    0x00000000UL                               /**< Mode DEFAULT for USART_RXDOUBLEXP */
#define USART_RXDOUBLEXP_RXDATAP0_DEFAULT     (_USART_RXDOUBLEXP_RXDATAP0_DEFAULT << 0)  /**< Shifted mode DEFAULT for USART_RXDOUBLEXP */
#define USART_RXDOUBLEXP_PERRP0               (0x1UL << 14)                              /**< Data Parity Error 0 Peek */
#define _USART_RXDOUBLEXP_PERRP0_SHIFT        14                                         /**< Shift value for USART_PERRP0 */
#define _USART_RXDOUBLEXP_PERRP0_MASK         0x4000UL                                   /**< Bit mask for USART_PERRP0 */
#define _USART_RXDOUBLEXP_PERRP0_DEFAULT      0x00000000UL                               /**< Mode DEFAULT for USART_RXDOUBLEXP */
#define USART_RXDOUBLEXP_PERRP0_DEFAULT       (_USART_RXDOUBLEXP_PERRP0_DEFAULT << 14)   /**< Shifted mode DEFAULT for USART_RXDOUBLEXP */
#define USART_RXDOUBLEXP_FERRP0               (0x1UL << 15)                              /**< Data Framing Error 0 Peek */
#define _USART_RXDOUBLEXP_FERRP0_SHIFT        15                                         /**< Shift value for USART_FERRP0 */
#define _USART_RXDOUBLEXP_FERRP0_MASK         0x8000UL                                   /**< Bit mask for USART_FERRP0 */
#define _USART_RXDOUBLEXP_FERRP0_DEFAULT      0x00000000UL                               /**< Mode DEFAULT for USART_RXDOUBLEXP */
#define USART_RXDOUBLEXP_FERRP0_DEFAULT       (_USART_RXDOUBLEXP_FERRP0_DEFAULT << 15)   /**< Shifted mode DEFAULT for USART_RXDOUBLEXP */
#define _USART_RXDOUBLEXP_RXDATAP1_SHIFT      16                                         /**< Shift value for USART_RXDATAP1 */
#define _USART_RXDOUBLEXP_RXDATAP1_MASK       0x1FF0000UL                                /**< Bit mask for USART_RXDATAP1 */
#define _USART_RXDOUBLEXP_RXDATAP1_DEFAULT    0x00000000UL                               /**< Mode DEFAULT for USART_RXDOUBLEXP */
#define USART_RXDOUBLEXP_RXDATAP1_DEFAULT     (_USART_RXDOUBLEXP_RXDATAP1_DEFAULT << 16) /**< Shifted mode DEFAULT for USART_RXDOUBLEXP */
#define USART_RXDOUBLEXP_PERRP1               (0x1UL << 30)                              /**< Data Parity Error 1 Peek */
#define _USART_RXDOUBLEXP_PERRP1_SHIFT        30                                         /**< Shift value for USART_PERRP1 */
#define _USART_RXDOUBLEXP_PERRP1_MASK         0x40000000UL                               /**< Bit mask for USART_PERRP1 */
#define _USART_RXDOUBLEXP_PERRP1_DEFAULT      0x00000000UL                               /**< Mode DEFAULT for USART_RXDOUBLEXP */
#define USART_RXDOUBLEXP_PERRP1_DEFAULT       (_USART_RXDOUBLEXP_PERRP1_DEFAULT << 30)   /**< Shifted mode DEFAULT for USART_RXDOUBLEXP */
#define USART_RXDOUBLEXP_FERRP1               (0x1UL << 31)                              /**< Data Framing Error 1 Peek */
#define _USART_RXDOUBLEXP_FERRP1_SHIFT        31                                         /**< Shift value for USART_FERRP1 */
#define _USART_RXDOUBLEXP_FERRP1_MASK         0x80000000UL                               /**< Bit mask for USART_FERRP1 */
#define _USART_RXDOUBLEXP_FERRP1_DEFAULT      0x00000000UL                               /**< Mode DEFAULT for USART_RXDOUBLEXP */
#define USART_RXDOUBLEXP_FERRP1_DEFAULT       (_USART_RXDOUBLEXP_FERRP1_DEFAULT << 31)   /**< Shifted mode DEFAULT for USART_RXDOUBLEXP */

/* Bit fields for USART TXDATAX */
#define _USART_TXDATAX_RESETVALUE             0x00000000UL                           /**< Default value for USART_TXDATAX */
#define _USART_TXDATAX_MASK                   0x0000F9FFUL                           /**< Mask for USART_TXDATAX */
#define _USART_TXDATAX_TXDATAX_SHIFT          0                                      /**< Shift value for USART_TXDATAX */
#define _USART_TXDATAX_TXDATAX_MASK           0x1FFUL                                /**< Bit mask for USART_TXDATAX */
#define _USART_TXDATAX_TXDATAX_DEFAULT        0x00000000UL                           /**< Mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_TXDATAX_DEFAULT         (_USART_TXDATAX_TXDATAX_DEFAULT << 0)  /**< Shifted mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_UBRXAT                  (0x1UL << 11)                          /**< Unblock RX After Transmission */
#define _USART_TXDATAX_UBRXAT_SHIFT           11                                     /**< Shift value for USART_UBRXAT */
#define _USART_TXDATAX_UBRXAT_MASK            0x800UL                                /**< Bit mask for USART_UBRXAT */
#define _USART_TXDATAX_UBRXAT_DEFAULT         0x00000000UL                           /**< Mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_UBRXAT_DEFAULT          (_USART_TXDATAX_UBRXAT_DEFAULT << 11)  /**< Shifted mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_TXTRIAT                 (0x1UL << 12)                          /**< Set TXTRI After Transmission */
#define _USART_TXDATAX_TXTRIAT_SHIFT          12                                     /**< Shift value for USART_TXTRIAT */
#define _USART_TXDATAX_TXTRIAT_MASK           0x1000UL                               /**< Bit mask for USART_TXTRIAT */
#define _USART_TXDATAX_TXTRIAT_DEFAULT        0x00000000UL                           /**< Mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_TXTRIAT_DEFAULT         (_USART_TXDATAX_TXTRIAT_DEFAULT << 12) /**< Shifted mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_TXBREAK                 (0x1UL << 13)                          /**< Transmit Data As Break */
#define _USART_TXDATAX_TXBREAK_SHIFT          13                                     /**< Shift value for USART_TXBREAK */
#define _USART_TXDATAX_TXBREAK_MASK           0x2000UL                               /**< Bit mask for USART_TXBREAK */
#define _USART_TXDATAX_TXBREAK_DEFAULT        0x00000000UL                           /**< Mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_TXBREAK_DEFAULT         (_USART_TXDATAX_TXBREAK_DEFAULT << 13) /**< Shifted mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_TXDISAT                 (0x1UL << 14)                          /**< Clear TXEN After Transmission */
#define _USART_TXDATAX_TXDISAT_SHIFT          14                                     /**< Shift value for USART_TXDISAT */
#define _USART_TXDATAX_TXDISAT_MASK           0x4000UL                               /**< Bit mask for USART_TXDISAT */
#define _USART_TXDATAX_TXDISAT_DEFAULT        0x00000000UL                           /**< Mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_TXDISAT_DEFAULT         (_USART_TXDATAX_TXDISAT_DEFAULT << 14) /**< Shifted mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_RXENAT                  (0x1UL << 15)                          /**< Enable RX After Transmission */
#define _USART_TXDATAX_RXENAT_SHIFT           15                                     /**< Shift value for USART_RXENAT */
#define _USART_TXDATAX_RXENAT_MASK            0x8000UL                               /**< Bit mask for USART_RXENAT */
#define _USART_TXDATAX_RXENAT_DEFAULT         0x00000000UL                           /**< Mode DEFAULT for USART_TXDATAX */
#define USART_TXDATAX_RXENAT_DEFAULT          (_USART_TXDATAX_RXENAT_DEFAULT << 15)  /**< Shifted mode DEFAULT for USART_TXDATAX */

/* Bit fields for USART TXDATA */
#define _USART_TXDATA_RESETVALUE              0x00000000UL                        /**< Default value for USART_TXDATA */
#define _USART_TXDATA_MASK                    0x000000FFUL                        /**< Mask for USART_TXDATA */
#define _USART_TXDATA_TXDATA_SHIFT            0                                   /**< Shift value for USART_TXDATA */
#define _USART_TXDATA_TXDATA_MASK             0xFFUL                              /**< Bit mask for USART_TXDATA */
#define _USART_TXDATA_TXDATA_DEFAULT          0x00000000UL                        /**< Mode DEFAULT for USART_TXDATA */
#define USART_TXDATA_TXDATA_DEFAULT           (_USART_TXDATA_TXDATA_DEFAULT << 0) /**< Shifted mode DEFAULT for USART_TXDATA */

/* Bit fields for USART TXDOUBLEX */
#define _USART_TXDOUBLEX_RESETVALUE           0x00000000UL                              /**< Default value for USART_TXDOUBLEX */
#define _USART_TXDOUBLEX_MASK                 0xF9FFF9FFUL                              /**< Mask for USART_TXDOUBLEX */
#define _USART_TXDOUBLEX_TXDATA0_SHIFT        0                                         /**< Shift value for USART_TXDATA0 */
#define _USART_TXDOUBLEX_TXDATA0_MASK         0x1FFUL                                   /**< Bit mask for USART_TXDATA0 */
#define _USART_TXDOUBLEX_TXDATA0_DEFAULT      0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXDATA0_DEFAULT       (_USART_TXDOUBLEX_TXDATA0_DEFAULT << 0)   /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_UBRXAT0               (0x1UL << 11)                             /**< Unblock RX After Transmission */
#define _USART_TXDOUBLEX_UBRXAT0_SHIFT        11                                        /**< Shift value for USART_UBRXAT0 */
#define _USART_TXDOUBLEX_UBRXAT0_MASK         0x800UL                                   /**< Bit mask for USART_UBRXAT0 */
#define _USART_TXDOUBLEX_UBRXAT0_DEFAULT      0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_UBRXAT0_DEFAULT       (_USART_TXDOUBLEX_UBRXAT0_DEFAULT << 11)  /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXTRIAT0              (0x1UL << 12)                             /**< Set TXTRI After Transmission */
#define _USART_TXDOUBLEX_TXTRIAT0_SHIFT       12                                        /**< Shift value for USART_TXTRIAT0 */
#define _USART_TXDOUBLEX_TXTRIAT0_MASK        0x1000UL                                  /**< Bit mask for USART_TXTRIAT0 */
#define _USART_TXDOUBLEX_TXTRIAT0_DEFAULT     0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXTRIAT0_DEFAULT      (_USART_TXDOUBLEX_TXTRIAT0_DEFAULT << 12) /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXBREAK0              (0x1UL << 13)                             /**< Transmit Data As Break */
#define _USART_TXDOUBLEX_TXBREAK0_SHIFT       13                                        /**< Shift value for USART_TXBREAK0 */
#define _USART_TXDOUBLEX_TXBREAK0_MASK        0x2000UL                                  /**< Bit mask for USART_TXBREAK0 */
#define _USART_TXDOUBLEX_TXBREAK0_DEFAULT     0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXBREAK0_DEFAULT      (_USART_TXDOUBLEX_TXBREAK0_DEFAULT << 13) /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXDISAT0              (0x1UL << 14)                             /**< Clear TXEN After Transmission */
#define _USART_TXDOUBLEX_TXDISAT0_SHIFT       14                                        /**< Shift value for USART_TXDISAT0 */
#define _USART_TXDOUBLEX_TXDISAT0_MASK        0x4000UL                                  /**< Bit mask for USART_TXDISAT0 */
#define _USART_TXDOUBLEX_TXDISAT0_DEFAULT     0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXDISAT0_DEFAULT      (_USART_TXDOUBLEX_TXDISAT0_DEFAULT << 14) /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_RXENAT0               (0x1UL << 15)                             /**< Enable RX After Transmission */
#define _USART_TXDOUBLEX_RXENAT0_SHIFT        15                                        /**< Shift value for USART_RXENAT0 */
#define _USART_TXDOUBLEX_RXENAT0_MASK         0x8000UL                                  /**< Bit mask for USART_RXENAT0 */
#define _USART_TXDOUBLEX_RXENAT0_DEFAULT      0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_RXENAT0_DEFAULT       (_USART_TXDOUBLEX_RXENAT0_DEFAULT << 15)  /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define _USART_TXDOUBLEX_TXDATA1_SHIFT        16                                        /**< Shift value for USART_TXDATA1 */
#define _USART_TXDOUBLEX_TXDATA1_MASK         0x1FF0000UL                               /**< Bit mask for USART_TXDATA1 */
#define _USART_TXDOUBLEX_TXDATA1_DEFAULT      0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXDATA1_DEFAULT       (_USART_TXDOUBLEX_TXDATA1_DEFAULT << 16)  /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_UBRXAT1               (0x1UL << 27)                             /**< Unblock RX After Transmission */
#define _USART_TXDOUBLEX_UBRXAT1_SHIFT        27                                        /**< Shift value for USART_UBRXAT1 */
#define _USART_TXDOUBLEX_UBRXAT1_MASK         0x8000000UL                               /**< Bit mask for USART_UBRXAT1 */
#define _USART_TXDOUBLEX_UBRXAT1_DEFAULT      0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_UBRXAT1_DEFAULT       (_USART_TXDOUBLEX_UBRXAT1_DEFAULT << 27)  /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXTRIAT1              (0x1UL << 28)                             /**< Set TXTRI After Transmission */
#define _USART_TXDOUBLEX_TXTRIAT1_SHIFT       28                                        /**< Shift value for USART_TXTRIAT1 */
#define _USART_TXDOUBLEX_TXTRIAT1_MASK        0x10000000UL                              /**< Bit mask for USART_TXTRIAT1 */
#define _USART_TXDOUBLEX_TXTRIAT1_DEFAULT     0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXTRIAT1_DEFAULT      (_USART_TXDOUBLEX_TXTRIAT1_DEFAULT << 28) /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXBREAK1              (0x1UL << 29)                             /**< Transmit Data As Break */
#define _USART_TXDOUBLEX_TXBREAK1_SHIFT       29                                        /**< Shift value for USART_TXBREAK1 */
#define _USART_TXDOUBLEX_TXBREAK1_MASK        0x20000000UL                              /**< Bit mask for USART_TXBREAK1 */
#define _USART_TXDOUBLEX_TXBREAK1_DEFAULT     0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXBREAK1_DEFAULT      (_USART_TXDOUBLEX_TXBREAK1_DEFAULT << 29) /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXDISAT1              (0x1UL << 30)                             /**< Clear TXEN After Transmission */
#define _USART_TXDOUBLEX_TXDISAT1_SHIFT       30                                        /**< Shift value for USART_TXDISAT1 */
#define _USART_TXDOUBLEX_TXDISAT1_MASK        0x40000000UL                              /**< Bit mask for USART_TXDISAT1 */
#define _USART_TXDOUBLEX_TXDISAT1_DEFAULT     0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_TXDISAT1_DEFAULT      (_USART_TXDOUBLEX_TXDISAT1_DEFAULT << 30) /**< Shifted mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_RXENAT1               (0x1UL << 31)                             /**< Enable RX After Transmission */
#define _USART_TXDOUBLEX_RXENAT1_SHIFT        31                                        /**< Shift value for USART_RXENAT1 */
#define _USART_TXDOUBLEX_RXENAT1_MASK         0x80000000UL                              /**< Bit mask for USART_RXENAT1 */
#define _USART_TXDOUBLEX_RXENAT1_DEFAULT      0x00000000UL                              /**< Mode DEFAULT for USART_TXDOUBLEX */
#define USART_TXDOUBLEX_RXENAT1_DEFAULT       (_USART_TXDOUBLEX_RXENAT1_DEFAULT << 31)  /**< Shifted mode DEFAULT for USART_TXDOUBLEX */

/* Bit fields for USART TXDOUBLE */
#define _USART_TXDOUBLE_RESETVALUE            0x00000000UL                           /**< Default value for USART_TXDOUBLE */
#define _USART_TXDOUBLE_MASK                  0x0000FFFFUL                           /**< Mask for USART_TXDOUBLE */
#define _USART_TXDOUBLE_TXDATA0_SHIFT         0                                      /**< Shift value for USART_TXDATA0 */
#define _USART_TXDOUBLE_TXDATA0_MASK          0xFFUL                                 /**< Bit mask for USART_TXDATA0 */
#define _USART_TXDOUBLE_TXDATA0_DEFAULT       0x00000000UL                           /**< Mode DEFAULT for USART_TXDOUBLE */
#define USART_TXDOUBLE_TXDATA0_DEFAULT        (_USART_TXDOUBLE_TXDATA0_DEFAULT << 0) /**< Shifted mode DEFAULT for USART_TXDOUBLE */
#define _USART_TXDOUBLE_TXDATA1_SHIFT         8                                      /**< Shift value for USART_TXDATA1 */
#define _USART_TXDOUBLE_TXDATA1_MASK          0xFF00UL                               /**< Bit mask for USART_TXDATA1 */
#define _USART_TXDOUBLE_TXDATA1_DEFAULT       0x00000000UL                           /**< Mode DEFAULT for USART_TXDOUBLE */
#define USART_TXDOUBLE_TXDATA1_DEFAULT        (_USART_TXDOUBLE_TXDATA1_DEFAULT << 8) /**< Shifted mode DEFAULT for USART_TXDOUBLE */

/* Bit fields for USART IF */
#define _USART_IF_RESETVALUE                  0x00000002UL                     /**< Default value for USART_IF */
#define _USART_IF_MASK                        0x00001FFFUL                     /**< Mask for USART_IF */
#define USART_IF_TXC                          (0x1UL << 0)                     /**< TX Complete Interrupt Flag */
#define _USART_IF_TXC_SHIFT                   0                                /**< Shift value for USART_TXC */
#define _USART_IF_TXC_MASK                    0x1UL                            /**< Bit mask for USART_TXC */
#define _USART_IF_TXC_DEFAULT                 0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_TXC_DEFAULT                  (_USART_IF_TXC_DEFAULT << 0)     /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_TXBL                         (0x1UL << 1)                     /**< TX Buffer Level Interrupt Flag */
#define _USART_IF_TXBL_SHIFT                  1                                /**< Shift value for USART_TXBL */
#define _USART_IF_TXBL_MASK                   0x2UL                            /**< Bit mask for USART_TXBL */
#define _USART_IF_TXBL_DEFAULT                0x00000001UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_TXBL_DEFAULT                 (_USART_IF_TXBL_DEFAULT << 1)    /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_RXDATAV                      (0x1UL << 2)                     /**< RX Data Valid Interrupt Flag */
#define _USART_IF_RXDATAV_SHIFT               2                                /**< Shift value for USART_RXDATAV */
#define _USART_IF_RXDATAV_MASK                0x4UL                            /**< Bit mask for USART_RXDATAV */
#define _USART_IF_RXDATAV_DEFAULT             0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_RXDATAV_DEFAULT              (_USART_IF_RXDATAV_DEFAULT << 2) /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_RXFULL                       (0x1UL << 3)                     /**< RX Buffer Full Interrupt Flag */
#define _USART_IF_RXFULL_SHIFT                3                                /**< Shift value for USART_RXFULL */
#define _USART_IF_RXFULL_MASK                 0x8UL                            /**< Bit mask for USART_RXFULL */
#define _USART_IF_RXFULL_DEFAULT              0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_RXFULL_DEFAULT               (_USART_IF_RXFULL_DEFAULT << 3)  /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_RXOF                         (0x1UL << 4)                     /**< RX Overflow Interrupt Flag */
#define _USART_IF_RXOF_SHIFT                  4                                /**< Shift value for USART_RXOF */
#define _USART_IF_RXOF_MASK                   0x10UL                           /**< Bit mask for USART_RXOF */
#define _USART_IF_RXOF_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_RXOF_DEFAULT                 (_USART_IF_RXOF_DEFAULT << 4)    /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_RXUF                         (0x1UL << 5)                     /**< RX Underflow Interrupt Flag */
#define _USART_IF_RXUF_SHIFT                  5                                /**< Shift value for USART_RXUF */
#define _USART_IF_RXUF_MASK                   0x20UL                           /**< Bit mask for USART_RXUF */
#define _USART_IF_RXUF_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_RXUF_DEFAULT                 (_USART_IF_RXUF_DEFAULT << 5)    /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_TXOF                         (0x1UL << 6)                     /**< TX Overflow Interrupt Flag */
#define _USART_IF_TXOF_SHIFT                  6                                /**< Shift value for USART_TXOF */
#define _USART_IF_TXOF_MASK                   0x40UL                           /**< Bit mask for USART_TXOF */
#define _USART_IF_TXOF_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_TXOF_DEFAULT                 (_USART_IF_TXOF_DEFAULT << 6)    /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_TXUF                         (0x1UL << 7)                     /**< TX Underflow Interrupt Flag */
#define _USART_IF_TXUF_SHIFT                  7                                /**< Shift value for USART_TXUF */
#define _USART_IF_TXUF_MASK                   0x80UL                           /**< Bit mask for USART_TXUF */
#define _USART_IF_TXUF_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_TXUF_DEFAULT                 (_USART_IF_TXUF_DEFAULT << 7)    /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_PERR                         (0x1UL << 8)                     /**< Parity Error Interrupt Flag */
#define _USART_IF_PERR_SHIFT                  8                                /**< Shift value for USART_PERR */
#define _USART_IF_PERR_MASK                   0x100UL                          /**< Bit mask for USART_PERR */
#define _USART_IF_PERR_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_PERR_DEFAULT                 (_USART_IF_PERR_DEFAULT << 8)    /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_FERR                         (0x1UL << 9)                     /**< Framing Error Interrupt Flag */
#define _USART_IF_FERR_SHIFT                  9                                /**< Shift value for USART_FERR */
#define _USART_IF_FERR_MASK                   0x200UL                          /**< Bit mask for USART_FERR */
#define _USART_IF_FERR_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_FERR_DEFAULT                 (_USART_IF_FERR_DEFAULT << 9)    /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_MPAF                         (0x1UL << 10)                    /**< Multi-Processor Address Frame Interrupt Flag */
#define _USART_IF_MPAF_SHIFT                  10                               /**< Shift value for USART_MPAF */
#define _USART_IF_MPAF_MASK                   0x400UL                          /**< Bit mask for USART_MPAF */
#define _USART_IF_MPAF_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_MPAF_DEFAULT                 (_USART_IF_MPAF_DEFAULT << 10)   /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_SSM                          (0x1UL << 11)                    /**< Slave-Select In Master Mode Interrupt Flag */
#define _USART_IF_SSM_SHIFT                   11                               /**< Shift value for USART_SSM */
#define _USART_IF_SSM_MASK                    0x800UL                          /**< Bit mask for USART_SSM */
#define _USART_IF_SSM_DEFAULT                 0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_SSM_DEFAULT                  (_USART_IF_SSM_DEFAULT << 11)    /**< Shifted mode DEFAULT for USART_IF */
#define USART_IF_CCF                          (0x1UL << 12)                    /**< Collision Check Fail Interrupt Flag */
#define _USART_IF_CCF_SHIFT                   12                               /**< Shift value for USART_CCF */
#define _USART_IF_CCF_MASK                    0x1000UL                         /**< Bit mask for USART_CCF */
#define _USART_IF_CCF_DEFAULT                 0x00000000UL                     /**< Mode DEFAULT for USART_IF */
#define USART_IF_CCF_DEFAULT                  (_USART_IF_CCF_DEFAULT << 12)    /**< Shifted mode DEFAULT for USART_IF */

/* Bit fields for USART IFS */
#define _USART_IFS_RESETVALUE                 0x00000000UL                     /**< Default value for USART_IFS */
#define _USART_IFS_MASK                       0x00001FF9UL                     /**< Mask for USART_IFS */
#define USART_IFS_TXC                         (0x1UL << 0)                     /**< Set TX Complete Interrupt Flag */
#define _USART_IFS_TXC_SHIFT                  0                                /**< Shift value for USART_TXC */
#define _USART_IFS_TXC_MASK                   0x1UL                            /**< Bit mask for USART_TXC */
#define _USART_IFS_TXC_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_TXC_DEFAULT                 (_USART_IFS_TXC_DEFAULT << 0)    /**< Shifted mode DEFAULT for USART_IFS */
#define USART_IFS_RXFULL                      (0x1UL << 3)                     /**< Set RX Buffer Full Interrupt Flag */
#define _USART_IFS_RXFULL_SHIFT               3                                /**< Shift value for USART_RXFULL */
#define _USART_IFS_RXFULL_MASK                0x8UL                            /**< Bit mask for USART_RXFULL */
#define _USART_IFS_RXFULL_DEFAULT             0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_RXFULL_DEFAULT              (_USART_IFS_RXFULL_DEFAULT << 3) /**< Shifted mode DEFAULT for USART_IFS */
#define USART_IFS_RXOF                        (0x1UL << 4)                     /**< Set RX Overflow Interrupt Flag */
#define _USART_IFS_RXOF_SHIFT                 4                                /**< Shift value for USART_RXOF */
#define _USART_IFS_RXOF_MASK                  0x10UL                           /**< Bit mask for USART_RXOF */
#define _USART_IFS_RXOF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_RXOF_DEFAULT                (_USART_IFS_RXOF_DEFAULT << 4)   /**< Shifted mode DEFAULT for USART_IFS */
#define USART_IFS_RXUF                        (0x1UL << 5)                     /**< Set RX Underflow Interrupt Flag */
#define _USART_IFS_RXUF_SHIFT                 5                                /**< Shift value for USART_RXUF */
#define _USART_IFS_RXUF_MASK                  0x20UL                           /**< Bit mask for USART_RXUF */
#define _USART_IFS_RXUF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_RXUF_DEFAULT                (_USART_IFS_RXUF_DEFAULT << 5)   /**< Shifted mode DEFAULT for USART_IFS */
#define USART_IFS_TXOF                        (0x1UL << 6)                     /**< Set TX Overflow Interrupt Flag */
#define _USART_IFS_TXOF_SHIFT                 6                                /**< Shift value for USART_TXOF */
#define _USART_IFS_TXOF_MASK                  0x40UL                           /**< Bit mask for USART_TXOF */
#define _USART_IFS_TXOF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_TXOF_DEFAULT                (_USART_IFS_TXOF_DEFAULT << 6)   /**< Shifted mode DEFAULT for USART_IFS */
#define USART_IFS_TXUF                        (0x1UL << 7)                     /**< Set TX Underflow Interrupt Flag */
#define _USART_IFS_TXUF_SHIFT                 7                                /**< Shift value for USART_TXUF */
#define _USART_IFS_TXUF_MASK                  0x80UL                           /**< Bit mask for USART_TXUF */
#define _USART_IFS_TXUF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_TXUF_DEFAULT                (_USART_IFS_TXUF_DEFAULT << 7)   /**< Shifted mode DEFAULT for USART_IFS */
#define USART_IFS_PERR                        (0x1UL << 8)                     /**< Set Parity Error Interrupt Flag */
#define _USART_IFS_PERR_SHIFT                 8                                /**< Shift value for USART_PERR */
#define _USART_IFS_PERR_MASK                  0x100UL                          /**< Bit mask for USART_PERR */
#define _USART_IFS_PERR_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_PERR_DEFAULT                (_USART_IFS_PERR_DEFAULT << 8)   /**< Shifted mode DEFAULT for USART_IFS */
#define USART_IFS_FERR                        (0x1UL << 9)                     /**< Set Framing Error Interrupt Flag */
#define _USART_IFS_FERR_SHIFT                 9                                /**< Shift value for USART_FERR */
#define _USART_IFS_FERR_MASK                  0x200UL                          /**< Bit mask for USART_FERR */
#define _USART_IFS_FERR_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_FERR_DEFAULT                (_USART_IFS_FERR_DEFAULT << 9)   /**< Shifted mode DEFAULT for USART_IFS */
#define USART_IFS_MPAF                        (0x1UL << 10)                    /**< Set Multi-Processor Address Frame Interrupt Flag */
#define _USART_IFS_MPAF_SHIFT                 10                               /**< Shift value for USART_MPAF */
#define _USART_IFS_MPAF_MASK                  0x400UL                          /**< Bit mask for USART_MPAF */
#define _USART_IFS_MPAF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_MPAF_DEFAULT                (_USART_IFS_MPAF_DEFAULT << 10)  /**< Shifted mode DEFAULT for USART_IFS */
#define USART_IFS_SSM                         (0x1UL << 11)                    /**< Set Slave-Select in Master mode Interrupt Flag */
#define _USART_IFS_SSM_SHIFT                  11                               /**< Shift value for USART_SSM */
#define _USART_IFS_SSM_MASK                   0x800UL                          /**< Bit mask for USART_SSM */
#define _USART_IFS_SSM_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_SSM_DEFAULT                 (_USART_IFS_SSM_DEFAULT << 11)   /**< Shifted mode DEFAULT for USART_IFS */
#define USART_IFS_CCF                         (0x1UL << 12)                    /**< Set Collision Check Fail Interrupt Flag */
#define _USART_IFS_CCF_SHIFT                  12                               /**< Shift value for USART_CCF */
#define _USART_IFS_CCF_MASK                   0x1000UL                         /**< Bit mask for USART_CCF */
#define _USART_IFS_CCF_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IFS */
#define USART_IFS_CCF_DEFAULT                 (_USART_IFS_CCF_DEFAULT << 12)   /**< Shifted mode DEFAULT for USART_IFS */

/* Bit fields for USART IFC */
#define _USART_IFC_RESETVALUE                 0x00000000UL                     /**< Default value for USART_IFC */
#define _USART_IFC_MASK                       0x00001FF9UL                     /**< Mask for USART_IFC */
#define USART_IFC_TXC                         (0x1UL << 0)                     /**< Clear TX Complete Interrupt Flag */
#define _USART_IFC_TXC_SHIFT                  0                                /**< Shift value for USART_TXC */
#define _USART_IFC_TXC_MASK                   0x1UL                            /**< Bit mask for USART_TXC */
#define _USART_IFC_TXC_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_TXC_DEFAULT                 (_USART_IFC_TXC_DEFAULT << 0)    /**< Shifted mode DEFAULT for USART_IFC */
#define USART_IFC_RXFULL                      (0x1UL << 3)                     /**< Clear RX Buffer Full Interrupt Flag */
#define _USART_IFC_RXFULL_SHIFT               3                                /**< Shift value for USART_RXFULL */
#define _USART_IFC_RXFULL_MASK                0x8UL                            /**< Bit mask for USART_RXFULL */
#define _USART_IFC_RXFULL_DEFAULT             0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_RXFULL_DEFAULT              (_USART_IFC_RXFULL_DEFAULT << 3) /**< Shifted mode DEFAULT for USART_IFC */
#define USART_IFC_RXOF                        (0x1UL << 4)                     /**< Clear RX Overflow Interrupt Flag */
#define _USART_IFC_RXOF_SHIFT                 4                                /**< Shift value for USART_RXOF */
#define _USART_IFC_RXOF_MASK                  0x10UL                           /**< Bit mask for USART_RXOF */
#define _USART_IFC_RXOF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_RXOF_DEFAULT                (_USART_IFC_RXOF_DEFAULT << 4)   /**< Shifted mode DEFAULT for USART_IFC */
#define USART_IFC_RXUF                        (0x1UL << 5)                     /**< Clear RX Underflow Interrupt Flag */
#define _USART_IFC_RXUF_SHIFT                 5                                /**< Shift value for USART_RXUF */
#define _USART_IFC_RXUF_MASK                  0x20UL                           /**< Bit mask for USART_RXUF */
#define _USART_IFC_RXUF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_RXUF_DEFAULT                (_USART_IFC_RXUF_DEFAULT << 5)   /**< Shifted mode DEFAULT for USART_IFC */
#define USART_IFC_TXOF                        (0x1UL << 6)                     /**< Clear TX Overflow Interrupt Flag */
#define _USART_IFC_TXOF_SHIFT                 6                                /**< Shift value for USART_TXOF */
#define _USART_IFC_TXOF_MASK                  0x40UL                           /**< Bit mask for USART_TXOF */
#define _USART_IFC_TXOF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_TXOF_DEFAULT                (_USART_IFC_TXOF_DEFAULT << 6)   /**< Shifted mode DEFAULT for USART_IFC */
#define USART_IFC_TXUF                        (0x1UL << 7)                     /**< Clear TX Underflow Interrupt Flag */
#define _USART_IFC_TXUF_SHIFT                 7                                /**< Shift value for USART_TXUF */
#define _USART_IFC_TXUF_MASK                  0x80UL                           /**< Bit mask for USART_TXUF */
#define _USART_IFC_TXUF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_TXUF_DEFAULT                (_USART_IFC_TXUF_DEFAULT << 7)   /**< Shifted mode DEFAULT for USART_IFC */
#define USART_IFC_PERR                        (0x1UL << 8)                     /**< Clear Parity Error Interrupt Flag */
#define _USART_IFC_PERR_SHIFT                 8                                /**< Shift value for USART_PERR */
#define _USART_IFC_PERR_MASK                  0x100UL                          /**< Bit mask for USART_PERR */
#define _USART_IFC_PERR_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_PERR_DEFAULT                (_USART_IFC_PERR_DEFAULT << 8)   /**< Shifted mode DEFAULT for USART_IFC */
#define USART_IFC_FERR                        (0x1UL << 9)                     /**< Clear Framing Error Interrupt Flag */
#define _USART_IFC_FERR_SHIFT                 9                                /**< Shift value for USART_FERR */
#define _USART_IFC_FERR_MASK                  0x200UL                          /**< Bit mask for USART_FERR */
#define _USART_IFC_FERR_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_FERR_DEFAULT                (_USART_IFC_FERR_DEFAULT << 9)   /**< Shifted mode DEFAULT for USART_IFC */
#define USART_IFC_MPAF                        (0x1UL << 10)                    /**< Clear Multi-Processor Address Frame Interrupt Flag */
#define _USART_IFC_MPAF_SHIFT                 10                               /**< Shift value for USART_MPAF */
#define _USART_IFC_MPAF_MASK                  0x400UL                          /**< Bit mask for USART_MPAF */
#define _USART_IFC_MPAF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_MPAF_DEFAULT                (_USART_IFC_MPAF_DEFAULT << 10)  /**< Shifted mode DEFAULT for USART_IFC */
#define USART_IFC_SSM                         (0x1UL << 11)                    /**< Clear Slave-Select In Master Mode Interrupt Flag */
#define _USART_IFC_SSM_SHIFT                  11                               /**< Shift value for USART_SSM */
#define _USART_IFC_SSM_MASK                   0x800UL                          /**< Bit mask for USART_SSM */
#define _USART_IFC_SSM_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_SSM_DEFAULT                 (_USART_IFC_SSM_DEFAULT << 11)   /**< Shifted mode DEFAULT for USART_IFC */
#define USART_IFC_CCF                         (0x1UL << 12)                    /**< Clear Collision Check Fail Interrupt Flag */
#define _USART_IFC_CCF_SHIFT                  12                               /**< Shift value for USART_CCF */
#define _USART_IFC_CCF_MASK                   0x1000UL                         /**< Bit mask for USART_CCF */
#define _USART_IFC_CCF_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for USART_IFC */
#define USART_IFC_CCF_DEFAULT                 (_USART_IFC_CCF_DEFAULT << 12)   /**< Shifted mode DEFAULT for USART_IFC */

/* Bit fields for USART IEN */
#define _USART_IEN_RESETVALUE                 0x00000000UL                      /**< Default value for USART_IEN */
#define _USART_IEN_MASK                       0x00001FFFUL                      /**< Mask for USART_IEN */
#define USART_IEN_TXC                         (0x1UL << 0)                      /**< TX Complete Interrupt Enable */
#define _USART_IEN_TXC_SHIFT                  0                                 /**< Shift value for USART_TXC */
#define _USART_IEN_TXC_MASK                   0x1UL                             /**< Bit mask for USART_TXC */
#define _USART_IEN_TXC_DEFAULT                0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_TXC_DEFAULT                 (_USART_IEN_TXC_DEFAULT << 0)     /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_TXBL                        (0x1UL << 1)                      /**< TX Buffer Level Interrupt Enable */
#define _USART_IEN_TXBL_SHIFT                 1                                 /**< Shift value for USART_TXBL */
#define _USART_IEN_TXBL_MASK                  0x2UL                             /**< Bit mask for USART_TXBL */
#define _USART_IEN_TXBL_DEFAULT               0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_TXBL_DEFAULT                (_USART_IEN_TXBL_DEFAULT << 1)    /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_RXDATAV                     (0x1UL << 2)                      /**< RX Data Valid Interrupt Enable */
#define _USART_IEN_RXDATAV_SHIFT              2                                 /**< Shift value for USART_RXDATAV */
#define _USART_IEN_RXDATAV_MASK               0x4UL                             /**< Bit mask for USART_RXDATAV */
#define _USART_IEN_RXDATAV_DEFAULT            0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_RXDATAV_DEFAULT             (_USART_IEN_RXDATAV_DEFAULT << 2) /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_RXFULL                      (0x1UL << 3)                      /**< RX Buffer Full Interrupt Enable */
#define _USART_IEN_RXFULL_SHIFT               3                                 /**< Shift value for USART_RXFULL */
#define _USART_IEN_RXFULL_MASK                0x8UL                             /**< Bit mask for USART_RXFULL */
#define _USART_IEN_RXFULL_DEFAULT             0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_RXFULL_DEFAULT              (_USART_IEN_RXFULL_DEFAULT << 3)  /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_RXOF                        (0x1UL << 4)                      /**< RX Overflow Interrupt Enable */
#define _USART_IEN_RXOF_SHIFT                 4                                 /**< Shift value for USART_RXOF */
#define _USART_IEN_RXOF_MASK                  0x10UL                            /**< Bit mask for USART_RXOF */
#define _USART_IEN_RXOF_DEFAULT               0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_RXOF_DEFAULT                (_USART_IEN_RXOF_DEFAULT << 4)    /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_RXUF                        (0x1UL << 5)                      /**< RX Underflow Interrupt Enable */
#define _USART_IEN_RXUF_SHIFT                 5                                 /**< Shift value for USART_RXUF */
#define _USART_IEN_RXUF_MASK                  0x20UL                            /**< Bit mask for USART_RXUF */
#define _USART_IEN_RXUF_DEFAULT               0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_RXUF_DEFAULT                (_USART_IEN_RXUF_DEFAULT << 5)    /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_TXOF                        (0x1UL << 6)                      /**< TX Overflow Interrupt Enable */
#define _USART_IEN_TXOF_SHIFT                 6                                 /**< Shift value for USART_TXOF */
#define _USART_IEN_TXOF_MASK                  0x40UL                            /**< Bit mask for USART_TXOF */
#define _USART_IEN_TXOF_DEFAULT               0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_TXOF_DEFAULT                (_USART_IEN_TXOF_DEFAULT << 6)    /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_TXUF                        (0x1UL << 7)                      /**< TX Underflow Interrupt Enable */
#define _USART_IEN_TXUF_SHIFT                 7                                 /**< Shift value for USART_TXUF */
#define _USART_IEN_TXUF_MASK                  0x80UL                            /**< Bit mask for USART_TXUF */
#define _USART_IEN_TXUF_DEFAULT               0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_TXUF_DEFAULT                (_USART_IEN_TXUF_DEFAULT << 7)    /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_PERR                        (0x1UL << 8)                      /**< Parity Error Interrupt Enable */
#define _USART_IEN_PERR_SHIFT                 8                                 /**< Shift value for USART_PERR */
#define _USART_IEN_PERR_MASK                  0x100UL                           /**< Bit mask for USART_PERR */
#define _USART_IEN_PERR_DEFAULT               0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_PERR_DEFAULT                (_USART_IEN_PERR_DEFAULT << 8)    /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_FERR                        (0x1UL << 9)                      /**< Framing Error Interrupt Enable */
#define _USART_IEN_FERR_SHIFT                 9                                 /**< Shift value for USART_FERR */
#define _USART_IEN_FERR_MASK                  0x200UL                           /**< Bit mask for USART_FERR */
#define _USART_IEN_FERR_DEFAULT               0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_FERR_DEFAULT                (_USART_IEN_FERR_DEFAULT << 9)    /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_MPAF                        (0x1UL << 10)                     /**< Multi-Processor Address Frame Interrupt Enable */
#define _USART_IEN_MPAF_SHIFT                 10                                /**< Shift value for USART_MPAF */
#define _USART_IEN_MPAF_MASK                  0x400UL                           /**< Bit mask for USART_MPAF */
#define _USART_IEN_MPAF_DEFAULT               0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_MPAF_DEFAULT                (_USART_IEN_MPAF_DEFAULT << 10)   /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_SSM                         (0x1UL << 11)                     /**< Slave-Select In Master Mode Interrupt Enable */
#define _USART_IEN_SSM_SHIFT                  11                                /**< Shift value for USART_SSM */
#define _USART_IEN_SSM_MASK                   0x800UL                           /**< Bit mask for USART_SSM */
#define _USART_IEN_SSM_DEFAULT                0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_SSM_DEFAULT                 (_USART_IEN_SSM_DEFAULT << 11)    /**< Shifted mode DEFAULT for USART_IEN */
#define USART_IEN_CCF                         (0x1UL << 12)                     /**< Collision Check Fail Interrupt Enable */
#define _USART_IEN_CCF_SHIFT                  12                                /**< Shift value for USART_CCF */
#define _USART_IEN_CCF_MASK                   0x1000UL                          /**< Bit mask for USART_CCF */
#define _USART_IEN_CCF_DEFAULT                0x00000000UL                      /**< Mode DEFAULT for USART_IEN */
#define USART_IEN_CCF_DEFAULT                 (_USART_IEN_CCF_DEFAULT << 12)    /**< Shifted mode DEFAULT for USART_IEN */

/* Bit fields for USART IRCTRL */
#define _USART_IRCTRL_RESETVALUE              0x00000000UL                          /**< Default value for USART_IRCTRL */
#define _USART_IRCTRL_MASK                    0x000000FFUL                          /**< Mask for USART_IRCTRL */
#define USART_IRCTRL_IREN                     (0x1UL << 0)                          /**< Enable IrDA Module */
#define _USART_IRCTRL_IREN_SHIFT              0                                     /**< Shift value for USART_IREN */
#define _USART_IRCTRL_IREN_MASK               0x1UL                                 /**< Bit mask for USART_IREN */
#define _USART_IRCTRL_IREN_DEFAULT            0x00000000UL                          /**< Mode DEFAULT for USART_IRCTRL */
#define USART_IRCTRL_IREN_DEFAULT             (_USART_IRCTRL_IREN_DEFAULT << 0)     /**< Shifted mode DEFAULT for USART_IRCTRL */
#define _USART_IRCTRL_IRPW_SHIFT              1                                     /**< Shift value for USART_IRPW */
#define _USART_IRCTRL_IRPW_MASK               0x6UL                                 /**< Bit mask for USART_IRPW */
#define _USART_IRCTRL_IRPW_DEFAULT            0x00000000UL                          /**< Mode DEFAULT for USART_IRCTRL */
#define _USART_IRCTRL_IRPW_ONE                0x00000000UL                          /**< Mode ONE for USART_IRCTRL */
#define _USART_IRCTRL_IRPW_TWO                0x00000001UL                          /**< Mode TWO for USART_IRCTRL */
#define _USART_IRCTRL_IRPW_THREE              0x00000002UL                          /**< Mode THREE for USART_IRCTRL */
#define _USART_IRCTRL_IRPW_FOUR               0x00000003UL                          /**< Mode FOUR for USART_IRCTRL */
#define USART_IRCTRL_IRPW_DEFAULT             (_USART_IRCTRL_IRPW_DEFAULT << 1)     /**< Shifted mode DEFAULT for USART_IRCTRL */
#define USART_IRCTRL_IRPW_ONE                 (_USART_IRCTRL_IRPW_ONE << 1)         /**< Shifted mode ONE for USART_IRCTRL */
#define USART_IRCTRL_IRPW_TWO                 (_USART_IRCTRL_IRPW_TWO << 1)         /**< Shifted mode TWO for USART_IRCTRL */
#define USART_IRCTRL_IRPW_THREE               (_USART_IRCTRL_IRPW_THREE << 1)       /**< Shifted mode THREE for USART_IRCTRL */
#define USART_IRCTRL_IRPW_FOUR                (_USART_IRCTRL_IRPW_FOUR << 1)        /**< Shifted mode FOUR for USART_IRCTRL */
#define USART_IRCTRL_IRFILT                   (0x1UL << 3)                          /**< IrDA RX Filter */
#define _USART_IRCTRL_IRFILT_SHIFT            3                                     /**< Shift value for USART_IRFILT */
#define _USART_IRCTRL_IRFILT_MASK             0x8UL                                 /**< Bit mask for USART_IRFILT */
#define _USART_IRCTRL_IRFILT_DEFAULT          0x00000000UL                          /**< Mode DEFAULT for USART_IRCTRL */
#define USART_IRCTRL_IRFILT_DEFAULT           (_USART_IRCTRL_IRFILT_DEFAULT << 3)   /**< Shifted mode DEFAULT for USART_IRCTRL */
#define _USART_IRCTRL_IRPRSSEL_SHIFT          4                                     /**< Shift value for USART_IRPRSSEL */
#define _USART_IRCTRL_IRPRSSEL_MASK           0x70UL                                /**< Bit mask for USART_IRPRSSEL */
#define _USART_IRCTRL_IRPRSSEL_DEFAULT        0x00000000UL                          /**< Mode DEFAULT for USART_IRCTRL */
#define _USART_IRCTRL_IRPRSSEL_PRSCH0         0x00000000UL                          /**< Mode PRSCH0 for USART_IRCTRL */
#define _USART_IRCTRL_IRPRSSEL_PRSCH1         0x00000001UL                          /**< Mode PRSCH1 for USART_IRCTRL */
#define _USART_IRCTRL_IRPRSSEL_PRSCH2         0x00000002UL                          /**< Mode PRSCH2 for USART_IRCTRL */
#define _USART_IRCTRL_IRPRSSEL_PRSCH3         0x00000003UL                          /**< Mode PRSCH3 for USART_IRCTRL */
#define _USART_IRCTRL_IRPRSSEL_PRSCH4         0x00000004UL                          /**< Mode PRSCH4 for USART_IRCTRL */
#define _USART_IRCTRL_IRPRSSEL_PRSCH5         0x00000005UL                          /**< Mode PRSCH5 for USART_IRCTRL */
#define _USART_IRCTRL_IRPRSSEL_PRSCH6         0x00000006UL                          /**< Mode PRSCH6 for USART_IRCTRL */
#define _USART_IRCTRL_IRPRSSEL_PRSCH7         0x00000007UL                          /**< Mode PRSCH7 for USART_IRCTRL */
#define USART_IRCTRL_IRPRSSEL_DEFAULT         (_USART_IRCTRL_IRPRSSEL_DEFAULT << 4) /**< Shifted mode DEFAULT for USART_IRCTRL */
#define USART_IRCTRL_IRPRSSEL_PRSCH0          (_USART_IRCTRL_IRPRSSEL_PRSCH0 << 4)  /**< Shifted mode PRSCH0 for USART_IRCTRL */
#define USART_IRCTRL_IRPRSSEL_PRSCH1          (_USART_IRCTRL_IRPRSSEL_PRSCH1 << 4)  /**< Shifted mode PRSCH1 for USART_IRCTRL */
#define USART_IRCTRL_IRPRSSEL_PRSCH2          (_USART_IRCTRL_IRPRSSEL_PRSCH2 << 4)  /**< Shifted mode PRSCH2 for USART_IRCTRL */
#define USART_IRCTRL_IRPRSSEL_PRSCH3          (_USART_IRCTRL_IRPRSSEL_PRSCH3 << 4)  /**< Shifted mode PRSCH3 for USART_IRCTRL */
#define USART_IRCTRL_IRPRSSEL_PRSCH4          (_USART_IRCTRL_IRPRSSEL_PRSCH4 << 4)  /**< Shifted mode PRSCH4 for USART_IRCTRL */
#define USART_IRCTRL_IRPRSSEL_PRSCH5          (_USART_IRCTRL_IRPRSSEL_PRSCH5 << 4)  /**< Shifted mode PRSCH5 for USART_IRCTRL */
#define USART_IRCTRL_IRPRSSEL_PRSCH6          (_USART_IRCTRL_IRPRSSEL_PRSCH6 << 4)  /**< Shifted mode PRSCH6 for USART_IRCTRL */
#define USART_IRCTRL_IRPRSSEL_PRSCH7          (_USART_IRCTRL_IRPRSSEL_PRSCH7 << 4)  /**< Shifted mode PRSCH7 for USART_IRCTRL */
#define USART_IRCTRL_IRPRSEN                  (0x1UL << 7)                          /**< IrDA PRS Channel Enable */
#define _USART_IRCTRL_IRPRSEN_SHIFT           7                                     /**< Shift value for USART_IRPRSEN */
#define _USART_IRCTRL_IRPRSEN_MASK            0x80UL                                /**< Bit mask for USART_IRPRSEN */
#define _USART_IRCTRL_IRPRSEN_DEFAULT         0x00000000UL                          /**< Mode DEFAULT for USART_IRCTRL */
#define USART_IRCTRL_IRPRSEN_DEFAULT          (_USART_IRCTRL_IRPRSEN_DEFAULT << 7)  /**< Shifted mode DEFAULT for USART_IRCTRL */

/* Bit fields for USART ROUTE */
#define _USART_ROUTE_RESETVALUE               0x00000000UL                         /**< Default value for USART_ROUTE */
#define _USART_ROUTE_MASK                     0x0000070FUL                         /**< Mask for USART_ROUTE */
#define USART_ROUTE_RXPEN                     (0x1UL << 0)                         /**< RX Pin Enable */
#define _USART_ROUTE_RXPEN_SHIFT              0                                    /**< Shift value for USART_RXPEN */
#define _USART_ROUTE_RXPEN_MASK               0x1UL                                /**< Bit mask for USART_RXPEN */
#define _USART_ROUTE_RXPEN_DEFAULT            0x00000000UL                         /**< Mode DEFAULT for USART_ROUTE */
#define USART_ROUTE_RXPEN_DEFAULT             (_USART_ROUTE_RXPEN_DEFAULT << 0)    /**< Shifted mode DEFAULT for USART_ROUTE */
#define USART_ROUTE_TXPEN                     (0x1UL << 1)                         /**< TX Pin Enable */
#define _USART_ROUTE_TXPEN_SHIFT              1                                    /**< Shift value for USART_TXPEN */
#define _USART_ROUTE_TXPEN_MASK               0x2UL                                /**< Bit mask for USART_TXPEN */
#define _USART_ROUTE_TXPEN_DEFAULT            0x00000000UL                         /**< Mode DEFAULT for USART_ROUTE */
#define USART_ROUTE_TXPEN_DEFAULT             (_USART_ROUTE_TXPEN_DEFAULT << 1)    /**< Shifted mode DEFAULT for USART_ROUTE */
#define USART_ROUTE_CSPEN                     (0x1UL << 2)                         /**< CS Pin Enable */
#define _USART_ROUTE_CSPEN_SHIFT              2                                    /**< Shift value for USART_CSPEN */
#define _USART_ROUTE_CSPEN_MASK               0x4UL                                /**< Bit mask for USART_CSPEN */
#define _USART_ROUTE_CSPEN_DEFAULT            0x00000000UL                         /**< Mode DEFAULT for USART_ROUTE */
#define USART_ROUTE_CSPEN_DEFAULT             (_USART_ROUTE_CSPEN_DEFAULT << 2)    /**< Shifted mode DEFAULT for USART_ROUTE */
#define USART_ROUTE_CLKPEN                    (0x1UL << 3)                         /**< CLK Pin Enable */
#define _USART_ROUTE_CLKPEN_SHIFT             3                                    /**< Shift value for USART_CLKPEN */
#define _USART_ROUTE_CLKPEN_MASK              0x8UL                                /**< Bit mask for USART_CLKPEN */
#define _USART_ROUTE_CLKPEN_DEFAULT           0x00000000UL                         /**< Mode DEFAULT for USART_ROUTE */
#define USART_ROUTE_CLKPEN_DEFAULT            (_USART_ROUTE_CLKPEN_DEFAULT << 3)   /**< Shifted mode DEFAULT for USART_ROUTE */
#define _USART_ROUTE_LOCATION_SHIFT           8                                    /**< Shift value for USART_LOCATION */
#define _USART_ROUTE_LOCATION_MASK            0x700UL                              /**< Bit mask for USART_LOCATION */
#define _USART_ROUTE_LOCATION_LOC0            0x00000000UL                         /**< Mode LOC0 for USART_ROUTE */
#define _USART_ROUTE_LOCATION_DEFAULT         0x00000000UL                         /**< Mode DEFAULT for USART_ROUTE */
#define _USART_ROUTE_LOCATION_LOC1            0x00000001UL                         /**< Mode LOC1 for USART_ROUTE */
#define _USART_ROUTE_LOCATION_LOC2            0x00000002UL                         /**< Mode LOC2 for USART_ROUTE */
#define _USART_ROUTE_LOCATION_LOC3            0x00000003UL                         /**< Mode LOC3 for USART_ROUTE */
#define _USART_ROUTE_LOCATION_LOC4            0x00000004UL                         /**< Mode LOC4 for USART_ROUTE */
#define _USART_ROUTE_LOCATION_LOC5            0x00000005UL                         /**< Mode LOC5 for USART_ROUTE */
#define USART_ROUTE_LOCATION_LOC0             (_USART_ROUTE_LOCATION_LOC0 << 8)    /**< Shifted mode LOC0 for USART_ROUTE */
#define USART_ROUTE_LOCATION_DEFAULT          (_USART_ROUTE_LOCATION_DEFAULT << 8) /**< Shifted mode DEFAULT for USART_ROUTE */
#define USART_ROUTE_LOCATION_LOC1             (_USART_ROUTE_LOCATION_LOC1 << 8)    /**< Shifted mode LOC1 for USART_ROUTE */
#define USART_ROUTE_LOCATION_LOC2             (_USART_ROUTE_LOCATION_LOC2 << 8)    /**< Shifted mode LOC2 for USART_ROUTE */
#define USART_ROUTE_LOCATION_LOC3             (_USART_ROUTE_LOCATION_LOC3 << 8)    /**< Shifted mode LOC3 for USART_ROUTE */
#define USART_ROUTE_LOCATION_LOC4             (_USART_ROUTE_LOCATION_LOC4 << 8)    /**< Shifted mode LOC4 for USART_ROUTE */
#define USART_ROUTE_LOCATION_LOC5             (_USART_ROUTE_LOCATION_LOC5 << 8)    /**< Shifted mode LOC5 for USART_ROUTE */

/* Bit fields for USART INPUT */
#define _USART_INPUT_RESETVALUE               0x00000000UL                         /**< Default value for USART_INPUT */
#define _USART_INPUT_MASK                     0x0000001FUL                         /**< Mask for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_SHIFT           0                                    /**< Shift value for USART_RXPRSSEL */
#define _USART_INPUT_RXPRSSEL_MASK            0xFUL                                /**< Bit mask for USART_RXPRSSEL */
#define _USART_INPUT_RXPRSSEL_DEFAULT         0x00000000UL                         /**< Mode DEFAULT for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH0          0x00000000UL                         /**< Mode PRSCH0 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH1          0x00000001UL                         /**< Mode PRSCH1 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH2          0x00000002UL                         /**< Mode PRSCH2 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH3          0x00000003UL                         /**< Mode PRSCH3 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH4          0x00000004UL                         /**< Mode PRSCH4 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH5          0x00000005UL                         /**< Mode PRSCH5 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH6          0x00000006UL                         /**< Mode PRSCH6 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH7          0x00000007UL                         /**< Mode PRSCH7 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH8          0x00000008UL                         /**< Mode PRSCH8 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH9          0x00000009UL                         /**< Mode PRSCH9 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH10         0x0000000AUL                         /**< Mode PRSCH10 for USART_INPUT */
#define _USART_INPUT_RXPRSSEL_PRSCH11         0x0000000BUL                         /**< Mode PRSCH11 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_DEFAULT          (_USART_INPUT_RXPRSSEL_DEFAULT << 0) /**< Shifted mode DEFAULT for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH0           (_USART_INPUT_RXPRSSEL_PRSCH0 << 0)  /**< Shifted mode PRSCH0 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH1           (_USART_INPUT_RXPRSSEL_PRSCH1 << 0)  /**< Shifted mode PRSCH1 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH2           (_USART_INPUT_RXPRSSEL_PRSCH2 << 0)  /**< Shifted mode PRSCH2 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH3           (_USART_INPUT_RXPRSSEL_PRSCH3 << 0)  /**< Shifted mode PRSCH3 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH4           (_USART_INPUT_RXPRSSEL_PRSCH4 << 0)  /**< Shifted mode PRSCH4 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH5           (_USART_INPUT_RXPRSSEL_PRSCH5 << 0)  /**< Shifted mode PRSCH5 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH6           (_USART_INPUT_RXPRSSEL_PRSCH6 << 0)  /**< Shifted mode PRSCH6 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH7           (_USART_INPUT_RXPRSSEL_PRSCH7 << 0)  /**< Shifted mode PRSCH7 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH8           (_USART_INPUT_RXPRSSEL_PRSCH8 << 0)  /**< Shifted mode PRSCH8 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH9           (_USART_INPUT_RXPRSSEL_PRSCH9 << 0)  /**< Shifted mode PRSCH9 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH10          (_USART_INPUT_RXPRSSEL_PRSCH10 << 0) /**< Shifted mode PRSCH10 for USART_INPUT */
#define USART_INPUT_RXPRSSEL_PRSCH11          (_USART_INPUT_RXPRSSEL_PRSCH11 << 0) /**< Shifted mode PRSCH11 for USART_INPUT */
#define USART_INPUT_RXPRS                     (0x1UL << 4)                         /**< PRS RX Enable */
#define _USART_INPUT_RXPRS_SHIFT              4                                    /**< Shift value for USART_RXPRS */
#define _USART_INPUT_RXPRS_MASK               0x10UL                               /**< Bit mask for USART_RXPRS */
#define _USART_INPUT_RXPRS_DEFAULT            0x00000000UL                         /**< Mode DEFAULT for USART_INPUT */
#define USART_INPUT_RXPRS_DEFAULT             (_USART_INPUT_RXPRS_DEFAULT << 4)    /**< Shifted mode DEFAULT for USART_INPUT */

/* Bit fields for USART I2SCTRL */
#define _USART_I2SCTRL_RESETVALUE             0x00000000UL                           /**< Default value for USART_I2SCTRL */
#define _USART_I2SCTRL_MASK                   0x0000071FUL                           /**< Mask for USART_I2SCTRL */
#define USART_I2SCTRL_EN                      (0x1UL << 0)                           /**< Enable I2S Mode */
#define _USART_I2SCTRL_EN_SHIFT               0                                      /**< Shift value for USART_EN */
#define _USART_I2SCTRL_EN_MASK                0x1UL                                  /**< Bit mask for USART_EN */
#define _USART_I2SCTRL_EN_DEFAULT             0x00000000UL                           /**< Mode DEFAULT for USART_I2SCTRL */
#define USART_I2SCTRL_EN_DEFAULT              (_USART_I2SCTRL_EN_DEFAULT << 0)       /**< Shifted mode DEFAULT for USART_I2SCTRL */
#define USART_I2SCTRL_MONO                    (0x1UL << 1)                           /**< Stero or Mono */
#define _USART_I2SCTRL_MONO_SHIFT             1                                      /**< Shift value for USART_MONO */
#define _USART_I2SCTRL_MONO_MASK              0x2UL                                  /**< Bit mask for USART_MONO */
#define _USART_I2SCTRL_MONO_DEFAULT           0x00000000UL                           /**< Mode DEFAULT for USART_I2SCTRL */
#define USART_I2SCTRL_MONO_DEFAULT            (_USART_I2SCTRL_MONO_DEFAULT << 1)     /**< Shifted mode DEFAULT for USART_I2SCTRL */
#define USART_I2SCTRL_JUSTIFY                 (0x1UL << 2)                           /**< Justification of I2S Data */
#define _USART_I2SCTRL_JUSTIFY_SHIFT          2                                      /**< Shift value for USART_JUSTIFY */
#define _USART_I2SCTRL_JUSTIFY_MASK           0x4UL                                  /**< Bit mask for USART_JUSTIFY */
#define _USART_I2SCTRL_JUSTIFY_DEFAULT        0x00000000UL                           /**< Mode DEFAULT for USART_I2SCTRL */
#define _USART_I2SCTRL_JUSTIFY_LEFT           0x00000000UL                           /**< Mode LEFT for USART_I2SCTRL */
#define _USART_I2SCTRL_JUSTIFY_RIGHT          0x00000001UL                           /**< Mode RIGHT for USART_I2SCTRL */
#define USART_I2SCTRL_JUSTIFY_DEFAULT         (_USART_I2SCTRL_JUSTIFY_DEFAULT << 2)  /**< Shifted mode DEFAULT for USART_I2SCTRL */
#define USART_I2SCTRL_JUSTIFY_LEFT            (_USART_I2SCTRL_JUSTIFY_LEFT << 2)     /**< Shifted mode LEFT for USART_I2SCTRL */
#define USART_I2SCTRL_JUSTIFY_RIGHT           (_USART_I2SCTRL_JUSTIFY_RIGHT << 2)    /**< Shifted mode RIGHT for USART_I2SCTRL */
#define USART_I2SCTRL_DMASPLIT                (0x1UL << 3)                           /**< Separate DMA Request For Left/Right Data */
#define _USART_I2SCTRL_DMASPLIT_SHIFT         3                                      /**< Shift value for USART_DMASPLIT */
#define _USART_I2SCTRL_DMASPLIT_MASK          0x8UL                                  /**< Bit mask for USART_DMASPLIT */
#define _USART_I2SCTRL_DMASPLIT_DEFAULT       0x00000000UL                           /**< Mode DEFAULT for USART_I2SCTRL */
#define USART_I2SCTRL_DMASPLIT_DEFAULT        (_USART_I2SCTRL_DMASPLIT_DEFAULT << 3) /**< Shifted mode DEFAULT for USART_I2SCTRL */
#define USART_I2SCTRL_DELAY                   (0x1UL << 4)                           /**< Delay on I2S data */
#define _USART_I2SCTRL_DELAY_SHIFT            4                                      /**< Shift value for USART_DELAY */
#define _USART_I2SCTRL_DELAY_MASK             0x10UL                                 /**< Bit mask for USART_DELAY */
#define _USART_I2SCTRL_DELAY_DEFAULT          0x00000000UL                           /**< Mode DEFAULT for USART_I2SCTRL */
#define USART_I2SCTRL_DELAY_DEFAULT           (_USART_I2SCTRL_DELAY_DEFAULT << 4)    /**< Shifted mode DEFAULT for USART_I2SCTRL */
#define _USART_I2SCTRL_FORMAT_SHIFT           8                                      /**< Shift value for USART_FORMAT */
#define _USART_I2SCTRL_FORMAT_MASK            0x700UL                                /**< Bit mask for USART_FORMAT */
#define _USART_I2SCTRL_FORMAT_DEFAULT         0x00000000UL                           /**< Mode DEFAULT for USART_I2SCTRL */
#define _USART_I2SCTRL_FORMAT_W32D32          0x00000000UL                           /**< Mode W32D32 for USART_I2SCTRL */
#define _USART_I2SCTRL_FORMAT_W32D24M         0x00000001UL                           /**< Mode W32D24M for USART_I2SCTRL */
#define _USART_I2SCTRL_FORMAT_W32D24          0x00000002UL                           /**< Mode W32D24 for USART_I2SCTRL */
#define _USART_I2SCTRL_FORMAT_W32D16          0x00000003UL                           /**< Mode W32D16 for USART_I2SCTRL */
#define _USART_I2SCTRL_FORMAT_W32D8           0x00000004UL                           /**< Mode W32D8 for USART_I2SCTRL */
#define _USART_I2SCTRL_FORMAT_W16D16          0x00000005UL                           /**< Mode W16D16 for USART_I2SCTRL */
#define _USART_I2SCTRL_FORMAT_W16D8           0x00000006UL                           /**< Mode W16D8 for USART_I2SCTRL */
#define _USART_I2SCTRL_FORMAT_W8D8            0x00000007UL                           /**< Mode W8D8 for USART_I2SCTRL */
#define USART_I2SCTRL_FORMAT_DEFAULT          (_USART_I2SCTRL_FORMAT_DEFAULT << 8)   /**< Shifted mode DEFAULT for USART_I2SCTRL */
#define USART_I2SCTRL_FORMAT_W32D32           (_USART_I2SCTRL_FORMAT_W32D32 << 8)    /**< Shifted mode W32D32 for USART_I2SCTRL */
#define USART_I2SCTRL_FORMAT_W32D24M          (_USART_I2SCTRL_FORMAT_W32D24M << 8)   /**< Shifted mode W32D24M for USART_I2SCTRL */
#define USART_I2SCTRL_FORMAT_W32D24           (_USART_I2SCTRL_FORMAT_W32D24 << 8)    /**< Shifted mode W32D24 for USART_I2SCTRL */
#define USART_I2SCTRL_FORMAT_W32D16           (_USART_I2SCTRL_FORMAT_W32D16 << 8)    /**< Shifted mode W32D16 for USART_I2SCTRL */
#define USART_I2SCTRL_FORMAT_W32D8            (_USART_I2SCTRL_FORMAT_W32D8 << 8)     /**< Shifted mode W32D8 for USART_I2SCTRL */
#define USART_I2SCTRL_FORMAT_W16D16           (_USART_I2SCTRL_FORMAT_W16D16 << 8)    /**< Shifted mode W16D16 for USART_I2SCTRL */
#define USART_I2SCTRL_FORMAT_W16D8            (_USART_I2SCTRL_FORMAT_W16D8 << 8)     /**< Shifted mode W16D8 for USART_I2SCTRL */
#define USART_I2SCTRL_FORMAT_W8D8             (_USART_I2SCTRL_FORMAT_W8D8 << 8)      /**< Shifted mode W8D8 for USART_I2SCTRL */

/** @} End of group EFM32LG990F256_USART */


/**************************************************************************//**
 * @defgroup EFM32LG990F256_UART_BitFields  EFM32LG990F256_UART Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for UART CTRL */
#define _UART_CTRL_RESETVALUE                0x00000000UL                            /**< Default value for UART_CTRL */
#define _UART_CTRL_MASK                      0x7DFFFF7FUL                            /**< Mask for UART_CTRL */
#define UART_CTRL_SYNC                       (0x1UL << 0)                            /**< USART Synchronous Mode */
#define _UART_CTRL_SYNC_SHIFT                0                                       /**< Shift value for USART_SYNC */
#define _UART_CTRL_SYNC_MASK                 0x1UL                                   /**< Bit mask for USART_SYNC */
#define _UART_CTRL_SYNC_DEFAULT              0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_SYNC_DEFAULT               (_UART_CTRL_SYNC_DEFAULT << 0)          /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_LOOPBK                     (0x1UL << 1)                            /**< Loopback Enable */
#define _UART_CTRL_LOOPBK_SHIFT              1                                       /**< Shift value for USART_LOOPBK */
#define _UART_CTRL_LOOPBK_MASK               0x2UL                                   /**< Bit mask for USART_LOOPBK */
#define _UART_CTRL_LOOPBK_DEFAULT            0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_LOOPBK_DEFAULT             (_UART_CTRL_LOOPBK_DEFAULT << 1)        /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_CCEN                       (0x1UL << 2)                            /**< Collision Check Enable */
#define _UART_CTRL_CCEN_SHIFT                2                                       /**< Shift value for USART_CCEN */
#define _UART_CTRL_CCEN_MASK                 0x4UL                                   /**< Bit mask for USART_CCEN */
#define _UART_CTRL_CCEN_DEFAULT              0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_CCEN_DEFAULT               (_UART_CTRL_CCEN_DEFAULT << 2)          /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_MPM                        (0x1UL << 3)                            /**< Multi-Processor Mode */
#define _UART_CTRL_MPM_SHIFT                 3                                       /**< Shift value for USART_MPM */
#define _UART_CTRL_MPM_MASK                  0x8UL                                   /**< Bit mask for USART_MPM */
#define _UART_CTRL_MPM_DEFAULT               0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_MPM_DEFAULT                (_UART_CTRL_MPM_DEFAULT << 3)           /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_MPAB                       (0x1UL << 4)                            /**< Multi-Processor Address-Bit */
#define _UART_CTRL_MPAB_SHIFT                4                                       /**< Shift value for USART_MPAB */
#define _UART_CTRL_MPAB_MASK                 0x10UL                                  /**< Bit mask for USART_MPAB */
#define _UART_CTRL_MPAB_DEFAULT              0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_MPAB_DEFAULT               (_UART_CTRL_MPAB_DEFAULT << 4)          /**< Shifted mode DEFAULT for UART_CTRL */
#define _UART_CTRL_OVS_SHIFT                 5                                       /**< Shift value for USART_OVS */
#define _UART_CTRL_OVS_MASK                  0x60UL                                  /**< Bit mask for USART_OVS */
#define _UART_CTRL_OVS_DEFAULT               0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define _UART_CTRL_OVS_X16                   0x00000000UL                            /**< Mode X16 for UART_CTRL */
#define _UART_CTRL_OVS_X8                    0x00000001UL                            /**< Mode X8 for UART_CTRL */
#define _UART_CTRL_OVS_X6                    0x00000002UL                            /**< Mode X6 for UART_CTRL */
#define _UART_CTRL_OVS_X4                    0x00000003UL                            /**< Mode X4 for UART_CTRL */
#define UART_CTRL_OVS_DEFAULT                (_UART_CTRL_OVS_DEFAULT << 5)           /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_OVS_X16                    (_UART_CTRL_OVS_X16 << 5)               /**< Shifted mode X16 for UART_CTRL */
#define UART_CTRL_OVS_X8                     (_UART_CTRL_OVS_X8 << 5)                /**< Shifted mode X8 for UART_CTRL */
#define UART_CTRL_OVS_X6                     (_UART_CTRL_OVS_X6 << 5)                /**< Shifted mode X6 for UART_CTRL */
#define UART_CTRL_OVS_X4                     (_UART_CTRL_OVS_X4 << 5)                /**< Shifted mode X4 for UART_CTRL */
#define UART_CTRL_CLKPOL                     (0x1UL << 8)                            /**< Clock Polarity */
#define _UART_CTRL_CLKPOL_SHIFT              8                                       /**< Shift value for USART_CLKPOL */
#define _UART_CTRL_CLKPOL_MASK               0x100UL                                 /**< Bit mask for USART_CLKPOL */
#define _UART_CTRL_CLKPOL_DEFAULT            0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define _UART_CTRL_CLKPOL_IDLELOW            0x00000000UL                            /**< Mode IDLELOW for UART_CTRL */
#define _UART_CTRL_CLKPOL_IDLEHIGH           0x00000001UL                            /**< Mode IDLEHIGH for UART_CTRL */
#define UART_CTRL_CLKPOL_DEFAULT             (_UART_CTRL_CLKPOL_DEFAULT << 8)        /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_CLKPOL_IDLELOW             (_UART_CTRL_CLKPOL_IDLELOW << 8)        /**< Shifted mode IDLELOW for UART_CTRL */
#define UART_CTRL_CLKPOL_IDLEHIGH            (_UART_CTRL_CLKPOL_IDLEHIGH << 8)       /**< Shifted mode IDLEHIGH for UART_CTRL */
#define UART_CTRL_CLKPHA                     (0x1UL << 9)                            /**< Clock Edge For Setup/Sample */
#define _UART_CTRL_CLKPHA_SHIFT              9                                       /**< Shift value for USART_CLKPHA */
#define _UART_CTRL_CLKPHA_MASK               0x200UL                                 /**< Bit mask for USART_CLKPHA */
#define _UART_CTRL_CLKPHA_DEFAULT            0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define _UART_CTRL_CLKPHA_SAMPLELEADING      0x00000000UL                            /**< Mode SAMPLELEADING for UART_CTRL */
#define _UART_CTRL_CLKPHA_SAMPLETRAILING     0x00000001UL                            /**< Mode SAMPLETRAILING for UART_CTRL */
#define UART_CTRL_CLKPHA_DEFAULT             (_UART_CTRL_CLKPHA_DEFAULT << 9)        /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_CLKPHA_SAMPLELEADING       (_UART_CTRL_CLKPHA_SAMPLELEADING << 9)  /**< Shifted mode SAMPLELEADING for UART_CTRL */
#define UART_CTRL_CLKPHA_SAMPLETRAILING      (_UART_CTRL_CLKPHA_SAMPLETRAILING << 9) /**< Shifted mode SAMPLETRAILING for UART_CTRL */
#define UART_CTRL_MSBF                       (0x1UL << 10)                           /**< Most Significant Bit First */
#define _UART_CTRL_MSBF_SHIFT                10                                      /**< Shift value for USART_MSBF */
#define _UART_CTRL_MSBF_MASK                 0x400UL                                 /**< Bit mask for USART_MSBF */
#define _UART_CTRL_MSBF_DEFAULT              0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_MSBF_DEFAULT               (_UART_CTRL_MSBF_DEFAULT << 10)         /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_CSMA                       (0x1UL << 11)                           /**< Action On Slave-Select In Master Mode */
#define _UART_CTRL_CSMA_SHIFT                11                                      /**< Shift value for USART_CSMA */
#define _UART_CTRL_CSMA_MASK                 0x800UL                                 /**< Bit mask for USART_CSMA */
#define _UART_CTRL_CSMA_DEFAULT              0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define _UART_CTRL_CSMA_NOACTION             0x00000000UL                            /**< Mode NOACTION for UART_CTRL */
#define _UART_CTRL_CSMA_GOTOSLAVEMODE        0x00000001UL                            /**< Mode GOTOSLAVEMODE for UART_CTRL */
#define UART_CTRL_CSMA_DEFAULT               (_UART_CTRL_CSMA_DEFAULT << 11)         /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_CSMA_NOACTION              (_UART_CTRL_CSMA_NOACTION << 11)        /**< Shifted mode NOACTION for UART_CTRL */
#define UART_CTRL_CSMA_GOTOSLAVEMODE         (_UART_CTRL_CSMA_GOTOSLAVEMODE << 11)   /**< Shifted mode GOTOSLAVEMODE for UART_CTRL */
#define UART_CTRL_TXBIL                      (0x1UL << 12)                           /**< TX Buffer Interrupt Level */
#define _UART_CTRL_TXBIL_SHIFT               12                                      /**< Shift value for USART_TXBIL */
#define _UART_CTRL_TXBIL_MASK                0x1000UL                                /**< Bit mask for USART_TXBIL */
#define _UART_CTRL_TXBIL_DEFAULT             0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define _UART_CTRL_TXBIL_EMPTY               0x00000000UL                            /**< Mode EMPTY for UART_CTRL */
#define _UART_CTRL_TXBIL_HALFFULL            0x00000001UL                            /**< Mode HALFFULL for UART_CTRL */
#define UART_CTRL_TXBIL_DEFAULT              (_UART_CTRL_TXBIL_DEFAULT << 12)        /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_TXBIL_EMPTY                (_UART_CTRL_TXBIL_EMPTY << 12)          /**< Shifted mode EMPTY for UART_CTRL */
#define UART_CTRL_TXBIL_HALFFULL             (_UART_CTRL_TXBIL_HALFFULL << 12)       /**< Shifted mode HALFFULL for UART_CTRL */
#define UART_CTRL_RXINV                      (0x1UL << 13)                           /**< Receiver Input Invert */
#define _UART_CTRL_RXINV_SHIFT               13                                      /**< Shift value for USART_RXINV */
#define _UART_CTRL_RXINV_MASK                0x2000UL                                /**< Bit mask for USART_RXINV */
#define _UART_CTRL_RXINV_DEFAULT             0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_RXINV_DEFAULT              (_UART_CTRL_RXINV_DEFAULT << 13)        /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_TXINV                      (0x1UL << 14)                           /**< Transmitter output Invert */
#define _UART_CTRL_TXINV_SHIFT               14                                      /**< Shift value for USART_TXINV */
#define _UART_CTRL_TXINV_MASK                0x4000UL                                /**< Bit mask for USART_TXINV */
#define _UART_CTRL_TXINV_DEFAULT             0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_TXINV_DEFAULT              (_UART_CTRL_TXINV_DEFAULT << 14)        /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_CSINV                      (0x1UL << 15)                           /**< Chip Select Invert */
#define _UART_CTRL_CSINV_SHIFT               15                                      /**< Shift value for USART_CSINV */
#define _UART_CTRL_CSINV_MASK                0x8000UL                                /**< Bit mask for USART_CSINV */
#define _UART_CTRL_CSINV_DEFAULT             0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_CSINV_DEFAULT              (_UART_CTRL_CSINV_DEFAULT << 15)        /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_AUTOCS                     (0x1UL << 16)                           /**< Automatic Chip Select */
#define _UART_CTRL_AUTOCS_SHIFT              16                                      /**< Shift value for USART_AUTOCS */
#define _UART_CTRL_AUTOCS_MASK               0x10000UL                               /**< Bit mask for USART_AUTOCS */
#define _UART_CTRL_AUTOCS_DEFAULT            0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_AUTOCS_DEFAULT             (_UART_CTRL_AUTOCS_DEFAULT << 16)       /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_AUTOTRI                    (0x1UL << 17)                           /**< Automatic TX Tristate */
#define _UART_CTRL_AUTOTRI_SHIFT             17                                      /**< Shift value for USART_AUTOTRI */
#define _UART_CTRL_AUTOTRI_MASK              0x20000UL                               /**< Bit mask for USART_AUTOTRI */
#define _UART_CTRL_AUTOTRI_DEFAULT           0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_AUTOTRI_DEFAULT            (_UART_CTRL_AUTOTRI_DEFAULT << 17)      /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_SCMODE                     (0x1UL << 18)                           /**< SmartCard Mode */
#define _UART_CTRL_SCMODE_SHIFT              18                                      /**< Shift value for USART_SCMODE */
#define _UART_CTRL_SCMODE_MASK               0x40000UL                               /**< Bit mask for USART_SCMODE */
#define _UART_CTRL_SCMODE_DEFAULT            0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_SCMODE_DEFAULT             (_UART_CTRL_SCMODE_DEFAULT << 18)       /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_SCRETRANS                  (0x1UL << 19)                           /**< SmartCard Retransmit */
#define _UART_CTRL_SCRETRANS_SHIFT           19                                      /**< Shift value for USART_SCRETRANS */
#define _UART_CTRL_SCRETRANS_MASK            0x80000UL                               /**< Bit mask for USART_SCRETRANS */
#define _UART_CTRL_SCRETRANS_DEFAULT         0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_SCRETRANS_DEFAULT          (_UART_CTRL_SCRETRANS_DEFAULT << 19)    /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_SKIPPERRF                  (0x1UL << 20)                           /**< Skip Parity Error Frames */
#define _UART_CTRL_SKIPPERRF_SHIFT           20                                      /**< Shift value for USART_SKIPPERRF */
#define _UART_CTRL_SKIPPERRF_MASK            0x100000UL                              /**< Bit mask for USART_SKIPPERRF */
#define _UART_CTRL_SKIPPERRF_DEFAULT         0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_SKIPPERRF_DEFAULT          (_UART_CTRL_SKIPPERRF_DEFAULT << 20)    /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_BIT8DV                     (0x1UL << 21)                           /**< Bit 8 Default Value */
#define _UART_CTRL_BIT8DV_SHIFT              21                                      /**< Shift value for USART_BIT8DV */
#define _UART_CTRL_BIT8DV_MASK               0x200000UL                              /**< Bit mask for USART_BIT8DV */
#define _UART_CTRL_BIT8DV_DEFAULT            0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_BIT8DV_DEFAULT             (_UART_CTRL_BIT8DV_DEFAULT << 21)       /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_ERRSDMA                    (0x1UL << 22)                           /**< Halt DMA On Error */
#define _UART_CTRL_ERRSDMA_SHIFT             22                                      /**< Shift value for USART_ERRSDMA */
#define _UART_CTRL_ERRSDMA_MASK              0x400000UL                              /**< Bit mask for USART_ERRSDMA */
#define _UART_CTRL_ERRSDMA_DEFAULT           0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_ERRSDMA_DEFAULT            (_UART_CTRL_ERRSDMA_DEFAULT << 22)      /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_ERRSRX                     (0x1UL << 23)                           /**< Disable RX On Error */
#define _UART_CTRL_ERRSRX_SHIFT              23                                      /**< Shift value for USART_ERRSRX */
#define _UART_CTRL_ERRSRX_MASK               0x800000UL                              /**< Bit mask for USART_ERRSRX */
#define _UART_CTRL_ERRSRX_DEFAULT            0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_ERRSRX_DEFAULT             (_UART_CTRL_ERRSRX_DEFAULT << 23)       /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_ERRSTX                     (0x1UL << 24)                           /**< Disable TX On Error */
#define _UART_CTRL_ERRSTX_SHIFT              24                                      /**< Shift value for USART_ERRSTX */
#define _UART_CTRL_ERRSTX_MASK               0x1000000UL                             /**< Bit mask for USART_ERRSTX */
#define _UART_CTRL_ERRSTX_DEFAULT            0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_ERRSTX_DEFAULT             (_UART_CTRL_ERRSTX_DEFAULT << 24)       /**< Shifted mode DEFAULT for UART_CTRL */
#define _UART_CTRL_TXDELAY_SHIFT             26                                      /**< Shift value for USART_TXDELAY */
#define _UART_CTRL_TXDELAY_MASK              0xC000000UL                             /**< Bit mask for USART_TXDELAY */
#define _UART_CTRL_TXDELAY_DEFAULT           0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define _UART_CTRL_TXDELAY_NONE              0x00000000UL                            /**< Mode NONE for UART_CTRL */
#define _UART_CTRL_TXDELAY_SINGLE            0x00000001UL                            /**< Mode SINGLE for UART_CTRL */
#define _UART_CTRL_TXDELAY_DOUBLE            0x00000002UL                            /**< Mode DOUBLE for UART_CTRL */
#define _UART_CTRL_TXDELAY_TRIPLE            0x00000003UL                            /**< Mode TRIPLE for UART_CTRL */
#define UART_CTRL_TXDELAY_DEFAULT            (_UART_CTRL_TXDELAY_DEFAULT << 26)      /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_TXDELAY_NONE               (_UART_CTRL_TXDELAY_NONE << 26)         /**< Shifted mode NONE for UART_CTRL */
#define UART_CTRL_TXDELAY_SINGLE             (_UART_CTRL_TXDELAY_SINGLE << 26)       /**< Shifted mode SINGLE for UART_CTRL */
#define UART_CTRL_TXDELAY_DOUBLE             (_UART_CTRL_TXDELAY_DOUBLE << 26)       /**< Shifted mode DOUBLE for UART_CTRL */
#define UART_CTRL_TXDELAY_TRIPLE             (_UART_CTRL_TXDELAY_TRIPLE << 26)       /**< Shifted mode TRIPLE for UART_CTRL */
#define UART_CTRL_BYTESWAP                   (0x1UL << 28)                           /**< Byteswap In Double Accesses */
#define _UART_CTRL_BYTESWAP_SHIFT            28                                      /**< Shift value for USART_BYTESWAP */
#define _UART_CTRL_BYTESWAP_MASK             0x10000000UL                            /**< Bit mask for USART_BYTESWAP */
#define _UART_CTRL_BYTESWAP_DEFAULT          0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_BYTESWAP_DEFAULT           (_UART_CTRL_BYTESWAP_DEFAULT << 28)     /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_AUTOTX                     (0x1UL << 29)                           /**< Always Transmit When RX Not Full */
#define _UART_CTRL_AUTOTX_SHIFT              29                                      /**< Shift value for USART_AUTOTX */
#define _UART_CTRL_AUTOTX_MASK               0x20000000UL                            /**< Bit mask for USART_AUTOTX */
#define _UART_CTRL_AUTOTX_DEFAULT            0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_AUTOTX_DEFAULT             (_UART_CTRL_AUTOTX_DEFAULT << 29)       /**< Shifted mode DEFAULT for UART_CTRL */
#define UART_CTRL_MVDIS                      (0x1UL << 30)                           /**< Majority Vote Disable */
#define _UART_CTRL_MVDIS_SHIFT               30                                      /**< Shift value for USART_MVDIS */
#define _UART_CTRL_MVDIS_MASK                0x40000000UL                            /**< Bit mask for USART_MVDIS */
#define _UART_CTRL_MVDIS_DEFAULT             0x00000000UL                            /**< Mode DEFAULT for UART_CTRL */
#define UART_CTRL_MVDIS_DEFAULT              (_UART_CTRL_MVDIS_DEFAULT << 30)        /**< Shifted mode DEFAULT for UART_CTRL */

/* Bit fields for UART FRAME */
#define _UART_FRAME_RESETVALUE               0x00001005UL                             /**< Default value for UART_FRAME */
#define _UART_FRAME_MASK                     0x0000330FUL                             /**< Mask for UART_FRAME */
#define _UART_FRAME_DATABITS_SHIFT           0                                        /**< Shift value for USART_DATABITS */
#define _UART_FRAME_DATABITS_MASK            0xFUL                                    /**< Bit mask for USART_DATABITS */
#define _UART_FRAME_DATABITS_FOUR            0x00000001UL                             /**< Mode FOUR for UART_FRAME */
#define _UART_FRAME_DATABITS_FIVE            0x00000002UL                             /**< Mode FIVE for UART_FRAME */
#define _UART_FRAME_DATABITS_SIX             0x00000003UL                             /**< Mode SIX for UART_FRAME */
#define _UART_FRAME_DATABITS_SEVEN           0x00000004UL                             /**< Mode SEVEN for UART_FRAME */
#define _UART_FRAME_DATABITS_DEFAULT         0x00000005UL                             /**< Mode DEFAULT for UART_FRAME */
#define _UART_FRAME_DATABITS_EIGHT           0x00000005UL                             /**< Mode EIGHT for UART_FRAME */
#define _UART_FRAME_DATABITS_NINE            0x00000006UL                             /**< Mode NINE for UART_FRAME */
#define _UART_FRAME_DATABITS_TEN             0x00000007UL                             /**< Mode TEN for UART_FRAME */
#define _UART_FRAME_DATABITS_ELEVEN          0x00000008UL                             /**< Mode ELEVEN for UART_FRAME */
#define _UART_FRAME_DATABITS_TWELVE          0x00000009UL                             /**< Mode TWELVE for UART_FRAME */
#define _UART_FRAME_DATABITS_THIRTEEN        0x0000000AUL                             /**< Mode THIRTEEN for UART_FRAME */
#define _UART_FRAME_DATABITS_FOURTEEN        0x0000000BUL                             /**< Mode FOURTEEN for UART_FRAME */
#define _UART_FRAME_DATABITS_FIFTEEN         0x0000000CUL                             /**< Mode FIFTEEN for UART_FRAME */
#define _UART_FRAME_DATABITS_SIXTEEN         0x0000000DUL                             /**< Mode SIXTEEN for UART_FRAME */
#define UART_FRAME_DATABITS_FOUR             (_UART_FRAME_DATABITS_FOUR << 0)         /**< Shifted mode FOUR for UART_FRAME */
#define UART_FRAME_DATABITS_FIVE             (_UART_FRAME_DATABITS_FIVE << 0)         /**< Shifted mode FIVE for UART_FRAME */
#define UART_FRAME_DATABITS_SIX              (_UART_FRAME_DATABITS_SIX << 0)          /**< Shifted mode SIX for UART_FRAME */
#define UART_FRAME_DATABITS_SEVEN            (_UART_FRAME_DATABITS_SEVEN << 0)        /**< Shifted mode SEVEN for UART_FRAME */
#define UART_FRAME_DATABITS_DEFAULT          (_UART_FRAME_DATABITS_DEFAULT << 0)      /**< Shifted mode DEFAULT for UART_FRAME */
#define UART_FRAME_DATABITS_EIGHT            (_UART_FRAME_DATABITS_EIGHT << 0)        /**< Shifted mode EIGHT for UART_FRAME */
#define UART_FRAME_DATABITS_NINE             (_UART_FRAME_DATABITS_NINE << 0)         /**< Shifted mode NINE for UART_FRAME */
#define UART_FRAME_DATABITS_TEN              (_UART_FRAME_DATABITS_TEN << 0)          /**< Shifted mode TEN for UART_FRAME */
#define UART_FRAME_DATABITS_ELEVEN           (_UART_FRAME_DATABITS_ELEVEN << 0)       /**< Shifted mode ELEVEN for UART_FRAME */
#define UART_FRAME_DATABITS_TWELVE           (_UART_FRAME_DATABITS_TWELVE << 0)       /**< Shifted mode TWELVE for UART_FRAME */
#define UART_FRAME_DATABITS_THIRTEEN         (_UART_FRAME_DATABITS_THIRTEEN << 0)     /**< Shifted mode THIRTEEN for UART_FRAME */
#define UART_FRAME_DATABITS_FOURTEEN         (_UART_FRAME_DATABITS_FOURTEEN << 0)     /**< Shifted mode FOURTEEN for UART_FRAME */
#define UART_FRAME_DATABITS_FIFTEEN          (_UART_FRAME_DATABITS_FIFTEEN << 0)      /**< Shifted mode FIFTEEN for UART_FRAME */
#define UART_FRAME_DATABITS_SIXTEEN          (_UART_FRAME_DATABITS_SIXTEEN << 0)      /**< Shifted mode SIXTEEN for UART_FRAME */
#define _UART_FRAME_PARITY_SHIFT             8                                        /**< Shift value for USART_PARITY */
#define _UART_FRAME_PARITY_MASK              0x300UL                                  /**< Bit mask for USART_PARITY */
#define _UART_FRAME_PARITY_DEFAULT           0x00000000UL                             /**< Mode DEFAULT for UART_FRAME */
#define _UART_FRAME_PARITY_NONE              0x00000000UL                             /**< Mode NONE for UART_FRAME */
#define _UART_FRAME_PARITY_EVEN              0x00000002UL                             /**< Mode EVEN for UART_FRAME */
#define _UART_FRAME_PARITY_ODD               0x00000003UL                             /**< Mode ODD for UART_FRAME */
#define UART_FRAME_PARITY_DEFAULT            (_UART_FRAME_PARITY_DEFAULT << 8)        /**< Shifted mode DEFAULT for UART_FRAME */
#define UART_FRAME_PARITY_NONE               (_UART_FRAME_PARITY_NONE << 8)           /**< Shifted mode NONE for UART_FRAME */
#define UART_FRAME_PARITY_EVEN               (_UART_FRAME_PARITY_EVEN << 8)           /**< Shifted mode EVEN for UART_FRAME */
#define UART_FRAME_PARITY_ODD                (_UART_FRAME_PARITY_ODD << 8)            /**< Shifted mode ODD for UART_FRAME */
#define _UART_FRAME_STOPBITS_SHIFT           12                                       /**< Shift value for USART_STOPBITS */
#define _UART_FRAME_STOPBITS_MASK            0x3000UL                                 /**< Bit mask for USART_STOPBITS */
#define _UART_FRAME_STOPBITS_HALF            0x00000000UL                             /**< Mode HALF for UART_FRAME */
#define _UART_FRAME_STOPBITS_DEFAULT         0x00000001UL                             /**< Mode DEFAULT for UART_FRAME */
#define _UART_FRAME_STOPBITS_ONE             0x00000001UL                             /**< Mode ONE for UART_FRAME */
#define _UART_FRAME_STOPBITS_ONEANDAHALF     0x00000002UL                             /**< Mode ONEANDAHALF for UART_FRAME */
#define _UART_FRAME_STOPBITS_TWO             0x00000003UL                             /**< Mode TWO for UART_FRAME */
#define UART_FRAME_STOPBITS_HALF             (_UART_FRAME_STOPBITS_HALF << 12)        /**< Shifted mode HALF for UART_FRAME */
#define UART_FRAME_STOPBITS_DEFAULT          (_UART_FRAME_STOPBITS_DEFAULT << 12)     /**< Shifted mode DEFAULT for UART_FRAME */
#define UART_FRAME_STOPBITS_ONE              (_UART_FRAME_STOPBITS_ONE << 12)         /**< Shifted mode ONE for UART_FRAME */
#define UART_FRAME_STOPBITS_ONEANDAHALF      (_UART_FRAME_STOPBITS_ONEANDAHALF << 12) /**< Shifted mode ONEANDAHALF for UART_FRAME */
#define UART_FRAME_STOPBITS_TWO              (_UART_FRAME_STOPBITS_TWO << 12)         /**< Shifted mode TWO for UART_FRAME */

/* Bit fields for UART TRIGCTRL */
#define _UART_TRIGCTRL_RESETVALUE            0x00000000UL                            /**< Default value for UART_TRIGCTRL */
#define _UART_TRIGCTRL_MASK                  0x00000077UL                            /**< Mask for UART_TRIGCTRL */
#define _UART_TRIGCTRL_TSEL_SHIFT            0                                       /**< Shift value for USART_TSEL */
#define _UART_TRIGCTRL_TSEL_MASK             0x7UL                                   /**< Bit mask for USART_TSEL */
#define _UART_TRIGCTRL_TSEL_DEFAULT          0x00000000UL                            /**< Mode DEFAULT for UART_TRIGCTRL */
#define _UART_TRIGCTRL_TSEL_PRSCH0           0x00000000UL                            /**< Mode PRSCH0 for UART_TRIGCTRL */
#define _UART_TRIGCTRL_TSEL_PRSCH1           0x00000001UL                            /**< Mode PRSCH1 for UART_TRIGCTRL */
#define _UART_TRIGCTRL_TSEL_PRSCH2           0x00000002UL                            /**< Mode PRSCH2 for UART_TRIGCTRL */
#define _UART_TRIGCTRL_TSEL_PRSCH3           0x00000003UL                            /**< Mode PRSCH3 for UART_TRIGCTRL */
#define _UART_TRIGCTRL_TSEL_PRSCH4           0x00000004UL                            /**< Mode PRSCH4 for UART_TRIGCTRL */
#define _UART_TRIGCTRL_TSEL_PRSCH5           0x00000005UL                            /**< Mode PRSCH5 for UART_TRIGCTRL */
#define _UART_TRIGCTRL_TSEL_PRSCH6           0x00000006UL                            /**< Mode PRSCH6 for UART_TRIGCTRL */
#define _UART_TRIGCTRL_TSEL_PRSCH7           0x00000007UL                            /**< Mode PRSCH7 for UART_TRIGCTRL */
#define UART_TRIGCTRL_TSEL_DEFAULT           (_UART_TRIGCTRL_TSEL_DEFAULT << 0)      /**< Shifted mode DEFAULT for UART_TRIGCTRL */
#define UART_TRIGCTRL_TSEL_PRSCH0            (_UART_TRIGCTRL_TSEL_PRSCH0 << 0)       /**< Shifted mode PRSCH0 for UART_TRIGCTRL */
#define UART_TRIGCTRL_TSEL_PRSCH1            (_UART_TRIGCTRL_TSEL_PRSCH1 << 0)       /**< Shifted mode PRSCH1 for UART_TRIGCTRL */
#define UART_TRIGCTRL_TSEL_PRSCH2            (_UART_TRIGCTRL_TSEL_PRSCH2 << 0)       /**< Shifted mode PRSCH2 for UART_TRIGCTRL */
#define UART_TRIGCTRL_TSEL_PRSCH3            (_UART_TRIGCTRL_TSEL_PRSCH3 << 0)       /**< Shifted mode PRSCH3 for UART_TRIGCTRL */
#define UART_TRIGCTRL_TSEL_PRSCH4            (_UART_TRIGCTRL_TSEL_PRSCH4 << 0)       /**< Shifted mode PRSCH4 for UART_TRIGCTRL */
#define UART_TRIGCTRL_TSEL_PRSCH5            (_UART_TRIGCTRL_TSEL_PRSCH5 << 0)       /**< Shifted mode PRSCH5 for UART_TRIGCTRL */
#define UART_TRIGCTRL_TSEL_PRSCH6            (_UART_TRIGCTRL_TSEL_PRSCH6 << 0)       /**< Shifted mode PRSCH6 for UART_TRIGCTRL */
#define UART_TRIGCTRL_TSEL_PRSCH7            (_UART_TRIGCTRL_TSEL_PRSCH7 << 0)       /**< Shifted mode PRSCH7 for UART_TRIGCTRL */
#define UART_TRIGCTRL_RXTEN                  (0x1UL << 4)                            /**< Receive Trigger Enable */
#define _UART_TRIGCTRL_RXTEN_SHIFT           4                                       /**< Shift value for USART_RXTEN */
#define _UART_TRIGCTRL_RXTEN_MASK            0x10UL                                  /**< Bit mask for USART_RXTEN */
#define _UART_TRIGCTRL_RXTEN_DEFAULT         0x00000000UL                            /**< Mode DEFAULT for UART_TRIGCTRL */
#define UART_TRIGCTRL_RXTEN_DEFAULT          (_UART_TRIGCTRL_RXTEN_DEFAULT << 4)     /**< Shifted mode DEFAULT for UART_TRIGCTRL */
#define UART_TRIGCTRL_TXTEN                  (0x1UL << 5)                            /**< Transmit Trigger Enable */
#define _UART_TRIGCTRL_TXTEN_SHIFT           5                                       /**< Shift value for USART_TXTEN */
#define _UART_TRIGCTRL_TXTEN_MASK            0x20UL                                  /**< Bit mask for USART_TXTEN */
#define _UART_TRIGCTRL_TXTEN_DEFAULT         0x00000000UL                            /**< Mode DEFAULT for UART_TRIGCTRL */
#define UART_TRIGCTRL_TXTEN_DEFAULT          (_UART_TRIGCTRL_TXTEN_DEFAULT << 5)     /**< Shifted mode DEFAULT for UART_TRIGCTRL */
#define UART_TRIGCTRL_AUTOTXTEN              (0x1UL << 6)                            /**< AUTOTX Trigger Enable */
#define _UART_TRIGCTRL_AUTOTXTEN_SHIFT       6                                       /**< Shift value for USART_AUTOTXTEN */
#define _UART_TRIGCTRL_AUTOTXTEN_MASK        0x40UL                                  /**< Bit mask for USART_AUTOTXTEN */
#define _UART_TRIGCTRL_AUTOTXTEN_DEFAULT     0x00000000UL                            /**< Mode DEFAULT for UART_TRIGCTRL */
#define UART_TRIGCTRL_AUTOTXTEN_DEFAULT      (_UART_TRIGCTRL_AUTOTXTEN_DEFAULT << 6) /**< Shifted mode DEFAULT for UART_TRIGCTRL */

/* Bit fields for UART CMD */
#define _UART_CMD_RESETVALUE                 0x00000000UL                        /**< Default value for UART_CMD */
#define _UART_CMD_MASK                       0x00000FFFUL                        /**< Mask for UART_CMD */
#define UART_CMD_RXEN                        (0x1UL << 0)                        /**< Receiver Enable */
#define _UART_CMD_RXEN_SHIFT                 0                                   /**< Shift value for USART_RXEN */
#define _UART_CMD_RXEN_MASK                  0x1UL                               /**< Bit mask for USART_RXEN */
#define _UART_CMD_RXEN_DEFAULT               0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_RXEN_DEFAULT                (_UART_CMD_RXEN_DEFAULT << 0)       /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_RXDIS                       (0x1UL << 1)                        /**< Receiver Disable */
#define _UART_CMD_RXDIS_SHIFT                1                                   /**< Shift value for USART_RXDIS */
#define _UART_CMD_RXDIS_MASK                 0x2UL                               /**< Bit mask for USART_RXDIS */
#define _UART_CMD_RXDIS_DEFAULT              0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_RXDIS_DEFAULT               (_UART_CMD_RXDIS_DEFAULT << 1)      /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_TXEN                        (0x1UL << 2)                        /**< Transmitter Enable */
#define _UART_CMD_TXEN_SHIFT                 2                                   /**< Shift value for USART_TXEN */
#define _UART_CMD_TXEN_MASK                  0x4UL                               /**< Bit mask for USART_TXEN */
#define _UART_CMD_TXEN_DEFAULT               0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_TXEN_DEFAULT                (_UART_CMD_TXEN_DEFAULT << 2)       /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_TXDIS                       (0x1UL << 3)                        /**< Transmitter Disable */
#define _UART_CMD_TXDIS_SHIFT                3                                   /**< Shift value for USART_TXDIS */
#define _UART_CMD_TXDIS_MASK                 0x8UL                               /**< Bit mask for USART_TXDIS */
#define _UART_CMD_TXDIS_DEFAULT              0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_TXDIS_DEFAULT               (_UART_CMD_TXDIS_DEFAULT << 3)      /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_MASTEREN                    (0x1UL << 4)                        /**< Master Enable */
#define _UART_CMD_MASTEREN_SHIFT             4                                   /**< Shift value for USART_MASTEREN */
#define _UART_CMD_MASTEREN_MASK              0x10UL                              /**< Bit mask for USART_MASTEREN */
#define _UART_CMD_MASTEREN_DEFAULT           0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_MASTEREN_DEFAULT            (_UART_CMD_MASTEREN_DEFAULT << 4)   /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_MASTERDIS                   (0x1UL << 5)                        /**< Master Disable */
#define _UART_CMD_MASTERDIS_SHIFT            5                                   /**< Shift value for USART_MASTERDIS */
#define _UART_CMD_MASTERDIS_MASK             0x20UL                              /**< Bit mask for USART_MASTERDIS */
#define _UART_CMD_MASTERDIS_DEFAULT          0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_MASTERDIS_DEFAULT           (_UART_CMD_MASTERDIS_DEFAULT << 5)  /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_RXBLOCKEN                   (0x1UL << 6)                        /**< Receiver Block Enable */
#define _UART_CMD_RXBLOCKEN_SHIFT            6                                   /**< Shift value for USART_RXBLOCKEN */
#define _UART_CMD_RXBLOCKEN_MASK             0x40UL                              /**< Bit mask for USART_RXBLOCKEN */
#define _UART_CMD_RXBLOCKEN_DEFAULT          0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_RXBLOCKEN_DEFAULT           (_UART_CMD_RXBLOCKEN_DEFAULT << 6)  /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_RXBLOCKDIS                  (0x1UL << 7)                        /**< Receiver Block Disable */
#define _UART_CMD_RXBLOCKDIS_SHIFT           7                                   /**< Shift value for USART_RXBLOCKDIS */
#define _UART_CMD_RXBLOCKDIS_MASK            0x80UL                              /**< Bit mask for USART_RXBLOCKDIS */
#define _UART_CMD_RXBLOCKDIS_DEFAULT         0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_RXBLOCKDIS_DEFAULT          (_UART_CMD_RXBLOCKDIS_DEFAULT << 7) /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_TXTRIEN                     (0x1UL << 8)                        /**< Transmitter Tristate Enable */
#define _UART_CMD_TXTRIEN_SHIFT              8                                   /**< Shift value for USART_TXTRIEN */
#define _UART_CMD_TXTRIEN_MASK               0x100UL                             /**< Bit mask for USART_TXTRIEN */
#define _UART_CMD_TXTRIEN_DEFAULT            0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_TXTRIEN_DEFAULT             (_UART_CMD_TXTRIEN_DEFAULT << 8)    /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_TXTRIDIS                    (0x1UL << 9)                        /**< Transmitter Tristate Disable */
#define _UART_CMD_TXTRIDIS_SHIFT             9                                   /**< Shift value for USART_TXTRIDIS */
#define _UART_CMD_TXTRIDIS_MASK              0x200UL                             /**< Bit mask for USART_TXTRIDIS */
#define _UART_CMD_TXTRIDIS_DEFAULT           0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_TXTRIDIS_DEFAULT            (_UART_CMD_TXTRIDIS_DEFAULT << 9)   /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_CLEARTX                     (0x1UL << 10)                       /**< Clear TX */
#define _UART_CMD_CLEARTX_SHIFT              10                                  /**< Shift value for USART_CLEARTX */
#define _UART_CMD_CLEARTX_MASK               0x400UL                             /**< Bit mask for USART_CLEARTX */
#define _UART_CMD_CLEARTX_DEFAULT            0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_CLEARTX_DEFAULT             (_UART_CMD_CLEARTX_DEFAULT << 10)   /**< Shifted mode DEFAULT for UART_CMD */
#define UART_CMD_CLEARRX                     (0x1UL << 11)                       /**< Clear RX */
#define _UART_CMD_CLEARRX_SHIFT              11                                  /**< Shift value for USART_CLEARRX */
#define _UART_CMD_CLEARRX_MASK               0x800UL                             /**< Bit mask for USART_CLEARRX */
#define _UART_CMD_CLEARRX_DEFAULT            0x00000000UL                        /**< Mode DEFAULT for UART_CMD */
#define UART_CMD_CLEARRX_DEFAULT             (_UART_CMD_CLEARRX_DEFAULT << 11)   /**< Shifted mode DEFAULT for UART_CMD */

/* Bit fields for UART STATUS */
#define _UART_STATUS_RESETVALUE              0x00000040UL                              /**< Default value for UART_STATUS */
#define _UART_STATUS_MASK                    0x00001FFFUL                              /**< Mask for UART_STATUS */
#define UART_STATUS_RXENS                    (0x1UL << 0)                              /**< Receiver Enable Status */
#define _UART_STATUS_RXENS_SHIFT             0                                         /**< Shift value for USART_RXENS */
#define _UART_STATUS_RXENS_MASK              0x1UL                                     /**< Bit mask for USART_RXENS */
#define _UART_STATUS_RXENS_DEFAULT           0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXENS_DEFAULT            (_UART_STATUS_RXENS_DEFAULT << 0)         /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXENS                    (0x1UL << 1)                              /**< Transmitter Enable Status */
#define _UART_STATUS_TXENS_SHIFT             1                                         /**< Shift value for USART_TXENS */
#define _UART_STATUS_TXENS_MASK              0x2UL                                     /**< Bit mask for USART_TXENS */
#define _UART_STATUS_TXENS_DEFAULT           0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXENS_DEFAULT            (_UART_STATUS_TXENS_DEFAULT << 1)         /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_MASTER                   (0x1UL << 2)                              /**< SPI Master Mode */
#define _UART_STATUS_MASTER_SHIFT            2                                         /**< Shift value for USART_MASTER */
#define _UART_STATUS_MASTER_MASK             0x4UL                                     /**< Bit mask for USART_MASTER */
#define _UART_STATUS_MASTER_DEFAULT          0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_MASTER_DEFAULT           (_UART_STATUS_MASTER_DEFAULT << 2)        /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXBLOCK                  (0x1UL << 3)                              /**< Block Incoming Data */
#define _UART_STATUS_RXBLOCK_SHIFT           3                                         /**< Shift value for USART_RXBLOCK */
#define _UART_STATUS_RXBLOCK_MASK            0x8UL                                     /**< Bit mask for USART_RXBLOCK */
#define _UART_STATUS_RXBLOCK_DEFAULT         0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXBLOCK_DEFAULT          (_UART_STATUS_RXBLOCK_DEFAULT << 3)       /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXTRI                    (0x1UL << 4)                              /**< Transmitter Tristated */
#define _UART_STATUS_TXTRI_SHIFT             4                                         /**< Shift value for USART_TXTRI */
#define _UART_STATUS_TXTRI_MASK              0x10UL                                    /**< Bit mask for USART_TXTRI */
#define _UART_STATUS_TXTRI_DEFAULT           0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXTRI_DEFAULT            (_UART_STATUS_TXTRI_DEFAULT << 4)         /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXC                      (0x1UL << 5)                              /**< TX Complete */
#define _UART_STATUS_TXC_SHIFT               5                                         /**< Shift value for USART_TXC */
#define _UART_STATUS_TXC_MASK                0x20UL                                    /**< Bit mask for USART_TXC */
#define _UART_STATUS_TXC_DEFAULT             0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXC_DEFAULT              (_UART_STATUS_TXC_DEFAULT << 5)           /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXBL                     (0x1UL << 6)                              /**< TX Buffer Level */
#define _UART_STATUS_TXBL_SHIFT              6                                         /**< Shift value for USART_TXBL */
#define _UART_STATUS_TXBL_MASK               0x40UL                                    /**< Bit mask for USART_TXBL */
#define _UART_STATUS_TXBL_DEFAULT            0x00000001UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXBL_DEFAULT             (_UART_STATUS_TXBL_DEFAULT << 6)          /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXDATAV                  (0x1UL << 7)                              /**< RX Data Valid */
#define _UART_STATUS_RXDATAV_SHIFT           7                                         /**< Shift value for USART_RXDATAV */
#define _UART_STATUS_RXDATAV_MASK            0x80UL                                    /**< Bit mask for USART_RXDATAV */
#define _UART_STATUS_RXDATAV_DEFAULT         0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXDATAV_DEFAULT          (_UART_STATUS_RXDATAV_DEFAULT << 7)       /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXFULL                   (0x1UL << 8)                              /**< RX FIFO Full */
#define _UART_STATUS_RXFULL_SHIFT            8                                         /**< Shift value for USART_RXFULL */
#define _UART_STATUS_RXFULL_MASK             0x100UL                                   /**< Bit mask for USART_RXFULL */
#define _UART_STATUS_RXFULL_DEFAULT          0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXFULL_DEFAULT           (_UART_STATUS_RXFULL_DEFAULT << 8)        /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXBDRIGHT                (0x1UL << 9)                              /**< TX Buffer Expects Double Right Data */
#define _UART_STATUS_TXBDRIGHT_SHIFT         9                                         /**< Shift value for USART_TXBDRIGHT */
#define _UART_STATUS_TXBDRIGHT_MASK          0x200UL                                   /**< Bit mask for USART_TXBDRIGHT */
#define _UART_STATUS_TXBDRIGHT_DEFAULT       0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXBDRIGHT_DEFAULT        (_UART_STATUS_TXBDRIGHT_DEFAULT << 9)     /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXBSRIGHT                (0x1UL << 10)                             /**< TX Buffer Expects Single Right Data */
#define _UART_STATUS_TXBSRIGHT_SHIFT         10                                        /**< Shift value for USART_TXBSRIGHT */
#define _UART_STATUS_TXBSRIGHT_MASK          0x400UL                                   /**< Bit mask for USART_TXBSRIGHT */
#define _UART_STATUS_TXBSRIGHT_DEFAULT       0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_TXBSRIGHT_DEFAULT        (_UART_STATUS_TXBSRIGHT_DEFAULT << 10)    /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXDATAVRIGHT             (0x1UL << 11)                             /**< RX Data Right */
#define _UART_STATUS_RXDATAVRIGHT_SHIFT      11                                        /**< Shift value for USART_RXDATAVRIGHT */
#define _UART_STATUS_RXDATAVRIGHT_MASK       0x800UL                                   /**< Bit mask for USART_RXDATAVRIGHT */
#define _UART_STATUS_RXDATAVRIGHT_DEFAULT    0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXDATAVRIGHT_DEFAULT     (_UART_STATUS_RXDATAVRIGHT_DEFAULT << 11) /**< Shifted mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXFULLRIGHT              (0x1UL << 12)                             /**< RX Full of Right Data */
#define _UART_STATUS_RXFULLRIGHT_SHIFT       12                                        /**< Shift value for USART_RXFULLRIGHT */
#define _UART_STATUS_RXFULLRIGHT_MASK        0x1000UL                                  /**< Bit mask for USART_RXFULLRIGHT */
#define _UART_STATUS_RXFULLRIGHT_DEFAULT     0x00000000UL                              /**< Mode DEFAULT for UART_STATUS */
#define UART_STATUS_RXFULLRIGHT_DEFAULT      (_UART_STATUS_RXFULLRIGHT_DEFAULT << 12)  /**< Shifted mode DEFAULT for UART_STATUS */

/* Bit fields for UART CLKDIV */
#define _UART_CLKDIV_RESETVALUE              0x00000000UL                    /**< Default value for UART_CLKDIV */
#define _UART_CLKDIV_MASK                    0x001FFFC0UL                    /**< Mask for UART_CLKDIV */
#define _UART_CLKDIV_DIV_SHIFT               6                               /**< Shift value for USART_DIV */
#define _UART_CLKDIV_DIV_MASK                0x1FFFC0UL                      /**< Bit mask for USART_DIV */
#define _UART_CLKDIV_DIV_DEFAULT             0x00000000UL                    /**< Mode DEFAULT for UART_CLKDIV */
#define UART_CLKDIV_DIV_DEFAULT              (_UART_CLKDIV_DIV_DEFAULT << 6) /**< Shifted mode DEFAULT for UART_CLKDIV */

/* Bit fields for UART RXDATAX */
#define _UART_RXDATAX_RESETVALUE             0x00000000UL                        /**< Default value for UART_RXDATAX */
#define _UART_RXDATAX_MASK                   0x0000C1FFUL                        /**< Mask for UART_RXDATAX */
#define _UART_RXDATAX_RXDATA_SHIFT           0                                   /**< Shift value for USART_RXDATA */
#define _UART_RXDATAX_RXDATA_MASK            0x1FFUL                             /**< Bit mask for USART_RXDATA */
#define _UART_RXDATAX_RXDATA_DEFAULT         0x00000000UL                        /**< Mode DEFAULT for UART_RXDATAX */
#define UART_RXDATAX_RXDATA_DEFAULT          (_UART_RXDATAX_RXDATA_DEFAULT << 0) /**< Shifted mode DEFAULT for UART_RXDATAX */
#define UART_RXDATAX_PERR                    (0x1UL << 14)                       /**< Data Parity Error */
#define _UART_RXDATAX_PERR_SHIFT             14                                  /**< Shift value for USART_PERR */
#define _UART_RXDATAX_PERR_MASK              0x4000UL                            /**< Bit mask for USART_PERR */
#define _UART_RXDATAX_PERR_DEFAULT           0x00000000UL                        /**< Mode DEFAULT for UART_RXDATAX */
#define UART_RXDATAX_PERR_DEFAULT            (_UART_RXDATAX_PERR_DEFAULT << 14)  /**< Shifted mode DEFAULT for UART_RXDATAX */
#define UART_RXDATAX_FERR                    (0x1UL << 15)                       /**< Data Framing Error */
#define _UART_RXDATAX_FERR_SHIFT             15                                  /**< Shift value for USART_FERR */
#define _UART_RXDATAX_FERR_MASK              0x8000UL                            /**< Bit mask for USART_FERR */
#define _UART_RXDATAX_FERR_DEFAULT           0x00000000UL                        /**< Mode DEFAULT for UART_RXDATAX */
#define UART_RXDATAX_FERR_DEFAULT            (_UART_RXDATAX_FERR_DEFAULT << 15)  /**< Shifted mode DEFAULT for UART_RXDATAX */

/* Bit fields for UART RXDATA */
#define _UART_RXDATA_RESETVALUE              0x00000000UL                       /**< Default value for UART_RXDATA */
#define _UART_RXDATA_MASK                    0x000000FFUL                       /**< Mask for UART_RXDATA */
#define _UART_RXDATA_RXDATA_SHIFT            0                                  /**< Shift value for USART_RXDATA */
#define _UART_RXDATA_RXDATA_MASK             0xFFUL                             /**< Bit mask for USART_RXDATA */
#define _UART_RXDATA_RXDATA_DEFAULT          0x00000000UL                       /**< Mode DEFAULT for UART_RXDATA */
#define UART_RXDATA_RXDATA_DEFAULT           (_UART_RXDATA_RXDATA_DEFAULT << 0) /**< Shifted mode DEFAULT for UART_RXDATA */

/* Bit fields for UART RXDOUBLEX */
#define _UART_RXDOUBLEX_RESETVALUE           0x00000000UL                            /**< Default value for UART_RXDOUBLEX */
#define _UART_RXDOUBLEX_MASK                 0xC1FFC1FFUL                            /**< Mask for UART_RXDOUBLEX */
#define _UART_RXDOUBLEX_RXDATA0_SHIFT        0                                       /**< Shift value for USART_RXDATA0 */
#define _UART_RXDOUBLEX_RXDATA0_MASK         0x1FFUL                                 /**< Bit mask for USART_RXDATA0 */
#define _UART_RXDOUBLEX_RXDATA0_DEFAULT      0x00000000UL                            /**< Mode DEFAULT for UART_RXDOUBLEX */
#define UART_RXDOUBLEX_RXDATA0_DEFAULT       (_UART_RXDOUBLEX_RXDATA0_DEFAULT << 0)  /**< Shifted mode DEFAULT for UART_RXDOUBLEX */
#define UART_RXDOUBLEX_PERR0                 (0x1UL << 14)                           /**< Data Parity Error 0 */
#define _UART_RXDOUBLEX_PERR0_SHIFT          14                                      /**< Shift value for USART_PERR0 */
#define _UART_RXDOUBLEX_PERR0_MASK           0x4000UL                                /**< Bit mask for USART_PERR0 */
#define _UART_RXDOUBLEX_PERR0_DEFAULT        0x00000000UL                            /**< Mode DEFAULT for UART_RXDOUBLEX */
#define UART_RXDOUBLEX_PERR0_DEFAULT         (_UART_RXDOUBLEX_PERR0_DEFAULT << 14)   /**< Shifted mode DEFAULT for UART_RXDOUBLEX */
#define UART_RXDOUBLEX_FERR0                 (0x1UL << 15)                           /**< Data Framing Error 0 */
#define _UART_RXDOUBLEX_FERR0_SHIFT          15                                      /**< Shift value for USART_FERR0 */
#define _UART_RXDOUBLEX_FERR0_MASK           0x8000UL                                /**< Bit mask for USART_FERR0 */
#define _UART_RXDOUBLEX_FERR0_DEFAULT        0x00000000UL                            /**< Mode DEFAULT for UART_RXDOUBLEX */
#define UART_RXDOUBLEX_FERR0_DEFAULT         (_UART_RXDOUBLEX_FERR0_DEFAULT << 15)   /**< Shifted mode DEFAULT for UART_RXDOUBLEX */
#define _UART_RXDOUBLEX_RXDATA1_SHIFT        16                                      /**< Shift value for USART_RXDATA1 */
#define _UART_RXDOUBLEX_RXDATA1_MASK         0x1FF0000UL                             /**< Bit mask for USART_RXDATA1 */
#define _UART_RXDOUBLEX_RXDATA1_DEFAULT      0x00000000UL                            /**< Mode DEFAULT for UART_RXDOUBLEX */
#define UART_RXDOUBLEX_RXDATA1_DEFAULT       (_UART_RXDOUBLEX_RXDATA1_DEFAULT << 16) /**< Shifted mode DEFAULT for UART_RXDOUBLEX */
#define UART_RXDOUBLEX_PERR1                 (0x1UL << 30)                           /**< Data Parity Error 1 */
#define _UART_RXDOUBLEX_PERR1_SHIFT          30                                      /**< Shift value for USART_PERR1 */
#define _UART_RXDOUBLEX_PERR1_MASK           0x40000000UL                            /**< Bit mask for USART_PERR1 */
#define _UART_RXDOUBLEX_PERR1_DEFAULT        0x00000000UL                            /**< Mode DEFAULT for UART_RXDOUBLEX */
#define UART_RXDOUBLEX_PERR1_DEFAULT         (_UART_RXDOUBLEX_PERR1_DEFAULT << 30)   /**< Shifted mode DEFAULT for UART_RXDOUBLEX */
#define UART_RXDOUBLEX_FERR1                 (0x1UL << 31)                           /**< Data Framing Error 1 */
#define _UART_RXDOUBLEX_FERR1_SHIFT          31                                      /**< Shift value for USART_FERR1 */
#define _UART_RXDOUBLEX_FERR1_MASK           0x80000000UL                            /**< Bit mask for USART_FERR1 */
#define _UART_RXDOUBLEX_FERR1_DEFAULT        0x00000000UL                            /**< Mode DEFAULT for UART_RXDOUBLEX */
#define UART_RXDOUBLEX_FERR1_DEFAULT         (_UART_RXDOUBLEX_FERR1_DEFAULT << 31)   /**< Shifted mode DEFAULT for UART_RXDOUBLEX */

/* Bit fields for UART RXDOUBLE */
#define _UART_RXDOUBLE_RESETVALUE            0x00000000UL                          /**< Default value for UART_RXDOUBLE */
#define _UART_RXDOUBLE_MASK                  0x0000FFFFUL                          /**< Mask for UART_RXDOUBLE */
#define _UART_RXDOUBLE_RXDATA0_SHIFT         0                                     /**< Shift value for USART_RXDATA0 */
#define _UART_RXDOUBLE_RXDATA0_MASK          0xFFUL                                /**< Bit mask for USART_RXDATA0 */
#define _UART_RXDOUBLE_RXDATA0_DEFAULT       0x00000000UL                          /**< Mode DEFAULT for UART_RXDOUBLE */
#define UART_RXDOUBLE_RXDATA0_DEFAULT        (_UART_RXDOUBLE_RXDATA0_DEFAULT << 0) /**< Shifted mode DEFAULT for UART_RXDOUBLE */
#define _UART_RXDOUBLE_RXDATA1_SHIFT         8                                     /**< Shift value for USART_RXDATA1 */
#define _UART_RXDOUBLE_RXDATA1_MASK          0xFF00UL                              /**< Bit mask for USART_RXDATA1 */
#define _UART_RXDOUBLE_RXDATA1_DEFAULT       0x00000000UL                          /**< Mode DEFAULT for UART_RXDOUBLE */
#define UART_RXDOUBLE_RXDATA1_DEFAULT        (_UART_RXDOUBLE_RXDATA1_DEFAULT << 8) /**< Shifted mode DEFAULT for UART_RXDOUBLE */

/* Bit fields for UART RXDATAXP */
#define _UART_RXDATAXP_RESETVALUE            0x00000000UL                          /**< Default value for UART_RXDATAXP */
#define _UART_RXDATAXP_MASK                  0x0000C1FFUL                          /**< Mask for UART_RXDATAXP */
#define _UART_RXDATAXP_RXDATAP_SHIFT         0                                     /**< Shift value for USART_RXDATAP */
#define _UART_RXDATAXP_RXDATAP_MASK          0x1FFUL                               /**< Bit mask for USART_RXDATAP */
#define _UART_RXDATAXP_RXDATAP_DEFAULT       0x00000000UL                          /**< Mode DEFAULT for UART_RXDATAXP */
#define UART_RXDATAXP_RXDATAP_DEFAULT        (_UART_RXDATAXP_RXDATAP_DEFAULT << 0) /**< Shifted mode DEFAULT for UART_RXDATAXP */
#define UART_RXDATAXP_PERRP                  (0x1UL << 14)                         /**< Data Parity Error Peek */
#define _UART_RXDATAXP_PERRP_SHIFT           14                                    /**< Shift value for USART_PERRP */
#define _UART_RXDATAXP_PERRP_MASK            0x4000UL                              /**< Bit mask for USART_PERRP */
#define _UART_RXDATAXP_PERRP_DEFAULT         0x00000000UL                          /**< Mode DEFAULT for UART_RXDATAXP */
#define UART_RXDATAXP_PERRP_DEFAULT          (_UART_RXDATAXP_PERRP_DEFAULT << 14)  /**< Shifted mode DEFAULT for UART_RXDATAXP */
#define UART_RXDATAXP_FERRP                  (0x1UL << 15)                         /**< Data Framing Error Peek */
#define _UART_RXDATAXP_FERRP_SHIFT           15                                    /**< Shift value for USART_FERRP */
#define _UART_RXDATAXP_FERRP_MASK            0x8000UL                              /**< Bit mask for USART_FERRP */
#define _UART_RXDATAXP_FERRP_DEFAULT         0x00000000UL                          /**< Mode DEFAULT for UART_RXDATAXP */
#define UART_RXDATAXP_FERRP_DEFAULT          (_UART_RXDATAXP_FERRP_DEFAULT << 15)  /**< Shifted mode DEFAULT for UART_RXDATAXP */

/* Bit fields for UART RXDOUBLEXP */
#define _UART_RXDOUBLEXP_RESETVALUE          0x00000000UL                              /**< Default value for UART_RXDOUBLEXP */
#define _UART_RXDOUBLEXP_MASK                0xC1FFC1FFUL                              /**< Mask for UART_RXDOUBLEXP */
#define _UART_RXDOUBLEXP_RXDATAP0_SHIFT      0                                         /**< Shift value for USART_RXDATAP0 */
#define _UART_RXDOUBLEXP_RXDATAP0_MASK       0x1FFUL                                   /**< Bit mask for USART_RXDATAP0 */
#define _UART_RXDOUBLEXP_RXDATAP0_DEFAULT    0x00000000UL                              /**< Mode DEFAULT for UART_RXDOUBLEXP */
#define UART_RXDOUBLEXP_RXDATAP0_DEFAULT     (_UART_RXDOUBLEXP_RXDATAP0_DEFAULT << 0)  /**< Shifted mode DEFAULT for UART_RXDOUBLEXP */
#define UART_RXDOUBLEXP_PERRP0               (0x1UL << 14)                             /**< Data Parity Error 0 Peek */
#define _UART_RXDOUBLEXP_PERRP0_SHIFT        14                                        /**< Shift value for USART_PERRP0 */
#define _UART_RXDOUBLEXP_PERRP0_MASK         0x4000UL                                  /**< Bit mask for USART_PERRP0 */
#define _UART_RXDOUBLEXP_PERRP0_DEFAULT      0x00000000UL                              /**< Mode DEFAULT for UART_RXDOUBLEXP */
#define UART_RXDOUBLEXP_PERRP0_DEFAULT       (_UART_RXDOUBLEXP_PERRP0_DEFAULT << 14)   /**< Shifted mode DEFAULT for UART_RXDOUBLEXP */
#define UART_RXDOUBLEXP_FERRP0               (0x1UL << 15)                             /**< Data Framing Error 0 Peek */
#define _UART_RXDOUBLEXP_FERRP0_SHIFT        15                                        /**< Shift value for USART_FERRP0 */
#define _UART_RXDOUBLEXP_FERRP0_MASK         0x8000UL                                  /**< Bit mask for USART_FERRP0 */
#define _UART_RXDOUBLEXP_FERRP0_DEFAULT      0x00000000UL                              /**< Mode DEFAULT for UART_RXDOUBLEXP */
#define UART_RXDOUBLEXP_FERRP0_DEFAULT       (_UART_RXDOUBLEXP_FERRP0_DEFAULT << 15)   /**< Shifted mode DEFAULT for UART_RXDOUBLEXP */
#define _UART_RXDOUBLEXP_RXDATAP1_SHIFT      16                                        /**< Shift value for USART_RXDATAP1 */
#define _UART_RXDOUBLEXP_RXDATAP1_MASK       0x1FF0000UL                               /**< Bit mask for USART_RXDATAP1 */
#define _UART_RXDOUBLEXP_RXDATAP1_DEFAULT    0x00000000UL                              /**< Mode DEFAULT for UART_RXDOUBLEXP */
#define UART_RXDOUBLEXP_RXDATAP1_DEFAULT     (_UART_RXDOUBLEXP_RXDATAP1_DEFAULT << 16) /**< Shifted mode DEFAULT for UART_RXDOUBLEXP */
#define UART_RXDOUBLEXP_PERRP1               (0x1UL << 30)                             /**< Data Parity Error 1 Peek */
#define _UART_RXDOUBLEXP_PERRP1_SHIFT        30                                        /**< Shift value for USART_PERRP1 */
#define _UART_RXDOUBLEXP_PERRP1_MASK         0x40000000UL                              /**< Bit mask for USART_PERRP1 */
#define _UART_RXDOUBLEXP_PERRP1_DEFAULT      0x00000000UL                              /**< Mode DEFAULT for UART_RXDOUBLEXP */
#define UART_RXDOUBLEXP_PERRP1_DEFAULT       (_UART_RXDOUBLEXP_PERRP1_DEFAULT << 30)   /**< Shifted mode DEFAULT for UART_RXDOUBLEXP */
#define UART_RXDOUBLEXP_FERRP1               (0x1UL << 31)                             /**< Data Framing Error 1 Peek */
#define _UART_RXDOUBLEXP_FERRP1_SHIFT        31                                        /**< Shift value for USART_FERRP1 */
#define _UART_RXDOUBLEXP_FERRP1_MASK         0x80000000UL                              /**< Bit mask for USART_FERRP1 */
#define _UART_RXDOUBLEXP_FERRP1_DEFAULT      0x00000000UL                              /**< Mode DEFAULT for UART_RXDOUBLEXP */
#define UART_RXDOUBLEXP_FERRP1_DEFAULT       (_UART_RXDOUBLEXP_FERRP1_DEFAULT << 31)   /**< Shifted mode DEFAULT for UART_RXDOUBLEXP */

/* Bit fields for UART TXDATAX */
#define _UART_TXDATAX_RESETVALUE             0x00000000UL                          /**< Default value for UART_TXDATAX */
#define _UART_TXDATAX_MASK                   0x0000F9FFUL                          /**< Mask for UART_TXDATAX */
#define _UART_TXDATAX_TXDATAX_SHIFT          0                                     /**< Shift value for USART_TXDATAX */
#define _UART_TXDATAX_TXDATAX_MASK           0x1FFUL                               /**< Bit mask for USART_TXDATAX */
#define _UART_TXDATAX_TXDATAX_DEFAULT        0x00000000UL                          /**< Mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_TXDATAX_DEFAULT         (_UART_TXDATAX_TXDATAX_DEFAULT << 0)  /**< Shifted mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_UBRXAT                  (0x1UL << 11)                         /**< Unblock RX After Transmission */
#define _UART_TXDATAX_UBRXAT_SHIFT           11                                    /**< Shift value for USART_UBRXAT */
#define _UART_TXDATAX_UBRXAT_MASK            0x800UL                               /**< Bit mask for USART_UBRXAT */
#define _UART_TXDATAX_UBRXAT_DEFAULT         0x00000000UL                          /**< Mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_UBRXAT_DEFAULT          (_UART_TXDATAX_UBRXAT_DEFAULT << 11)  /**< Shifted mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_TXTRIAT                 (0x1UL << 12)                         /**< Set TXTRI After Transmission */
#define _UART_TXDATAX_TXTRIAT_SHIFT          12                                    /**< Shift value for USART_TXTRIAT */
#define _UART_TXDATAX_TXTRIAT_MASK           0x1000UL                              /**< Bit mask for USART_TXTRIAT */
#define _UART_TXDATAX_TXTRIAT_DEFAULT        0x00000000UL                          /**< Mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_TXTRIAT_DEFAULT         (_UART_TXDATAX_TXTRIAT_DEFAULT << 12) /**< Shifted mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_TXBREAK                 (0x1UL << 13)                         /**< Transmit Data As Break */
#define _UART_TXDATAX_TXBREAK_SHIFT          13                                    /**< Shift value for USART_TXBREAK */
#define _UART_TXDATAX_TXBREAK_MASK           0x2000UL                              /**< Bit mask for USART_TXBREAK */
#define _UART_TXDATAX_TXBREAK_DEFAULT        0x00000000UL                          /**< Mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_TXBREAK_DEFAULT         (_UART_TXDATAX_TXBREAK_DEFAULT << 13) /**< Shifted mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_TXDISAT                 (0x1UL << 14)                         /**< Clear TXEN After Transmission */
#define _UART_TXDATAX_TXDISAT_SHIFT          14                                    /**< Shift value for USART_TXDISAT */
#define _UART_TXDATAX_TXDISAT_MASK           0x4000UL                              /**< Bit mask for USART_TXDISAT */
#define _UART_TXDATAX_TXDISAT_DEFAULT        0x00000000UL                          /**< Mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_TXDISAT_DEFAULT         (_UART_TXDATAX_TXDISAT_DEFAULT << 14) /**< Shifted mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_RXENAT                  (0x1UL << 15)                         /**< Enable RX After Transmission */
#define _UART_TXDATAX_RXENAT_SHIFT           15                                    /**< Shift value for USART_RXENAT */
#define _UART_TXDATAX_RXENAT_MASK            0x8000UL                              /**< Bit mask for USART_RXENAT */
#define _UART_TXDATAX_RXENAT_DEFAULT         0x00000000UL                          /**< Mode DEFAULT for UART_TXDATAX */
#define UART_TXDATAX_RXENAT_DEFAULT          (_UART_TXDATAX_RXENAT_DEFAULT << 15)  /**< Shifted mode DEFAULT for UART_TXDATAX */

/* Bit fields for UART TXDATA */
#define _UART_TXDATA_RESETVALUE              0x00000000UL                       /**< Default value for UART_TXDATA */
#define _UART_TXDATA_MASK                    0x000000FFUL                       /**< Mask for UART_TXDATA */
#define _UART_TXDATA_TXDATA_SHIFT            0                                  /**< Shift value for USART_TXDATA */
#define _UART_TXDATA_TXDATA_MASK             0xFFUL                             /**< Bit mask for USART_TXDATA */
#define _UART_TXDATA_TXDATA_DEFAULT          0x00000000UL                       /**< Mode DEFAULT for UART_TXDATA */
#define UART_TXDATA_TXDATA_DEFAULT           (_UART_TXDATA_TXDATA_DEFAULT << 0) /**< Shifted mode DEFAULT for UART_TXDATA */

/* Bit fields for UART TXDOUBLEX */
#define _UART_TXDOUBLEX_RESETVALUE           0x00000000UL                             /**< Default value for UART_TXDOUBLEX */
#define _UART_TXDOUBLEX_MASK                 0xF9FFF9FFUL                             /**< Mask for UART_TXDOUBLEX */
#define _UART_TXDOUBLEX_TXDATA0_SHIFT        0                                        /**< Shift value for USART_TXDATA0 */
#define _UART_TXDOUBLEX_TXDATA0_MASK         0x1FFUL                                  /**< Bit mask for USART_TXDATA0 */
#define _UART_TXDOUBLEX_TXDATA0_DEFAULT      0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXDATA0_DEFAULT       (_UART_TXDOUBLEX_TXDATA0_DEFAULT << 0)   /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_UBRXAT0               (0x1UL << 11)                            /**< Unblock RX After Transmission */
#define _UART_TXDOUBLEX_UBRXAT0_SHIFT        11                                       /**< Shift value for USART_UBRXAT0 */
#define _UART_TXDOUBLEX_UBRXAT0_MASK         0x800UL                                  /**< Bit mask for USART_UBRXAT0 */
#define _UART_TXDOUBLEX_UBRXAT0_DEFAULT      0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_UBRXAT0_DEFAULT       (_UART_TXDOUBLEX_UBRXAT0_DEFAULT << 11)  /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXTRIAT0              (0x1UL << 12)                            /**< Set TXTRI After Transmission */
#define _UART_TXDOUBLEX_TXTRIAT0_SHIFT       12                                       /**< Shift value for USART_TXTRIAT0 */
#define _UART_TXDOUBLEX_TXTRIAT0_MASK        0x1000UL                                 /**< Bit mask for USART_TXTRIAT0 */
#define _UART_TXDOUBLEX_TXTRIAT0_DEFAULT     0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXTRIAT0_DEFAULT      (_UART_TXDOUBLEX_TXTRIAT0_DEFAULT << 12) /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXBREAK0              (0x1UL << 13)                            /**< Transmit Data As Break */
#define _UART_TXDOUBLEX_TXBREAK0_SHIFT       13                                       /**< Shift value for USART_TXBREAK0 */
#define _UART_TXDOUBLEX_TXBREAK0_MASK        0x2000UL                                 /**< Bit mask for USART_TXBREAK0 */
#define _UART_TXDOUBLEX_TXBREAK0_DEFAULT     0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXBREAK0_DEFAULT      (_UART_TXDOUBLEX_TXBREAK0_DEFAULT << 13) /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXDISAT0              (0x1UL << 14)                            /**< Clear TXEN After Transmission */
#define _UART_TXDOUBLEX_TXDISAT0_SHIFT       14                                       /**< Shift value for USART_TXDISAT0 */
#define _UART_TXDOUBLEX_TXDISAT0_MASK        0x4000UL                                 /**< Bit mask for USART_TXDISAT0 */
#define _UART_TXDOUBLEX_TXDISAT0_DEFAULT     0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXDISAT0_DEFAULT      (_UART_TXDOUBLEX_TXDISAT0_DEFAULT << 14) /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_RXENAT0               (0x1UL << 15)                            /**< Enable RX After Transmission */
#define _UART_TXDOUBLEX_RXENAT0_SHIFT        15                                       /**< Shift value for USART_RXENAT0 */
#define _UART_TXDOUBLEX_RXENAT0_MASK         0x8000UL                                 /**< Bit mask for USART_RXENAT0 */
#define _UART_TXDOUBLEX_RXENAT0_DEFAULT      0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_RXENAT0_DEFAULT       (_UART_TXDOUBLEX_RXENAT0_DEFAULT << 15)  /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define _UART_TXDOUBLEX_TXDATA1_SHIFT        16                                       /**< Shift value for USART_TXDATA1 */
#define _UART_TXDOUBLEX_TXDATA1_MASK         0x1FF0000UL                              /**< Bit mask for USART_TXDATA1 */
#define _UART_TXDOUBLEX_TXDATA1_DEFAULT      0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXDATA1_DEFAULT       (_UART_TXDOUBLEX_TXDATA1_DEFAULT << 16)  /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_UBRXAT1               (0x1UL << 27)                            /**< Unblock RX After Transmission */
#define _UART_TXDOUBLEX_UBRXAT1_SHIFT        27                                       /**< Shift value for USART_UBRXAT1 */
#define _UART_TXDOUBLEX_UBRXAT1_MASK         0x8000000UL                              /**< Bit mask for USART_UBRXAT1 */
#define _UART_TXDOUBLEX_UBRXAT1_DEFAULT      0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_UBRXAT1_DEFAULT       (_UART_TXDOUBLEX_UBRXAT1_DEFAULT << 27)  /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXTRIAT1              (0x1UL << 28)                            /**< Set TXTRI After Transmission */
#define _UART_TXDOUBLEX_TXTRIAT1_SHIFT       28                                       /**< Shift value for USART_TXTRIAT1 */
#define _UART_TXDOUBLEX_TXTRIAT1_MASK        0x10000000UL                             /**< Bit mask for USART_TXTRIAT1 */
#define _UART_TXDOUBLEX_TXTRIAT1_DEFAULT     0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXTRIAT1_DEFAULT      (_UART_TXDOUBLEX_TXTRIAT1_DEFAULT << 28) /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXBREAK1              (0x1UL << 29)                            /**< Transmit Data As Break */
#define _UART_TXDOUBLEX_TXBREAK1_SHIFT       29                                       /**< Shift value for USART_TXBREAK1 */
#define _UART_TXDOUBLEX_TXBREAK1_MASK        0x20000000UL                             /**< Bit mask for USART_TXBREAK1 */
#define _UART_TXDOUBLEX_TXBREAK1_DEFAULT     0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXBREAK1_DEFAULT      (_UART_TXDOUBLEX_TXBREAK1_DEFAULT << 29) /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXDISAT1              (0x1UL << 30)                            /**< Clear TXEN After Transmission */
#define _UART_TXDOUBLEX_TXDISAT1_SHIFT       30                                       /**< Shift value for USART_TXDISAT1 */
#define _UART_TXDOUBLEX_TXDISAT1_MASK        0x40000000UL                             /**< Bit mask for USART_TXDISAT1 */
#define _UART_TXDOUBLEX_TXDISAT1_DEFAULT     0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_TXDISAT1_DEFAULT      (_UART_TXDOUBLEX_TXDISAT1_DEFAULT << 30) /**< Shifted mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_RXENAT1               (0x1UL << 31)                            /**< Enable RX After Transmission */
#define _UART_TXDOUBLEX_RXENAT1_SHIFT        31                                       /**< Shift value for USART_RXENAT1 */
#define _UART_TXDOUBLEX_RXENAT1_MASK         0x80000000UL                             /**< Bit mask for USART_RXENAT1 */
#define _UART_TXDOUBLEX_RXENAT1_DEFAULT      0x00000000UL                             /**< Mode DEFAULT for UART_TXDOUBLEX */
#define UART_TXDOUBLEX_RXENAT1_DEFAULT       (_UART_TXDOUBLEX_RXENAT1_DEFAULT << 31)  /**< Shifted mode DEFAULT for UART_TXDOUBLEX */

/* Bit fields for UART TXDOUBLE */
#define _UART_TXDOUBLE_RESETVALUE            0x00000000UL                          /**< Default value for UART_TXDOUBLE */
#define _UART_TXDOUBLE_MASK                  0x0000FFFFUL                          /**< Mask for UART_TXDOUBLE */
#define _UART_TXDOUBLE_TXDATA0_SHIFT         0                                     /**< Shift value for USART_TXDATA0 */
#define _UART_TXDOUBLE_TXDATA0_MASK          0xFFUL                                /**< Bit mask for USART_TXDATA0 */
#define _UART_TXDOUBLE_TXDATA0_DEFAULT       0x00000000UL                          /**< Mode DEFAULT for UART_TXDOUBLE */
#define UART_TXDOUBLE_TXDATA0_DEFAULT        (_UART_TXDOUBLE_TXDATA0_DEFAULT << 0) /**< Shifted mode DEFAULT for UART_TXDOUBLE */
#define _UART_TXDOUBLE_TXDATA1_SHIFT         8                                     /**< Shift value for USART_TXDATA1 */
#define _UART_TXDOUBLE_TXDATA1_MASK          0xFF00UL                              /**< Bit mask for USART_TXDATA1 */
#define _UART_TXDOUBLE_TXDATA1_DEFAULT       0x00000000UL                          /**< Mode DEFAULT for UART_TXDOUBLE */
#define UART_TXDOUBLE_TXDATA1_DEFAULT        (_UART_TXDOUBLE_TXDATA1_DEFAULT << 8) /**< Shifted mode DEFAULT for UART_TXDOUBLE */

/* Bit fields for UART IF */
#define _UART_IF_RESETVALUE                  0x00000002UL                    /**< Default value for UART_IF */
#define _UART_IF_MASK                        0x00001FFFUL                    /**< Mask for UART_IF */
#define UART_IF_TXC                          (0x1UL << 0)                    /**< TX Complete Interrupt Flag */
#define _UART_IF_TXC_SHIFT                   0                               /**< Shift value for USART_TXC */
#define _UART_IF_TXC_MASK                    0x1UL                           /**< Bit mask for USART_TXC */
#define _UART_IF_TXC_DEFAULT                 0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_TXC_DEFAULT                  (_UART_IF_TXC_DEFAULT << 0)     /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_TXBL                         (0x1UL << 1)                    /**< TX Buffer Level Interrupt Flag */
#define _UART_IF_TXBL_SHIFT                  1                               /**< Shift value for USART_TXBL */
#define _UART_IF_TXBL_MASK                   0x2UL                           /**< Bit mask for USART_TXBL */
#define _UART_IF_TXBL_DEFAULT                0x00000001UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_TXBL_DEFAULT                 (_UART_IF_TXBL_DEFAULT << 1)    /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_RXDATAV                      (0x1UL << 2)                    /**< RX Data Valid Interrupt Flag */
#define _UART_IF_RXDATAV_SHIFT               2                               /**< Shift value for USART_RXDATAV */
#define _UART_IF_RXDATAV_MASK                0x4UL                           /**< Bit mask for USART_RXDATAV */
#define _UART_IF_RXDATAV_DEFAULT             0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_RXDATAV_DEFAULT              (_UART_IF_RXDATAV_DEFAULT << 2) /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_RXFULL                       (0x1UL << 3)                    /**< RX Buffer Full Interrupt Flag */
#define _UART_IF_RXFULL_SHIFT                3                               /**< Shift value for USART_RXFULL */
#define _UART_IF_RXFULL_MASK                 0x8UL                           /**< Bit mask for USART_RXFULL */
#define _UART_IF_RXFULL_DEFAULT              0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_RXFULL_DEFAULT               (_UART_IF_RXFULL_DEFAULT << 3)  /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_RXOF                         (0x1UL << 4)                    /**< RX Overflow Interrupt Flag */
#define _UART_IF_RXOF_SHIFT                  4                               /**< Shift value for USART_RXOF */
#define _UART_IF_RXOF_MASK                   0x10UL                          /**< Bit mask for USART_RXOF */
#define _UART_IF_RXOF_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_RXOF_DEFAULT                 (_UART_IF_RXOF_DEFAULT << 4)    /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_RXUF                         (0x1UL << 5)                    /**< RX Underflow Interrupt Flag */
#define _UART_IF_RXUF_SHIFT                  5                               /**< Shift value for USART_RXUF */
#define _UART_IF_RXUF_MASK                   0x20UL                          /**< Bit mask for USART_RXUF */
#define _UART_IF_RXUF_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_RXUF_DEFAULT                 (_UART_IF_RXUF_DEFAULT << 5)    /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_TXOF                         (0x1UL << 6)                    /**< TX Overflow Interrupt Flag */
#define _UART_IF_TXOF_SHIFT                  6                               /**< Shift value for USART_TXOF */
#define _UART_IF_TXOF_MASK                   0x40UL                          /**< Bit mask for USART_TXOF */
#define _UART_IF_TXOF_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_TXOF_DEFAULT                 (_UART_IF_TXOF_DEFAULT << 6)    /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_TXUF                         (0x1UL << 7)                    /**< TX Underflow Interrupt Flag */
#define _UART_IF_TXUF_SHIFT                  7                               /**< Shift value for USART_TXUF */
#define _UART_IF_TXUF_MASK                   0x80UL                          /**< Bit mask for USART_TXUF */
#define _UART_IF_TXUF_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_TXUF_DEFAULT                 (_UART_IF_TXUF_DEFAULT << 7)    /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_PERR                         (0x1UL << 8)                    /**< Parity Error Interrupt Flag */
#define _UART_IF_PERR_SHIFT                  8                               /**< Shift value for USART_PERR */
#define _UART_IF_PERR_MASK                   0x100UL                         /**< Bit mask for USART_PERR */
#define _UART_IF_PERR_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_PERR_DEFAULT                 (_UART_IF_PERR_DEFAULT << 8)    /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_FERR                         (0x1UL << 9)                    /**< Framing Error Interrupt Flag */
#define _UART_IF_FERR_SHIFT                  9                               /**< Shift value for USART_FERR */
#define _UART_IF_FERR_MASK                   0x200UL                         /**< Bit mask for USART_FERR */
#define _UART_IF_FERR_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_FERR_DEFAULT                 (_UART_IF_FERR_DEFAULT << 9)    /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_MPAF                         (0x1UL << 10)                   /**< Multi-Processor Address Frame Interrupt Flag */
#define _UART_IF_MPAF_SHIFT                  10                              /**< Shift value for USART_MPAF */
#define _UART_IF_MPAF_MASK                   0x400UL                         /**< Bit mask for USART_MPAF */
#define _UART_IF_MPAF_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_MPAF_DEFAULT                 (_UART_IF_MPAF_DEFAULT << 10)   /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_SSM                          (0x1UL << 11)                   /**< Slave-Select In Master Mode Interrupt Flag */
#define _UART_IF_SSM_SHIFT                   11                              /**< Shift value for USART_SSM */
#define _UART_IF_SSM_MASK                    0x800UL                         /**< Bit mask for USART_SSM */
#define _UART_IF_SSM_DEFAULT                 0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_SSM_DEFAULT                  (_UART_IF_SSM_DEFAULT << 11)    /**< Shifted mode DEFAULT for UART_IF */
#define UART_IF_CCF                          (0x1UL << 12)                   /**< Collision Check Fail Interrupt Flag */
#define _UART_IF_CCF_SHIFT                   12                              /**< Shift value for USART_CCF */
#define _UART_IF_CCF_MASK                    0x1000UL                        /**< Bit mask for USART_CCF */
#define _UART_IF_CCF_DEFAULT                 0x00000000UL                    /**< Mode DEFAULT for UART_IF */
#define UART_IF_CCF_DEFAULT                  (_UART_IF_CCF_DEFAULT << 12)    /**< Shifted mode DEFAULT for UART_IF */

/* Bit fields for UART IFS */
#define _UART_IFS_RESETVALUE                 0x00000000UL                    /**< Default value for UART_IFS */
#define _UART_IFS_MASK                       0x00001FF9UL                    /**< Mask for UART_IFS */
#define UART_IFS_TXC                         (0x1UL << 0)                    /**< Set TX Complete Interrupt Flag */
#define _UART_IFS_TXC_SHIFT                  0                               /**< Shift value for USART_TXC */
#define _UART_IFS_TXC_MASK                   0x1UL                           /**< Bit mask for USART_TXC */
#define _UART_IFS_TXC_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_TXC_DEFAULT                 (_UART_IFS_TXC_DEFAULT << 0)    /**< Shifted mode DEFAULT for UART_IFS */
#define UART_IFS_RXFULL                      (0x1UL << 3)                    /**< Set RX Buffer Full Interrupt Flag */
#define _UART_IFS_RXFULL_SHIFT               3                               /**< Shift value for USART_RXFULL */
#define _UART_IFS_RXFULL_MASK                0x8UL                           /**< Bit mask for USART_RXFULL */
#define _UART_IFS_RXFULL_DEFAULT             0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_RXFULL_DEFAULT              (_UART_IFS_RXFULL_DEFAULT << 3) /**< Shifted mode DEFAULT for UART_IFS */
#define UART_IFS_RXOF                        (0x1UL << 4)                    /**< Set RX Overflow Interrupt Flag */
#define _UART_IFS_RXOF_SHIFT                 4                               /**< Shift value for USART_RXOF */
#define _UART_IFS_RXOF_MASK                  0x10UL                          /**< Bit mask for USART_RXOF */
#define _UART_IFS_RXOF_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_RXOF_DEFAULT                (_UART_IFS_RXOF_DEFAULT << 4)   /**< Shifted mode DEFAULT for UART_IFS */
#define UART_IFS_RXUF                        (0x1UL << 5)                    /**< Set RX Underflow Interrupt Flag */
#define _UART_IFS_RXUF_SHIFT                 5                               /**< Shift value for USART_RXUF */
#define _UART_IFS_RXUF_MASK                  0x20UL                          /**< Bit mask for USART_RXUF */
#define _UART_IFS_RXUF_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_RXUF_DEFAULT                (_UART_IFS_RXUF_DEFAULT << 5)   /**< Shifted mode DEFAULT for UART_IFS */
#define UART_IFS_TXOF                        (0x1UL << 6)                    /**< Set TX Overflow Interrupt Flag */
#define _UART_IFS_TXOF_SHIFT                 6                               /**< Shift value for USART_TXOF */
#define _UART_IFS_TXOF_MASK                  0x40UL                          /**< Bit mask for USART_TXOF */
#define _UART_IFS_TXOF_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_TXOF_DEFAULT                (_UART_IFS_TXOF_DEFAULT << 6)   /**< Shifted mode DEFAULT for UART_IFS */
#define UART_IFS_TXUF                        (0x1UL << 7)                    /**< Set TX Underflow Interrupt Flag */
#define _UART_IFS_TXUF_SHIFT                 7                               /**< Shift value for USART_TXUF */
#define _UART_IFS_TXUF_MASK                  0x80UL                          /**< Bit mask for USART_TXUF */
#define _UART_IFS_TXUF_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_TXUF_DEFAULT                (_UART_IFS_TXUF_DEFAULT << 7)   /**< Shifted mode DEFAULT for UART_IFS */
#define UART_IFS_PERR                        (0x1UL << 8)                    /**< Set Parity Error Interrupt Flag */
#define _UART_IFS_PERR_SHIFT                 8                               /**< Shift value for USART_PERR */
#define _UART_IFS_PERR_MASK                  0x100UL                         /**< Bit mask for USART_PERR */
#define _UART_IFS_PERR_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_PERR_DEFAULT                (_UART_IFS_PERR_DEFAULT << 8)   /**< Shifted mode DEFAULT for UART_IFS */
#define UART_IFS_FERR                        (0x1UL << 9)                    /**< Set Framing Error Interrupt Flag */
#define _UART_IFS_FERR_SHIFT                 9                               /**< Shift value for USART_FERR */
#define _UART_IFS_FERR_MASK                  0x200UL                         /**< Bit mask for USART_FERR */
#define _UART_IFS_FERR_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_FERR_DEFAULT                (_UART_IFS_FERR_DEFAULT << 9)   /**< Shifted mode DEFAULT for UART_IFS */
#define UART_IFS_MPAF                        (0x1UL << 10)                   /**< Set Multi-Processor Address Frame Interrupt Flag */
#define _UART_IFS_MPAF_SHIFT                 10                              /**< Shift value for USART_MPAF */
#define _UART_IFS_MPAF_MASK                  0x400UL                         /**< Bit mask for USART_MPAF */
#define _UART_IFS_MPAF_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_MPAF_DEFAULT                (_UART_IFS_MPAF_DEFAULT << 10)  /**< Shifted mode DEFAULT for UART_IFS */
#define UART_IFS_SSM                         (0x1UL << 11)                   /**< Set Slave-Select in Master mode Interrupt Flag */
#define _UART_IFS_SSM_SHIFT                  11                              /**< Shift value for USART_SSM */
#define _UART_IFS_SSM_MASK                   0x800UL                         /**< Bit mask for USART_SSM */
#define _UART_IFS_SSM_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_SSM_DEFAULT                 (_UART_IFS_SSM_DEFAULT << 11)   /**< Shifted mode DEFAULT for UART_IFS */
#define UART_IFS_CCF                         (0x1UL << 12)                   /**< Set Collision Check Fail Interrupt Flag */
#define _UART_IFS_CCF_SHIFT                  12                              /**< Shift value for USART_CCF */
#define _UART_IFS_CCF_MASK                   0x1000UL                        /**< Bit mask for USART_CCF */
#define _UART_IFS_CCF_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IFS */
#define UART_IFS_CCF_DEFAULT                 (_UART_IFS_CCF_DEFAULT << 12)   /**< Shifted mode DEFAULT for UART_IFS */

/* Bit fields for UART IFC */
#define _UART_IFC_RESETVALUE                 0x00000000UL                    /**< Default value for UART_IFC */
#define _UART_IFC_MASK                       0x00001FF9UL                    /**< Mask for UART_IFC */
#define UART_IFC_TXC                         (0x1UL << 0)                    /**< Clear TX Complete Interrupt Flag */
#define _UART_IFC_TXC_SHIFT                  0                               /**< Shift value for USART_TXC */
#define _UART_IFC_TXC_MASK                   0x1UL                           /**< Bit mask for USART_TXC */
#define _UART_IFC_TXC_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_TXC_DEFAULT                 (_UART_IFC_TXC_DEFAULT << 0)    /**< Shifted mode DEFAULT for UART_IFC */
#define UART_IFC_RXFULL                      (0x1UL << 3)                    /**< Clear RX Buffer Full Interrupt Flag */
#define _UART_IFC_RXFULL_SHIFT               3                               /**< Shift value for USART_RXFULL */
#define _UART_IFC_RXFULL_MASK                0x8UL                           /**< Bit mask for USART_RXFULL */
#define _UART_IFC_RXFULL_DEFAULT             0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_RXFULL_DEFAULT              (_UART_IFC_RXFULL_DEFAULT << 3) /**< Shifted mode DEFAULT for UART_IFC */
#define UART_IFC_RXOF                        (0x1UL << 4)                    /**< Clear RX Overflow Interrupt Flag */
#define _UART_IFC_RXOF_SHIFT                 4                               /**< Shift value for USART_RXOF */
#define _UART_IFC_RXOF_MASK                  0x10UL                          /**< Bit mask for USART_RXOF */
#define _UART_IFC_RXOF_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_RXOF_DEFAULT                (_UART_IFC_RXOF_DEFAULT << 4)   /**< Shifted mode DEFAULT for UART_IFC */
#define UART_IFC_RXUF                        (0x1UL << 5)                    /**< Clear RX Underflow Interrupt Flag */
#define _UART_IFC_RXUF_SHIFT                 5                               /**< Shift value for USART_RXUF */
#define _UART_IFC_RXUF_MASK                  0x20UL                          /**< Bit mask for USART_RXUF */
#define _UART_IFC_RXUF_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_RXUF_DEFAULT                (_UART_IFC_RXUF_DEFAULT << 5)   /**< Shifted mode DEFAULT for UART_IFC */
#define UART_IFC_TXOF                        (0x1UL << 6)                    /**< Clear TX Overflow Interrupt Flag */
#define _UART_IFC_TXOF_SHIFT                 6                               /**< Shift value for USART_TXOF */
#define _UART_IFC_TXOF_MASK                  0x40UL                          /**< Bit mask for USART_TXOF */
#define _UART_IFC_TXOF_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_TXOF_DEFAULT                (_UART_IFC_TXOF_DEFAULT << 6)   /**< Shifted mode DEFAULT for UART_IFC */
#define UART_IFC_TXUF                        (0x1UL << 7)                    /**< Clear TX Underflow Interrupt Flag */
#define _UART_IFC_TXUF_SHIFT                 7                               /**< Shift value for USART_TXUF */
#define _UART_IFC_TXUF_MASK                  0x80UL                          /**< Bit mask for USART_TXUF */
#define _UART_IFC_TXUF_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_TXUF_DEFAULT                (_UART_IFC_TXUF_DEFAULT << 7)   /**< Shifted mode DEFAULT for UART_IFC */
#define UART_IFC_PERR                        (0x1UL << 8)                    /**< Clear Parity Error Interrupt Flag */
#define _UART_IFC_PERR_SHIFT                 8                               /**< Shift value for USART_PERR */
#define _UART_IFC_PERR_MASK                  0x100UL                         /**< Bit mask for USART_PERR */
#define _UART_IFC_PERR_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_PERR_DEFAULT                (_UART_IFC_PERR_DEFAULT << 8)   /**< Shifted mode DEFAULT for UART_IFC */
#define UART_IFC_FERR                        (0x1UL << 9)                    /**< Clear Framing Error Interrupt Flag */
#define _UART_IFC_FERR_SHIFT                 9                               /**< Shift value for USART_FERR */
#define _UART_IFC_FERR_MASK                  0x200UL                         /**< Bit mask for USART_FERR */
#define _UART_IFC_FERR_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_FERR_DEFAULT                (_UART_IFC_FERR_DEFAULT << 9)   /**< Shifted mode DEFAULT for UART_IFC */
#define UART_IFC_MPAF                        (0x1UL << 10)                   /**< Clear Multi-Processor Address Frame Interrupt Flag */
#define _UART_IFC_MPAF_SHIFT                 10                              /**< Shift value for USART_MPAF */
#define _UART_IFC_MPAF_MASK                  0x400UL                         /**< Bit mask for USART_MPAF */
#define _UART_IFC_MPAF_DEFAULT               0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_MPAF_DEFAULT                (_UART_IFC_MPAF_DEFAULT << 10)  /**< Shifted mode DEFAULT for UART_IFC */
#define UART_IFC_SSM                         (0x1UL << 11)                   /**< Clear Slave-Select In Master Mode Interrupt Flag */
#define _UART_IFC_SSM_SHIFT                  11                              /**< Shift value for USART_SSM */
#define _UART_IFC_SSM_MASK                   0x800UL                         /**< Bit mask for USART_SSM */
#define _UART_IFC_SSM_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_SSM_DEFAULT                 (_UART_IFC_SSM_DEFAULT << 11)   /**< Shifted mode DEFAULT for UART_IFC */
#define UART_IFC_CCF                         (0x1UL << 12)                   /**< Clear Collision Check Fail Interrupt Flag */
#define _UART_IFC_CCF_SHIFT                  12                              /**< Shift value for USART_CCF */
#define _UART_IFC_CCF_MASK                   0x1000UL                        /**< Bit mask for USART_CCF */
#define _UART_IFC_CCF_DEFAULT                0x00000000UL                    /**< Mode DEFAULT for UART_IFC */
#define UART_IFC_CCF_DEFAULT                 (_UART_IFC_CCF_DEFAULT << 12)   /**< Shifted mode DEFAULT for UART_IFC */

/* Bit fields for UART IEN */
#define _UART_IEN_RESETVALUE                 0x00000000UL                     /**< Default value for UART_IEN */
#define _UART_IEN_MASK                       0x00001FFFUL                     /**< Mask for UART_IEN */
#define UART_IEN_TXC                         (0x1UL << 0)                     /**< TX Complete Interrupt Enable */
#define _UART_IEN_TXC_SHIFT                  0                                /**< Shift value for USART_TXC */
#define _UART_IEN_TXC_MASK                   0x1UL                            /**< Bit mask for USART_TXC */
#define _UART_IEN_TXC_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_TXC_DEFAULT                 (_UART_IEN_TXC_DEFAULT << 0)     /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_TXBL                        (0x1UL << 1)                     /**< TX Buffer Level Interrupt Enable */
#define _UART_IEN_TXBL_SHIFT                 1                                /**< Shift value for USART_TXBL */
#define _UART_IEN_TXBL_MASK                  0x2UL                            /**< Bit mask for USART_TXBL */
#define _UART_IEN_TXBL_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_TXBL_DEFAULT                (_UART_IEN_TXBL_DEFAULT << 1)    /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_RXDATAV                     (0x1UL << 2)                     /**< RX Data Valid Interrupt Enable */
#define _UART_IEN_RXDATAV_SHIFT              2                                /**< Shift value for USART_RXDATAV */
#define _UART_IEN_RXDATAV_MASK               0x4UL                            /**< Bit mask for USART_RXDATAV */
#define _UART_IEN_RXDATAV_DEFAULT            0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_RXDATAV_DEFAULT             (_UART_IEN_RXDATAV_DEFAULT << 2) /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_RXFULL                      (0x1UL << 3)                     /**< RX Buffer Full Interrupt Enable */
#define _UART_IEN_RXFULL_SHIFT               3                                /**< Shift value for USART_RXFULL */
#define _UART_IEN_RXFULL_MASK                0x8UL                            /**< Bit mask for USART_RXFULL */
#define _UART_IEN_RXFULL_DEFAULT             0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_RXFULL_DEFAULT              (_UART_IEN_RXFULL_DEFAULT << 3)  /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_RXOF                        (0x1UL << 4)                     /**< RX Overflow Interrupt Enable */
#define _UART_IEN_RXOF_SHIFT                 4                                /**< Shift value for USART_RXOF */
#define _UART_IEN_RXOF_MASK                  0x10UL                           /**< Bit mask for USART_RXOF */
#define _UART_IEN_RXOF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_RXOF_DEFAULT                (_UART_IEN_RXOF_DEFAULT << 4)    /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_RXUF                        (0x1UL << 5)                     /**< RX Underflow Interrupt Enable */
#define _UART_IEN_RXUF_SHIFT                 5                                /**< Shift value for USART_RXUF */
#define _UART_IEN_RXUF_MASK                  0x20UL                           /**< Bit mask for USART_RXUF */
#define _UART_IEN_RXUF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_RXUF_DEFAULT                (_UART_IEN_RXUF_DEFAULT << 5)    /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_TXOF                        (0x1UL << 6)                     /**< TX Overflow Interrupt Enable */
#define _UART_IEN_TXOF_SHIFT                 6                                /**< Shift value for USART_TXOF */
#define _UART_IEN_TXOF_MASK                  0x40UL                           /**< Bit mask for USART_TXOF */
#define _UART_IEN_TXOF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_TXOF_DEFAULT                (_UART_IEN_TXOF_DEFAULT << 6)    /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_TXUF                        (0x1UL << 7)                     /**< TX Underflow Interrupt Enable */
#define _UART_IEN_TXUF_SHIFT                 7                                /**< Shift value for USART_TXUF */
#define _UART_IEN_TXUF_MASK                  0x80UL                           /**< Bit mask for USART_TXUF */
#define _UART_IEN_TXUF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_TXUF_DEFAULT                (_UART_IEN_TXUF_DEFAULT << 7)    /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_PERR                        (0x1UL << 8)                     /**< Parity Error Interrupt Enable */
#define _UART_IEN_PERR_SHIFT                 8                                /**< Shift value for USART_PERR */
#define _UART_IEN_PERR_MASK                  0x100UL                          /**< Bit mask for USART_PERR */
#define _UART_IEN_PERR_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_PERR_DEFAULT                (_UART_IEN_PERR_DEFAULT << 8)    /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_FERR                        (0x1UL << 9)                     /**< Framing Error Interrupt Enable */
#define _UART_IEN_FERR_SHIFT                 9                                /**< Shift value for USART_FERR */
#define _UART_IEN_FERR_MASK                  0x200UL                          /**< Bit mask for USART_FERR */
#define _UART_IEN_FERR_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_FERR_DEFAULT                (_UART_IEN_FERR_DEFAULT << 9)    /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_MPAF                        (0x1UL << 10)                    /**< Multi-Processor Address Frame Interrupt Enable */
#define _UART_IEN_MPAF_SHIFT                 10                               /**< Shift value for USART_MPAF */
#define _UART_IEN_MPAF_MASK                  0x400UL                          /**< Bit mask for USART_MPAF */
#define _UART_IEN_MPAF_DEFAULT               0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_MPAF_DEFAULT                (_UART_IEN_MPAF_DEFAULT << 10)   /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_SSM                         (0x1UL << 11)                    /**< Slave-Select In Master Mode Interrupt Enable */
#define _UART_IEN_SSM_SHIFT                  11                               /**< Shift value for USART_SSM */
#define _UART_IEN_SSM_MASK                   0x800UL                          /**< Bit mask for USART_SSM */
#define _UART_IEN_SSM_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_SSM_DEFAULT                 (_UART_IEN_SSM_DEFAULT << 11)    /**< Shifted mode DEFAULT for UART_IEN */
#define UART_IEN_CCF                         (0x1UL << 12)                    /**< Collision Check Fail Interrupt Enable */
#define _UART_IEN_CCF_SHIFT                  12                               /**< Shift value for USART_CCF */
#define _UART_IEN_CCF_MASK                   0x1000UL                         /**< Bit mask for USART_CCF */
#define _UART_IEN_CCF_DEFAULT                0x00000000UL                     /**< Mode DEFAULT for UART_IEN */
#define UART_IEN_CCF_DEFAULT                 (_UART_IEN_CCF_DEFAULT << 12)    /**< Shifted mode DEFAULT for UART_IEN */

/* Bit fields for UART IRCTRL */
#define _UART_IRCTRL_RESETVALUE              0x00000000UL                         /**< Default value for UART_IRCTRL */
#define _UART_IRCTRL_MASK                    0x000000FFUL                         /**< Mask for UART_IRCTRL */
#define UART_IRCTRL_IREN                     (0x1UL << 0)                         /**< Enable IrDA Module */
#define _UART_IRCTRL_IREN_SHIFT              0                                    /**< Shift value for USART_IREN */
#define _UART_IRCTRL_IREN_MASK               0x1UL                                /**< Bit mask for USART_IREN */
#define _UART_IRCTRL_IREN_DEFAULT            0x00000000UL                         /**< Mode DEFAULT for UART_IRCTRL */
#define UART_IRCTRL_IREN_DEFAULT             (_UART_IRCTRL_IREN_DEFAULT << 0)     /**< Shifted mode DEFAULT for UART_IRCTRL */
#define _UART_IRCTRL_IRPW_SHIFT              1                                    /**< Shift value for USART_IRPW */
#define _UART_IRCTRL_IRPW_MASK               0x6UL                                /**< Bit mask for USART_IRPW */
#define _UART_IRCTRL_IRPW_DEFAULT            0x00000000UL                         /**< Mode DEFAULT for UART_IRCTRL */
#define _UART_IRCTRL_IRPW_ONE                0x00000000UL                         /**< Mode ONE for UART_IRCTRL */
#define _UART_IRCTRL_IRPW_TWO                0x00000001UL                         /**< Mode TWO for UART_IRCTRL */
#define _UART_IRCTRL_IRPW_THREE              0x00000002UL                         /**< Mode THREE for UART_IRCTRL */
#define _UART_IRCTRL_IRPW_FOUR               0x00000003UL                         /**< Mode FOUR for UART_IRCTRL */
#define UART_IRCTRL_IRPW_DEFAULT             (_UART_IRCTRL_IRPW_DEFAULT << 1)     /**< Shifted mode DEFAULT for UART_IRCTRL */
#define UART_IRCTRL_IRPW_ONE                 (_UART_IRCTRL_IRPW_ONE << 1)         /**< Shifted mode ONE for UART_IRCTRL */
#define UART_IRCTRL_IRPW_TWO                 (_UART_IRCTRL_IRPW_TWO << 1)         /**< Shifted mode TWO for UART_IRCTRL */
#define UART_IRCTRL_IRPW_THREE               (_UART_IRCTRL_IRPW_THREE << 1)       /**< Shifted mode THREE for UART_IRCTRL */
#define UART_IRCTRL_IRPW_FOUR                (_UART_IRCTRL_IRPW_FOUR << 1)        /**< Shifted mode FOUR for UART_IRCTRL */
#define UART_IRCTRL_IRFILT                   (0x1UL << 3)                         /**< IrDA RX Filter */
#define _UART_IRCTRL_IRFILT_SHIFT            3                                    /**< Shift value for USART_IRFILT */
#define _UART_IRCTRL_IRFILT_MASK             0x8UL                                /**< Bit mask for USART_IRFILT */
#define _UART_IRCTRL_IRFILT_DEFAULT          0x00000000UL                         /**< Mode DEFAULT for UART_IRCTRL */
#define UART_IRCTRL_IRFILT_DEFAULT           (_UART_IRCTRL_IRFILT_DEFAULT << 3)   /**< Shifted mode DEFAULT for UART_IRCTRL */
#define _UART_IRCTRL_IRPRSSEL_SHIFT          4                                    /**< Shift value for USART_IRPRSSEL */
#define _UART_IRCTRL_IRPRSSEL_MASK           0x70UL                               /**< Bit mask for USART_IRPRSSEL */
#define _UART_IRCTRL_IRPRSSEL_DEFAULT        0x00000000UL                         /**< Mode DEFAULT for UART_IRCTRL */
#define _UART_IRCTRL_IRPRSSEL_PRSCH0         0x00000000UL                         /**< Mode PRSCH0 for UART_IRCTRL */
#define _UART_IRCTRL_IRPRSSEL_PRSCH1         0x00000001UL                         /**< Mode PRSCH1 for UART_IRCTRL */
#define _UART_IRCTRL_IRPRSSEL_PRSCH2         0x00000002UL                         /**< Mode PRSCH2 for UART_IRCTRL */
#define _UART_IRCTRL_IRPRSSEL_PRSCH3         0x00000003UL                         /**< Mode PRSCH3 for UART_IRCTRL */
#define _UART_IRCTRL_IRPRSSEL_PRSCH4         0x00000004UL                         /**< Mode PRSCH4 for UART_IRCTRL */
#define _UART_IRCTRL_IRPRSSEL_PRSCH5         0x00000005UL                         /**< Mode PRSCH5 for UART_IRCTRL */
#define _UART_IRCTRL_IRPRSSEL_PRSCH6         0x00000006UL                         /**< Mode PRSCH6 for UART_IRCTRL */
#define _UART_IRCTRL_IRPRSSEL_PRSCH7         0x00000007UL                         /**< Mode PRSCH7 for UART_IRCTRL */
#define UART_IRCTRL_IRPRSSEL_DEFAULT         (_UART_IRCTRL_IRPRSSEL_DEFAULT << 4) /**< Shifted mode DEFAULT for UART_IRCTRL */
#define UART_IRCTRL_IRPRSSEL_PRSCH0          (_UART_IRCTRL_IRPRSSEL_PRSCH0 << 4)  /**< Shifted mode PRSCH0 for UART_IRCTRL */
#define UART_IRCTRL_IRPRSSEL_PRSCH1          (_UART_IRCTRL_IRPRSSEL_PRSCH1 << 4)  /**< Shifted mode PRSCH1 for UART_IRCTRL */
#define UART_IRCTRL_IRPRSSEL_PRSCH2          (_UART_IRCTRL_IRPRSSEL_PRSCH2 << 4)  /**< Shifted mode PRSCH2 for UART_IRCTRL */
#define UART_IRCTRL_IRPRSSEL_PRSCH3          (_UART_IRCTRL_IRPRSSEL_PRSCH3 << 4)  /**< Shifted mode PRSCH3 for UART_IRCTRL */
#define UART_IRCTRL_IRPRSSEL_PRSCH4          (_UART_IRCTRL_IRPRSSEL_PRSCH4 << 4)  /**< Shifted mode PRSCH4 for UART_IRCTRL */
#define UART_IRCTRL_IRPRSSEL_PRSCH5          (_UART_IRCTRL_IRPRSSEL_PRSCH5 << 4)  /**< Shifted mode PRSCH5 for UART_IRCTRL */
#define UART_IRCTRL_IRPRSSEL_PRSCH6          (_UART_IRCTRL_IRPRSSEL_PRSCH6 << 4)  /**< Shifted mode PRSCH6 for UART_IRCTRL */
#define UART_IRCTRL_IRPRSSEL_PRSCH7          (_UART_IRCTRL_IRPRSSEL_PRSCH7 << 4)  /**< Shifted mode PRSCH7 for UART_IRCTRL */
#define UART_IRCTRL_IRPRSEN                  (0x1UL << 7)                         /**< IrDA PRS Channel Enable */
#define _UART_IRCTRL_IRPRSEN_SHIFT           7                                    /**< Shift value for USART_IRPRSEN */
#define _UART_IRCTRL_IRPRSEN_MASK            0x80UL                               /**< Bit mask for USART_IRPRSEN */
#define _UART_IRCTRL_IRPRSEN_DEFAULT         0x00000000UL                         /**< Mode DEFAULT for UART_IRCTRL */
#define UART_IRCTRL_IRPRSEN_DEFAULT          (_UART_IRCTRL_IRPRSEN_DEFAULT << 7)  /**< Shifted mode DEFAULT for UART_IRCTRL */

/* Bit fields for UART ROUTE */
#define _UART_ROUTE_RESETVALUE               0x00000000UL                        /**< Default value for UART_ROUTE */
#define _UART_ROUTE_MASK                     0x0000070FUL                        /**< Mask for UART_ROUTE */
#define UART_ROUTE_RXPEN                     (0x1UL << 0)                        /**< RX Pin Enable */
#define _UART_ROUTE_RXPEN_SHIFT              0                                   /**< Shift value for USART_RXPEN */
#define _UART_ROUTE_RXPEN_MASK               0x1UL                               /**< Bit mask for USART_RXPEN */
#define _UART_ROUTE_RXPEN_DEFAULT            0x00000000UL                        /**< Mode DEFAULT for UART_ROUTE */
#define UART_ROUTE_RXPEN_DEFAULT             (_UART_ROUTE_RXPEN_DEFAULT << 0)    /**< Shifted mode DEFAULT for UART_ROUTE */
#define UART_ROUTE_TXPEN                     (0x1UL << 1)                        /**< TX Pin Enable */
#define _UART_ROUTE_TXPEN_SHIFT              1                                   /**< Shift value for USART_TXPEN */
#define _UART_ROUTE_TXPEN_MASK               0x2UL                               /**< Bit mask for USART_TXPEN */
#define _UART_ROUTE_TXPEN_DEFAULT            0x00000000UL                        /**< Mode DEFAULT for UART_ROUTE */
#define UART_ROUTE_TXPEN_DEFAULT             (_UART_ROUTE_TXPEN_DEFAULT << 1)    /**< Shifted mode DEFAULT for UART_ROUTE */
#define UART_ROUTE_CSPEN                     (0x1UL << 2)                        /**< CS Pin Enable */
#define _UART_ROUTE_CSPEN_SHIFT              2                                   /**< Shift value for USART_CSPEN */
#define _UART_ROUTE_CSPEN_MASK               0x4UL                               /**< Bit mask for USART_CSPEN */
#define _UART_ROUTE_CSPEN_DEFAULT            0x00000000UL                        /**< Mode DEFAULT for UART_ROUTE */
#define UART_ROUTE_CSPEN_DEFAULT             (_UART_ROUTE_CSPEN_DEFAULT << 2)    /**< Shifted mode DEFAULT for UART_ROUTE */
#define UART_ROUTE_CLKPEN                    (0x1UL << 3)                        /**< CLK Pin Enable */
#define _UART_ROUTE_CLKPEN_SHIFT             3                                   /**< Shift value for USART_CLKPEN */
#define _UART_ROUTE_CLKPEN_MASK              0x8UL                               /**< Bit mask for USART_CLKPEN */
#define _UART_ROUTE_CLKPEN_DEFAULT           0x00000000UL                        /**< Mode DEFAULT for UART_ROUTE */
#define UART_ROUTE_CLKPEN_DEFAULT            (_UART_ROUTE_CLKPEN_DEFAULT << 3)   /**< Shifted mode DEFAULT for UART_ROUTE */
#define _UART_ROUTE_LOCATION_SHIFT           8                                   /**< Shift value for USART_LOCATION */
#define _UART_ROUTE_LOCATION_MASK            0x700UL                             /**< Bit mask for USART_LOCATION */
#define _UART_ROUTE_LOCATION_LOC0            0x00000000UL                        /**< Mode LOC0 for UART_ROUTE */
#define _UART_ROUTE_LOCATION_DEFAULT         0x00000000UL                        /**< Mode DEFAULT for UART_ROUTE */
#define _UART_ROUTE_LOCATION_LOC1            0x00000001UL                        /**< Mode LOC1 for UART_ROUTE */
#define _UART_ROUTE_LOCATION_LOC2            0x00000002UL                        /**< Mode LOC2 for UART_ROUTE */
#define _UART_ROUTE_LOCATION_LOC3            0x00000003UL                        /**< Mode LOC3 for UART_ROUTE */
#define _UART_ROUTE_LOCATION_LOC4            0x00000004UL                        /**< Mode LOC4 for UART_ROUTE */
#define _UART_ROUTE_LOCATION_LOC5            0x00000005UL                        /**< Mode LOC5 for UART_ROUTE */
#define UART_ROUTE_LOCATION_LOC0             (_UART_ROUTE_LOCATION_LOC0 << 8)    /**< Shifted mode LOC0 for UART_ROUTE */
#define UART_ROUTE_LOCATION_DEFAULT          (_UART_ROUTE_LOCATION_DEFAULT << 8) /**< Shifted mode DEFAULT for UART_ROUTE */
#define UART_ROUTE_LOCATION_LOC1             (_UART_ROUTE_LOCATION_LOC1 << 8)    /**< Shifted mode LOC1 for UART_ROUTE */
#define UART_ROUTE_LOCATION_LOC2             (_UART_ROUTE_LOCATION_LOC2 << 8)    /**< Shifted mode LOC2 for UART_ROUTE */
#define UART_ROUTE_LOCATION_LOC3             (_UART_ROUTE_LOCATION_LOC3 << 8)    /**< Shifted mode LOC3 for UART_ROUTE */
#define UART_ROUTE_LOCATION_LOC4             (_UART_ROUTE_LOCATION_LOC4 << 8)    /**< Shifted mode LOC4 for UART_ROUTE */
#define UART_ROUTE_LOCATION_LOC5             (_UART_ROUTE_LOCATION_LOC5 << 8)    /**< Shifted mode LOC5 for UART_ROUTE */

/* Bit fields for UART INPUT */
#define _UART_INPUT_RESETVALUE               0x00000000UL                        /**< Default value for UART_INPUT */
#define _UART_INPUT_MASK                     0x0000001FUL                        /**< Mask for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_SHIFT           0                                   /**< Shift value for USART_RXPRSSEL */
#define _UART_INPUT_RXPRSSEL_MASK            0xFUL                               /**< Bit mask for USART_RXPRSSEL */
#define _UART_INPUT_RXPRSSEL_DEFAULT         0x00000000UL                        /**< Mode DEFAULT for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH0          0x00000000UL                        /**< Mode PRSCH0 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH1          0x00000001UL                        /**< Mode PRSCH1 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH2          0x00000002UL                        /**< Mode PRSCH2 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH3          0x00000003UL                        /**< Mode PRSCH3 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH4          0x00000004UL                        /**< Mode PRSCH4 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH5          0x00000005UL                        /**< Mode PRSCH5 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH6          0x00000006UL                        /**< Mode PRSCH6 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH7          0x00000007UL                        /**< Mode PRSCH7 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH8          0x00000008UL                        /**< Mode PRSCH8 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH9          0x00000009UL                        /**< Mode PRSCH9 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH10         0x0000000AUL                        /**< Mode PRSCH10 for UART_INPUT */
#define _UART_INPUT_RXPRSSEL_PRSCH11         0x0000000BUL                        /**< Mode PRSCH11 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_DEFAULT          (_UART_INPUT_RXPRSSEL_DEFAULT << 0) /**< Shifted mode DEFAULT for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH0           (_UART_INPUT_RXPRSSEL_PRSCH0 << 0)  /**< Shifted mode PRSCH0 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH1           (_UART_INPUT_RXPRSSEL_PRSCH1 << 0)  /**< Shifted mode PRSCH1 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH2           (_UART_INPUT_RXPRSSEL_PRSCH2 << 0)  /**< Shifted mode PRSCH2 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH3           (_UART_INPUT_RXPRSSEL_PRSCH3 << 0)  /**< Shifted mode PRSCH3 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH4           (_UART_INPUT_RXPRSSEL_PRSCH4 << 0)  /**< Shifted mode PRSCH4 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH5           (_UART_INPUT_RXPRSSEL_PRSCH5 << 0)  /**< Shifted mode PRSCH5 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH6           (_UART_INPUT_RXPRSSEL_PRSCH6 << 0)  /**< Shifted mode PRSCH6 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH7           (_UART_INPUT_RXPRSSEL_PRSCH7 << 0)  /**< Shifted mode PRSCH7 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH8           (_UART_INPUT_RXPRSSEL_PRSCH8 << 0)  /**< Shifted mode PRSCH8 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH9           (_UART_INPUT_RXPRSSEL_PRSCH9 << 0)  /**< Shifted mode PRSCH9 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH10          (_UART_INPUT_RXPRSSEL_PRSCH10 << 0) /**< Shifted mode PRSCH10 for UART_INPUT */
#define UART_INPUT_RXPRSSEL_PRSCH11          (_UART_INPUT_RXPRSSEL_PRSCH11 << 0) /**< Shifted mode PRSCH11 for UART_INPUT */
#define UART_INPUT_RXPRS                     (0x1UL << 4)                        /**< PRS RX Enable */
#define _UART_INPUT_RXPRS_SHIFT              4                                   /**< Shift value for USART_RXPRS */
#define _UART_INPUT_RXPRS_MASK               0x10UL                              /**< Bit mask for USART_RXPRS */
#define _UART_INPUT_RXPRS_DEFAULT            0x00000000UL                        /**< Mode DEFAULT for UART_INPUT */
#define UART_INPUT_RXPRS_DEFAULT             (_UART_INPUT_RXPRS_DEFAULT << 4)    /**< Shifted mode DEFAULT for UART_INPUT */

/* Bit fields for UART I2SCTRL */
#define _UART_I2SCTRL_RESETVALUE             0x00000000UL                          /**< Default value for UART_I2SCTRL */
#define _UART_I2SCTRL_MASK                   0x0000071FUL                          /**< Mask for UART_I2SCTRL */
#define UART_I2SCTRL_EN                      (0x1UL << 0)                          /**< Enable I2S Mode */
#define _UART_I2SCTRL_EN_SHIFT               0                                     /**< Shift value for USART_EN */
#define _UART_I2SCTRL_EN_MASK                0x1UL                                 /**< Bit mask for USART_EN */
#define _UART_I2SCTRL_EN_DEFAULT             0x00000000UL                          /**< Mode DEFAULT for UART_I2SCTRL */
#define UART_I2SCTRL_EN_DEFAULT              (_UART_I2SCTRL_EN_DEFAULT << 0)       /**< Shifted mode DEFAULT for UART_I2SCTRL */
#define UART_I2SCTRL_MONO                    (0x1UL << 1)                          /**< Stero or Mono */
#define _UART_I2SCTRL_MONO_SHIFT             1                                     /**< Shift value for USART_MONO */
#define _UART_I2SCTRL_MONO_MASK              0x2UL                                 /**< Bit mask for USART_MONO */
#define _UART_I2SCTRL_MONO_DEFAULT           0x00000000UL                          /**< Mode DEFAULT for UART_I2SCTRL */
#define UART_I2SCTRL_MONO_DEFAULT            (_UART_I2SCTRL_MONO_DEFAULT << 1)     /**< Shifted mode DEFAULT for UART_I2SCTRL */
#define UART_I2SCTRL_JUSTIFY                 (0x1UL << 2)                          /**< Justification of I2S Data */
#define _UART_I2SCTRL_JUSTIFY_SHIFT          2                                     /**< Shift value for USART_JUSTIFY */
#define _UART_I2SCTRL_JUSTIFY_MASK           0x4UL                                 /**< Bit mask for USART_JUSTIFY */
#define _UART_I2SCTRL_JUSTIFY_DEFAULT        0x00000000UL                          /**< Mode DEFAULT for UART_I2SCTRL */
#define _UART_I2SCTRL_JUSTIFY_LEFT           0x00000000UL                          /**< Mode LEFT for UART_I2SCTRL */
#define _UART_I2SCTRL_JUSTIFY_RIGHT          0x00000001UL                          /**< Mode RIGHT for UART_I2SCTRL */
#define UART_I2SCTRL_JUSTIFY_DEFAULT         (_UART_I2SCTRL_JUSTIFY_DEFAULT << 2)  /**< Shifted mode DEFAULT for UART_I2SCTRL */
#define UART_I2SCTRL_JUSTIFY_LEFT            (_UART_I2SCTRL_JUSTIFY_LEFT << 2)     /**< Shifted mode LEFT for UART_I2SCTRL */
#define UART_I2SCTRL_JUSTIFY_RIGHT           (_UART_I2SCTRL_JUSTIFY_RIGHT << 2)    /**< Shifted mode RIGHT for UART_I2SCTRL */
#define UART_I2SCTRL_DMASPLIT                (0x1UL << 3)                          /**< Separate DMA Request For Left/Right Data */
#define _UART_I2SCTRL_DMASPLIT_SHIFT         3                                     /**< Shift value for USART_DMASPLIT */
#define _UART_I2SCTRL_DMASPLIT_MASK          0x8UL                                 /**< Bit mask for USART_DMASPLIT */
#define _UART_I2SCTRL_DMASPLIT_DEFAULT       0x00000000UL                          /**< Mode DEFAULT for UART_I2SCTRL */
#define UART_I2SCTRL_DMASPLIT_DEFAULT        (_UART_I2SCTRL_DMASPLIT_DEFAULT << 3) /**< Shifted mode DEFAULT for UART_I2SCTRL */
#define UART_I2SCTRL_DELAY                   (0x1UL << 4)                          /**< Delay on I2S data */
#define _UART_I2SCTRL_DELAY_SHIFT            4                                     /**< Shift value for USART_DELAY */
#define _UART_I2SCTRL_DELAY_MASK             0x10UL                                /**< Bit mask for USART_DELAY */
#define _UART_I2SCTRL_DELAY_DEFAULT          0x00000000UL                          /**< Mode DEFAULT for UART_I2SCTRL */
#define UART_I2SCTRL_DELAY_DEFAULT           (_UART_I2SCTRL_DELAY_DEFAULT << 4)    /**< Shifted mode DEFAULT for UART_I2SCTRL */
#define _UART_I2SCTRL_FORMAT_SHIFT           8                                     /**< Shift value for USART_FORMAT */
#define _UART_I2SCTRL_FORMAT_MASK            0x700UL                               /**< Bit mask for USART_FORMAT */
#define _UART_I2SCTRL_FORMAT_DEFAULT         0x00000000UL                          /**< Mode DEFAULT for UART_I2SCTRL */
#define _UART_I2SCTRL_FORMAT_W32D32          0x00000000UL                          /**< Mode W32D32 for UART_I2SCTRL */
#define _UART_I2SCTRL_FORMAT_W32D24M         0x00000001UL                          /**< Mode W32D24M for UART_I2SCTRL */
#define _UART_I2SCTRL_FORMAT_W32D24          0x00000002UL                          /**< Mode W32D24 for UART_I2SCTRL */
#define _UART_I2SCTRL_FORMAT_W32D16          0x00000003UL                          /**< Mode W32D16 for UART_I2SCTRL */
#define _UART_I2SCTRL_FORMAT_W32D8           0x00000004UL                          /**< Mode W32D8 for UART_I2SCTRL */
#define _UART_I2SCTRL_FORMAT_W16D16          0x00000005UL                          /**< Mode W16D16 for UART_I2SCTRL */
#define _UART_I2SCTRL_FORMAT_W16D8           0x00000006UL                          /**< Mode W16D8 for UART_I2SCTRL */
#define _UART_I2SCTRL_FORMAT_W8D8            0x00000007UL                          /**< Mode W8D8 for UART_I2SCTRL */
#define UART_I2SCTRL_FORMAT_DEFAULT          (_UART_I2SCTRL_FORMAT_DEFAULT << 8)   /**< Shifted mode DEFAULT for UART_I2SCTRL */
#define UART_I2SCTRL_FORMAT_W32D32           (_UART_I2SCTRL_FORMAT_W32D32 << 8)    /**< Shifted mode W32D32 for UART_I2SCTRL */
#define UART_I2SCTRL_FORMAT_W32D24M          (_UART_I2SCTRL_FORMAT_W32D24M << 8)   /**< Shifted mode W32D24M for UART_I2SCTRL */
#define UART_I2SCTRL_FORMAT_W32D24           (_UART_I2SCTRL_FORMAT_W32D24 << 8)    /**< Shifted mode W32D24 for UART_I2SCTRL */
#define UART_I2SCTRL_FORMAT_W32D16           (_UART_I2SCTRL_FORMAT_W32D16 << 8)    /**< Shifted mode W32D16 for UART_I2SCTRL */
#define UART_I2SCTRL_FORMAT_W32D8            (_UART_I2SCTRL_FORMAT_W32D8 << 8)     /**< Shifted mode W32D8 for UART_I2SCTRL */
#define UART_I2SCTRL_FORMAT_W16D16           (_UART_I2SCTRL_FORMAT_W16D16 << 8)    /**< Shifted mode W16D16 for UART_I2SCTRL */
#define UART_I2SCTRL_FORMAT_W16D8            (_UART_I2SCTRL_FORMAT_W16D8 << 8)     /**< Shifted mode W16D8 for UART_I2SCTRL */
#define UART_I2SCTRL_FORMAT_W8D8             (_UART_I2SCTRL_FORMAT_W8D8 << 8)      /**< Shifted mode W8D8 for UART_I2SCTRL */

/** @} End of group EFM32LG990F256_UART */



/**************************************************************************//**
 * @defgroup EFM32LG990F256_EMU_BitFields  EFM32LG990F256_EMU Bit Fields
 * @{
 *****************************************************************************/

/* Bit fields for EMU CTRL */
#define _EMU_CTRL_RESETVALUE                 0x00000000UL                      /**< Default value for EMU_CTRL */
#define _EMU_CTRL_MASK                       0x0000000FUL                      /**< Mask for EMU_CTRL */
#define EMU_CTRL_EMVREG                      (0x1UL << 0)                      /**< Energy Mode Voltage Regulator Control */
#define _EMU_CTRL_EMVREG_SHIFT               0                                 /**< Shift value for EMU_EMVREG */
#define _EMU_CTRL_EMVREG_MASK                0x1UL                             /**< Bit mask for EMU_EMVREG */
#define _EMU_CTRL_EMVREG_DEFAULT             0x00000000UL                      /**< Mode DEFAULT for EMU_CTRL */
#define _EMU_CTRL_EMVREG_REDUCED             0x00000000UL                      /**< Mode REDUCED for EMU_CTRL */
#define _EMU_CTRL_EMVREG_FULL                0x00000001UL                      /**< Mode FULL for EMU_CTRL */
#define EMU_CTRL_EMVREG_DEFAULT              (_EMU_CTRL_EMVREG_DEFAULT << 0)   /**< Shifted mode DEFAULT for EMU_CTRL */
#define EMU_CTRL_EMVREG_REDUCED              (_EMU_CTRL_EMVREG_REDUCED << 0)   /**< Shifted mode REDUCED for EMU_CTRL */
#define EMU_CTRL_EMVREG_FULL                 (_EMU_CTRL_EMVREG_FULL << 0)      /**< Shifted mode FULL for EMU_CTRL */
#define EMU_CTRL_EM2BLOCK                    (0x1UL << 1)                      /**< Energy Mode 2 Block */
#define _EMU_CTRL_EM2BLOCK_SHIFT             1                                 /**< Shift value for EMU_EM2BLOCK */
#define _EMU_CTRL_EM2BLOCK_MASK              0x2UL                             /**< Bit mask for EMU_EM2BLOCK */
#define _EMU_CTRL_EM2BLOCK_DEFAULT           0x00000000UL                      /**< Mode DEFAULT for EMU_CTRL */
#define EMU_CTRL_EM2BLOCK_DEFAULT            (_EMU_CTRL_EM2BLOCK_DEFAULT << 1) /**< Shifted mode DEFAULT for EMU_CTRL */
#define _EMU_CTRL_EM4CTRL_SHIFT              2                                 /**< Shift value for EMU_EM4CTRL */
#define _EMU_CTRL_EM4CTRL_MASK               0xCUL                             /**< Bit mask for EMU_EM4CTRL */
#define _EMU_CTRL_EM4CTRL_DEFAULT            0x00000000UL                      /**< Mode DEFAULT for EMU_CTRL */
#define EMU_CTRL_EM4CTRL_DEFAULT             (_EMU_CTRL_EM4CTRL_DEFAULT << 2)  /**< Shifted mode DEFAULT for EMU_CTRL */

/* Bit fields for EMU LOCK */
#define _EMU_LOCK_RESETVALUE                 0x00000000UL                      /**< Default value for EMU_LOCK */
#define _EMU_LOCK_MASK                       0x0000FFFFUL                      /**< Mask for EMU_LOCK */
#define _EMU_LOCK_LOCKKEY_SHIFT              0                                 /**< Shift value for EMU_LOCKKEY */
#define _EMU_LOCK_LOCKKEY_MASK               0xFFFFUL                          /**< Bit mask for EMU_LOCKKEY */
#define _EMU_LOCK_LOCKKEY_DEFAULT            0x00000000UL                      /**< Mode DEFAULT for EMU_LOCK */
#define _EMU_LOCK_LOCKKEY_LOCK               0x00000000UL                      /**< Mode LOCK for EMU_LOCK */
#define _EMU_LOCK_LOCKKEY_UNLOCKED           0x00000000UL                      /**< Mode UNLOCKED for EMU_LOCK */
#define _EMU_LOCK_LOCKKEY_LOCKED             0x00000001UL                      /**< Mode LOCKED for EMU_LOCK */
#define _EMU_LOCK_LOCKKEY_UNLOCK             0x0000ADE8UL                      /**< Mode UNLOCK for EMU_LOCK */
#define EMU_LOCK_LOCKKEY_DEFAULT             (_EMU_LOCK_LOCKKEY_DEFAULT << 0)  /**< Shifted mode DEFAULT for EMU_LOCK */
#define EMU_LOCK_LOCKKEY_LOCK                (_EMU_LOCK_LOCKKEY_LOCK << 0)     /**< Shifted mode LOCK for EMU_LOCK */
#define EMU_LOCK_LOCKKEY_UNLOCKED            (_EMU_LOCK_LOCKKEY_UNLOCKED << 0) /**< Shifted mode UNLOCKED for EMU_LOCK */
#define EMU_LOCK_LOCKKEY_LOCKED              (_EMU_LOCK_LOCKKEY_LOCKED << 0)   /**< Shifted mode LOCKED for EMU_LOCK */
#define EMU_LOCK_LOCKKEY_UNLOCK              (_EMU_LOCK_LOCKKEY_UNLOCK << 0)   /**< Shifted mode UNLOCK for EMU_LOCK */

/* Bit fields for EMU AUXCTRL */
#define _EMU_AUXCTRL_RESETVALUE              0x00000000UL                             /**< Default value for EMU_AUXCTRL */
#define _EMU_AUXCTRL_MASK                    0x00000101UL                             /**< Mask for EMU_AUXCTRL */
#define EMU_AUXCTRL_HRCCLR                   (0x1UL << 0)                             /**< Hard Reset Cause Clear */
#define _EMU_AUXCTRL_HRCCLR_SHIFT            0                                        /**< Shift value for EMU_HRCCLR */
#define _EMU_AUXCTRL_HRCCLR_MASK             0x1UL                                    /**< Bit mask for EMU_HRCCLR */
#define _EMU_AUXCTRL_HRCCLR_DEFAULT          0x00000000UL                             /**< Mode DEFAULT for EMU_AUXCTRL */
#define EMU_AUXCTRL_HRCCLR_DEFAULT           (_EMU_AUXCTRL_HRCCLR_DEFAULT << 0)       /**< Shifted mode DEFAULT for EMU_AUXCTRL */
#define EMU_AUXCTRL_REDLFXOBOOST             (0x1UL << 8)                             /**< Reduce LFXO Start-up Boost Current */
#define _EMU_AUXCTRL_REDLFXOBOOST_SHIFT      8                                        /**< Shift value for EMU_REDLFXOBOOST */
#define _EMU_AUXCTRL_REDLFXOBOOST_MASK       0x100UL                                  /**< Bit mask for EMU_REDLFXOBOOST */
#define _EMU_AUXCTRL_REDLFXOBOOST_DEFAULT    0x00000000UL                             /**< Mode DEFAULT for EMU_AUXCTRL */
#define EMU_AUXCTRL_REDLFXOBOOST_DEFAULT     (_EMU_AUXCTRL_REDLFXOBOOST_DEFAULT << 8) /**< Shifted mode DEFAULT for EMU_AUXCTRL */

/* Bit fields for EMU EM4CONF */
#define _EMU_EM4CONF_RESETVALUE              0x00000000UL                            /**< Default value for EMU_EM4CONF */
#define _EMU_EM4CONF_MASK                    0x0001001FUL                            /**< Mask for EMU_EM4CONF */
#define EMU_EM4CONF_VREGEN                   (0x1UL << 0)                            /**< EM4 voltage regulator enable */
#define _EMU_EM4CONF_VREGEN_SHIFT            0                                       /**< Shift value for EMU_VREGEN */
#define _EMU_EM4CONF_VREGEN_MASK             0x1UL                                   /**< Bit mask for EMU_VREGEN */
#define _EMU_EM4CONF_VREGEN_DEFAULT          0x00000000UL                            /**< Mode DEFAULT for EMU_EM4CONF */
#define EMU_EM4CONF_VREGEN_DEFAULT           (_EMU_EM4CONF_VREGEN_DEFAULT << 0)      /**< Shifted mode DEFAULT for EMU_EM4CONF */
#define EMU_EM4CONF_BURTCWU                  (0x1UL << 1)                            /**< Backup RTC EM4 wakeup enable */
#define _EMU_EM4CONF_BURTCWU_SHIFT           1                                       /**< Shift value for EMU_BURTCWU */
#define _EMU_EM4CONF_BURTCWU_MASK            0x2UL                                   /**< Bit mask for EMU_BURTCWU */
#define _EMU_EM4CONF_BURTCWU_DEFAULT         0x00000000UL                            /**< Mode DEFAULT for EMU_EM4CONF */
#define EMU_EM4CONF_BURTCWU_DEFAULT          (_EMU_EM4CONF_BURTCWU_DEFAULT << 1)     /**< Shifted mode DEFAULT for EMU_EM4CONF */
#define _EMU_EM4CONF_OSC_SHIFT               2                                       /**< Shift value for EMU_OSC */
#define _EMU_EM4CONF_OSC_MASK                0xCUL                                   /**< Bit mask for EMU_OSC */
#define _EMU_EM4CONF_OSC_DEFAULT             0x00000000UL                            /**< Mode DEFAULT for EMU_EM4CONF */
#define _EMU_EM4CONF_OSC_ULFRCO              0x00000000UL                            /**< Mode ULFRCO for EMU_EM4CONF */
#define _EMU_EM4CONF_OSC_LFRCO               0x00000001UL                            /**< Mode LFRCO for EMU_EM4CONF */
#define _EMU_EM4CONF_OSC_LFXO                0x00000002UL                            /**< Mode LFXO for EMU_EM4CONF */
#define EMU_EM4CONF_OSC_DEFAULT              (_EMU_EM4CONF_OSC_DEFAULT << 2)         /**< Shifted mode DEFAULT for EMU_EM4CONF */
#define EMU_EM4CONF_OSC_ULFRCO               (_EMU_EM4CONF_OSC_ULFRCO << 2)          /**< Shifted mode ULFRCO for EMU_EM4CONF */
#define EMU_EM4CONF_OSC_LFRCO                (_EMU_EM4CONF_OSC_LFRCO << 2)           /**< Shifted mode LFRCO for EMU_EM4CONF */
#define EMU_EM4CONF_OSC_LFXO                 (_EMU_EM4CONF_OSC_LFXO << 2)            /**< Shifted mode LFXO for EMU_EM4CONF */
#define EMU_EM4CONF_BUBODRSTDIS              (0x1UL << 4)                            /**< Disable reset from Backup BOD in EM4 */
#define _EMU_EM4CONF_BUBODRSTDIS_SHIFT       4                                       /**< Shift value for EMU_BUBODRSTDIS */
#define _EMU_EM4CONF_BUBODRSTDIS_MASK        0x10UL                                  /**< Bit mask for EMU_BUBODRSTDIS */
#define _EMU_EM4CONF_BUBODRSTDIS_DEFAULT     0x00000000UL                            /**< Mode DEFAULT for EMU_EM4CONF */
#define EMU_EM4CONF_BUBODRSTDIS_DEFAULT      (_EMU_EM4CONF_BUBODRSTDIS_DEFAULT << 4) /**< Shifted mode DEFAULT for EMU_EM4CONF */
#define EMU_EM4CONF_LOCKCONF                 (0x1UL << 16)                           /**< EM4 configuration lock enable */
#define _EMU_EM4CONF_LOCKCONF_SHIFT          16                                      /**< Shift value for EMU_LOCKCONF */
#define _EMU_EM4CONF_LOCKCONF_MASK           0x10000UL                               /**< Bit mask for EMU_LOCKCONF */
#define _EMU_EM4CONF_LOCKCONF_DEFAULT        0x00000000UL                            /**< Mode DEFAULT for EMU_EM4CONF */
#define EMU_EM4CONF_LOCKCONF_DEFAULT         (_EMU_EM4CONF_LOCKCONF_DEFAULT << 16)   /**< Shifted mode DEFAULT for EMU_EM4CONF */

/* Bit fields for EMU BUCTRL */
#define _EMU_BUCTRL_RESETVALUE               0x00000000UL                      /**< Default value for EMU_BUCTRL */
#define _EMU_BUCTRL_MASK                     0x00000067UL                      /**< Mask for EMU_BUCTRL */
#define EMU_BUCTRL_EN                        (0x1UL << 0)                      /**< Enable backup mode */
#define _EMU_BUCTRL_EN_SHIFT                 0                                 /**< Shift value for EMU_EN */
#define _EMU_BUCTRL_EN_MASK                  0x1UL                             /**< Bit mask for EMU_EN */
#define _EMU_BUCTRL_EN_DEFAULT               0x00000000UL                      /**< Mode DEFAULT for EMU_BUCTRL */
#define EMU_BUCTRL_EN_DEFAULT                (_EMU_BUCTRL_EN_DEFAULT << 0)     /**< Shifted mode DEFAULT for EMU_BUCTRL */
#define EMU_BUCTRL_STATEN                    (0x1UL << 1)                      /**< Enable backup mode status export */
#define _EMU_BUCTRL_STATEN_SHIFT             1                                 /**< Shift value for EMU_STATEN */
#define _EMU_BUCTRL_STATEN_MASK              0x2UL                             /**< Bit mask for EMU_STATEN */
#define _EMU_BUCTRL_STATEN_DEFAULT           0x00000000UL                      /**< Mode DEFAULT for EMU_BUCTRL */
#define EMU_BUCTRL_STATEN_DEFAULT            (_EMU_BUCTRL_STATEN_DEFAULT << 1) /**< Shifted mode DEFAULT for EMU_BUCTRL */
#define EMU_BUCTRL_BODCAL                    (0x1UL << 2)                      /**< Enable BOD calibration mode */
#define _EMU_BUCTRL_BODCAL_SHIFT             2                                 /**< Shift value for EMU_BODCAL */
#define _EMU_BUCTRL_BODCAL_MASK              0x4UL                             /**< Bit mask for EMU_BODCAL */
#define _EMU_BUCTRL_BODCAL_DEFAULT           0x00000000UL                      /**< Mode DEFAULT for EMU_BUCTRL */
#define EMU_BUCTRL_BODCAL_DEFAULT            (_EMU_BUCTRL_BODCAL_DEFAULT << 2) /**< Shifted mode DEFAULT for EMU_BUCTRL */
#define _EMU_BUCTRL_PROBE_SHIFT              5                                 /**< Shift value for EMU_PROBE */
#define _EMU_BUCTRL_PROBE_MASK               0x60UL                            /**< Bit mask for EMU_PROBE */
#define _EMU_BUCTRL_PROBE_DEFAULT            0x00000000UL                      /**< Mode DEFAULT for EMU_BUCTRL */
#define _EMU_BUCTRL_PROBE_DISABLE            0x00000000UL                      /**< Mode DISABLE for EMU_BUCTRL */
#define _EMU_BUCTRL_PROBE_VDDDREG            0x00000001UL                      /**< Mode VDDDREG for EMU_BUCTRL */
#define _EMU_BUCTRL_PROBE_BUIN               0x00000002UL                      /**< Mode BUIN for EMU_BUCTRL */
#define _EMU_BUCTRL_PROBE_BUOUT              0x00000003UL                      /**< Mode BUOUT for EMU_BUCTRL */
#define EMU_BUCTRL_PROBE_DEFAULT             (_EMU_BUCTRL_PROBE_DEFAULT << 5)  /**< Shifted mode DEFAULT for EMU_BUCTRL */
#define EMU_BUCTRL_PROBE_DISABLE             (_EMU_BUCTRL_PROBE_DISABLE << 5)  /**< Shifted mode DISABLE for EMU_BUCTRL */
#define EMU_BUCTRL_PROBE_VDDDREG             (_EMU_BUCTRL_PROBE_VDDDREG << 5)  /**< Shifted mode VDDDREG for EMU_BUCTRL */
#define EMU_BUCTRL_PROBE_BUIN                (_EMU_BUCTRL_PROBE_BUIN << 5)     /**< Shifted mode BUIN for EMU_BUCTRL */
#define EMU_BUCTRL_PROBE_BUOUT               (_EMU_BUCTRL_PROBE_BUOUT << 5)    /**< Shifted mode BUOUT for EMU_BUCTRL */

/* Bit fields for EMU PWRCONF */
#define _EMU_PWRCONF_RESETVALUE              0x00000000UL                           /**< Default value for EMU_PWRCONF */
#define _EMU_PWRCONF_MASK                    0x0000001FUL                           /**< Mask for EMU_PWRCONF */
#define EMU_PWRCONF_VOUTWEAK                 (0x1UL << 0)                           /**< BU_VOUT weak enable */
#define _EMU_PWRCONF_VOUTWEAK_SHIFT          0                                      /**< Shift value for EMU_VOUTWEAK */
#define _EMU_PWRCONF_VOUTWEAK_MASK           0x1UL                                  /**< Bit mask for EMU_VOUTWEAK */
#define _EMU_PWRCONF_VOUTWEAK_DEFAULT        0x00000000UL                           /**< Mode DEFAULT for EMU_PWRCONF */
#define EMU_PWRCONF_VOUTWEAK_DEFAULT         (_EMU_PWRCONF_VOUTWEAK_DEFAULT << 0)   /**< Shifted mode DEFAULT for EMU_PWRCONF */
#define EMU_PWRCONF_VOUTMED                  (0x1UL << 1)                           /**< BU_VOUT medium enable */
#define _EMU_PWRCONF_VOUTMED_SHIFT           1                                      /**< Shift value for EMU_VOUTMED */
#define _EMU_PWRCONF_VOUTMED_MASK            0x2UL                                  /**< Bit mask for EMU_VOUTMED */
#define _EMU_PWRCONF_VOUTMED_DEFAULT         0x00000000UL                           /**< Mode DEFAULT for EMU_PWRCONF */
#define EMU_PWRCONF_VOUTMED_DEFAULT          (_EMU_PWRCONF_VOUTMED_DEFAULT << 1)    /**< Shifted mode DEFAULT for EMU_PWRCONF */
#define EMU_PWRCONF_VOUTSTRONG               (0x1UL << 2)                           /**< BU_VOUT strong enable */
#define _EMU_PWRCONF_VOUTSTRONG_SHIFT        2                                      /**< Shift value for EMU_VOUTSTRONG */
#define _EMU_PWRCONF_VOUTSTRONG_MASK         0x4UL                                  /**< Bit mask for EMU_VOUTSTRONG */
#define _EMU_PWRCONF_VOUTSTRONG_DEFAULT      0x00000000UL                           /**< Mode DEFAULT for EMU_PWRCONF */
#define EMU_PWRCONF_VOUTSTRONG_DEFAULT       (_EMU_PWRCONF_VOUTSTRONG_DEFAULT << 2) /**< Shifted mode DEFAULT for EMU_PWRCONF */
#define _EMU_PWRCONF_PWRRES_SHIFT            3                                      /**< Shift value for EMU_PWRRES */
#define _EMU_PWRCONF_PWRRES_MASK             0x18UL                                 /**< Bit mask for EMU_PWRRES */
#define _EMU_PWRCONF_PWRRES_DEFAULT          0x00000000UL                           /**< Mode DEFAULT for EMU_PWRCONF */
#define _EMU_PWRCONF_PWRRES_RES0             0x00000000UL                           /**< Mode RES0 for EMU_PWRCONF */
#define _EMU_PWRCONF_PWRRES_RES1             0x00000001UL                           /**< Mode RES1 for EMU_PWRCONF */
#define _EMU_PWRCONF_PWRRES_RES2             0x00000002UL                           /**< Mode RES2 for EMU_PWRCONF */
#define _EMU_PWRCONF_PWRRES_RES3             0x00000003UL                           /**< Mode RES3 for EMU_PWRCONF */
#define EMU_PWRCONF_PWRRES_DEFAULT           (_EMU_PWRCONF_PWRRES_DEFAULT << 3)     /**< Shifted mode DEFAULT for EMU_PWRCONF */
#define EMU_PWRCONF_PWRRES_RES0              (_EMU_PWRCONF_PWRRES_RES0 << 3)        /**< Shifted mode RES0 for EMU_PWRCONF */
#define EMU_PWRCONF_PWRRES_RES1              (_EMU_PWRCONF_PWRRES_RES1 << 3)        /**< Shifted mode RES1 for EMU_PWRCONF */
#define EMU_PWRCONF_PWRRES_RES2              (_EMU_PWRCONF_PWRRES_RES2 << 3)        /**< Shifted mode RES2 for EMU_PWRCONF */
#define EMU_PWRCONF_PWRRES_RES3              (_EMU_PWRCONF_PWRRES_RES3 << 3)        /**< Shifted mode RES3 for EMU_PWRCONF */

/* Bit fields for EMU BUINACT */
#define _EMU_BUINACT_RESETVALUE              0x0000000BUL                          /**< Default value for EMU_BUINACT */
#define _EMU_BUINACT_MASK                    0x0000007FUL                          /**< Mask for EMU_BUINACT */
#define _EMU_BUINACT_BUENTHRES_SHIFT         0                                     /**< Shift value for EMU_BUENTHRES */
#define _EMU_BUINACT_BUENTHRES_MASK          0x7UL                                 /**< Bit mask for EMU_BUENTHRES */
#define _EMU_BUINACT_BUENTHRES_DEFAULT       0x00000003UL                          /**< Mode DEFAULT for EMU_BUINACT */
#define EMU_BUINACT_BUENTHRES_DEFAULT        (_EMU_BUINACT_BUENTHRES_DEFAULT << 0) /**< Shifted mode DEFAULT for EMU_BUINACT */
#define _EMU_BUINACT_BUENRANGE_SHIFT         3                                     /**< Shift value for EMU_BUENRANGE */
#define _EMU_BUINACT_BUENRANGE_MASK          0x18UL                                /**< Bit mask for EMU_BUENRANGE */
#define _EMU_BUINACT_BUENRANGE_DEFAULT       0x00000001UL                          /**< Mode DEFAULT for EMU_BUINACT */
#define EMU_BUINACT_BUENRANGE_DEFAULT        (_EMU_BUINACT_BUENRANGE_DEFAULT << 3) /**< Shifted mode DEFAULT for EMU_BUINACT */
#define _EMU_BUINACT_PWRCON_SHIFT            5                                     /**< Shift value for EMU_PWRCON */
#define _EMU_BUINACT_PWRCON_MASK             0x60UL                                /**< Bit mask for EMU_PWRCON */
#define _EMU_BUINACT_PWRCON_DEFAULT          0x00000000UL                          /**< Mode DEFAULT for EMU_BUINACT */
#define _EMU_BUINACT_PWRCON_NONE             0x00000000UL                          /**< Mode NONE for EMU_BUINACT */
#define _EMU_BUINACT_PWRCON_BUMAIN           0x00000001UL                          /**< Mode BUMAIN for EMU_BUINACT */
#define _EMU_BUINACT_PWRCON_MAINBU           0x00000002UL                          /**< Mode MAINBU for EMU_BUINACT */
#define _EMU_BUINACT_PWRCON_NODIODE          0x00000003UL                          /**< Mode NODIODE for EMU_BUINACT */
#define EMU_BUINACT_PWRCON_DEFAULT           (_EMU_BUINACT_PWRCON_DEFAULT << 5)    /**< Shifted mode DEFAULT for EMU_BUINACT */
#define EMU_BUINACT_PWRCON_NONE              (_EMU_BUINACT_PWRCON_NONE << 5)       /**< Shifted mode NONE for EMU_BUINACT */
#define EMU_BUINACT_PWRCON_BUMAIN            (_EMU_BUINACT_PWRCON_BUMAIN << 5)     /**< Shifted mode BUMAIN for EMU_BUINACT */
#define EMU_BUINACT_PWRCON_MAINBU            (_EMU_BUINACT_PWRCON_MAINBU << 5)     /**< Shifted mode MAINBU for EMU_BUINACT */
#define EMU_BUINACT_PWRCON_NODIODE           (_EMU_BUINACT_PWRCON_NODIODE << 5)    /**< Shifted mode NODIODE for EMU_BUINACT */

/* Bit fields for EMU BUACT */
#define _EMU_BUACT_RESETVALUE                0x0000000BUL                        /**< Default value for EMU_BUACT */
#define _EMU_BUACT_MASK                      0x0000007FUL                        /**< Mask for EMU_BUACT */
#define _EMU_BUACT_BUEXTHRES_SHIFT           0                                   /**< Shift value for EMU_BUEXTHRES */
#define _EMU_BUACT_BUEXTHRES_MASK            0x7UL                               /**< Bit mask for EMU_BUEXTHRES */
#define _EMU_BUACT_BUEXTHRES_DEFAULT         0x00000003UL                        /**< Mode DEFAULT for EMU_BUACT */
#define EMU_BUACT_BUEXTHRES_DEFAULT          (_EMU_BUACT_BUEXTHRES_DEFAULT << 0) /**< Shifted mode DEFAULT for EMU_BUACT */
#define _EMU_BUACT_BUEXRANGE_SHIFT           3                                   /**< Shift value for EMU_BUEXRANGE */
#define _EMU_BUACT_BUEXRANGE_MASK            0x18UL                              /**< Bit mask for EMU_BUEXRANGE */
#define _EMU_BUACT_BUEXRANGE_DEFAULT         0x00000001UL                        /**< Mode DEFAULT for EMU_BUACT */
#define EMU_BUACT_BUEXRANGE_DEFAULT          (_EMU_BUACT_BUEXRANGE_DEFAULT << 3) /**< Shifted mode DEFAULT for EMU_BUACT */
#define _EMU_BUACT_PWRCON_SHIFT              5                                   /**< Shift value for EMU_PWRCON */
#define _EMU_BUACT_PWRCON_MASK               0x60UL                              /**< Bit mask for EMU_PWRCON */
#define _EMU_BUACT_PWRCON_DEFAULT            0x00000000UL                        /**< Mode DEFAULT for EMU_BUACT */
#define _EMU_BUACT_PWRCON_NONE               0x00000000UL                        /**< Mode NONE for EMU_BUACT */
#define _EMU_BUACT_PWRCON_BUMAIN             0x00000001UL                        /**< Mode BUMAIN for EMU_BUACT */
#define _EMU_BUACT_PWRCON_MAINBU             0x00000002UL                        /**< Mode MAINBU for EMU_BUACT */
#define _EMU_BUACT_PWRCON_NODIODE            0x00000003UL                        /**< Mode NODIODE for EMU_BUACT */
#define EMU_BUACT_PWRCON_DEFAULT             (_EMU_BUACT_PWRCON_DEFAULT << 5)    /**< Shifted mode DEFAULT for EMU_BUACT */
#define EMU_BUACT_PWRCON_NONE                (_EMU_BUACT_PWRCON_NONE << 5)       /**< Shifted mode NONE for EMU_BUACT */
#define EMU_BUACT_PWRCON_BUMAIN              (_EMU_BUACT_PWRCON_BUMAIN << 5)     /**< Shifted mode BUMAIN for EMU_BUACT */
#define EMU_BUACT_PWRCON_MAINBU              (_EMU_BUACT_PWRCON_MAINBU << 5)     /**< Shifted mode MAINBU for EMU_BUACT */
#define EMU_BUACT_PWRCON_NODIODE             (_EMU_BUACT_PWRCON_NODIODE << 5)    /**< Shifted mode NODIODE for EMU_BUACT */

/* Bit fields for EMU STATUS */
#define _EMU_STATUS_RESETVALUE               0x00000000UL                     /**< Default value for EMU_STATUS */
#define _EMU_STATUS_MASK                     0x00000001UL                     /**< Mask for EMU_STATUS */
#define EMU_STATUS_BURDY                     (0x1UL << 0)                     /**< Backup mode ready */
#define _EMU_STATUS_BURDY_SHIFT              0                                /**< Shift value for EMU_BURDY */
#define _EMU_STATUS_BURDY_MASK               0x1UL                            /**< Bit mask for EMU_BURDY */
#define _EMU_STATUS_BURDY_DEFAULT            0x00000000UL                     /**< Mode DEFAULT for EMU_STATUS */
#define EMU_STATUS_BURDY_DEFAULT             (_EMU_STATUS_BURDY_DEFAULT << 0) /**< Shifted mode DEFAULT for EMU_STATUS */

/* Bit fields for EMU ROUTE */
#define _EMU_ROUTE_RESETVALUE                0x00000001UL                       /**< Default value for EMU_ROUTE */
#define _EMU_ROUTE_MASK                      0x00000001UL                       /**< Mask for EMU_ROUTE */
#define EMU_ROUTE_BUVINPEN                   (0x1UL << 0)                       /**< BU_VIN Pin Enable */
#define _EMU_ROUTE_BUVINPEN_SHIFT            0                                  /**< Shift value for EMU_BUVINPEN */
#define _EMU_ROUTE_BUVINPEN_MASK             0x1UL                              /**< Bit mask for EMU_BUVINPEN */
#define _EMU_ROUTE_BUVINPEN_DEFAULT          0x00000001UL                       /**< Mode DEFAULT for EMU_ROUTE */
#define EMU_ROUTE_BUVINPEN_DEFAULT           (_EMU_ROUTE_BUVINPEN_DEFAULT << 0) /**< Shifted mode DEFAULT for EMU_ROUTE */

/* Bit fields for EMU IF */
#define _EMU_IF_RESETVALUE                   0x00000000UL                 /**< Default value for EMU_IF */
#define _EMU_IF_MASK                         0x00000001UL                 /**< Mask for EMU_IF */
#define EMU_IF_BURDY                         (0x1UL << 0)                 /**< Backup functionality ready Interrupt Flag */
#define _EMU_IF_BURDY_SHIFT                  0                            /**< Shift value for EMU_BURDY */
#define _EMU_IF_BURDY_MASK                   0x1UL                        /**< Bit mask for EMU_BURDY */
#define _EMU_IF_BURDY_DEFAULT                0x00000000UL                 /**< Mode DEFAULT for EMU_IF */
#define EMU_IF_BURDY_DEFAULT                 (_EMU_IF_BURDY_DEFAULT << 0) /**< Shifted mode DEFAULT for EMU_IF */

/* Bit fields for EMU IFS */
#define _EMU_IFS_RESETVALUE                  0x00000000UL                  /**< Default value for EMU_IFS */
#define _EMU_IFS_MASK                        0x00000001UL                  /**< Mask for EMU_IFS */
#define EMU_IFS_BURDY                        (0x1UL << 0)                  /**< Set Backup functionality ready Interrupt Flag */
#define _EMU_IFS_BURDY_SHIFT                 0                             /**< Shift value for EMU_BURDY */
#define _EMU_IFS_BURDY_MASK                  0x1UL                         /**< Bit mask for EMU_BURDY */
#define _EMU_IFS_BURDY_DEFAULT               0x00000000UL                  /**< Mode DEFAULT for EMU_IFS */
#define EMU_IFS_BURDY_DEFAULT                (_EMU_IFS_BURDY_DEFAULT << 0) /**< Shifted mode DEFAULT for EMU_IFS */

/* Bit fields for EMU IFC */
#define _EMU_IFC_RESETVALUE                  0x00000000UL                  /**< Default value for EMU_IFC */
#define _EMU_IFC_MASK                        0x00000001UL                  /**< Mask for EMU_IFC */
#define EMU_IFC_BURDY                        (0x1UL << 0)                  /**< Clear Backup functionality ready Interrupt Flag */
#define _EMU_IFC_BURDY_SHIFT                 0                             /**< Shift value for EMU_BURDY */
#define _EMU_IFC_BURDY_MASK                  0x1UL                         /**< Bit mask for EMU_BURDY */
#define _EMU_IFC_BURDY_DEFAULT               0x00000000UL                  /**< Mode DEFAULT for EMU_IFC */
#define EMU_IFC_BURDY_DEFAULT                (_EMU_IFC_BURDY_DEFAULT << 0) /**< Shifted mode DEFAULT for EMU_IFC */

/* Bit fields for EMU IEN */
#define _EMU_IEN_RESETVALUE                  0x00000000UL                  /**< Default value for EMU_IEN */
#define _EMU_IEN_MASK                        0x00000001UL                  /**< Mask for EMU_IEN */
#define EMU_IEN_BURDY                        (0x1UL << 0)                  /**< Backup functionality ready Interrupt Enable */
#define _EMU_IEN_BURDY_SHIFT                 0                             /**< Shift value for EMU_BURDY */
#define _EMU_IEN_BURDY_MASK                  0x1UL                         /**< Bit mask for EMU_BURDY */
#define _EMU_IEN_BURDY_DEFAULT               0x00000000UL                  /**< Mode DEFAULT for EMU_IEN */
#define EMU_IEN_BURDY_DEFAULT                (_EMU_IEN_BURDY_DEFAULT << 0) /**< Shifted mode DEFAULT for EMU_IEN */

/* Bit fields for EMU BUBODBUVINCAL */
#define _EMU_BUBODBUVINCAL_RESETVALUE        0x0000000BUL                            /**< Default value for EMU_BUBODBUVINCAL */
#define _EMU_BUBODBUVINCAL_MASK              0x0000001FUL                            /**< Mask for EMU_BUBODBUVINCAL */
#define _EMU_BUBODBUVINCAL_THRES_SHIFT       0                                       /**< Shift value for EMU_THRES */
#define _EMU_BUBODBUVINCAL_THRES_MASK        0x7UL                                   /**< Bit mask for EMU_THRES */
#define _EMU_BUBODBUVINCAL_THRES_DEFAULT     0x00000003UL                            /**< Mode DEFAULT for EMU_BUBODBUVINCAL */
#define EMU_BUBODBUVINCAL_THRES_DEFAULT      (_EMU_BUBODBUVINCAL_THRES_DEFAULT << 0) /**< Shifted mode DEFAULT for EMU_BUBODBUVINCAL */
#define _EMU_BUBODBUVINCAL_RANGE_SHIFT       3                                       /**< Shift value for EMU_RANGE */
#define _EMU_BUBODBUVINCAL_RANGE_MASK        0x18UL                                  /**< Bit mask for EMU_RANGE */
#define _EMU_BUBODBUVINCAL_RANGE_DEFAULT     0x00000001UL                            /**< Mode DEFAULT for EMU_BUBODBUVINCAL */
#define EMU_BUBODBUVINCAL_RANGE_DEFAULT      (_EMU_BUBODBUVINCAL_RANGE_DEFAULT << 3) /**< Shifted mode DEFAULT for EMU_BUBODBUVINCAL */

/* Bit fields for EMU BUBODUNREGCAL */
#define _EMU_BUBODUNREGCAL_RESETVALUE        0x0000000BUL                            /**< Default value for EMU_BUBODUNREGCAL */
#define _EMU_BUBODUNREGCAL_MASK              0x0000001FUL                            /**< Mask for EMU_BUBODUNREGCAL */
#define _EMU_BUBODUNREGCAL_THRES_SHIFT       0                                       /**< Shift value for EMU_THRES */
#define _EMU_BUBODUNREGCAL_THRES_MASK        0x7UL                                   /**< Bit mask for EMU_THRES */
#define _EMU_BUBODUNREGCAL_THRES_DEFAULT     0x00000003UL                            /**< Mode DEFAULT for EMU_BUBODUNREGCAL */
#define EMU_BUBODUNREGCAL_THRES_DEFAULT      (_EMU_BUBODUNREGCAL_THRES_DEFAULT << 0) /**< Shifted mode DEFAULT for EMU_BUBODUNREGCAL */
#define _EMU_BUBODUNREGCAL_RANGE_SHIFT       3                                       /**< Shift value for EMU_RANGE */
#define _EMU_BUBODUNREGCAL_RANGE_MASK        0x18UL                                  /**< Bit mask for EMU_RANGE */
#define _EMU_BUBODUNREGCAL_RANGE_DEFAULT     0x00000001UL                            /**< Mode DEFAULT for EMU_BUBODUNREGCAL */
#define EMU_BUBODUNREGCAL_RANGE_DEFAULT      (_EMU_BUBODUNREGCAL_RANGE_DEFAULT << 3) /**< Shifted mode DEFAULT for EMU_BUBODUNREGCAL */

/** @} End of group EFM32LG990F256_EMU */



/**************************************************************************//**
 * @defgroup EFM32LG990F256_UNLOCK EFM32LG990F256 Unlock Codes
 * @{
 *****************************************************************************/
#define MSC_UNLOCK_CODE      0x1B71 /**< MSC unlock code */
#define EMU_UNLOCK_CODE      0xADE8 /**< EMU unlock code */
#define CMU_UNLOCK_CODE      0x580E /**< CMU unlock code */
#define TIMER_UNLOCK_CODE    0xCE80 /**< TIMER unlock code */
#define GPIO_UNLOCK_CODE     0xA534 /**< GPIO unlock code */
#define BURTC_UNLOCK_CODE    0xAEE8 /**< BURTC unlock code */

/** @} End of group EFM32LG990F256_UNLOCK */

/** @} End of group EFM32LG990F256_BitFields */

/**************************************************************************//**
 * @defgroup EFM32LG990F256_Alternate_Function EFM32LG990F256 Alternate Function
 * @{
 *****************************************************************************/

#include "efm32lg_af_ports.h"
#include "efm32lg_af_pins.h"

/** @} End of group EFM32LG990F256_Alternate_Function */

/**************************************************************************//**
 *  @brief Set the value of a bit field within a register.
 *
 *  @param REG
 *       The register to update
 *  @param MASK
 *       The mask for the bit field to update
 *  @param VALUE
 *       The value to write to the bit field
 *  @param OFFSET
 *       The number of bits that the field is offset within the register.
 *       0 (zero) means LSB.
 *****************************************************************************/
#define SET_BIT_FIELD(REG, MASK, VALUE, OFFSET) \
  REG = ((REG) &~(MASK)) | (((VALUE) << (OFFSET)) & (MASK));

/** @} End of group EFM32LG990F256 */

/** @} End of group Parts */

#ifdef __cplusplus
}
#endif
#endif /* __EFM32LG990F256_H */
