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

add_executable(pico_effects pico_effects.c metaballs.c plasma.c rotozoom.c)

pico_set_program_name(pico_effects "pico_effects")
pico_set_program_version(pico_effects "0.1")

# Enable usb output, disable uart output
pico_enable_stdio_usb(pico_effects 1)
pico_enable_stdio_uart(pico_effects 0)

# Add the standard library to the build
target_link_libraries(pico_effects pico_stdlib)

# Add any user requested libraries
target_link_libraries(pico_effects
        hardware_spi
        hardware_dma
        hagl
        hagl_hal
        )

pico_add_extra_outputs(pico_effects)

add_subdirectory(lib/hagl)
add_subdirectory(lib/hagl_hal)

target_compile_definitions(pico_effects PRIVATE
  NO_MENUCONFIG=1
  HAGL_HAL_USE_DOUBLE_BUFFER=1
)

