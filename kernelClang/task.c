#include "memory.h"
#include "task.h"

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

ptask new_task(void (*entry)()){
    ptask new_t = (ptask)kmalloc(sizeof(task));
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
    }
    return new_t;
}
void finish_task(){
    //TODO
}
void sleep_task(ptask desti){
    desti->state = SLEEP;
    //TODO
}
void switch_task(ptask now, ptask after){
    sleep_task(now);
    //TODO

}