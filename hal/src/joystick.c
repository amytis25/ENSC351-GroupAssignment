

#include "hal/led.h"
#include "hal/timing.h"
#include "hal/SPI.h"
#include <stdio.h> // fopen, fprintf, fclose, perror
#include <stdlib.h>  // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <stdbool.h>
#include <time.h>
#include "hal/joystick.h"

// Normalize 12-bit ADC value (0..4095) to -100..100 with center at 2048
static int normalize_adc(int raw) {
    const int center = 2048;
    const int span = 2048; // symmetric span
    int offset = raw - center; // -2048..+2047
    double frac = (double)offset / (double)span; // approx -1..+1
    if (frac > 1.0) frac = 1.0;
    if (frac < -1.0) frac = -1.0;
    return (int)(frac * 100.0);
}

joystick_values Read_Joystick_Values(void) {

    joystick_values jv = {0, 0}; // Default return value in case of error
    int ch0_x = Read_ADC_Values(0); // read channel 0 (x)
    int ch1_y = Read_ADC_Values(1); // read channel 1 (y)

    if (ch0_x < 0 || ch1_y < 0) {
        fprintf(stderr, "SPI read error\n");
    } else {
        int n_x = normalize_adc(ch0_x);
        int n_y = normalize_adc(ch1_y);
        //printf("CH0 (x) raw=%d normalized=%d  CH1 (y) raw=%d normalized=%d\n",ch0_x, n_x, ch1_y, n_y);
        jv.x = n_x;
        jv.y = n_y;
    }
}
