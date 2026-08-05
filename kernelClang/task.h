enum STATE {
    RUNNING,
    SLEEP,
    FREEZE,
    EXITED
};

typedef enum STATE STATE; 

#define STACK_SIZE 4096
#define NULL ((ptask)0)

struct task{
    void (*entry)();
    int esp;
    char stack[STACK_SIZE];

    STATE state;

    struct task* prev;
    struct task* next;
};

typedef struct task task;
typedef task* ptask;