/*
 * dfu.c
 *
 *  Created on: 6.5.2014
 *      Author: pitkape
 */
#ifdef UART_DFU

#include <stdint.h>
#include <stddef.h>

#include "em_msc.h"
#include "main.h"
#include "api/gecko_bgapi.h"
#include "flash.h"
#include "uart.h"

unsigned char* upload_ptr = (unsigned char*)BOOTLOADER_SIZE;

static uint32_t temp_buf_len = 0;
static uint32_t flash_word_buf[FLASH_PAGE_SIZE * 2 / sizeof(uint32_t)];
static unsigned char *temp_buf = (unsigned char*)flash_word_buf;
static uint32_t max_temp_buf_len = sizeof(flash_word_buf);

// ********************************************************************************

#define EBLTAG_HEADER           0x0000
#define EBLTAG_METADATA         0xF608
#define EBLTAG_PROG             0xFE01
#define EBLTAG_MFGPROG          0x02FE
#define EBLTAG_ERASEPROG        0xFD03
#define EBLTAG_END              0xFC04
// Tags for encrypted EBL files
#define EBLTAG_ENC_HEADER       0xFB05
#define EBLTAG_ENC_INIT         0xFA06
#define EBLTAG_ENC_EBL_DATA     0xF907  // Encrypted EBL data
#define EBLTAG_ENC_MAC          0xF709
// Tags for signing
#define EBLTAG_ENC_SIGNATURE_ECDSA_P256    0xF70A  // ECDSA signature over the EBL stream
//
// ********************************************************************************
enum DFUState{
  DFU_EBL,
  DFU_WAIT_HEADER_HDR,
  DFU_WAIT_PROGRAM_HDR,
  DFU_WAIT_FLASH_DATA,
  DFU_CHECK_SIGNATURE,
  DFU_EBL_END,
  DFU_PASS
} state = DFU_EBL;
#define EBL_HEADER_HDR_SIZE (12)
#define EBL_PROGRAM_HDR_SIZE (4)
#define EBL_TAGLEN_SIZE (4)

uint8_t dfu_active = 0;
static uint32_t ebl_tag = 0;
static uint32_t ebl_size = 0;
static int32_t ebl_remaining = 0;

#define P(X, I) ((uint32_t)((X)[I]))

#define EBL_TYPE(X) ((P(X, 0) << 8) | (P(X, 1)))
#define EBL_LENGTH(X) ((P(X, 2) << 8) | (P(X, 3)))
#define EBL_PROGRAM_ADDRESS(X) ((P(X, 0) << 24) | (P(X, 1) << 16) | (P(X, 2) << 8) | (P(X, 3)))
//Drop data from head of temp buf
void drop_temp_buf(uint32_t d)
{
  int nlen = temp_buf_len - d;
  int i;
  for (i = 0; i < nlen; i++) {
    temp_buf[i] = temp_buf[i + d];
  }
  temp_buf_len = nlen;
}
void bglib_output(uint8 len1, uint8 *data1, uint16 len2, uint8*data2, uint8 type2)
{
  uart_output(len1, data1, len2, data2, type2);
}
uint32_t previous_flash_page = 0;
static void flash_row(uint32_t len)
{
  /*
     protect boot loader
   */
  if (((uint32_t)upload_ptr) >= BOOTLOADER_SIZE
      && ((uint32_t)upload_ptr) < FLASH_SIZE
      ) {
    uint32_t p = ((uint32_t)upload_ptr) & ~(FLASH_PAGE_SIZE - 1);

    if (p != previous_flash_page) {
      FLASH_eraseOneBlock((uint32_t)upload_ptr);
      previous_flash_page = p;
    }

    FLASH_writeBlock(upload_ptr, len, temp_buf);
  }
  upload_ptr += len;
  //copy leftovers

  drop_temp_buf(len);
  ebl_remaining -= len;
}

