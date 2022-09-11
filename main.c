/*

MIT No Attribution

Copyright (c) 2021 Mika Tuupola

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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <wchar.h>
#include <pico/stdlib.h>
#include <hardware/clocks.h>

#include <sys/time.h>

#include <hagl_hal.h>
#include <hagl.h>
#include <font6x9.h>
#include <fps.h>
#include <aps.h>

#include "metaballs.h"
#include "plasma.h"
#include "rotozoom.h"
#include "deform.h"

static uint8_t effect = 0;
volatile bool fps_flag = false;
volatile bool switch_flag = true;
static fps_instance_t fps;
static aps_instance_t bps;

static uint8_t *buffer;
static hagl_backend_t backend;
static hagl_backend_t *display;

wchar_t message[32];

static const uint64_t US_PER_FRAME_60_FPS = 1000000 / 60;
static const uint64_t US_PER_FRAME_30_FPS = 1000000 / 30;
static const uint64_t US_PER_FRAME_25_FPS = 1000000 / 25;

static char demo[4][32] = {
    "METABALLS",
    "PLASMA",
    "ROTOZOOM",
    "DEFORM",
};

bool switch_timer_callback(struct repeating_timer *t) {
    switch_flag = true;
    return true;
}

bool show_timer_callback(struct repeating_timer *t) {
    fps_flag = true;
    return true;
}

void static inline switch_demo() {
    switch_flag = false;

    switch(effect) {
    case 0:
        printf("Closing metaballs.\n");
        //metaballs_close();
        break;
    case 1:
        printf("Closing plasma.\n");
        plasma_close();
        break;
    case 2:
        printf("Closing rotozoom.\n");
        //rotozoom_close();
        break;
    case 3:
        printf("Closing deform.\n");
        deform_close();
        break;
    }

    effect = (effect + 1) % 4;

    switch(effect) {
    case 0:
        printf("Initialising metaballs.\n");
        metaballs_init();
        break;
    case 1:
        printf("Initialising plasma.\n");
        plasma_init(display);
        break;
    case 2:
        printf("Initialising rotozoom.\n");
        rotozoom_init();
        break;
    case 3:
        printf("Initialising deform.\n");
        deform_init();
        break;
    }

    fps_init(&fps);
    aps_init(&bps);
}

void static inline show_fps() {
    color_t green = hagl_color(display, 0, 255, 0);

    fps_flag = 0;

    /* Set clip window to full screen so we can display the messages. */
    hagl_set_clip_window(display, 0, 0, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 1);

    /* Print the message on top left corner. */
    swprintf(message, sizeof(message), L"%s    ", demo[effect]);
    hagl_put_text(display, message, 4, 4, green, font6x9);

    /* Print the message on lower left corner. */
    swprintf(message, sizeof(message), L"%.*f FPS  ", 0, fps.current);
    hagl_put_text(display, message, 4, DISPLAY_HEIGHT - 14, green, font6x9);

    /* Print the message on lower right corner. */
    swprintf(message, sizeof(message), L"%.*f KBPS  ", 0, bps.current / 1024);
    hagl_put_text(display, message, DISPLAY_WIDTH - 60, DISPLAY_HEIGHT - 14, green, font6x9);

    /* Set clip window back to smaller so effects do not mess the messages. */
    hagl_set_clip_window(display, 0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);
}

int main()
{
    size_t bytes = 0;
    struct repeating_timer switch_timer;
    struct repeating_timer show_timer;

    set_sys_clock_khz(133000, true);
    clock_configure(
        clk_peri,
        0,
        CLOCKS_CLK_PERI_CTRL_AUXSRC_VALUE_CLKSRC_PLL_SYS,
        133000 * 1000,
        133000 * 1000
    );

    stdio_init_all();

    /* Sleep so that we have time to open the serial console. */
    sleep_ms(5000);

    fps_init(&fps);

    // memset(&backend, 0, sizeof(hagl_backend_t));
    // backend.buffer = malloc(MIPI_DISPLAY_WIDTH * MIPI_DISPLAY_HEIGHT * (DISPLAY_DEPTH / 8));
    // backend.buffer2 = malloc(MIPI_DISPLAY_WIDTH * MIPI_DISPLAY_HEIGHT * (DISPLAY_DEPTH / 8));
    // hagl_hal_init(&backend);
    // display = &backend;

    // memset(&backend, 0, sizeof(hagl_backend_t));
    // backend.buffer = malloc(MIPI_DISPLAY_WIDTH * MIPI_DISPLAY_HEIGHT * (DISPLAY_DEPTH / 8));
    // hagl_hal_init(&backend);
    // display = &backend;

    display = hagl_init();

    hagl_clear(display);
    hagl_set_clip_window(display, 0, 20, DISPLAY_WIDTH - 1, DISPLAY_HEIGHT - 21);

    /* Change demo every 10 seconds. */
    add_repeating_timer_ms(10000, switch_timer_callback, NULL, &switch_timer);

    /* Update displayed FPS counter every 250 ms. */
    add_repeating_timer_ms(250, show_timer_callback, NULL, &show_timer);

    while (1) {

        uint64_t start = time_us_64();

        switch(effect) {
        case 0:
            metaballs_animate();
            metaballs_render(display);
            break;
        case 1:
            plasma_animate();
            plasma_render(display);
            break;
        case 2:
            rotozoom_animate();
            rotozoom_render(display);
            break;
        case 3:
            deform_animate();
            deform_render(display);
            break;
        }

        /* Update the displayed fps if requested. */
        if (fps_flag) {
            show_fps();
        }

        /* Flush back buffer contents to display. NOP if single buffering. */
        bytes = hagl_flush(display);

        aps_update(&bps, bytes);
        fps_update(&fps);

        /* Print the message in console and switch to next demo. */
        if (switch_flag) {
            printf("%s at %d fps / %d kBps\r\n", demo[effect], (uint32_t)fps.current, (uint32_t)(bps.current / 1024));
            switch_demo();
        }

        /* Cap the demos to 60 fps. This is mostly to accommodate to smaller */
        /* screens where plasma will run too fast. */
        busy_wait_until(start + US_PER_FRAME_60_FPS);
    };

    return 0;
}
