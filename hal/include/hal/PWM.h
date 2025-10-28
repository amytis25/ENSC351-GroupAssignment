#ifndef PWM_H
#define PWM_H
#include <stdio.h> // fopen, fprintf, fclose, perror
#include <stdlib.h>  // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <stdbool.h>
#include <time.h>


#define PWM_DUTY_CYCLE_FILE "/dev/hat/pwm/GPIO15/duty_cycle"
#define PWM_PERIOD_FILE "/dev/hat/pwm/GPIO15/period"
#define PWM_ENABLE_FILE "/dev/hat/pwm/GPIO15/enable"


// PWM helper Functions
bool PWM_setDutyCycle(int dutyCycle);
bool PWM_setPeriod(int period);
bool PWM_setFrequency(int Hz, int dutyCyclePercent);
bool PWM_enable();
bool PWM_disable();



#endif