/***************************************************************************//**
 * @file
 * @brief NVM3 API definition.
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef NVM3_H
#define NVM3_H

#include <stdint.h>
#include <stdbool.h>
#include "nvm3_hal.h"
#include <stddef.h>

#if defined(NVM3_SUPPORT_ENCRYPTION)

// Make Zigbee encryption default if no other is defined
#if !defined(NVM3_USE_MBEDTLS) && !defined(NVM3_USE_ZIGBEECRYPTO)
#define NVM3_USE_ZIGBEECRYPTO
#endif

#if defined(NVM3_USE_MBEDTLS)
#include "mbedtls/entropy.h"
#include "mbedtls/ccm.h"
#include "mbedtls/ctr_drbg.h"
#endif // End NVM3_USE_MBEDTLS

#if defined(NVM3_USE_ZIGBEECRYPTO)
#endif // End NVM3_USE_ZIGBEECRYPTO
#endif // End NVM3_SUPPORT_ENCRYPTION

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************//**
 * @addtogroup emdrv
 * @{
 ******************************************************************************/

/***************************************************************************//**
 * @addtogroup NVM3
 * @{
 ******************************************************************************/

#define ECODE_NVM3_OK                               (ECODE_OK)                                   ///< Success return value
#define ECODE_NVM3_ERR_ALIGNMENT_INVALID            (ECODE_EMDRV_NVM3_BASE | 0x00000001U)        ///< Invalid data alignment
#define ECODE_NVM3_ERR_SIZE_TOO_SMALL               (ECODE_EMDRV_NVM3_BASE | 0x00000002U)        ///< Not enough NVM memory specified
#define ECODE_NVM3_ERR_NO_VALID_PAGES               (ECODE_EMDRV_NVM3_BASE | 0x00000003U)        ///< Initialization aborted, no valid page found
#define ECODE_NVM3_ERR_PAGE_SIZE_NOT_SUPPORTED      (ECODE_EMDRV_NVM3_BASE | 0x00000004U)        ///< The page size is not supported
#define ECODE_NVM3_ERR_OBJECT_SIZE_NOT_SUPPORTED    (ECODE_EMDRV_NVM3_BASE | 0x00000005U)        ///< The object size is not supported
#define ECODE_NVM3_ERR_STORAGE_FULL                 (ECODE_EMDRV_NVM3_BASE | 0x00000006U)        ///< No more NVM space available
#define ECODE_NVM3_ERR_NOT_OPENED                   (ECODE_EMDRV_NVM3_BASE | 0x00000007U)        ///< The module has not been successfully opened
#define ECODE_NVM3_ERR_OPENED_WITH_OTHER_PARAMETERS (ECODE_EMDRV_NVM3_BASE | 0x00000008U)        ///< The module has already been opened with other parameters
#define ECODE_NVM3_ERR_PARAMETER                    (ECODE_EMDRV_NVM3_BASE | 0x00000009U)        ///< Illegal parameter
#define ECODE_NVM3_ERR_KEY_INVALID                  (ECODE_EMDRV_NVM3_BASE | 0x0000000AU)        ///< Invalid key value
#define ECODE_NVM3_ERR_KEY_NOT_FOUND                (ECODE_EMDRV_NVM3_BASE | 0x0000000BU)        ///< Key not found
#define ECODE_NVM3_ERR_OBJECT_IS_NOT_DATA           (ECODE_EMDRV_NVM3_BASE | 0x0000000CU)        ///< Trying to access a data object which is currently a counter object
#define ECODE_NVM3_ERR_OBJECT_IS_NOT_A_COUNTER      (ECODE_EMDRV_NVM3_BASE | 0x0000000DU)        ///< Trying to access a counter object which is currently a data object
#define ECODE_NVM3_ERR_ERASE_FAILED                 (ECODE_EMDRV_NVM3_BASE | 0x0000000EU)        ///< Erase failed
#define ECODE_NVM3_ERR_WRITE_DATA_SIZE              (ECODE_EMDRV_NVM3_BASE | 0x0000000FU)        ///< The object is too large
#define ECODE_NVM3_ERR_WRITE_FAILED                 (ECODE_EMDRV_NVM3_BASE | 0x00000010U)        ///< Error in the write operation
#define ECODE_NVM3_ERR_READ_DATA_SIZE               (ECODE_EMDRV_NVM3_BASE | 0x00000011U)        ///< Trying to read with a length different from actual object size
#define ECODE_NVM3_ERR_READ_FAILED                  (ECODE_EMDRV_NVM3_BASE | 0x00000012U)        ///< Error in the read operation
#define ECODE_NVM3_ERR_INIT_WITH_FULL_NVM           (ECODE_EMDRV_NVM3_BASE | 0x00000013U)        ///< The module was opened with a full NVM
#define ECODE_NVM3_ERR_RESIZE_PARAMETER             (ECODE_EMDRV_NVM3_BASE | 0x00000014U)        ///< Illegal parameter
#define ECODE_NVM3_ERR_RESIZE_NOT_ENOUGH_SPACE      (ECODE_EMDRV_NVM3_BASE | 0x00000015U)        ///< Not enough NVM to complete resize
#define ECODE_NVM3_ERR_ERASE_COUNT_ERROR            (ECODE_EMDRV_NVM3_BASE | 0x00000016U)        ///< Erase counts are not valid
#define ECODE_NVM3_ERR_ADDRESS_RANGE                (ECODE_EMDRV_NVM3_BASE | 0x00000017U)        ///< Address and size is out of range of available NVM
#define ECODE_NVM3_ERR_NVM_NOT_AVAILABLE            (ECODE_EMDRV_NVM3_BASE | 0x00000018U)        ///< The NVM interface is not available like GBL for ext Flash
#define ECODE_NVM3_ERR_NVM_ACCESS                   (ECODE_EMDRV_NVM3_BASE | 0x00000019U)        ///< A NVM function call was failing
#define ECODE_NVM3_ERR_ENCRYPTION_INIT              (ECODE_EMDRV_NVM3_BASE | 0x0000001AU)        ///< Initialization of encryption functions failed
#define ECODE_NVM3_ERR_ENCRYPTION_ENCODE            (ECODE_EMDRV_NVM3_BASE | 0x0000001BU)        ///< Encryption encode failed
#define ECODE_NVM3_ERR_ENCRYPTION_DECODE            (ECODE_EMDRV_NVM3_BASE | 0x0000001CU)        ///< Encryption decode failed
#define ECODE_NVM3_ERR_ENCRYPTION_NOT_SUPPORTED     (ECODE_EMDRV_NVM3_BASE | 0x0000001DU)        ///< Encryption is not supported
#define ECODE_NVM3_ERR_ENCRYPTION_KEY_ERROR         (ECODE_EMDRV_NVM3_BASE | 0x0000001EU)        ///< Encryption key is missing
#define ECODE_NVM3_ERR_RANDOM_NUMBER                (ECODE_EMDRV_NVM3_BASE | 0x0000001FU)        ///< Error in obtaining random number
#define ECODE_NVM3_ERR_INT_WRITE_TO_NOT_ERASED      (ECODE_EMDRV_NVM3_BASE | 0x00000020U)        ///< Write to memory that is not erased
#define ECODE_NVM3_ERR_INT_ADDR_INVALID             (ECODE_EMDRV_NVM3_BASE | 0x00000021U)        ///< Internal error trying to access invalid memory
#define ECODE_NVM3_ERR_INT_KEY_MISMATCH             (ECODE_EMDRV_NVM3_BASE | 0x00000022U)        ///< Key validation failure
#define ECODE_NVM3_ERR_INT_SIZE_ERROR               (ECODE_EMDRV_NVM3_BASE | 0x00000023U)        ///< Internal size mismatch error
#define ECODE_NVM3_ERR_INT_EMULATOR                 (ECODE_EMDRV_NVM3_BASE | 0x00000024U)        ///< Internal Emulator error
#define ECODE_NVM3_ERR_INT_TEST                     (ECODE_EMDRV_NVM3_BASE | 0x00000030U)        ///< Internal Test error

