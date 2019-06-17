/***************************************************************************//**
 * @file
 * @brief Source file for RAIL antenna functionality
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

#include "command_interpreter.h"
#include "response_print.h"

#include "rail.h"
#include "rail_types.h"
#include "rail_chip_specific.h"

extern RAIL_Handle_t railHandle;
extern RAIL_AntennaConfig_t halAntennaConfig;

/***************************************************************************//**
 * @brief
 *   Select the RF Path to use for TX and RX.
 *
 * @details
 *   Provide access to set system function, where its source file changes for
 *   for each platform. Allows use with existing RPC config for all platforms.
 *
 * @param rfPath Sets the default antenna path.
 *
 ******************************************************************************/
void RAILAPP_SetRfPath(RAIL_AntennaSel_t rfPath)
{
 #ifdef  _SILICON_LABS_32B_SERIES_2
  halAntennaConfig.defaultPath = rfPath;
  RAIL_ConfigAntenna(railHandle, &halAntennaConfig);
 #else//!_SILICON_LABS_32B_SERIES_2
  (void) rfPath;
 #endif//_SILICON_LABS_32B_SERIES_2
}

#ifdef CLI_INTERFACE
void CI_SetRfPath(int argc, char **argv)
{
  uint32_t rfPath = ciGetUnsigned(argv[1]);
  RAILAPP_SetRfPath(rfPath);
  responsePrint(argv[0], "RfPath:%d", rfPath);
}
#endif
