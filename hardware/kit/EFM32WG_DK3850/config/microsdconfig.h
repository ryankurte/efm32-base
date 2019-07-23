/***************************************************************************//**
 * @file
 * @brief Provide MicroSD SPI configuration parameters.
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

#ifndef __MICROSDCONFIG_H
#define __MICROSDCONFIG_H

/* Don't increase MICROSD_HI_SPI_FREQ beyond 24MHz. */
#define MICROSD_HI_SPI_FREQ     24000000

#define MICROSD_LO_SPI_FREQ     100000
#define MICROSD_USART           USART0
#define MICROSD_LOC             USART_ROUTE_LOCATION_LOC1
#define MICROSD_CMUCLOCK        cmuClock_USART0
#define MICROSD_GPIOPORT        gpioPortE
#define MICROSD_MOSIPIN         7
#define MICROSD_MISOPIN         6
#define MICROSD_CSPIN           4
#define MICROSD_CLKPIN          5

#endif /* __MICROSDCONFIG_H */
