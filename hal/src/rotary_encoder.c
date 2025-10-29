// hal/rotary_encoder.c
// Minimal GPIO button via /dev/gpiochip2 (v1 API). No sysfs.

// ============ MODIFIED BUTTON.C =================
#define _GNU_SOURCE
#include "hal/rotary_encoder.h"
#include "hal/periodTimer.h"  // <-- we will mark steps here

#include <stdbool.h>
#include <fcntl.h>
#include <gpiod.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <linux/gpio.h>

#define GPIOCHIP_PATH "/dev/gpiochip2"
#define OUTPUT_A   15     // gpiochip2 15 for "GPIO5"
#define OUTPUT_B   17     // gpiochip2 17 for "GPIO6"
#define ACTIVE_LOW 1      // pressed pulls line LOW (typical with pull-up)

static int line_fd = -1;

bool rotary_init(void) {
    int chip_fd = open(GPIOCHIP_PATH, O_RDONLY | O_CLOEXEC);
    if (chip_fd < 0) { perror("open gpiochip"); return false; }

    struct gpiohandle_request req;
    memset(&req, 0, sizeof(req));
    req.lineoffsets[0] = OUTPUT_A;
    req.lineoffsets[1] = OUTPUT_B;
    req.lines = 2;
    req.flags = GPIOHANDLE_REQUEST_INPUT;
#ifdef GPIOHANDLE_REQUEST_BIAS_PULL_UP
    req.flags |= GPIOHANDLE_REQUEST_BIAS_PULL_UP; // use internal pull-ups if supported
#endif

    if (ioctl(chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &req) < 0) {
        perror("GPIO_GET_LINEHANDLE_IOCTL");
        close(chip_fd);
        return false;
    }
    close(chip_fd);

    line_fd = req.fd; // handle used for reads
    printf("Rotary encoder ready (A=%d, B=%d)\n", OUTPUT_A, OUTPUT_B);
    return true;
}

static int AB_read(void)
{
    if (line_fd < 0) return -1;

    struct gpiohandle_data data;
    memset(&data, 0, sizeof(data));
    if (ioctl(line_fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) < 0) {
        perror("GPIOHANDLE_GET_LINE_VALUES_IOCTL");
        return -1;
    }

    int A = data.values[0];
    int B = data.values[1];
#if ACTIVE_LOW
    A = !A;
    B = !B;
#endif
    return ((A << 1) | B);   // combine into 2-bit value: AB
}

// ====== chatgpt suggested code ========
static atomic_int g_count = 0;   // global rotation count

// thread function that polls the encoder
static void* rotary_thread(void* arg)
{
    int prev = AB_read();
    if (prev < 0) return NULL;

    while (1) {
        int curr = AB_read();
        if (curr < 0) break;

        int step = decode_step(prev, curr);
        if (step == +1)
            g_count++;               // clockwise
        else if (step == -1)
            g_count--;               // counter-clockwise

        prev = curr;
        usleep(2000);                // 2 ms poll
    }
    return NULL;
}

// start polling (call this from your main)
void rotary_start(void)
{
    pthread_t tid;
    pthread_create(&tid, NULL, rotary_thread, NULL);
    pthread_detach(tid);             // run in background
}

// read current position safely
int rotary_getCount(void)
{
    return atomic_load(&g_count);
}

// ====== end of chatgpt suggested code ========

/*
bool GPIO_read(bool *valueA, bool *valueB) {
    if (line_fd < 0 || !valueA || !valueB) return false;

    struct gpiohandle_data data;
    memset(&data, 0, sizeof(data));

    if (ioctl(line_fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) < 0) {
        perror("GPIOHANDLE_GET_LINE_VALUES_IOCTL");
        return false;
    }
    */

// valid edges in Gray code: 00->01->11->10->00 (CW) and reverse for CCW
static int decode_step(int prev, int curr) 
{
    static const int table[4][4] = {
    /*prev\curr 00 01 10 11 */
    /*00*/  { 0, +1, -1,  0 },
    /*01*/  { -1, 0,  0, +1 },
    /*10*/  { +1, 0,  0, -1 },
    /*11*/  { 0, -1, +1,  0 }
    };
    return table[prev & 3][curr & 3];
}

void rotary_close(void) {
    if (line_fd >= 0) { close(line_fd); line_fd = -1; }
}

