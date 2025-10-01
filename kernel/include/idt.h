#ifndef IDT_H
#define IDT_H

#include <stdint.h>

// IDT entry structure (8 bytes for 32-bit)
struct idt_entry {
    uint16_t base_lo;     // Lower 16 bits of ISR address
    uint16_t sel;         // Kernel code segment selector (0x08 from GDT)
    uint8_t  zero;        // Always 0
    uint8_t  flags;       // Type and attributes
    uint16_t base_hi;     // Upper 16 bits of ISR address
} __attribute__((packed));

// IDT pointer structure for LIDT instruction
struct idt_ptr {
    uint16_t limit;       // Size of IDT - 1
    uint32_t base;        // Address of IDT
} __attribute__((packed));

// IDT flags
#define IDT_FLAG_PRESENT    0x80  // Segment is present
#define IDT_FLAG_RING0      0x00  // Ring 0 (kernel)
#define IDT_FLAG_RING3      0x60  // Ring 3 (user)
#define IDT_FLAG_GATE_INT   0x0E  // 32-bit interrupt gate
#define IDT_FLAG_GATE_TRAP  0x0F  // 32-bit trap gate

// Standard IDT flags for interrupt gate
#define IDT_FLAGS_KERNEL_INT (IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_INT)  // 0x8E

// Initialize IDT
void idt_init(void);

// Set an IDT gate
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

// PIC ports
#define PIC1_COMMAND    0x20    // Master PIC command port
#define PIC1_DATA       0x21    // Master PIC data port
#define PIC2_COMMAND    0xA0    // Slave PIC command port
#define PIC2_DATA       0xA1    // Slave PIC data port

// PIC initialization command words
#define ICW1_INIT       0x10    // Initialization command
#define ICW1_ICW4       0x01    // ICW4 needed
#define ICW4_8086       0x01    // 8086 mode

// PIC End of Interrupt command
#define PIC_EOI         0x20    // End of Interrupt

// IRQ offsets after remapping
#define IRQ_OFFSET      32      // Remap IRQs to start at 32

// PIC functions
void pic_init(void);
void pic_send_eoi(uint8_t irq);
void pic_set_mask(uint8_t irq);
void pic_clear_mask(uint8_t irq);

#endif // IDT_H
