#CMSIS cmake inclusion file
#Adds CMSIS headers to the build
project(cmsis)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME} INTERFACE Include)
