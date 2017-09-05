/*
 * File: wstk/sensors.c
 * Description: Si701x Temperature/Humidity sensor driver
 *
 * Author(s): Szilveszter Papp, szilveszter.papp@silabs.com
 *
 * Copyright 2015 by Silicon Labs. All rights reserved.
 */

#include PLATFORM_HEADER
#include CONFIGURATION_HEADER
#include "stack/include/ember.h"

#include "hal/hal.h"

#include "bspconfig.h"

#include "em_gpio.h"
#include "em_cmu.h"

#include "i2cspm.h"
#include "i2cspmconfig.h"

#include "si7013.h"

#if defined(_EZR32_HAPPY_FAMILY) || defined(BSP_WSTK_BRD4100A) \
  || defined(BSP_WSTK_BRD4151A) || defined(BSP_WSTK_BRD4152B)  \
  || defined(BSP_WSTK_BRD4153A) || defined(BSP_WSTK_BRD4252A)
// EZR32HG boards and some EFR32 MG11 boards enable the sensor by HW.
  #define TEMP_HUMIDITY_SENSOR_HW_ENABLED
#elif defined(BOARD_EZR32)
  #define TEMP_HUMIDITY_SENSOR_GPIO_PORT  gpioPortF
  #define TEMP_HUMIDITY_SENSOR_GPIO_MODE  gpioModePushPull
  #define TEMP_HUMIDITY_SENSOR_GPIO_PIN   8
#elif defined(CORTEXM3_EFR32)
  #if defined(EFR32_SERIES1_CONFIG1_MICRO) || defined(EFR32_SERIES1_CONFIG3_MICRO)
    #define TEMP_HUMIDITY_SENSOR_GPIO_PORT  gpioPortD
    #define TEMP_HUMIDITY_SENSOR_GPIO_MODE  gpioModePushPull
    #define TEMP_HUMIDITY_SENSOR_GPIO_PIN   15
  #elif defined(EFR32_SERIES1_CONFIG2_MICRO)
    #define TEMP_HUMIDITY_SENSOR_GPIO_PORT  gpioPortB
    #define TEMP_HUMIDITY_SENSOR_GPIO_MODE  gpioModePushPull
    #define TEMP_HUMIDITY_SENSOR_GPIO_PIN   10
  #endif
#else
  #error "Humidity sensor: unsupported board!"
#endif

static I2CSPM_Init_TypeDef i2cInit = I2CSPM_INIT_DEFAULT;

bool emberAfPluginWstkSensorsInit(void)
{
  // Enable GPIO clock
  CMU_ClockEnable(cmuClock_GPIO, true);

  // Enable si7021 sensor isolation switch
  // This is enabled for EZR32HG devices by default on the radio board
#if !defined(TEMP_HUMIDITY_SENSOR_HW_ENABLED)
  GPIO_PinModeSet(TEMP_HUMIDITY_SENSOR_GPIO_PORT,
                  TEMP_HUMIDITY_SENSOR_GPIO_PIN,
                  TEMP_HUMIDITY_SENSOR_GPIO_MODE,
                  1);
#endif

  I2CSPM_Init(&i2cInit);

  return Si7013_Detect(i2cInit.port, SI7021_ADDR, NULL);
}

EmberStatus emberAfPluginWstkSensorsGetSample(uint32_t *rhData, int32_t *tData)
{
  return ((Si7013_MeasureRHAndTemp(i2cInit.port,
                                   SI7021_ADDR,
                                   rhData,
                                   tData) == 0)
          ? EMBER_SUCCESS
          : EMBER_ERR_FATAL);
}
