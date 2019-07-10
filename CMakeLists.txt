# Base EFM32 CMake file
#
# This can be used as is as a project base, or by adding the efm32-base
# repository as a submodule to another project, copying this CMakeLists file
# to the top level directory, and updating the BASE_LOCATION variable to reflect this
# change
#
## Copyright (c) 2016 Ryan Kurte
# This file is covered under the MIT license available at: https://opensource.org/licenses/MIT

###### Project Environment #####

# Set minimum CMake version
cmake_minimum_required(VERSION 2.8.4)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Set efm32-base project location
set(BASE_LOCATION .)

# Set the compiler (must be prior to project setup)
set(COMPILER_PREFIX "/home/andrew/opt/SimplicityStudio_v4/developer/toolchains/gnu_arm/7.2_2017q4/bin/")
include(${BASE_LOCATION}/toolchain/arm-gcc.cmake)

##### Project Setup #####

# Set our output target
set(TARGET efm32-test)

# Configure project and languages
project(${TARGET} C CXX ASM)

# Set device
if(NOT DEVICE)
	set(DEVICE BGM13P22F512GA)
endif()

set(BOARD BRD4306A)

# Set build
if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE DEBUG)
endif()

##### Modules #####

# Libraries can be added to the LIBS variable
# or manually included here.

# Add base libs (emlib, CMSIS, device files)
include(${BASE_LOCATION}/toolchain/efm32-base.cmake)

##### Files #####

# Add project headers
include_directories(${BASE_LOCATION}/include)

# Add project sources
set(SOURCES
	source/main.c
)

##### Outputs #####

# Generate executable and link
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${LIBS})

##### Post build #####

# Add post build commands
include(${BASE_LOCATION}/toolchain/post-build.cmake)

# Add JLink commands
include(${BASE_LOCATION}/toolchain/jlink.cmake)

message(CMAKE_SOURCE_DIR =${CMAKE_SOURCE_DIR})
message(CMAKE_C_COMPILER =${CMAKE_C_COMPILER})
message(CMAKE_C_FLAGS =${CMAKE_C_FLAGS})
message(CMAKE_C_LINK_EXECUTABLE =${CMAKE_C_LINK_EXECUTABLE})
message(CMAKE_EXE_LINKER_FLAGS =${CMAKE_EXE_LINKER_FLAGS})
message(CMAKE_AR =${CMAKE_AR})

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message("Includes: ")
foreach(dir ${dirs})
	message(STATUS "dir='${dir}'")
endforeach()
