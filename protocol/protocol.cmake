string(TOLOWER ${DEVICE} DEVICE_L)
message("DEVICE_L = ${DEVICE_L}")

set(BLE_LINKER_SCRIPT "${CMAKE_CURRENT_LIST_DIR}/bluetooth/ble_stack/linker/GCC/${DEVICE_L}.ld")
if (NOT EXISTS ${BLE_LINKER_SCRIPT})
    return()
endif ()

message("Found bluetooth specific linker script, using this instead: ${BLE_LINKER_SCRIPT}")
set(LINKER_SCRIPT ${BLE_LINKER_SCRIPT})

include_directories(
        protocol/bluetooth/ble_stack/inc/common
        protocol/bluetooth/ble_stack/inc/soc)
