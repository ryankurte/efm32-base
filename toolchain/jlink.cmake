# JLink functions
# Adds targets for JLink programmers and emulators
# Copyright (c) 2016 Ryan Kurte
# This file is covered under the MIT license available at: https://opensource.org/licenses/MIT

# Configure flasher script for the project
set(BINARY ${PROJECT_NAME}.bin)
configure_file(${CMAKE_CURRENT_LIST_DIR}/flash.in ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink)

if(JLINK_SERVER_IP)
	set(JLINK_IP_ARG -ip ${JLINK_SERVER_IP})
endif()

if (NOT JLINK_DEVICE)
    set(JLINK_DEVICE ${DEVICE})
endif ()

#Add JLink commands
add_custom_target(debug
        COMMAND ${DEBUGGER} -tui -command ${CMAKE_CURRENT_LIST_DIR}/remote.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME}
	)

add_custom_target(debug-server 
	COMMAND JLinkGDBServer -device ${JLINK_DEVICE} -speed 4000 -if SWD
        DEPENDS ${PROJECT_NAME}
	)

add_custom_target(d
        COMMAND ${DEBUGGER} -command ${CMAKE_CURRENT_LIST_DIR}/remote.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
        DEPENDS ${PROJECT_NAME}
	)

add_custom_target(ds
	COMMAND JLinkGDBServer -device ${JLINK_DEVICE} -speed 4000 -if SWD
        DEPENDS ${PROJECT_NAME}
	)

add_custom_target(flash 
	COMMAND JLinkExe ${JLINK_IP_ARG} -device ${JLINK_DEVICE} -speed 4000 -if SWD -CommanderScript ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink
        DEPENDS ${PROJECT_NAME}
	)

add_custom_target(f 
	COMMAND JLinkExe ${JLINK_IP_ARG} -device ${JLINK_DEVICE} -speed 4000 -if SWD -CommanderScript ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink
        DEPENDS ${PROJECT_NAME}
	)

add_custom_target(erase 
	COMMAND JLinkExe ${JLINK_IP_ARG} -device ${JLINK_DEVICE} -speed 4000 -if SWD -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/erase.jlink 
	)

add_custom_target(e 
	COMMAND JLinkExe ${JLINK_IP_ARG} -device ${JLINK_DEVICE} -speed 4000 -if SWD -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/erase.jlink 
	)

add_custom_target(reset 
	COMMAND JLinkExe ${JLINK_IP_ARG} -device ${JLINK_DEVICE} -speed 4000 -if SWD -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/reset.jlink 
	)

add_custom_target(r 
	COMMAND JLinkExe ${JLINK_IP_ARG} -device ${JLINK_DEVICE} -speed 4000 -if SWD -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/reset.jlink 
	)
