#include "../stdbool.h"
#include "../stdint.h"

enum STATE {
    RUNNING,
    SUSPEND,
    EXITED
};

typedef enum STATE STATE; 

#define STACK_SIZE 4096
#define IS_NULL(pointer) (!(pointer))

struct task{
    void (*entry)();
    int esp;
    char stack[STACK_SIZE];

    STATE state;

    struct task* prev;
    struct task* next;
};

// 任务段

struct TSS32 {
    uint32_t prev_link;
    uint32_t esp0;
    uint32_t ss0;
    uint32_t esp1, ss1;
    uint32_t esp2, ss2;
    uint32_t cr3, eip, eflags, eax, ecx, edx, ebx, esp, ebp, esi, edi;
    uint32_t es, cs, ss, ds, fs, gs;
    uint32_t ldt;
    uint32_t trap;
    uint32_t iomap_base;
} __attribute__((packed));

typedef struct task task;
typedef task* ptask;

void* new_task(void (*entry)());
bool finish_task(void* task);
STATE task_state(void* task);