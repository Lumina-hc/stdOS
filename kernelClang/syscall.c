#include "memory/task.h"
#include "memory/memory.h"
#include "stdint.h"
#include "std.h"

extern void syscall_entry(void);

#define SYS_PRINT   1
#define SYS_READ    2
#define SYS_EXIT    3
#define SYS_YIELD   4
#define SYS_GETPID  5

typedef struct {
    uint32_t ebp, edi, esi, edx, ecx, ebx, eax;
    uint32_t eip, cs, eflags, esp_user, ss;
} __attribute__((packed)) trap_frame;

static int sys_print(trap_frame* tf) {
    char* msg = (char*)tf->ebx;
    print(msg);
    return 0;
}

static int sys_read(trap_frame* tf) {
    char* buf = (char*)tf->ebx;
    int max = (int)tf->ecx;
    int i = 0;
    while (i < max - 1) {
        char c = getchar();
        if (c == 13) {
            buf[i] = 0;
            print("\n");
            return i;
        }
        if (c == 8) {
            if (i > 0) {
                i--;
                char bs[3] = {8, ' ', 8};
                print(bs);
            }
            continue;
        }
        buf[i++] = c;
        char s[2] = {c, 0};
        print(s);
    }
    buf[i] = 0;
    return i;
}

static int sys_exit(trap_frame* tf) {
    print("[process exited]\n");
    while(1) { asm_hlt(); }
    return 0;
}

static int sys_yield(trap_frame* tf) {
    return 0;
}

static int sys_getpid(trap_frame* tf) {
    return 0;
}

typedef int (*syscall_fn)(trap_frame*);
static syscall_fn syscall_table[] = {
    0,
    sys_print,
    sys_read,
    sys_exit,
    sys_yield,
    sys_getpid,
};

#define SYSCALL_COUNT (sizeof(syscall_table) / sizeof(syscall_table[0]))

void syscall_handler(trap_frame* tf) {
    uint32_t num = tf->eax;
    if (num < SYSCALL_COUNT && syscall_table[num]) {
        tf->eax = syscall_table[num](tf);
    } else {
        tf->eax = (uint32_t)-1;
    }
}

void syscall_init(void) {
    extern int registerHandler(int, void*);
    registerHandler(0x80, (void*)syscall_entry);
}
