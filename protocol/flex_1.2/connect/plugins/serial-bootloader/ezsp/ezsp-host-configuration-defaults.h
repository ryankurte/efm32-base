/** @file ezsp-host-configuration-defaults.h
 * @brief User-configurable parameters for host applications.
 *
 * The default values set in this file can be overridden by putting
 * \#defines into the host application's CONFIGURATION_HEADER.
 *
 * See @ref configuration for documentation.
 *
 */

/**
 * @addtogroup configuration
 *
 * See ezsp-host-configuration-defaults.h for source code.
 * @{
 */

#ifdef CONFIGURATION_HEADER
  #include CONFIGURATION_HEADER
#endif

#ifndef EZSP_HOST_SOURCE_ROUTE_TABLE_SIZE
/** @brief The size of the source route table on the EZSP host.
 *
 * @note This configuration value sets the size of the source route table
 * on the host, not on the node.
 * ::EMBER_SOURCE_ROUTE_TABLE_SIZE sets ::EZSP_CONFIG_SOURCE_ROUTE_TABLE_SIZE
 * if ezsp-utils.c is used, which sets the size of the source route table on
 * the NCP.
 */
  #define EZSP_HOST_SOURCE_ROUTE_TABLE_SIZE 32
#endif

#ifndef EZSP_HOST_RX_POOL_SIZE
/** @brief Define the size of the receive buffer pool on the EZSP host.
 *
 * The number of receive buffers does not need to be greater than the
 * number of packet buffers available on the ncp, because this
 * in turn is the maximum number of callbacks that could be received between
 * commands.  In reality a value of 20 is a generous allocation.
 */
  #define EZSP_HOST_RX_POOL_SIZE 20
#endif

#ifndef EZSP_HOST_FORM_AND_JOIN_BUFFER_SIZE
/** @brief The size of the buffer for caching data during scans.
 *
 * The form and join host library uses a flat buffer to store channel energy,
 * pan ids, and matching networks.  The underlying data structure is an
 * uint16_t[], so the true storage size is twice this value.  The library requires
 * the buffer be at least 32 bytes, so the minimum size here is 16.  A matching
 * network requires 16 to 20 bytes, depending on struct padding.
 */
  #define EZSP_HOST_FORM_AND_JOIN_BUFFER_SIZE 40
#endif

/** @}  END addtogroup */
