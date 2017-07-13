/**************************************************************************//**
 *  File        : startup_efm32x.s
 *
 *  Abstract    : This file contains interrupt vector and startup code.
 *
 *  Functions   : Reset_Handler
 *
 *  Target      : Silicon Labs EFM32GG11B devices.
 *
 *  Environment : Atollic TrueSTUDIO(R)
 *
 *  Distribution: The file is distributed "as is," without any warranty
 *                of any kind.
 *
 *  (c)Copyright Atollic AB.
 *  You may use this file as-is or modify it according to the needs of your
 *  project. This file may only be built (assembled or compiled and linked)
 *  using the Atollic TrueSTUDIO(R) product. The use of this file together
 *  with other tools than Atollic TrueSTUDIO(R) is not permitted.
 *
 *******************************************************************************
 * Silicon Labs release version
 * @version 5.2.1
 ******************************************************************************/
  .syntax unified
  .thumb

  .global Reset_Handler
  .global InterruptVector
  .global Default_Handler

  /* Linker script definitions */
  /* start address for the initialization values of the .data section */
  .word _sidata
  /* start address for the .data section */
  .word _sdata
  /* end address for the .data section */
  .word _edata
  /* start address for the .bss section */
  .word _sbss
  /* end address for the .bss section */
  .word _ebss

/**
**===========================================================================
**  Program - Reset_Handler
**  Abstract: This code gets called after reset.
**===========================================================================
*/
  .section  .text.Reset_Handler,"ax", %progbits
  .type Reset_Handler, %function
Reset_Handler:
  /* Set stack pointer */
  ldr   sp,=_estack

  /* Branch to SystemInit function */
  bl    SystemInit

  /* Copy data initialization values */
  ldr   r1,=_sidata
  ldr   r2,=_sdata
  ldr   r3,=_edata
  b     cmpdata
CopyLoop:
  ldr   r0, [r1], #4
  str   r0, [r2], #4
cmpdata:
  cmp   r2, r3
  blt   CopyLoop

  /* Clear BSS section */
  movs  r0, #0
  ldr   r2,=_sbss
  ldr   r3,=_ebss
  b     cmpbss
ClearLoop:
  str   r0, [r2], #4
cmpbss:
  cmp   r2, r3
  blt   ClearLoop

  /* Call static constructors */
  bl    __libc_init_array

  /* Branch to main */
  bl    main

  /* If main returns, branch to Default_Handler. */
  b     Default_Handler

  .size  Reset_Handler, .-Reset_Handler

/**
**===========================================================================
**  Program - Default_Handler
**  Abstract: This code gets called when the processor receives an
**    unexpected interrupt.
**===========================================================================
*/
  .section  .text.Default_Handler,"ax", %progbits
Default_Handler:
  b  Default_Handler

  .size  Default_Handler, .-Default_Handler

/**
**===========================================================================
**  Interrupt vector table
**===========================================================================
*/
  .section .isr_vector,"a", %progbits
