/***************************************************************************//**
 * @file
 * @brief EFP (Energy Friendly PMIC) driver.
 * @version 5.7.0
 *******************************************************************************
 * # License
 * <b>Copyright 2019 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc.  Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement.  This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/
#include "em_assert.h"
#include "em_common.h"
#include "em_core.h"
#include "em_gpio.h"
#include "sl_efpdrv.h"

#include <stdbool.h>
#include <math.h>

/**************************************************************************//**
* @addtogroup kitdrv
* @{
******************************************************************************/

/* *INDENT-OFF* */
/***************************************************************************//**
 * @addtogroup SL_EFPDRV
 * @brief EFP (Energy Friendly PMIC) driver.
 * @{

   @note This version of the sl_efpdrv is Alpha level only. Silicon Labs will
   NOT support any production deployments with this Alpha version.
   Production deployments will only be supported with the GA version of this
   feature. This version is intended for lab and evaluation purpose only.

   @details
   Documentation placeholder.
 ******************************************************************************/
/* *INDENT-ON* */

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

#define EFPDRV_EFP01_I2C_ADDR   0x60

enum {
  em_transition_mode_i2c = 0,
  em_transition_mode_gpio,
  em_transition_mode_emu
} em_transition_mode;

#if defined(SL_EFPDRV_EM_CTRL_GPIO_BITBANG) || defined(SL_EFPDRV_EM_CTRL_I2C)
static CORE_DECLARE_IRQ_STATE;
#endif

#if defined(_SILICON_LABS_32B_SERIES_2)
static unsigned int i2c_instance_index;
#endif

static bool in_direct_mode;
static sl_efpdrv_init_data init_data;

/***************************************************************************//**
 * @brief
 *   Configure I2C GPIO pins.
 *
 * @param[in] enable
 *   If true configure I2C pins for I2C use, if false pins are configured as
 *   GPIO pins to enable "direct mode" EM transfer mode.
 ******************************************************************************/
static void enable_i2c_pins(bool enable)
{
  if (enable) {
    in_direct_mode = false;
    GPIO_PinModeSet(init_data.i2c_sda_port, init_data.i2c_sda_pin,
                    gpioModeWiredAnd, 1);
    GPIO_PinModeSet(init_data.i2c_scl_port, init_data.i2c_scl_pin,
                    gpioModeWiredAnd, 1);

#if defined(_SILICON_LABS_32B_SERIES_0)
    init_data.i2c_instance->ROUTE |= I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN;
#elif defined(_SILICON_LABS_32B_SERIES_1)
    init_data.i2c_instance->ROUTEPEN |= I2C_ROUTEPEN_SDAPEN | I2C_ROUTEPEN_SCLPEN;
#elif defined(_SILICON_LABS_32B_SERIES_2)
    GPIO->I2CROUTE[i2c_instance_index].ROUTEEN |= GPIO_I2C_ROUTEEN_SDAPEN
                                                  | GPIO_I2C_ROUTEEN_SCLPEN;
#endif
  } else {
    in_direct_mode = true;
    GPIO_PinModeSet(init_data.i2c_sda_port, init_data.i2c_sda_pin,
                    gpioModePushPull, 1);
    GPIO_PinModeSet(init_data.i2c_scl_port, init_data.i2c_scl_pin,
                    gpioModePushPull, 1);

#if defined(_SILICON_LABS_32B_SERIES_0)
    init_data.i2c_instance->ROUTE &= ~(I2C_ROUTE_SDAPEN | I2C_ROUTE_SCLPEN);
#elif defined(_SILICON_LABS_32B_SERIES_1)
    init_data.i2c_instance->ROUTEPEN &= ~(I2C_ROUTEPEN_SDAPEN
                                          | I2C_ROUTEPEN_SCLPEN);
#elif defined(_SILICON_LABS_32B_SERIES_2)
    GPIO->I2CROUTE[i2c_instance_index].ROUTEEN &= ~(GPIO_I2C_ROUTEEN_SDAPEN
                                                    | GPIO_I2C_ROUTEEN_SCLPEN);
#endif
  }
}

