#include "std.h"

int strlen(char* s){
    int i=0;
    while(s[i]) i++;
    return i;
}

char getchar(void){
    return (char)keyboard();
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

static char* i2str(int num)
{
    static char buf[12];   // 内核常用 trick
    char* p = buf;
    int n = num;

    if (n == 0) {
        *p++ = '0';
        *p = '\0';
        return buf;
    }

    if (n < 0) {
        *p++ = '-';
        n = -n;
    }

    char* start = p;
    while (n) {
        *p++ = '0' + (n % 10);
        n /= 10;
    }

    char* end = p - 1;
    while (start < end) {
        char tmp = *start;
        *start = *end;
        *end = tmp;
        start++;
        end--;
    }

    *p = '\0';
    return buf;
}

char* fstr(const char* fmt, ...){
    va_list ap;
    va_start(ap, fmt);
    char* desti = "";
    int i = 0;
    for (const char* str = fmt;*str;str++){
        if (*str == '%'){
            str++;
            if (*str){
                switch(*str){
                    case'd': case'D':{
                        int v = va_arg(ap, int);
                        char* t = i2str(v);
                        while(*t)  desti[i++] = *t++;
                        break;
                    }
                    case'c': case'C':{
                        int c = va_arg(ap, char);
                        desti[i++] = (char)c;
                        break;
                    }
                }
            }
        }else desti[i++] = *str;
    }
    desti[i] = '\0';
    va_end(ap);
    return desti;
}