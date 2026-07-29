global print

print: ; void print(char *msg);
    push ebp
    mov ebp,esp
    push esi
    mov esi,[ebp + 8]
    jmp .loop
.loop:
    lodsb
    cmp al,0
    je print_end
    pushad
    call putchar
    popad
    jmp .loop

print_end:
    pop esi
    pop ebp
    ret

putchar:
    cmp al,0x0a
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

extern video
extern cursor

global asm_hlt,asm_cli,asm_sti
global io_in8, io_in16, io_in32
global io_out8, io_out16, io_out32

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
    mov edx, [esp+4]
    mov eax,0
    in al,dx
    ret
io_in16:
    mov edx, [esp+4]
    mov eax,0
    in ax,dx
    ret
io_in32:
    mov edx, [esp+4]
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