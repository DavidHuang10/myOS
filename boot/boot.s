# Multiboot header
.section .multiboot
    .align 4
    .long 0x1BADB002               # magic number for Multiboot
    .long 0x0                      # flags (we use 0 here for simplicity)
    .long -(0x1BADB002)            # checksum to make the header zero

# Entry point
.section .text
.global _start
_start:
    mov $kernel_main, %eax         # Load address of kernel_main
    call *%eax                     # Call kernel_main

halt:
    hlt                            # Halt the CPU
    jmp halt                       # Infinite loop

