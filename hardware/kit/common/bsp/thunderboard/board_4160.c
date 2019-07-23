/***************************************************************************//**
 * @file
 * @brief BOARD module source file
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

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "i2cspm.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "em_prs.h"
#include "em_timer.h"
#include "em_usart.h"
#include "bspconfig.h"
#include "mx25flash_config.h"

#include "thunderboard/board.h"
#include "thunderboard/util.h"
#include "thunderboard/board_rgbled_profiles.h"
#include "thunderboard/board_pic_regs.h"

/**************************************************************************//**
* @addtogroup TBSense_BSP
* @{
******************************************************************************/

/***************************************************************************//**
 * @defgroup BOARD_4160 BOARD Module for Thunderboard Sense
 * @{
 * @brief Board hardware control, configuraton and miscellaneous functions
 * @details This module contains functions releated to board features. It allows
 *  control over power management features, interrupt controller and RGB LEDs.
 *
 * The BOARD module uses the common I2CSPM driver to communicate with the
 * on-board Power and Interrupt Controller over I<sup>2</sup>C. The following
 * board features can be enabled when needed using the BOARD Module:
 * - Environmental sensor group
 *   - RH/Temp (Si7021)
 *   - UV/ALS (Si1133)
 *   - Barometric Pressure (BMP280)
 * - Indoor air quality sensor (CCS811)
 * - Inertial sensor (ICM-20648)
 * - Microphone (SPV1840)
 * - RGB LED boost converter
 *
 * The BOARD Module also contains functions for controlling the RGB LEDs. The
 * implementation uses one of the TIMER peripherals in PWM mode, with three
 * compare capture channels, one each for red, green and blue.
 *
 ******************************************************************************/

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

#define BOARD_ENABLED_NONE      0
#define BOARD_ENABLED_BAP       (1 << 0)
#define BOARD_ENABLED_RHTEMP    (1 << 1)
#define BOARD_ENABLED_ALS       (1 << 2)
#define BOARD_ENABLED_HALL      (1 << 3)
#define BOARD_ENABLED_GAS       (1 << 4)
#define BOARD_ENABLED_MIC       (1 << 5)
#define BOARD_ENABLED_IMU       (1 << 6)
#define BOARD_ENABLED_ENV_SENSOR (BOARD_ENABLED_BAP | BOARD_ENABLED_RHTEMP | BOARD_ENABLED_ALS | BOARD_ENABLED_HALL)

/*******************************************************************************
 *******************************   TYPEDEFS   **********************************
 ******************************************************************************/

typedef struct {
  uint16_t enabled;
  uint16_t busInUse;
} BOARD_SensorInfo_t;

/****************************************************************************/
/* Local variables                                                          */
/****************************************************************************/
static I2C_TypeDef        *i2cDevice;                                  /*  I2C device used to communicate with the IO expander         */
static bool               isLegacyIntCtrl;                             /*  Shows if the legacy interrupt control method should be used */
static TIMER_TypeDef      *pwmTimer;                                   /*  Timer used to generate PWM for the RGB LEDs                 */
static const uint8_t      lightLevels[] = BOARD_RGBLED_PROFILE_EXP;    /*  Array to linearize the light level of the RGB LEDs          */
static bool               rgbledEnable;                                /*  The status of the RGB LEDs, enabled or disabled             */
static bool               timerEnable;                                 /*  The status of the PWM timer, enabled or disabled            */
static BOARD_SensorInfo_t sensorInfo;                                  /* Structure holding usage and enable status of the sensors     */
static BOARD_IrqCallback  picIRQCallback;                              /* PIC interrupt callback function                              */
static BOARD_IrqCallback  pushButton0IRQCallback;                      /* Pusbutton 0 interrupt callback function                      */
static BOARD_IrqCallback  pushButton1IRQCallback;                      /* Pusbutton 1 interrupt callback function                      */

/****************************************************************************/
/* Local function prototypes                                                */
/****************************************************************************/
static void        flashSPIInit          (void);
static void        gpioInterruptHandler  (uint8_t pin);
static inline void picWake               (void);
static inline void picSleep              (void);
static uint32_t    picReadReg            (uint8_t addr, uint8_t *result);
static uint32_t    picWriteReg           (uint8_t addr, uint8_t value);

/** @endcond */

