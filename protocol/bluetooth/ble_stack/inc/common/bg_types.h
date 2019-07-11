/***************************************************************************//**
 * @brief Basic types used in Bluetooth stack
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

#ifndef BG_TYPES_H
#define BG_TYPES_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t   uint8;
typedef uint16_t  uint16;
typedef uint32_t  uint32;
typedef int8_t    int8;
typedef int16_t   int16;
typedef int32_t   int32;

typedef struct {
  uint8 len;
  uint8  data[];
}uint8array;

typedef struct {
  uint16 len;
  uint8  data[];
}uint16array;

typedef struct {
  uint8 len;
  int8  data[];
}string;

typedef struct {
  uint8 addr[6];
}bd_addr;

typedef struct {
  uint8 name[16];
}ser_name;

typedef struct {
  uint8 data[16];
} uuid_128;

typedef struct {
  uint8 data[16];
} aes_key_128;

#define MAX_UUID_LEN 16

typedef struct {
  uint8_t uuid[MAX_UUID_LEN];
  uint32_t len;
} sl_bt_uuid_t;

#if !defined _MSC_VER && !defined __APPLE__
/* Don't define under MSVC and OSX */

#ifndef memcpy
void * memcpy(void *OUT, const void *IN, size_t N);
#endif
#ifndef memcmp
int  memcmp(const void *S1, const void *S2, size_t N);
#endif
#ifndef memset
void * memset(void *DST, int c, size_t LENGTH);
#endif
#ifndef memmove
void * memmove(void *DST, const void *src, size_t LENGTH);
#endif
#endif

#endif
