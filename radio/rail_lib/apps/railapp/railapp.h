/***************************************************************************//**
 * @file
 * @brief Header file for RAIL application functionality
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

#ifndef __RAILAPP_H__
#define __RAILAPP_H__

#include "railapp_antenna.h"

#ifdef CLI_INTERFACE
#define RAILAPP_CI_COMMANDS \
  ANTENNA_CI_COMMANDS
#endif

#endif
