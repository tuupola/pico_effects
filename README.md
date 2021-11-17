## Old schoold demo effects for Raspberry Pi Pico

![Pico effects](https://appelsiini.net/img/2021/pico-st7735-rotozoom.jpg)

Created to test the [HAGL graphics library](https://github.com/tuupola/hagl). For quick reference see the [recording on Vimeo](https://vimeo.com/510236080). If you cannot compile yourself build directory contains a [recent firmware](https://github.com/tuupola/pico_effects/raw/master/build/firmware.uf2).

## Compile

Below instructions assume you do not have [Raspberry Pi Pico SDK](https://github.com/raspberrypi/pico-sdk) installed yet.

```
$ git clone --recursive https://github.com/raspberrypi/pico-sdk.git
$ git clone --recursive  https://github.com/tuupola/pico_effects.git
$ cd pico_effects/build
$ cmake ..
$ make -j8
```

If the SDK is already installed you can pass the installation path manually.

```
$ git clone --recursive  https://github.com/tuupola/pico_effects.git
$ cd pico_effects/build
$ cmake .. -DPICO_SDK_PATH=../pico-sdk
$ make -j8
```

You can then "flash" the firmware by putting the Pico into `BOOTSEL` mode and copying the uf2 file to the automatically mounted volume.

```
$ cp firmware.uf2 /run/media/<username>/RPI-RP2/
```

If you are using macOS the command would be the following.

```
$ cp firmware.uf2 /Volumes/RPI-RP2
```

## Run on computer

HAGL is hardware agnostic. You can run the demos also [on your computer](https://github.com/tuupola/sdl2_effects).
