/**
 * @file token-stack.h
 * @brief Definitions for stack tokens.
 * See @ref token_stack for documentation.
 *
 * The file token-stack.h should not be included directly.
 * It is accessed by the other token files.
 *
 * <!-- Maurizio Nanni -->
 * <!-- Copyright 2013 by Silicon Labs. All rights reserved.             *80*-->
 */

/**
 * @addtogroup token_stack
 *
 * The tokens listed here are divided into three sections (the three main
 * types of tokens mentioned in token.h):
 * - manufacturing
 * - stack
 * - application
 *
 * For a full explanation of the tokens, see hal/micro/token.h.
 * See token-stack.h for source code.
 *
 * There is a set of tokens predefined in the APPLICATION DATA section at the
 * end of token-stack.h because these tokens are required by the stack,
 * but they are classified as application tokens since they are sized by the
 * application via its CONFIGURATION_HEADER.
 *
 * The user application can include its own tokens in a header file similar
 * to this one. The macro ::APPLICATION_TOKEN_HEADER should be defined to equal
 * the name of the header file in which application tokens are defined.
 * See the APPLICATION DATA section at the end of token-stack.h
 * for examples of token definitions.
 *
 * Since token-stack.h contains both the typedefs and the token defs, there are
 * two \#defines used to select which one is needed when this file is included.
 * \#define DEFINETYPES is used to select the type definitions and
 * \#define DEFINETOKENS is used to select the token definitions.
 * Refer to token.h and token.c to see how these are used.
 *
 * @{
 */

// The basic TOKEN_DEF macro should not be used directly since the simplified
//  definitions are safer to use.  For completeness of information, the basic
//  macro has the following format:
//
//  TOKEN_DEF(name,creator,iscnt,isidx,type,arraysize,...)
//  name - The root name used for the token
//  creator - a "creator code" used to uniquely identify the token
//  iscnt - a boolean flag that is set to identify a counter token
//  isidx - a boolean flag that is set to identify an indexed token
//  type - the basic type or typdef of the token
//  arraysize - the number of elements making up an indexed token
//  ... - initializers used when reseting the tokens to default values
//
//
// The following convenience macros are used to simplify the definition
//  process for commonly specified parameters to the basic TOKEN_DEF macro
//  DEFINE_BASIC_TOKEN(name, type, ...)
//  DEFINE_INDEXED_TOKEN(name, type, arraysize, ...)
//  DEFINE_COUNTER_TOKEN(name, type, ...)
//  DEFINE_FIXED_BASIC_TOKEN(name, type, address, ...)
//  DEFINE_FIXED_INDEXED_TOKEN(name, type, arraysize, address, ...)
//  DEFINE_FIXED_COUNTER_TOKEN(name, type, address, ...)
//  DEFINE_MFG_TOKEN(name, type, address, ...)
//

/**
 * @name Convenience Macros
 * @brief The following convenience macros are used to simplify the definition
 * process for commonly specified parameters to the basic TOKEN_DEF macro.
 * Please see hal/micro/token.h for a more complete explanation.
 *@{
 */
