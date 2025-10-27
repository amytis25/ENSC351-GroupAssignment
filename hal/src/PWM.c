#include <stdio.h> // fopen, fprintf, fclose, perror
#include <stdlib.h>  // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <stdbool.h>
#include <time.h>
#include "hal/timing.h"
#include "hal/PWM.h"

#define NANOSECONDS_IN_SECOND 1000000000

// PWM helper Functions
bool PWM_setDutyCycle(int dutyCycle){
    FILE *file = fopen(PWM_DUTY_CYCLE_FILE, "w");
    if (file == NULL) {
        perror("PWM_setDutyCycle: cannot open file");
        return false;
    }
    fprintf(file, "%d", dutyCycle);
    fclose(file);
    return true;
}

bool PWM_setPeriod(int period){
    FILE *file = fopen(PWM_PERIOD_FILE, "w");
    if (file == NULL) {
        perror("PWM_setPeriod: cannot open file");
        return false;
    }
    fprintf(file, "%d", period);
    fclose(file);
    return true;
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
    int dutyCycle = period * (dutyCyclePercent / 100); // duty cycle in nanoseconds
    return PWM_setPeriod(period) && PWM_setDutyCycle(dutyCycle);
}

bool PWM_enable(){
    FILE *file = fopen(PWM_ENABLE_FILE, "w");
    if (file == NULL) {
        perror("PWM_enable: cannot open file");
        return false;
    }
    fprintf(file, "1");
    fclose(file);
    return true;
}

bool PWM_disable(){
    FILE *file = fopen(PWM_ENABLE_FILE, "w");
    if (file == NULL) {
        perror("PWM_disable: cannot open file");
        return false;
    }
    fprintf(file, "0");
    fclose(file);
    return true;
}