/***************************************************************************//**
 * @brief
 *   Perform a VDD measurement.
 *
 * @param[in] msn_addr
 *   Register address of most significant byte of measurement.
 *
 * @param[in] lsby_addr
 *   Register address of least significant byte of measurement.
 *
 * @param[out] voltage_mv
 *   The result of the VDD measurement.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int get_vdd(uint8_t msn_addr, uint8_t lsby_addr, uint16_t *voltage_mv)
{
  int ret_val;
  uint16_t read_value;
  uint8_t adc_cc_ctrl, tmp, ss_mode;

  // Get ADC_INTERVAL value.
  ret_val = sl_efpdrv_read_register(EFP01_ADC_CC_CTRL, &adc_cc_ctrl);

  if (ret_val == SL_EFPDRV_OK) {
    if ((adc_cc_ctrl & _EFP01_ADC_CC_CTRL_ADC_INTERVAL_MASK) == 0) {
      // Issue an ADC start command if automated measurements are not enabled.
      ret_val = sl_efpdrv_write_register_field(EFP01_CMD,
                                               1,
                                               _EFP01_CMD_ADC_START_MASK,
                                               _EFP01_CMD_ADC_START_SHIFT);
    }
  }

  // Wait at least 18.5 usec for the result, read of SS mode provides the delay.
  if (ret_val == SL_EFPDRV_OK) {
    ret_val = sl_efpdrv_read_register(EFP01_CC_MODE, &ss_mode);
  }

  if (ret_val == SL_EFPDRV_OK) {                    // Read result MSB.
    ret_val = sl_efpdrv_read_register(msn_addr, &tmp);
  }

  if (ret_val == SL_EFPDRV_OK) {                    // Read result LSB.
    read_value = tmp << 8;
    ret_val = sl_efpdrv_read_register(lsby_addr, &tmp);
  }

  if (ret_val == SL_EFPDRV_OK) {                    // Scale result correctly.
    read_value |= tmp;
    if (ss_mode & _EFP01_CC_MODE_SC_MODE_MASK) {
      // Result is a VDDA measurement.
      *voltage_mv = (uint16_t)lroundf(read_value * 1.01f);
    } else {
      // Result is a VDDB measurement.
      *voltage_mv = (uint16_t)lroundf(read_value * 1.49f);
    }
  }

  return ret_val;
}

/// @endcond

#if defined(SL_EFPDRV_EM_CTRL_GPIO_BITBANG) || defined(SL_EFPDRV_EM_CTRL_I2C)
/***************************************************************************//**
 * @brief
 *   Reimplementation of EMU function with same name.
 *
 * @details
 *   This function will be called upon wakeup from the _WFI() instruction in
 *   EMU_EnterEM2/3() functions, and will set the EFP in EM0 mode.
 *   A critical section is used to get predictable EFP energy mode sequence
 *   timing.
 ******************************************************************************/
void EMU_EM23PostsleepHook(void)
{
  sl_efpdrv_enter_em0();
  CORE_EXIT_CRITICAL();
}
#endif

#if defined(SL_EFPDRV_EM_CTRL_GPIO_BITBANG) || defined(SL_EFPDRV_EM_CTRL_I2C)
/***************************************************************************//**
 * @brief
 *   Reimplementation of EMU function with same name.
 *
 * @details
 *   This function will be called prior to the _WFI() instruction in
 *   EMU_EnterEM2/3() functions, and will set the EFP in EM2 mode.
 *   A critical section is used to get predictable EFP energy mode sequence
 *   timing.
 ******************************************************************************/
void EMU_EM23PresleepHook(void)
{
  CORE_ENTER_CRITICAL();
  sl_efpdrv_enter_em2();
}
#endif

