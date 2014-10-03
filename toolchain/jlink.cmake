

# Configure flasher script for the project
set(BINARY ${TARGET}.bin)
configure_file(${CMAKE_CURRENT_LIST_DIR}/flash.in ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink)

#Add JLink commands
add_custom_target(flash 
	COMMAND JLinkExe -device ${DEVICE} -speed 4000 -CommanderScript ${CMAKE_CURRENT_BINARY_DIR}/flash.jlink 
	DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.bin
	)

#Add flash command
add_custom_target(erase 
	COMMAND JLinkExe -device ${DEVICE} -speed 4000 -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/erase.jlink 
	)

#Add flash command
add_custom_target(reset 
	COMMAND JLinkExe -device ${DEVICE} -speed 4000 -CommanderScript ${CMAKE_CURRENT_LIST_DIR}/reset.jlink 
	)
