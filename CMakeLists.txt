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

# Set the compiler (must be prior to project setup)
include(${CMAKE_CURRENT_LIST_DIR}/toolchain/arm-gcc.cmake)

##### Project Setup #####

# Configure project and languages
project(efm32-test C CXX ASM)

# Set device
if (NOT DEVICE)
    #    set(DEVICE EFM32G210F128)
    set(DEVICE BGM13P22F512GA)
    set(BOARD BRD4306A)
    set(BLE_LIB EFR32BG13P)
endif ()

# Set build
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE DEBUG)
endif ()

##### Modules #####

# Libraries can be added to the LIBS variable
# or manually included here.

# Add base libs (emlib, CMSIS, device files)
include(${PROJECT_SOURCE_DIR}/toolchain/efm32-base.cmake)

add_subdirectory(cmsis)
add_subdirectory(emlib)
add_subdirectory(device)
add_subdirectory(protocol)
add_subdirectory(hardware)

# this is normally set in efm32-base.cmake, but libraries may modify it so set
# it after libraries/subdirectories have been added
set(CMAKE_EXE_LINKER_FLAGS "${COMMON_DEFINITIONS} -Xlinker -T${LINKER_SCRIPT} -Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_PROJECT_NAME}.map -Wl,--gc-sections -Wl,-v")

##### Files #####

# Add project headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Generate executable and link
add_executable(${PROJECT_NAME}
        source/main.c)

target_link_libraries(${PROJECT_NAME} ${LIBS} emlib cmsis device)

# link optional libraries if available
if (${HAS_HARDWARE})
    target_link_libraries(${PROJECT_NAME} hardware)
endif ()

if (${HAS_PROTOCOL})
    target_link_libraries(${PROJECT_NAME} protocol)
endif ()

##### Post build #####

# Add post build commands
include(${PROJECT_SOURCE_DIR}/toolchain/post-build.cmake)

# Add JLink commands
include(${PROJECT_SOURCE_DIR}/toolchain/jlink.cmake)

##### CMake debug prints #####
message("COMPILER_PREFIX =${COMPILER_PREFIX}")
message("CMAKE_SOURCE_DIR =${CMAKE_SOURCE_DIR}")
message("CMAKE_C_COMPILER =${CMAKE_C_COMPILER}")
message("CMAKE_C_FLAGS =${CMAKE_C_FLAGS}")
message("CMAKE_C_LINK_EXECUTABLE =${CMAKE_C_LINK_EXECUTABLE}")
message("CMAKE_EXE_LINKER_FLAGS =${CMAKE_EXE_LINKER_FLAGS}")
message("CMAKE_AR =${CMAKE_AR}")

message("Definitions: ")
get_directory_property(defs DIRECTORY ${CMAKE_SOURCE_DIR} COMPILE_DEFINITIONS)
foreach (def ${defs})
    message(STATUS "-D${def}")
endforeach ()

get_property(dirs TARGET ${PROJECT_NAME} PROPERTY INCLUDE_DIRECTORIES)
message("Includes: ")
foreach (dir ${dirs})
    message(STATUS "${dir}")
endforeach ()

get_property(libs TARGET ${PROJECT_NAME} PROPERTY LINK_LIBRARIES)
message("Libraries:")
foreach (libs ${libs})
    message(STATUS "${libs}")
endforeach ()