BITS 32

section .text.entry
global _start
extern kmain
extern __bss_start
extern __bss_end

_start:
    cli
    mov esp, stack_top
    xor ebp, ebp
    mov edi, __bss_start
    mov ecx, __bss_end
    sub ecx, edi
    xor eax, eax
    shr ecx, 2
    rep stosd
    call kmain
hang:
    hlt
    jmp hang

section .stack nobits
align 16
stack_bottom:
resb 16384
stack_top:
