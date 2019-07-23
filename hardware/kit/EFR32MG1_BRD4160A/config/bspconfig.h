/***************************************************************************//**
 * @file
 * @brief Provide BSP (board support package) configuration parameters.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

#ifndef BSPCONFIG_H
#define BSPCONFIG_H

#include "em_gpio.h"

#define BSP_STK
#define BSP_TBSENSE

#define BSP_BCC_USART         USART0
#define BSP_BCC_CLK           cmuClock_USART0
#define BSP_BCC_TX_LOCATION   USART_ROUTELOC0_TXLOC_LOC0
#define BSP_BCC_RX_LOCATION   USART_ROUTELOC0_RXLOC_LOC0
#define BSP_BCC_TXPORT        gpioPortA
#define BSP_BCC_TXPIN         0
#define BSP_BCC_RXPORT        gpioPortA
#define BSP_BCC_RXPIN         1
#define BSP_BCC_ENABLE_PORT   gpioPortA
#define BSP_BCC_ENABLE_PIN    5                 /* VCOM_ENABLE */

#define BSP_DISP_ENABLE_PORT  gpioPortD
#define BSP_DISP_ENABLE_PIN   15                /* MemLCD display enable */

#define BSP_EXTFLASH_PRESENT 1

#define BSP_GPIO_LEDS
#define BSP_NO_OF_LEDS          2
#define BSP_GPIO_LED0_PORT      gpioPortD
#define BSP_GPIO_LED0_PIN       11
#define BSP_GPIO_LED1_PORT      gpioPortD
#define BSP_GPIO_LED1_PIN       12
#define BSP_GPIO_LEDARRAY_INIT  { { BSP_GPIO_LED0_PORT, BSP_GPIO_LED0_PIN }, { BSP_GPIO_LED1_PORT, BSP_GPIO_LED1_PIN } }

#define BSP_GPIO_BUTTONS
#define BSP_NO_OF_BUTTONS       2
#define BSP_GPIO_PB0_PORT       gpioPortD
#define BSP_GPIO_PB0_PIN        14
#define BSP_GPIO_PB1_PORT       gpioPortD
#define BSP_GPIO_PB1_PIN        15

#define BSP_GPIO_BUTTONARRAY_INIT { { BSP_GPIO_PB0_PORT, BSP_GPIO_PB0_PIN }, { BSP_GPIO_PB1_PORT, BSP_GPIO_PB1_PIN } }

#define BSP_INIT_DEFAULT        0

#define BSP_LFXO_CTUNE          32U
#define BSP_HFXO_CTUNE          346U

#if !defined(EMU_DCDCINIT_WSTK_DEFAULT)
/* Use emlib defaults */
#define EMU_DCDCINIT_WSTK_DEFAULT       EMU_DCDCINIT_DEFAULT
#endif

#if !defined(CMU_HFXOINIT_WSTK_DEFAULT)
#define CMU_HFXOINIT_WSTK_DEFAULT                                         \
  {                                                                       \
    false,      /* Low-noise mode for EFR32 */                            \
    false,      /* Disable auto-start on EM0/1 entry */                   \
    false,      /* Disable auto-select on EM0/1 entry */                  \
    false,      /* Disable auto-start and select on RAC wakeup */         \
    _CMU_HFXOSTARTUPCTRL_CTUNE_DEFAULT,                                   \
    BSP_HFXO_CTUNE, /* Steady-state CTUNE for boards without load caps */ \
    _CMU_HFXOSTEADYSTATECTRL_REGISH_DEFAULT,                              \
    _CMU_HFXOSTARTUPCTRL_IBTRIMXOCORE_DEFAULT,                            \
    0x7,        /* Recommended steady-state XO core bias current */       \
    0x6,        /* Recommended peak detection threshold */                \
    _CMU_HFXOTIMEOUTCTRL_SHUNTOPTTIMEOUT_DEFAULT,                         \
    0xA,        /* Recommended peak detection timeout  */                 \
    _CMU_HFXOTIMEOUTCTRL_STEADYTIMEOUT_DEFAULT,                           \
    _CMU_HFXOTIMEOUTCTRL_STARTUPTIMEOUT_DEFAULT,                          \
    cmuOscMode_Crystal,                                                   \
  }
#endif

#if !defined(RADIO_PTI_INIT)
#define RADIO_PTI_INIT                                                     \
  {                                                                        \
    RADIO_PTI_MODE_UART,    /* Simplest output mode is UART mode */        \
    1600000,                /* Choose 1.6 MHz for best compatibility */    \
    6,                      /* TBSENSE uses location 6 for DOUT */         \
    gpioPortB,              /* Get the port for this loc */                \
    12,                     /* Get the pin, location should match above */ \
    6,                      /* TBSENSE uses location 6 for DCLK */         \
    gpioPortB,              /* Get the port for this loc */                \
    11,                     /* Get the pin, location should match above */ \
    6,                      /* TBSENSE uses location 6 for DFRAME */       \
    gpioPortB,              /* Get the port for this loc */                \
    13,                     /* Get the pin, location should match above */ \
  }
#endif

#if !defined(RAIL_PTI_CONFIG)
#define RAIL_PTI_CONFIG                                                    \
  {                                                                        \
    RAIL_PTI_MODE_UART,     /* Simplest output mode is UART mode */        \
    1600000,                /* Choose 1.6 MHz for best compatibility */    \
    6,                      /* TBSENSE uses location 6 for DOUT */         \
    gpioPortB,              /* Get the port for this loc */                \
    12,                     /* Get the pin, location should match above */ \
    6,                      /* TBSENSE uses location 6 for DCLK */         \
    gpioPortB,              /* Get the port for this loc */                \
    11,                     /* Get the pin, location should match above */ \
    6,                      /* TBSENSE uses location 6 for DFRAME */       \
    gpioPortB,              /* Get the port for this loc */                \
    13,                     /* Get the pin, location should match above */ \
  }
