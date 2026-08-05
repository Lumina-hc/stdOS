#include "stdbool.h"

typedef char* va_list;
#define va_start(ap, last) \
    (ap = (char*)(&(last)) + sizeof(last))
#define va_arg(ap, type) \
    (*(type*)(ap += sizeof(type), ap - sizeof(type)))
#define va_end(ap) ((void)(ap))

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

void rline(char* buffer);
void print(char* msg);

int keyboard(void);
char getchar(void);

extern volatile unsigned int tick;

int strlen(char* s);
int strcmp(char* source, const char* desti);
char* fstr(const char* fmt, ...);
void timer_init(void);