/***************************************************************************//**
 * @brief
 *   Configure I2C GPIO pins for "direct mode" EM transfer mode.
 *
 * @note
 *   This must be the last EFP driver function called prior to entering EM2/3
 *   via EMU_EnterEMx() calls. Any EFP API call that involves an I2C transfer
 *   will leave the I2C GPIO pins in I2C mode.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_enable_direct_mode(void)
{
  return sl_efpdrv_enter_em0();
}

/***************************************************************************//**
 * @brief
 *   Set EFP in EM0 energy mode.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_enter_em0(void)
{
  int ret_val = SL_EFPDRV_OK;

  if (em_transition_mode == em_transition_mode_gpio) {
    if (!in_direct_mode) {
      ret_val = sl_efpdrv_write_register_field(EFP01_EM_CRSREG_CTRL,
                                               1 << _EFP01_EM_CRSREG_CTRL_EM_DIRECTEN_SHIFT,
                                               _EFP01_EM_CRSREG_CTRL_EM_DIRECTEN_MASK,
                                               _EFP01_EM_CRSREG_CTRL_EM_DIRECTEN_SHIFT);
      enable_i2c_pins(false);
    }
    GPIO_PinOutSet(init_data.i2c_scl_port, init_data.i2c_scl_pin);
  } else if (em_transition_mode == em_transition_mode_i2c) {
    ret_val = sl_efpdrv_write_register_field(EFP01_EM_CRSREG_CTRL,
                                             _EFP01_EM_CRSREG_CTRL_EM_STSEL_EM0,
                                             _EFP01_EM_CRSREG_CTRL_EM_STSEL_MASK,
                                             _EFP01_EM_CRSREG_CTRL_EM_STSEL_SHIFT);
  }
  return ret_val;
}

/***************************************************************************//**
 * @brief
 *   Set EFP in EM2 energy mode.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_enter_em2(void)
{
  int ret_val = SL_EFPDRV_OK;

  if (em_transition_mode == em_transition_mode_gpio) {
    if (!in_direct_mode) {
      ret_val = sl_efpdrv_write_register_field(EFP01_EM_CRSREG_CTRL,
                                               1 << _EFP01_EM_CRSREG_CTRL_EM_DIRECTEN_SHIFT,
                                               _EFP01_EM_CRSREG_CTRL_EM_DIRECTEN_MASK,
                                               _EFP01_EM_CRSREG_CTRL_EM_DIRECTEN_SHIFT);
      enable_i2c_pins(false);
    }
    GPIO_PinOutClear(init_data.i2c_scl_port, init_data.i2c_scl_pin);
  } else if (em_transition_mode == em_transition_mode_i2c) {
    ret_val = sl_efpdrv_write_register_field(EFP01_EM_CRSREG_CTRL,
                                             _EFP01_EM_CRSREG_CTRL_EM_STSEL_EM2,
                                             _EFP01_EM_CRSREG_CTRL_EM_STSEL_MASK,
                                             _EFP01_EM_CRSREG_CTRL_EM_STSEL_SHIFT);
  }
  return ret_val;
}

/***************************************************************************//**
 * @brief
 *   Measure average VDD.
 *
 * @note
 *   This function will measure VDDA or VDDB depending of the voltage on the
 *   VDDA pin after power on. If VDDA is powered this function returns a VDDA
 *   reading, otherwise a VDDB reading is returned.
 *
 * @param[out] voltage_mv
 *   Average VDD voltage expressed in miilivolts.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_get_vdd_avg(uint16_t *voltage_mv)
{
  return get_vdd(EFP01_VDD_AVG_MSN, EFP01_VDD_AVG_LSBY, voltage_mv);
}

/***************************************************************************//**
 * @brief
 *   Measure maximum VDD.
 *
 * @note
 *   This function will measure VDDA or VDDB depending of the voltage on the
 *   VDDA pin after power on. If VDDA is powered this function returns a VDDA
 *   reading, otherwise a VDDB reading is returned.
 *
 * @param[out] voltage_mv
 *   Maximum VDD voltage expressed in miilivolts.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_get_vdd_max(uint16_t *voltage_mv)
{
  return get_vdd(EFP01_VDD_MAX_MSN, EFP01_VDD_MAX_LSBY, voltage_mv);
}

/***************************************************************************//**
 * @brief
 *   Measure minimum VDD.
 *
 * @note
 *   This function will measure VDDA or VDDB depending of the voltage on the
 *   VDDA pin after power on. If VDDA is powered this function returns a VDDA
 *   reading, otherwise a VDDB reading is returned.
 *
 * @param[out] voltage_mv
 *   Minimum VDD voltage expressed in miilivolts.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_get_vdd_min(uint16_t *voltage_mv)
{
  return get_vdd(EFP01_VDD_MIN_MSN, EFP01_VDD_MIN_LSBY, voltage_mv);
}

/***************************************************************************//**
 * @brief
 *   Initialize EFP driver and configure underlying EFP hardware.
 *
 * @param[in] init
 *   Initialization data structure.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_init(const sl_efpdrv_init_data *init)
{
  uint8_t *p;
  int ret_val, i;
  init_data = *init;

  // Assemble an I2C SPM initialization structure.
  I2CSPM_Init_TypeDef i2c_init = {
    .port            = init->i2c_instance,
    .sclPort         = init->i2c_scl_port,
    .sclPin          = init->i2c_scl_pin,
    .sdaPort         = init->i2c_sda_port,
    .sdaPin          = init->i2c_sda_pin,
#if defined(_SILICON_LABS_32B_SERIES_0)
    .portLocation;   = init->i2c_port_location,
#elif defined(_SILICON_LABS_32B_SERIES_1)
    .portLocationScl = init->i2c_scl_port_location,
    .portLocationSda = init->i2c_sda_port_location,
#endif
    .i2cRefFreq      = 0,
    .i2cMaxFreq      = I2C_FREQ_FASTPLUS_MAX,
    .i2cClhr         = i2cClockHLRStandard
  };

#if defined(_SILICON_LABS_32B_SERIES_2)
  if ( false ) {
#if defined(I2C0)
  } else if (init->i2c_instance == I2C0) {
    i2c_instance_index = 0;
#endif
#if defined(I2C1)
  } else if (init->i2c_instance == I2C1) {
    i2c_instance_index = 1;
#endif
#if defined(I2C2)
  } else if (init->i2c_instance == I2C2) {
    i2c_instance_index = 2;
#endif
  }
#endif

#if defined(SL_EFPDRV_EM_CTRL_GPIO_BITBANG)
  em_transition_mode = em_transition_mode_gpio;
#elif defined(SL_EFPDRV_EM_CTRL_I2C)
  em_transition_mode = em_transition_mode_i2c;
#elif defined(SL_EFPDRV_EM_CTRL_EMU)
  em_transition_mode = em_transition_mode_emu;
#else
  #error "Illegal EFPDRV configuration"
#endif

  // Initialize I2C interface.
  I2CSPM_Init(&i2c_init);
  in_direct_mode = false;
  // Set I2C slow mode and enable internal pullups.
  ret_val = sl_efpdrv_write_register(EFP01_I2C_CTRL,
                                     (1 << _EFP01_I2C_CTRL_I2C_PU_SHIFT)
                                     | (1 << _EFP01_I2C_CTRL_IO_SLOW_MODE_SHIFT));
  enable_i2c_pins(true);

  // Do configuration register writes if a configuration is defined.
  if (init->config_size > 0) {
    i = init->config_size;
    p = init->config_data;
    while (i && (ret_val == SL_EFPDRV_OK)) {
      ret_val = sl_efpdrv_write_register(*p, *(p + 1));
      i--;
      p += 2;
    }
  }

  // Configure GPIO pin as EFP IRQ input.
  if ((init->enable_irq_pin) && (ret_val == SL_EFPDRV_OK)) {
    uint8_t tmp;
    // Clear interrupt flags.
    ret_val  = sl_efpdrv_read_register(EFP01_STATUS_G, &tmp);
    ret_val |= sl_efpdrv_write_register(EFP01_STATUS_G, tmp);
    ret_val |= sl_efpdrv_read_register(EFP01_STATUS_V, &tmp);
    ret_val |= sl_efpdrv_write_register(EFP01_STATUS_V, tmp);
    if (ret_val == SL_EFPDRV_OK) {
      GPIO_PinModeSet(init->irq_port, init->irq_pin, gpioModeInputPull, 1);
    }
  }

  // Prepare for GPIO bitbanged "direct mode" EM transitions.
  if ((em_transition_mode == em_transition_mode_gpio)
      && (ret_val == SL_EFPDRV_OK)) {
    // Prepare GPIO for bitbang use.
    ret_val = sl_efpdrv_enable_direct_mode();
  }

  return ret_val;
}

/***************************************************************************//**
 * @brief
 *   Read EFP OTP register.
 *
 * @param[in] addr
 *   Register address.
 *
 * @param[out] data
 *   The value read from the OTP register.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_read_otp(uint8_t addr, uint8_t *data)
{
  int ret_val;
  uint8_t read_value;

  ret_val = sl_efpdrv_write_register(0x0F, 0x06);   // Enable OTP access.

  if (ret_val == SL_EFPDRV_OK) { // Need a few us delay, use reg. read as delay.
    ret_val = sl_efpdrv_read_register(EFP01_OTP_STATCTRL, &read_value);
  }

  if (ret_val == SL_EFPDRV_OK) {                    // Setup OTP address.
    ret_val = sl_efpdrv_write_register(EFP01_OTP_ADDR, addr & 0x7F);
  }

  if (ret_val == SL_EFPDRV_OK) {                    // Issue OTP read command.
    ret_val = sl_efpdrv_write_register_field(EFP01_OTP_STATCTRL,
                                             1,
                                             _EFP01_OTP_STATCTRL_CMD_READOTP_MASK,
                                             _EFP01_OTP_STATCTRL_CMD_READOTP_SHIFT);
  }

  if (ret_val == SL_EFPDRV_OK) { // Need a few us delay, use reg. read as delay.
    ret_val = sl_efpdrv_read_register(EFP01_OTP_STATCTRL, &read_value);
  }

  if (ret_val == SL_EFPDRV_OK) {                    // Readout OTP data.
    ret_val = sl_efpdrv_read_register(EFP01_OTP_RDATA, &read_value);
  }

  if (ret_val == SL_EFPDRV_OK) {                    // Disable OTP access.
    *data = read_value;                             // OTP value.
    ret_val = sl_efpdrv_write_register(0x0F, 0x00);
  }

  return ret_val;
}

/***************************************************************************//**
 * @brief
 *   Read EFP control register.
 *
 * @param[in] addr
 *   Register address.
 *
 * @param[out] data
 *   The value read from the register.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_read_register(uint8_t addr, uint8_t *data)
{
  uint8_t read_value;

  I2C_TransferSeq_TypeDef seq = {
    .addr   = EFPDRV_EFP01_I2C_ADDR,
    .flags  = I2C_FLAG_WRITE_READ,
    .buf[0] = { .data = &addr, .len = 1 },
    .buf[1] = { .data = &read_value, .len = 1 }
  };

  if (in_direct_mode) {
    enable_i2c_pins(true);
  }

  if (I2CSPM_Transfer(init_data.i2c_instance, &seq) != i2cTransferDone) {
    EFM_ASSERT(false);
    return SL_EFPDRV_I2C_ERROR;
  }
  *data = read_value;
  return SL_EFPDRV_OK;
}

/***************************************************************************//**
 * @brief
 *   Read EFP control register field.
 *
 * @param[in] addr
 *   Register address.
 *
 * @param[out] data
 *   The value read from the register field.
 *
 * @param[in] mask
 *   Bit mask corresponding to target bit field.
 *
 * @param[in] pos
 *   Bit number of least significant bit of target bit field.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_read_register_field(uint8_t addr,
                                  uint8_t *data,
                                  uint8_t mask,
                                  uint8_t pos)
{
  int ret_val;
  uint8_t tmp;

  ret_val = sl_efpdrv_read_register(addr, &tmp);
  if (ret_val == SL_EFPDRV_OK) {
    *data = (tmp & mask) >> pos;
  }
  return ret_val;
}

/***************************************************************************//**
 * @brief
 *   Set regulator VOA EM01 peak output current.
 *
 * @param[in] current_ma
 *   Target peak current expressed in milliamperes.
 *
 * @param[in] vddb_mv
 *   VDDB expressed in millivolt.
 *
 * @param[in] voa_mv
 *   VOA expressed in millivolt.
 *
 * @param[in] inductor_nh
 *   Inductor size expressed in nanohenry.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_voa_em01_peak_current(unsigned int current_ma,
                                        unsigned int vddb_mv,
                                        unsigned int voa_mv,
                                        unsigned int inductor_nh)
{
  long l;
  float f;

  f = (current_ma - 90.0f - (25.0f * (vddb_mv - voa_mv) / inductor_nh)) / 9.0f;
  l = lroundf(f);
  l = SL_MAX(l, 0);
  l = SL_MIN(l, _EFP01_BB_IPK_BB_IPK_MASK >> _EFP01_BB_IPK_BB_IPK_SHIFT);

  return sl_efpdrv_write_register_field(EFP01_BB_IPK, (uint8_t)l,
                                        _EFP01_BB_IPK_BB_IPK_MASK,
                                        _EFP01_BB_IPK_BB_IPK_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Set regulator VOA EM23 peak output current.
 *
 * @param[in] current_ma
 *   Target peak current expressed in milliamperes.
 *
 * @param[in] vddb_mv
 *   VDDB expressed in millivolt.
 *
 * @param[in] voa_mv
 *   VOA expressed in millivolt.
 *
 * @param[in] inductor_nh
 *   Inductor size expressed in nanohenry.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_voa_em23_peak_current(unsigned int current_ma,
                                        unsigned int vddb_mv,
                                        unsigned int voa_mv,
                                        unsigned int inductor_nh)
{
  long l;
  float f;

  f = (current_ma - 108.0f - (25.0f * (vddb_mv - voa_mv) / inductor_nh)) / 36.0f;
  l = lroundf(f);
  l = SL_MAX(l, 0);
  l = SL_MIN(l, _EFP01_BB_IPK_BB_IPK_EM2_MASK >> _EFP01_BB_IPK_BB_IPK_EM2_SHIFT);

  return sl_efpdrv_write_register_field(EFP01_BB_IPK, (uint8_t)l,
                                        _EFP01_BB_IPK_BB_IPK_EM2_MASK,
                                        _EFP01_BB_IPK_BB_IPK_EM2_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Set regulator VOA mode.
 *
 * @param[in] mode
 *   Regulator mode.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_voa_mode(sl_efpdrv_voa_mode_t mode)
{
  return sl_efpdrv_write_register_field(EFP01_BB_CTRL3,
                                        mode,
                                        _EFP01_BB_CTRL3_BB_MODE_MASK,
                                        _EFP01_BB_CTRL3_BB_MODE_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Set regulator VOA output voltage.
 *
 * @param[in] voltage_mv
 *   Voltage in millivolt.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_voa_voltage(unsigned int voltage_mv)
{
  uint8_t voa = (uint8_t)lroundf((voltage_mv - 1737.4f) / 30.6f)
                & _EFP01_VOA_V_VOA_V_MASK;
  return sl_efpdrv_write_register(EFP01_VOA_V, voa);
}

/***************************************************************************//**
 * @brief
 *   Set VOA SW (switched output) mode.
 *
 * @param[in] mode
 *   VOA SW mode.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_voa_sw_mode(sl_efpdrv_voa_sw_mode_t mode)
{
  return sl_efpdrv_write_register_field(EFP01_BIAS_SW,
                                        mode,
                                        _EFP01_BIAS_SW_VOA_SW_CON_MASK,
                                        _EFP01_BIAS_SW_VOA_SW_CON_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Set regulator VOB EM01 peak output current.
 *
 * @param[in] current_ma
 *   Target peak current expressed in milliamperes.
 *
 * @param[in] vddb_mv
 *   VDDB expressed in millivolt.
 *
 * @param[in] vob_mv
 *   VOB expressed in millivolt.
 *
 * @param[in] inductor_nh
 *   Inductor size expressed in nanohenry.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_vob_em01_peak_current(unsigned int current_ma,
                                        unsigned int vddb_mv,
                                        unsigned int vob_mv,
                                        unsigned int inductor_nh)
{
  long l;
  float f;

  f = (current_ma - 90.0f - (25.0f * (vddb_mv - vob_mv) / inductor_nh)) / 9.0f;
  l = lroundf(f);
  l = SL_MAX(l, 0);
  l = SL_MIN(l, _EFP01_BK_IPK_BK_IPK_MASK >> _EFP01_BK_IPK_BK_IPK_SHIFT);

  return sl_efpdrv_write_register_field(EFP01_BK_IPK, (uint8_t)l,
                                        _EFP01_BK_IPK_BK_IPK_MASK,
                                        _EFP01_BK_IPK_BK_IPK_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Set regulator VOB EM01 output voltage.
 *
 * @param[in] voltage_mv
 *   Voltage in millivolt.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_vob_em01_voltage(unsigned int voltage_mv)
{
  uint8_t vob = (uint8_t)lroundf((voltage_mv - 809.5f) / 22.3f);
  return sl_efpdrv_write_register_field(EFP01_VOB_EM0_V, vob,
                                        _EFP01_VOB_EM0_V_VOB_EM0_V_MASK,
                                        _EFP01_VOB_EM0_V_VOB_EM0_V_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Set regulator VOB EM23 peak output current.
 *
 * @param[in] current_ma
 *   Target peak current expressed in milliamperes.
 *
 * @param[in] vddb_mv
 *   VDDB expressed in millivolt.
 *
 * @param[in] vob_mv
 *   VOB expressed in millivolt.
 *
 * @param[in] inductor_nh
 *   Inductor size expressed in nanohenry.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_vob_em23_peak_current(unsigned int current_ma,
                                        unsigned int vddb_mv,
                                        unsigned int vob_mv,
                                        unsigned int inductor_nh)
{
  long l;
  float f;

  f = (current_ma - 108.0f - (25.0f * (vddb_mv - vob_mv) / inductor_nh)) / 36.0f;
  l = lroundf(f);
  l = SL_MAX(l, 0);
  l = SL_MIN(l, _EFP01_BK_IPK_BK_IPK_EM2_MASK >> _EFP01_BK_IPK_BK_IPK_EM2_SHIFT);

  return sl_efpdrv_write_register_field(EFP01_BK_IPK, (uint8_t)l,
                                        _EFP01_BK_IPK_BK_IPK_EM2_MASK,
                                        _EFP01_BK_IPK_BK_IPK_EM2_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Set regulator VOB EM23 output voltage.
 *
 * @param[in] voltage_mv
 *   Voltage in millivolt.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_vob_em23_voltage(unsigned int voltage_mv)
{
  uint8_t vob = (uint8_t)lroundf((voltage_mv - 809.5f) / 22.3f)
                & _EFP01_VOB_EM2_V_VOB_EM2_V_MASK;
  return sl_efpdrv_write_register(EFP01_VOB_EM2_V, vob);
}

/***************************************************************************//**
 * @brief
 *   Set regulator VOB mode.
 *
 * @param[in] mode
 *   Regulator mode.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_vob_mode(sl_efpdrv_vob_mode_t mode)
{
  return sl_efpdrv_write_register_field(EFP01_BK_CTRL1,
                                        mode,
                                        _EFP01_BK_CTRL1_BK_MODE_MASK,
                                        _EFP01_BK_CTRL1_BK_MODE_SHIFT);
}

/***************************************************************************//**
 * @brief
 *   Set regulator VOC output voltage.
 *
 * @param[in] voltage_mv
 *   Voltage in millivolt.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_set_voc_voltage(unsigned int voltage_mv)
{
  uint8_t voc = (uint8_t)lroundf((voltage_mv - 1717.8f) / 30.5f)
                & _EFP01_VOC_V_VOC_V_MASK;
  return sl_efpdrv_write_register(EFP01_VOC_V, voc);
}

/***************************************************************************//**
 * @brief
 *   Turn off the internal EMU LDO that supplies DECOUPLE pin.
 *
 * @note
 *   Make sure that an external source supply the DECOUPLE pin before calling
 *   this function.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_NOT_SUPPORTED.
 ******************************************************************************/
