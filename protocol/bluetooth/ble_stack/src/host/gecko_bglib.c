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

#include "gecko_bglib.h"

struct gecko_cmd_packet* gecko_wait_message(void)//wait for event from system
{
  uint32_t msg_length;
  uint32_t header;
  uint8_t  *payload;
  struct gecko_cmd_packet *pck, *retVal = NULL;
  int      ret;
  //sync to header byte
  ret = bglib_input(1, (uint8_t*)&header);
  if (ret < 0 || (header & 0x78) != gecko_dev_type_gecko) {
    return 0;
  }
  ret = bglib_input(BGLIB_MSG_HEADER_LEN - 1, &((uint8_t*)&header)[1]);
  if (ret < 0) {
    return 0;
  }

  msg_length = BGLIB_MSG_LEN(header);

  if (msg_length > BGLIB_MSG_MAX_PAYLOAD) {
    return 0;
  }

  if ((header & 0xf8) == (gecko_dev_type_gecko | gecko_msg_type_evt)) {
    //received event
    if ((gecko_queue_w + 1) % BGLIB_QUEUE_LEN == gecko_queue_r) {
      //drop packet
      if (msg_length) {
        uint8 tmp_payload[BGLIB_MSG_MAX_PAYLOAD];
        bglib_input(msg_length, tmp_payload);
      }
      return 0;      //NO ROOM IN QUEUE
    }
    pck = &gecko_queue[gecko_queue_w];
    gecko_queue_w = (gecko_queue_w + 1) % BGLIB_QUEUE_LEN;
  } else if ((header & 0xf8) == gecko_dev_type_gecko) {//response
    retVal = pck = gecko_rsp_msg;
  } else {
    //fail
    return 0;
  }
  pck->header = header;
  payload = (uint8_t*)&pck->data.payload;
  /**
   * Read the payload data if required and store it after the header.
   */
  if (msg_length) {
    ret = bglib_input(msg_length, payload);
    if (ret < 0) {
      return 0;
    }
  }

  // Using retVal avoid double handling of event msg types in outer function
  return retVal;
}

int gecko_event_pending(void)
{
  if (gecko_queue_w != gecko_queue_r) {//event is waiting in queue
    return 1;
  }

  //something in uart waiting to be read
  if (bglib_peek && bglib_peek()) {
    return 1;
  }

  return 0;
}

struct gecko_cmd_packet* gecko_get_event(int block)
{
  struct gecko_cmd_packet* p;

  while (1) {
    if (gecko_queue_w != gecko_queue_r) {
      p = &gecko_queue[gecko_queue_r];
      gecko_queue_r = (gecko_queue_r + 1) % BGLIB_QUEUE_LEN;
      return p;
    }
    //if not blocking and nothing in uart -> out
    if (!block && bglib_peek && bglib_peek() == 0) {
      return NULL;
    }

    //read more messages from device
    if ( (p = gecko_wait_message()) ) {
      return p;
    }
  }
}

struct gecko_cmd_packet* gecko_wait_event(void)
{
  return gecko_get_event(1);
}

struct gecko_cmd_packet* gecko_peek_event(void)
{
  return gecko_get_event(0);
}

struct gecko_cmd_packet* gecko_wait_response(void)
{
  struct gecko_cmd_packet* p;
  while (1) {
    p = gecko_wait_message();
    if (p && !(p->header & gecko_msg_type_evt)) {
      return p;
    }
  }
}

void gecko_handle_command(uint32_t hdr, void* data)
{
  //packet in gecko_cmd_msg is waiting for output
  bglib_output(BGLIB_MSG_HEADER_LEN + BGLIB_MSG_LEN(gecko_cmd_msg->header), (uint8_t*)gecko_cmd_msg);
  gecko_wait_response();
}

void gecko_handle_command_noresponse(uint32_t hdr, void* data)
{
  //packet in gecko_cmd_msg is waiting for output
  bglib_output(BGLIB_MSG_HEADER_LEN + BGLIB_MSG_LEN(gecko_cmd_msg->header), (uint8_t*)gecko_cmd_msg);
}
