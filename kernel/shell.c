#include "include/shell.h"
#include "include/print.h"
#include "include/keyboard.h"
#include "include/string.h"
#include "include/timer.h"

// Shell state
static char command_buffer[SHELL_BUFFER_SIZE];
static uint32_t buffer_pos = 0;

// Command history (optional)
#define HISTORY_SIZE 10
static char command_history[HISTORY_SIZE][SHELL_BUFFER_SIZE];
static uint32_t history_pos = 0;
static uint32_t history_count = 0;

// Display shell prompt
void shell_prompt(void) {
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    terminal_writestring("myos");
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK));
    terminal_writestring(" > ");
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

// Display welcome banner
static void shell_banner(void) {
    terminal_clear();
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    terminal_writestring("=====================================\n");
    terminal_writestring("     Welcome to MyOS v0.1\n");
    terminal_writestring("   A Simple Educational Operating System\n");
    terminal_writestring("=====================================\n\n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_writestring("Type 'help' for available commands.\n\n");
}

// Command: help
static void cmd_help(void) {
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK));
    terminal_writestring("Available commands:\n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_writestring("  help           - Show this help message\n");
    terminal_writestring("  clear          - Clear the screen\n");
    terminal_writestring("  echo <text>    - Print text to screen\n");
    terminal_writestring("  color <0-15>   - Change text color\n");
    terminal_writestring("  uptime         - Show system uptime\n");
    terminal_writestring("  clock          - Display ticking clock\n");
    terminal_writestring("  exception      - Test exception handling (CAUTION)\n");
    terminal_writestring("  about          - About MyOS\n");
    terminal_writestring("  test           - Run test commands\n");
}

// Command: clear
static void cmd_clear(void) {
    terminal_clear();
}

// Command: echo
static void cmd_echo(const char* args) {
    if (args && strlen(args) > 0) {
        terminal_writestring(args);
        terminal_writestring("\n");
    } else {
        terminal_writestring("\n");
    }
}

// Command: color
static void cmd_color(const char* args) {
    if (args && strlen(args) > 0) {
        int color = atoi(args);
        if (color >= 0 && color <= 15) {
            terminal_setcolor(vga_entry_color(color, VGA_COLOR_BLACK));
            terminal_writestring("Color changed to ");
            char buffer[8];
            itoa(color, buffer, 10);
            terminal_writestring(buffer);
            terminal_writestring("\n");
        } else {
            terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
            terminal_writestring("Error: Color must be 0-15\n");
            terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
        }
    } else {
        terminal_writestring("Usage: color <0-15>\n");
        terminal_writestring("Colors: 0=Black, 1=Blue, 2=Green, 3=Cyan, 4=Red,\n");
        terminal_writestring("        5=Magenta, 6=Brown, 7=LightGrey, 8=DarkGrey,\n");
        terminal_writestring("        9=LightBlue, 10=LightGreen, 11=LightCyan,\n");
        terminal_writestring("        12=LightRed, 13=LightMagenta, 14=Yellow, 15=White\n");
    }
}

// Command: uptime
static void cmd_uptime(void) {
    uint32_t seconds = timer_get_uptime_seconds();
    uint32_t ticks = timer_get_ticks();

    terminal_writestring("System uptime:\n");

    // Calculate hours, minutes, seconds
    uint32_t hours = seconds / 3600;
    uint32_t minutes = (seconds % 3600) / 60;
    uint32_t secs = seconds % 60;

    // Display time
    char buffer[16];
    itoa(hours, buffer, 10);
    terminal_writestring("  ");
    terminal_writestring(buffer);
    terminal_writestring(" hours, ");

    itoa(minutes, buffer, 10);
    terminal_writestring(buffer);
    terminal_writestring(" minutes, ");

    itoa(secs, buffer, 10);
    terminal_writestring(buffer);
    terminal_writestring(" seconds\n");

    // Display total ticks
    terminal_writestring("  Total ticks: ");
    itoa(ticks, buffer, 10);
    terminal_writestring(buffer);
    terminal_writestring("\n");
}

