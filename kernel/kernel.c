#include <stdint.h>
#include "include/gdt.h"
#include "include/idt.h"
#include "include/print.h"
#include "include/keyboard.h"
#include "include/timer.h"
#include "include/shell.h"

// Main kernel function
void kernel_main() {
    // Initialize terminal
    terminal_initialize();

    // Initialize GDT
    gdt_init();

    // Initialize IDT and PIC
    idt_init();

    // Initialize timer (100 Hz = 100 ticks per second)
    timer_init(100);

    // Initialize keyboard
    keyboard_init();

    // Initialize and run shell
    shell_init();
    shell_run();

    // Should never reach here
    while (1) {
        asm volatile("hlt");
    }
}

