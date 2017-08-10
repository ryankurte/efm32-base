# JLink functions
# Adds targets for JLink programmers and emulators
# Copyright (c) 2016 Ryan Kurte
# This file is covered under the MIT license available at: https://opensource.org/licenses/MIT

# BlackMagic Debug Probe commands

# Set default serial port for bmpm
if(NOT DEFINED BMPM_PORT)
set(BMPM_PORT /dev/cu.usbmodemD5DED0F1)
endif()


# Configure bmpm scripts with device name
configure_file(${CMAKE_CURRENT_LIST_DIR}/bmpm-debug.in ${CMAKE_CURRENT_BINARY_DIR}/bmpm-debug.gdbconf)
configure_file(${CMAKE_CURRENT_LIST_DIR}/bmpm-flash.in ${CMAKE_CURRENT_BINARY_DIR}/bmpm-flash.gdbconf)
configure_file(${CMAKE_CURRENT_LIST_DIR}/bmpm-erase.in ${CMAKE_CURRENT_BINARY_DIR}/bmpm-erase.gdbconf)
configure_file(${CMAKE_CURRENT_LIST_DIR}/bmpm-reset.in ${CMAKE_CURRENT_BINARY_DIR}/bmpm-reset.gdbconf)

#Add BlackMagic commands
add_custom_target(debug 
	COMMAND ${DEBUGGER} -command=${CMAKE_CURRENT_BINARY_DIR}/bmpm-debug.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${TARGET} 
	DEPENDS ${TARGET}
	)

add_custom_target(flash 
	COMMAND ${DEBUGGER} -command=${CMAKE_CURRENT_BINARY_DIR}/bmpm-flash.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${TARGET} 
	DEPENDS ${TARGET}
	)

add_custom_target(erase 
	COMMAND ${DEBUGGER} -command=${CMAKE_CURRENT_BINARY_DIR}/bmpm-erase.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${TARGET} 
	DEPENDS ${TARGET}
	)

add_custom_target(reset 
	COMMAND ${DEBUGGER} -command=${CMAKE_CURRENT_BINARY_DIR}/bmpm-reset.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${TARGET} 
	DEPENDS ${TARGET}
	)
