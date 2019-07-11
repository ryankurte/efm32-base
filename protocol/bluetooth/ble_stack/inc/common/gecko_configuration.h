/***************************************************************************//**
 * @brief Definition of Bluetooth stack configuration
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

#ifndef GECKO_CONFIGURATION
#define GECKO_CONFIGURATION

#include "bg_gattdb_def.h"

#define DEFAULT_BLUETOOTH_HEAP(CONNECTIONS) (4824 +(CONNECTIONS)*(472))

#define SLEEP_FLAGS_DEEP_SLEEP_ENABLE  4
typedef struct {
  uint8_t     flags;
}gecko_sleep_config_t;


// Link layer config:
#ifndef GECKO_BLUETOOTH_LL_PRIORITIES_DEFINED
#define GECKO_BLUETOOTH_LL_PRIORITIES_DEFINED

typedef struct {
  uint8_t scan_min;
  uint8_t scan_max;
  uint8_t adv_min;
  uint8_t adv_max;
  uint8_t conn_min;
  uint8_t conn_max;
  uint8_t init_min;
  uint8_t init_max;
  uint8_t threshold_coex;
  uint8_t rail_mapping_offset;
  uint8_t rail_mapping_range;
  uint8_t threshold_coex_req;
  uint8_t coex_pwm_period;
  uint8_t coex_pwm_dutycycle;
  uint8_t afh_scan_interval;
  uint8_t adv_step;
  uint8_t scan_step;
}gecko_bluetooth_ll_priorities;

//Default priority configuration
#define GECKO_BLUETOOTH_PRIORITIES_DEFAULT { 191, 143, 175, 127, 135, 0, 55, 15, 175, 16, 16, 255, 0, 0, 0, 4, 4 }

#endif


typedef struct {
  uint8_t max_connections;  //!< Maximum number of connections to support, if 0 defaults to 4
  uint8_t max_advertisers;  //!< Maximum number of advertisers to support, if 0 defaults to 1
  uint8_t max_periodic_sync; //!< Maximum number of periodic synchronizations to support, if 0 defaults to 0
  uint8_t max_channels;  // Maximum number of L2cap Connection-oriented channels to support, if 0 defaults to 0
  //heap configuration, if NULL uses default
  void    *heap;
  uint16_t heap_size;
  uint16_t sleep_clock_accuracy;   // ppm, 0 = default (250 ppm)
  uint32_t linklayer_config;
  gecko_bluetooth_ll_priorities * linklayer_priorities; //Priority configuration, if NULL uses default values
}gecko_bluetooth_config_t;

/** PAVDD input straight from power supply */
#define GECKO_RADIO_PA_INPUT_VBAT   0

/** PAVDD input from DC/DC */
#define GECKO_RADIO_PA_INPUT_DCDC   1

typedef struct {
  uint8_t config_enable; // Non-zero value indicates this is a valid PA config

  uint8_t input;         // PAVDD input, either GECKO_RADIO_PA_INPUT_VBAT or GECKO_RADIO_PA_INPUT_DCDC
                         // default: GECKO_RADIO_PA_INPUT_DCDC

  uint8_t pa_mode;       // Selected PA power mode.
                         // See power mode definitions in RAIL rail_chip_specific.h. If the given mode is not
                         // supported by the device the stack will try with a lower mode. If power mode eventually
                         // failed to configure, an error will be returned from gecko_stack_init or gecko_init.
                         // Default value: RAIL_TX_POWER_MODE_2P4_HP
}gecko_radio_pa_config_t;

#define GECKO_OTA_FLAGS_RANDOM_ADDRESS      0x10000

typedef struct {
  uint32_t flags;
  uint8_t  device_name_len;
  char     *device_name_ptr;
}gecko_ota_config_t;

#define GECKO_MBEDTLS_FLAGS_NO_MBEDTLS_DEVICE_INIT      1
typedef struct {
  uint8_t flags;
  uint8_t dev_number;
}gecko_mbedtls_config_t;

#define GECKO_CONFIG_FLAG_RTOS                         256
#define GECKO_CONFIG_FLAG_NO_SLEEPDRV_INIT             512

typedef void (*gecko_priority_schedule_callback)(void);
typedef void (*gecko_stack_schedule_callback)(void);

#define GECKO_RF_CONFIG_ANTENNA                        (1 << 0)
typedef struct {
  int16_t tx_gain; // RF TX gain in unit of 0.1 dBm. For example, -20 means -2.0 dBm (signal loss).
  int16_t rx_gain; // RF RX gain in unit of 0.1 dBm.
  uint8_t flags; // bit 0 is enabling antenna config, other bits 1-7 are reserved.
  uint8_t antenna; // A configuration for antenna selection.
                   // Value should be come from RAIL_AntennaSel_t enum.
                   // See antenna path selection in RAIL rail_chip_specific.h.
} gecko_rf_config_t;

typedef struct {
  uint32_t config_flags;
  gecko_sleep_config_t  sleep;
  gecko_bluetooth_config_t bluetooth;
  //

  const struct bg_gattdb_def *gattdb;
  //Callback for priority scheduling, used for RTOS support. If NULL uses pendsv irq.
  //This is called from Interrupt context
  gecko_priority_schedule_callback scheduler_callback;

  //Callback for requesting Bluetooth stack scheduling, used for RTOS support
  //This is called from Interrupt context
  gecko_stack_schedule_callback stack_schedule_callback;

  gecko_radio_pa_config_t pa;

  gecko_ota_config_t ota;

  gecko_mbedtls_config_t mbedtls;
  uint8_t max_timers;  // Max number of soft timers, up to 16, the application will use through BGAPI. Default: 4
  gecko_rf_config_t rf;
  uint16_t btmesh_heap_size; // The amount of heap reserved for the BT Mesh stack
}gecko_configuration_t;

#endif