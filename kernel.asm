[bits 32]
global _start

extern __bss_start, __bss_end
extern setIdt, irq_init, clear, kmain

_start:
    cli
    mov ax,0x10
    mov ds,ax
    mov es,ax
    mov ss,ax
    mov esp,0x200000

    mov edi,__bss_start
    mov ecx,__bss_end
    sub ecx,edi
    xor eax,eax
    cld
    rep stosb

    call setIdt
    call irq_init
    call clear
    call kmain

.h:
    hlt
    jmp .h
