# EFM32 Base CMake file
#
# Configures the project files and environment for any EFM32 project

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
set(CPU_FIX "")
elseif(CPU_FAMILY_U STREQUAL "EFM32WG")
set(CPU_TYPE "m4")
set(CPU_FIX "")
else()
set(CPU_TYPE "m3")
set(CPU_FIX "-mfix-cortex-m3-ldrd")
endif()

# Include libraries
include(${CMAKE_CURRENT_LIST_DIR}/../device/device.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../cmsis/cmsis.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../emlib/emlib.cmake)

# Set compiler flags
set(COMMON_FLAGS "-D${DEVICE} -mcpu=cortex-${CPU_TYPE} -mthumb -Wall -Wextra -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -fomit-frame-pointer")
set(DEPENDENCY_FLAGS "-MMD -MP")

set(CMAKE_C_FLAGS "${COMMON_FLAGS} ${CPU_FIX} -std=gnu99 --specs=nano.specs  ${DEPENDENCY_FLAGS}")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} ${CPU_FIX} --specs=nano.specs  ${DEPENDENCY_FLAGS}")
SET(CMAKE_ASM_FLAGS "${COMMON_FLAGS} -x assembler-with-cpp -DLOOP_ADDR=0x8000")
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_FLAGS} -Xlinker -T${LINKER_SCRIPT} -Wl,--gc-sections -Wl,-Map=${TARGET}.map")

# Set default inclusions
set(LIBS ${LIBS})

# Debug Flags
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -gdwarf-2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -gdwarf-2")
set(CMAKE_ASM_FLAGS_DEBUG "-O0 -g -gdwarf-2")

# Release Flags
set(CMAKE_C_FLAGS_RELEASE "-Os '-DNDEBUG=1'")
set(CMAKE_CXX_FLAGS_RELEASE "-Os '-DNDEBUG=1'")
set(CMAKE_ASM_FLAGS_RELEASE "-Os '-DNDEBUG=1'")