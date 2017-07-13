/**************************************************************************//**
 * @file startup_efm32gg11b.c
 * @brief CMSIS Compatible EFM32GG11B startup file in C for IAR EWARM
 * @version 5.2.1
 ******************************************************************************
 * # License
 * <b>Copyright 2017 Silicon Laboratories, Inc. http://www.silabs.com</b>
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

#include "em_device.h"        /* The correct device header file. */

#pragma language=extended
#pragma segment="CSTACK"

/* IAR start function */
extern void __iar_program_start(void);
/* CMSIS init function */
extern void SystemInit(void);

/* Auto defined by linker */
extern unsigned char CSTACK$$Limit;

__weak void Reset_Handler(void)
{
  SystemInit();
  __iar_program_start();
}

__weak void NMI_Handler(void)
{
  while(1);
}

__weak void HardFault_Handler(void)
{
  while(1);
}

__weak void MemManage_Handler(void)
{
  while(1);
}

__weak void BusFault_Handler(void)
{
  while(1);
}

__weak void UsageFault_Handler(void)
{
  while(1);
}

__weak void SVC_Handler(void)
{
  while(1);
}

__weak void DebugMon_Handler(void)
{
  while(1);
}

__weak void PendSV_Handler(void)
{
  while(1);
}

__weak void SysTick_Handler(void)
{
  while(1);
}


__weak void EMU_IRQHandler(void)
{
  while(1);
}

__weak void WDOG0_IRQHandler(void)
{
  while(1);
}

__weak void LDMA_IRQHandler(void)
{
  while(1);
}

__weak void GPIO_EVEN_IRQHandler(void)
{
  while(1);
}

__weak void SMU_IRQHandler(void)
{
  while(1);
}

__weak void TIMER0_IRQHandler(void)
{
  while(1);
}

__weak void USART0_RX_IRQHandler(void)
{
  while(1);
}

__weak void USART0_TX_IRQHandler(void)
{
  while(1);
}

__weak void ACMP0_IRQHandler(void)
{
  while(1);
}

__weak void ADC0_IRQHandler(void)
{
  while(1);
}

__weak void IDAC0_IRQHandler(void)
{
  while(1);
}

__weak void I2C0_IRQHandler(void)
{
  while(1);
}

__weak void I2C1_IRQHandler(void)
{
  while(1);
}

__weak void GPIO_ODD_IRQHandler(void)
{
  while(1);
}

__weak void TIMER1_IRQHandler(void)
{
  while(1);
}

__weak void TIMER2_IRQHandler(void)
{
  while(1);
}

__weak void TIMER3_IRQHandler(void)
{
  while(1);
}

__weak void USART1_RX_IRQHandler(void)
{
  while(1);
}

__weak void USART1_TX_IRQHandler(void)
{
  while(1);
}

__weak void USART2_RX_IRQHandler(void)
{
  while(1);
}

__weak void USART2_TX_IRQHandler(void)
{
  while(1);
}

__weak void UART0_RX_IRQHandler(void)
{
  while(1);
}

__weak void UART0_TX_IRQHandler(void)
{
  while(1);
}

__weak void UART1_RX_IRQHandler(void)
{
  while(1);
}

__weak void UART1_TX_IRQHandler(void)
{
  while(1);
}

__weak void LEUART0_IRQHandler(void)
{
  while(1);
}

__weak void LEUART1_IRQHandler(void)
{
  while(1);
}

__weak void LETIMER0_IRQHandler(void)
{
  while(1);
}

__weak void PCNT0_IRQHandler(void)
{
  while(1);
}

__weak void PCNT1_IRQHandler(void)
{
  while(1);
}

__weak void PCNT2_IRQHandler(void)
{
  while(1);
}

__weak void RTCC_IRQHandler(void)
{
  while(1);
}

__weak void CMU_IRQHandler(void)
{
  while(1);
}

__weak void MSC_IRQHandler(void)
{
  while(1);
}

__weak void CRYPTO0_IRQHandler(void)
{
  while(1);
}

__weak void CRYOTIMER_IRQHandler(void)
{
  while(1);
}

__weak void FPUEH_IRQHandler(void)
{
  while(1);
}

__weak void USART3_RX_IRQHandler(void)
{
  while(1);
}

__weak void USART3_TX_IRQHandler(void)
{
  while(1);
}

