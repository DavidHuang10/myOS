When coding, make sure comments seem natural and non-obvious. make them technical.

# MyOS - Project Context

## Overview
MyOS is a simple educational operating system written in C and x86 assembly. It's designed for learning kernel development and OS architecture fundamentals.

## Build System
- Uses a custom Makefile for building
- Target architecture: x86 32-bit
- Bootloader: GRUB (multiboot-compliant)
- Assembler: GNU `as` (32-bit mode)
- Compiler: GCC with `-m32` flag
- Linker: GNU `ld` (elf_i386)

## Project Structure
- `boot/` - Bootloader assembly code and linker script
  - `boot.s` - Assembly bootloader entry point
  - `linker.ld` - Memory layout linker script
- `kernel/` - Kernel C source files
  - `kernel.c` - Main kernel entry point
  - `idt.c` - Interrupt Descriptor Table
  - `keyboard.c` - Keyboard driver
  - `port_io.c` - I/O port operations
  - `print.c` - Screen output functions
  - `include/` - Header files
- `isodir/` - Generated ISO directory structure (ignored in git)

## Build Process
1. Assemble `boot.o` from `boot/boot.s`
2. Compile `kernel.o` from `kernel/kernel.c`
3. Link `boot.o` and `kernel.o` into `kernel.bin`
4. Copy `kernel.bin` to ISO directory structure
5. Generate GRUB config
6. Create bootable ISO with `grub-mkrescue`

## Key Flags
- `-ffreestanding` - Freestanding environment (no standard library)
- `-nostdlib -nostartfiles -nodefaultlibs` - No standard C runtime
- `--32` / `-m32` - 32-bit x86 architecture
- `-m elf_i386` - 32-bit ELF format

## Testing
Run with QEMU: `qemu-system-x86_64 -cdrom myos.iso`

## Important Notes
- All code must be 32-bit x86 compatible
- No standard library functions available
- Direct hardware access via port I/O
- Multiboot specification compliance required for GRUB
