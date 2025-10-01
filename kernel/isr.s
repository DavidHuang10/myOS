.section .text

# External C handlers
.extern isr_handler
.extern irq_handler

# Macro for ISRs without error code
.macro ISR_NOERRCODE num
.global isr\num
isr\num:
    cli                    # Disable interrupts
    push $0                # Push dummy error code
    push $\num             # Push interrupt number
    jmp isr_common_stub    # Jump to common handler
.endm

# Macro for ISRs with error code
.macro ISR_ERRCODE num
.global isr\num
isr\num:
    cli                    # Disable interrupts
    # Error code already pushed by CPU
    push $\num             # Push interrupt number
    jmp isr_common_stub    # Jump to common handler
.endm

# CPU Exception ISRs (0-31)
ISR_NOERRCODE 0    # Division by zero
ISR_NOERRCODE 1    # Debug
ISR_NOERRCODE 2    # Non-maskable interrupt
ISR_NOERRCODE 3    # Breakpoint
ISR_NOERRCODE 4    # Overflow
ISR_NOERRCODE 5    # Bound range exceeded
ISR_NOERRCODE 6    # Invalid opcode
ISR_NOERRCODE 7    # Device not available
ISR_ERRCODE   8    # Double fault
ISR_NOERRCODE 9    # Coprocessor segment overrun
ISR_ERRCODE   10   # Invalid TSS
ISR_ERRCODE   11   # Segment not present
ISR_ERRCODE   12   # Stack-segment fault
ISR_ERRCODE   13   # General protection fault
ISR_ERRCODE   14   # Page fault
ISR_NOERRCODE 15   # Reserved
ISR_NOERRCODE 16   # x87 floating-point exception
ISR_ERRCODE   17   # Alignment check
ISR_NOERRCODE 18   # Machine check
ISR_NOERRCODE 19   # SIMD floating-point exception
ISR_NOERRCODE 20   # Virtualization exception
ISR_NOERRCODE 21   # Reserved
ISR_NOERRCODE 22   # Reserved
ISR_NOERRCODE 23   # Reserved
ISR_NOERRCODE 24   # Reserved
ISR_NOERRCODE 25   # Reserved
ISR_NOERRCODE 26   # Reserved
ISR_NOERRCODE 27   # Reserved
ISR_NOERRCODE 28   # Reserved
ISR_NOERRCODE 29   # Reserved
ISR_ERRCODE   30   # Security exception
ISR_NOERRCODE 31   # Reserved

# Common ISR handler stub
isr_common_stub:
    pusha              # Push all general purpose registers

    mov %ds, %ax       # Save data segment
    push %eax

    mov $0x10, %ax     # Load kernel data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    push %esp          # Push stack pointer (contains register state)

    call isr_handler   # Call C handler

    add $4, %esp       # Clean up pushed stack pointer

    pop %eax           # Restore data segment
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    popa               # Pop all general purpose registers
    add $8, %esp       # Clean up error code and interrupt number
    sti                # Re-enable interrupts
    iret               # Return from interrupt

# Macro for IRQ handlers
.macro IRQ num, irq_num
.global irq\num
irq\num:
    cli
    push $0            # Dummy error code
    push $\irq_num     # IRQ number
    jmp irq_common_stub
.endm

# Hardware IRQ handlers (32-47 after remapping)
IRQ 0, 32   # Timer
IRQ 1, 33   # Keyboard
IRQ 2, 34   # Cascade
IRQ 3, 35   # COM2
IRQ 4, 36   # COM1
IRQ 5, 37   # LPT2
IRQ 6, 38   # Floppy
IRQ 7, 39   # LPT1
IRQ 8, 40   # RTC
IRQ 9, 41   # Free
IRQ 10, 42  # Free
IRQ 11, 43  # Free
IRQ 12, 44  # PS/2 Mouse
IRQ 13, 45  # FPU
IRQ 14, 46  # Primary ATA
IRQ 15, 47  # Secondary ATA

# Common IRQ handler stub
irq_common_stub:
    pusha

    mov %ds, %ax
    push %eax

    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    push %esp

    call irq_handler   # Call C handler

    add $4, %esp

    pop %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    popa
    add $8, %esp
    sti
    iret

# IDT flush function
.global idt_flush
idt_flush:
    mov 4(%esp), %eax  # Get IDT pointer argument
    lidt (%eax)        # Load IDT
    ret