void handle_message(const struct gecko_cmd_packet *pck)
{
  switch (BGLIB_MSG_ID(*((uint32_t *)pck))) {
    case gecko_cmd_dfu_flash_set_address_id:
    {
      gecko_rsp_dfu_flash_set_address(bg_err_success);
      dfu_active = 1;
    }
    break;
    case gecko_cmd_dfu_flash_upload_id:
    {
      int len = pck->dfu_flash_upload_cmd.data.len;
      int i;
      if ((temp_buf_len + len) > max_temp_buf_len) {
        gecko_rsp_dfu_flash_upload_finish(bg_err_buffers_full);
      }
      for (i = 0; i < len; i++) {
        temp_buf[temp_buf_len++] = pck->dfu_flash_upload_cmd.data.data[i];
      }
      while (temp_buf_len > 0) {
        if (ebl_remaining <= 0) {//read next tag
          if (temp_buf_len < 4) {     //wait for more data
            goto pass;
          }
          ebl_tag = EBL_TYPE(temp_buf);
          ebl_size = EBL_LENGTH(temp_buf);        /*EBL length does not contain type&length fields*/
          ebl_remaining = ebl_size;
          drop_temp_buf(EBL_TAGLEN_SIZE);
          switch (ebl_tag) {
            case EBLTAG_HEADER: state = DFU_WAIT_HEADER_HDR; break;
            case EBLTAG_ERASEPROG: state = DFU_WAIT_PROGRAM_HDR; break;
            default: state = DFU_PASS;
          }
        }
        switch (state) {
          case DFU_WAIT_HEADER_HDR:
            if (temp_buf_len < EBL_HEADER_HDR_SIZE) {
              goto pass;
            }
            upload_ptr = (uint8_t*)EBL_PROGRAM_ADDRESS(temp_buf + 4);
            drop_temp_buf(EBL_HEADER_HDR_SIZE);
            ebl_remaining -= EBL_HEADER_HDR_SIZE;
            state = DFU_WAIT_FLASH_DATA;
            break;
          case DFU_WAIT_PROGRAM_HDR:
            if (temp_buf_len < EBL_PROGRAM_HDR_SIZE) {
              goto pass;
            }
            upload_ptr = (uint8_t*)EBL_PROGRAM_ADDRESS(temp_buf);
            drop_temp_buf(EBL_PROGRAM_HDR_SIZE);
            ebl_remaining -= EBL_PROGRAM_HDR_SIZE;
            state = DFU_WAIT_FLASH_DATA;
            break;
          case DFU_WAIT_FLASH_DATA:
            //flash
          {
            uint32_t l = temp_buf_len;
            if (l > ebl_remaining) {
              l = ebl_remaining;
            }
            //write multiple of 4 bytes
            if ((l % 4) && (ebl_remaining != l)) {
              l = l & ~(0x3);
            }
            flash_row(l);
          }

            goto pass;
            //
            break;
          case DFU_PASS:
          default:          //drop everything
            if (temp_buf_len >= ebl_remaining) {
              drop_temp_buf(ebl_remaining);
              ebl_remaining = 0;
            } else {
              ebl_remaining -= temp_buf_len;
              temp_buf_len = 0;
            }
            break;
        }
      }

      pass:
      gecko_rsp_dfu_flash_upload(bg_err_success);
    }
    break;
    case gecko_cmd_dfu_flash_upload_finish_id:
    {
      if (check_image_crc((uint8_t *)BOOTLOADER_SIZE)) {
        gecko_rsp_dfu_flash_upload_finish(bg_err_success);
      } else {
        gecko_rsp_dfu_flash_upload_finish(bg_err_security_image_checksum_error);
      }
    }
    break;
    case gecko_cmd_dfu_reset_id:
      if (pck->dfu_reset_cmd.dfu) {
        gecko_evt_dfu_boot(BOOTLOADER_VERSION);
      } else {
        NVIC_SystemReset();
      }
      break;
  }
}
#endif /* UART_DFU */
