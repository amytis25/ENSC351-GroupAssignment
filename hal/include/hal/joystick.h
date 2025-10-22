// SPI.c
// ENSC 351 Fall 2025
// SPI functions for BeagleY-AI

#ifndef JOYSTICK_H
#define JOYSTICK_H


#include "hal/led.h"
#include "hal/timing.h"
#include "hal/SPI.h"
#include <stdio.h> // fopen, fprintf, fclose, perror
#include <stdlib.h>  // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <stdbool.h>
#include <time.h>

// joystick_values struct to hold normalized x and y values
typedef struct  {
    int x; // normalized x value (-100 to 100)
    int y; // normalized y value (-100 to 100)
} joystick_values;

static int normalize_adc(int raw);

joystick_values Read_Joystick_Values(void);

#endif 