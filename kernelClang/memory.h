typedef unsigned long size_t;

void HeapInit();
void* kmalloc(size_t size);
void* kcalloc(size_t size);
void kfree(void* mem);