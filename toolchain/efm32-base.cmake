# EFM32 Base CMake file
#
# Configures the project files and environment for any EFM32 project
#
# Copyright (c) 2016 Ryan Kurte
# This file is covered under the MIT license available at: https://opensource.org/licenses/MIT

if(NOT DEFINED DEVICE)
message(FATAL_ERROR "No processor defined")
endif(NOT DEFINED DEVICE)
message("Device: ${DEVICE}")

# Convert to upper case
string(TOUPPER ${DEVICE} DEVICE_U)
message("Processor: ${DEVICE_U}")

# Determine device family
# Ideally this regex would be ^((EFM|EZR|EFR)32[A-Z]{1,2}([0-9]{1,2}[A-Z])?), but it appears to be
# too complex for CMake...
string(REGEX MATCH "^((EFM|EZR|EFR)32[A-Z]+([0-9]([0-9])?[A-Z])?)" CPU_FAMILY_U "${DEVICE_U}")
string(TOLOWER ${CPU_FAMILY_U} CPU_FAMILY_L)
message("Family: ${CPU_FAMILY_U}")

if(NOT DEFINED FLASH_START)
set(FLASH_START 0x00000000)
message("No FLASH_START defined. Using default: ${FLASH_START}")
endif(NOT DEFINED FLASH_START)

# Determine core type
# TODO: find a neater (array based) way of doing this
if(CPU_FAMILY_U STREQUAL "EFM32ZG" OR CPU_FAMILY_U STREQUAL "EFM32HG")
	message("Architecture: cortex-m0plus")
	set(CPU_TYPE "m0plus")
	set(CPU_FIX "")
elseif(CPU_FAMILY_U STREQUAL "EFM32WG" OR CPU_FAMILY_U STREQUAL "EZR32WG" 
		OR CPU_FAMILY_U STREQUAL "EFM32PG1B" OR CPU_FAMILY_U STREQUAL "EFR32FG1B"
		OR CPU_FAMILY_U STREQUAL "EFM32PG12B" OR CPU_FAMILY_U STREQUAL "EFR32FG12B"
		OR CPU_FAMILY_U STREQUAL "EFM32PG13B" OR CPU_FAMILY_U STREQUAL "EFR32FG13B")
	message("Architecture: cortex-m4")
	set(CPU_TYPE "m4")
set(CPU_FIX "")
else()
	message("Architecture: cortex-m3 (default)")
	set(CPU_TYPE "m3")
	set(CPU_FIX "-mfix-cortex-m3-ldrd")
endif()

# Include libraries
include(${CMAKE_CURRENT_LIST_DIR}/../device/device.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../cmsis/cmsis.cmake)
if(NOT DEFINED NO_EMLIB)
include(${CMAKE_CURRENT_LIST_DIR}/../emlib/emlib.cmake)
endif()

# Set compiler flags
# Common arguments
add_definitions("-D${DEVICE}")
set(COMMON_DEFINITIONS "-Wextra -Wall -Wno-unused-parameter -mcpu=cortex-${CPU_TYPE} -mthumb -fno-builtin -ffunction-sections -fdata-sections -fomit-frame-pointer ${OPTIONAL_DEBUG_SYMBOLS}")
set(DEPFLAGS "-MMD -MP")

# Enable FLTO optimization if required
if(USE_FLTO)
	set(OPTFLAGS "-Os -flto")
else()
	set(OPTFLAGS "-Os")
endif()

# Build flags
set(CMAKE_C_FLAGS "-std=gnu99 ${COMMON_DEFINITIONS} ${CPU_FIX} --specs=nano.specs ${DEPFLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_DEFINITIONS} ${CPU_FIX} --specs=nano.specs ${DEPFLAGS}")
set(CMAKE_ASM_FLAGS "${COMMON_DEFINITIONS} -x assembler-with-cpp -DLOOP_ADDR=0x8000")
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_DEFINITIONS} -Xlinker -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,-v")

# Set default inclusions
set(LIBS ${LIBS} -lgcc -lc -lnosys -lgcc -lc -lnosys)

# Debug Flags
set(COMMON_DEBUG_FLAGS "-O0 -g -gdwarf-2")
set(CMAKE_C_FLAGS_DEBUG   "${COMMON_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")
set(CMAKE_ASM_FLAGS_DEBUG "${COMMON_DEBUG_FLAGS}")

# Release Flags
set(COMMON_RELEASE_FLAGS "${OPTFLAGS} -DNDEBUG=1 -DRELEASE=1")
set(CMAKE_C_FLAGS_RELEASE 	"${COMMON_RELEASE_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_RELEASE_FLAGS}")
set(CMAKE_ASM_FLAGS_RELEASE "${COMMON_RELEASE_FLAGS}")


