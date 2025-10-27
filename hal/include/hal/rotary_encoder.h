// rotary_encoder.h

#pragma once
#include <stdbool.h>
#include <stdint.h>

typedef struct Rotary Rotary;

//  Create and start a rotary encoder reader thread
//  chip_name: e.g., "gpiochip0"
//  line_a_offset / line_b_offset: GPIO line offsets for channels A and B
//  active_low: set true if your wiring inverts the logic level
//  debounce_us: ignore edges that occur within this many microseconds of the last processed edge
Rotary* Rotary_create(const char* chip_name, int line_a_offset, int line_b_offset,
                      bool active_low, int debounce_us);

void Rotary_destroy(Rotary* r);

int32_t Rotary_getCount(Rotary* r);
void    Rotary_setCount(Rotary* r, int32_t value);
void    Rotary_reset(Rotary* r);

// Optional: non-blocking snapshot of last raw AB state (00..11)
int     Rotary_getAB(Rotary* r);