/***************************************************************************//**
 * @brief
 *    Initializes the Thunderboard Sense board
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_init(void)
{
  TIMER_Init_TypeDef   timerInit      = TIMER_INIT_DEFAULT;
  TIMER_InitCC_TypeDef ccInit         = TIMER_INITCC_DEFAULT;
  I2CSPM_Init_TypeDef  i2cInit        = I2CSPM_INIT_DEFAULT;
  int timeout;
  int i;
  uint32_t picdevid;
  uint8_t major, minor, patch;
  uint32_t status;

  rgbledEnable    = false;
  timerEnable     = false;
  isLegacyIntCtrl = true;

  sensorInfo.enabled = 0x00;
  sensorInfo.busInUse = 0x00;

  status = BOARD_OK;

  /* Enable GPIO clock */
  CMU_ClockEnable(cmuClock_GPIO, true);

  /*********************************************************************/
  /** Push button config                                              **/
  /*********************************************************************/
  GPIO_PinModeSet(BOARD_BUTTON_LEFT_PORT, BOARD_BUTTON_LEFT_PIN, gpioModeInput, 0);
  GPIO_PinModeSet(BOARD_BUTTON_RIGHT_PORT, BOARD_BUTTON_RIGHT_PIN, gpioModeInput, 0);

  /*********************************************************************/
  /** PIC Config                                                      **/
  /*********************************************************************/
  GPIO_PinModeSet(BOARD_PIC_INT_WAKE_PORT, BOARD_PIC_INT_WAKE_PIN, gpioModeWiredAnd, 1);
  GPIO_IntConfig(BOARD_PIC_INT_WAKE_PORT, BOARD_PIC_INT_WAKE_PIN, false, true, true);

  I2CSPM_Init(&i2cInit);
  i2cDevice   = i2cInit.port;

  /*********************************************************************/
  /** Wait for IO-expander to boot up                                 **/
  /*********************************************************************/
  timeout = 50;
  while ( timeout-- > 0 ) {
    status = BOARD_picWriteReg(0, 0);
    if ( status == BOARD_OK ) {
      break;
    }
    UTIL_delay(10);
  }

  if ( status == BOARD_OK ) {
    picdevid = BOARD_picGetDeviceId();
    if ( picdevid != BOARD_PIC_DEVICE_ID ) {
      status = BOARD_ERROR_PIC_ID_MISMATCH;
    }
  }

  if ( status == BOARD_OK ) {
    picWake();
    status = BOARD_picGetFwRevision(&major, &minor, &patch);
    picSleep();
    if ( status == BOARD_OK ) {
      if ( major == 0 && minor <= 3 ) {
        isLegacyIntCtrl = true;
      } else {
        isLegacyIntCtrl = false;
      }
    }
  }

  /*********************************************************************/
  /** PIC default register content                                    **/
  /*********************************************************************/
  picWake();
  for ( i = 0; i < BOARD_PIC_NUM_APP_REGS; i++ ) {
    picWriteReg(i, 0);
  }
  picSleep();

  /*********************************************************************/
  /** RGBLED / LED config                                             **/
  /*********************************************************************/
  CMU_ClockEnable(BOARD_RGBLED_CMU_CLK, true);
  GPIO_PinModeSet(BOARD_RGBLED_RED_PORT, BOARD_RGBLED_RED_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(BOARD_RGBLED_GREEN_PORT, BOARD_RGBLED_GREEN_PIN, gpioModePushPull, 0);
  GPIO_PinModeSet(BOARD_RGBLED_BLUE_PORT, BOARD_RGBLED_BLUE_PIN, gpioModePushPull, 0);

  pwmTimer = BOARD_RGBLED_TIMER;

  timerInit.debugRun = true;
  timerInit.prescale = timerPrescale2;
  TIMER_Init(pwmTimer, &timerInit);

  /* 65536 counts at 38.4 MHz / 2 =  293 Hz. */
  TIMER_TopSet(pwmTimer, 0xFFFF);

  ccInit.mode  = timerCCModePWM;
  TIMER_InitCC(pwmTimer, 0, &ccInit);
  TIMER_InitCC(pwmTimer, 1, &ccInit);
  TIMER_InitCC(pwmTimer, 2, &ccInit);

  pwmTimer->ROUTEPEN  = 0;
  pwmTimer->ROUTELOC0 = (BOARD_RGBLED_RED_CCLOC   << _TIMER_ROUTELOC0_CC0LOC_SHIFT)
                        | (BOARD_RGBLED_GREEN_CCLOC << _TIMER_ROUTELOC0_CC1LOC_SHIFT)
                        | (BOARD_RGBLED_BLUE_CCLOC  << _TIMER_ROUTELOC0_CC2LOC_SHIFT);

  return status;
}

/***************************************************************************//**
 * @brief
 *    Puts the Flash chip in deep power down mode
 *
 * @return
 *    Returns none
 ******************************************************************************/
void BOARD_flashDeepPowerDown(void)
{
  /* Initialize USART for SPI transactions to the flash */
  flashSPIInit();

  /* Wake up device first if it was in DP already */
  GPIO_PinOutClear(MX25_PORT_CS, MX25_PIN_CS);
  UTIL_delay(10);
  GPIO_PinOutSet(MX25_PORT_CS, MX25_PIN_CS);

  /* Send DP command ( 0xB9 ) */
  GPIO_PinOutClear(MX25_PORT_CS, MX25_PIN_CS);
  USART_SpiTransfer(MX25_USART, 0xB9);
  GPIO_PinOutSet(MX25_PORT_CS, MX25_PIN_CS);

  /* We must disable SPI communication */
  USART_Reset(MX25_USART);
  GPIO_PinModeSet(MX25_PORT_MOSI, MX25_PIN_MOSI, gpioModeDisabled, 0);
  GPIO_PinModeSet(MX25_PORT_MISO, MX25_PIN_MISO, gpioModeDisabled, 0);
  GPIO_PinModeSet(MX25_PORT_SCLK, MX25_PIN_SCLK, gpioModeDisabled, 0);

  return;
}

