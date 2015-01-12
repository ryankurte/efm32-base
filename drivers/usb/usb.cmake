# EFM32 EM_USB CMake file
#
# Configures the EM_USB and adds it to the build

##### Files #####

# Headers
include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)

# Source files
FILE(GLOB EFM_USB_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/*.c)

MESSAGE("WARNING: emusb module sources must be directly included")

##### Outputs #####

# Create emlib library
add_library(emusb ${EFM_USB_SOURCES})

# Add library to build
set(LIBS ${LIBS} emusb)