InterruptVector:
  .word _estack                   /* 0 - Stack pointer */
  .word Reset_Handler             /* 1 - Reset */
  .word NMI_Handler               /* 2 - NMI  */
  .word HardFault_Handler         /* 3 - Hard fault */
  .word MemManage_Handler         /* 4 - Memory management fault */
  .word BusFault_Handler          /* 5 - Bus fault */
  .word UsageFault_Handler        /* 6 - Usage fault */
  .word 0                         /* 7 - Reserved */
  .word 0                         /* 8 - Reserved */
  .word 0                         /* 9 - Reserved */
  .word 0                         /* 10 - Reserved */
  .word SVC_Handler               /* 11 - SVCall */
  .word DebugMonitor_Handler      /* 12 - Reserved for Debug */
  .word 0                         /* 13 - Reserved */
  .word PendSV_Handler            /* 14 - PendSV */
  .word SysTick_Handler           /* 15 - Systick */

  /* External Interrupts */

  .word   EMU_IRQHandler      /* 0 - EMU */
  .word   WDOG0_IRQHandler      /* 1 - WDOG0 */
  .word   LDMA_IRQHandler      /* 2 - LDMA */
  .word   GPIO_EVEN_IRQHandler      /* 3 - GPIO_EVEN */
  .word   SMU_IRQHandler      /* 4 - SMU */
  .word   TIMER0_IRQHandler      /* 5 - TIMER0 */
  .word   USART0_RX_IRQHandler      /* 6 - USART0_RX */
  .word   USART0_TX_IRQHandler      /* 7 - USART0_TX */
  .word   ACMP0_IRQHandler      /* 8 - ACMP0 */
  .word   ADC0_IRQHandler      /* 9 - ADC0 */
  .word   IDAC0_IRQHandler      /* 10 - IDAC0 */
  .word   I2C0_IRQHandler      /* 11 - I2C0 */
  .word   I2C1_IRQHandler      /* 12 - I2C1 */
  .word   GPIO_ODD_IRQHandler      /* 13 - GPIO_ODD */
  .word   TIMER1_IRQHandler      /* 14 - TIMER1 */
  .word   TIMER2_IRQHandler      /* 15 - TIMER2 */
  .word   TIMER3_IRQHandler      /* 16 - TIMER3 */
  .word   USART1_RX_IRQHandler      /* 17 - USART1_RX */
  .word   USART1_TX_IRQHandler      /* 18 - USART1_TX */
  .word   USART2_RX_IRQHandler      /* 19 - USART2_RX */
  .word   USART2_TX_IRQHandler      /* 20 - USART2_TX */
  .word   UART0_RX_IRQHandler      /* 21 - UART0_RX */
  .word   UART0_TX_IRQHandler      /* 22 - UART0_TX */
  .word   UART1_RX_IRQHandler      /* 23 - UART1_RX */
  .word   UART1_TX_IRQHandler      /* 24 - UART1_TX */
  .word   LEUART0_IRQHandler      /* 25 - LEUART0 */
  .word   LEUART1_IRQHandler      /* 26 - LEUART1 */
  .word   LETIMER0_IRQHandler      /* 27 - LETIMER0 */
  .word   PCNT0_IRQHandler      /* 28 - PCNT0 */
  .word   PCNT1_IRQHandler      /* 29 - PCNT1 */
  .word   PCNT2_IRQHandler      /* 30 - PCNT2 */
  .word   RTCC_IRQHandler      /* 31 - RTCC */
  .word   CMU_IRQHandler      /* 32 - CMU */
  .word   MSC_IRQHandler      /* 33 - MSC */
  .word   CRYPTO0_IRQHandler      /* 34 - CRYPTO0 */
  .word   CRYOTIMER_IRQHandler      /* 35 - CRYOTIMER */
  .word   FPUEH_IRQHandler      /* 36 - FPUEH */
  .word   USART3_RX_IRQHandler      /* 37 - USART3_RX */
  .word   USART3_TX_IRQHandler      /* 38 - USART3_TX */
  .word   USART4_RX_IRQHandler      /* 39 - USART4_RX */
  .word   USART4_TX_IRQHandler      /* 40 - USART4_TX */
  .word   WTIMER0_IRQHandler      /* 41 - WTIMER0 */
  .word   WTIMER1_IRQHandler      /* 42 - WTIMER1 */
  .word   WTIMER2_IRQHandler      /* 43 - WTIMER2 */
  .word   WTIMER3_IRQHandler      /* 44 - WTIMER3 */
  .word   I2C2_IRQHandler      /* 45 - I2C2 */
  .word   VDAC0_IRQHandler      /* 46 - VDAC0 */
  .word   TIMER4_IRQHandler      /* 47 - TIMER4 */
  .word   TIMER5_IRQHandler      /* 48 - TIMER5 */
  .word   TIMER6_IRQHandler      /* 49 - TIMER6 */
  .word   USART5_RX_IRQHandler      /* 50 - USART5_RX */
  .word   USART5_TX_IRQHandler      /* 51 - USART5_TX */
  .word   CSEN_IRQHandler      /* 52 - CSEN */
  .word   LESENSE_IRQHandler      /* 53 - LESENSE */
  .word   EBI_IRQHandler      /* 54 - EBI */
  .word   ACMP2_IRQHandler      /* 55 - ACMP2 */
  .word   ADC1_IRQHandler      /* 56 - ADC1 */
  .word   LCD_IRQHandler      /* 57 - LCD */
  .word   SDIO_IRQHandler      /* 58 - SDIO */
  .word   ETH_IRQHandler      /* 59 - ETH */
  .word   CAN0_IRQHandler      /* 60 - CAN0 */
  .word   CAN1_IRQHandler      /* 61 - CAN1 */
  .word   USB_IRQHandler      /* 62 - USB */
  .word   RTC_IRQHandler      /* 63 - RTC */
  .word   WDOG1_IRQHandler      /* 64 - WDOG1 */
  .word   LETIMER1_IRQHandler      /* 65 - LETIMER1 */
  .word   TRNG0_IRQHandler      /* 66 - TRNG0 */
  .word   QSPI0_IRQHandler      /* 67 - QSPI0 */


