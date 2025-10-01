#include "include/print.h"
#include "include/port_io.h"

// Terminal state
static uint8_t terminal_row;
static uint8_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

// VGA cursor control ports
#define VGA_CTRL_REGISTER 0x3D4
#define VGA_DATA_REGISTER 0x3D5
#define VGA_CURSOR_HIGH   0x0E
#define VGA_CURSOR_LOW    0x0F

// Update hardware cursor position
static void update_cursor(uint8_t x, uint8_t y) {
    uint16_t pos = y * VGA_WIDTH + x;

    outb(VGA_CTRL_REGISTER, VGA_CURSOR_LOW);
    outb(VGA_DATA_REGISTER, (uint8_t)(pos & 0xFF));

    outb(VGA_CTRL_REGISTER, VGA_CURSOR_HIGH);
    outb(VGA_DATA_REGISTER, (uint8_t)((pos >> 8) & 0xFF));
}

// Initialize terminal
void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*)VGA_MEMORY;

    // Clear screen
    terminal_clear();
}

// Clear entire screen
void terminal_clear(void) {
    for (uint8_t y = 0; y < VGA_HEIGHT; y++) {
        for (uint8_t x = 0; x < VGA_WIDTH; x++) {
            const uint32_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;
    update_cursor(terminal_column, terminal_row);
}

// Set text color
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Put character at specific position
void terminal_putentryat(char c, uint8_t color, uint8_t x, uint8_t y) {
    const uint32_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

// Scroll screen up by one line
void terminal_scroll(void) {
    // Move all lines up by one
    for (uint8_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (uint8_t x = 0; x < VGA_WIDTH; x++) {
            uint32_t src_index = (y + 1) * VGA_WIDTH + x;
            uint32_t dst_index = y * VGA_WIDTH + x;
            terminal_buffer[dst_index] = terminal_buffer[src_index];
        }
    }

    // Clear last line
    for (uint8_t x = 0; x < VGA_WIDTH; x++) {
        uint32_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
}

// Handle backspace
void terminal_backspace(void) {
    if (terminal_column > 0) {
        terminal_column--;
    } else if (terminal_row > 0) {
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
    }

    // Clear character at current position
    terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
    update_cursor(terminal_column, terminal_row);
}

// Put character at current cursor position
void terminal_putchar(char c) {
    // Handle special characters
    if (c == '\n') {
        // Newline
        terminal_column = 0;
        if (++terminal_row >= VGA_HEIGHT) {
            terminal_scroll();
            terminal_row = VGA_HEIGHT - 1;
        }
    } else if (c == '\r') {
        // Carriage return
        terminal_column = 0;
    } else if (c == '\t') {
        // Tab (4 spaces)
        for (int i = 0; i < 4; i++) {
            terminal_putchar(' ');
        }
        return;  // Cursor already updated by recursive calls
    } else if (c == '\b') {
        // Backspace
        terminal_backspace();
        return;
    } else {
        // Regular character
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);

        if (++terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row >= VGA_HEIGHT) {
                terminal_scroll();
                terminal_row = VGA_HEIGHT - 1;
            }
        }
    }

    update_cursor(terminal_column, terminal_row);
}

// Write data of specific size
void terminal_write(const char* data, uint32_t size) {
    for (uint32_t i = 0; i < size; i++) {
        terminal_putchar(data[i]);
    }
}

// Write null-terminated string
void terminal_writestring(const char* data) {
    uint32_t i = 0;
    while (data[i] != '\0') {
        terminal_putchar(data[i]);
        i++;
    }
}

// Legacy function for backward compatibility
void print_string(const char *str) {
    terminal_writestring(str);
}

