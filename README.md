# MyOS

MyOS is a simple educational operating system developed for learning purposes. It serves as a foundation for understanding kernel development and the basics of OS architecture. This project showcases how to set up a minimal bootloader, kernel, and the structure necessary to build an ISO image that can be booted using GRUB.

## Table of Contents

- [Features](#features)
- [Getting Started](#getting-started)
- [Directory Structure](#directory-structure)
- [Building MyOS](#building-myos)
- [Running MyOS](#running-myos)
- [License](#license)

## Features

- Basic kernel functionality with an infinite loop for testing.
- Multiboot-compliant bootloader.
- Simple structure to build an ISO image for booting.
- Clean separation of assembly and C code.

## Getting Started

To get started with MyOS, ensure you have the following tools installed on your system:

- `gcc` (GNU Compiler Collection)
- `nasm` (Netwide Assembler)
- `grub` (GRand Unified Bootloader)
- `make` (Build automation tool)

### Prerequisites

Make sure to install the necessary dependencies. On Ubuntu, you can use:

```bash
sudo apt-get update
sudo apt-get install build-essential nasm grub2
```

## Directory Structure

The project is organized as follows:

```
.
├── boot
│   ├── boot.s        # Assembly code for bootloader
│   └── linker.ld     # Linker script for defining memory layout
├── isodir            # Directory structure for ISO image
│   └── boot
│       └── grub
│           └── grub.cfg  # GRUB configuration file
├── kernel
│   ├── idt.c         # Interrupt Descriptor Table implementation
│   ├── include       # Header files for the kernel
│   ├── kernel.c      # Main kernel code
│   ├── keyboard.c    # Keyboard handling code
│   ├── port_io.c     # Port I/O operations
│   └── print.c       # Print functions for kernel
├── Makefile          # Build script for the project
├── myos.iso          # Output ISO file
└── README.md         # This README file
```

## Building MyOS

To build the operating system, simply run the following command in the terminal:

```bash
make
```

This command will compile the necessary files, link them, and create an ISO image named `myos.iso`.

## Running MyOS

Once the build process is complete, you can run MyOS in a virtual machine. For example, using QEMU, run:

```bash
qemu-system-x86_64 -cdrom myos.iso
```

This command will boot your operating system in a QEMU emulator.

## License

This project is open-source and available under the [MIT License](LICENSE).
