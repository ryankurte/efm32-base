/**
 * @file config.h
 * See @ref stack_info for documentation.
 */

/** @addtogroup version
 * See also config.h.
 *
 * This documentation was produced from the following software release and build.
 * <table border=1>
 * <tr><td>SOFTWARE_VERSION</td><td>0x4700</td><td>High byte = release number,
 * low byte = patch number</td></tr>
 * </table>
 *
 * @{
 */

// The 4 digit version: A.B.C.D
#define EMBER_MAJOR_VERSION   1
#define EMBER_MINOR_VERSION   2
#define EMBER_PATCH_VERSION   1
#define EMBER_SPECIAL_VERSION 0

// 2 bytes
#define EMBER_BUILD_NUMBER   64

#define EMBER_FULL_VERSION (  ((uint16_t)EMBER_MAJOR_VERSION << 12)   \
                              | ((uint16_t)EMBER_MINOR_VERSION <<  8) \
                              | ((uint16_t)EMBER_PATCH_VERSION <<  4) \
                              | ((uint16_t)EMBER_SPECIAL_VERSION))

#define EMBER_VERSION_TYPE EMBER_VERSION_TYPE_GA

/**
 * Software version. High byte = release number, low byte = patch number.
 */
#define SOFTWARE_VERSION EMBER_FULL_VERSION

/** @} // End group
 */
