/**************************************************************************//**
* @file uart.c
* @brief Bootloader UART functions.
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

#include "em_usart.h"
#include "em_gpio.h"
#include "main.h"
#include "dfu.h"
#include "uart.h"
#include <string.h>

#pragma data_alignment=4
static uint8_t tmp_buf[2048];

#define _USART_ROUTELOC0_TXLOC_SHIFT            8                                       /**< Shift value for USART_TXLOC */
#define _USART_ROUTELOC0_RXLOC_SHIFT            0                                       /**< Shift value for USART_RXLOC */

#define LOC_PA0    0
#define LOC_PA1    1
#define LOC_PA2    2
#define LOC_PA3    3
#define LOC_PA4    4
#define LOC_PA5    5

static const char* pinnames[] =
{
  "PA0", "PA1", "PA2", "PA3",
  "PA4", "PA5", "PB11", "PB12",
  "PB13", "PB14", "PB15", "PC6",
  "PC7", "PC8", "PC9", "PC10",
  "PC11", " ", "PD10", "PD11",
  "PD12", "PD13", "PD14", "PD15",
  "PF0", "PF1", "PF2", "PF3",
  "PF4", "PF5", "PF6", "PF7",
  ""
};

static const uint8_t portlist[] =
{
  //GPIO_LOC_PA0
  gpioPortA, gpioPortA, gpioPortA, gpioPortA, gpioPortA, gpioPortA,
  gpioPortB, gpioPortB, gpioPortB, gpioPortB, gpioPortB,
  gpioPortC, gpioPortC, gpioPortC, gpioPortC, gpioPortC, gpioPortC, 0,
  gpioPortD, gpioPortD, gpioPortD, gpioPortD, gpioPortD, gpioPortD,
  gpioPortF, gpioPortF, gpioPortF, gpioPortF, gpioPortF, gpioPortF, gpioPortF, gpioPortF
};
static const uint8_t pinlist[] =
{
  //GPIO_LOC_PA0
  0, 1, 2, 3, 4, 5, 11, 12, 13, 14, 15, 6, 7, 8, 9, 10, 11, 0, 10, 11, 12, 13, 14, 15, 0, 1, 2, 3, 4, 5, 6, 7
};

static void USART_txByte(uint8_t data)
{
  /* Check that transmit buffer is empty */
  while (!(BOOTLOADER_USART->STATUS & USART_STATUS_TXBL)) ;

  BOOTLOADER_USART->TXDATA = (uint32_t) data;
}
static void uart_put(uint8_t *p, int len)
{
  while (len--) {
    USART_txByte(*p++);
  }
}
void uart_output(uint8_t len1, uint8_t *data1, uint16_t len2, uint8_t*data2, uint8_t type2)
{
  uart_put(data1, len1);
  if (len2) {
    uart_put(data2, len2);
  }
}
static uint8_t USART_rxByte(void)
{
  while (!(BOOTLOADER_USART->STATUS & USART_STATUS_RXDATAV)) ;

  return((uint8_t)(BOOTLOADER_USART->RXDATA & 0xFF));
}
static void uart_read(uint8_t* buf, uint16_t len)
{
  while (len--) {
    *buf++ = USART_rxByte();
  }
}
int uart_input()
{
  uint16_t len;
  struct gecko_cmd_packet *packet = (struct gecko_cmd_packet*)tmp_buf;

  if (!(BOOTLOADER_USART->STATUS & USART_STATUS_RXDATAV)) {
    return 0;
  }
  //sync to first valid byte
  uart_read(tmp_buf, 1);
  if (tmp_buf[0] != 0x20) {
    return 0;
  }
  uart_read(&tmp_buf[1], sizeof(struct gecko_header) - 1);

  len = (packet->header.type_hilen & 0x7) << 8;
  len |= packet->header.lolen;
  if (len) {
    uart_read(&packet->payload, len);
  }

  handle_message(packet);

  return 1;
}

static uint8_t pin_to_location(const char*pinname, uint8_t shift)
{
  uint8_t i;
  const char *pn;
  for (i = 0; i < sizeof(pinnames) / sizeof(char*); i++) {
    pn = pinnames[i];
    if (strcmp(pn, pinname) == 0) {
      return (i - shift) & 0x1f;
    }
  }
  return 0;     // return default location
}

static uint8_t gpio_location_to_port(uint8_t shift, uint8_t loc)
{
  return portlist[(loc + shift) & 0x1f];
}

static uint8_t gpio_location_to_pin_index(uint8_t shift, uint8_t loc)
{
  return pinlist[(loc + shift) & 0x1f];
}

void uart_init()
{
/* Configure BOOTLOADER_USART */
/* Configuration needs to be given with USART_InitAsync_TypeDef struct */
  USART_InitAsync(BOOTLOADER_USART, &uartInit);

  uint8_t tx_loc = pin_to_location(tx_pin, LOC_PA0);
  uint8_t rx_loc = pin_to_location(rx_pin, LOC_PA1);
  uint8_t cts_loc, rts_loc;

  BOOTLOADER_USART->ROUTEPEN  = USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN;

  //BOOTLOADER_USART GPIO init, default values TX=PA0, RX=PA1

  BOOTLOADER_USART->ROUTELOC0 = (tx_loc << _USART_ROUTELOC0_TXLOC_SHIFT)
                                | (rx_loc << _USART_ROUTELOC0_RXLOC_SHIFT);

  //TX
  GPIO_PinModeSet((GPIO_Port_TypeDef)gpio_location_to_port(LOC_PA0, tx_loc),
                  gpio_location_to_pin_index(LOC_PA0, tx_loc), gpioModePushPull, 1);

  //RX
  GPIO_PinModeSet((GPIO_Port_TypeDef)gpio_location_to_port(LOC_PA1, rx_loc),
                  gpio_location_to_pin_index(LOC_PA1, rx_loc), gpioModeInput, 0);

  //RTS set to always enabled
  if (rts_pin != NULL) {
    rts_loc = pin_to_location(rts_pin, LOC_PA5);
    GPIO_PinModeSet((GPIO_Port_TypeDef)gpio_location_to_port(LOC_PA5, rts_loc),
                    gpio_location_to_pin_index(LOC_PA5, rts_loc), gpioModePushPull, 0);
  }
  //CTS
  if (cts_pin != NULL) {
    cts_loc = pin_to_location(cts_pin, LOC_PA4);
    BOOTLOADER_USART->ROUTELOC1 = (cts_loc << _USART_ROUTELOC1_CTSLOC_SHIFT);
    GPIO_PinModeSet((GPIO_Port_TypeDef)gpio_location_to_port(LOC_PA4, cts_loc),
                    gpio_location_to_pin_index(LOC_PA4, cts_loc), gpioModeInput, 0);
    BOOTLOADER_USART->CTRLX = USART_CTRLX_CTSEN | USART_CTRLX_DBGHALT;   // CTS enabled
    BOOTLOADER_USART->ROUTEPEN |= USART_ROUTEPEN_CTSPEN;
  }

  /* Clear RX/TX buffers, Enable RX/TX */
  BOOTLOADER_USART->CMD = USART_CMD_CLEARRX | USART_CMD_CLEARTX
                          | USART_CMD_RXEN | USART_CMD_TXEN;
}

#endif /* UART_DFU */