/***************************************************************************//**
 *  @brief Definitions of NVM3 constraints.
 ******************************************************************************/
#define NVM3_MIN_PAGE_SIZE              512U                            ///< The minimum page size supported
#define NVM3_MAX_OBJECT_SIZE_LOW_LIMIT  204U                            ///< The minimum value for the maximum object size
#define NVM3_MAX_OBJECT_SIZE_HIGH_LIMIT 4096U                           ///< The maximum value for the maximum object size
#define NVM3_MAX_OBJECT_SIZE_DEFAULT    1900U                           ///< The default value for the maximum object size

#if !defined(NVM3_MAX_OBJECT_SIZE)
#define NVM3_MAX_OBJECT_SIZE            NVM3_MAX_OBJECT_SIZE_DEFAULT
#endif

#define NVM3_MIN_FRAGMENT_COUNT         (2U)
#if defined(FLASH_PAGE_SIZE)
#define NVM3_MAX_OBJECT_SIZE_X          (NVM3_MAX_OBJECT_SIZE + 8)      // Adjust for an object header
#define FLASH_PAGE_SIZE_X               (FLASH_PAGE_SIZE - 20)          // Adjust for a page header
#define NVM3_FRAGMENT_COUNT             (((NVM3_MAX_OBJECT_SIZE_X - 1) / FLASH_PAGE_SIZE_X) + NVM3_MIN_FRAGMENT_COUNT)
#endif

/***************************************************************************//**
 *  @brief NVM3 static data definition helper macro for applications using linker
 *  script placement of the NVM memory area. This macro exports the section 'name'_section
 *  to the linker. The user must place the section name in a linker script
 *  at an address aligned with the page size of the underlying memory system. The size of
 *  the NVM area must be a multiple of the page size.
 *  @n This macro also allocates the static NVM3 cache.
 *  @n Use this macro with @ref NVM3_DEFINE_SECTION_INIT_DATA() to create initialization
 *  data for @ref nvm3_open(). See @ref nvm3_example section for usage examples.
 ******************************************************************************/
