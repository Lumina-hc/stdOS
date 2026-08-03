#include "stdbool.h"
#include "stdint.h"

typedef unsigned long size_t;
typedef unsigned int uintptr_t;

struct MemBlk{
    size_t size;
    bool free;
    struct MemBlk* next;
    struct MemBlk* prev;
};

typedef struct MemBlk HBL; // Heap block
typedef struct MemBlk* HBLP;// Heap block pointer

#define VNULL ((void*) 0)
#define HNULL ((HBLP ) 0)
#define AL8(x) (((x) + 7) & ~7)

#define HSIZE (1024*1024)

HBLP HeapHead = HNULL;
uint8_t Heap[HSIZE];

// | head | Heap Memory |

void HeapInit(){
    HBLP first = (HBLP)AL8((uintptr_t) Heap);
    first->free = 1;
    first->size = HSIZE - sizeof(HBL); // head pointer
    first->prev = HNULL;
    first->next = HNULL;

    HeapHead = first;
}

HBLP find_free_mem(size_t size)
{
    HBLP cur = HeapHead;

    while (cur) {
        if (cur->free && cur->size >= size) return cur;
        cur = cur->next;
    }
    return HNULL;
}

#define MINHEAPSIZE (sizeof(HBL) + 8)

void* kmalloc(size_t size)
{
    if (size == 0) return VNULL;

    size = AL8(size);

    HBLP blk = find_free_mem(size);
    if (blk == HNULL) return VNULL;

    size_t total = blk->size;
    if (total >= size + MINHEAPSIZE) {
        HBLP newblk = (HBLP)((uint8_t*)(blk + 1) + size);

        newblk->size = total - size - sizeof(HBL);
        newblk->free = 1;
        newblk->prev = blk;
        newblk->next = blk->next;

        if (newblk->next) newblk->next->prev = newblk;

        blk->next = newblk;
        blk->size = size;
    }

    blk->free = 0;

    return (void*)(blk + 1);
}

void kfree(void* mem)
{
    if (!mem) return;

    HBLP blk = ((HBLP)mem) - 1;
    blk->free = 1;

    if (blk->next && blk->next->free) {
        HBLP next = blk->next;
        blk->size += sizeof(HBL) + next->size;
        blk->next = next->next;
        if (blk->next) blk->next->prev = blk;
    }

    if (blk->prev && blk->prev->free) {
        HBLP prev = blk->prev;
        prev->size += sizeof(HBL) + blk->size;
        prev->next = blk->next;
        if (prev->next)
            prev->next->prev = prev;
    }
}