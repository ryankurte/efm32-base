# ARM post build commands
# Copyright (c) 2016 Ryan Kurte
# This file is covered under the MIT license available at: https://opensource.org/licenses/MIT

# Create binary file
add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${OBJCOPY} -O binary ${TARGET} ${TARGET}.bin)

add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${OBJCOPY} -O ihex ${TARGET} ${TARGET}.hex)

add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${OBJDUMP} -d -S ${TARGET} > ${TARGET}.dmp)

add_custom_command(TARGET ${TARGET} POST_BUILD COMMAND ${OBJSIZE} ${TARGET})
