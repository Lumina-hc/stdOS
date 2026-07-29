global clear, C_read_line

clear:
    mov edi,video
    mov ecx,2000
    mov ax,0x0720
    rep stosw
    mov dword[cursor],0
    call update_cursor
    ret

print:
    lodsb
    cmp al,0
    je print_end
    call putchar
    jmp print

print_end:
    ret

putchar:
    cmp al,13
    je newline

    mov ebx,[cursor]
    mov [video+ebx],al
    mov byte[video+ebx+1],7
    add ebx,2
    mov [cursor],ebx
    call update_cursor
    ret

newline:
    mov eax,[cursor]
    mov ebx,160
    xor edx,edx
    div ebx
    inc eax
    mul ebx
    mov [cursor],eax
    call update_cursor
    ret

update_cursor:
    mov eax,[cursor]
    shr eax,1

    mov dx,0x3d4
    mov al,0x0f
    out dx,al

    mov dx,0x3d5
    mov al,byte[eax]
    out dx,al

    mov dx,0x3d4
    mov al,0x0e
    out dx,al

    mov dx,0x3d5
    mov al,byte[eax+1]
    out dx,al

    ret

C_read_line:
    mov edi,[esp + 4]
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
    mov byte [video+ebx],' '
    mov byte [video+ebx+1],7
    call update_cursor
    jmp read_loop

input_done:
    mov byte[esi],0
    mov al,13
    call putchar
    ret