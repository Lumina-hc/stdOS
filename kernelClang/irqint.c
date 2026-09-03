#include "stdint.h"
#define IRQ(x) (0x20+x)

#define MAX_TIMER 500

#define TF_FREEZE 0
#define TF_ALLOC 1
#define TF_USING 2

typedef struct {
    uint32_t flag;
    uint32_t time_out;
}TIMER;

typedef struct {
    uint32_t count;

    TIMER timer[MAX_TIMER];
}_TIMER;

_TIMER Timer;

int registerHandler(int vector, void (*handler)(void));

int io_in8(int port);
void io_out8(int port, int data);
void asm_sti(void);

void pic_enable_irq(unsigned char irq){
    unsigned char mask = io_in8(0x21);
    mask &= ~(1 << irq);
    io_out8(0x21, mask);
}

void timer_handler(void){
    Timer.count++;
    for (int i = 0;i < MAX_TIMER;i++){
        TIMER* temp = &Timer.timer[i];
        if(temp->flag == TF_USING){
            if (temp->time_out > 0) temp->time_out--;
            if (temp->time_out == 0) temp->flag = TF_ALLOC;
        }
    }
    io_out8(0x20, 0x20);
    return ;
}

#define KB_BUF 256

volatile unsigned char kb_buf[KB_BUF];
// FIFO缓冲区
volatile int kb_head = 0;
volatile int kb_tail = 0;

void keyboard_handler(void){
    unsigned char sc = (unsigned char)io_in8(0x60);
    int next = (kb_head + 1) % KB_BUF;
    if (next != kb_tail) {
        kb_buf[kb_head] = sc;
        kb_head = next;
    }
    io_out8(0x20, 0x20);
}

int get_scancode(void){
    while (kb_tail == kb_head) {}
    unsigned char sc = kb_buf[kb_tail];
    kb_tail = (kb_tail + 1) % KB_BUF;
    return (int)sc;
}

void keyboard_init(void){
    registerHandler(IRQ(1), keyboard_handler);
    pic_enable_irq(1);
}


void timer_init(void){
    // 每10ms发生一次中断
    io_out8(0x43,0x34);
    io_out8(0x40,0x9c);
    io_out8(0x40,0x2e);

    Timer.count = 0;

    for (int i = 0;i < MAX_TIMER;i++)
        Timer.timer[i].flag = TF_FREEZE;

    registerHandler(IRQ(0), timer_handler);
    pic_enable_irq(0);
}

void IRQ_init(void){
    timer_init();
    keyboard_init();
    asm_sti();
}