/***************************************************************************//**
 * @brief
 *    Gets the state of the pushbuttons
 *
 * @return
 *    Returns the state of the buttons
 ******************************************************************************/
uint8_t BOARD_pushButtonGetState(void)
{
  uint32_t portState;
  uint8_t  buttonState;

  portState   = GPIO_PortInGet(BOARD_BUTTON_PORT);
  buttonState = (uint8_t) ( (portState >> BOARD_BUTTON_SHIFT) & BOARD_BUTTON_MASK);

  buttonState = ~buttonState & BOARD_BUTTON_MASK;

  return buttonState;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the pushbutton GPIO interrupt
 *
 * @param[in] enable
 *    Set true to enable, false to disable the pushbutton interrupts
 *
 * @return
 *    None
 ******************************************************************************/
void BOARD_pushButtonEnableIRQ(bool enable)
{
  GPIO_IntConfig(BOARD_BUTTON_PORT, BOARD_BUTTON_LEFT_PIN, false, true, enable);
  GPIO_IntConfig(BOARD_BUTTON_PORT, BOARD_BUTTON_RIGHT_PIN, false, true, enable);
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the RGB LED power supply line
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    None
 ******************************************************************************/
void BOARD_rgbledPowerEnable(bool enable)
{
  BOARD_picRegBitsSet(BOARD_PIC_REG_LED_CTRL, enable, BOARD_PIC_REG_LED_CTRL_PWR_EN);

  return;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the RGB LED power supply line
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @param[in] mask
 *    Mask indicating which bits to modify
 *
 * @return
 *    None
 ******************************************************************************/
void BOARD_rgbledEnable(bool enable, uint8_t mask)
{
  if ( ( (mask != 0) && (enable == true) ) || ( ( (mask & 0xf) == 0xf) && (enable == false) ) ) {
    BOARD_rgbledPowerEnable(enable);
    rgbledEnable = enable;
  }
  mask = (mask << BOARD_PIC_REG_LED_CTRL_LED_SHIFT) & BOARD_PIC_REG_LED_CTRL_LED_MASK;
  BOARD_picRegBitsSet(BOARD_PIC_REG_LED_CTRL, enable, mask);

  return;
}

/***************************************************************************//**
 * @brief
 *    Turns on or off the red and/or green LED
 *
 * @param[in] leds
 *    The two LSB bits determine the state of the green and red LED. If the bit
 *    is 1 then the LED will be turned on.
 *
 * @return
 *    None
 ******************************************************************************/
void BOARD_ledSet(uint8_t leds)
{
  if ( rgbledEnable && leds != 0 ) {
    /* Reset PWM colors and stop timer */
    BOARD_rgbledSetRawColor(0, 0, 0);
    BOARD_rgbledEnable(false, 0xFF);
  }
  uint8_t pins[2] = { BOARD_RGBLED_GREEN_PIN, BOARD_RGBLED_RED_PIN };
  for ( int i = 0; i < 2; i++ ) {
    if ( ( (leds >> i) & 1) == 1 ) {
      GPIO_PinOutSet(BOARD_RGBLED_RED_PORT, pins[i]);
    } else {
      GPIO_PinOutClear(BOARD_RGBLED_RED_PORT, pins[i]);
    }
  }

  return;
}

/***************************************************************************//**
 * @brief
 *    Sets the raw color of the RGB LEDs. The brightness is non-linear function
 *    of the raw color value.
 *
 * @param[in] red
 *    A 16-bit value, which determines the PWM value of the red channel
 *
 * @param[in] green
 *    A 16-bit value, which determines the PWM value of the green channel
 *
 * @param[in] blue
 *    A 16-bit value, which determines the PWM value of the blue channel
 *
 * @return
 *    None
 ******************************************************************************/
void BOARD_rgbledSetRawColor(uint16_t red, uint16_t green, uint16_t blue)
{
  if ( (red == 0) && (green == 0) && (blue == 0) ) {
    timerEnable = false;
    TIMER_Enable(pwmTimer, timerEnable);
    TIMER_CompareBufSet(pwmTimer, 0, 0);
    TIMER_CompareBufSet(pwmTimer, 1, 0);
    TIMER_CompareBufSet(pwmTimer, 2, 0);
    /* Ensure LED pins are disabled before changing ROUTE */
    BOARD_ledSet(0);
    pwmTimer->ROUTEPEN  = 0;
  } else {
    timerEnable = true;
    TIMER_Enable(pwmTimer, timerEnable);
    TIMER_CompareBufSet(pwmTimer, 0, red);
    TIMER_CompareBufSet(pwmTimer, 1, green);
    TIMER_CompareBufSet(pwmTimer, 2, blue);
    pwmTimer->ROUTEPEN  = TIMER_ROUTEPEN_CC0PEN | TIMER_ROUTEPEN_CC1PEN | TIMER_ROUTEPEN_CC2PEN;
  }

  return;
}

/***************************************************************************//**
 * @brief
 *    Sets the color of the RGB LEDs. The brightness of the LEDs is almost
 *    linear to the color value.
 *
 * @param[in] red
 *    An 8-bit value, which determines the brightness of the red channel
 *
 * @param[in] green
 *    An 8-bit value, which determines the brightness of the green channel
 *
 * @param[in] blue
 *    An 8-bit value, which determines the brightness of the blue channel
 *
 * @return
 *    None
 ******************************************************************************/
void BOARD_rgbledSetColor(uint8_t red, uint8_t green, uint8_t blue)
{
  BOARD_rgbledSetRawColor(256 * (uint16_t)lightLevels[red],
                          256 * (uint16_t)lightLevels[green],
                          256 * (uint16_t)lightLevels[blue]);

  return;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the accelerometer and gyroscope sensor
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_imuEnable(bool enable)
{
  uint32_t status;

  status = BOARD_picWriteReg(BOARD_PIC_REG_IMU_CTRL, enable);

  if ( status == BOARD_OK ) {
    if ( enable ) {
      sensorInfo.enabled |= BOARD_ENABLED_IMU;
    } else {
      sensorInfo.enabled &= ~BOARD_ENABLED_IMU;
    }
  }

  return status;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the accelerometer and gyroscope GPIO interrupt
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_imuEnableIRQ(bool enable)
{
  uint32_t status;

  /* Enable SPI interrupts in IO-expander */
  status = BOARD_picRegBitsSet(BOARD_PIC_REG_INT_ENABLE, enable, BOARD_PIC_REG_INT_ENABLE_IMU);

  return status;
}

/***************************************************************************//**
 * @brief
 *    Clears the accelerometer and gyroscope GPIO interrupt
 *    In the ThunderBoard Sense app example the IMU registers need to be read
 *    at quite high rate. Clearing the PIC interrupt register would take too
 *    much time. Due to the nature of this example only the IMU interrupt is
 *    active and the program can work correctly without identifying the
 *    interrupt source and clearing the IT flag in the PIC.
 *    If speed is not a limiting factor then always clear the PIC interrupt
 *    register by calling the BOARD_picIntClear(BOARD_PIC_REG_INT_CLEAR_IMU);
 *    function.
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
void BOARD_imuClearIRQ(void)
{
  GPIO_IntClear(1 << EXTI_PIC_INT);

  /* Clear SPI interrupts in IO-expander */
  // BOARD_picIntClear(BOARD_PIC_REG_INT_CLEAR_IMU);

  return;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the environmental sensor group (Pressure, RH/Temp,
 *    UV/Ambient light and Hall sensors)
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_envSensEnable(bool enable)
{
  uint32_t status;

  status = BOARD_picWriteReg(BOARD_PIC_REG_ENV_SENSOR_CTRL, enable);

  return status;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the Barometric Pressure sensor
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_bapEnable(bool enable)
{
  uint32_t status;

  status = BOARD_OK;

  if ( enable ) {
    /* Enable the environmental sensor group */
    status = BOARD_envSensEnable(true);
    /* If it was succesful then set the enabled bit in the sensorInfo structure for this sensor*/
    if ( status == BOARD_OK ) {
      sensorInfo.enabled |= BOARD_ENABLED_BAP;
    }
  } else {
    /* Check if other sensors enabled */
    if ( (sensorInfo.enabled & ~BOARD_ENABLED_BAP) & BOARD_ENABLED_ENV_SENSOR ) {
      /* If yes then just clear the enabled bit but do not turn off the power */
      sensorInfo.enabled &= ~BOARD_ENABLED_BAP;
    } else {
      /* If no other sensors enabled then turn off the power of the sensor group */
      status = BOARD_envSensEnable(false);
      if ( status == BOARD_OK ) {
        sensorInfo.enabled &= ~BOARD_ENABLED_BAP;
      }
    }
  }

  return status;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the RH/Temp sensor
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_rhtempEnable(bool enable)
{
  uint32_t status;

  status = BOARD_OK;

  if ( enable ) {
    /* Enable the environmental sensor group */
    status = BOARD_envSensEnable(true);
    /* If it was succesful then set the enabled bit in the sensorInfo structure for this sensor*/
    if ( status == BOARD_OK ) {
      sensorInfo.enabled |= BOARD_ENABLED_RHTEMP;
    }
  } else {
    /* Check if other sensors enabled */
    if ( (sensorInfo.enabled & ~BOARD_ENABLED_RHTEMP) & BOARD_ENABLED_ENV_SENSOR ) {
      /* If yes then just clear the enabled bit but do not turn off the power */
      sensorInfo.enabled &= ~BOARD_ENABLED_RHTEMP;
    } else {
      /* If no other sensors enabled then turn off the power of the sensor group */
      status = BOARD_envSensEnable(false);
      if ( status == BOARD_OK ) {
        sensorInfo.enabled &= ~BOARD_ENABLED_RHTEMP;
      }
    }
  }

  return status;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the UV/Ambient light sensor
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_alsEnable(bool enable)
{
  uint32_t status;

  status = BOARD_OK;

  if ( enable ) {
    /* Enable the environmental sensor group */
    status = BOARD_envSensEnable(true);
    /* If it was succesful then set the enabled bit in the sensorInfo structure for this sensor*/
    if ( status == BOARD_OK ) {
      sensorInfo.enabled |= BOARD_ENABLED_ALS;
    }
  } else {
    /* Check if other sensors enabled */
    if ( (sensorInfo.enabled & ~BOARD_ENABLED_ALS) & BOARD_ENABLED_ENV_SENSOR ) {
      /* If yes then just clear the enabled bit but do not turn off the power */
      sensorInfo.enabled &= ~BOARD_ENABLED_ALS;
    } else {
      /* If no other sensors enabled then turn off the power of the sensor group */
      status = BOARD_envSensEnable(false);
      if ( status == BOARD_OK ) {
        sensorInfo.enabled &= ~BOARD_ENABLED_ALS;
      }
    }
  }

  return status;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the UV/Ambient light sensor GPIO interrupts
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_alsEnableIRQ(bool enable)
{
  uint32_t status;

  /* Enable I2C interrupts in IO-expander */
  status = BOARD_picRegBitsSet(BOARD_PIC_REG_INT_ENABLE, enable, BOARD_PIC_REG_INT_ENABLE_UV_ALS);

  return status;
}

/***************************************************************************//**
 * @brief
 *    Clears the UV/Ambient light sensor GPIO interrupts
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
void BOARD_alsClearIRQ(void)
{
  GPIO_IntClear(1 << EXTI_PIC_INT);

  /* Clear I2C interrupt in IO-expander */
  BOARD_picIntClear(BOARD_PIC_REG_INT_CLEAR_UV_ALS);

  return;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the environmental sensor group (Pressure, RH/Temp,
 *    UV/Ambient light and Hall sensors) GPIO interrupts
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_envSensEnableIRQ(bool enable)
{
  uint32_t status;

  /* Enable I2C interrupts in IO-expander */
  status = BOARD_picRegBitsSet(BOARD_PIC_REG_INT_ENABLE, enable, BOARD_PIC_REG_INT_ENABLE_UV_ALS);

  return status;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the MEMS microphone
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_micEnable(bool enable)
{
  uint32_t status;

  status = BOARD_picWriteReg(BOARD_PIC_REG_MIC_CTRL, enable);

  return status;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the Air Quality / Gas Sensor
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_gasSensorEnable(bool enable)
{
  uint32_t status;

  if ( enable ) {
    sensorInfo.enabled |= BOARD_ENABLED_GAS;
  } else {
    sensorInfo.enabled &= ~BOARD_ENABLED_GAS;
  }

  if ( enable ) {
    status = BOARD_picWriteReg(BOARD_PIC_REG_CCS_CTRL, BOARD_PIC_REG_CCS_CTRL_EN);
  } else {
    status = BOARD_picWriteReg(BOARD_PIC_REG_CCS_CTRL, 0);
  }

  return status;
}

/***************************************************************************//**
 * @brief
 *    Wakes up the Air Quality / Gas Sensor
 *
 * @param[in] wake
 *    Set true to wake up, false otherwise
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_gasSensorWake(bool wake)
{
  uint32_t status;

  if ( wake ) {
    status = BOARD_picWriteReg(BOARD_PIC_REG_CCS_CTRL, BOARD_PIC_REG_CCS_CTRL_EN | BOARD_PIC_REG_CCS_CTRL_WAKE);
  } else {
    status = BOARD_picWriteReg(BOARD_PIC_REG_CCS_CTRL, BOARD_PIC_REG_CCS_CTRL_EN);
  }

  return status;
}

/***************************************************************************//**
 * @brief
 *    Enables or disables the Air Quality / Gas Sensor GPIO interrupt
 *
 * @param[in] enable
 *    Set true to enable, false to disable
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_gasSensorEnableIRQ(bool enable)
{
  uint32_t status;

  /* Enable I2C interrupts in IO-expander */
  status = BOARD_picRegBitsSet(BOARD_PIC_REG_INT_ENABLE, enable, BOARD_PIC_REG_INT_ENABLE_CCS811);

  return status;
}

/***************************************************************************//**
 * @brief
 *    Clears the Air Quality / Gas Sensor GPIO interrupt
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
void BOARD_gasSensorClearIRQ(void)
{
  GPIO_IntClear(1 << EXTI_PIC_INT);

  /* Clear I2C interrupt in IO-expander */
  BOARD_picIntClear(BOARD_PIC_REG_INT_CLEAR_CCS811);

  return;
}

/***************************************************************************//**
 * @brief
 *    Sets up the route register of the I2C device to use the correct
 *    set of pins
 *
 * * @param[in] select
 *    The I2C bus route to use (None, Environmental sensors, Gas sensor, Hall
 *    sensor)
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_i2cBusSelect(uint8_t select)
{
  uint32_t status;

  (void) select;

  status = BOARD_OK;

  return status;
}

/***************************************************************************//**
 * @brief
 *    Reads the interrupt status flags from the interrupt controller
 *
 * @param[out] flags
 *    Holds the flags that indicate which events have taken place
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_picIntGet(uint8_t *flags)
{
  uint32_t status;

  status = BOARD_picReadReg(BOARD_PIC_REG_INT_FLAG, flags);

  return status;
}

/***************************************************************************//**
 * @brief
 *    Clears the interrupt status flags in the interrupt controller
 *
 * @param[out] flags
 *    Holds the flags that indicate which events have taken place
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_picIntClear(uint8_t flags)
{
  uint32_t status;

  status = BOARD_picWriteReg(BOARD_PIC_REG_INT_CLEAR, flags);

  return status;
}

/***************************************************************************//**
 * @brief
 *    Writes a register in the IO expander
 *
 * @param[in] reg
 *    The register address to write
 *
 * @param[in] value
 *    The data to write to the register
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_picWriteReg(uint8_t reg, uint8_t value)
{
  uint32_t status;

  picWake();
  status = picWriteReg(reg, value);
  picSleep();

  return status;
}

/***************************************************************************//**
 * @brief
 *    Reads a register in the IO expander
 *
 * @param[in] reg
 *    The register address to read
 *
 * @param[out] result
 *    The data read from the device
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_picReadReg(uint8_t reg, uint8_t *result)
{
  uint32_t status;

  picWake();
  status = picReadReg(reg, result);
  picSleep();

  return status;
}

/***************************************************************************//**
 * @brief
 *    Sets the given bit(s) in a register in the IO expander
 *
 * @param[in] addr
 *    The address of the register
 *
 * @param[in] set
 *    Specifies whether the bits selected by the mask should be set or cleared
 *
 * @param[in] bitMask
 *    The mask specifies which bits should be changed
 *
 * @return
 *    None
 ******************************************************************************/
uint32_t BOARD_picRegBitsSet(uint8_t addr, bool set, uint8_t bitMask)
{
  uint32_t status;
  uint8_t  value;

  picWake();

  status = picReadReg(addr, &value);
  if ( status != BOARD_OK ) {
    return status;
  }

  if ( set ) {
    value |= bitMask;
  } else {
    value &= ~bitMask;
  }

  status = picWriteReg(addr, value);
  picSleep();

  return status;
}

/***************************************************************************//**
 * @brief
 *    Reads the device ID of the IO expander
 *
 * @return
 *    Returns the device ID read from the device
 ******************************************************************************/
uint32_t BOARD_picGetDeviceId(void)
{
  uint32_t result;
  uint8_t *pU8;

  pU8 = (uint8_t *) &result;

  BOARD_picReadReg(BOARD_PIC_REG_DEVICE_ID0, pU8++);
  BOARD_picReadReg(BOARD_PIC_REG_DEVICE_ID1, pU8++);
  BOARD_picReadReg(BOARD_PIC_REG_DEVICE_ID2, pU8++);
  BOARD_picReadReg(BOARD_PIC_REG_DEVICE_ID3, pU8++);

  return result;
}

/***************************************************************************//**
 * @brief
 *    Reads the hardware revision of the IO expander
 *
 * @return
 *    Returns the HW revision the device
 ******************************************************************************/
uint8_t BOARD_picGetHwRevision(void)
{
  uint8_t rev;

  BOARD_picReadReg(BOARD_PIC_REG_BOARD_REV, &rev);

  return rev;
}

/***************************************************************************//**
 * @brief
 *    Reads the firmware revision of the IO expander
 *
 * @param[out] major
 *    The major version number of the firmware
 *
 * @param[out] minor
 *    The minor version number of the firmware
 *
 * @param[out] patch
 *    The patch version number of the firmware
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
uint32_t BOARD_picGetFwRevision(uint8_t *major, uint8_t *minor, uint8_t *patch)
{
  uint32_t status;

  *major = 0;
  *minor = 0;
  *patch = 0;

  /* Keep int/wake low (legacy behavior) */
  GPIO_IntDisable(1 << BOARD_PIC_INT_WAKE_PIN);
  GPIO_PinOutClear(BOARD_PIC_INT_WAKE_PORT, BOARD_PIC_INT_WAKE_PIN);

  status = picReadReg(BOARD_PIC_REG_VERSION_MAJOR, major);
  if ( status != BOARD_OK ) {
    goto cleanup;
  }
  status = picReadReg(BOARD_PIC_REG_VERSION_MINOR, minor);
  if ( status != BOARD_OK ) {
    goto cleanup;
  }
  status = picReadReg(BOARD_PIC_REG_VERSION_PATCH, patch);

  cleanup:
  GPIO_IntClear(1 << BOARD_PIC_INT_WAKE_PIN);
  GPIO_IntEnable(1 << BOARD_PIC_INT_WAKE_PIN);

  GPIO_PinOutSet(BOARD_PIC_INT_WAKE_PORT, BOARD_PIC_INT_WAKE_PIN);

  return status;
}

/***************************************************************************//**
 * @brief
 *    Checks if the legacy interrupt control method should be used
 *
 * @return
 *    Returns true if the legacy method should be used (before board rev A02)
 ******************************************************************************/
bool BOARD_picIsLegacyIntCtrl(void)
{
  return isLegacyIntCtrl;
}

/** @cond DO_NOT_INCLUDE_WITH_DOXYGEN */

/****************************************************************************/
/* Local functions                                                          */
/****************************************************************************/
/***************************************************************************//**
 * @brief
 *    Performs register read through the I2C bus from the IO expander
 *
 * @param[in] addr
 *    The register address to read
 *
 * @param[out] result
 *    The data read from the device
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
static uint32_t picReadReg(uint8_t addr, uint8_t *result)
{
  uint32_t status;

  I2C_TransferSeq_TypeDef    seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr        = 0x90;
  seq.flags       = I2C_FLAG_WRITE_READ;
  seq.buf[0].len  = 1;
  seq.buf[0].data = &addr;
  seq.buf[1].len  = 1;
  seq.buf[1].data = result;

  ret = I2CSPM_Transfer(i2cDevice, &seq);

  status = BOARD_OK;

  if ( ret == i2cTransferInProgress ) {
    status = BOARD_ERROR_I2C_TRANSFER_TIMEOUT;
  } else if ( ret == i2cTransferNack ) {
    status = BOARD_ERROR_I2C_TRANSFER_NACK;
  } else if ( ret != i2cTransferDone ) {
    status = BOARD_ERROR_I2C_TRANSFER_FAILED;
  }

  return status;
}

/***************************************************************************//**
 * @brief
 *    Performs register write through the I2C bus to the IO expander
 *
 * @param[in] addr
 *    The register address to write
 *
 * @param[in] value
 *    The data to write to the register
 *
 * @return
 *    Returns zero on OK, non-zero otherwise
 ******************************************************************************/
static uint32_t picWriteReg(uint8_t addr, uint8_t value)
{
  uint32_t status;

  I2C_TransferSeq_TypeDef seq;
  I2C_TransferReturn_TypeDef ret;

  seq.addr        = 0x90;
  seq.flags       = I2C_FLAG_WRITE_WRITE;
  seq.buf[0].len  = 1;
  seq.buf[0].data = &addr;
  seq.buf[1].len  = 1;
  seq.buf[1].data = &value;

  ret = I2CSPM_Transfer(i2cDevice, &seq);

  status = BOARD_OK;

  if ( ret == i2cTransferInProgress ) {
    status = BOARD_ERROR_I2C_TRANSFER_TIMEOUT;
  } else if ( ret == i2cTransferNack ) {
    status = BOARD_ERROR_I2C_TRANSFER_NACK;
  } else if ( ret != i2cTransferDone ) {
    status = BOARD_ERROR_I2C_TRANSFER_FAILED;
  }

  return status;
}

/***************************************************************************//**
 * @brief
 *    Wakes up the IO expander
 *
 * @return
 *    None
 ******************************************************************************/
static inline void picWake(void)
{
  volatile int delay;

  GPIO_IntDisable(1 << BOARD_PIC_INT_WAKE_PIN);

  GPIO_PinOutClear(BOARD_PIC_INT_WAKE_PORT, BOARD_PIC_INT_WAKE_PIN);

  if ( !isLegacyIntCtrl ) {
    /* Minimum 5 us delay */
    delay = 90;   // starts working around 60, add some margin
    while ( --delay ) ;

    GPIO_IntClear(1 << BOARD_PIC_INT_WAKE_PIN);
    GPIO_IntEnable(1 << BOARD_PIC_INT_WAKE_PIN);

    GPIO_PinOutSet(BOARD_PIC_INT_WAKE_PORT, BOARD_PIC_INT_WAKE_PIN);
  }

  return;
}

/***************************************************************************//**
 * @brief
 *    Puts the IO Expander in sleep mode.
 *
 * @return
 *    None
 ******************************************************************************/
static inline void picSleep(void)
{
  if ( isLegacyIntCtrl ) {
    GPIO_IntClear(1 << BOARD_PIC_INT_WAKE_PIN);
    GPIO_IntEnable(1 << BOARD_PIC_INT_WAKE_PIN);

    GPIO_PinOutSet(BOARD_PIC_INT_WAKE_PORT, BOARD_PIC_INT_WAKE_PIN);
  }

  return;
}

/***************************************************************************//**
 * @brief
 *    Function to register the gas sensor interrupt callback function
 *
 * @param[in] cb
 *    The callback function to be registered
 *
 * @return
 *    Returns none
 ******************************************************************************/
void BOARD_gasSensorSetIRQCallback(BOARD_IrqCallback cb)
{
  GPIOINT_CallbackRegister(EXTI_PIC_INT, gpioInterruptHandler);
  picIRQCallback = cb;

  return;
}

/***************************************************************************//**
 * @brief
 *    Function to register the ALS sensor interrupt callback function
 *
 * @param[in] cb
 *    The callback function to be registered
 *
 * @return
 *    Returns none
 ******************************************************************************/
void BOARD_alsSetIRQCallback(BOARD_IrqCallback cb)
{
  GPIOINT_CallbackRegister(EXTI_PIC_INT, gpioInterruptHandler);
  picIRQCallback = cb;

  return;
}

/***************************************************************************//**
 * @brief
 *    Function to register the IMU sensor interrupt callback function
 *
 * @param[in] cb
 *    The callback function to be registered
 *
 * @return
 *    Returns none
 *****************************************************************************/
void BOARD_imuSetIRQCallback(BOARD_IrqCallback cb)
{
  GPIOINT_CallbackRegister(EXTI_PIC_INT, gpioInterruptHandler);
  picIRQCallback = cb;

  return;
}

/***************************************************************************//**
 * @brief
 *    Function to register the pushbutton 0 interrupt callback function
 *
 * @param[in] cb
 *    The callback function to be registered
 *
 * @return
 *    Returns none
 ******************************************************************************/
void BOARD_pushButton0SetIRQCallback(BOARD_IrqCallback cb)
{
  GPIOINT_CallbackRegister(EXTI_BUTTON0, gpioInterruptHandler);
  pushButton0IRQCallback = cb;

  return;
}

/***************************************************************************//**
 * @brief
 *    Function to clear the pushbutton 0 interrupt
 *
 * @return
 *    Returns none
 ******************************************************************************/
void BOARD_pushButton0ClearIRQ(void)
{
  GPIO_IntClear(1 << EXTI_BUTTON0);

  return;
}

/***************************************************************************//**
 * @brief
 *    Function to register the pushbutton 1 interrupt callback function
 *
 * @param[in] cb
 *    The callback function to be registered
 *
 * @return
 *    Returns none
 ******************************************************************************/
void BOARD_pushButton1SetIRQCallback(BOARD_IrqCallback cb)
{
  GPIOINT_CallbackRegister(EXTI_BUTTON1, gpioInterruptHandler);
  pushButton1IRQCallback = cb;

  return;
}

/***************************************************************************//**
 * @brief
 *    Function to clear the pushbutton 1 interrupt
 *
 * @return
 *    Returns none
 ******************************************************************************/
void BOARD_pushButton1ClearIRQ(void)
{
  GPIO_IntClear(1 << EXTI_BUTTON1);

  return;
}

/***************************************************************************//**
 * @brief
 *    Initializes the SPI bus to communicate with the Flash chip
 *
 * @return
 *    Returns none
 ******************************************************************************/
static void flashSPIInit(void)
{
  USART_InitSync_TypeDef init = USART_INITSYNC_DEFAULT;

  /* Enable clocks */
  CMU_ClockEnable(cmuClock_GPIO, true);
  CMU_ClockEnable(MX25_USART_CLK, true);

  /* Configure GPIO pins */
  GPIO_PinModeSet(MX25_PORT_MOSI, MX25_PIN_MOSI, gpioModePushPull, 1);
  GPIO_PinModeSet(MX25_PORT_MISO, MX25_PIN_MISO, gpioModeInput, 0);
  GPIO_PinModeSet(MX25_PORT_SCLK, MX25_PIN_SCLK, gpioModePushPull, 1);
  GPIO_PinModeSet(MX25_PORT_CS, MX25_PIN_CS, gpioModePushPull, 1);

  /* Configure USART */
  init.msbf         = true;
  init.baudrate     = 8000000;
  USART_InitSync(MX25_USART, &init);

  MX25_USART->ROUTELOC0 = (MX25_USART_LOC_MISO | MX25_USART_LOC_MOSI | MX25_USART_LOC_SCLK);
  MX25_USART->ROUTEPEN  = (USART_ROUTEPEN_RXPEN | USART_ROUTEPEN_TXPEN | USART_ROUTEPEN_CLKPEN);

  /* Wait for flash to power up */
  UTIL_delay(10);

  /* Set EM4 pin retention so chip select stays high if we enter EM4 */
  EMU->EM4CTRL |= EMU_EM4CTRL_EM4IORETMODE_EM4EXIT;

  return;
}

/***************************************************************************//**
 * @brief
 *    Common callback from the GPIOINT driver
 *
 * @return
 *    Returns none
 ******************************************************************************/
static void gpioInterruptHandler(uint8_t pin)
{
  BOARD_IrqCallback callback;

  callback = NULL;

  switch ( pin ) {
    case EXTI_PIC_INT:
      callback = picIRQCallback;
      break;

    case EXTI_BUTTON0:
      callback = pushButton0IRQCallback;
      break;

    case EXTI_BUTTON1:
      callback = pushButton1IRQCallback;
      break;

    default:
      callback = NULL;
  }

  if (callback != NULL) {
    callback();
  }

  return;
}

/** @endcond */

/** @} (end defgroup BOARD_4160) */
/** @} {end addtogroup TBSense_BSP} */
