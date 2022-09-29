cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Default path to the SDK
if (NOT PICO_SDK_PATH)
  set(PICO_SDK_PATH "../pico-sdk")
endif ()

# Pull in Pico SDK (must be before project)
include(${PICO_SDK_PATH}/pico_sdk_init.cmake)
project(firmware C CXX ASM)

pico_sdk_init()

add_executable(firmware main.c metaballs.c plasma.c rotozoom.c deform.c)

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

#
# If you have a large screen and run out of memory use
# the HAGL_HAL_PIXEL_SIZE=2 setting.
#
target_compile_definitions(firmware PRIVATE
#  HAGL_HAL_USE_SINGLE_BUFFER
  HAGL_HAL_USE_DOUBLE_BUFFER
#  HAGL_HAL_USE_TRIPLE_BUFFER
#  HAGL_HAL_USE_DMA
  HAGL_HAL_DEBUG
#  HAGL_HAL_PIXEL_SIZE=2
)

#
# There are ready made config files for popular Pico displays.
#

include("external/hagl_hal/cmake/pimoroni-pico-display-pack.cmake")
#include("external/hagl_hal/cmake/pimoroni-picosystem.cmake")
#include("external/hagl_hal/cmake/waveshare-pico-lcd-096.cmake")
#include("external/hagl_hal/cmake/waveshare-pico-lcd-130.cmake")
#include("external/hagl_hal/cmake/waveshare-pico-lcd-144.cmake")
#include("external/hagl_hal/cmake/waveshare-rp2040-lcd-096.cmake")
#include("external/hagl_hal/cmake/generic-ili9163-128x128.cmake")
#include("external/hagl_hal/cmake/generic-ili9341-320x240.cmake")
#include("external/hagl_hal/cmake/generic-st7735-128x128.cmake")

#
# For other boards you can set the config manually here.
#

# target_compile_definitions(firmware PRIVATE
#     MIPI_DISPLAY_PIN_CS=9
#     MIPI_DISPLAY_PIN_DC=8
#     MIPI_DISPLAY_PIN_RST=12
#     MIPI_DISPLAY_PIN_BL=13
#     MIPI_DISPLAY_PIN_CLK=10
#     MIPI_DISPLAY_PIN_MOSI=11
#     MIPI_DISPLAY_PIN_MISO=-1

#     MIPI_DISPLAY_SPI_PORT=spi1
#     MIPI_DISPLAY_SPI_CLOCK_SPEED_HZ=68000000

#     MIPI_DISPLAY_PIXEL_FORMAT=MIPI_DCS_PIXEL_FORMAT_16BIT
#     MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_RGB
#     MIPI_DISPLAY_WIDTH=240
#     MIPI_DISPLAY_HEIGHT=240
#     MIPI_DISPLAY_OFFSET_X=0
#     MIPI_DISPLAY_OFFSET_Y=0
#     MIPI_DISPLAY_INVERT=1
# )
