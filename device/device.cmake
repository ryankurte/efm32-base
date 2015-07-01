# EFM32 Device Library CMake file
#
# Configures the required linker script, startup script, and system file and adds them to the build

# Add headers for the specific device
include_directories(${CMAKE_CURRENT_LIST_DIR}/${CPU_FAMILY_U}/Include)

# Configure linker script and set linker script name
set(LINKER_SCRIPT_MASTER ${CMAKE_CURRENT_LIST_DIR}/${CPU_FAMILY_U}/Source/GCC/${CPU_FAMILY_L}.ld)
configure_file(${LINKER_SCRIPT_MASTER} ${CMAKE_CURRENT_BINARY_DIR}/${CPU_FAMILY_L}.ld)
set(LINKER_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/${CPU_FAMILY_L}.ld)

# Set startup file name
set(STARTUP_FILE ${CMAKE_CURRENT_LIST_DIR}/${CPU_FAMILY_U}/Source/GCC/startup_${CPU_FAMILY_L}.S)

# Set system file name
set(SYSTEM_FILE ${CMAKE_CURRENT_LIST_DIR}/${CPU_FAMILY_U}/Source/system_${CPU_FAMILY_L}.c)

# Create device library
add_library(device ${STARTUP_FILE} ${SYSTEM_FILE})

# Add library to the build
set(LIBS ${LIBS} device)