#define NVM3_DEFINE_SECTION_STATIC_DATA(name, nvmSize, cacheSize) \
  static nvm3_CacheEntry_t name##_cache[cacheSize];               \
  static const uint8_t name##_nvm[nvmSize]                        \
  SL_ATTRIBUTE_SECTION(STRINGIZE(name##_section))

/***************************************************************************//**
 *  @brief NVM3 initialization data helper macro to be used with @ref
 *  NVM3_DEFINE_SECTION_STATIC_DATA(). The @p name parameter in both macros must
 *  match.
 *  @n Call @ref nvm3_open() after this macro to initialize NVM3. See @ref
 *  nvm3_example section for code examples.
 ******************************************************************************/
#if defined(NVM3_SUPPORT_ENCRYPTION) && defined(NVM3_USE_ZIGBEECRYPTO)
#define NVM3_DEFINE_SECTION_INIT_DATA(name, flashHandle) \
  nvm3_Init_t name =                                     \
  {                                                      \
    (nvm3_HalPtr_t)name##_nvm,                           \
    sizeof(name##_nvm),                                  \
    name##_cache,                                        \
    sizeof(name##_cache) / sizeof(nvm3_CacheEntry_t),    \
    NVM3_MAX_OBJECT_SIZE,                                \
    0,                                                   \
    flashHandle,                                         \
    NULL,                                                \
    NULL,                                                \
  }
#elif defined(NVM3_SUPPORT_ENCRYPTION) && !defined(NVM3_USE_ZIGBEECRYPTO)
#define NVM3_DEFINE_SECTION_INIT_DATA(name, flashHandle) \
  nvm3_Init_t name =                                     \
  {                                                      \
    (nvm3_HalPtr_t)name##_nvm,                           \
    sizeof(name##_nvm),                                  \
    name##_cache,                                        \
    sizeof(name##_cache) / sizeof(nvm3_CacheEntry_t),    \
    NVM3_MAX_OBJECT_SIZE,                                \
    0,                                                   \
    flashHandle,                                         \
    NULL,                                                \
  }
#else
#define NVM3_DEFINE_SECTION_INIT_DATA(name, flashHandle) \
  nvm3_Init_t name =                                     \
  {                                                      \
    (nvm3_HalPtr_t)name##_nvm,                           \
    sizeof(name##_nvm),                                  \
    name##_cache,                                        \
    sizeof(name##_cache) / sizeof(nvm3_CacheEntry_t),    \
    NVM3_MAX_OBJECT_SIZE,                                \
    0,                                                   \
    flashHandle,                                         \
  }
#endif

#define NVM3_KEY_INVALID            0xFFFFFFFFU                   ///< Invalid key identifier
#define NVM3_KEY_SIZE               20U                           ///< Unique object key identifier size in number of bits
#define NVM3_KEY_MASK               ((1U << NVM3_KEY_SIZE) - 1U)  ///< Unique object key identifier mask
#define NVM3_KEY_MIN                0U                            ///< Minimum object key value
#define NVM3_KEY_MAX                NVM3_KEY_MASK                 ///< Maximum object key value

#define NVM3_OBJECTTYPE_DATA        0U                            ///< The object is data
#define NVM3_OBJECTTYPE_COUNTER     1U                            ///< The object is a counter

/// @brief The data type for object keys. Only the 20 least significant bits are used.
typedef uint32_t nvm3_ObjectKey_t;

/// @brief The datatype for each cache entry. The cache must be an array of these.
typedef struct nvm3_CacheEntry {
  nvm3_ObjectKey_t key;           ///< key
  void             *ptr;          ///< pointer
} nvm3_CacheEntry_t;

/// @cond DO_NOT_INCLUDE_WITH_DOXYGEN

typedef struct nvm3_Cache {
  nvm3_CacheEntry_t *entryPtr;
  size_t            entryCount;
  bool              overflow;
} nvm3_Cache_t;

typedef struct nvm3_ObjFragDetail {
  void                *adr;
  uint16_t            len;
  uint8_t             typ;
} nvm3_ObjFragDetail_t;

typedef struct nvm3_ObjFrag {
  uint8_t              idx;                 // Fragment index
  bool                 isFirstFragFound;    // The object first fragment found
  bool                 isLastFragFound;     // The object last fragment found
#if defined(FLASH_PAGE_SIZE)
  nvm3_ObjFragDetail_t detail[NVM3_FRAGMENT_COUNT];
#else
  nvm3_ObjFragDetail_t detail[NVM3_MIN_FRAGMENT_COUNT];
#endif
} nvm3_ObjFrag_t;

typedef struct nvm3_Obj {
  nvm3_ObjectKey_t  key;          // The object key
  struct nvm3_Obj   *objAdr;      // The object pointer
  struct nvm3_Obj   *nextObjAdr;  // The next object pointer
  const void        *srcPtr;      // May be used to carry the source address of the data
  size_t            totalLen;     // The object total length
  uint8_t           objType;      // The object type
  bool              isHdrValid;   // The object header is valid
  bool              isValid;      // The object is valid
  bool              isFragmented; // The object is fragmented
  nvm3_ObjFrag_t    frag;         // The object fragment information
} nvm3_Obj_t;

#if defined(NVM3_USE_ZIGBEECRYPTO)
typedef uint32_t (*nvm3_GetRandomNumber_t)(void);
#endif

typedef struct {
  nvm3_HalPtr_t nvmAdr;               // NVM address
  size_t nvmSize;                     // NVM size
  nvm3_Cache_t cache;                 // Cache management data
  size_t maxObjectSize;               // The maximum object size in bytes
  size_t repackHeadroom;              // The size difference between the user and forced repacks
  size_t totalNvmPageCnt;             // The total number of NVM pages
  size_t validNvmPageCnt;             // The number of valid NVM pages
  size_t fifoFirstIdx;                // FIFO bottom page
  void *fifoFirstObj;                 // The first object location
  void *fifoNextObj;                  // The next free object location
  size_t unusedNvmSize;               // The size of the unused NVM
  bool hasBeenOpened;                 // Open status
  size_t minUnused;                   // The minimum value of the unusedNvmSize
  const nvm3_HalHandle_t *halHandle;  // HAL handle
  nvm3_HalInfo_t halInfo;             // HAL information
  bool encrypted;                     // Encryption information
#if defined(NVM3_SUPPORT_ENCRYPTION)
  uint8_t *cryptoKey;                 // Pointer to the 128 bit crypto key
#if defined(NVM3_USE_MBEDTLS)
  mbedtls_entropy_context entropyCtx; // Entropy context
  mbedtls_ctr_drbg_context drbgCtx;   // Drbg context
  mbedtls_ccm_context ccmCtx;         // CCM context
  mbedtls_aes_context aesCtx;         // AES context
#endif // NVM3_USE_MBEDTLS
#if defined(NVM3_USE_ZIGBEECRYPTO)
  nvm3_GetRandomNumber_t getRndNumber; // Get a random number
#endif
#endif
} nvm3_Handle_t;

/// @endcond

/// @brief NVM3 initialization data.
typedef struct {
  nvm3_HalPtr_t nvmAdr;               ///< NVM memory area base address
  size_t nvmSize;                     ///< NVM memory area size in bytes
  nvm3_CacheEntry_t *cachePtr;        ///< A pointer to cache
  size_t cacheEntryCount;             ///< The size of the cache in number of elements
  size_t maxObjectSize;               ///< The maximum object size in bytes
  size_t repackHeadroom;              ///< The size difference between the user and forced repacks
  const nvm3_HalHandle_t *halHandle;  ///< HAL handle
#if defined(NVM3_SUPPORT_ENCRYPTION)
#if defined(NVM3_USE_ZIGBEECRYPTO)
  nvm3_GetRandomNumber_t getRndNumber; ///< Get a random number
#endif
  uint8_t *cryptoKey;                 ///< Pointer to the 128 bit crypto key
#endif
} nvm3_Init_t;

// Definition of NVM3 variables
/// @brief A variable used by the NVM3 functions.
extern nvm3_Obj_t nvm3_internalObjectHandleA;
/// @brief A variable used by the NVM3 functions.
extern nvm3_Obj_t nvm3_internalObjectHandleB;
/// @brief A variable used by the NVM3 functions.
extern nvm3_Obj_t nvm3_internalObjectHandleC;
/// @brief A variable used by the NVM3 functions.
extern nvm3_Obj_t nvm3_internalObjectHandleD;
/// @brief A variable that must contain the maximum number of object fragments.
extern const uint8_t  nvm3_maxFragmentCount;
extern const size_t   nvm3_objHandleSize;

/***************************************************************************//**
 * @brief
 *  Open an NVM3 driver instance, which is represented by a handle
 *  keeping information about the state. A successful open will initialize
 *  the handle and the cache with information about the objects already in the
 *  NVM-memory.
 *  Several NVM3 instances using different handles must NOT overlap NVM-memory.
 *  To change some of the parameters,
 *  first call @ref nvm3_close and then @ref nvm3_open.
 *  @note The driver handle must be initialized to zero before it is used the
 *  first time. The @ref nvm3_open can be called repeatedly with the same handle
 *  and initialization data. In that case, the next calls will be regarded as a
 *  "no operation" and the function will return the same status as the previous
 *  call.
 *
 * @param[out] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[in] i
 *   A pointer to NVM3 driver initialization data.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success and a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_open(nvm3_Handle_t *h, const nvm3_Init_t *i);

/***************************************************************************//**
 * @brief
 *  Close the NVM3 driver instance.
 *
 * @param[in] h
 *   A pointer to the NVM3 driver handle.
 *
 * @return
 *   @ref ECODE_NVM3_OK is always returned.
 ******************************************************************************/
Ecode_t nvm3_close(nvm3_Handle_t *h);

/***************************************************************************//**
 * @brief
 *  Write the object value identified with the key to NVM.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[in] key
 *   A 20-bit object identifier.
 *
 * @param[in] value
 *   A pointer to the object data to write.
 *
 * @param[in] len
 *   The size of the object data in number of bytes.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_writeData(nvm3_Handle_t *h, nvm3_ObjectKey_t key, const void *value, size_t len);

/***************************************************************************//**
 * @brief
 *  Read the object data identified with a given key from NVM.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[in] key
 *   A 20-bit object identifier.
 *
 * @param[out] value
 *   A pointer to the application data buffer. The read function will copy
 *   data to this location.
 *
 * @param[in] maxLen
 *   The maximum object size in number of bytes. The @ref nvm3_getObjectInfo() function
 *   can be used to find the actual size.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_readData(nvm3_Handle_t *h, nvm3_ObjectKey_t key, void *value, size_t maxLen);

/***************************************************************************//**
 * @brief
 *  Find the type and size of an object in NVM.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[in] key
 *   A 20-bit object identifier.
 *
 * @param[out] type
 *   A pointer to the location where NVM3 shall write the object type. The type can
 *   be either @ref NVM3_OBJECTTYPE_DATA or @ref NVM3_OBJECTTYPE_COUNTER.
 *
 * @param[out] len
 *   A pointer to the location where NVM3 writes the object size.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_getObjectInfo(nvm3_Handle_t *h, nvm3_ObjectKey_t key,
                           uint32_t *type, size_t *len);

/***************************************************************************//**
 * @brief
 *  Create a list of object keys for valid objects in NVM.
 *
 * @note
 *  The function @ref nvm3_countObjects() is also provided to count the
 *  number of valid objects.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[out] keyListPtr
 *   A pointer to a buffer for the key list.
 *
 * @param[in] keyListSize
 *   The number of elements in the key list buffer. If the keyListSize = 0,
 *   the @p keyListPtr can be NULL and the function will return the total
 *   number of objects.
 *
 * @param[in] keyMin
 *   The lower search key. Set to @ref NVM3_KEY_MIN to match all keys.
 *
 * @param[in] keyMax
 *   The upper search key. Set to @ref NVM3_KEY_MAX to match all keys.
 *
 * @return
 *   The number of keys written to the key list. This value is less than or equal
 *   to @p keyListSize. If the @p keyListSize = 0, the function will return the
 *   total number of objects matching the key Min - Max pattern.
 ******************************************************************************/
size_t  nvm3_enumObjects(nvm3_Handle_t *h,
                         nvm3_ObjectKey_t *keyListPtr, size_t keyListSize,
                         nvm3_ObjectKey_t keyMin, nvm3_ObjectKey_t keyMax);

/***************************************************************************//**
 * @brief
 *  Create a list of object keys for deleted objects in NVM.
 *
 * @note
 *  The function @ref nvm3_countDeletedObjects() is also provided to count the
 *  number of deleted objects.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[out] keyListPtr
 *   A pointer to a buffer for the key list.
 *
 * @param[in] keyListSize
 *   The number of elements in the key list buffer. If the keyListSize = 0,
 *   the @p keyListPtr can be NULL and the function will return the total
 *   number of objects.
 *
 * @param[in] keyMin
 *   The lower search key. Set to @ref NVM3_KEY_MIN to match all keys.
 *
 * @param[in] keyMax
 *   The upper search key. Set to @ref NVM3_KEY_MAX to match all keys.
 *
 * @return
 *   The number of keys written to the key list. This value is less than or equal
 *   to @p keyListSize. If the @p keyListSize = 0, the function will return the
 *   total number of objects matching the key Min - Max pattern.
 ******************************************************************************/
size_t  nvm3_enumDeletedObjects(nvm3_Handle_t *h,
                                nvm3_ObjectKey_t *keyListPtr, size_t keyListSize,
                                nvm3_ObjectKey_t keyMin, nvm3_ObjectKey_t keyMax);

/***************************************************************************//**
 * @brief
 *  Delete an object from NVM.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[in] key
 *   A 20-bit object identifier.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_deleteObject(nvm3_Handle_t *h, nvm3_ObjectKey_t key);

/***************************************************************************//**
 * @brief
 *  Store a counter in NVM.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[in] key
 *   A 20-bit object identifier.
 *
 * @param[in] value
 *   The counter value to write.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_writeCounter(nvm3_Handle_t *h, nvm3_ObjectKey_t key, uint32_t value);

/***************************************************************************//**
 * @brief
 *  Read a counter value from NVM.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[in] key
 *   A 20-bit object identifier.
 *
 * @param[out] value
 *   A pointer to the counter location. The read function will copy
 *   the counter value to this location.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_readCounter(nvm3_Handle_t *h, nvm3_ObjectKey_t key, uint32_t *value);

/***************************************************************************//**
 * @brief
 *  Increment a counter object value by 1 and read out optionally.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[in] key
 *   A 20-bit object identifier.
 *
 * @param[out] newValue
 *   A pointer to the counter readout location. The counter is incremented before the value
 *   is written to this location. Set this value to NULL to ignore readout.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_incrementCounter(nvm3_Handle_t *h, nvm3_ObjectKey_t key, uint32_t *newValue);

/***************************************************************************//**
 * @brief
 *  Delete all objects in NVM.
 *
 * @note
 *  Users don't need to call this function to get NVM3 into an
 *  initial valid state.
 *
 * @warning
 *  The execution time depends on the configured NVM size and may therefore be
 *  significant.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_eraseAll(nvm3_Handle_t *h);

/***************************************************************************//**
 * @brief
 *  Get the number of page erases of the most erased page in the NVM area since
 *  the first initialization.
 *
 * @note
 *  Except for pages marked as bad, pages will have an erase count equal to the
 *  most erased or one less because of the wear leveling algorithm.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[in] eraseCnt
 *   A pointer to the location where the NVM3 shall place the page
 *   erasure counter value.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_getEraseCount(nvm3_Handle_t *h, uint32_t *eraseCnt);

/***************************************************************************//**
 * @brief
 *   Set the page erase count.
 *   Normally, the application should not be concerned with the erase count value.
 *   If NVM3 is substituting a previous solution, it is possible to transfer
 *   the erase count to NVM3 when initializing the NVM for the first time.
 *   The erase count must be set before the @ref nvm3_open is called and will
 *   only take effect if the NVM is completely erased or contains unknown
 *   data to NVM3. In that case, all pages will be initialized with the supplied
 *   erase count. After @ref nvm3_open is called, the value will be consumed
 *   and will have no effect on further calls to @ref nvm3_open.
 *
 * @param[in] eraseCnt
 *   The erase count.
 ******************************************************************************/
void nvm3_setEraseCount(uint32_t eraseCnt);

/***************************************************************************//**
 * @brief
 *  Execute a repack operation. NVM3 will copy data or erase pages when repacking
 *  is needed. Calling @ref nvm3_repack() may block access to the non-volatile
 *  memory for up to one page erasure time plus an small execution overhead.
 *  The exact worst-case timing characteristics can be found in the data sheet for the
 *  part.
 *
 * @note
 *  Calling @ref nvm3_repack() is not mandatory because the functions that
 *  write data to NVM will trigger a repack if needed. Because a
 *  repack operation may be time consuming, the application may want to be
 *  in control of when repacking occurs by calling this function.
 *
 *  More information about the repack operation can be found in the
 * @ref nvm3_repack section.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_repack(nvm3_Handle_t *h);

/***************************************************************************//**
 * @brief
 *   Check the internal status of NVM3 and return true if a repack
 *   operation is required. The application must call @ref nvm3_repack() to
 *   perform the actual repack operation.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @return
 *   true if repacking is needed, false if repacking is not needed.
 ******************************************************************************/
bool    nvm3_repackNeeded(nvm3_Handle_t *h);

/***************************************************************************//**
 * @brief
 *   Resize the NVM area used by an open NVM3 instance.
 *   The area can be resized by changing the start or end address either up
 *   or down in memory. Because the input parameters to NVM3 are start address
 *   and size, users should be cautious. Either move the start address up or
 *   down in memory and adjust the size accordingly to keep the end address,
 *   or keep the address and change the size only.
 *   It is not possible to resize the area by doing changes in both ends of the
 *   NVM address range at the same time.
 *   If the resize operation returns @ref ECODE_NVM3_OK, the instance is still
 *   open and can be used to access objects in the resized NVM.
 *   If the resize operation fails, the instance will still be open but with
 *   unchanged size.
 *
 * @note
 *   It is possible to decrease the NVM area to a new size that is not capable
 *   of keeping the already stored objects. The result is loss of data.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @param[in] newAddr
 *   The start address of the NVM after resize.
 *
 * @param[in] newSize
 *   The size of the NVM after resize.
 *
 * @return
 *   @ref ECODE_NVM3_OK on success or a NVM3 @ref Ecode_t on failure.
 ******************************************************************************/
Ecode_t nvm3_resize(nvm3_Handle_t *h, nvm3_HalPtr_t newAddr, size_t newSize);

/***************************************************************************//**
 * @brief
 *  Count valid objects.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @return
 *   The number of valid objects.
 ******************************************************************************/
__STATIC_INLINE size_t nvm3_countObjects(nvm3_Handle_t *h)
{
  return nvm3_enumObjects(h, NULL, 0, NVM3_KEY_MIN, NVM3_KEY_MAX);
}

/***************************************************************************//**
 * @brief
 *  Count deleted objects.
 *
 * @param[in] h
 *   A pointer to an NVM3 driver handle.
 *
 * @return
 *   The number of deleted objects.
 ******************************************************************************/
__STATIC_INLINE size_t nvm3_countDeletedObjects(nvm3_Handle_t *h)
{
  return nvm3_enumDeletedObjects(h, NULL, 0, NVM3_KEY_MIN, NVM3_KEY_MAX);
}

/** @} (end addtogroup NVM3) */
/** @} (end addtogroup emdrv) */

#ifdef __cplusplus
}
#endif

#include "nvm3_default.h"

/************ THIS SECTION IS FOR DOCUMENTATION ONLY !**********************//**
 * @addtogroup emdrv
 * @{
 * @addtogroup NVM3
 * @brief NVM3 Non-Volatile Memory Management driver
 * @{

   @note Using NVM3 to store data in external Flash is beta tested only.
   Silicon Labs will NOT support any production deployments with this Beta
   release.
   Production deployments will only be supported with the GA version of this
   feature. This version is intended for lab and evaluation purpose only.

   @details
   @li @ref nvm3_intro
   @li @ref nvm3_objects
   @li @ref nvm3_repack
   @li @ref nvm3_cache
   @li @ref nvm3_data
   @li @ref nvm3_stack
   @li @ref nvm3_api
   @li @ref nvm3_memory_placement
   @li @ref nvm3_configuration
   @li @ref nvm3_bad_page_handling
   @li @ref nvm3_error_handling
   @li @ref nvm3_support_internal_flash
   @li @ref nvm3_support_external_flash
   @li @ref nvm3_libraries
   @li @ref nvm3_example

   @n @section nvm3_intro Introduction
   The NVM3 driver provides a way for an application to safely store and
   retrieve variable size objects in a page-based non-volatile memory.
   Objects are identified with 20-bit object identifiers denoted as keys.

   The driver is designed to use pages in a sequential order to provide equal
   usage and wear. The driver is resilient to power loss or reset events,
   ensuring that objects retrieved from the driver are in a valid state. A
   valid object will always be the last successfully stored object. NVM3 can
   detect NVM defects and mark pages as unusable. NVM3 will continue to operate
   on good pages after defect pages are detected.

   @n @section nvm3_objects Objects
   An NVM3 object is data that can be stored in NVM. The object
   is handled as an array of bytes up to @ref NVM3_MAX_OBJECT_SIZE in size.
   NVM3 can handle two types of objects.
   -# Regular data objects. Data objects can store information of
   any size up to maximum @ref NVM3_MAX_OBJECT_SIZE bytes.
   -# 32-bit counter objects. Counter objects can store 32-bit counters that are
   accessed with a separate set of API functions. The counter object is designed
   to be compact while minimizing memory wear in applications that require
   frequent persistent counter increments.

   See @ref nvm3_api for more details on the API.

   @n @section nvm3_repack Repacking
   As the NVM fills up, it reaches a point where it can no longer store
   additional objects and a repacking operation is required to release
   out-of-date objects to free up NVM. Because erasing pages takes a long time,
   the NVM3 driver does not trigger the process by itself unless free memory
   reaches a critical low level. As an alternative, the application can trigger
   the repacking process by calling the @ref nvm3_repack() function.
   During the call, NVM3 will either move data to a new page or erase pages
   that can be reused. At most, the call will block for a period equal to a page
   erasure time plus a small execution overhead. Page erasure time for the
   EFM32 or EFR32 parts can be found in the data sheet.

   The application can use @ref nvm3_repackNeeded() to determine if repacking
   is needed. To initiate repacks, call @ref nvm3_repack(). Note that
   this function will perform repacks only if they are needed.

   NVM3 uses two thresholds for repacking:
   -# Forced threshold. This is the threshold used to force automatic repacking
      when free memory reaches a critical low level.
   -# User threshold. This is the threshold used by @ref nvm3_repackNeeded().
      @ref nvm3_repack() will not perform repacking unless free memory is below
      this threshold.

   An NVM3 function that deletes or modifies a data or counter object will trigger
   an automatic repack operation when free memory is below the forced threshold.
   The check is done before the object is modified, not after.

   The user can define the user threshold by entering a value in the repackHeadroom
   member of the @ref nvm3_Init_t structure used by the @ref nvm3_open() function.
   The repackHeadroom value defines the difference between the user and forced
   threshold. The forced threshold is the minimum low memory threshold defined
   by the page size and maximum object size and cannot be changed by the user.
   The default value for the repack headroom is 0, meaning that the forced and
   user thresholds are equal.

   @note The repack threshold can be changed to prevent multiple modifications
   of objects between user called repacks from causing automatic repacks. Note
   that "high" values of the repack headroom may cause
   increased NVM wear from increased number of repacks.

   @n @section nvm3_cache Caching
   Caching is an optional feature. The NVM3 cache is an object location lookup
   cache. Data is not stored in the cache. Using cache speeds up object
   access and the performance will very much depend on objects being
   available in the cache. To ensure that cache can hold all necessary
   information, it must be configured to a size equivalent to or larger than the
   number of objects stored in NVM including those deleted as long as they are
   not discarded by the repack function. If the cache is available,
   the driver will first look in the cache to find the position of the object in NVM.
   If the object position is not found in the cache, the object position will be
   found by searching the NVM. The search will start at the last stored object
   and search all the way to the oldest object. If the object is found, the cache
   is updated accordingly.

   The application must allocate and support data for the cache.
   See the @ref nvm3_open function for more details. The size of each cache
   element is one uint32_t and one pointer giving a total of 8 bytes (2 words)
   pr. entry for EFM32 and EFR32 devices.

   @note The cache is fully initialized by @ref nvm3_open() and automatically
   updated by any subsequent write, read, or delete function call.

   @n @section nvm3_data Global Data (variables)
   The NVM3 library is using global variables to store intermediate data
   during open, read, write, increment, and delete calls. Because the actual
   memory configuration is not defined at the time the NVM3 library is built,
   but rather at the time the user application is built, the size of data
   structures must be determined by the application configuration.
   Also, the application must set the value of the @ref nvm3MaxFragmentCount
   at run-time before any NVM3 functions are called.

   NVM3 does not support overlapped calls. If there is any chance that the
   application can issue overlapped calls, the NVM3 locking mechanism must be
   present and protect from that.

   @note If the application is using more than one NVM3 instance, the variables
   will be shared between the instances. Be sure to allocate data that have a
   size that is large enough for the largest usage.

   @n @section nvm3_stack Stack Usage
   NVM3 library function calls are nested several levels deep. The stack
   usage has been measured on some EFM32 and EFR32 targets with library builds
   for IAR and armgcc. The maximum stack usage measured was 420 bytes for
   IAR and 472 bytes for ARM GCC builds. The unit test used to validate the
   stack usage has a 10% margin and is using a stack limit at 462 bytes for IAR
   and 520 for ARM GCC.
   Please note that the actual stack usage is a little different on the Cortex
   M0 Plus, M3, M4 and M33 versions of the library.

   @n @section nvm3_api The API
   The NVM3 API is defined in the @ref nvm3.h file, and the application code
   must include the @ref nvm3.h header file to get access to all definitions,
   datatypes, and function prototypes defined by NVM3.

   This section contains brief descriptions of NVM3 functions. For
   more information about parameters and return values see the Function
   Documentation section. Most functions return an @ref Ecode_t that has the
   value @ref ECODE_NVM3_OK on success or see @ref nvm3.h for other values.

   @ref nvm3_open() and @ref nvm3_close().
   @n Functions to open and close an NVM3 instance. @ref nvm3_open() takes a handle
   of type nvm3_Handle_t and initialization data of type @ref nvm3_Init_t.
   The helper macro pair @ref NVM3_DEFINE_SECTION_STATIC_DATA() and
   @ref NVM3_DEFINE_SECTION_INIT_DATA() are provided to simplify initialization
   data definition. For usage examples, see the @ref nvm3_example section.

   @ref nvm3_getObjectInfo(), @ref nvm3_enumObjects(), @ref nvm3_deleteObject()
   and nvm3_countObjects()
   @n These functions work on all objects. @ref nvm3_enumObjects()
   gets a list of keys to valid objects in the NVM. The search can also
   be constrained by the function parameters.
   @ref nvm3_countObjects() can be useful at startup to distinguish between a
   first startup without any valid objects present and later reboots with valid
   objects persistently stored in NVM.

   @ref nvm3_writeData() and @ref nvm3_readData()
   @n Write and read data objects.

   @ref nvm3_writeCounter(), @ref nvm3_readCounter() and @ref nvm3_incrementCounter()
   @n Write, read, and increment 32-bit counter objects.

   @ref nvm3_eraseAll()
   @n Erase all objects in NVM.

   @ref nvm3_getEraseCount()
   @n Return the erasure count for the most erased page in NVM.

   @ref nvm3_repack() and @ref nvm3_repackNeeded()
   @n Manage NVM3 repacking operations.

   @ref nvm3_resize()
   @n Resize the NVM area used by an open NVM3 instance.

   @n @section nvm3_memory_placement Memory Placement
   The application is responsible for placing the NVM area correctly. Minimum
   requirements for memory placement are:
   -# NVM area start address must be aligned with a page of the underlying
    memory system.
   -# NVM area size must be a multiple of the page size.

   The minimum required NVM size is dependent on both the NVM page size and the
   NVM3_MAX_OBJECT_SIZE value. For a device with 2 kB page size and some typical
   values for NVM3_MAX_OBJECT_SIZE, the following is the minimum required
   number of pages:
     - For NVM3_MAX_OBJECT_SIZE=208:  3 pages
     - For NVM3_MAX_OBJECT_SIZE=1900: 4 pages
     - For NVM3_MAX_OBJECT_SIZE=4096: 5 pages

   @ref NVM3_DEFINE_SECTION_STATIC_DATA() and @ref NVM3_DEFINE_SECTION_INIT_DATA()
   macros are provided to support the creation of the NVM area and initialization
   data.
   A linker section called 'name'_section is defined by @ref NVM3_DEFINE_SECTION_STATIC_DATA().
   The NVM area is placed within the linker section. The application linker script
   must place the section according to the requirements above. An error is returned
   by @ref nvm3_open() on alignment or size violation.

   @n @section nvm3_configuration Configuration Options
   There are no compile-time configuration options for NVM3. All configuration
   parameters are contained in @ref nvm3_Init_t.

   @note The @ref nvm3_data must however be configured for correct
   size and have correct values for NVM3 to behave correctly.

   @n @section nvm3_bad_page_handling Bad NVM Page Handling
   NVM3 has been designed to detect page erase and write errors during
   normal operation and mark failing pages as BAD. If a write operation
   fails, all objects that have been written to the page prior to the write
   error are copied to the next free page before the page is marked as BAD and
   the write operation resumes. If the recover operation is successful, the
   operation is regarded as complete and the function will return @ref
   ECODE_NVM3_OK status.

   @note Erase and write errors may not be detected by NVM3 if the device is
   used until end of life where the failure mode can be that the NVM content is
   changing during a power cycle.

   @n @section nvm3_error_handling Error Handling
   The NVM3 error handling involves most functions
   returning an error code. The @ref nvm3_countObjects is
   different because it returns the actual number of objects
   found.

   The behavior and return values for most functions, such as @ref
   nvm3_readData, @ref nvm3_writeData, and so on should be self explanatory,
   while the @ref nvm3_open is slightly different. @ref nvm3_open will always try
   to recover from the previous state and continue without an error, if possible.
   In other words, if a valid NVM3 instance is
   established, @ref nvm3_open will recover from brown outs and power cycles at
   any time in any operation and bring the system to a valid state where all
   pages and objects are in a known state and return success whenever possible.
   From this state, normal operation can resume. If @ref nvm3_open returns an
   error, it's an indication of either a design or coding error,
   or that many of the NVM pages have been marked as BAD leaving insufficient
   space in the NVM to progress. Operation may not resume if @ref nvm3_open
   returns an error.

   @note Because the @ref nvm3_open may need to do recovery operations,
   the execution time will occasionally vary.

   @n @section nvm3_support_internal_flash Storing objects in internal Flash
   NVM3 has support for writing and reading objects in internal i.e., memory
   mapped Flash memory through the nvm3_hal_flash.c "driver". nvm3_hal_flash.c
   is using EMLIB functions to write and erase data, while using regular
   memory functions to read data from Flash.

   @n The "driver" for internal Flash is selected by setting the halHandle in
   the nvm3_open initialization structure to point to nvm3_halFlashHandle.

   @n @section nvm3_support_external_flash Storing objects in external Flash
   MNV3 has support for writing and reading objects in external Flash by using
   the nvm3_hal_extflash.c "driver". The nvm3_hal_extflash.c is using the Gecko
   Bootloader to read, write, and erase data in the Flash. By using this scheme,
   NVM3 does not deal with the hardware directly. All Flash communication must
   be handled by the bootloader. As long as the bootloader is
   configured and built for hardware, supports raw reads, writes and
   erases, NVM3 should be good to go.

   @n nvm3_hal_extflash.c is using the "raw" read and write functions. It does
   not use the slot feature supported by the Gecko Bootloader.
   Be careful when configuring the bootloader memory map to avoid conflicts
   between NVM3 and other usage, including slot configuration.

   @n All data objects are encrypted using CCM encryption and counter base
   values are encrypted using AES encryption. NVM3 encryption keys are stored
   in internal Flash. Reading the external memory alone will not give access to
   the unencrypted data or counter values.

   @n The "driver" for external Flash is selected by setting the halHandle in
   the nvm3_open initialization structure to point to nvm3_halExtFlashHandle.

   @n @section nvm3_libraries NVM3 Libraries
   The NVM3 comes with pre-compiled libraries for Cortex M0, M3, M4 and M33
   compiled with either Arm GCC or IAR toolchains. In addition, there are versions
   with and without encryption support. The versions without encryption support
   can only be used to store data in internal Flash, while the ones with
   encryption support can be used for both internal and external Flash.
   Objects in internal Flash will never be encrypted, while objects in external
   Flash must always be encrypted.

   @n @section nvm3_example Examples

   Example 1 shows initialization, usage of data objects and repacking.

   @include nvm3_example_1.c

   Example 2 shows initialization and usage of counter objects. The
   counter object uses a compact way of storing a 32-bit counter value while minimizing
   NVM wear.

   @include nvm3_example_2.c

 * @} end group NVM3 ********************************************************
 * @} end group emdrv ****************************************************/

#endif /* NVM3_H */
