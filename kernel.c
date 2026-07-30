#include "kernelClang/std.h"
#include "kernelClang/fat16.h"

FAT16 fs;
char buf[128];
char tmp[1024];

static int icmp(char *a, char *b){
    for(;*a&&*b;a++,b++){
        char ca=*a, cb=*b;
        if(ca>='A'&&ca<='Z') ca+=32;
        if(cb>='A'&&cb<='Z') cb+=32;
        if(ca!=cb) return 0;
    }
    return *a==*b;
}

void command(char *s){
    if(*s==0) return;
    if(icmp(s,"help")){ print("help clear about hlt ls cat cd mkdir rm write\n"); return; }
    if(icmp(s,"clear")){ clear(); return; }
    if(icmp(s,"about")){ print("stdOS FAT16\n"); return; }
    if(icmp(s,"hlt")){ asm_hlt(); return; }
    if(icmp(s,"ls")){ fat16_list(&fs); return; }
    if((s[0]=='c'||s[0]=='C')&&(s[1]=='d'||s[1]=='D')&&s[2]==' '){ if(fat16_cd(&fs,s+3)) print("not found\n"); return; }
    if((s[0]=='c'||s[0]=='C')&&(s[1]=='a'||s[1]=='A')&&(s[2]=='t'||s[2]=='T')&&s[3]==' '){
        int n=fat16_read(&fs,s+4,(unsigned char*)tmp,1024);
        if(n<0) print("not found\n");
        else{ tmp[n]=0; print(tmp); print("\n"); }
        return;
    }
    if((s[0]=='m'||s[0]=='M')&&(s[1]=='k'||s[1]=='K')&&(s[2]=='d'||s[2]=='D')&&(s[3]=='i'||s[3]=='I')&&(s[4]=='r'||s[4]=='R')&&s[5]==' '){
        int r=fat16_mkdir(&fs,s+6);
        if(r==-1) print("disk full\n");
        else if(r==-2) print("exists\n");
        return;
    }
    if((s[0]=='r'||s[0]=='R')&&(s[1]=='m'||s[1]=='M')&&s[2]==' '){ if(fat16_rm(&fs,s+3)) print("not found\n"); return; }
    if(icmp(s,"write ")){
        int i=6; while(s[i]!=' '&&s[i]) i++;
        if(!s[i]){ print("write <name> <data>\n"); return; }
        s[i]=0; char *name=s+6; char *data=s+i+1;
        if(fat16_write(&fs,name,(unsigned char*)data,strlen(data))) print("write failed\n");
        return;
    }
    if(icmp(s,"echo")){ int i=5; if(s[i]){ print(s+i); print("\n"); } return; }
    print("unknown\n");
}

void kmain(void){
    fat16_init(&fs);
    print("stdOS FAT16\n");
    while(1){
        print("> ");
        rline(buf);
        command(buf);
    }
}