// Command: about
static void cmd_about(void) {
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    terminal_writestring("\nMyOS - Educational Operating System\n");
    terminal_writestring("=====================================\n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_writestring("Version: 0.1\n");
    terminal_writestring("Architecture: x86 (32-bit)\n");
    terminal_writestring("Boot: Multiboot/GRUB\n");
    terminal_writestring("\nFeatures:\n");
    terminal_writestring("  - VGA text mode driver\n");
    terminal_writestring("  - Keyboard input\n");
    terminal_writestring("  - Interrupt handling (IDT/PIC)\n");
    terminal_writestring("  - Basic shell\n\n");
}

// Command: clock
static void cmd_clock(void) {
    terminal_writestring("Displaying clock for 10 seconds...\n");
    terminal_writestring("Press Ctrl+C to stop (not implemented yet)\n\n");

    uint32_t start_tick = timer_get_ticks();
    uint32_t last_second = 0;

    while (timer_get_ticks() - start_tick < 1000) {  // 10 seconds at 100 Hz
        uint32_t current_second = timer_get_uptime_seconds();

        if (current_second != last_second) {
            last_second = current_second;

            // Move cursor to beginning of line (print \r)
            terminal_writestring("\rUptime: ");

            char buffer[16];
            itoa(current_second, buffer, 10);
            terminal_writestring(buffer);
            terminal_writestring(" seconds   ");
        }

        // Small delay
        timer_sleep(10);  // Sleep for 0.1 seconds
    }

    terminal_writestring("\n\nClock display ended.\n");
}

// Command: exception (test exception handlers)
static void cmd_exception(const char* args) {
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
    terminal_writestring("WARNING: This will trigger a kernel panic!\n");
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));

    if (!args || strlen(args) == 0) {
        terminal_writestring("Usage: exception <type>\n");
        terminal_writestring("Types:\n");
        terminal_writestring("  div0    - Division by zero\n");
        terminal_writestring("  gpf     - General protection fault\n");
        terminal_writestring("  invop   - Invalid opcode\n");
        return;
    }

    terminal_writestring("Triggering exception in 1 second...\n");
    timer_sleep(100);  // 1 second at 100 Hz

    if (strcmp(args, "div0") == 0) {
        // Trigger division by zero exception
        volatile int x = 1;
        volatile int y = 0;
        volatile int z = x / y;
        (void)z;  // Suppress unused warning
    } else if (strcmp(args, "gpf") == 0) {
        // Trigger general protection fault via invalid segment load
        asm volatile("mov $0xFFFF, %ax; mov %ax, %ds");
    } else if (strcmp(args, "invop") == 0) {
        // Trigger invalid opcode using UD2 instruction
        asm volatile("ud2");
    } else {
        terminal_writestring("Unknown exception type\n");
    }
}

// Command: test
static void cmd_test(void) {
    terminal_writestring("Running tests...\n");

    // Test colors
    terminal_writestring("Color test: ");
    for (int i = 1; i <= 15; i++) {
        terminal_setcolor(vga_entry_color(i, VGA_COLOR_BLACK));
        terminal_writestring("*");
    }
    terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    terminal_writestring("\n");

    // Test string functions
    terminal_writestring("String test: ");
    char buffer[32];
    itoa(42, buffer, 10);
    terminal_writestring("itoa(42) = ");
    terminal_writestring(buffer);
    terminal_writestring("\n");

    terminal_writestring("Tests complete!\n");
}

// Parse and execute command
void shell_process_command(const char* cmd) {
    // Trim whitespace
    char trimmed_cmd[SHELL_BUFFER_SIZE];
    strcpy(trimmed_cmd, cmd);
    char* trimmed = strtrim(trimmed_cmd);

    // Empty command
    if (strlen(trimmed) == 0) {
        return;
    }

    // Add to history
    if (history_count < HISTORY_SIZE) {
        strcpy(command_history[history_count], trimmed);
        history_count++;
    }

    // Parse command and arguments
    char* args = 0;
    for (uint32_t i = 0; trimmed[i] != '\0'; i++) {
        if (trimmed[i] == ' ') {
            trimmed[i] = '\0';
            args = &trimmed[i + 1];
            // Skip leading spaces in args
            while (*args == ' ') {
                args++;
            }
            break;
        }
    }

    // Execute command
    if (strcmp(trimmed, "help") == 0) {
        cmd_help();
    } else if (strcmp(trimmed, "clear") == 0) {
        cmd_clear();
    } else if (strcmp(trimmed, "echo") == 0) {
        cmd_echo(args);
    } else if (strcmp(trimmed, "color") == 0) {
        cmd_color(args);
    } else if (strcmp(trimmed, "uptime") == 0) {
        cmd_uptime();
    } else if (strcmp(trimmed, "clock") == 0) {
        cmd_clock();
    } else if (strcmp(trimmed, "exception") == 0) {
        cmd_exception(args);
    } else if (strcmp(trimmed, "about") == 0) {
        cmd_about();
    } else if (strcmp(trimmed, "test") == 0) {
        cmd_test();
    } else {
        terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK));
        terminal_writestring("Unknown command: ");
        terminal_writestring(trimmed);
        terminal_writestring("\n");
        terminal_writestring("Type 'help' for available commands.\n");
        terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    }
}

// Initialize shell
void shell_init(void) {
    // Clear command buffer
    buffer_pos = 0;
    memset(command_buffer, 0, SHELL_BUFFER_SIZE);

    // Display banner
    shell_banner();

    // Display first prompt
    shell_prompt();
}

// Main shell loop
void shell_run(void) {
    while (1) {
        // Get character from keyboard
        char c = keyboard_getchar();

        // Handle special keys
        if (c == '\n') {
            // Enter - execute command
            terminal_writestring("\n");
            command_buffer[buffer_pos] = '\0';

            // Process command
            shell_process_command(command_buffer);

            // Reset buffer
            buffer_pos = 0;
            memset(command_buffer, 0, SHELL_BUFFER_SIZE);

            // Show prompt
            shell_prompt();
        } else if (c == '\b') {
            // Backspace - already handled by keyboard echo
            if (buffer_pos > 0) {
                buffer_pos--;
                command_buffer[buffer_pos] = '\0';
            }
        } else if (buffer_pos < SHELL_BUFFER_SIZE - 1) {
            // Regular character - add to buffer
            command_buffer[buffer_pos] = c;
            buffer_pos++;
        } else {
            // Buffer full - beep or ignore
            // Could add beep sound here if PC speaker implemented
        }
    }
}
