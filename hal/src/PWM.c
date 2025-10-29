#include <stdio.h> // fopen, fprintf, fclose, perror
#include <stdlib.h>  // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <stdbool.h>
#include <time.h>
#include "hal/timing.h"
#include "hal/PWM.h"

#define DEBUG 

#define NANOSECONDS_IN_SECOND 1000000000

// Helper function to write to a file
static bool writeToFile(const char* filename, const char* value) {
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        fprintf(stderr, "Error opening file '%s': ", filename);
        perror(NULL);
        return false;
    }
    
    if (fprintf(file, "%s", value) < 0) {
        fprintf(stderr, "Error writing to file '%s': ", filename);
        perror(NULL);
        fclose(file);
        return false;
    }
    
    fclose(file);
    return true;
}

// PWM helper Functions
bool PWM_setDutyCycle(int dutyCycle){
    #ifdef DEBUG
    printf("Setting duty cycle to %d\n", dutyCycle);
    #endif
    writeToFile(PWM_DUTY_CYCLE_FILE, "0"); // --- IGNORE ---
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "%d", dutyCycle);
    if (n < 0) return false;
    return writeToFile(PWM_DUTY_CYCLE_FILE, buf);
}

bool PWM_setPeriod(int period){
    #ifdef DEBUG
    printf("Setting period to %d\n", period);
    #endif
    char buf[32];
    int n = snprintf(buf, sizeof(buf), "%d", period);
    if (n < 0) return false;
    return writeToFile(PWM_PERIOD_FILE, buf);
}

bool PWM_setFrequency(int Hz, int dutyCyclePercent){
    if (Hz <= 0) {
        fprintf(stderr, "PWM_setFrequency: frequency must be positive\n");
        return false;
    }
    if (dutyCyclePercent < 0 || dutyCyclePercent > 100) {
        fprintf(stderr, "PWM_setFrequency: invalid duty cycle percentage\n");
        return false;
    }
    int period = NANOSECONDS_IN_SECOND / Hz; // period in nanoseconds
    #ifdef DEBUG
    printf("Period should be %d\n", period);
    #endif
    int dutyCycle = period * (float)(dutyCyclePercent / 100.0); // duty cycle in nanoseconds
    #ifdef DEBUG
    printf("Duty cycle should be %d\n", dutyCycle);
    #endif
    #ifdef DEBUG
    printf("Setting PWM frequency to %d Hz with duty cycle %d%%\n", Hz, dutyCyclePercent);
    #endif
    return PWM_setPeriod(period) && PWM_setDutyCycle(dutyCycle);
}

bool PWM_enable(){
    #ifdef DEBUG
    printf("Enabling PWM\n");
    #endif
    return writeToFile(PWM_ENABLE_FILE, "1");
}

bool PWM_disable(){
    #ifdef DEBUG
    printf("Disabling PWM\n");
    #endif
    return writeToFile(PWM_ENABLE_FILE, "0");
}