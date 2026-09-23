.section ".text.boot"

.global _start

_start:
    // Read MPIDR and halt all but one core
    mrc p15, 0, r5, c0, c0, 5   // read MPIDR into r5
    and r5, r5, #3              // get last 2 bits
    cmp r5, #0                  // check if 0
    bne halt                    // halt otherwise

    // Set up stack (grows down from _start)
    ldr r5, =_start
    mov sp, r5

    // Clear BSS
    ldr r4, =__bss_start        // r4 is the iteration variable
    ldr r9, =__bss_end          // r9 is the end counter
    mov r5, #0                  // Set up r5-r8 as "source" registers
    mov r6, #0
    mov r7, #0
    mov r8, #0
    b   2f
1:
    stmia r4!, {r5-r8}          // store r5-r8 at r4 and advance r4
2:
    cmp r4, r9
    blo 1b

    // Call kernel main
    ldr r3, =kernel_main
    blx r3

halt:
    wfe
    b halt
