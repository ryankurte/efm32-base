/***************************************************************************//**
 * @file
 * @brief  * @brief Token configuration parameters
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

#ifndef TOKEN_CONFIG_H
#define TOKEN_CONFIG_H

#define TOKEN_DEVINFO_BASE_ADDRESS    0x0FE08000
#define TOKEN_LOCKBITS_BASE_ADDRESS   0x0FE04000
#define TOKEN_MAINFLASH_BASE_ADDRESS  0x00000000
#define TOKEN_USERDATA_BASE_ADDRESS   0x0FE00000

#define TOKEN_COUNT 8

#define SB_RADIO_CTUNE_OFFSET          0x0100
#define SB_RADIO_CTUNE_ADDR            (TOKEN_USERDATA_BASE_ADDRESS + SB_RADIO_CTUNE_OFFSET)
#define SB_RADIO_CTUNE_SIZE            2

#define SB_RADIO_OUTPUT_POWER_OFFSET   0x0702
#define SB_RADIO_OUTPUT_POWER_ADDR     (TOKEN_USERDATA_BASE_ADDRESS + SB_RADIO_OUTPUT_POWER_OFFSET)
#define SB_RADIO_OUTPUT_POWER_SIZE     2

#define SB_RADIO_CHANNEL_OFFSET        0x0704
#define SB_RADIO_CHANNEL_ADDR          (TOKEN_USERDATA_BASE_ADDRESS + SB_RADIO_CHANNEL_OFFSET)
#define SB_RADIO_CHANNEL_SIZE          2

#define SB_RADIO_NODE_ID_OFFSET        0x0706
#define SB_RADIO_NODE_ID_ADDR          (TOKEN_USERDATA_BASE_ADDRESS + SB_RADIO_NODE_ID_OFFSET)
#define SB_RADIO_NODE_ID_SIZE          1

#define SB_RADIO_NODE_ROLE_OFFSET      0x0707
#define SB_RADIO_NODE_ROLE_ADDR        (TOKEN_USERDATA_BASE_ADDRESS + SB_RADIO_NODE_ROLE_OFFSET)
#define SB_RADIO_NODE_ROLE_SIZE        1

#define SB_RADIO_BEACON_PERIOD_OFFSET  0x0708
#define SB_RADIO_BEACON_PERIOD_ADDR    (TOKEN_USERDATA_BASE_ADDRESS + SB_RADIO_BEACON_PERIOD_OFFSET)
#define SB_RADIO_BEACON_PERIOD_SIZE    2

#define SB_NODE_COLOUR_OFFSET          0x0710
#define SB_NODE_COLOUR_ADDR            (TOKEN_USERDATA_BASE_ADDRESS + SB_NODE_COLOUR_OFFSET)
#define SB_NODE_COLOUR_SIZE            4

#define SB_NODE_COUNT_OFFSET           0x0714
#define SB_NODE_COUNT_ADDR             (TOKEN_USERDATA_BASE_ADDRESS + SB_NODE_COUNT_OFFSET)
#define SB_NODE_COUNT_SIZE             1

#endif // TOKEN_CONFIG_H
