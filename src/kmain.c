#include <stdint.h>

#define COLOR_C(color, ch) (color << 4 | ch)

void cls(){
    for (uint16_t* i = (uint16_t*)0xB8000; i < (uint16_t*)0xB9000; i++) 
        *i = 0x00;
}

void kmain(void)
{
    volatile uint16_t* vga = (volatile uint16_t *)0xB8000;
    const char *msg = "stdOS boot ok";
    cls();
    for (int i = 0; msg[i]; i++)
        vga[i] = COLOR_C(0x0F0, (unsigned char)msg[i]);
    for (;;)
        __asm__ __volatile__("hlt");
}