/**
**===========================================================================
**  Weak interrupt handlers redirected to Default_Handler. These can be
**  overridden in user code.
**===========================================================================
*/
  .weak NMI_Handler
  .thumb_set NMI_Handler, Default_Handler

  .weak HardFault_Handler
  .thumb_set HardFault_Handler, Default_Handler

  .weak MemManage_Handler
  .thumb_set MemManage_Handler, Default_Handler

  .weak BusFault_Handler
  .thumb_set BusFault_Handler, Default_Handler

  .weak UsageFault_Handler
  .thumb_set UsageFault_Handler, Default_Handler

  .weak SVC_Handler
  .thumb_set SVC_Handler, Default_Handler

  .weak DebugMonitor_Handler
  .thumb_set DebugMonitor_Handler, Default_Handler

  .weak PendSV_Handler
  .thumb_set PendSV_Handler, Default_Handler

  .weak SysTick_Handler
  .thumb_set SysTick_Handler, Default_Handler


  .weak       EMU_IRQHandler
  .thumb_set  EMU_IRQHandler, Default_Handler

  .weak       WDOG0_IRQHandler
  .thumb_set  WDOG0_IRQHandler, Default_Handler

  .weak       LDMA_IRQHandler
  .thumb_set  LDMA_IRQHandler, Default_Handler

  .weak       GPIO_EVEN_IRQHandler
  .thumb_set  GPIO_EVEN_IRQHandler, Default_Handler

  .weak       SMU_IRQHandler
  .thumb_set  SMU_IRQHandler, Default_Handler

  .weak       TIMER0_IRQHandler
  .thumb_set  TIMER0_IRQHandler, Default_Handler

  .weak       USART0_RX_IRQHandler
  .thumb_set  USART0_RX_IRQHandler, Default_Handler

  .weak       USART0_TX_IRQHandler
  .thumb_set  USART0_TX_IRQHandler, Default_Handler

  .weak       ACMP0_IRQHandler
  .thumb_set  ACMP0_IRQHandler, Default_Handler

  .weak       ADC0_IRQHandler
  .thumb_set  ADC0_IRQHandler, Default_Handler

  .weak       IDAC0_IRQHandler
  .thumb_set  IDAC0_IRQHandler, Default_Handler

  .weak       I2C0_IRQHandler
  .thumb_set  I2C0_IRQHandler, Default_Handler

  .weak       I2C1_IRQHandler
  .thumb_set  I2C1_IRQHandler, Default_Handler

  .weak       GPIO_ODD_IRQHandler
  .thumb_set  GPIO_ODD_IRQHandler, Default_Handler

  .weak       TIMER1_IRQHandler
  .thumb_set  TIMER1_IRQHandler, Default_Handler

  .weak       TIMER2_IRQHandler
  .thumb_set  TIMER2_IRQHandler, Default_Handler

  .weak       TIMER3_IRQHandler
  .thumb_set  TIMER3_IRQHandler, Default_Handler

  .weak       USART1_RX_IRQHandler
  .thumb_set  USART1_RX_IRQHandler, Default_Handler

  .weak       USART1_TX_IRQHandler
  .thumb_set  USART1_TX_IRQHandler, Default_Handler

  .weak       USART2_RX_IRQHandler
  .thumb_set  USART2_RX_IRQHandler, Default_Handler

  .weak       USART2_TX_IRQHandler
  .thumb_set  USART2_TX_IRQHandler, Default_Handler

  .weak       UART0_RX_IRQHandler
  .thumb_set  UART0_RX_IRQHandler, Default_Handler

  .weak       UART0_TX_IRQHandler
  .thumb_set  UART0_TX_IRQHandler, Default_Handler

  .weak       UART1_RX_IRQHandler
  .thumb_set  UART1_RX_IRQHandler, Default_Handler

  .weak       UART1_TX_IRQHandler
  .thumb_set  UART1_TX_IRQHandler, Default_Handler

  .weak       LEUART0_IRQHandler
  .thumb_set  LEUART0_IRQHandler, Default_Handler

  .weak       LEUART1_IRQHandler
  .thumb_set  LEUART1_IRQHandler, Default_Handler

  .weak       LETIMER0_IRQHandler
  .thumb_set  LETIMER0_IRQHandler, Default_Handler

  .weak       PCNT0_IRQHandler
  .thumb_set  PCNT0_IRQHandler, Default_Handler

  .weak       PCNT1_IRQHandler
  .thumb_set  PCNT1_IRQHandler, Default_Handler

  .weak       PCNT2_IRQHandler
  .thumb_set  PCNT2_IRQHandler, Default_Handler

  .weak       RTCC_IRQHandler
  .thumb_set  RTCC_IRQHandler, Default_Handler

  .weak       CMU_IRQHandler
  .thumb_set  CMU_IRQHandler, Default_Handler

  .weak       MSC_IRQHandler
  .thumb_set  MSC_IRQHandler, Default_Handler

  .weak       CRYPTO0_IRQHandler
  .thumb_set  CRYPTO0_IRQHandler, Default_Handler

  .weak       CRYOTIMER_IRQHandler
  .thumb_set  CRYOTIMER_IRQHandler, Default_Handler

  .weak       FPUEH_IRQHandler
  .thumb_set  FPUEH_IRQHandler, Default_Handler

  .weak       USART3_RX_IRQHandler
  .thumb_set  USART3_RX_IRQHandler, Default_Handler

  .weak       USART3_TX_IRQHandler
  .thumb_set  USART3_TX_IRQHandler, Default_Handler

  .weak       USART4_RX_IRQHandler
  .thumb_set  USART4_RX_IRQHandler, Default_Handler

  .weak       USART4_TX_IRQHandler
  .thumb_set  USART4_TX_IRQHandler, Default_Handler

  .weak       WTIMER0_IRQHandler
  .thumb_set  WTIMER0_IRQHandler, Default_Handler

  .weak       WTIMER1_IRQHandler
  .thumb_set  WTIMER1_IRQHandler, Default_Handler

  .weak       WTIMER2_IRQHandler
  .thumb_set  WTIMER2_IRQHandler, Default_Handler

  .weak       WTIMER3_IRQHandler
  .thumb_set  WTIMER3_IRQHandler, Default_Handler

  .weak       I2C2_IRQHandler
  .thumb_set  I2C2_IRQHandler, Default_Handler

  .weak       VDAC0_IRQHandler
  .thumb_set  VDAC0_IRQHandler, Default_Handler

  .weak       TIMER4_IRQHandler
  .thumb_set  TIMER4_IRQHandler, Default_Handler

  .weak       TIMER5_IRQHandler
  .thumb_set  TIMER5_IRQHandler, Default_Handler

  .weak       TIMER6_IRQHandler
  .thumb_set  TIMER6_IRQHandler, Default_Handler

  .weak       USART5_RX_IRQHandler
  .thumb_set  USART5_RX_IRQHandler, Default_Handler

  .weak       USART5_TX_IRQHandler
  .thumb_set  USART5_TX_IRQHandler, Default_Handler

  .weak       CSEN_IRQHandler
  .thumb_set  CSEN_IRQHandler, Default_Handler

  .weak       LESENSE_IRQHandler
  .thumb_set  LESENSE_IRQHandler, Default_Handler

  .weak       EBI_IRQHandler
  .thumb_set  EBI_IRQHandler, Default_Handler

  .weak       ACMP2_IRQHandler
  .thumb_set  ACMP2_IRQHandler, Default_Handler

  .weak       ADC1_IRQHandler
  .thumb_set  ADC1_IRQHandler, Default_Handler

  .weak       LCD_IRQHandler
  .thumb_set  LCD_IRQHandler, Default_Handler

  .weak       SDIO_IRQHandler
  .thumb_set  SDIO_IRQHandler, Default_Handler

  .weak       ETH_IRQHandler
  .thumb_set  ETH_IRQHandler, Default_Handler

  .weak       CAN0_IRQHandler
  .thumb_set  CAN0_IRQHandler, Default_Handler

  .weak       CAN1_IRQHandler
  .thumb_set  CAN1_IRQHandler, Default_Handler

  .weak       USB_IRQHandler
  .thumb_set  USB_IRQHandler, Default_Handler

  .weak       RTC_IRQHandler
  .thumb_set  RTC_IRQHandler, Default_Handler

  .weak       WDOG1_IRQHandler
  .thumb_set  WDOG1_IRQHandler, Default_Handler

  .weak       LETIMER1_IRQHandler
  .thumb_set  LETIMER1_IRQHandler, Default_Handler

  .weak       TRNG0_IRQHandler
  .thumb_set  TRNG0_IRQHandler, Default_Handler

  .weak       QSPI0_IRQHandler
  .thumb_set  QSPI0_IRQHandler, Default_Handler




  .end
