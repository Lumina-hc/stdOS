bits 32

keyboard:
wait_key:
    in al,0x64
    test al,1
    jz wait_key
    in al,0x60
    test al,0x80
    jnz wait_key
    xor ebx,ebx
    mov bl,al
    mov al, [key_table + ebx]
    cmp al,0
    jz wait_key
    ret

key_table:
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
