#ifndef __TOKEN_MACROS_H_
#define __TOKEN_MACROS_H_

#include <stdint.h>

#include "token_config.h"

#define TOKEN_ADDR_(x, y) x##y
#define TOKEN_ADDR(name) TOKEN_ADDR_(name, _ADDR)

#define TOKEN_getU8(name) (*(uint8_t *)TOKEN_ADDR(name))
#define TOKEN_getU16(name) (*(uint16_t *)TOKEN_ADDR(name))
#define TOKEN_getU32(name) (*(uint32_t *)TOKEN_ADDR(name))

void     TOKEN_init     (void);
uint16_t TOKEN_getCount (void);

#endif
