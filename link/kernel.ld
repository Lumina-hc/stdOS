ENTRY(_start)
SECTIONS
{
    . = 0x8000;
    .text : { *(.text.entry) *(.text .text.*) }
    .rodata : { *(.rodata .rodata.*) }
    .data : { *(.data .data.*) }
    . = ALIGN(4);
    __bss_start = .;
    .bss : { *(.bss .bss.*) *(COMMON) }
    . = ALIGN(4);
    __bss_end = .;
    .stack (NOLOAD) : ALIGN(16) { *(.stack) }
    /DISCARD/ : { *(.comment) *(.eh_frame) *(.note*) }
}
