#include "stdbool.h"

void clear(void);

void asm_hlt(void);
void asm_cli(void);
void asm_sti(void);
// io

int io_in8(int port);
int io_in16(int port);
int io_in32(int port);

void io_out8(int port, int data);
void io_out16(int port, int data);
void io_out32(int port, int data);

// standard input/output
void C_read_line(char* buffer);
void rline(char* buffer){
    C_read_line(buffer);
}

void print(char* msg);

// C
bool strcmp(char* source, const char* desti){
    int i = 0;
    while(source[i++] != '\0' && desti[i] != '\0') if (source[i] != desti[i]) return 0;
    return 1;
}