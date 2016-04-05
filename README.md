# EFM32 Base Project

This base project is designed to provide a quick and platform independent method of building projects for Silicon Labs EFM32 microcontrollers.  
Designed and tested on OSX and Debian. Windows might work with cygwig/mingw/gnuwindows tools, but hasn't been attempted.

[![Build Status](https://travis-ci.org/ryankurte/efm32-base.svg)](https://travis-ci.org/ryankurte/efm32-base)

## Motivation
 - Getting started with Microcontrollers is hard (and time consuming)
 - Vendor IDEs are ultimately useless when approaching testing and build-automation

This project addresses this by providing a common base for for projects using Silicon Labs EFM32 and EZR32 processors.   
This will likely be superseeded by ARM-Mbed Yotta going forwards, check out http://yottadocs.mbed.com/ for more information.

## Dependencies
 - cmake - use brew on osx, or your favourite package manager for linux
 - make - should be available by default, otherwise as above
 - arm-none-eabi-gcc - Embedded ARM Compiler/Toolchain (https://launchpad.net/gcc-arm-embedded/+download)
 - JLink tools - download and install from https://www.segger.com/jlink-software.html

If you're into local tools, check out https://git.io/vRP63 for a script to load and couple a compiler version into your project.

## Usage

This project can either be used directly or as a submodule in a larger project.
Note that submodule use will allow updates to this project for fixes or further device support.  
For an example project using this method, see [ryankurte/usb-thing](https://github.com/ryankurte/usb-thing).

### To use the project directly:

1. Download this repository
2. Change the project name and device in the CMakeLists.txt file
3. Move your source  and include files into the source and include directories
4. Add your source files to the CMakeLists.txt file
5. Make something awesome!

### To use this project as a submodule:

1. Add the submodule to your (already git controlled) project using:  
   `git submodule add https://github.com/ryankurte/efm32-base.git`  
   `git submodule update`  
2. Copy the CMakeLists.txt file from this project (efm32-base) to the top level of your project
3. Update the project name and BASE_LOCATION variables in the new CMakeLists.txt
4. Add your source files (and cmake libraries) to the CMakeLists.txt file
5. Make something even more awesome!

## Building
Once you have integrated this project with your project, you can build in the standard cmake manner.

1. `mkdir build` to create the build directory
2. `cd build` to switch to the build directory
3. `cmake ..` to configure the build
4. `make` to execute the build
5. `make flash` to flash to a device

If you have any issues, suggestions or alterations, feel free to open an issue or a pull request.
