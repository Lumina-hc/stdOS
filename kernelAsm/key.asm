bits 32

global keyboard

shift db 0

keyboard:
    in al,0x64
    test al,1
    jz keyboard
    in al,0x60
    mov bl,al
    and bl,0x80
    jnz .brk

    cmp al,0x2A
    je .sdn
    cmp al,0x36
    je .sdn

    xor ebx,ebx
    mov bl,al
    cmp byte [shift],0
    jne .sft
    mov al,[key_norm+ebx]
    cmp al,0
    je keyboard
    ret

.sft:
    mov al,[key_shift+ebx]
    cmp al,0
    je keyboard
    ret

.sdn:
    mov byte [shift],1
    jmp keyboard

.brk:
    cmp al,0xAA
    je .sup
    cmp al,0xB6
    je .sup
    jmp keyboard

.sup:
    mov byte [shift],0
    jmp keyboard

key_norm:
db 0,0
db '1','2','3','4','5','6','7','8','9','0'
db '-','=',0x08,0x09
db 'q','w','e','r','t','y','u','i','o','p'
db '[',']',13,0
db 'a','s','d','f','g','h','j','k','l'
db ';',0x27,'`',0
db 0x5C,'z','x','c','v','b','n','m'
db ',','.','/',0,0,0,' ',0
times 197 db 0

key_shift:
db 0,0
db '!','@','#','$','%','^','&','*','(',')'
db '_','+',0x08,0x09
db 'Q','W','E','R','T','Y','U','I','O','P'
db '{','}',13,0
db 'A','S','D','F','G','H','J','K','L'
db ':',0x22,'~',0
db '|','Z','X','C','V','B','N','M'
db '<','>','?',0,0,0,' ',0
times 197 db 0
