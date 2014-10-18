# Base EFM32 CMake file
# This can be used as is as a project base, or by adding the efm32-base
# repository as a submodule in another project, copying this CMakeLists file
# to the top level directory, and updating the toolchain paths to reflect this
# change

cmake_minimum_required (VERSION 2.8.4)

# Toolchain and support files
include(${CMAKE_CURRENT_LIST_DIR}/toolchain/arm-gcc.cmake)

# Project Configuration
project(efm32 C CXX ASM)

# Set device and target configuration
set(DEVICE EFM32GG990F1024)
set(TARGET blink)

include(${PROJECT_SOURCE_DIR}/toolchain/efm32-base.cmake)

# Add project sources
set(SOURCES
	source/main.c
	)

# Generate executable and link
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${LIBS})

# Add post build commands
include(${PROJECT_SOURCE_DIR}/toolchain/post-build.cmake)

# Add JLink commands
include(${PROJECT_SOURCE_DIR}/toolchain/jlink.cmake)
