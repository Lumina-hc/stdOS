bits 32

global syscall_entry
extern syscall_handler

syscall_entry:
    push eax
    push ebx
    push ecx
    push edx
    push esi
    push edi
    push ebp

    push esp
    call syscall_handler
    add esp, 4

    pop ebp
    pop edi
    pop esi
    pop edx
    pop ecx
    pop ebx
    pop eax

    iretd
