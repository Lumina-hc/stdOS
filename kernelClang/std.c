#include "std.h"

void rline(char* buffer){
    C_read_line(buffer);
}

char getchar(void){
    return (char)keyboard();
}

int strlen(char* s){
    int i=0;
    while(s[i]) i++;
    return i;
}

int strcmp(char* source, const char* desti){
    int i=0;
    while(source[i]&&desti[i]){
        if(source[i]!=desti[i]) return 0;
        i++;
    }
    if(source[i]!=desti[i]) return 0;
    return 1;
}
