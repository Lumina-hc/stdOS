#define IRQ(x) (0x20+x)

int registerHandler(int vector, void (*handler)(void));
int io_in8(int port);
void io_out8(int port, int data);
volatile unsigned int tick = 0;

void pic_enable_irq(unsigned char irq){
    unsigned char mask = io_in8(0x21);
    mask &= ~(1 << irq);
    io_out8(0x21, mask);
}

void timer_handler(void){
    tick++;
    io_out8(0x20, 0x20);
}

void timer_init(void){
    keyboard_init();
    registerHandler(IRQ(0), timer_handler);
    pic_enable_irq(0);
}