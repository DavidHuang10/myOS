#include "include/gdt.h"

// GDT with 3 entries: null, code, data
#define GDT_ENTRIES 3
struct gdt_entry gdt[GDT_ENTRIES];
struct gdt_ptr gdt_pointer;

// External assembly function to load GDT
extern void gdt_flush(uint32_t);

// Set a GDT entry
static void gdt_set_gate(int num, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran) {
    // Base address
    gdt[num].base_low = (base & 0xFFFF);
    gdt[num].base_middle = (base >> 16) & 0xFF;
    gdt[num].base_high = (base >> 24) & 0xFF;

    // Limit (size)
    gdt[num].limit_low = (limit & 0xFFFF);
    gdt[num].granularity = (limit >> 16) & 0x0F;

    // Granularity and access flags
    gdt[num].granularity |= gran & 0xF0;
    gdt[num].access = access;
}

// Initialize GDT
void gdt_init(void) {
    gdt_pointer.limit = (sizeof(struct gdt_entry) * GDT_ENTRIES) - 1;
    gdt_pointer.base = (uint32_t)&gdt;

    // Null descriptor (required)
    gdt_set_gate(0, 0, 0, 0, 0);

    // Code segment descriptor
    // Base = 0x00000000, Limit = 0xFFFFFFFF
    // Access = 0x9A: Present, Ring 0, Executable, Readable
    // Granularity = 0xCF: 4KB granularity, 32-bit
    gdt_set_gate(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

    // Data segment descriptor
    // Base = 0x00000000, Limit = 0xFFFFFFFF
    // Access = 0x92: Present, Ring 0, Writable
    // Granularity = 0xCF: 4KB granularity, 32-bit
    gdt_set_gate(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

    // Load GDT
    gdt_flush((uint32_t)&gdt_pointer);
}
