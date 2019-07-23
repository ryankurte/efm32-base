/***************************************************************************//**
 * @brief Adaptation for running Bluetooth in RTOS
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

#ifndef RTOS_BLUETOOTH_H
#define RTOS_BLUETOOTH_H
#include "rtos_gecko.h"
#include <kernel/include/os.h>

//Bluetooth event flag group
extern OS_FLAG_GRP bluetooth_event_flags;
//Bluetooth event flag definitions
#define BLUETOOTH_EVENT_FLAG_STACK       ((OS_FLAGS)1)    //Bluetooth task needs an update
#define BLUETOOTH_EVENT_FLAG_LL          ((OS_FLAGS)2)    //Linklayer task needs an update
#define BLUETOOTH_EVENT_FLAG_CMD_WAITING ((OS_FLAGS)4)    //BGAPI command is waiting to be processed
#define BLUETOOTH_EVENT_FLAG_RSP_WAITING ((OS_FLAGS)8)    //BGAPI response is waiting to be processed
#define BLUETOOTH_EVENT_FLAG_EVT_WAITING ((OS_FLAGS)16)   //BGAPI event is waiting to be processed
#define BLUETOOTH_EVENT_FLAG_EVT_HANDLED ((OS_FLAGS)32)   //BGAPI event is handled

//Bluetooth event data pointer
extern volatile struct gecko_cmd_packet*  bluetooth_evt;

// Function prototype for initializing Bluetooth stack.
typedef errorcode_t(*bluetooth_stack_init_func)();

/**
 * Start Bluetooth tasks. The given Bluetooth stack initialization function
 * will be called at a proper time. Application should not initialize
 * Bluetooth stack anywhere else.
 *
 * @param ll_priority link layer task priority
 * @param stack_priority Bluetooth stack task priority
 * @param initialize_bluetooth_stack The function for initializing Bluetooth stack
 */
void bluetooth_start(OS_PRIO ll_priority,
                     OS_PRIO stack_priority,
                     bluetooth_stack_init_func initialize_bluetooth_stack);

// Set the callback for wakeup, Bluetooth task will call this when it has a new event
// It must only used to wake up application task, for example by posting task semaphore
typedef void (*wakeupCallback)();
void BluetoothSetWakeupCallback(wakeupCallback cb);
//Bluetooth stack needs an update
void BluetoothUpdate();
//Linklayer is updated
void BluetoothLLCallback();

//Mutex functions for using Bluetooth from multiple tasks
void BluetoothPend(RTOS_ERR *err);
void BluetoothPost(RTOS_ERR *err);

#endif //RTOS_BLUETOOTH_H
