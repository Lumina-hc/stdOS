#include "kernelClang/CF.h"
void clear(void);
void C_read_line(char* buffer);

void command(char* buffer){
    if (*buffer == 0) return ;
    else print("unknown command\n");
}

#define banner "stdOS 32bit kernel\n\0"
#define prompt "> \0"
char buffer[128];

void kmain(void){
    print(banner);
    while(1){
        print(prompt);
        C_read_line(buffer);
        command(buffer);
    }
}