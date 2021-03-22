cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_SDK_PATH "../../pico-sdk")

# Pull in Pico SDK (must be before project)
include(pico_sdk_import.cmake)
project(firmware C CXX ASM)

pico_sdk_init()

add_executable(firmware main.c metaballs.c plasma.c rotozoom.c deform.c clock.c)

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
  NDEBUG
  #HAGL_HAL_USE_SINGLE_BUFFER
  #HAGL_HAL_USE_DOUBLE_BUFFER
  HAGL_HAL_USE_TRIPLE_BUFFER
  HAGL_HAL_USE_DMA
  HAGL_HAL_DEBUG
)

#
# Example how to override HAGL HAL settings to make the display vertical.
#

#target_compile_definitions(firmware PRIVATE
#  MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_SWAP_XY|MIPI_DCS_ADDRESS_MODE_MIRROR_Y
#  MIPI_DISPLAY_WIDTH=240
#  MIPI_DISPLAY_HEIGHT=135
#  MIPI_DISPLAY_OFFSET_X=40
#  MIPI_DISPLAY_OFFSET_Y=52
#)

#
# Example how to override HAGL HAL settings for a 128x128 ILI9163 board.
# https://www.aliexpress.com/item/32226457046.html
#

#target_compile_definitions(firmware PRIVATE
#  MIPI_DISPLAY_PIN_RST=21
#  MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_BGR
#  MIPI_DISPLAY_WIDTH=128
#  MIPI_DISPLAY_HEIGHT=128
#  MIPI_DISPLAY_DEPTH=16
#  MIPI_DISPLAY_OFFSET_X=0
#  MIPI_DISPLAY_OFFSET_Y=0
#  MIPI_DISPLAY_INVERT=0
#)
