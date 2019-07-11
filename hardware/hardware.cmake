if (NOT DEFINED BOARD)
    message("no board defined, skipping hardware")
    return()
endif ()

include_directories(
        ${CMAKE_CURRENT_LIST_DIR}/../halconfig/inc/hal-config
        hardware/kit/${CPU_FAMILY_U}_${BOARD}/config
        hardware/kit/common/bsp
        hardware/kit/common/drivers
        hardware/kit/common/halconfig
        hardware/module/config)