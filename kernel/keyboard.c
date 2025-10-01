#include "include/keyboard.h"
#include "include/port_io.h"
#include "include/print.h"
#include "include/idt.h"

// Keyboard state
static bool shift_pressed = false;
static bool caps_lock = false;
static bool ctrl_pressed = false;
static bool alt_pressed = false;

// Circular keyboard buffer
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static uint32_t buffer_read_pos = 0;
static uint32_t buffer_write_pos = 0;

// US QWERTY scancode to ASCII translation table
// Index = scancode, Value = ASCII character (lowercase)
static const char scancode_to_ascii[128] = {
    0,    0,   '1',  '2',  '3',  '4',  '5',  '6',   // 0x00-0x07
    '7',  '8', '9',  '0',  '-',  '=',  '\b', '\t',  // 0x08-0x0F
    'q',  'w', 'e',  'r',  't',  'y',  'u',  'i',   // 0x10-0x17
    'o',  'p', '[',  ']',  '\n', 0,    'a',  's',   // 0x18-0x1F
    'd',  'f', 'g',  'h',  'j',  'k',  'l',  ';',   // 0x20-0x27
    '\'', '`', 0,    '\\', 'z',  'x',  'c',  'v',   // 0x28-0x2F
    'b',  'n', 'm',  ',',  '.',  '/',  0,    '*',   // 0x30-0x37
    0,    ' ', 0,    0,    0,    0,    0,    0,     // 0x38-0x3F
    0,    0,   0,    0,    0,    0,    0,    '7',   // 0x40-0x47 (Function keys + Keypad)
    '8',  '9', '-',  '4',  '5',  '6',  '+',  '1',   // 0x48-0x4F (Keypad)
    '2',  '3', '0',  '.',  0,    0,    0,    0,     // 0x50-0x57 (Keypad)
    0,    0,   0,    0,    0,    0,    0,    0,     // 0x58-0x5F
    0,    0,   0,    0,    0,    0,    0,    0,     // 0x60-0x67
    0,    0,   0,    0,    0,    0,    0,    0,     // 0x68-0x6F
    0,    0,   0,    0,    0,    0,    0,    0,     // 0x70-0x77
    0,    0,   0,    0,    0,    0,    0,    0      // 0x78-0x7F
};

// Shifted characters (when shift is held)
static const char scancode_to_ascii_shift[128] = {
    0,    0,   '!',  '@',  '#',  '$',  '%',  '^',   // 0x00-0x07
    '&',  '*', '(',  ')',  '_',  '+',  '\b', '\t',  // 0x08-0x0F
    'Q',  'W', 'E',  'R',  'T',  'Y',  'U',  'I',   // 0x10-0x17
    'O',  'P', '{',  '}',  '\n', 0,    'A',  'S',   // 0x18-0x1F
    'D',  'F', 'G',  'H',  'J',  'K',  'L',  ':',   // 0x20-0x27
    '"',  '~', 0,    '|',  'Z',  'X',  'C',  'V',   // 0x28-0x2F
    'B',  'N', 'M',  '<',  '>',  '?',  0,    '*',   // 0x30-0x37
    0,    ' ', 0,    0,    0,    0,    0,    0,     // 0x38-0x3F
    0,    0,   0,    0,    0,    0,    0,    '7',   // 0x40-0x47
    '8',  '9', '-',  '4',  '5',  '6',  '+',  '1',   // 0x48-0x4F
    '2',  '3', '0',  '.',  0,    0,    0,    0,     // 0x50-0x57
    0,    0,   0,    0,    0,    0,    0,    0,     // 0x58-0x5F
    0,    0,   0,    0,    0,    0,    0,    0,     // 0x60-0x67
    0,    0,   0,    0,    0,    0,    0,    0,     // 0x68-0x6F
    0,    0,   0,    0,    0,    0,    0,    0,     // 0x70-0x77
    0,    0,   0,    0,    0,    0,    0,    0      // 0x78-0x7F
};

// Scancode definitions
#define SC_LSHIFT_PRESS   0x2A
#define SC_RSHIFT_PRESS   0x36
#define SC_LSHIFT_RELEASE 0xAA
#define SC_RSHIFT_RELEASE 0xB6
#define SC_CTRL_PRESS     0x1D
#define SC_CTRL_RELEASE   0x9D
#define SC_ALT_PRESS      0x38
#define SC_ALT_RELEASE    0xB8
#define SC_CAPSLOCK       0x3A

// Add character to keyboard buffer
static void buffer_add(char c) {
    uint32_t next_pos = (buffer_write_pos + 1) % KEYBOARD_BUFFER_SIZE;
    if (next_pos != buffer_read_pos) {  // Buffer not full
        keyboard_buffer[buffer_write_pos] = c;
        buffer_write_pos = next_pos;
    }
}

// Check if buffer has data
bool keyboard_has_data(void) {
    return buffer_read_pos != buffer_write_pos;
}

// Get character from buffer (blocking)
char keyboard_getchar(void) {
    // Wait for data
    while (!keyboard_has_data()) {
        asm volatile("hlt");  // Halt until interrupt
    }

    // Read from buffer
    char c = keyboard_buffer[buffer_read_pos];
    buffer_read_pos = (buffer_read_pos + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

// Keyboard interrupt handler
void keyboard_handler(void) {
    // Read scancode from keyboard
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    // Check for key release (bit 7 set)
    bool key_released = (scancode & 0x80) != 0;
    uint8_t key_code = scancode & 0x7F;

    // Handle modifier keys
    if (scancode == SC_LSHIFT_PRESS || scancode == SC_RSHIFT_PRESS) {
        shift_pressed = true;
        return;
    }
    if (scancode == SC_LSHIFT_RELEASE || scancode == SC_RSHIFT_RELEASE) {
        shift_pressed = false;
        return;
    }
    if (scancode == SC_CTRL_PRESS) {
        ctrl_pressed = true;
        return;
    }
    if (scancode == SC_CTRL_RELEASE) {
        ctrl_pressed = false;
        return;
    }
    if (scancode == SC_ALT_PRESS) {
        alt_pressed = true;
        return;
    }
    if (scancode == SC_ALT_RELEASE) {
        alt_pressed = false;
        return;
    }
    if (scancode == SC_CAPSLOCK && !key_released) {
        caps_lock = !caps_lock;  // Toggle caps lock
        return;
    }

    // Ignore key releases for regular keys
    if (key_released) {
        return;
    }

    // Translate scancode to ASCII
    char ascii = 0;

    if (shift_pressed) {
        ascii = scancode_to_ascii_shift[key_code];
    } else {
        ascii = scancode_to_ascii[key_code];

        // Apply caps lock to letters
        if (caps_lock && ascii >= 'a' && ascii <= 'z') {
            ascii = ascii - 'a' + 'A';
        }
    }

    // If we got a valid ASCII character, add to buffer and echo
    if (ascii != 0) {
        buffer_add(ascii);

        // Echo character to screen
        if (ascii == '\b') {
            terminal_backspace();
        } else {
            terminal_putchar(ascii);
        }
    }
}

// Initialize keyboard
void keyboard_init(void) {
    // Clear buffer
    buffer_read_pos = 0;
    buffer_write_pos = 0;

    // Reset keyboard state
    shift_pressed = false;
    caps_lock = false;
    ctrl_pressed = false;
    alt_pressed = false;

    // Enable keyboard IRQ (IRQ1)
    pic_clear_mask(1);
}
