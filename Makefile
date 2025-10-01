# MyOS Makefile - Enhanced Build System

# Target ISO name
TARGET = myos.iso
ISO_DIR = isodir
GRUB_CFG = $(ISO_DIR)/boot/grub/grub.cfg

# Object file directory
BUILD_DIR = build

# Compiler and tools - using cross-compiler toolchain
AS = x86_64-elf-as --32
CC = x86_64-elf-gcc
LD = x86_64-elf-ld
CFLAGS = -m32 -ffreestanding -nostdlib -nostartfiles -nodefaultlibs -Wall -Wextra -O2
LDFLAGS = -m elf_i386 -T boot/linker.ld

# Source files
ASM_SOURCES = boot/boot.s kernel/isr.s kernel/gdt_flush.s
C_SOURCES = $(wildcard kernel/*.c)

# Object files (in build directory)
ASM_OBJECTS = $(patsubst %.s, $(BUILD_DIR)/%.o, $(notdir $(ASM_SOURCES)))
C_OBJECTS = $(patsubst kernel/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Kernel binary
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

# Default target
all: $(TARGET)

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Compile C files
$(BUILD_DIR)/%.o: kernel/%.c | $(BUILD_DIR)
	@echo "CC $<"
	@$(CC) $(CFLAGS) -c -o $@ $<

# Assemble boot.s
$(BUILD_DIR)/boot.o: boot/boot.s | $(BUILD_DIR)
	@echo "AS $<"
	@$(AS) -o $@ $<

# Assemble ISR stubs
$(BUILD_DIR)/isr.o: kernel/isr.s | $(BUILD_DIR)
	@echo "AS $<"
	@$(AS) -o $@ $<

# Assemble GDT flush
$(BUILD_DIR)/gdt_flush.o: kernel/gdt_flush.s | $(BUILD_DIR)
	@echo "AS $<"
	@$(AS) -o $@ $<

# Link kernel binary
$(KERNEL_BIN): $(OBJECTS)
	@echo "LD $@"
	@$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

# Set up ISO directory structure
$(ISO_DIR)/boot/kernel.bin: $(KERNEL_BIN)
	@echo "INSTALL $@"
	@mkdir -p $(ISO_DIR)/boot/grub
	@cp $(KERNEL_BIN) $(ISO_DIR)/boot/kernel.bin

# Create grub.cfg
$(GRUB_CFG):
	@echo "GEN $@"
	@mkdir -p $(ISO_DIR)/boot/grub
	@echo 'menuentry "myos" { multiboot /boot/kernel.bin }' > $(GRUB_CFG)

# Build ISO image
$(TARGET): $(ISO_DIR)/boot/kernel.bin $(GRUB_CFG)
	@echo "ISO $@"
	@i686-elf-grub-mkrescue -o $(TARGET) $(ISO_DIR) 2>/dev/null

# Run in QEMU
run: $(TARGET)
	@echo "Starting QEMU..."
	@qemu-system-x86_64 -cdrom $(TARGET)

# Run with debugging
debug: $(TARGET)
	@echo "Starting QEMU with debugger..."
	@qemu-system-x86_64 -cdrom $(TARGET) -s -S

# Clean build artifacts
clean:
	@echo "CLEAN"
	@rm -rf $(BUILD_DIR)
	@rm -f $(TARGET)
	@rm -rf $(ISO_DIR)

# Clean and rebuild
rebuild: clean all

# Show build info
info:
	@echo "MyOS Build System"
	@echo "================="
	@echo "C Sources:   $(C_SOURCES)"
	@echo "ASM Sources: $(ASM_SOURCES)"
	@echo "Objects:     $(OBJECTS)"
	@echo "Kernel:      $(KERNEL_BIN)"
	@echo "ISO:         $(TARGET)"

# Compile with debug symbols
debug-build: CFLAGS += -g
debug-build: clean all

# Generate assembly listings
listings: $(C_SOURCES)
	@mkdir -p $(BUILD_DIR)/listings
	@for src in $(C_SOURCES); do \
		base=$$(basename $$src .c); \
		echo "Generating $$base.s..."; \
		$(CC) $(CFLAGS) -S -o $(BUILD_DIR)/listings/$$base.s $$src; \
	done
	@echo "Assembly listings generated in $(BUILD_DIR)/listings/"

# Dependency generation
depend: $(C_SOURCES)
	@echo "Generating dependencies..."
	@$(CC) $(CFLAGS) -MM $(C_SOURCES) > $(BUILD_DIR)/depend.mk

# Include dependencies if they exist
-include $(BUILD_DIR)/depend.mk

# Help target
help:
	@echo "MyOS Build System"
	@echo "================="
	@echo ""
	@echo "Targets:"
	@echo "  all          - Build the OS (default)"
	@echo "  clean        - Remove build artifacts"
	@echo "  rebuild      - Clean and build"
	@echo "  run          - Build and run in QEMU"
	@echo "  debug        - Build and run in QEMU with GDB server"
	@echo "  debug-build  - Build with debug symbols"
	@echo "  listings     - Generate assembly listings"
	@echo "  depend       - Generate dependency file"
	@echo "  info         - Show build configuration"
	@echo "  help         - Show this help message"
	@echo ""
	@echo "Examples:"
	@echo "  make              # Build OS"
	@echo "  make run          # Build and run"
	@echo "  make clean all    # Clean build"
	@echo "  make debug-build  # Build with debug info"

.PHONY: all clean rebuild run debug debug-build listings depend info help
