/*

MIT No Attribution

Copyright (c) 2020 Mika Tuupola

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

-cut-

SPDX-License-Identifier: MIT-0

*/

#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <hagl.h>

#include "plasma.h"

color_t palette[256];

static const uint8_t SPEED = 4;
static const uint8_t STEP = 1;

static inline int min(int a, int b) {
    if (a > b) {
        return b;
    };
    return a;
}

static inline int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

uint16_t find_color(color_t color, uint16_t hint)
{
    uint8_t index;
    uint16_t upper = min(256, hint + SPEED * 2);
    uint16_t lower = max(0, hint - SPEED * 2);

    index = upper;

    /* Try to match close to previous match. */
    while (index > lower) {
        if (palette[index] == color) {
            return index;
        } else {
            index--;
        }
    }

    /* If not found search through whole palette. */
    index = 0;
    while (index < 255) {
        if (palette[index] == color) {
            return index;
        } else {
            index++;
        }
    }

    return 0;
}

void plasma_init()
{
    /* Generate nice continous palette with unique colors. */
    uint16_t index = 0;
    uint16_t counter = 0;

    while (index < 256) {
        uint8_t r, g, b;
        counter++;
        r = 128.0 + 128.0 * sin((M_PI * counter / 128.0) + 1);
        g = 128.0 + 128.0 * sin((M_PI * counter / 64.0) + 1);
        b = 64;
        color_t color = hagl_color(r, g, b);

        if (0 == index) {
            palette[index] = color;
            index++;
        } else if (palette[index - 1] != color) {
            palette[index] = color;
            index++;
        }
    }

    for (uint16_t x = 0; x < DISPLAY_WIDTH; x = x + STEP) {
        for (uint16_t y = 0; y < DISPLAY_HEIGHT; y = y + STEP) {
                /* Generate three different sinusoids. */
                float v1 = 128.0 + (128.0 * sin(x / 32.0));
                float v2 = 128.0 + (128.0 * sin(y / 24.0));
                float v3 = 128.0 + (128.0 * sin(sqrt(x * x + y * y) / 24.0));
                /* Calculate average of the three sinusoids */
                /* and use it as color. */
                uint8_t color = (v1 + v2 + v3) / 3;
                hagl_put_pixel(x, y, palette[color]);
        }
    }
}

void plasma_render()
{
    static uint16_t previous = 0;
    for (uint16_t x = 0; x < DISPLAY_WIDTH; x = x + STEP) {
        for (uint16_t y = 0; y < DISPLAY_HEIGHT; y = y + STEP) {

            /* Find the pixels color index from the palette. */
            color_t color = hagl_get_pixel(x, y);
            uint16_t index = find_color(color, previous);
            previous = index;

            /* Animate by selecting next color from the palette. */
            index += SPEED;
            index %= 256;

            hagl_put_pixel(x, y, palette[index]);

            // if (1 == STEP) {
            //     hagl_put_pixel(x, y, palette[index]);
            // } else {
            //     hagl_fill_rectangle(x, y, x + STEP, y + STEP, palette[index]);
            // }
        }
    }
}
