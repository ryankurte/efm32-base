/***************************************************************************//**
 * @file
 * @brief Command Line Interface configuration file
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

#ifndef CLI_CONFIG_H
#define CLI_CONFIG_H

#include <stdio.h>

#define CLI_CONFIG_DEFAULT_VSPRINTF_FUNCTION    vsprintf

#define CLI_CONFIG_CMDLINE_MAX_STRING_LENGTH    80
#define CLI_CONFIG_CMDLINE_MAX_ARGUMENT_COUNT   16
#define CLI_CONFIG_CMDLINE_HISTORY_COUNT        5
#define CLI_CONFIG_CMDLINE_DEFAULT_TIMEOUT      1
#define CLI_CONFIG_MAX_INLINE_HELP_CMDS         3

#define CLI_CONFIG_PRINTF_BUFFER_SIZE           256

#define CLI_CONFIG_MAX_STRING_LENGTH            256

#define CLI_CONFIG_CMD_MAX_STRING_LENGTH        16

#define CLI_CONFIG_LOGIN_ATTEMPT_COUNT          3
#define CLI_CONFIG_LOGOUT_FUNC_MAX_ARGS         3

#define CLI_CONFIG_USER_LEVEL_MAX               10
#define CLI_CONFIG_USER_MAX_COUNT               1
#define CLI_CONFIG_USER_NAME_MAX_LENGTH         8
#define CLI_CONFIG_USER_PASS_MAX_LENGTH         8

#define CLI_CONFIG_PROMPT_STRING_LENGTH         8

#define CLI_CONFIG_MAX_CMDS                     128

#define CLI_CONFIG_CMDLINE_CHAR_COMPUTER_PREFIX ':'

#endif // CLI_CONFIG_H
