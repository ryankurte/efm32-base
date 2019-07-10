if (NOT DEFINED BOARD)
    message("no board defined, skipping hardware")
    return()
endif ()

include_directories(
        hardware/kit/${CPU_FAMILY_U}_${BOARD}/config
        hardware/kit/common/bsp
        hardware/kit/common/drivers
        hardware/kit/common/halconfig
        hardware/module/config)