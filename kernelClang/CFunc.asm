global print, putchar, update_cursor, scroll
global asm_hlt, asm_cli, asm_sti
global io_in8, io_in16, io_in32
global io_out8, io_out16, io_out32

extern video, cursor

print:
    push ebp
    mov ebp,esp
    push esi
    mov esi,[ebp+8]
.l:
    lodsb
    cmp al,0
    je .d
    pushad
    call putchar
    popad
    jmp .l
.d:
    pop esi
    pop ebp
    ret

putchar:
    cmp al,0x0a
    je newline
    cmp al,0x0d
    je newline
    mov ebx,[cursor]
    mov [video+ebx],al
    mov byte [video+ebx+1],7
    add ebx,2
    cmp ebx,4000
    jb .ok
    call scroll
    mov ebx,3840
.ok:
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
    cmp eax,4000
    jb .ok
    call scroll
    mov eax,3840
.ok:
    mov [cursor],eax
    call update_cursor
    ret

scroll:
    pushad
    cld
    mov esi,video
    add esi,160
    mov edi,video
    mov ecx,960
    rep movsd
    mov edi,video
    add edi,3840
    mov ecx,80
    mov ax,0x0720
    rep stosw
    mov dword [cursor],3840
    popad
    ret

update_cursor:
    mov eax,[cursor]
    shr eax,1
    push eax
    mov dx,0x3d4
    mov al,0x0f
    out dx,al
    mov dx,0x3d5
    pop eax
    out dx,al
    push eax
    mov dx,0x3d4
    mov al,0x0e
    out dx,al
    mov dx,0x3d5
    pop eax
    shr eax,8
    out dx,al
    ret

asm_hlt:
    hlt
    ret

asm_cli:
    cli
    ret

asm_sti:
    sti
    ret

io_in8:
    mov edx,[esp+4]
    mov eax,0
    in al,dx
    ret

io_in16:
    mov edx,[esp+4]
    mov eax,0
    in ax,dx
    ret

io_in32:
    mov edx,[esp+4]
    mov eax,0
    in eax,dx
    ret

io_out8:
    mov edx,[esp+4]
    mov eax,[esp+8]
    out dx,al
    ret

io_out16:
    mov edx,[esp+4]
    mov eax,[esp+8]
    out dx,ax
    ret

io_out32:
    mov edx,[esp+4]
    mov eax,[esp+8]
    out dx,eax
    ret
