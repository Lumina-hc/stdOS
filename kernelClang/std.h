#include "stdbool.h"
#include "stdint.h"

#define SEG_CODE 0x08
#define SEG_DATA 0x10
#define SEG_USER_CODE 0x18
#define SEG_USER_DATA 0x20

#define SEG_CODE 0x08
#define SEG_DATA 0x10
#define SEG_USER_CODE 0x18
#define SEG_USER_DATA 0x20

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

#define TF_FREEZE 0
#define TF_ALLOC 1
#define TF_USING 2

typedef struct {
    uint32_t flag;
    uint32_t time_out;
}TIMER;

#define MAX_TIMER 500

typedef struct {
    uint32_t count;

    TIMER timer[MAX_TIMER];
}_TIMER;

extern _TIMER Timer;

TIMER* timer_alloc(void);
void timer_free(TIMER* timer);
void timer_set(TIMER* timer, uint32_t timeout);

int strlen(char* s);
int strcmp(char* source, const char* desti);
char* fstr(const char* fmt, ...);
void IRQ_init(void);