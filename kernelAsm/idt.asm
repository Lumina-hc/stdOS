bits 32

IDT_PRESENT   equ 0x80
IDT_DPL0      equ 0x00
IDT_INT_GATE  equ 0x0E

IDT_ATTR_INT  equ IDT_PRESENT|IDT_DPL0|IDT_INT_GATE

CODE_SEL      equ 0x08
DATA_SEL      equ 0x10
IDT_ENTRIES   equ 256

global setIdt, irq_init, registerHandler

%macro vector_stub 1
    global vector_%1
    vector_%1:
    push dword 0
    push dword %1
    jmp alltraps
%endmacro

%macro vector_stub_err 1
    global vector_%1
    vector_%1:
    push dword %1
    jmp alltraps
%endmacro

%assign i 0
%rep IDT_ENTRIES
    %if i==8||(i>=10&&i<=14)||i==17
        vector_stub_err i
    %else
        vector_stub i
    %endif
    %assign i i+1
%endrep

alltraps:
    push ds
    push es
    push fs
    push gs
    pushad
    mov ax,DATA_SEL
    mov ds,ax
    mov es,ax
    mov eax,[esp+48]
    cmp eax,IDT_ENTRIES
    jae .ignore
    mov ebx,[handler_table+eax*4]
    test ebx,ebx
    jz .ignore
    push esp
    call ebx
    add esp,4
.ignore:
    popad
    pop gs
    pop fs
    pop es
    pop ds
    add esp,8
    iretd

setIdt:
    pushad
    push ds
    push es
    mov ax,DATA_SEL
    mov ds,ax
    mov es,ax
    cld
    mov edi,idt
    mov ecx,IDT_ENTRIES
    xor ebx,ebx
.loop:
    mov eax,[vector_table+ebx*4]
    mov word [edi],ax
    mov word [edi+2],CODE_SEL
    mov byte [edi+4],0
    mov byte [edi+5],IDT_ATTR_INT
    shr eax,16
    mov word [edi+6],ax
    add edi,8
    inc ebx
    dec ecx
    jnz .loop
    lidt [idt_desc]
    pop es
    pop ds
    popad
    ret

registerHandler:
    mov eax,[esp+4]
    mov ebx,[esp+8]
    cmp eax,IDT_ENTRIES
    jae .bad
    mov [handler_table+eax*4],ebx
    xor eax,eax
    ret
.bad:
    or eax,-1
    ret

global irq_init
irq_init:
    pushad
    mov al,0x11
    out 0x20,al
    out 0xA0,al
    mov al,0x20
    out 0x21,al
    mov al,0x28
    out 0xA1,al
    mov al,0x04
    out 0x21,al
    mov al,0x02
    out 0xA1,al
    mov al,0x01
    out 0x21,al
    out 0xA1,al
    mov al,0xFF
    out 0x21,al
    mov al,0xFF
    out 0xA1,al
    popad
    ret

section .data

vector_table:
%assign i 0
%rep IDT_ENTRIES
dd vector_%+i
%assign i i+1
%endrep

handler_table:
times IDT_ENTRIES dd 0

idt:
times IDT_ENTRIES*8 db 0
idt_end:

idt_desc:
dw idt_end-idt-1
dd idt
