
enable_language(ASM)

if(NOT DEFINED DEVICE)
message(FATAL_ERROR "No processor defined")
endif(NOT DEFINED DEVICE)

# Convert to upper case
string(TOUPPER ${DEVICE} DEVICE_U)
message("Processor: ${DEVICE_U}")

# Determine device family
string(REGEX MATCH "^(EFM32[A-Z]+)" CPU_FAMILY_U "${DEVICE_U}")
string(TOLOWER ${CPU_FAMILY_U} CPU_FAMILY_L)
message("Family: ${CPU_FAMILY_U}")

# Determine core type
if(CPU_FAMILY_U STREQUAL "EFM32ZG")
set(CPU_TYPE "m0-plus")
elseif(CPU_FAMILY_U STREQUAL "EFM32WG")
set(CPU_TYPE "m4")
else()
set(CPU_TYPE "m3")
endif()

# Include libraries
include(${CMAKE_CURRENT_LIST_DIR}/../device/device.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../emlib/emlib.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../cmsis/cmsis.cmake)

# Set compiler flags
add_definitions("-std=c99 -D${DEVICE} --specs=nano.specs -Wall -mcpu=cortex-${CPU_TYPE} -mthumb -fno-builtin -ffunction-sections -fdata-sections")
set(CMAKE_EXE_LINKER_FLAGS "-Xlinker --gc-sections -Xlinker -Map=${TARGET}.map -T ${LINKER_SCRIPT}")

set(LIBS ${LIBS} gcc c nosys)

set(CMAKE_BUILD_TYPE DEBUG)

# Debug Flags
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -gdwarf-2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -gdwarf-2")
set(CMAKE_ASM_FLAGS_DEBUG "-O0 -g -gdwarf-2")

#Release Flags
set(CMAKE_C_FLAGS_RELEASE "-Os")
set(CMAKE_CXX_FLAGS_RELEASE "-Os")
set(CMAKE_ASM_FLAGS_RELEASE "-Os")

