# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initalize pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
set(PICO_SDK_PATH "/Users/tuupola/Code/c/pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)

project(pico_effects C CXX ASM)

# Initialise the Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(pico_effects pico_effects.c )

pico_set_program_name(pico_effects "pico_effects")
pico_set_program_version(pico_effects "0.1")

pico_enable_stdio_uart(pico_effects 1)
pico_enable_stdio_usb(pico_effects 1)

# Add the standard library to the build
target_link_libraries(pico_effects pico_stdlib)

# Add any user requested libraries
target_link_libraries(pico_effects
        hardware_spi
        hardware_dma
        )

pico_add_extra_outputs(pico_effects)

