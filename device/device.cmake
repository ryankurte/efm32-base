#EFM32 Device Library Cmake file
#Adds the required linker and other files to the build

set(DEVICE_SOURCES "")

#Add device specific includes
include_directories(${CMAKE_CURRENT_LIST_DIR}/${CPU_FAMILY_L}/include)

#Set linker script name
set(LINKER_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/${CPU_FAMILY_L}/${CPU_FAMILY_L}.ld)

#Set startup file name
set(STARTUP_FILE ${CMAKE_CURRENT_LIST_DIR}/${CPU_FAMILY_L}/startup_${CPU_FAMILY_L}.S)

#Set system file name
set(SYSTEM_FILE ${CMAKE_CURRENT_LIST_DIR}/${CPU_FAMILY_L}/system_${CPU_FAMILY_L}.c)

#Create device library
add_library(device ${STARTUP_FILE} ${SYSTEM_FILE})

set(LIBS ${LIBS} device)