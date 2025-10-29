// rotary_encoder.h
// Interface for reading from a rotary encoder using GPIO

#pragma once
#include <stdbool.h>

// Initialize the rotary encoder on the specified GPIO pins
// Uses GPIO chip 2 with pins:
// - Output A: GPIO 15 (GPIO5)
// - Output B: GPIO 17 (GPIO6)
// Returns true if initialization was successful, false otherwise
bool rotary_init(void);

// Start the rotary encoder polling thread
// This function creates a background thread that continuously monitors the encoder
void rotary_start(void);

// Get the current count from the rotary encoder
// Returns the accumulated count (positive for clockwise, negative for counter-clockwise)
int rotary_getCount(void);

// Clean up and release resources used by the rotary encoder
void rotary_close(void);