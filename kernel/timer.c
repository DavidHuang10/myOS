#include "include/timer.h"
#include "include/port_io.h"
#include "include/idt.h"

// Incremented atomically on each IRQ0, must be volatile for ISR visibility
static volatile uint32_t timer_ticks = 0;
static uint32_t timer_frequency = 0;

// IRQ0 callback - increment tick counter
void timer_handler(void) {
    timer_ticks++;
}

// Configure PIT channel 0 for periodic interrupts at specified Hz
void timer_init(uint32_t frequency) {
    timer_frequency = frequency;

    // Calculate divisor for desired frequency
    uint32_t divisor = PIT_FREQUENCY / frequency;

    // Clamp divisor to valid 16-bit range
    if (divisor > 65535) {
        divisor = 65535;
    }
    if (divisor < 1) {
        divisor = 1;
    }

    // Configure PIT: channel 0, lobyte/hibyte access, mode 3 (square wave), binary
    // Bits [7:6]=00 (channel 0), [5:4]=11 (lobyte/hibyte), [3:1]=011 (mode 3), [0]=0 (binary)
    outb(PIT_COMMAND, 0x36);

    // Write 16-bit divisor as two 8-bit writes (LSB first)
    outb(PIT_CHANNEL0, (uint8_t)(divisor & 0xFF));
    outb(PIT_CHANNEL0, (uint8_t)((divisor >> 8) & 0xFF));

    // Unmask IRQ0 in PIC to enable timer interrupts
    pic_clear_mask(0);
}

// Atomic read of tick counter
uint32_t timer_get_ticks(void) {
    return timer_ticks;
}

// Convert ticks to seconds using configured frequency
uint32_t timer_get_uptime_seconds(void) {
    if (timer_frequency == 0) {
        return 0;
    }
    return timer_ticks / timer_frequency;
}

// Sleep by halting CPU until target tick count reached
void timer_sleep(uint32_t ticks) {
    uint32_t end_tick = timer_ticks + ticks;

    // HLT reduces power consumption while waiting for IRQ
    while (timer_ticks < end_tick) {
        asm volatile("hlt");
    }
}
