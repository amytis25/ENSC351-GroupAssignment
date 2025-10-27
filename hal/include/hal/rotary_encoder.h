// hal/rotary.h
// Simple quadrature rotary encoder reader using libgpiod (v1.x API).
// Thread-safe API with internal event thread and mutex-protected count.

#pragma once
#include <stdbool.h>
#include <stdint.h>

// Opaque handle:
typedef struct Rotary Rotary;

// Create & start the encoder:
//  chip_name: e.g., "gpiochip0" (or pass NULL to use gpiochip0)
//  line_a_offset / line_b_offset: GPIO line offsets for channels A and B
//  active_low: set true if your wiring inverts the logic level
//  debounce_us: ignore edges that occur within this many microseconds of the last processed edge
Rotary* Rotary_create(const char* chip_name, int line_a_offset, int line_b_offset,
                      bool active_low, int debounce_us);

// Stop thread, free resources
void Rotary_destroy(Rotary* r);

// Return and/or modify the accumulated step count (thread-safe)
int32_t Rotary_getCount(Rotary* r);
void    Rotary_setCount(Rotary* r, int32_t value);
void    Rotary_reset(Rotary* r);

// Optional: non-blocking snapshot of last raw AB state (00..11)
int     Rotary_getAB(Rotary* r);