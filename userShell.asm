bits 32
org 0x400000

SYS_PRINT equ 1
SYS_READ  equ 2
SYS_EXIT  equ 3

_start:
    mov ebx, welcome_msg
    mov eax, SYS_PRINT
    int 0x80

shell_loop:
    mov ebx, prompt
    mov eax, SYS_PRINT
    int 0x80

    mov ebx, input_buf
    mov ecx, 128
    mov eax, SYS_READ
    int 0x80

    mov esi, input_buf
    mov edi, cmd_exit
    call strcmp
    cmp eax, 1
    je do_exit

    mov esi, input_buf
    mov edi, cmd_help
    call strcmp
    cmp eax, 1
    je do_help

    mov esi, input_buf
    mov edi, cmd_about
    call strcmp
    cmp eax, 1
    je do_about

    mov ebx, unknown_msg
    mov eax, SYS_PRINT
    int 0x80
    jmp shell_loop

do_help:
    mov ebx, help_text
    mov eax, SYS_PRINT
    int 0x80
    jmp shell_loop

do_about:
    mov ebx, about_text
    mov eax, SYS_PRINT
    int 0x80
    jmp shell_loop

do_exit:
    mov eax, SYS_EXIT
    int 0x80

strcmp:
    push esi
    push edi
.loop:
    mov al, [esi]
    mov bl, [edi]
    cmp al, bl
    jne .noteq
    cmp al, 0
    je .done
    inc esi
    inc edi
    jmp .loop
.noteq:
    pop edi
    pop esi
    mov eax, 0
    ret
.done:
    pop edi
    pop esi
    mov eax, 1
    ret

welcome_msg: db "[user] Welcome! Type 'help' for commands.", 10, 0
prompt:      db "> ", 0
unknown_msg: db "unknown command", 10, 0
cmd_exit:    db "exit", 0
cmd_help:    db "help", 0
cmd_about:   db "about", 0
help_text:   db "Available: help, about, exit", 10, 0
about_text:  db "stdOS User Shell v0.1.0", 10, 0

input_buf: times 128 db 0
