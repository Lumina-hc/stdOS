#include "kernelClang/stdint.h"
#include "kernelClang/std.h"
#include "kernelClang/fat16.h"
#include "kernelClang/memory.h"

void set_pading();

FAT16 fs;
char buf[128];
char tmp[1024];

volatile unsigned int tick = 0;

void timer_handler(void){
    tick++;
    io_out8(0x20, 0x20);
}

void timer_init(void){
    registerHandler(0x20, timer_handler);
    io_out8(0x21, 0xFE);
}

static int icmp(char *a, char *b){
    for(;*a&&*b;a++,b++){
        char ca=*a, cb=*b;
        if(ca>='A'&&ca<='Z') ca+=32;
        if(cb>='A'&&cb<='Z') cb+=32;
        if(ca!=cb) return 0;
    }
    return *a==*b;
}

static int ipre(char *a, char *pre){
    for(;*pre;a++,pre++){
        char ca=*a, cp=*pre;
        if(ca>='A'&&ca<='Z') ca+=32;
        if(cp>='A'&&cp<='Z') cp+=32;
        if(ca!=cp) return 0;
    }
    return *a==' '||*a==0;
}

void command(char *s){
    if(*s==0) return;
    if(icmp(s,"help")){ print("help clear about ls cat cd mkdir rm write\n"); return; }
    if(icmp(s,"clear")){ clear(); return; }
    if(icmp(s,"about")){ print("stdOS v0.1.0\n"); return; }
    if(icmp(s,"ls")){ fat16_list(&fs); return; }
    if(ipre(s,"cd")){ if(fat16_cd(&fs,s+3)) print("not found\n"); return; }
    if(ipre(s,"cat")){
        int n=fat16_read(&fs,s+4,(unsigned char*)tmp,1024);
        if(n<0) print("not found\n");
        else{ tmp[n]=0; print(tmp); print("\n"); }
        return;
    }
    if(ipre(s,"mkdir")){
        int r=fat16_mkdir(&fs,s+6);
        if(r==-1) print("disk full\n");
        else if(r==-2) print("exists\n");
        return;
    }
    if(ipre(s,"rm")){ if(fat16_rm(&fs,s+3)) print("not found\n"); return; }
    if(ipre(s,"write")){
        int i=6; while(s[i]!=' '&&s[i]) i++;
        if(!s[i]){ print("write <name> <data>\n"); return; }
        s[i]=0; char *name=s+6; char *data=s+i+1;
        if(fat16_write(&fs,name,(unsigned char*)data,strlen(data))) print("write failed\n");
        return;
    }
    if(ipre(s,"echo")){ int i=5; if(s[i]){ print(s+i); print("\n"); } return; }
    print("unknown\n");
}

void OSinit(){
    timer_init();
    asm_sti();
    
    HeapInit();
    fat16_init(&fs);
    set_pading();
    return ;
}

void kmain(void){
    OSinit();
    print("stdOS Alpha 0.1.0\n");
    while(1){
        print("> ");
        rline(buf);
        command(buf);
    }
}
