
#Include emlib headers
include_directories("${CMAKE_CURRENT_LIST_DIR}/inc")

#Collect emlib source files
FILE(GLOB EMLIB_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/*.c")

#Add library to build
add_library(emlib ${EMLIB_SOURCES})

#Add to library list for convenience
set(LIBS ${LIBS} emlib)
