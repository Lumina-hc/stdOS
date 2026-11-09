#include <stdint.h>

void kmain(void)
{
    volatile uint16_t *vga = (volatile uint16_t *)0xB8000;
    const char *msg = "stdOS boot ok";
    for (int i = 0; msg[i]; i++)
        vga[i] = 0x0F00 | (unsigned char)msg[i];
    for (;;)
        __asm__ __volatile__("hlt");
}
