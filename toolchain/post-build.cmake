# ARM post build commands

# Create binary file
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${OBJCOPY} -O binary ${TARGET} ${TARGET}.bin)

add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${OBJCOPY} -O ihex ${TARGET} ${TARGET}.hex)

add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${OBJDUMP} -d -S ${TARGET} > ${TARGET}.dmp)