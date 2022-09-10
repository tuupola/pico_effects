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

target_compile_definitions(firmware PRIVATE
  #HAGL_HAL_USE_SINGLE_BUFFER
  HAGL_HAL_USE_DOUBLE_BUFFER
  #HAGL_HAL_USE_TRIPLE_BUFFER
  HAGL_HAL_USE_DMA
  HAGL_HAL_DEBUG
  MIPI_DISPLAY_SPI_CLOCK_SPEED_HZ=68000000
)

#
# Example how to override default HAGL HAL settings to make the display vertical.
#

#target_compile_definitions(firmware PRIVATE
#  MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_SWAP_XY|MIPI_DCS_ADDRESS_MODE_MIRROR_Y
#  MIPI_DISPLAY_WIDTH=240
#  MIPI_DISPLAY_HEIGHT=135
#  MIPI_DISPLAY_OFFSET_X=40
#  MIPI_DISPLAY_OFFSET_Y=52
#)

#
# Example how to override default HAGL HAL settings for a 128x128 ILI9163 board.
# https://www.aliexpress.com/item/32226457046.html
#

#target_compile_definitions(firmware PRIVATE
#  MIPI_DISPLAY_PIN_RST=21
#  MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_BGR
#  MIPI_DISPLAY_WIDTH=128
#  MIPI_DISPLAY_HEIGHT=128
#  MIPI_DISPLAY_OFFSET_X=0
#  MIPI_DISPLAY_OFFSET_Y=0
#  MIPI_DISPLAY_INVERT=0
#)

#
# Example of full HAGL HAL settings for a 128x128 ST7735S board.
# https://www.aliexpress.com/item/4000219159401.html
#

#target_compile_definitions(firmware PRIVATE
#  MIPI_DISPLAY_PIN_CS=17
#  MIPI_DISPLAY_PIN_DC=16 # A0
#  MIPI_DISPLAY_PIN_RST=21
#  MIPI_DISPLAY_PIN_BL=20
#  MIPI_DISPLAY_PIN_CLK=18
#  MIPI_DISPLAY_PIN_MOSI=19 # SDA
#  MIPI_DISPLAY_PIN_MISO=-1
#  MIPI_DISPLAY_PIXEL_FORMAT=MIPI_DCS_PIXEL_FORMAT_16BIT
#  MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_BGR
#  MIPI_DISPLAY_WIDTH=128
#  MIPI_DISPLAY_HEIGHT=128
#  MIPI_DISPLAY_OFFSET_X=2
#  MIPI_DISPLAY_OFFSET_Y=1
#  MIPI_DISPLAY_INVERT=0
#)

#
# Example of full HAGL HAL settings for a 320x240 ILI9341 board.
#

#target_compile_definitions(firmware PRIVATE
#  MIPI_DISPLAY_PIN_CS=17
#  MIPI_DISPLAY_PIN_DC=16
#  MIPI_DISPLAY_PIN_RST=21
#  MIPI_DISPLAY_PIN_BL=20
#  MIPI_DISPLAY_PIN_CLK=18
#  MIPI_DISPLAY_PIN_MOSI=19
#  MIPI_DISPLAY_PIN_MISO=-1
#  MIPI_DISPLAY_PIXEL_FORMAT=MIPI_DCS_PIXEL_FORMAT_16BIT
#  MIPI_DISPLAY_ADDRESS_MODE=MIPI_DCS_ADDRESS_MODE_BGR|MIPI_DCS_ADDRESS_MODE_SWAP_XY
#  MIPI_DISPLAY_WIDTH=320
#  MIPI_DISPLAY_HEIGHT=240
#  MIPI_DISPLAY_OFFSET_X=0
#  MIPI_DISPLAY_OFFSET_Y=0
#  MIPI_DISPLAY_INVERT=0
#)
