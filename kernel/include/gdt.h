#ifndef GDT_H
#define GDT_H

#include <stdint.h>

// GDT entry structure (8 bytes)
struct gdt_entry {
    uint16_t limit_low;      // Lower 16 bits of limit
    uint16_t base_low;       // Lower 16 bits of base
    uint8_t  base_middle;    // Next 8 bits of base
    uint8_t  access;         // Access flags (present, privilege, type)
    uint8_t  granularity;    // Granularity and upper 4 bits of limit
    uint8_t  base_high;      // Last 8 bits of base
} __attribute__((packed));

// GDT pointer structure for LGDT instruction
struct gdt_ptr {
    uint16_t limit;          // Size of GDT - 1
    uint32_t base;           // Address of GDT
} __attribute__((packed));

// Initialize GDT
void gdt_init(void);

#endif // GDT_H
