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
#include <stdio.h> // fopen, fprintf, fclose, perror
#include <stdlib.h>  // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <stdbool.h>
#include <time.h>

// Count light dips in the sample history
static int count_dips(double* samples, int size) {
    if (size < 3) return 0;
    
    int dips = 0;
    for (int i = 1; i < size - 1; i++) {
        // A dip is when the middle value is significantly lower than both neighbors
        if (samples[i] < samples[i-1] * 0.8 && samples[i] < samples[i+1] * 0.8) {
            dips++;
        }
    }
    return dips;
}

int main() {
    printf("Starting light_sampler application...\n");

    // Initialize modules
    Period_init();  // Initialize period timer first
    
    if (!rotary_init()) {
        fprintf(stderr, "Failed to initialize rotary encoder\n");
        return -1;
    }

    Sampler_init();
    rotary_start();
    PWM_enable();

    // Set initial PWM frequency
    int current_freq = 1;  // Start at 1 Hz
    PWM_setFrequency(current_freq, 50);  // 50% duty cycle

    // Main processing loop
    while (1) {
        // Update LED blink rate based on rotary encoder
        int encoder_count = rotary_getCount();
        int new_freq = 1 + abs(encoder_count % 10);  // Map to 1-10 Hz
        if (new_freq != current_freq) {
            current_freq = new_freq;
            PWM_setFrequency(current_freq, 50);
        }

    // Process light samples every second
       Sampler_moveCurrentDataToHistory();
       // Drain period-timer samples for the light-sample event so the
       // internal timestamp buffer doesn't overflow when sampling at
       // high rates (Sampler marks an event ~1000Hz). We don't need
       // the stats here, so discard them.
       Period_statistics_t _unused_stats;
       Period_getStatisticsAndClear(PERIOD_EVENT_SAMPLE_LIGHT, &_unused_stats);
              int history_size;
              double* samples = Sampler_getHistory(&history_size);
              if (samples) {
              int dips = count_dips(samples, history_size);
              double avg = Sampler_getAverageReading();
              long long total = Sampler_getNumSamplesTaken();
              
              // Print status
              printf("\nStatus Update:\n");
              printf("Light dips in last second: %d\n", dips);
              printf("Average light level: %.2f\n", avg);
              printf("Total samples: %lld\n", total);
              printf("LED frequency: %d Hz\n", current_freq);
              
              free(samples);
              }

              // Wait for next second
              sleepForMs(1000);
    }

    // Cleanup (never reached in this version, but good practice)
    PWM_disable();
    Sampler_cleanup();
    rotary_close();
    Period_cleanup();
    return 0;
}
