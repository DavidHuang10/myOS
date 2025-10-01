#ifndef SHELL_H
#define SHELL_H

// Command buffer size
#define SHELL_BUFFER_SIZE 256

// Initialize shell
void shell_init(void);

// Run shell (main loop)
void shell_run(void);

// Display shell prompt
void shell_prompt(void);

// Process command
void shell_process_command(const char* cmd);

#endif // SHELL_H
