// hal/rotary_encoder.c
// Minimal GPIO button via /dev/gpiochip2 (v1 API). No sysfs.
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
#define BUTTON_LINE   8     // from `gpioinfo`: gpiochip2 8 "GPIO17"
#define ACTIVE_LOW    1     // pressed pulls line LOW (typical with pull-up)

static int line_fd = -1;

bool Button_init(void) {
    int chip_fd = open(GPIOCHIP_PATH, O_RDONLY | O_CLOEXEC);
    if (chip_fd < 0) { perror("open gpiochip"); return false; }

    struct gpiohandle_request req;
    memset(&req, 0, sizeof(req));
    req.lineoffsets[0] = BUTTON_LINE;
    req.lines = 1;
    req.flags = GPIOHANDLE_REQUEST_INPUT;
#ifdef GPIOHANDLE_REQUEST_BIAS_PULL_UP
    req.flags |= GPIOHANDLE_REQUEST_BIAS_PULL_UP; // ok if unsupported; kernel ignores
#endif

    if (ioctl(chip_fd, GPIO_GET_LINEHANDLE_IOCTL, &req) < 0) {
        perror("GPIO_GET_LINEHANDLE_IOCTL");
        close(chip_fd);
        return false;
    }
    close(chip_fd);

    line_fd = req.fd; // handle used for reads
    return (line_fd >= 0);
}

bool Button_read(bool *pressed) {
    if (line_fd < 0 || !pressed) return false;

    struct gpiohandle_data data;
    memset(&data, 0, sizeof(data));

    if (ioctl(line_fd, GPIOHANDLE_GET_LINE_VALUES_IOCTL, &data) < 0) {
        perror("GPIOHANDLE_GET_LINE_VALUES_IOCTL");
        return false;
    }

#if ACTIVE_LOW
    *pressed = (data.values[0] == 0);  // pressed = logic low
#else
    *pressed = (data.values[0] == 1);  // pressed = logic high
#endif
    return true;
}

void Button_close(void) {
    if (line_fd >= 0) { close(line_fd); line_fd = -1; }
}

// ====== Configure polling/debounce ======
#define SAMPLE_US        500        // poll every 0.5 ms (2 kHz)
#define DEBOUNCE_US      800        // must be stable this long to accept a change
#define STABLE_SAMPLES   (DEBOUNCE_US / SAMPLE_US)

// ====== Internal state ======
typedef struct {
    struct gpiod_chip* chip;
    struct gpiod_line* lineA;
    struct gpiod_line* lineB;
    bool active_low;

    pthread_t thread;
    atomic_bool running;

    // step counters
    atomic_long count;        // total steps (CW +, CCW -)
    atomic_long delta;        // steps since last read

    // debounce & quadrature
    int last_rawAB;           // 0..3
    int stableAB;             // debounced state 0..3
    int stable_ctr;           // stability counter

    // gray-seq accumulation to only count full detents
    // A typical mechanical encoder produces 4 transitions per detent.
    // We'll count +1/-1 per full 4-state cycle.
    int seq_accum;            // +1/-1 steps through the 4-phase; count detent when |accum|==4
} enc_t;

static enc_t s;
static pthread_mutex_t s_lock = PTHREAD_MUTEX_INITIALIZER;

// ====== Small helpers ======
static inline int readAB(enc_t* e)
{
    int a = gpiod_line_get_value(e->lineA);
    int b = gpiod_line_get_value(e->lineB);
    if (a < 0 || b < 0) {
        // Non-fatal: treat as no change
        return e->stableAB;
    }
    if (e->active_low) { a = !a; b = !b; }
    return (a << 1) | (b & 0x1);
}

// Quadrature step table: returns +1 (CW), -1 (CCW), 0 (no move/invalid)
static inline int qstep(int prev, int curr)
{
    // valid edges in Gray code: 00->01->11->10->00 (CW) and reverse for CCW
    static const int table[4][4] = {
/*prev\curr 00 01 10 11 */
    /*00*/  { 0, +1, -1,  0 },
    /*01*/  { -1, 0,  0, +1 },
    /*10*/  { +1, 0,  0, -1 },
    /*11*/  { 0, -1, +1,  0 }
    };
    return table[prev & 3][curr & 3];
}

