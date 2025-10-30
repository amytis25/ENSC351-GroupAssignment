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

#define MS_IN_SECOND 1000

// Current PWM frequency (Hz). Made global so UDP control callbacks can access it.
static int current_freq = 1;
static bool console_output_enabled = true;  // Allow disabling console output

// Status reporting helper (called from both UDP and main thread)
static void send_status(const char* format, ...) {
    va_list args;
    va_start(args, format);
    
    // Always send to UDP stream if clients are connected
    char buffer[256];
    vsnprintf(buffer, sizeof(buffer), format, args);
    udp_send_stream_text("%s", buffer);
    
    // Optionally print to console
    if (console_output_enabled) {
        va_end(args);
        va_start(args, format);
        vprintf(format, args);
    }
    
    va_end(args);
}

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
    long long startTimeS = getTimeInMs();
    // Main processing loop
    while (1) {
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
            long long total = Sampler_getNumSamplesTaken();
            lastTime = getTimeInMs();
            long long TotalTimeS = (getTimeInMs()-startTimeS)/MS_IN_SECOND;
            // Print status
            // Send unified status update through our helper
            send_status("\n\nStatus Update, its been %lld seconds!\n", TotalTimeS);
            send_status("In the last %lld seconds:\n", timeDiff);
            send_status("Light dips: %d\n", dips_in_last_second);
            send_status("Average light level: %.2f\n", avg);
            send_status("Total samples: %lld\n", total);
            send_status("LED frequency: %d Hz\n\n", current_freq);
              
            free(samples);
            }
        }
              // Wait for next second
              //sleepForMs(1000);
    }

    // Cleanup (never reached in this version, but good practice)
    PWM_disable();
    Sampler_cleanup();
    rotary_close();
    Period_cleanup();
    return 0;
}