__weak void USART4_RX_IRQHandler(void)
{
  while(1);
}

__weak void USART4_TX_IRQHandler(void)
{
  while(1);
}

__weak void WTIMER0_IRQHandler(void)
{
  while(1);
}

__weak void WTIMER1_IRQHandler(void)
{
  while(1);
}

__weak void WTIMER2_IRQHandler(void)
{
  while(1);
}

__weak void WTIMER3_IRQHandler(void)
{
  while(1);
}

__weak void I2C2_IRQHandler(void)
{
  while(1);
}

__weak void VDAC0_IRQHandler(void)
{
  while(1);
}

__weak void TIMER4_IRQHandler(void)
{
  while(1);
}

__weak void TIMER5_IRQHandler(void)
{
  while(1);
}

__weak void TIMER6_IRQHandler(void)
{
  while(1);
}

__weak void USART5_RX_IRQHandler(void)
{
  while(1);
}

__weak void USART5_TX_IRQHandler(void)
{
  while(1);
}

__weak void CSEN_IRQHandler(void)
{
  while(1);
}

__weak void LESENSE_IRQHandler(void)
{
  while(1);
}

__weak void EBI_IRQHandler(void)
{
  while(1);
}

__weak void ACMP2_IRQHandler(void)
{
  while(1);
}

__weak void ADC1_IRQHandler(void)
{
  while(1);
}

__weak void LCD_IRQHandler(void)
{
  while(1);
}

__weak void SDIO_IRQHandler(void)
{
  while(1);
}

__weak void ETH_IRQHandler(void)
{
  while(1);
}

__weak void CAN0_IRQHandler(void)
{
  while(1);
}

__weak void CAN1_IRQHandler(void)
{
  while(1);
}

__weak void USB_IRQHandler(void)
{
  while(1);
}

__weak void RTC_IRQHandler(void)
{
  while(1);
}

__weak void WDOG1_IRQHandler(void)
{
  while(1);
}

__weak void LETIMER1_IRQHandler(void)
{
  while(1);
}

__weak void TRNG0_IRQHandler(void)
{
  while(1);
}

__weak void QSPI0_IRQHandler(void)
{
  while(1);
}