static void* reader_thread(void* arg)
{
    (void)arg;
    enc_t* e = &s;

    // Initial samples
    e->last_rawAB = readAB(e);
    e->stableAB   = e->last_rawAB;
    e->stable_ctr = 0;
    e->seq_accum  = 0;

    while (atomic_load(&e->running)) {
        int raw = readAB(e);

        // Debounce: require STABLE_SAMPLES consecutive identical raw readings
        if (raw == e->last_rawAB) {
            if (e->stable_ctr < STABLE_SAMPLES) e->stable_ctr++;
        } else {
            e->stable_ctr = 0;
        }
        e->last_rawAB = raw;

        if (e->stable_ctr >= STABLE_SAMPLES) {
            if (raw != e->stableAB) {
                // State change accepted; compute quadrature direction
                int dir = qstep(e->stableAB, raw);
                e->stableAB = raw;

                if (dir != 0) {
                    e->seq_accum += dir;

                    // Full detent reached? (4 transitions per detent)
                    if (e->seq_accum >= +4 || e->seq_accum <= -4) {
                        long step = (e->seq_accum > 0) ? +1 : -1;
                        atomic_fetch_add(&e->count, step);
                        atomic_fetch_add(&e->delta, step);

                        // Tag this detent completion for jitter stats:
                        Period_markEvent(PERIOD_EVENT_ROTARY_STEP);

                        // keep any overshoot (in case of very fast spinning)
                        e->seq_accum %= 4;
                    }
                }
            }
        }

        // Sleep
        struct timespec ts = { .tv_sec = 0, .tv_nsec = SAMPLE_US * 1000 };
        nanosleep(&ts, NULL);
    }
    return NULL;
}

// ====== Public API ======
bool RotaryEnc_init(const char* gpiochip_path,
                    unsigned lineA,
                    unsigned lineB,
                    bool active_low)
{
    memset(&s, 0, sizeof(s));
    s.active_low = active_low;

    s.chip = gpiod_chip_open(gpiochip_path);
    if (!s.chip) {
        fprintf(stderr, "RotaryEnc: failed to open chip '%s': %s\n",
                gpiochip_path, strerror(errno));
        return false;
    }

    s.lineA = gpiod_chip_get_line(s.chip, lineA);
    s.lineB = gpiod_chip_get_line(s.chip, lineB);
    if (!s.lineA || !s.lineB) {
        fprintf(stderr, "RotaryEnc: failed to get lines A=%u B=%u\n", lineA, lineB);
        RotaryEnc_cleanup();
        return false;
    }

    // Request inputs (best-effort: bias pull-up to reduce chatter)
#if defined(GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP)
    int flags = GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP;
#else
    int flags = 0;
#endif
    if (gpiod_line_request_input_flags(s.lineA, "rotary-A", flags) < 0 ||
        gpiod_line_request_input_flags(s.lineB, "rotary-B", flags) < 0) {
        fprintf(stderr, "RotaryEnc: failed to request input lines: %s\n", strerror(errno));
        RotaryEnc_cleanup();
        return false;
    }

    atomic_store(&s.count, 0);
    atomic_store(&s.delta, 0);
    atomic_store(&s.running, true);

    if (pthread_create(&s.thread, NULL, reader_thread, NULL) != 0) {
        fprintf(stderr, "RotaryEnc: pthread_create failed\n");
        RotaryEnc_cleanup();
        return false;
    }
    return true;
}

long RotaryEnc_getAndClearDelta(void)
{
    // atomic exchange
    return atomic_exchange(&s.delta, 0);
}

long RotaryEnc_peekCount(void)
{
    return atomic_load(&s.count);
}

void RotaryEnc_resetCount(long newVal)
{
    atomic_store(&s.count, newVal);
    atomic_store(&s.delta, 0);
    // Note: we do not touch debouncer/seq_accum here
}

void RotaryEnc_cleanup(void)
{
    if (s.chip || s.lineA || s.lineB) {
        atomic_store(&s.running, false);
        if (s.thread) { pthread_join(s.thread, NULL); }
    }

    if (s.lineA) { gpiod_line_release(s.lineA); s.lineA = NULL; }
    if (s.lineB) { gpiod_line_release(s.lineB); s.lineB = NULL; }
    if (s.chip)  { gpiod_chip_close(s.chip);    s.chip  = NULL; }
}