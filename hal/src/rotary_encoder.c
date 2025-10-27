// This program reads input of the rotary encoder from GPIO
// rotary_encoder.c
#define _GNU_SOURCE
#include "hal/rotary_encoder.h"

#include <gpiod.h>
#include <pthread.h>
#include <poll.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

typedef struct {
    struct gpiod_chip* chip;
    struct gpiod_line* lineA;
    struct gpiod_line* lineB;
    pthread_t          thread;
    pthread_mutex_t    mtx;
    volatile bool      running;
    int32_t            count;
    int                lastAB;       // 0..3, last sampled AB state
    bool               active_low;
    int                debounce_us;
    struct timespec    last_ts;      // last processed event time (monotonic)
} Rotary;

static inline int64_t ts_diff_us(const struct timespec* a, const struct timespec* b)
{
    int64_t us = (int64_t)(a->tv_sec - b->tv_sec) * 1000000LL;
    us += (int64_t)(a->tv_nsec - b->tv_nsec) / 1000LL;
    return us;
}

static int read_ab(Rotary* r)
{
    int va = gpiod_line_get_value(r->lineA);
    int vb = gpiod_line_get_value(r->lineB);
    if (va < 0 || vb < 0) return r->lastAB; // keep prior if read failed

    if (r->active_low) { va = !va; vb = !vb; }
    return ((va & 1) << 1) | (vb & 1);
}

// Quadrature state table method:
// valid transitions: 00->01->11->10->00 (CW) and reverse for CCW.
// We map (prev<<2)|curr to delta {-1,0,+1}; illegal transitions ignored.
static int quad_delta(int prev, int curr)
{
    static const int8_t table[16] = {
        // prev<<2 | curr : delta
        0,  // 0000
        +1, // 0001 (00->01)
        -1, // 0010 (00->10)
        0,  // 0011 (invalid 00->11)
        -1, // 0100 (01->00)
        0,  // 0101 (01->01)
        0,  // 0110 (01->10 invalid)
        +1, // 0111 (01->11)
        +1, // 1000 (10->00)
        0,  // 1001 (10->01 invalid)
        0,  // 1010 (10->10)
        -1, // 1011 (10->11)
        0,  // 1100 (11->00 invalid)
        -1, // 1101 (11->01)
        +1, // 1110 (11->10)
        0   // 1111 (11->11)
    };
    int idx = ((prev & 0x3) << 2) | (curr & 0x3);
    return table[idx];
}

static void* rotary_thread(void* arg)
{
    Rotary* r = (Rotary*)arg;
    int fdA = gpiod_line_event_get_fd(r->lineA);
    int fdB = gpiod_line_event_get_fd(r->lineB);

    struct pollfd pfds[2] = {
        { .fd = fdA, .events = POLLIN, .revents = 0 },
        { .fd = fdB, .events = POLLIN, .revents = 0 }
    };

    r->lastAB = read_ab(r);
    clock_gettime(CLOCK_MONOTONIC, &r->last_ts);

    while (r->running) {
        int ret = poll(pfds, 2, 250); // 250 ms timeout to allow clean shutdown
        if (!r->running) break;
        if (ret < 0) {
            if (errno == EINTR) continue;
            perror("poll");
            break;
        }
        if (ret == 0) continue; // timeout

        // Consume any pending events (A and/or B)
        struct gpiod_line_event ev;
        bool got = false;
        if (pfds[0].revents & POLLIN) {
            while (gpiod_line_event_read(r->lineA, &ev) == 0) got = true;
        }
        if (pfds[1].revents & POLLIN) {
            while (gpiod_line_event_read(r->lineB, &ev) == 0) got = true;
        }
        if (!got) continue;

        struct timespec now;
        clock_gettime(CLOCK_MONOTONIC, &now);

        // Debounce (time-based)
        if (r->debounce_us > 0 && ts_diff_us(&now, &r->last_ts) < r->debounce_us) {
            continue;
        }
        r->last_ts = now;

        // Sample both lines and compute delta
        int curr = read_ab(r);
        int prev = r->lastAB;
        if (curr != prev) {
            int d = quad_delta(prev, curr);
            if (d != 0) {
                pthread_mutex_lock(&r->mtx);
                r->count += d;
                pthread_mutex_unlock(&r->mtx);
            }
            r->lastAB = curr;
        }
    }
    return NULL;
}

Rotary* Rotary_create(const char* chip_name, int line_a_offset, int line_b_offset,
                      bool active_low, int debounce_us)
{
    Rotary* r = calloc(1, sizeof(*r));
    if (!r) return NULL;

    if (!chip_name) chip_name = "gpiochip0";

    r->chip = gpiod_chip_open_by_name(chip_name);
    if (!r->chip) { perror("gpiod_chip_open_by_name"); free(r); return NULL; }

    r->lineA = gpiod_chip_get_line(r->chip, line_a_offset);
    r->lineB = gpiod_chip_get_line(r->chip, line_b_offset);
    if (!r->lineA || !r->lineB) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(r->chip); free(r); return NULL;
    }

    // Request both lines for edge events
    int flags = active_low ? GPIOD_LINE_REQUEST_FLAG_ACTIVE_LOW : 0;
    if (gpiod_line_request_both_edges_events_flags(r->lineA, "rotaryA", flags) < 0 ||
        gpiod_line_request_both_edges_events_flags(r->lineB, "rotaryB", flags) < 0) {
        perror("gpiod_line_request_both_edges_events");
        if (gpiod_line_is_requested(r->lineA)) gpiod_line_release(r->lineA);
        if (gpiod_line_is_requested(r->lineB)) gpiod_line_release(r->lineB);
        gpiod_chip_close(r->chip); free(r); return NULL;
    }

    pthread_mutex_init(&r->mtx, NULL);
    r->active_low = active_low;
    r->debounce_us = debounce_us;
    r->running = true;

    if (pthread_create(&r->thread, NULL, rotary_thread, r) != 0) {
        perror("pthread_create");
        gpiod_line_release(r->lineA);
        gpiod_line_release(r->lineB);
        gpiod_chip_close(r->chip);
        pthread_mutex_destroy(&r->mtx);
        free(r);
        return NULL;
    }
    return r;
}

void Rotary_destroy(Rotary* r)
{
    if (!r) return;
    r->running = false;
    // Nudge the poll by sending a dummy timeout; or close FDs by releasing lines first after join
    pthread_join(r->thread, NULL);

    if (r->lineA) gpiod_line_release(r->lineA);
    if (r->lineB) gpiod_line_release(r->lineB);
    if (r->chip)  gpiod_chip_close(r->chip);
    pthread_mutex_destroy(&r->mtx);
    free(r);
}

int32_t Rotary_getCount(Rotary* r)
{
    if (!r) return 0;
    pthread_mutex_lock(&r->mtx);
    int32_t v = r->count;
    pthread_mutex_unlock(&r->mtx);
    return v;
}

void Rotary_setCount(Rotary* r, int32_t value)
{
    if (!r) return;
    pthread_mutex_lock(&r->mtx);
    r->count = value;
    pthread_mutex_unlock(&r->mtx);
}

void Rotary_reset(Rotary* r) { Rotary_setCount(r, 0); }

int Rotary_getAB(Rotary* r)
{
    if (!r) return -1;
    return read_ab(r);
}