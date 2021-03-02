cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_SDK_PATH "../../pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)
project(firmware C CXX ASM)

pico_sdk_init()

add_executable(firmware main.c metaballs.c plasma.c rotozoom.c clock.c)

pico_set_program_name(firmware "Pico Effects")
pico_set_program_version(firmware "0.1")

# Enable usb output, disable uart output
pico_enable_stdio_usb(firmware 1)
pico_enable_stdio_uart(firmware 0)

# Add the standard library to the build
target_link_libraries(firmware pico_stdlib)

# Add any user requested libraries
target_link_libraries(firmware
        hardware_spi
        hardware_dma
        hagl
        hagl_hal
        )

pico_add_extra_outputs(firmware)

add_subdirectory(external/hagl)
add_subdirectory(external/hagl_hal)

target_compile_definitions(firmware PRIVATE
  NO_MENUCONFIG
  HAGL_HAL_USE_DOUBLE_BUFFER
  #HAGL_HAL_USE_DMA
  HAGL_HAL_DEBUG
)