int sl_efpdrv_shutdown_emu_ldo(void)
{
#if defined(_SILICON_LABS_32B_SERIES_1)
  *(volatile uint32_t*)0x400E303C |= 0x00000040UL;
  return SL_EFPDRV_OK;

#else
#warning "Internal LDO disable not implemented for this device family."
  EFM_ASSERT(false);
  return SL_EFPDRV_NOT_SUPPORTED;
#endif
}

/***************************************************************************//**
 * @brief
 *   Write EFP control register.
 *
 * @param[in] addr
 *   Register address.
 *
 * @param[in] data
 *   The value to write to the register.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_write_register(uint8_t addr, uint8_t data)
{
  I2C_TransferSeq_TypeDef seq = {
    .addr   = EFPDRV_EFP01_I2C_ADDR,
    .flags  = I2C_FLAG_WRITE_WRITE,
    .buf[0] = { .data = &addr, .len = 1 },
    .buf[1] = { .data = &data, .len = 1 }
  };

  if (in_direct_mode) {
    enable_i2c_pins(true);
  }

  if (I2CSPM_Transfer(init_data.i2c_instance, &seq) != i2cTransferDone) {
    EFM_ASSERT(false);
    return SL_EFPDRV_I2C_ERROR;
  }
  return SL_EFPDRV_OK;
}

/***************************************************************************//**
 * @brief
 *   Write EFP control register field.
 *
 * @note
 *   This function performs a read-modify-write of a EFP register.
 *
 * @param[in] addr
 *   Register address.
 *
 * @param[in] data
 *   The value to write to the register field.
 *
 * @param[in] mask
 *   Bit mask corresponding to target bit field.
 *
 * @param[in] pos
 *   Bit number of least significant bit of target bit field.
 *
 * @return
 *   SL_EFPDRV_OK or SL_EFPDRV_I2C_ERROR on I2C transfer errors.
 ******************************************************************************/
int sl_efpdrv_write_register_field(uint8_t addr,
                                   uint8_t data,
                                   uint8_t mask,
                                   uint8_t pos)
{
  int ret_val;
  uint8_t tmp;

  ret_val = sl_efpdrv_read_register(addr, &tmp);
  if (ret_val == SL_EFPDRV_OK) {
    tmp = (tmp & ~mask) | ((data << pos) & mask);
    ret_val = sl_efpdrv_write_register(addr, tmp);
  }
  return ret_val;
}

/** @} (end addtogroup SL_EFPDRV) */
/** @} (end group kitdrv) */
