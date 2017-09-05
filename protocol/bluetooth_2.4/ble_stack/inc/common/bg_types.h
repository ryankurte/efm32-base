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
