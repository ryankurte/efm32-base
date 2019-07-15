# EFM32 Device Library CMake file
#
# Configures the required linker script, startup script, and system file and adds them to the build
project(device)

message("Adding device library for: ${CPU_FAMILY_U}")

set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/${CPU_FAMILY_U}/Source/GCC/${CPU_FAMILY_L}.ld PARENT_SCOPE)

add_library(${PROJECT_NAME}
        ${CPU_FAMILY_U}/Source/GCC/startup_${CPU_FAMILY_L}.S
        ${CPU_FAMILY_U}/Source/system_${CPU_FAMILY_L}.c)

target_include_directories(${PROJECT_NAME} PUBLIC ${CPU_FAMILY_U}/Include)

target_link_libraries(${PROJECT_NAME} cmsis)
