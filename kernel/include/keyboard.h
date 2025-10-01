#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

// Keyboard ports
#define KEYBOARD_DATA_PORT    0x60
#define KEYBOARD_STATUS_PORT  0x64

// Keyboard buffer size
#define KEYBOARD_BUFFER_SIZE  256

// Special keys
#define KEY_BACKSPACE  '\b'
#define KEY_TAB        '\t'
#define KEY_ENTER      '\n'
#define KEY_ESCAPE     0x1B
#define KEY_DELETE     0x7F

// Initialize keyboard driver
void keyboard_init(void);

// Get character from keyboard buffer (blocking)
char keyboard_getchar(void);

// Check if keyboard buffer has data
bool keyboard_has_data(void);

// Keyboard interrupt handler (called from ISR)
void keyboard_handler(void);

#endif // KEYBOARD_H
