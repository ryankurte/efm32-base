/**************************************************************************//**
* @file main.c
* @brief Bootloader main functions.
* @version x.xx
******************************************************************************
* @section License
* <b>Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com</b>
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

#include "main.h"
#include "flash.h"
#include "crc.h"
#include "aat.h"
#include "em_device.h"

bool check_image_crc(const uint8_t *address)
{
  const aat_t* aat = (const aat_t*) address;
  uint32_t crc = CRC32_START;
  uint32_t i;
  uint8_t page;

  // clear AAT
  for (i = 0; i < sizeof(aat_t); i++) {
    crc = halCommonCrc32(0xff, crc);
  }
  address += sizeof(aat_t);

  //rest of first flash page
  for (i = 0; i < FLASH_PAGE_SIZE - sizeof(aat_t); i++) {
    crc = halCommonCrc32(*address, crc);
    address += 1;
  }

  //rest of the flash pages
  for (page = (aat->pageRanges[0] & 0xff) + 1;
       page <= (aat->pageRanges[0] >> 8);
       page++) {
    for (i = 0; i < FLASH_PAGE_SIZE; i++) {
      crc = halCommonCrc32(*address, crc);
      address += 1;
    }
  }

  //not used page ranges
  for (i = 0; i < FLASH_PAGE_SIZE * 5; i++) {
    crc = halCommonCrc32(0xff, crc);
  }

  if (aat->imageCrc == ~crc) {
    return true;
  } else {
    return false;
  }
}

/******** OTA DFU **********/
#ifdef OTA_DFU
/*lint -e754 //stack not referenced */

#include "nvm_hal.h"

#ifndef __IAR_SYSTEMS_ICC__
void * __section_end(char const * section);
#endif

void flash_copy(uint8_t *src, uint8_t *dst, int len)
{
  int dl;
  while (len > 0) {
#if 1
    FLASH_eraseOneBlock((uint32_t) dst);
    dl = len;
    if (dl > FLASH_PAGE_SIZE) {
      dl = FLASH_PAGE_SIZE;
    }
    FLASH_writeBlock(dst, dl, src);
#endif
    dst += dl;
    src += dl;
    len -= dl;
  }
}

void ota_main()
{
  uint8_t* src;
  uint8_t* dst;
  uint8_t* end;
  aat_t *aat;
  const aat_t *saat = (const aat_t*)BOOTLOADER_SIZE;//stack aat

  /*Init CRC*/
  /* Enable the General purpose CRC engine */
  CMU->HFBUSCLKEN0 |= CMU_HFBUSCLKEN0_GPCRC;

  /*find aat from flash*/
  //aat=(aat_t*)((saat->pageRanges[0]>>8)*FLASH_PAGE_SIZE);

  for (src = (uint8_t*)(BOOTLOADER_SIZE + FLASH_PAGE_SIZE); src < (uint8_t*)FLASH_SIZE; src += FLASH_PAGE_SIZE) {
    aat = (aat_t*)src;
    //is it AAT
    if (aat->type != APP_ADDRESS_TABLE_TYPE) {
      continue;
    }
    dst = (uint8_t*)((aat->pageRanges[0] & 0xff) * FLASH_PAGE_SIZE);
    end = (uint8_t*)((aat->pageRanges[0] >> 8) * FLASH_PAGE_SIZE);
    //is it for stack address
    if (dst != (uint8_t*)BOOTLOADER_SIZE) {
      continue;
    }
    if (check_image_crc(src)) {
      flash_copy(src, (uint8_t*)BOOTLOADER_SIZE, (end + FLASH_PAGE_SIZE) - dst);
    }
    FLASH_eraseOneBlock((uint32_t) src);//invalidate source
    break;
  }

  //use vector table from aat
  SCB->VTOR = (uint32_t)saat->vectorTable;
  //Use stack from aat
  asm ("mov sp,%0" :: "r" (saat->topOfStack));
  saat->resetVector();
}
#endif /* OTA_DFU */

