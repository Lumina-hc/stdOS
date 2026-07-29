void print(char* msg);

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