## Old schoold demo effects for Raspberry Pi Pico

![Pico effects](https://appelsiini.net/img/2021/pico-st7735-rotozoom.jpg)

Created to test the [HAGL graphics library](https://github.com/tuupola/hagl). For quick reference see the [recording on Vimeo](https://vimeo.com/510236080). If you cannot compile yourself build directory contains a [recent firmware](https://github.com/tuupola/pico_effects/raw/master/build/firmware.uf2).

## Compile

Below instructions assume you have [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) installed and `PICO_SDK_PATH` environment correctly set up.

```
$ git clone --recursive git@github.com:tuupola/pico_effects.git
$ cd pico_effects/build
$ cmake ..
$ make -j8
```

If you are using macOS you can then "flash" by putting the Pico into `BOOTSEL` mode and copying the uf2 file.

```
$ cp firmware.uf2 /Volumes/RPI-RP2
```

## Run on computer

HAGL is hardware agnostic. You can run the demos also [on your computer](https://github.com/tuupola/sdl2_effects).

