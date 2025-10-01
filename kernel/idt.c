#include "include/idt.h"
#include "include/port_io.h"
#include "include/keyboard.h"
#include "include/timer.h"
#include "include/print.h"
#include "include/string.h"

// IDT with 256 entries
#define IDT_ENTRIES 256
struct idt_entry idt[IDT_ENTRIES];
struct idt_ptr idt_pointer;

// CPU exception names for error reporting
static const char* exception_messages[32] = {
    "Division By Zero",
    "Debug",
    "Non-Maskable Interrupt",
    "Breakpoint",
    "Overflow",
    "Bound Range Exceeded",
    "Invalid Opcode",
    "Device Not Available",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Invalid TSS",
    "Segment Not Present",
    "Stack-Segment Fault",
    "General Protection Fault",
    "Page Fault",
    "Reserved",
    "x87 Floating-Point Exception",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Virtualization Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Security Exception",
    "Reserved"
};

// External assembly function to load IDT
extern void idt_flush(uint32_t);

// ISR function pointers (declared in assembly)
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

// IRQ handlers (hardware interrupts, remapped to 32-47)
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

// Set an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = base & 0xFFFF;
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    idt[num].sel = sel;
    idt[num].zero = 0;
    idt[num].flags = flags;
}

// Initialize IDT
void idt_init(void) {
    idt_pointer.limit = (sizeof(struct idt_entry) * IDT_ENTRIES) - 1;
    idt_pointer.base = (uint32_t)&idt;

    // Clear IDT
    for (int i = 0; i < IDT_ENTRIES; i++) {
        idt_set_gate(i, 0, 0, 0);
    }

    // Set up CPU exception handlers (0-31)
    idt_set_gate(0, (uint32_t)isr0, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(1, (uint32_t)isr1, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(2, (uint32_t)isr2, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(3, (uint32_t)isr3, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(4, (uint32_t)isr4, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(5, (uint32_t)isr5, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(6, (uint32_t)isr6, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(7, (uint32_t)isr7, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(8, (uint32_t)isr8, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(9, (uint32_t)isr9, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(10, (uint32_t)isr10, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(11, (uint32_t)isr11, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(12, (uint32_t)isr12, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(13, (uint32_t)isr13, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(14, (uint32_t)isr14, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(15, (uint32_t)isr15, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(16, (uint32_t)isr16, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(17, (uint32_t)isr17, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(18, (uint32_t)isr18, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(19, (uint32_t)isr19, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(20, (uint32_t)isr20, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(21, (uint32_t)isr21, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(22, (uint32_t)isr22, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(23, (uint32_t)isr23, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(24, (uint32_t)isr24, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(25, (uint32_t)isr25, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(26, (uint32_t)isr26, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(27, (uint32_t)isr27, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(28, (uint32_t)isr28, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(29, (uint32_t)isr29, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(30, (uint32_t)isr30, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(31, (uint32_t)isr31, 0x08, IDT_FLAGS_KERNEL_INT);

    // Set up IRQ handlers (32-47) - after PIC remapping
    idt_set_gate(32, (uint32_t)irq0, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(33, (uint32_t)irq1, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(34, (uint32_t)irq2, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(35, (uint32_t)irq3, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(36, (uint32_t)irq4, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(37, (uint32_t)irq5, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(38, (uint32_t)irq6, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(39, (uint32_t)irq7, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(40, (uint32_t)irq8, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(41, (uint32_t)irq9, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(42, (uint32_t)irq10, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(43, (uint32_t)irq11, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(44, (uint32_t)irq12, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(45, (uint32_t)irq13, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(46, (uint32_t)irq14, 0x08, IDT_FLAGS_KERNEL_INT);
    idt_set_gate(47, (uint32_t)irq15, 0x08, IDT_FLAGS_KERNEL_INT);

    // Load IDT
    idt_flush((uint32_t)&idt_pointer);

    // Initialize and remap PIC
    pic_init();

    // Enable interrupts
    asm volatile("sti");
}

// Common ISR handler (called from assembly) - handles CPU exceptions
void isr_handler(uint32_t int_no, uint32_t err_code) {
    // Display kernel panic in white on red background
    terminal_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
    terminal_writestring("\n\n");
    terminal_writestring("================== KERNEL PANIC ==================\n");

    // Display exception name
    terminal_writestring("Exception: ");
    if (int_no < 32) {
        terminal_writestring(exception_messages[int_no]);
    } else {
        terminal_writestring("Unknown");
    }
    terminal_writestring("\n");

    // Display vector number in decimal and hex
    terminal_writestring("Vector: ");
    char buffer[16];
    itoa(int_no, buffer, 10);
    terminal_writestring(buffer);
    terminal_writestring(" (0x");
    itoa(int_no, buffer, 16);
    terminal_writestring(buffer);
    terminal_writestring(")\n");

    // Display error code
    terminal_writestring("Error Code: 0x");
    itoa(err_code, buffer, 16);
    terminal_writestring(buffer);
    terminal_writestring("\n");

    // Exception-specific details
    if (int_no == 14) {  // Page fault
        // Read CR2 register containing faulting virtual address
        uint32_t faulting_address;
        asm volatile("mov %%cr2, %0" : "=r"(faulting_address));

        terminal_writestring("\nPage Fault Details:\n");
        terminal_writestring("  Faulting Address: 0x");
        itoa(faulting_address, buffer, 16);
        terminal_writestring(buffer);
        terminal_writestring("\n");

        terminal_writestring("  Caused by: ");
        if (!(err_code & 0x1)) {
            terminal_writestring("Page not present");
        } else if (err_code & 0x2) {
            terminal_writestring("Write to read-only page");
        } else if (err_code & 0x4) {
            terminal_writestring("User mode access to kernel page");
        } else if (err_code & 0x8) {
            terminal_writestring("Reserved bit set");
        } else {
            terminal_writestring("Unknown reason");
        }
        terminal_writestring("\n");
    } else if (int_no == 13) {  // General Protection Fault
        terminal_writestring("\nGeneral Protection Fault Details:\n");
        terminal_writestring("  Segment Selector: 0x");
        itoa(err_code & 0xFFF8, buffer, 16);
        terminal_writestring(buffer);
        terminal_writestring("\n");

        terminal_writestring("  Table: ");
        if (err_code & 0x2) {
            terminal_writestring("IDT");
        } else if (err_code & 0x4) {
            terminal_writestring("LDT");
        } else {
            terminal_writestring("GDT");
        }
        terminal_writestring("\n");

        if (err_code & 0x1) {
            terminal_writestring("  External event\n");
        }
    } else if (int_no == 0) {  // Division by zero
        terminal_writestring("\nDivision by zero detected!\n");
        terminal_writestring("Check your arithmetic operations.\n");
    } else if (int_no == 6) {  // Invalid opcode
        terminal_writestring("\nInvalid or undefined instruction!\n");
        terminal_writestring("Possible causes:\n");
        terminal_writestring("  - Corrupted code\n");
        terminal_writestring("  - Jump to invalid address\n");
        terminal_writestring("  - Wrong CPU architecture\n");
    }

    // Display halt message
    terminal_writestring("\n");
    terminal_writestring("==================================================\n");
    terminal_writestring("The system has been halted to prevent damage.\n");
    terminal_writestring("Please reboot the system.\n");
    terminal_writestring("==================================================\n\n");

    // Halt system: disable interrupts and loop indefinitely
    asm volatile("cli");
    for (;;) {
        asm volatile("hlt");
    }
}

// Common IRQ handler (called from assembly)
void irq_handler(uint32_t irq_no) {
    // Handle specific IRQs
    if (irq_no == 32) {  // IRQ0 - Timer
        timer_handler();
    } else if (irq_no == 33) {  // IRQ1 - Keyboard
        keyboard_handler();
    }

    // Send EOI to PIC
    pic_send_eoi(irq_no - IRQ_OFFSET);
}

// Initialize PIC and remap IRQs
void pic_init(void) {
    // Save masks
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);

    // Start initialization sequence (ICW1)
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    // Set vector offsets (ICW2)
    outb(PIC1_DATA, IRQ_OFFSET);         // Master PIC: IRQ 0-7 mapped to 32-39
    io_wait();
    outb(PIC2_DATA, IRQ_OFFSET + 8);     // Slave PIC: IRQ 8-15 mapped to 40-47
    io_wait();

    // Tell PICs about cascade (ICW3)
    outb(PIC1_DATA, 0x04);               // Tell Master: Slave is at IRQ2
    io_wait();
    outb(PIC2_DATA, 0x02);               // Tell Slave: cascade identity is 2
    io_wait();

    // Set 8086 mode (ICW4)
    outb(PIC1_DATA, ICW4_8086);
    io_wait();
    outb(PIC2_DATA, ICW4_8086);
    io_wait();

    // Restore masks (or set new ones)
    outb(PIC1_DATA, mask1);
    outb(PIC2_DATA, mask2);

    // Mask all IRQs except IRQ2 (cascade)
    outb(PIC1_DATA, 0xFB);  // 11111011 - all masked except IRQ2
    outb(PIC2_DATA, 0xFF);  // 11111111 - all masked
}

// Send End of Interrupt (EOI) to PIC
void pic_send_eoi(uint8_t irq) {
    // If IRQ came from slave PIC, send EOI to both
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    // Always send EOI to master
    outb(PIC1_COMMAND, PIC_EOI);
}

// Mask (disable) an IRQ
void pic_set_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) | (1 << irq);
    outb(port, value);
}

// Unmask (enable) an IRQ
void pic_clear_mask(uint8_t irq) {
    uint16_t port;
    uint8_t value;

    if (irq < 8) {
        port = PIC1_DATA;
    } else {
        port = PIC2_DATA;
        irq -= 8;
    }
    value = inb(port) & ~(1 << irq);
    outb(port, value);
}
