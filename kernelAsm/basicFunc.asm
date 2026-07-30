global clear, C_read_line
extern putchar, update_cursor, cursor, keyboard

clear:
    mov edi,0xb8000
    mov ecx,2000
    mov ax,0x0720
    rep stosw
    mov dword [cursor],0
    call update_cursor
    ret

C_read_line:
    mov edi,[esp+4]
    jmp read_line

read_line:
    mov esi,edi

read_loop:
    call keyboard
    cmp al,13
    je input_done
    cmp al,0x08
    je do_backspace
    mov [esi],al
    inc esi
    call putchar
    jmp read_loop

do_backspace:
    cmp esi,edi
    jbe read_loop
    dec esi
    mov byte [esi],0
    mov ebx,[cursor]
    sub ebx,2
    mov [cursor],ebx
    mov byte [0xb8000+ebx],' '
    mov byte [0xb8000+ebx+1],7
    call update_cursor
    jmp read_loop

input_done:
    mov byte [esi],0
    mov al,13
    call putchar
    ret
