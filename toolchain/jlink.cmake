# JLink functions
# Adds targets for JLink programmers and emulators

# Configure flasher script for the project
set(BINARY ${TARGET}.bin)
configure_file(${CMAKE_CURRENT_LIST_DIR}/flash.in ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink)

#Add JLink commands
add_custom_target(debug 
	COMMAND ${DEBUGGER} -tui -command ${CMAKE_CURRENT_LIST_DIR}/remote.gdbconf ${CMAKE_CURRENT_BINARY_DIR}/${TARGET} 
	DEPENDS ${TARGET}
	)

add_custom_target(debug-server 
	COMMAND JLinkGDBServer -device ${DEVICE} -speed 4000 -if SWD
	DEPENDS ${TARGET}
	)

add_custom_target(flash 
	COMMAND JLinkExe -device ${DEVICE} -speed 4000 -if SWD -CommanderScript ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink 
	DEPENDS ${TARGET}
	)

add_custom_target(erase 
	COMMAND JLinkExe -device ${DEVICE} -speed 4000 -if SWD -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/erase.jlink 
	)

add_custom_target(reset 
	COMMAND JLinkExe -device ${DEVICE} -speed 4000 -if SWD -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/reset.jlink 
	)
