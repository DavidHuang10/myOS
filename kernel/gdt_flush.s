.section .text
.global gdt_flush
.type gdt_flush, @function

gdt_flush:
    # Load GDT pointer (argument in 4(%esp))
    mov 4(%esp), %eax
    lgdt (%eax)

    # Reload segment registers with data segment (0x10 = 16 = 2nd entry * 8)
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs
    mov %ax, %ss

    # Far jump to reload CS with code segment (0x08 = 8 = 1st entry * 8)
    ljmp $0x08, $flush_complete

flush_complete:
    ret
