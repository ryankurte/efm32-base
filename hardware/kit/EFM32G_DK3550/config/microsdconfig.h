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

/* Don't increase MICROSD_HI_SPI_FREQ beyond 8MHz. Next step will be 16MHz  */
/* which is out of spec.                                                    */
#define MICROSD_HI_SPI_FREQ     8000000

#define MICROSD_LO_SPI_FREQ     100000
#define MICROSD_USART           USART0
#define MICROSD_LOC             USART_ROUTE_LOCATION_LOC2
#define MICROSD_CMUCLOCK        cmuClock_USART0
#define MICROSD_GPIOPORT        gpioPortC
#define MICROSD_MOSIPIN         11
#define MICROSD_MISOPIN         10
#define MICROSD_CSPIN           8
#define MICROSD_CLKPIN          9

#endif /* __MICROSDCONFIG_H */