#endif

#if !defined(RADIO_PA_2P4_INIT)
#define RADIO_PA_2P4_INIT                                    \
  {                                                          \
    PA_SEL_2P4_HP,    /* Power Amplifier mode */             \
    PA_VOLTMODE_DCDC, /* Power Amplifier vPA Voltage mode */ \
    100,              /* Desired output power in dBm * 10 */ \
    0,                /* Output power offset in dBm * 10 */  \
    10,               /* Desired ramp time in us */          \
  }
#endif

#if !defined(RAIL_PA_2P4_CONFIG)
#define RAIL_PA_2P4_CONFIG                                            \
  {                                                                   \
    RAIL_TX_POWER_MODE_2P4_HP, /* Power Amplifier mode */             \
    1800,                      /* Power Amplifier vPA Voltage mode */ \
    10,                        /* Desired ramp time in us */          \
  }
#endif

#if !defined(RADIO_PA_SUBGIG_INIT)
#define RADIO_PA_SUBGIG_INIT                                 \
  {                                                          \
    PA_SEL_SUBGIG,    /* Power Amplifier mode */             \
    PA_VOLTMODE_DCDC, /* Power Amplifier vPA Voltage mode */ \
    100,              /* Desired output power in dBm * 10 */ \
    0,                /* Output power offset in dBm * 10 */  \
    10,               /* Desired ramp time in us */          \
  }
#endif

#if !defined(RAIL_PA_SUBGIG_CONFIG)
#define RAIL_PA_SUBGIG_CONFIG                                         \
  {                                                                   \
    RAIL_TX_POWER_MODE_SUBGIG, /* Power Amplifier mode */             \
    1800,                      /* Power Amplifier vPA Voltage mode */ \
    10,                        /* Desired ramp time in us */          \
  }
#endif

#if !defined(RAIL_PA_DEFAULT_POWER)
#define RAIL_PA_DEFAULT_POWER 100
#endif

/***************************************************************************//**
 * @defgroup BOARD_Config_Settings BOARD module configuration
 * @{
 * @brief BOARD module configuration macro definitions
 ******************************************************************************/
#define BOARD_PIC_DEVICE_ID       0x50584F49      /**< PIC device ID                    */
#define BOARD_PIC_INT_WAKE_PORT   gpioPortD       /**< PIC INT/Wakeup port              */
#define BOARD_PIC_INT_WAKE_PIN    10              /**< PIC INT/Wakeup pin               */

#define BOARD_RGBLED_TIMER        (TIMER0)        /**< RGB LED PWM control timer        */
#define BOARD_RGBLED_CMU_CLK      cmuClock_TIMER0 /**< RGB LED PWM control clock source */
#define BOARD_RGBLED_RED_PORT     gpioPortD       /**< RGB LED Red port                 */
#define BOARD_RGBLED_RED_PIN      11              /**< RGB LED Red pin                  */
#define BOARD_RGBLED_RED_CCLOC    19              /**< RGB LED Red CC location          */
#define BOARD_RGBLED_GREEN_PORT   gpioPortD       /**< RGB LED Green port               */
#define BOARD_RGBLED_GREEN_PIN    12              /**< RGB LED Green pin                */
#define BOARD_RGBLED_GREEN_CCLOC  19              /**< RGB LED Green CC location        */
#define BOARD_RGBLED_BLUE_PORT    gpioPortD       /**< RGB LED Blue port                */
#define BOARD_RGBLED_BLUE_PIN     13              /**< RGB LED Blue pin                 */
#define BOARD_RGBLED_BLUE_CCLOC   19              /**< RGB LED Blue CC location         */

/* PIC interrupt index numbers */
#define BOARD_PIC_INT_IDX_CCS811    0             /**< Interrupt index number, CSS811   */
#define BOARD_PIC_INT_IDX_IMU       1             /**< Interrupt index number, IMU      */
#define BOARD_PIC_INT_IDX_UV_ALS    2             /**< Interrupt index number, UV/ALS   */

/* External interrupts */
#define EXTI_BUTTON1              15
#define EXTI_BUTTON0              14
#define EXTI_PIC_INT              10

#define BOARD_BUTTON_PORT         gpioPortD       /**< Pushbutton port                  */
#define BOARD_BUTTON_SHIFT        14              /**< Pushbutton shift value           */
#define BOARD_BUTTON_LEFT         0x01            /**< Left pushbutton value            */
#define BOARD_BUTTON_RIGHT        0x02            /**< Right pushbutton value           */
#define BOARD_BUTTON_MASK         0x03            /**< Pushbutton mask                  */
#define BOARD_BUTTON_LEFT_PORT    gpioPortD       /**< Left pushbutton port             */
#define BOARD_BUTTON_LEFT_PIN     14              /**< Left pushbutton pin              */
#define BOARD_BUTTON_RIGHT_PORT   gpioPortD       /**< Right pushbutton port            */
#define BOARD_BUTTON_RIGHT_PIN    15              /**< Right pushbutton pin             */
#define BOARD_BUTTON_INT_FLAG     0x04            /**< Pushbutton interrupt flag value  */
#define BOARD_BUTTON_INT_ENABLE   true            /**< Pushbutton interrupt enable      */

/** @} {end defgroup BOARD_Config_Setting} */

#define BSP_BCP_VERSION 2
#include "bsp_bcp.h"

#endif // BSPCONFIG_H
