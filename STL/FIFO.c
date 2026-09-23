#include "../kernelClang/stdint.h"
#include "../kernelClang/memory/memory.h"

struct Node {
    uint8_t data;
    struct Node *next, *prev;
};

struct FIFO8 {
    struct Node Data;
    uint32_t size;
    uint32_t free;
    struct Node *tail, *head;
};

#define NNULL ((struct Node*)0)

struct FIFO8* F8malloc(uint32_t size){
    struct FIFO8* temp = (struct FIFO8*)(kmalloc(sizeof(struct Node) * (size + 2) + sizeof(uint32_t)*2));
    return temp;
}

void init_FIFO8(struct FIFO8* fifo, uint32_t size){
    fifo->size = size;
    fifo->free = size;

    fifo->tail = &(fifo->Data);
    fifo->head = fifo->tail;

    struct Node*opd = fifo;
    struct Node*prev= opd;

    for (int i = 0;i < size;i++) {
        prev = opd;
        if (i == 0)  opd->prev = NNULL; 
        else {
            opd->prev = prev;
            opd->prev->next = opd;
        }
        opd->data = 0;
        if (i == size - 1) opd->next = NNULL;
    }
    return ;
}

void push_FIFO8(struct FIFO8* fifo, uint8_t data){
    
}