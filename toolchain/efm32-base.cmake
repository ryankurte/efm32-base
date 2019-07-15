# EFM32 Base CMake file
#
# Configures the project files and environment for any EFM32 project
#
# Copyright (c) 2016 Ryan Kurte
# This file is covered under the MIT license available at: https://opensource.org/licenses/MIT

if (NOT DEFINED DEVICE)
    message(FATAL_ERROR "No processor defined")
endif ()
message("Device: ${DEVICE}")

# Convert to upper case
string(TOUPPER ${DEVICE} DEVICE_U)
message("Processor: ${DEVICE_U}")

# Determine device family by searching for an appropriate device directory
set(DEVICE_FOUND FALSE)
set(TEMP_DEVICE "${DEVICE_U}")

while (NOT DEVICE_FOUND)
    if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/../device/${TEMP_DEVICE}")
        set(DEVICE_FOUND TRUE)
    else ()
        string(LENGTH ${TEMP_DEVICE} TEMP_DEVICE_LEN)
        math(EXPR TEMP_DEVICE_LEN "${TEMP_DEVICE_LEN}-1")
        string(SUBSTRING ${TEMP_DEVICE} 0 ${TEMP_DEVICE_LEN} TEMP_DEVICE)
    endif ()

    if (${TEMP_DEVICE_LEN} EQUAL "0")
        break()
    endif ()
endwhile ()

# Quit generating if we couldn't find a device
if (NOT DEVICE_FOUND)
    message(FATAL_ERROR "failed to find device")
endif ()

set(CPU_FAMILY_U ${TEMP_DEVICE})
string(TOLOWER ${CPU_FAMILY_U} CPU_FAMILY_L)
message("Family: ${CPU_FAMILY_U}")

if (NOT DEFINED FLASH_START)
    set(FLASH_START 0x00000000)
    message("No FLASH_START defined. Using default: ${FLASH_START}")
endif (NOT DEFINED FLASH_START)

# Determine core type
# TODO: find a neater (array based) way of doing this
if (CPU_FAMILY_U STREQUAL "EFM32ZG" OR CPU_FAMILY_U STREQUAL "EFM32HG")
    message("Architecture: cortex-m0plus")
    set(CPU_TYPE "m0plus")
    set(CPU_FIX "")
elseif (CPU_FAMILY_U STREQUAL "EFM32WG" OR CPU_FAMILY_U STREQUAL "EZR32WG"
        OR CPU_FAMILY_U STREQUAL "EFM32PG1B" OR CPU_FAMILY_U STREQUAL "EFR32FG1B"
        OR CPU_FAMILY_U STREQUAL "EFM32PG12B" OR CPU_FAMILY_U STREQUAL "EFR32FG12B"
        OR CPU_FAMILY_U STREQUAL "EFM32PG13B" OR CPU_FAMILY_U STREQUAL "EFR32FG13B"
        OR CPU_FAMILY_U STREQUAL "EFR32MG12P" OR CPU_FAMILY_U STREQUAL "EFR32MG13P"
        OR CPU_FAMILY_U STREQUAL "BGM13")
    message("Architecture: cortex-m4")
    set(CPU_TYPE "m4")
    set(CPU_FIX "")
else ()
    message("Architecture: cortex-m3 (default)")
    set(CPU_TYPE "m3")
    set(CPU_FIX "-mfix-cortex-m3-ldrd")
endif ()

# Set compiler flags
# Common arguments
add_definitions("-D${DEVICE}")
set(COMMON_DEFINITIONS "-Wextra -Wall -Wno-unused-parameter -mcpu=cortex-${CPU_TYPE} -mthumb -fno-builtin -ffunction-sections -fdata-sections -fomit-frame-pointer ${OPTIONAL_DEBUG_SYMBOLS}")
set(DEPFLAGS "-MMD -MP")

# Enable FLTO optimization if required
if (USE_FLTO)
    set(OPTFLAGS "-Os -flto")
else ()
    set(OPTFLAGS "-Os")
endif ()

# Build flags
set(CMAKE_C_FLAGS "-std=gnu99 ${COMMON_DEFINITIONS} ${CPU_FIX} --specs=nano.specs ${DEPFLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_DEFINITIONS} ${CPU_FIX} --specs=nano.specs ${DEPFLAGS}")
set(CMAKE_ASM_FLAGS "${COMMON_DEFINITIONS} -x assembler-with-cpp -DLOOP_ADDR=0x8000")

# Set default inclusions
set(LIBS ${LIBS} -lgcc -lc -lnosys -lgcc -lc -lnosys)

# Debug Flags
set(COMMON_DEBUG_FLAGS "-O0 -g -gdwarf-2")
set(CMAKE_C_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")
set(CMAKE_ASM_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")

# Release Flags
set(COMMON_RELEASE_FLAGS "${OPTFLAGS} -DNDEBUG=1 -DRELEASE=1")
set(CMAKE_C_FLAGS_RELEASE "${COMMON_RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_RELEASE_FLAGS}")
set(CMAKE_ASM_FLAGS_RELEASE "${COMMON_RELEASE_FLAGS}")

# print debug info helper function
function(print_debug_info)
    message("COMPILER_PREFIX =${COMPILER_PREFIX}")
    message("CMAKE_SOURCE_DIR =${CMAKE_SOURCE_DIR}")
    message("CMAKE_C_COMPILER =${CMAKE_C_COMPILER}")
    message("CMAKE_C_FLAGS =${CMAKE_C_FLAGS}")
    message("CMAKE_C_LINK_EXECUTABLE =${CMAKE_C_LINK_EXECUTABLE}")
    message("CMAKE_EXE_LINKER_FLAGS =${CMAKE_EXE_LINKER_FLAGS}")
    message("CMAKE_AR =${CMAKE_AR}")

    message("Definitions: ")
    get_directory_property(defs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS)
    foreach (def ${defs})
        message(STATUS "-D${def}")
    endforeach ()

    get_property(dirs TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)
    message("Includes: ")
    foreach (dir ${dirs})
        message(STATUS "${dir}")
    endforeach ()

    get_property(libs TARGET ${PROJECT_NAME} PROPERTY LINK_LIBRARIES)
    message("Libraries:")
    foreach (libs ${libs})
        message(STATUS "${libs}")
    endforeach ()
endfunction()
