# CMake helper for building rust libraries as a component of a C/C++ project
# Copyright 2018 Ryan Kurte

# USAGE
# 1. Add the cargo folder (must match library name) to your top level CMakeLists.txt
#     set(RUST_SOURCE rs)
# 2. Include rust.cmake
#     include(rust.cmake)
# 3. Add rust-lib as a dependency for your target
#     add_dependencies(${CMAKE_PROJECT_NAME} rust-lib)
# 4. Link rust libraries to your target
#     target_link_libraries(${CMAKE_PROJECT_NAME} ${RUST_LIBS})

# Locate cargo instance
find_program(CARGO cargo)

message(STATUS "[RUST] Building Rust project ${RUST_SOURCE}")

# Rust project base directory
set(RUST_DIR "${PROJECT_SOURCE_DIR}/${RUST_SOURCE}") 

# Set cargo output dir to cmake binary dir (simplifies cleaning etc.)
set(ARGS --target-dir ${CMAKE_BINARY_DIR})

# Interpret MCPU if set
if(MCPU STREQUAL "cortex-m3")
	message(STATUS "[RUST] Detected MCPU=cortex-m3")
	set(RUST_TARGET thumbv7m-none-eabi)
elseif(MCPU STREQUAL "cortex-m4")
	message(STATUS "[RUST] Detected MCPU=cortex-m4")
	set(RUST_TARGET thumbv7em-none-eabi)
endif()

# Override cargo target if specified
if(RUST_TARGET)
	message(STATUS "[RUST] Overriding target with ${RUST_TARGET}")
	# Non-native target requires --target specifier
	set(ARGS ${ARGS} --target=${RUST_TARGET})
	# Non-native target goes into target specific dir
	set(RUST_TARGETDIR ${CMAKE_BINARY_DIR}/${RUST_TARGET})
else()
	message(STATUS "[RUST] Using native target")
	# Native target builds into /target
	set(RUST_TARGETDIR ${CMAKE_BINARY_DIR})
	# Native target requires pthread and dl
	set(RUST_LIBS pthread dl)
endif()

# Add release or debug args
if (CMAKE_BUILD_TYPE STREQUAL "Release")
	message(STATUS "[RUST] Detected release build")
	set(ARGS ${ARGS} --release)
	set(RUST_OUTDIR ${RUST_TARGETDIR}/release/)
else()
	message(STATUS "[RUST] Using default debug build")
	set(RUST_OUTDIR ${RUST_TARGETDIR}/debug/)
endif()

# Add a target to test the rust library
add_custom_target(rust-test 
	COMMAND ${CARGO} test ${ARGS}
	WORKING_DIRECTORY ${RUST_DIR}
	DEPENDS ${RUST_DIR}/*)

# Add a target to build the rust library
add_custom_target(rust-lib 
	COMMAND ${CARGO} build ${ARGS}
	WORKING_DIRECTORY ${RUST_DIR}
	DEPENDS ${RUST_DIR}/*)

# Add a target to clean the rust library
add_custom_target(rust-clean 
	COMMAND ${CARGO} clean ${ARGS}
	WORKING_DIRECTORY ${RUST_DIR}
)

# Include the C header output dir
include_directories(${CMAKE_BINARY_DIR}/c-header-C99)

# Define a RUST_LIBS variable for linking
set(RUST_LIBS ${RUST_LIBS} ${RUST_OUTDIR}/lib${RUST_SOURCE}.a)

