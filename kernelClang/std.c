#include "std.h"

uint32_t abs(int num){
    uint32_t n = num;
    return n;
}

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
    static char buf[12];
    char* p = buf;
    int n = abs(num);

    if (n == 0) {
        *p++ = '0';
        *p = '\0';
        return buf;
    }

    if (num < 0){
        *p++ = '-';
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

static char* hex(int num){
    static char buf[12];
    char* p = buf;
    int n = abs(num);

    *p++ = '0';
    *p++ = 'x';

    if (n == 0) {
        *p++ = '0';
        *p = '\0';
        return buf;
    }

    if (num < 0){
        *p++ = '-';
    }

    char* start = p;
    while (n) {
        *p++ = ((n % 16) >= 10) ? (n % 16 - 10 + 'A') : (n % 16 + '0');
        n /= 16;
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
                    case'h': case'H':{
                        int v = va_arg(ap,int);
                        char* t = hex(v);
                        while(*t)  desti[i++] = *t++;
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

// timer

TIMER* timer_alloc(void){ // unit: ms
    for (int i = 0;i < MAX_TIMER;i++){
        if(Timer.timer[i].flag == TF_FREEZE){
            Timer.timer[i].flag = TF_ALLOC;
            return &Timer.timer[i];
        }
    }
    return (TIMER*) -1;
}
void timer_free(TIMER* timer){
    if (timer == ((TIMER*) -1)) return ;
    timer->flag = TF_FREEZE;
    return ;
}

void timer_set(TIMER* timer, uint32_t timeout){
    timer->time_out = timeout;
    timer->flag = TF_USING;
    return ;
}