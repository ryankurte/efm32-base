/***************************************************************************//**
 * @file
 * @brief helper functions for configuring SWO
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

#include "em_cmu.h"
#include "bsp_trace.h"

/**
 * @brief
 *   Output a single character on SWO.
 *
 * @detail
 *   This function will output a character on SWO using ITM channel 0.
 */
int RETARGET_WriteChar(char c)
{
  return ITM_SendChar(c);
}

/**
 * @brief
 *   This function returns 0 always
 *
 * @details
 *   SWO is a one-way link, it is not possible to read data from SWO.
 */
int RETARGET_ReadChar(void)
{
  return 0;
}

/**
 * @brief
 *   Setup SWO for output
 */
void RETARGET_SwoInit(void)
{
  BSP_TraceSwoSetup();
}
