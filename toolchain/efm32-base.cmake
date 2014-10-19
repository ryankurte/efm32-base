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
elseif(CPU_FAMILY_U STREQUAL "EFM32WG")
set(CPU_TYPE "m4")
else()
set(CPU_TYPE "m3")
endif()

# Include libraries
include(${CMAKE_CURRENT_LIST_DIR}/../device/device.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../cmsis/cmsis.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../emlib/emlib.cmake)

# Set compiler flags
add_definitions(-D${DEVICE})
set(CMAKE_C_FLAGS "-mcpu=cortex-${CPU_TYPE} -mthumb -Wall -Wextra -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99")
set(CMAKE_CXX_FLAGS "-mcpu=cortex-${CPU_TYPE} -mthumb -Wall -Wextra -mno-sched-prolog -fno-builtin -ffunction-sections -fdata-sections -std=c99")
SET(CMAKE_ASM_FLAGS "-x assembler-with-cpp")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT} -Xlinker --gc-sections -Xlinker -Map=${TARGET}.map --specs=nano.specs -Wl,--start-group -lgcc -lc -Wl,--end-group")

# Set default inclusions
set(LIBS ${LIBS} nosys)

# Debug Flags
set(CMAKE_C_FLAGS_DEBUG "-O0 -g -gdwarf-2 '-DDEBUG_EFM=1' '-DDEBUG=1'")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -gdwarf-2 '-DDEBUG_EFM=1' '-DDEBUG=1'")
set(CMAKE_ASM_FLAGS_DEBUG "-O0 -g -gdwarf-2 '-DDEBUG_EFM=1' '-DDEBUG=1'")

# Release Flags
set(CMAKE_C_FLAGS_RELEASE "-Os '-DNDEBUG=1'")
set(CMAKE_CXX_FLAGS_RELEASE "-Os '-DNDEBUG=1'")
set(CMAKE_ASM_FLAGS_RELEASE "-Os '-DNDEBUG=1'")

# Add post build commands
include(${CMAKE_CURRENT_LIST_DIR}/post-build.cmake)

# Add JLink commands
include(${CMAKE_CURRENT_LIST_DIR}/jlink.cmake)