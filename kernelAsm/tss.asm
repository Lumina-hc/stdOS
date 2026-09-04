bits 32

global tss_init, tss_set_kernel_stack, jump_to_user
extern tss0, syscall_init

TSS_SEL equ 0x28

tss_init:
    sub esp, 16
    sgdt [esp]
    mov ebx, [esp + 2]
    add ebx, 40
    mov eax, tss0
    mov word [ebx + 2], ax
    shr eax, 16
    mov byte [ebx + 4], al
    shr eax, 8
    mov byte [ebx + 7], al
    mov ax, TSS_SEL
    ltr ax
    add esp, 16
    call syscall_init
    ret

tss_set_kernel_stack:
    mov eax, [esp + 4]
    mov [tss0 + 4], eax
    ret

jump_to_user:
    cli
    mov eax, [esp + 4]
    mov ebx, [esp + 8]
    lea ecx, [eax]
    and ecx, 0xFFFFF000
    add ecx, 4096
    mov [tss0 + 4], ecx
    mov word [tss0 + 8], 0x10
    push dword 0x20
    push eax
    pushfd
    pop ecx
    or ecx, 0x200
    push ecx
    push dword 0x18
    push ebx
    mov ax, 0x20
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    iretd
