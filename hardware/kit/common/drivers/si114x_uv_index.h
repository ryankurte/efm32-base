/***************************************************************************//**
 * @file
 * @brief Si114x UV Index measurement
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

#ifndef __SI1146_UV_INDEX_H
#define __SI1146_UV_INDEX_H

int Si114x_MeasureUVIndex(u16 *uvIndex);

int Si114x_ConfigureUV(int fullInit, HANDLE *handle);

int Si114x_EnableVisRange(int vis_range_bit);

#endif
