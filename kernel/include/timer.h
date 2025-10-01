#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

// PIT oscillator base frequency
#define PIT_FREQUENCY   1193182
#define PIT_COMMAND     0x43     // Mode/command register
#define PIT_CHANNEL0    0x40     // Channel 0 data port

// Initialize PIT to generate interrupts at specified frequency
void timer_init(uint32_t frequency);

// Returns monotonic tick counter incremented by IRQ0
uint32_t timer_get_ticks(void);

// Busy-wait for specified tick count using HLT instruction
void timer_sleep(uint32_t ticks);

// ISR callback invoked by IRQ0 handler
void timer_handler(void);

// Returns elapsed time in seconds since timer initialization
uint32_t timer_get_uptime_seconds(void);

#endif // TIMER_H
