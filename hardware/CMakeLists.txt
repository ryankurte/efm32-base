project(hardware)

if (NOT DEFINED BOARD)
    message("No board defined, skipping hardware")
    set(HAS_HARDWARE FALSE PARENT_SCOPE)
    return()
endif ()

set(HAS_HARDWARE TRUE PARENT_SCOPE)

message("Board defined, adding hardware library for: ${CPU_FAMILY_U}_${BOARD}")

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE
        halconfig/inc/hal-config
        kit/${CPU_FAMILY_U}_${BOARD}/config
        kit/common/bsp
        kit/common/drivers
        kit/common/halconfig
        module/config)
