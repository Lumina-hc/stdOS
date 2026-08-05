int registerHandler(int vector, void (*handler)(void));
void io_out8(int port, int data);
volatile unsigned int tick = 0;

void timer_handler(void){
    tick++;
    io_out8(0x20, 0x20);
}

void timer_init(void){
    registerHandler(0x20, timer_handler);
    io_out8(0x21, 0xFE);
}