#include "stdbool.h"

void clear(void);
void asm_hlt(void);
void asm_cli(void);
void asm_sti(void);

int io_in8(int port);
int io_in16(int port);
int io_in32(int port);
void io_out8(int port, int data);
void io_out16(int port, int data);
void io_out32(int port, int data);

void C_read_line(char* buffer);
void rline(char* buffer);
void print(char* msg);

int keyboard(void);
char getchar(void);

int registerHandler(int vector, void (*handler)(void));
extern volatile unsigned int tick;

int strlen(char* s);
int strcmp(char* source, const char* desti);
