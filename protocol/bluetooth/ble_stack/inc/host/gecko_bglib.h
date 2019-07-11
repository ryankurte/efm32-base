/***************************************************************************//**
 * @brief Adaptation layer between host application and BGAPI protocol
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

#ifndef GECKO_BGLIB_H
#define GECKO_BGLIB_H

/*****************************************************************************
 *
 *  This is an adaptation layer between host application and BGAPI protocol.
 *  It provides synchronization mechanism for BGAPI-protocol that allows
 *  using same application architecture between application in mcu and external
 *  host.
 *
 *  Synchronization is done by waiting for response after each command. If
 *  any events are received during response waiting, they are queued and
 *  delivered next time gecko_wait_event is called.
 *
 *  Queue length is controlled by defining macro "BGLIB_QUEUE_LEN", default is 30.
 *  Queue length depends on use cases and allowed host memory usage.
 *
 *  BGLIB usage:
 *      Define library, it must be defined globally:
 *          BGLIB_DEFINE();
 *
 *      Declare and define output function, prototype is:
 *          void my_output(uint16 len,uint8* data);
 *          Function sends "len" amount of data from pointer "data" to device.
 *
 *      Declare and define input function, prototype is:
 *          void my_input(uint16 len,uint8* data);
 *          Function reads "len" amount of data to pointer "data" from device.
 *          Function return nonzero if it failed.
 *
 *      Initialize library,and provide output and input function:
 *          BGLIB_INITIALIZE(my_output,my_input);
 *
 *
 *  Receiving event:
 *   Events are received by gecko_wait_event-function.
 *
 *   prototype:
 *       struct gecko_cmd_packet* gecko_wait_event(void)
 *
 *   Event ID can be read from header of event by BGLIB_MSG_ID-macro.
 *
 *   Event data can be accessed thru returned pointer.
 *
 *   Example:
 *       struct gecko_cmd_packet *p;
 *
 *       p=gecko_wait_event()
 *
 *       if(BGLIB_MSG_ID(p->header)==gecko_evt_gatt_server_characteristic_status_id)
 *       {
 *           c=evt->evt_gatt_server_characteristic_status.connection;//accesses connection field of event data
 *       }
 *
 *  Sending commands:
 *   BGAPI-commands can be sent by calling helper functions, that then build bgapi-message and call output-function.
 *   Function then waits for response and return pointer to that message.
 *
 *   Example:
 *       //Configure GPIO F5 to output, with value 1
 *       if(gecko_cmd_hardware_configure_gpio(5, 5, hardware_gpio_mode_push_pull, 1)->result==bg_err_success)
 *       {//function call was succesful
 *
 *       }
 *
 *
 ****************************************************************************/

#if _MSC_VER  //msvc
#define inline __inline
#endif

#include "host_gecko.h"

#ifndef BGLIB_QUEUE_LEN
#define BGLIB_QUEUE_LEN 30
#endif

#define BGLIB_DEFINE()                                      \
  struct gecko_cmd_packet _gecko_cmd_msg;                   \
  struct gecko_cmd_packet _gecko_rsp_msg;                   \
  struct gecko_cmd_packet *gecko_cmd_msg = &_gecko_cmd_msg; \
  struct gecko_cmd_packet *gecko_rsp_msg = &_gecko_rsp_msg; \
  void (*bglib_output)(uint32_t len1, uint8_t* data1);      \
  int32_t (*bglib_input)(uint32_t len1, uint8_t* data1);    \
  int32_t (*bglib_peek)(void);                              \
  struct gecko_cmd_packet gecko_queue[BGLIB_QUEUE_LEN];     \
  int    gecko_queue_w = 0;                                 \
  int    gecko_queue_r = 0;

extern struct gecko_cmd_packet gecko_queue[BGLIB_QUEUE_LEN];
extern int    gecko_queue_w;
extern int    gecko_queue_r;

/**
 * Initialize BGLIB
 * @param OFUNC
 * @param IFUNC
 */
#define BGLIB_INITIALIZE(OFUNC, IFUNC) bglib_output = OFUNC; bglib_input = IFUNC; bglib_peek = NULL;

/**
 * Initialize BGLIB to support nonblocking mode
 * @param OFUNC
 * @param IFUNC
 * @param PFUNC peek function to check if there is data to be read from UART
 */
#define BGLIB_INITIALIZE_NONBLOCK(OFUNC, IFUNC, PFUNC) bglib_output = OFUNC; bglib_input = IFUNC; bglib_peek = PFUNC;

extern void(*bglib_output)(uint32_t len1, uint8_t* data1);
extern int32_t (*bglib_input)(uint32_t len1, uint8_t* data1);
extern int32_t(*bglib_peek)(void);

#endif
