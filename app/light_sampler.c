// light_sampler.c
// ENSC 351 – Assignment 2 
// main program

/* 
 - Use the light sensor to read current light level.
        - doing with SPI.c. sampler.c??????
 - Compute and print out the number of dips in the light intensity seen in the previous second.
        - another func for this (?) light_dip_detector.c
 - Use the rotary encoder to control how fast the LED blinks (using PWM). This LED emitter
    is designed to flash directly at the “detector” light sensor.
        - using rotary_encoder.c to encode switch with value and PWM.c to take this value and set the LED blink rate
        - rotary_encoder.c should call PWM.c to set the blink rate
 - Each second, print some information to the terminal.
        - this seems to call sampler.c???? period_timer??? help????
 - Listen to a UDP socket and respond to commands
        - UDP.c
 - Use the terminal to display some simple status information.
        - helper function in main function? 
*/

#include "hal/timing.h"
#include "hal/sampler.h"
#include "hal/SPI.h"
#include "hal/periodTimer.h"
#include "hal/PWM.h"
#include "hal/rotary_encoder.h"
#include "hal/UDP.h"
#include <stdio.h> // fopen, fprintf, fclose, perror
#include <stdlib.h>  // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <stdbool.h>
#include <time.h>
#include <stdarg.h>  // for va_list, va_start, va_end
#include <signal.h>  // for signal handling

#define MS_IN_SECOND 1000

// Flag to indicate when the program should exit
static volatile bool running = true;

// Signal handler for CTRL+C
static void cleanup_handler(int signo) {
    if (signo == SIGINT) {
        printf("\nReceived CTRL+C, cleaning up...\n");
        running = false;
    }
}

// Function to cleanup all resources
static void cleanup_resources(void) {
    // Stop UDP server
    udp_stop();
    
    // Cleanup all modules in reverse order of initialization
    PWM_disable();
    Sampler_cleanup();
    rotary_close();
    Period_cleanup();
    
    printf("Cleanup complete. Exiting.\n");
}

// Current PWM frequency (Hz). Made global so UDP control callbacks can access it.
static int current_freq = 1;
static bool console_output_enabled = true;  // Allow disabling console output

// Format and display status each second (fixed-width fields for stable alignment)
static void display_status(
    int samples_in_second,
    int led_hz,
    double avg_light,
    int dips,
    const Period_statistics_t *light_stats,
    const double *history_samples,
    int history_size)
{
    // Line 1: counts and levels
    // Fields are fixed-width to keep columns aligned as values change
    printf("\nSamples: %4d  LED: %3d Hz  Light: %6.3fV  Dips: %3d\n",
           samples_in_second, led_hz, avg_light, dips);

    // Timing jitter information for samples collected during the previous second
    // Format: Smpl ms[{min}, {max}] avg {avg}/{num-samples}
    if (light_stats) {
        printf("Smpl ms[%6.1f, %6.1f] avg %6.1f/%4d\n",
               light_stats->minPeriodInMs,
               light_stats->maxPeriodInMs,
               light_stats->avgPeriodInMs,
               light_stats->numSamples);
    }

    // Line 2: up to 10 evenly-spaced samples from the previous second
    if (history_samples && history_size > 0) {
        int to_show = history_size < 10 ? history_size : 10;
        // Evenly spaced indices across [0, history_size-1]
        for (int k = 0; k < to_show; k++) {
            int idx;
            if (history_size <= 10) {
                idx = k;
            } else {
                // Map k in [0,9] to an index in [0,history_size-1]
                // round((k * (N-1)) / 9.0)
                double pos = (double)k * (double)(history_size - 1) / 9.0;
                idx = (int)(pos + 0.5);
            }
            // Print as {sample number}:{value} with stable widths
            printf(" %4d:%6.3f", idx, history_samples[idx]);
        }
        printf("\n");
    }
}

// (Removed) send_status helper was used for UDP + console combined output.
// Terminal output is now handled by display_status() for fixed-format printing.

// UDP control wrappers (called from UDP thread)
static bool cb_set_console_output(bool enabled) {
    console_output_enabled = enabled;
    return true;
}

