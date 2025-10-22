// timing.h
// ENSC 351 Fall 2025
// Timing functions for BeagleY-AI

#ifndef TIMING_H
#define TIMING_H
#include <stdio.h> // fopen, fprintf, fclose, perror
#include <stdlib.h>  // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <stdbool.h>
#include <time.h>

// Get the current time in milliseconds
long long getTimeInMs(void);

// Sleep for the specified delay in milliseconds
void sleepForMs(long long delayInMs);



#endif

