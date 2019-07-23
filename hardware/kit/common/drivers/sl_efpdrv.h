/***************************************************************************//**
 * @file
 * @brief EFP (Energy Friendly PMIC) driver API definitions.
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
#ifndef SL_EFPDRV_H
#define SL_EFPDRV_H

#include "i2cspm.h"
#include "sl_efpdrv_config.h" // Application specific settings and configuration.
#include "sl_efp01.h"
#include "em_gpio.h"

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**************************************************************************//**
* @addtogroup kitdrv
* @{
******************************************************************************/

/***************************************************************************//**
 * @addtogroup SL_EFPDRV
 * @{
 ******************************************************************************/

#define SL_EFPDRV_OK              0   ///< Success.
#define SL_EFPDRV_I2C_ERROR       1   ///< I2C transfer error.
#define SL_EFPDRV_NOT_SUPPORTED   2   ///< Operation not supported.

/// VOA regulator modes.
typedef enum {
  efp_voa_mode_disabled                = _EFP01_BB_CTRL3_BB_MODE_DISABLED,      ///< VOA disabled.
  efp_voa_mode_wired_buck              = _EFP01_BB_CTRL3_BB_MODE_WIREDBUCK,     ///< VOA wired buck.
  efp_voa_mode_wired_boost             = _EFP01_BB_CTRL3_BB_MODE_WIREDBOOST,    ///< VOA wired boost.
  efp_voa_mode_wired_buck_ldo          = _EFP01_BB_CTRL3_BB_MODE_WIREDBUCKLDO,  ///< VOA wired buck and LDO.
  efp_voa_mode_buck_boost_autonomous   = _EFP01_BB_CTRL3_BB_MODE_AUTONOMOUS,    ///< VOA buck boost autonomous.
  efp_voa_mode_buck_boost_forced_buck  = _EFP01_BB_CTRL3_BB_MODE_FORCEBUCK,     ///< VOA buck boost forced buck.
  efp_voa_mode_buck_boost_forced_ntm   = _EFP01_BB_CTRL3_BB_MODE_FORCENTM,      ///< VOA buck boost forced NTM.
  efp_voa_mode_buck_boost_forced_boost = _EFP01_BB_CTRL3_BB_MODE_FORCEBOOST     ///< VOA buck boost forced boost.
} sl_efpdrv_voa_mode_t;

/// VOA SW (switched output) modes.
typedef enum {
  efp_voa_sw_mode_off                  = _EFP01_BIAS_SW_VOA_SW_CON_OFF,     ///< VOA SW off.
  efp_voa_sw_mode_on                   = _EFP01_BIAS_SW_VOA_SW_CON_ON,      ///< VOA SW on in EM0, EM2 and EM4.
  efp_voa_sw_mode_on_in_em0            = _EFP01_BIAS_SW_VOA_SW_CON_ONINEM0, ///< VOA SW on in EM0 only.
  efp_voa_sw_mode_on_in_em02           = _EFP01_BIAS_SW_VOA_SW_CON_ONINEM02 ///< VOA SW on in EM0 and EM2.
} sl_efpdrv_voa_sw_mode_t;

/// VOB regulator modes.
typedef enum {
  efp_vob_mode_disabled = _EFP01_BK_CTRL1_BK_MODE_DISABLED,   ///< VOB disabled.
  efp_vob_mode_buck     = _EFP01_BK_CTRL1_BK_MODE_BUCKONLY,   ///< VOB buck only.
  efp_vob_mode_ldo      = _EFP01_BK_CTRL1_BK_MODE_LDOONLY,    ///< VOB LDO only.
  efp_vob_mode_buck_ldo = _EFP01_BK_CTRL1_BK_MODE_BUCKLDO     ///< VOB buck and LDO.
} sl_efpdrv_vob_mode_t;

