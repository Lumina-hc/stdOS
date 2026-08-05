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

#define KB_BUF 256

volatile unsigned char kb_buf[KB_BUF];
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
    keyboard_init();
    registerHandler(IRQ(0), timer_handler);
    pic_enable_irq(0);
}