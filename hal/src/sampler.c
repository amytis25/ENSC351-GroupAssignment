
#include <stdio.h> // fopen, fprintf, fclose, perror
#include <stdlib.h>  // exit, EXIT_FAILURE, EXIT_SUCCESS
#include <stdbool.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#include "hal/timing.h"
#include "hal/sampler.h"
#include "hal/SPI.h"
#include "hal/periodTimer.h"

#define SENSOR_CHANNEL 0 // ADC channel for light sensor

#define MAX_SAMPLES_PER_SECOND 1000
#define MAX_SAMPLE_SIZE (MAX_SAMPLES_PER_SECOND + 0.1*MAX_SAMPLES_PER_SECOND) // buffer for 10% overhead

static pthread_t samplerThreadId;
static bool keepRunning = false;

// Thread function declaration
static void* samplerThread(void* arg);

// Buffers
static double *currentSamples = NULL;
static int currentSize = 0;

static double *historySamples = NULL;
static int historySize = 0;

// Stats
static long long totalSamples = 0;
static double avgExp = 0.0;
static bool firstSample = true;

// Synchronization
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void Sampler_init(void){
    keepRunning = true;
    currentSamples = malloc(sizeof(double) * MAX_SAMPLE_SIZE);
    if (!currentSamples) {
        perror("Sampler_init: malloc");
        exit(-1);
    }
    if (Read_ADC_Values(SENSOR_CHANNEL) < 0) {
        perror("Sampler_init: failed Read_ADC_Values");
        exit(-1);
    }
    if (pthread_create(&samplerThreadId, NULL, samplerThread, NULL) != 0) {
        perror("Sampler_init: pthread_create");
        exit(-1);
    }
}

void Sampler_cleanup(void){
    keepRunning = false;
    pthread_join(samplerThreadId, NULL);

    pthread_mutex_lock(&lock);
    free(currentSamples);
    free(historySamples);
    currentSamples = historySamples = NULL;
    currentSize = historySize = 0;
    pthread_mutex_unlock(&lock);
}

// Must be called once every 1s.
// Moves the samples that it has been collecting this second into
// the history, which makes the samples available for reads (below).
void Sampler_moveCurrentDataToHistory(void){
    pthread_mutex_lock(&lock);
    free(historySamples);
    historySamples = malloc(sizeof(double) * currentSize);
    if (historySamples) {
        memcpy(historySamples, currentSamples, sizeof(double) * currentSize);
        historySize = currentSize;
    } else {
        historySize = 0;
    }
    currentSize = 0; // reset for next second
    pthread_mutex_unlock(&lock);
}

// Get the number of samples collected during the previous complete second.
int Sampler_getHistorySize(void){
    pthread_mutex_lock(&lock);
    int size = historySize;
    pthread_mutex_unlock(&lock);
    return size;
}

// Get a copy of the samples in the sample history.
// Returns a newly allocated array and sets `size` to be the
// number of elements in the returned array (output-only parameter).
// The calling code must call free() on the returned pointer.
// Note: It provides both data and size to ensure consistency.
double* Sampler_getHistory(int *size){
    pthread_mutex_lock(&lock);
    *size = historySize;
    double *copy = NULL;
    if (historySamples && historySize > 0) {
        copy = malloc(sizeof(double) * historySize);
        if (copy) {
            memcpy(copy, historySamples, sizeof(double) * historySize);
        }
    }
    pthread_mutex_unlock(&lock);
    return copy;
}

// Get the average light level (not tied to the history).
double Sampler_getAverageReading(void){
    pthread_mutex_lock(&lock);
    double val = avgExp;
    pthread_mutex_unlock(&lock);
    return val;
}

// Get the total number of light level samples taken so far.
long long Sampler_getNumSamplesTaken(void){
    pthread_mutex_lock(&lock);
    long long val = totalSamples;
    pthread_mutex_unlock(&lock);
    return val;
}

// Sampler thread function
// Continuously samples light levels and stores them.
static void* samplerThread(void* arg) {
    (void)arg;  // Suppress unused parameter warning

     while (keepRunning) {
        // 1) Sample ADC
        if (Read_ADC_Values(SENSOR_CHANNEL) < 0) {
            perror("samplerThread: failed Read_ADC_Values");
            continue;
        }
        double volts = Read_ADC_Values(SENSOR_CHANNEL);
        
        // 2) Record timing event
        Period_markEvent(PERIOD_EVENT_SAMPLE_LIGHT);

        // 3) Update exponential average and store sample
        pthread_mutex_lock(&lock);
        if (firstSample) {
            avgExp = volts;
            firstSample = false;
        } else {
            avgExp = 0.999 * avgExp + 0.001 * volts;
        }
        if (currentSize < MAX_SAMPLE_SIZE) {
            currentSamples[currentSize++] = volts;
        }
        totalSamples++;
        pthread_mutex_unlock(&lock);

        // 4) Sleep for 1 ms
        sleepForMs(1);
     }
        return NULL;
}