#define DEFINE_BASIC_TOKEN(name, type, ...) \
  TOKEN_DEF(name, CREATOR_##name, 0, 0, type, 1, __VA_ARGS__)

#define DEFINE_COUNTER_TOKEN(name, type, ...) \
  TOKEN_DEF(name, CREATOR_##name, 1, 0, type, 1, __VA_ARGS__)

#define DEFINE_INDEXED_TOKEN(name, type, arraysize, ...) \
  TOKEN_DEF(name, CREATOR_##name, 0, 1, type, (arraysize), __VA_ARGS__)

#define DEFINE_FIXED_BASIC_TOKEN(name, type, address, ...) \
  TOKEN_NEXT_ADDRESS(name, (address))                      \
  TOKEN_DEF(name, CREATOR_##name, 0, 0, type, 1, __VA_ARGS__)

#define DEFINE_FIXED_COUNTER_TOKEN(name, type, address, ...) \
  TOKEN_NEXT_ADDRESS(name, (address))                        \
  TOKEN_DEF(name, CREATOR_##name, 1, 0, type, 1, __VA_ARGS__)

#define DEFINE_FIXED_INDEXED_TOKEN(name, type, arraysize, address, ...) \
  TOKEN_NEXT_ADDRESS(name, (address))                                   \
  TOKEN_DEF(name, CREATOR_##name, 0, 1, type, (arraysize), __VA_ARGS__)
/** @} END Convenience Macros */

// The Simulated EEPROM unit tests define all of their own tokens.
#ifndef SIM_EEPROM_TEST

// The creator codes are here in one list instead of next to their token
// definitions so comparision of the codes is easier.  The only requirement
// on these creator definitions is that they all must be unique.  A favorite
// method for picking creator codes is to use two ASCII characters inorder
// to make the codes more memorable.

/**
 * @name Creator Codes
 * @brief The CREATOR is used as a distinct identifier tag for the
 * token.
 *
 * The CREATOR is necessary because the token name is defined
 * differently depending on the hardware platform, therefore the CREATOR makes
 * sure that token definitions and data stay tagged and known.  The only
 * requirement is that each creator definition must be unique.  Please
 * see hal/micro/token.h for a more complete explanation.
 *@{
 */

// STACK CREATORS
#define CREATOR_STACK_NVDATA_VERSION                         0x0001
#define CREATOR_STACK_ANALYSIS_REBOOT                        0x0002
#define CREATOR_STACK_NODE_DATA                              0x0003
#define CREATOR_STACK_SECURITY_KEY                           0x0004
#define CREATOR_STACK_NONCE_COUNTER                          0x0005
#define CREATOR_STACK_CHILD_TABLE                            0x0006
#define CREATOR_STACK_LAST_ASSIGNED_ID                       0x0007
#define CREATOR_STACK_BOOT_COUNTER                           0x0008

/** @} END Creator Codes  */

//////////////////////////////////////////////////////////////////////////////
// MANUFACTURING DATA
// Since the manufacturing data is platform specific, we pull in the proper
// file here.
#if defined(CORTEXM3)
// cortexm3 handles mfg tokens seperately via mfg-token.h
#elif defined(EMBER_TEST)
  #include "hal/micro/cortexm3/efm32/token-manufacturing.h"
#else
  #error no platform defined
#endif

//////////////////////////////////////////////////////////////////////////////
// STACK DATA
// *the addresses of these tokens must not change*

/**
 * @brief The current version number of the stack tokens.
 * MSB is the version, LSB is a complement.
 *
 * Please see hal/micro/token.h for a more complete explanation.
 */
#define CURRENT_STACK_TOKEN_VERSION 0x03FC //MSB is version, LSB is complement

#ifdef DEFINETYPES

typedef uint16_t tokTypeStackNvdataVersion;
typedef uint16_t tokTypeStackAnalysisReboot;
typedef uint32_t tokTypeStackNonceCounter;

typedef struct {
  uint8_t networkKey[16];
} tokTypeStackKey;

typedef struct {
  uint16_t panId;
  int8_t radioTxPower;
  uint8_t radioFreqChannel;
  uint8_t nodeType;
  uint16_t nodeId;
  uint16_t parentId;
} tokTypeStackNodeData;

typedef struct {
  EmberEUI64 longId;
  EmberNodeId shortId;
  uint8_t flags;
} tokTypeStackChildTableEntry;

typedef uint16_t tokTypeStackLastAllocatedId;

typedef uint16_t tokTypeStackBootCounter;

#endif //DEFINETYPES

#ifdef DEFINETOKENS

DEFINE_BASIC_TOKEN(STACK_NVDATA_VERSION,
                   tokTypeStackNvdataVersion,
                   CURRENT_STACK_TOKEN_VERSION)

DEFINE_BASIC_TOKEN(STACK_ANALYSIS_REBOOT,
                   tokTypeStackAnalysisReboot,
                   0x0000)

DEFINE_BASIC_TOKEN(STACK_SECURITY_KEY,
                   tokTypeStackKey,
                   { 0, })

DEFINE_BASIC_TOKEN(STACK_NODE_DATA,
                   tokTypeStackNodeData,
                   { 0xFFFF, 0, 0xFF, 0xFF, 0xFFFF, 0xFFFF })

DEFINE_BASIC_TOKEN(STACK_LAST_ASSIGNED_ID,
                   tokTypeStackLastAllocatedId,
                   0x0000)

DEFINE_INDEXED_TOKEN(STACK_CHILD_TABLE,
                     tokTypeStackChildTableEntry,
                     EMBER_CHILD_TABLE_TOKEN_SIZE,
                     { 0, })

DEFINE_COUNTER_TOKEN(STACK_NONCE_COUNTER,
                     tokTypeStackNonceCounter,
                     0x00000000)

DEFINE_COUNTER_TOKEN(STACK_BOOT_COUNTER,
                     tokTypeStackBootCounter,
                     0x0000)

#endif //DEFINETOKENS

#ifdef PHY_EM3XX
//////////////////////////////////////////////////////////////////////////////
// PHY DATA
#include "token-phy.h"
#endif

#ifdef MAC_DEBUG_TOKEN
//////////////////////////////////////////////////////////////////////////////
// MAC DEBUG DATA
#include "stack/mac/token-mac-debug.h"
#endif

///////////////////////////////////////////////////////////////////////////////
// APPLICATION DATA

#ifdef APPLICATION_TOKEN_HEADER
  #include APPLICATION_TOKEN_HEADER
#endif

#else //SIM_EEPROM_TEST

//The Simulated EEPROM unit tests define all of their tokens via the
//APPLICATION_TOKEN_HEADER macro.
  #ifdef APPLICATION_TOKEN_HEADER
    #include APPLICATION_TOKEN_HEADER
  #endif

#endif //SIM_EEPROM_TEST

/** @} END addtogroup */