/// EFP driver initialization structure.
typedef struct {
  unsigned int        config_size;            ///< Number of register writes inside the configuration data. Set to 0 on preprogrammed parts.
  uint8_t             *config_data;           ///< Configuration data, pairs of (addr,data),(addr,data),... Set to NULL on preprogrammed parts.
  bool                enable_irq_pin;         ///< Initialize a GPIO pin as EFP IRQ input.
  GPIO_Port_TypeDef   irq_port;               ///< GPIO port to use for EFP IRQ GPIO pin.
  unsigned int        irq_pin;                ///< GPIO pin number to use for EFP IRQ GPIO pin.
  I2C_TypeDef         *i2c_instance;          ///< I2C peripheral instance pointer.
  GPIO_Port_TypeDef   i2c_scl_port;           ///< GPIO port to use for I2C SCL signal.
  unsigned int        i2c_scl_pin;            ///< GPIO pin number to use for I2C SCL signal.
  GPIO_Port_TypeDef   i2c_sda_port;           ///< GPIO port to use for I2C SDA signal.
  unsigned int        i2c_sda_pin;            ///< GPIO pin number to use for I2C SDA signal.
#if defined(_SILICON_LABS_32B_SERIES_0)
  unsigned int        i2c_port_location;      ///< I2C location number to use for I2C signals.
#elif defined(_SILICON_LABS_32B_SERIES_1)
  unsigned int        i2c_scl_port_location;  ///< I2C location number to use for I2C SCL signal.
  unsigned int        i2c_sda_port_location;  ///< I2C location number to use for I2C SDA signal.
#endif
} sl_efpdrv_init_data;

// EFP driver API function protoypes.
int sl_efpdrv_enable_direct_mode(void);
int sl_efpdrv_enter_em0(void);
int sl_efpdrv_enter_em2(void);
int sl_efpdrv_get_vdd_avg(uint16_t *voltage_mv);
int sl_efpdrv_get_vdd_max(uint16_t *voltage_mv);
int sl_efpdrv_get_vdd_min(uint16_t *voltage_mv);
int sl_efpdrv_init(const sl_efpdrv_init_data *init);
int sl_efpdrv_read_otp(uint8_t addr, uint8_t *data);
int sl_efpdrv_read_register(uint8_t addr, uint8_t *data);
int sl_efpdrv_read_register_field(uint8_t addr, uint8_t *data,
                                  uint8_t mask, uint8_t pos);
int sl_efpdrv_set_voa_em01_peak_current(unsigned int current_ma,
                                        unsigned int vddb_mv,
                                        unsigned int voa_mv,
                                        unsigned int inductor_nh);
int sl_efpdrv_set_voa_em23_peak_current(unsigned int current_ma,
                                        unsigned int vddb_mv,
                                        unsigned int voa_mv,
                                        unsigned int inductor_nh);
int sl_efpdrv_set_voa_mode(sl_efpdrv_voa_mode_t mode);
int sl_efpdrv_set_voa_sw_mode(sl_efpdrv_voa_sw_mode_t mode);
int sl_efpdrv_set_voa_voltage(unsigned int voltage_mv);
int sl_efpdrv_set_vob_em01_peak_current(unsigned int current_ma,
                                        unsigned int vddb_mv,
                                        unsigned int vob_mv,
                                        unsigned int inductor_nh);
int sl_efpdrv_set_vob_em01_voltage(unsigned int voltage_mv);
int sl_efpdrv_set_vob_em23_peak_current(unsigned int current_ma,
                                        unsigned int vddb_mv,
                                        unsigned int vob_mv,
                                        unsigned int inductor_nh);
int sl_efpdrv_set_vob_em23_voltage(unsigned int voltage_mv);
int sl_efpdrv_set_vob_mode(sl_efpdrv_vob_mode_t mode);
int sl_efpdrv_set_voc_voltage(unsigned int voltage_mv);
int sl_efpdrv_shutdown_emu_ldo(void);
int sl_efpdrv_write_register(uint8_t addr, uint8_t data);
int sl_efpdrv_write_register_field(uint8_t addr, uint8_t data,
                                   uint8_t mask, uint8_t pos);

/** @} (end addtogroup SL_EFPDRV) */
/** @} (end group kitdrv) */

#ifdef __cplusplus
}
#endif

#endif // #define SL_EFPDRV_H
