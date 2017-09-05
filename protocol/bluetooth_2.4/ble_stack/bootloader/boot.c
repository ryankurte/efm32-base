/**************************************************************************//**
* @file boot.c
* @brief Boot Loader
* @version x.xx
******************************************************************************
* @section License
* <b>Copyright 2015 Silicon Laboratories, Inc. http://www.silabs.com</b>
*******************************************************************************
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
* DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Silicon Labs has no
* obligation to support this Software. Silicon Labs is providing the
* Software "AS IS", with no express or implied warranties of any kind,
* including, but not limited to, any implied warranties of merchantability
* or fitness for any particular purpose or warranties against infringement
* of any proprietary rights of a third party.
*
* Silicon Labs will not be liable for any consequential, incidental, or
* special damages, or any other relief, or for any claim by any third party,
* arising from your use of this Software.
*
******************************************************************************/
#ifdef UART_DFU

#include <stdio.h>
#include <stdbool.h>

#include "em_device.h"
#include "boot.h"
#include "config.h"

/**************************************************************************//**
 * @brief Checks to see if the reset vector of the application is valid
 * @return false if the firmware is not valid, true if it is.
 *****************************************************************************/
bool BOOT_checkFirmwareIsValid(void)
{
  uint32_t pc;
  pc = *((uint32_t *) BOOTLOADER_SIZE + 1);

  if (pc < FLASH_SIZE) {
    return true;
  }
  return false;
}

/**************************************************************************//**
 * @brief This function sets up the Cortex M-3 with a new SP and PC.
 *****************************************************************************/
void BOOT_jump(uint32_t sp, uint32_t pc)
{
  (void) sp;
  (void) pc;
  /* Set new MSP, PSP based on SP (r0)*/
  __asm("msr msp, r0");
  __asm("msr psp, r0");

  /* Jump to PC (r1)*/
  __asm("mov pc, r1");
}

/**************************************************************************//**
 * @brief Boots the application
 *****************************************************************************/
void BOOT_boot(void)
{
  /* Reset registers */

#if defined(USART_OVERLAPS_WITH_BOOTLOADER) \
  && !defined(_SILICON_LABS_32B_PLATFORM_2)
  CMU->LFBCLKEN0    = _CMU_LFBCLKEN0_RESETVALUE;
  GPIO->ROUTE       = _GPIO_ROUTE_RESETVALUE;
  GPIO->EXTIPSELL   = _GPIO_EXTIPSELL_RESETVALUE;
  GPIO->EXTIFALL    = _GPIO_EXTIFALL_RESETVALUE;
  GPIO->IEN         = _GPIO_IEN_RESETVALUE;
  GPIO->IFC         = 0xFFFFFFFF;
#endif

  /* Reset GPIO settings */
  GPIO->P[0].MODEL  = _GPIO_P_MODEL_RESETVALUE;
  /* Clear all interrupts set. */
  NVIC->ICER[0]     = 0xFFFFFFFF;
#if (__CORTEX_M != 0)
  NVIC->ICER[1]     = 0xFFFFFFFF;
#endif

#if defined(_SILICON_LABS_32B_PLATFORM_2)
  BOOTLOADER_USART->CLKDIV    = _USART_CLKDIV_RESETVALUE;
  BOOTLOADER_USART->ROUTEPEN  = _USART_ROUTEPEN_RESETVALUE;
  BOOTLOADER_USART->ROUTELOC0 = _USART_ROUTELOC0_RESETVALUE;

  MSC->WDATA     = _MSC_WDATA_RESETVALUE;
  MSC->ADDRB     = _MSC_ADDRB_RESETVALUE;
  MSC->WRITECTRL = _MSC_WRITECTRL_RESETVALUE;
  MSC->LOCK      = MSC_LOCK_LOCKKEY_LOCK;

  CMU->LFECLKSEL   = _CMU_LFECLKSEL_RESETVALUE;
  CMU->LFECLKEN0   = _CMU_LFECLKEN0_RESETVALUE;
  CMU->HFBUSCLKEN0 = _CMU_HFBUSCLKEN0_RESETVALUE;
  CMU->HFPERCLKEN0 = _CMU_HFPERCLKEN0_RESETVALUE;
  CMU->OSCENCMD    = CMU_OSCENCMD_LFRCODIS;
  CMU->CTRL        = _CMU_CTRL_RESETVALUE;
  // Change to default clock frequency.
  CMU->HFRCOCTRL   = *(uint32_t *)(&DEVINFO->HFRCOCAL8);

#else
  RTC->CTRL         = _RTC_CTRL_RESETVALUE;
  RTC->COMP0        = _RTC_COMP0_RESETVALUE;
  RTC->IEN          = _RTC_IEN_RESETVALUE;
  /* Disable RTC clock */
  CMU->LFACLKEN0    = _CMU_LFACLKEN0_RESETVALUE;
  CMU->LFCLKSEL     = _CMU_LFCLKSEL_RESETVALUE;
  /* Disable LFRCO */
  CMU->OSCENCMD     = CMU_OSCENCMD_LFRCODIS;
  /* Disable LE interface */
  CMU->HFCORECLKEN0 = _CMU_HFCORECLKEN0_RESETVALUE;
  /* Reset clocks */
  CMU->HFPERCLKDIV  = _CMU_HFPERCLKDIV_RESETVALUE;
  CMU->HFPERCLKEN0  = _CMU_HFPERCLKEN0_RESETVALUE;
#endif
}
#endif /* UART_DFU */
