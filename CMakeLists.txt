# Base EFM32 CMake file
#
# This can be used as is as a project base, or by adding the efm32-base
# repository as a submodule in another project, copying this CMakeLists file
# to the top level directory, and updating the toolchain paths to reflect this
# change

###### Project Environment #####

# Set minimum CMake version
cmake_minimum_required(VERSION 2.8.4)

# Check which directory we are running in so we can find our toolchains etc
get_filename_component(BASE_DIRECTORY ${CMAKE_CURRENT_LIST_DIR} NAME)
if(NOT ${BASE_DIRECTORY} STREQUAL "efm32-base")
	set(PROJECT_PREFIX "efm32-base")
endif()
message("Project prefix: " ${PROJECT_PREFIX})

# Set the toolchain/languages/support files etc
include(${CMAKE_CURRENT_LIST_DIR}/${PROJECT_PREFIX}/toolchain/arm-gcc.cmake)

##### Project Setup #####

# Set our output target
set(TARGET efm32-test)

# Configure project and languages
project(${TARGET} C CXX ASM)

# Set device
set(DEVICE EFM32GG990F1024)

# Set build
set(CMAKE_BUILD_TYPE DEBUG)

##### Modules #####

# Libraries should add themselves to the LIBS variable

# Add base libs (emlib, CMSIS, device files)
include(${PROJECT_SOURCE_DIR}/${PROJECT_PREFIX}/toolchain/efm32-base.cmake)

##### Files #####

# Add project headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Add project sources
set(SOURCES
	source/main.c
)

##### Outputs #####

# Generate executable and link
add_executable(${TARGET} ${SOURCES})
target_link_libraries(${TARGET} ${LIBS})