/* With IAR, the CSTACK is defined via project options settings */
#pragma data_alignment=256
#pragma location = ".intvec"
const void * const __vector_table[]=  {
  &CSTACK$$Limit,
  (void *) Reset_Handler,           /*  1 - Reset (start instruction) */
  (void *) NMI_Handler,             /*  2 - NMI */
  (void *) HardFault_Handler,       /*  3 - HardFault */
  (void *) MemManage_Handler,
  (void *) BusFault_Handler,
  (void *) UsageFault_Handler,
  (void *) 0,
  (void *) 0,
  (void *) 0,
  (void *) 0,
  (void *) SVC_Handler,
  (void *) DebugMon_Handler,
  (void *) 0,
  (void *) PendSV_Handler,
  (void *) SysTick_Handler,
  (void *) EMU_IRQHandler,  /* 0 - EMU */
  (void *) WDOG0_IRQHandler,  /* 1 - WDOG0 */
  (void *) LDMA_IRQHandler,  /* 2 - LDMA */
  (void *) GPIO_EVEN_IRQHandler,  /* 3 - GPIO_EVEN */
  (void *) SMU_IRQHandler,  /* 4 - SMU */
  (void *) TIMER0_IRQHandler,  /* 5 - TIMER0 */
  (void *) USART0_RX_IRQHandler,  /* 6 - USART0_RX */
  (void *) USART0_TX_IRQHandler,  /* 7 - USART0_TX */
  (void *) ACMP0_IRQHandler,  /* 8 - ACMP0 */
  (void *) ADC0_IRQHandler,  /* 9 - ADC0 */
  (void *) IDAC0_IRQHandler,  /* 10 - IDAC0 */
  (void *) I2C0_IRQHandler,  /* 11 - I2C0 */
  (void *) I2C1_IRQHandler,  /* 12 - I2C1 */
  (void *) GPIO_ODD_IRQHandler,  /* 13 - GPIO_ODD */
  (void *) TIMER1_IRQHandler,  /* 14 - TIMER1 */
  (void *) TIMER2_IRQHandler,  /* 15 - TIMER2 */
  (void *) TIMER3_IRQHandler,  /* 16 - TIMER3 */
  (void *) USART1_RX_IRQHandler,  /* 17 - USART1_RX */
  (void *) USART1_TX_IRQHandler,  /* 18 - USART1_TX */
  (void *) USART2_RX_IRQHandler,  /* 19 - USART2_RX */
  (void *) USART2_TX_IRQHandler,  /* 20 - USART2_TX */
  (void *) UART0_RX_IRQHandler,  /* 21 - UART0_RX */
  (void *) UART0_TX_IRQHandler,  /* 22 - UART0_TX */
  (void *) UART1_RX_IRQHandler,  /* 23 - UART1_RX */
  (void *) UART1_TX_IRQHandler,  /* 24 - UART1_TX */
  (void *) LEUART0_IRQHandler,  /* 25 - LEUART0 */
  (void *) LEUART1_IRQHandler,  /* 26 - LEUART1 */
  (void *) LETIMER0_IRQHandler,  /* 27 - LETIMER0 */
  (void *) PCNT0_IRQHandler,  /* 28 - PCNT0 */
  (void *) PCNT1_IRQHandler,  /* 29 - PCNT1 */
  (void *) PCNT2_IRQHandler,  /* 30 - PCNT2 */
  (void *) RTCC_IRQHandler,  /* 31 - RTCC */
  (void *) CMU_IRQHandler,  /* 32 - CMU */
  (void *) MSC_IRQHandler,  /* 33 - MSC */
  (void *) CRYPTO0_IRQHandler,  /* 34 - CRYPTO0 */
  (void *) CRYOTIMER_IRQHandler,  /* 35 - CRYOTIMER */
  (void *) FPUEH_IRQHandler,  /* 36 - FPUEH */
  (void *) USART3_RX_IRQHandler,  /* 37 - USART3_RX */
  (void *) USART3_TX_IRQHandler,  /* 38 - USART3_TX */
  (void *) USART4_RX_IRQHandler,  /* 39 - USART4_RX */
  (void *) USART4_TX_IRQHandler,  /* 40 - USART4_TX */
  (void *) WTIMER0_IRQHandler,  /* 41 - WTIMER0 */
  (void *) WTIMER1_IRQHandler,  /* 42 - WTIMER1 */
  (void *) WTIMER2_IRQHandler,  /* 43 - WTIMER2 */
  (void *) WTIMER3_IRQHandler,  /* 44 - WTIMER3 */
  (void *) I2C2_IRQHandler,  /* 45 - I2C2 */
  (void *) VDAC0_IRQHandler,  /* 46 - VDAC0 */
  (void *) TIMER4_IRQHandler,  /* 47 - TIMER4 */
  (void *) TIMER5_IRQHandler,  /* 48 - TIMER5 */
  (void *) TIMER6_IRQHandler,  /* 49 - TIMER6 */
  (void *) USART5_RX_IRQHandler,  /* 50 - USART5_RX */
  (void *) USART5_TX_IRQHandler,  /* 51 - USART5_TX */
  (void *) CSEN_IRQHandler,  /* 52 - CSEN */
  (void *) LESENSE_IRQHandler,  /* 53 - LESENSE */
  (void *) EBI_IRQHandler,  /* 54 - EBI */
  (void *) ACMP2_IRQHandler,  /* 55 - ACMP2 */
  (void *) ADC1_IRQHandler,  /* 56 - ADC1 */
  (void *) LCD_IRQHandler,  /* 57 - LCD */
  (void *) SDIO_IRQHandler,  /* 58 - SDIO */
  (void *) ETH_IRQHandler,  /* 59 - ETH */
  (void *) CAN0_IRQHandler,  /* 60 - CAN0 */
  (void *) CAN1_IRQHandler,  /* 61 - CAN1 */
  (void *) USB_IRQHandler,  /* 62 - USB */
  (void *) RTC_IRQHandler,  /* 63 - RTC */
  (void *) WDOG1_IRQHandler,  /* 64 - WDOG1 */
  (void *) LETIMER1_IRQHandler,  /* 65 - LETIMER1 */
  (void *) TRNG0_IRQHandler,  /* 66 - TRNG0 */
  (void *) QSPI0_IRQHandler,  /* 67 - QSPI0 */

};
