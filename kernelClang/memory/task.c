#include "memory.h"
#include "task.h"

#define NULL ((ptask)0)

int save_esp();
void restore_esp(int before);

ptask head = NULL;
ptask tail = NULL;

struct TSS32 tss0;

void* new_task(void (*entry)()){
    ptask new_t = (ptask)kmalloc(sizeof(task));
    if (IS_NULL(new_t)) return (void *)NULL;

    new_t->state = SUSPEND;
    new_t->is_user = 0;
    new_t->kernel_esp = (int)(new_t->kernel_stack + KERNEL_STACK_SIZE);
    new_t->esp = (int)(new_t->stack + STACK_SIZE);
    new_t->entry = entry;

    if (head == NULL) {
        new_t->next = new_t->prev = NULL;
        head = tail = new_t;
    } else {
        new_t->next = tail->next;
        new_t->prev = tail;
        tail->next = new_t;
        if (new_t->next) new_t->next->prev = new_t;
        tail = new_t;
    }
    return (void*)new_t;
}

void* new_user_task(void (*entry)()){
    ptask new_t = (ptask)kmalloc(sizeof(task));
    if (IS_NULL(new_t)) return (void *)NULL;

    new_t->state = SUSPEND;
    new_t->is_user = 1;
    new_t->kernel_esp = (int)(new_t->kernel_stack + KERNEL_STACK_SIZE);
    new_t->esp = (int)(new_t->stack + STACK_SIZE);
    new_t->entry = entry;

    if (head == NULL) {
        new_t->next = new_t->prev = NULL;
        head = tail = new_t;
    } else {
        new_t->next = tail->next;
        new_t->prev = tail;
        tail->next = new_t;
        if (new_t->next) new_t->next->prev = new_t;
        tail = new_t;
    }
    return (void*)new_t;
}

bool finish_task(void* task){
    ptask exit_task = (ptask)task;
    if (IS_NULL(exit_task)) return 0;
    if (exit_task->state == EXITED) return 0;

    exit_task->state = EXITED;

    if (exit_task == head) head = exit_task->next;
    if (exit_task == tail) tail = exit_task->prev;

    if (exit_task->prev) exit_task->prev->next = exit_task->next;
    if (exit_task->next) exit_task->next->prev = exit_task->prev;

    kfree(exit_task);
    return 1;
}

STATE task_state(void* task){
    ptask t = (ptask) task;
    return t->state;
}

void suspend_task(ptask desti){
    desti->state = SUSPEND;
}

void switch_task(ptask now, ptask after){

}
