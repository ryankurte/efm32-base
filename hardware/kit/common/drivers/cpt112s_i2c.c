/***************************************************************************//**
 * @file
 * @brief helper functions for managing capsense inputs
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

#include "cpt112s_i2c.h"
#include "cpt112s_config.h"
#include "em_chip.h"
#include "em_cmu.h"
#include "em_i2c.h"
#include "em_gpio.h"
#include "em_device.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/***************************************************************************//**
 * @addtogroup kitdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup CPT112S_I2C
 * @{
 ******************************************************************************/

/*******************************************************************************
 ***************************** LOCAL VARIABLES *********************************
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

// GPIO data structure declaration
static uint16_t capsenseCurrent, sliderCurrent;
static uint16_t capsensePrevious, sliderPrevious;

// Buffers
static uint8_t i2c_rxBuffer[CPT112S_I2C_RXBUFFER_SIZE];

// Transmission flags
static volatile bool i2c_rxInProgress;
static volatile bool i2c_startTx;

/*******************************************************************************
 *******************************  PROTOTYPES ***********************************
 ******************************************************************************/

static void setupOscillators(void);
static void setupI2C(void);
static void performI2CTransfer(void);
static void parseI2C(void);

/** @endcond */

/*******************************************************************************
 ******************************  FUNCTIONS   ***********************************
 ******************************************************************************/

/******************************************************************************
 * @brief Return a bitmask containing the current state for all capsense
 * buttons
 *****************************************************************************/
uint16_t CPT112S_getCapsenseCurrent(void)
{
  return capsenseCurrent;
}

/******************************************************************************
 * @brief Return a bitmask containing the previous state for all capsense
 * buttons
 *****************************************************************************/
uint16_t CPT112S_getCapsensePrevious(void)
{
  return capsensePrevious;
}

/******************************************************************************
 * @brief Return the current slider value
 *****************************************************************************/
uint16_t CPT112S_getSliderCurrent(void)
{
  return sliderCurrent;
}

/******************************************************************************
 * @brief Return the previous slider value
 *****************************************************************************/
uint16_t CPT112S_getSliderPrevious(void)
{
  return sliderPrevious;
}

/******************************************************************************
 * @brief GPIO data structure initialization
 *****************************************************************************/
void CPT112S_init(void)
{
  setupOscillators();
  setupI2C();

  capsenseCurrent = 0;
  capsensePrevious = 0;
  sliderCurrent = 0xFFFF;
  sliderPrevious = 0xFFFF;
}

/******************************************************************************
 * @brief Parse i2c packet received from slave, update i2c data structure
 *****************************************************************************/
void CPT112S_update(void)
{
  // get previous states of Cap-sense button array
  capsensePrevious = capsenseCurrent;
  sliderPrevious = sliderCurrent;

  while (!GPIO_PinInGet(CS0_I2C_INT_PORT, CS0_I2C_INT_PIN)) {
    // update current button states
    parseI2C();
  }
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/*******************************************************************************
 ***************************** LOCAL FUNCTIONS *********************************
 ******************************************************************************/
void parseI2C()
{
  performI2CTransfer();

  if ((i2c_rxBuffer[0] & 0x0F) == CPT112S_I2C_TOUCH_EVENT) {
    sliderCurrent = 0xFFFF;
    capsenseCurrent |= 1 << i2c_rxBuffer[1];
  } else if ((i2c_rxBuffer[0] & 0x0F) == CPT112S_I2C_RELEASE_EVENT) {
    sliderCurrent = 0xFFFF;
    capsenseCurrent &= ~(1 << i2c_rxBuffer[1]);
  } else if ((i2c_rxBuffer[0] & 0x0F) == CPT112S_I2C_SLIDER_ACTIVITY) {
    sliderCurrent = (i2c_rxBuffer[1] << 8) | (i2c_rxBuffer[2]);
  }
}

/**************************************************************************//**
 * @brief Starting oscillators and enabling clocks
 *****************************************************************************/
void setupOscillators(void)
{
  /* Enabling clock to the I2C, GPIO, LE */
  CMU_ClockEnable(cmuClock_I2C0, true);
  CMU_ClockEnable(cmuClock_GPIO, true);

  /* Starting LFXO and waiting until it is stable */
  CMU_OscillatorEnable(cmuOsc_LFRCO, true, true);

  /* Routing the LFXO clock to the RTC */
  CMU_ClockSelectSet(cmuClock_LFA, cmuSelect_LFRCO);
}

/**************************************************************************//**
 * @brief Setup I2C
 *****************************************************************************/
void setupI2C(void)
{
#ifdef I2C_INIT_FAST_MODE
  // Using default settings
  I2C_Init_TypeDef i2cInit = I2C_INIT_FAST_MODE;
  #else
  // Using default settings
  I2C_Init_TypeDef i2cInit = I2C_INIT_DEFAULT;
#endif

#ifdef CS0_SENSOR_EN_PORT
  /* Enable sensor isolation switch for external pull-up resistors */
  GPIO_PinModeSet(CS0_SENSOR_EN_PORT, CS0_SENSOR_EN_PIN, gpioModePushPull, 1);
#endif

  /* Configure SDA/SCL */
  GPIO_PinModeSet(CS0_I2C_SDA_PORT, CS0_I2C_SDA_PIN, gpioModeWiredAndPullUpFilter, 1);
  GPIO_PinModeSet(CS0_I2C_SCL_PORT, CS0_I2C_SCL_PIN, gpioModeWiredAndPullUpFilter, 1);

  // Configure interrupt pin
  GPIO_PinModeSet(CS0_I2C_INT_PORT, CS0_I2C_INT_PIN, gpioModeInput, 0);

#ifdef CS0_I2C_SDA_LOC
  /* Enable pins */
  I2C0->ROUTEPEN = I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;
  I2C0->ROUTELOC0 = (CS0_I2C_SDA_LOC << _I2C_ROUTELOC0_SDALOC_SHIFT)
                    | (CS0_I2C_SCL_LOC << _I2C_ROUTELOC0_SCLLOC_SHIFT);
#else
  /* Enable pins at location */
  I2C0->ROUTE = I2C_ROUTE_SDAPEN
                | I2C_ROUTE_SCLPEN
                | (CS0_I2C_LOC << _I2C_ROUTE_LOCATION_SHIFT);
#endif

  /* Initializing the I2C */
  I2C_Init(I2C0, &i2cInit);

  /* Setting the status flags and index */
  i2c_rxInProgress = false;
  i2c_startTx = false;

  I2C0->CTRL |= I2C_CTRL_AUTOACK | I2C_CTRL_AUTOSN;
}

/**************************************************************************//**
 * @brief Transmitting I2C data. Will busy-wait until the transfer is complete.
 *****************************************************************************/
void performI2CTransfer(void)
{
  /* Transfer structure */
  I2C_TransferSeq_TypeDef i2cTransfer;

  /* Initializing I2C transfer */
  i2cTransfer.addr          = CPT112S_I2C_ADDRESS;
  i2cTransfer.flags         = I2C_FLAG_READ;
  i2cTransfer.buf[0].data   = i2c_rxBuffer;
  i2cTransfer.buf[0].len    = CPT112S_I2C_RXBUFFER_SIZE;
  I2C_TransferInit(I2C0, &i2cTransfer);

  /* Sending data */
  while (I2C_Transfer(I2C0) == i2cTransferInProgress) ;
}

/** @endcond */

/** @} (end group CPT112S_I2C) */
/** @} (end group kitdrv) */
