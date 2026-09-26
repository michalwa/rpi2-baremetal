SECTIONS {
    . = 0x8000;

    .text.boot : { *(.text.boot) }
    .text      : { *(.text) }

    . = ALIGN(0x4);
    .rodata    : { *(.rodata) }
    . = ALIGN(0x4);
    .data      : { *(.data) }

    . = ALIGN(0x4);
    __bss_start = .;
    .bss : { *(.bss) }
    __bss_end = .;
}
