#include "kernelClang/stdint.h"
#include "kernelClang/std.h"
#include "kernelClang/disk/fat16.h"
#include "kernelClang/memory/memory.h"
#include "kernelClang/memory/task.h"

FAT16 fs;
char buf[128];
char tmp[1024];

extern char __kernel_end[];

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
    if(icmp(s,"help")){ print("help clear about ls cat cd mkdir rm write shell\n"); return; }
    if(icmp(s,"clear")){ clear(); return; }
    if(icmp(s,"about")){ print("stdOS v0.2.0\n"); return; }
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
    if(icmp(s,"shell")){
        char* src = (char*)__kernel_end;
        char* dst = (char*)0x400000;
        for(int i = 0; i < 4096; i++) dst[i] = src[i];
        print("entering user mode...\n");
        jump_to_user(0x400000 + 0x1000, 0x400000);
        print("returned from user mode?\n");
        return;
    }
    print("unknown\n");
}

void OSinit(){
    IRQ_init();
    HeapInit();
    fat16_init(&fs);
    return ;
}

void kmain(void){
    OSinit();
    print("stdOS Alpha 0.2.0\n");
    while(1){
        print("> ");
        rline(buf);
        command(buf);
    }
}
