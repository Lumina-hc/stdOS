#include "memory.h"
#include "task.h"

#define NULL ((ptask)0)

/*
save_esp:
    pushad
    mov eax,esp
    ret

restore_esp:
    mov esp,[esp+4]
    popad
    ret
*/

int save_esp();
void restore_esp(int before);

ptask head = NULL;
ptask tail = NULL;

void* new_task(void (*entry)()){
    ptask new_t = (ptask)kmalloc(sizeof(task));
    if (IS_NULL(new_t)) return (void *)NULL; 

    new_t -> state = SUSPEND;

    new_t -> esp =  (new_t -> stack + STACK_SIZE);
    new_t -> entry = entry;
    if (head == NULL && head == tail) head = tail = new_t;
    if (head == NULL && head == tail) {
        new_t->next = new_t->prev = NULL;
    }else {
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
    if (exit_task -> state != SUSPEND) return 0;
    exit_task -> state = EXITED;
    if (IS_NULL(exit_task)) return 0;
    if (IS_NULL(head)) return 0;
    else {
        exit_task -> prev -> next = exit_task -> next;
        if (!IS_NULL(exit_task -> next)) exit_task -> next -> prev = exit_task -> prev;
        kfree(exit_task); 
        return 1;
    }
}

STATE task_state(void* task){
    ptask t = (ptask) task;
    return t -> state;
}

void suspend_task(ptask desti){
    desti->state = SUSPEND;
    //TODO
}
void switch_task(ptask now, ptask after){
    //TODO

}