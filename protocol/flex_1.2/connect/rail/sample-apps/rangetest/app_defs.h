/***************************************************************************//**
 * @file app_defs.h
 * @brief Defines to configure the main test application for this specific use
 * use case. These defines set things like channels, packet configuration, etc.
 * @copyright Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __APP_DEFS_H_
#define __APP_DEFS_H_

/******************************************************************************
 * Application Memory Defintions
 *****************************************************************************/
// Memory manager configuration
#define BUFFER_POOL_SIZE 15
#define MAX_BUFFER_SIZE  256

// General application memory sizes
#define APP_MAX_PACKET_LENGTH  (MAX_BUFFER_SIZE - 12) /* sizeof(RAIL_RxPacketInfo_t) == 12) */

/******************************************************************************
 * Application User Interface
 *****************************************************************************/
#define APP_DEMO_STRING_INIT "RAIL Range Test App"

#endif