/******** UART DFU **********/
#ifdef UART_DFU

#include "em_gpio.h"

#include "uart.h"
#include "boot.h"
#include "api/gecko_bgapi.h"

#define DEBUG_STAY 0

// Vector table in RAM. We construct a new vector table to conserve space in
// flash as it is sparsly populated.
#define SW_RESET_COMMAND (*((uint32_t*)(SRAM_BASE)))
struct gecko_cmd_packet bglib_temp_msg;
extern uint8_t dfu_active;
const aat_t *saat = (const aat_t*)BOOTLOADER_SIZE;//stack aat

void main_loop()
{
  volatile uint32_t l = 1000000;
  do {
    uart_input();
#if !DEBUG_STAY
    if (!dfu_active && (*MAIN_APP != 0xFFFFFFFF)) {
      l--;
    }
#endif
  } while (l);
}

void hw_init()
{
  /*clear reset state*/
  RMU->CMD = RMU_CMD_RCCLR;
  SW_RESET_COMMAND = 0;
  /*enable clocks*/
  CMU->CTRL        = CMU_CTRL_HFPERCLKEN;
  CMU->HFBUSCLKEN0 = CMU_HFBUSCLKEN0_GPIO | CMU_HFBUSCLKEN0_LE;
  // Change to 38MHz HFRCO. Use calibration data from DI page.
  CMU->HFRCOCTRL = *(uint32_t *)(&DEVINFO->HFRCOCAL12);
  // Set WSHFLE to allow access to LE peripherals when running at frequencies
  // above 32 MHz.
  CMU->CTRL |= CMU_CTRL_WSHFLE;

  /**/
  CMU->HFPERCLKEN0 |= BOOTLOADER_USART_CLOCKEN;

  uart_init();
  //Enable WSTK UART
#if WSTK_VCOM
  SET_PINMODEL(gpioPortA, 5, gpioModePushPull, 1);
#endif
}

void uart_main()
{
  /*check reason for restart*/
  /*if reset is from hardware or from software with reason, then start bootloader*/
#if !DEBUG_STAY
  if ((RMU->RSTCAUSE & (RMU_RSTCAUSE_PORST | RMU_RSTCAUSE_AVDDBOD | RMU_RSTCAUSE_DVDDBOD | RMU_RSTCAUSE_DECBOD | RMU_RSTCAUSE_EXTRST | RMU_RSTCAUSE_LOCKUPRST))
      || ((RMU->RSTCAUSE & (RMU_RSTCAUSE_SYSREQRST | RMU_RSTCAUSE_WDOGRST | RMU_RSTCAUSE_EM4RST))
          && SW_RESET_COMMAND > 0) || (*MAIN_APP == 0xFFFFFFFF))
#endif
  {
    hw_init();
    gecko_evt_dfu_boot(BOOTLOADER_VERSION);
    main_loop();
  } else {
    hw_init();
  }
  //check image crc
  //skip crc check if original software is installed
  if ((saat->imageCrc != IMAGE_CRC_MAGIC) || (saat->timeStamp != IMAGE_TIMESTAMP_MAGIC)) {
    /* if crc error send event and erase first memory page */
    if (!check_image_crc((uint8_t *)BOOTLOADER_SIZE)) {
      gecko_evt_dfu_boot_failure(bg_err_security_image_checksum_error);
      do {
        uart_input();
      } while (1);
    }
  }

  BOOT_boot();
  //use vector table from aat
  SCB->VTOR = (uint32_t)saat->vectorTable;
  //Use stack from aat
  asm ("mov sp,%0" :: "r" (saat->topOfStack));
  saat->resetVector();
}
#endif /* UART_DFU */

void main()
{
#ifdef OTA_DFU
  ota_main();
#endif
#ifdef UART_DFU
  uart_main();
#endif
}
