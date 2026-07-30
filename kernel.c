#include "kernelClang/std.h"

void command(char* buffer){
    if (*buffer == 0) return ;
    else if (strcmp(buffer, "help")) print("help cln about\n");
    else print("unknown command\n");
}

#define banner "stdOS 32bit kernel\n"
#define prompt "> "
char buf[128];

void kmain(void){
    print(banner);
    while(1){
        print(prompt);
        rline(buf);
        command(buf);
    }
}