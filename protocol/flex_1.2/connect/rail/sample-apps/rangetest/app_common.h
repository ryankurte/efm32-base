/***************************************************************************//**
 * @file app_common.h
 * @brief This header file defines variables to be shared between the main
 * test application and customer specific sections.
 * @copyright Copyright 2016 Silicon Laboratories, Inc. http://www.silabs.com
 ******************************************************************************/

#ifndef __APPS_COMMON_H__
#define __APPS_COMMON_H__

#include "em_gpio.h" // For ButtonArray definition
#ifdef CONFIGURATION_HEADER
#include CONFIGURATION_HEADER
#endif

/******************************************************************************
 * Constants
 *****************************************************************************/
#define COUNTOF(a) (sizeof(a) / sizeof(a[0]))

/******************************************************************************
 * Variable Export
 *****************************************************************************/
typedef struct ButtonArray{
  GPIO_Port_TypeDef   port;
  unsigned int        pin;
} ButtonArray_t;

#endif // __APPS_COMMON_H__
