#ifndef CONFIG_H
#define CONFIG_H

#ifdef UART_DFU
#ifndef UART_DFU_CONF
#define UART_DFU_CONF "uart_config.h"
#endif
#include UART_DFU_CONF
#endif

#define BOOTLOADER_USART                USART0
#define BOOTLOADER_USART_CLOCKEN        CMU_HFPERCLKEN0_USART0
#ifndef BOOTLOADER_SIZE
#define BOOTLOADER_SIZE                 (0x4000)
#endif
#endif