static bool cb_set_frequency(int hz) {
    if (hz <= 0 || hz > 1000) return false;
    // Keep duty at 50% when changing frequency via this simple wrapper
    bool ok = PWM_setFrequency(hz, 50);
    if (ok) current_freq = hz;
    return ok;
}
static bool cb_set_duty(int pct) {
    if (pct < 0 || pct > 100) return false;
    // Reuse PWM_setFrequency helper to compute period+duty in ns
    return PWM_setFrequency(current_freq, pct);
}

int main() {
    // Set up signal handler for CTRL+C
    if (signal(SIGINT, cleanup_handler) == SIG_ERR) {
        fprintf(stderr, "Failed to set up signal handler\n");
        return -1;
    }
    
    printf("Starting light_sampler application...\n");

    // Initialize modules
    Period_init();  // Initialize period timer first
    if (!PWM_export()) {
        fprintf(stderr, "Failed to export PWM\n");
        return -1;
    }
    
    if (!rotary_init()) {
        fprintf(stderr, "Failed to initialize rotary encoder\n");
        return -1;
    }

    Sampler_init();
    rotary_start();
    PWM_enable();

    UdpCallbacks cb = {
        .get_count = Sampler_getNumSamplesTaken,
        .get_history_size = Sampler_getHistorySize,
        .get_dips = Sampler_getDipCount,        // Now uses Period timer directly
        .get_history = Sampler_getHistory,
        .set_frequency = cb_set_frequency,
        .set_duty = cb_set_duty,
        .set_console_output = cb_set_console_output  // Allow remote control of console output
    };

    if (udp_start(12345, cb) != 0) {
        fprintf(stderr, "udp_start failed\n");
    }
    // Set initial PWM frequency
    PWM_setFrequency(current_freq, 50);  // 50% duty cycle
    long long lastTime = getTimeInMs();
    //long long startTimeS = getTimeInMs();
    // Main processing loop
    while (running) {
        // Update LED blink rate based on rotary encoder
        int edges = rotary_getCount();
        
        // Convert edges to detents (4 edges per detent)
        int detents = edges / 4;
        
        // Start at 10 Hz, add detents, and clamp to 1-500 Hz range
        int new_freq = 10 + detents;
        if (new_freq < 1)   new_freq = 1;    // Minimum 1 Hz
        if (new_freq > 500) new_freq = 500;  // Maximum 500 Hz
        
        if (new_freq != current_freq) {
            current_freq = new_freq;
            PWM_setFrequency(current_freq, 50);  // 50% duty cycle
        }
        
       long long timeDiff = (getTimeInMs() - lastTime)/MS_IN_SECOND;
       if (timeDiff >= 1) {
            // Process light samples every second
            Sampler_moveCurrentDataToHistory();
            // Drain period-timer samples for the light-sample event so the
            // internal timestamp buffer doesn't overflow when sampling at
            // high rates (Sampler marks an event ~1000Hz). We don't need
            // the stats here, so discard them.
            Period_statistics_t _lastSecondsSample;
            Period_statistics_t dipStats;
            Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &_lastSecondsSample);
            Period_getStatisticsAndClear(PERIOD_EVENT_DIP, &dipStats);
            int dips_in_last_second = dipStats.numSamples;
            int history_size;
            double* samples = Sampler_getHistory(&history_size);
            if (samples) {
            //int dips = Sampler_getDipCount();
            
            double avg = Sampler_getAverageReading();
            lastTime = getTimeInMs();
            // long long TotalTimeS = (getTimeInMs()-startTimeS)/MS_IN_SECOND; // no longer printed
            // Print terminal status exactly as specified
            display_status(
                history_size,           // samples in previous second
                current_freq,            // LED Hz
                avg,                     // averaged light level (V)
                dips_in_last_second,     // dips found in previous second
                &_lastSecondsSample,     // timing jitter stats for light samples
                samples,                 // history samples from previous second
                history_size);
              
            free(samples);
            }
        }
              // Wait for next second
              //sleepForMs(1000);
    }

    // Perform cleanup
    cleanup_resources();
    return 0;
}
