/**
 * @file memory-buffer.h
 * @brief Ember Connect API dynamically allocate and free memory.
 * See @ref memory_buffer for documentation.
 *
 * <!--Copyright 2015 by Silicon Labs. All rights reserved.              *80*-->
 */

/**
 * @addtogroup memory_buffer
 *
 * See memory-buffer.h for source code.
 * @{
 */

/** @brief A special ::EmberBuffer ID indicating that no memory is currently
 * allocated.
 */
#define EMBER_NULL_BUFFER    NULL_BUFFER

/** @name Handlers*/
//@{

/** @brief This handler is invoked by the memory buffers system garbage
 * collector and allows the application to properly mark the application-defined
 * ::EmberBuffer variables.
 */
void emberMarkApplicationBuffersHandler(void);

//@}//END Handlers

/** @name APIs*/
//@{

/** @brief An API for dynamically allocating memory.
 *
 * @param dataSizeInBytes   The size in bytes of the memory to be allocated.
 *
 * @return An ::EmberBuffer value of ::EMBER_NULL_BUFFER if the memory
 * management system could not allocate the requested memory, or any other
 * ::EmberBuffer value indicating that the requested memory was successfully
 * allocated.
 * The allocated memory can easily be freed by assigning an ::EmberBuffer
 * variable to EMBER_NULL_BUFFER. The memory will be freed by the garbage
 * collector during the next ::emberTick() call.
 */
EmberBuffer emberAllocateBuffer(uint16_t dataSizeInBytes);

/** @brief This API prevents the garbage collector from reclaiming the memory
 * associated with the passed ::EmberBuffer. The application should call
 * this API within the ::emberMarkApplicationBuffersHandler() stack handler
 * for each ::EmberBuffer object.
 *
 * @param buffer   A pointer to the ::EmberBuffer buffer to be marked.
 */
void emberMarkBuffer(EmberBuffer *buffer);

/** @brief An API that returns a pointer to the memory segment corresponding to
 * the passed ::EmberBuffer buffer. Notice that the garbage collector can move
 * memory segments to defragment the available memory. As result, the
 * application should always use this API to obtain an updated pointer prior to
 * accessing the memory.
 *
 * @param buffer   A pointer to the ::EmberBuffer buffer for which the
 * corresponding memory pointer should be returned.
 *
 * @return  A NULL pointer if the passed ::EmberBuffer value is
 * ::EMBER_NULL_BUFFER. Otherwise, a pointer to the corresponding memory
 * segment.
 */
uint8_t *emberGetBufferPointer(EmberBuffer buffer);

/** @brief An API that returns the length in bytes of the passed ::EmberBuffer
 * buffer.
 *
 * @param buffer   A pointer to the ::EmberBuffer buffer for which the
 * corresponding length in bytes should be returned.
 *
 * @return  The length in bytes of memory segment corresponding to the passed
 * ::EmberBuffer buffer.
 */
uint16_t emberGetBufferLength(EmberBuffer buffer);

/** @brief An API that returns the available memory at the buffer manager in
 * bytes.
 *
 * @return  The number of available bytes.
 */
uint16_t emberGetAvailableBufferMemory(void);

//@}//END APIs

/** @} // END addtogroup
 */
