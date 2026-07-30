#include "std.h"
#include "fat16.h"
#include "diskio.h"

void to83(char *in, char *name, char *ext){
    int di=0,dot=0;
    for(int i=0;i<8;i++) name[i]=' ';
    for(int i=0;i<3;i++) ext[i]=' ';
    name[8]=0; ext[3]=0;
    for(int i=0;in[i]&&i<256;i++){
        char c=in[i];
        if(c>='a'&&c<='z') c-=32;
        if(c=='.'&&!dot){ dot=1; di=0; continue; }
        if(!dot){ if(di<8) name[di++]=c; }
        else{ if(di<3) ext[di++]=c; }
    }
}

static int read_fat(FAT16 *f, unsigned int clu){
    unsigned int sec=f->reserved+(clu*2)/f->bps;
    unsigned int off=(clu*2)%f->bps;
    if(disk_read_sectors(sec,1,f->buf)) return -1;
    return *(unsigned short *)(f->buf+off)&0xFFFF;
}

static int write_fat(FAT16 *f, unsigned int clu, unsigned int val){
    unsigned int sec=f->reserved+(clu*2)/f->bps;
    unsigned int off=(clu*2)%f->bps;
    if(disk_read_sectors(sec,1,f->buf)) return -1;
    *(unsigned short *)(f->buf+off)=val&0xFFFF;
    return disk_write_sectors(sec,1,f->buf);
}

static int alloc_clu(FAT16 *f, unsigned int *clu){
    unsigned int max=(f->fat_sec*f->bps)/2;
    if(max>65524) max=65524;
    for(*clu=2; *clu<max; (*clu)++){
        if(read_fat(f,*clu)==0){
            write_fat(f,*clu,0xFFFF);
            for(int i=0;i<512;i++) f->buf[i]=0;
            disk_write_sectors(f->data_start+(*clu-2)*f->spc,1,f->buf);
            return 0;
        }
    }
    return -1;
}

static void free_chain(FAT16 *f, unsigned int clu){
    while(1){
        unsigned int n=read_fat(f,clu);
        write_fat(f,clu,0);
        if(n>=0xFFF8) break;
        clu=n;
    }
}

static int match(unsigned char *e, char *name, char *ext){
    for(int i=0;i<11;i++){
        unsigned char c=(i<8)?name[i]:ext[i-8];
        if(e[i]!=c) return 0;
    }
    return 1;
}

static int find_in_root(FAT16 *f, char *name, char *ext, unsigned int *clu, unsigned int *sz){
    unsigned int lba=f->reserved+f->fats*f->fat_sec;
    for(unsigned int i=0;i<f->root_sec;i++){
        if(disk_read_sectors(lba+i,1,f->buf)) return -1;
        for(unsigned int off=0;off<f->bps;off+=32){
            if(f->buf[off]==0) return -1;
            if(f->buf[off]==0xE5) continue;
            if(match(f->buf+off,name,ext)){
                *clu=*(unsigned short *)(f->buf+off+26);
                *sz=*(unsigned int *)(f->buf+off+28);
                return f->buf[off+11];
            }
        }
    }
    return -1;
}

static int find_in_dir(FAT16 *f, unsigned int clu, char *name, char *ext, unsigned int *rcl, unsigned int *sz){
    while(1){
        unsigned int lba=f->data_start+(clu-2)*f->spc;
        for(unsigned int s=0;s<f->spc;s++){
            if(disk_read_sectors(lba+s,1,f->buf)) return -1;
            for(unsigned int off=0;off<f->bps;off+=32){
                if(f->buf[off]==0) return -1;
                if(f->buf[off]==0xE5) continue;
                if(match(f->buf+off,name,ext)){
                    *rcl=*(unsigned short *)(f->buf+off+26);
                    *sz=*(unsigned int *)(f->buf+off+28);
                    return f->buf[off+11];
                }
            }
        }
        unsigned int n=read_fat(f,clu);
        if(n>=0xFFF8) break;
        clu=n;
    }
    return -1;
}

static int find(FAT16 *f, char *name, char *ext, unsigned int *clu, unsigned int *sz){
    if(f->cur_dir==0) return find_in_root(f,name,ext,clu,sz);
    return find_in_dir(f,f->cur_dir,name,ext,clu,sz);
}

static int add_root(FAT16 *f, unsigned char *ent){
    unsigned int lba=f->reserved+f->fats*f->fat_sec;
    for(unsigned int i=0;i<f->root_sec;i++){
        if(disk_read_sectors(lba+i,1,f->buf)) return -1;
        for(unsigned int off=0;off<f->bps;off+=32){
            if(f->buf[off]==0||f->buf[off]==0xE5){
                for(int j=0;j<32;j++) f->buf[off+j]=ent[j];
                return disk_write_sectors(lba+i,1,f->buf);
            }
        }
    }
    return -1;
}

static int add_dir(FAT16 *f, unsigned int dclu, unsigned char *ent){
    while(1){
        unsigned int lba=f->data_start+(dclu-2)*f->spc;
        for(unsigned int s=0;s<f->spc;s++){
            if(disk_read_sectors(lba+s,1,f->buf)) return -1;
            for(unsigned int off=0;off<f->bps;off+=32){
                if(f->buf[off]==0||f->buf[off]==0xE5){
                    for(int j=0;j<32;j++) f->buf[off+j]=ent[j];
                    return disk_write_sectors(lba+s,1,f->buf);
                }
            }
        }
        unsigned int n=read_fat(f,dclu);
        if(n>=0xFFF8){
            unsigned int nc;
            if(alloc_clu(f,&nc)) return -1;
            write_fat(f,dclu,nc);
            for(int j=0;j<32;j++) f->buf[j]=ent[j];
            for(int j=32;j<512;j++) f->buf[j]=0;
            return disk_write_sectors(f->data_start+(nc-2)*f->spc,1,f->buf);
        }
        dclu=n;
    }
}

static void mkent(unsigned char *e, char *name, char *ext, unsigned char attr, unsigned int clu, unsigned int sz){
    for(int i=0;i<32;i++) e[i]=0;
    for(int i=0;i<8;i++) e[i]=name[i];
    for(int i=0;i<3;i++) e[8+i]=ext[i];
    e[11]=attr;
    *(unsigned short *)(e+26)=clu&0xFFFF;
    *(unsigned int *)(e+28)=sz;
}

void fat16_init(FAT16 *f){
    if(disk_read_sectors(0,1,f->buf)) return;
    f->bps=*(unsigned short *)(f->buf+11);
    f->spc=f->buf[13];
    f->reserved=*(unsigned short *)(f->buf+14);
    f->fats=f->buf[16];
    f->root_ent=*(unsigned short *)(f->buf+17);
    f->fat_sec=*(unsigned short *)(f->buf+22);
    f->root_sec=(f->root_ent*32+f->bps-1)/f->bps;
    f->data_start=f->reserved+f->fats*f->fat_sec+f->root_sec;
    f->cur_dir=0;
}

int fat16_list(FAT16 *f){
    if(f->cur_dir==0){
        unsigned int lba=f->reserved+f->fats*f->fat_sec;
        for(unsigned int i=0;i<f->root_sec;i++){
            if(disk_read_sectors(lba+i,1,f->buf)) return -1;
            for(unsigned int off=0;off<f->bps;off+=32){
                if(f->buf[off]==0) return 0;
                if(f->buf[off]==0xE5) continue;
                char n[13]; int di=0;
                for(int j=0;j<8;j++)
                    if(f->buf[off+j]!=' '){
                        char c=f->buf[off+j];
                        if(c>='A'&&c<='Z') c+=32;
                        n[di++]=c;
                    }
                int he=0;
                for(int j=0;j<3;j++)
                    if(f->buf[off+8+j]!=' ') he=1;
                if(he){ n[di++]='.';
                    for(int j=0;j<3;j++)
                        if(f->buf[off+8+j]!=' '){
                            char c=f->buf[off+8+j];
                            if(c>='A'&&c<='Z') c+=32;
                            n[di++]=c;
                        } }
                n[di]=0;
                print(f->buf[off+11]&ATTR_DIR?"[DIR] ":"      ");
                print(n); print("\n");
            }
        }
    } else {
        unsigned int clu=f->cur_dir;
        while(1){
            unsigned int lba=f->data_start+(clu-2)*f->spc;
            for(unsigned int s=0;s<f->spc;s++){
                if(disk_read_sectors(lba+s,1,f->buf)) return -1;
                for(unsigned int off=0;off<f->bps;off+=32){
                    if(f->buf[off]==0) return 0;
                    if(f->buf[off]==0xE5) continue;
                    char n[13]; int di=0;
                    for(int j=0;j<8;j++)
                        if(f->buf[off+j]!=' '){
                            char c=f->buf[off+j];
                            if(c>='A'&&c<='Z') c+=32;
                            n[di++]=c;
                        }
                    int he=0;
                    for(int j=0;j<3;j++)
                        if(f->buf[off+8+j]!=' ') he=1;
                    if(he){ n[di++]='.';
                        for(int j=0;j<3;j++)
                            if(f->buf[off+8+j]!=' '){
                                char c=f->buf[off+8+j];
                                if(c>='A'&&c<='Z') c+=32;
                                n[di++]=c;
                            } }
                    n[di]=0;
                    print(f->buf[off+11]&ATTR_DIR?"[DIR] ":"      ");
                    print(n); print("\n");
                }
            }
            unsigned int n=read_fat(f,clu);
            if(n>=0xFFF8) break;
            clu=n;
        }
    }
    return 0;
}

int fat16_cd(FAT16 *f, char *name){
    if(name[0]=='.'&&name[1]=='.'&&name[2]==0){
        if(f->cur_dir==0) return 0;
        unsigned int lba=f->data_start+(f->cur_dir-2)*f->spc;
        if(disk_read_sectors(lba,1,f->buf)) return -1;
        f->cur_dir=*(unsigned short *)(f->buf+58);
        return 0;
    }
    char n[9],ext[4];
    to83(name,n,ext);
    unsigned int clu,sz;
    int attr=find(f,n,ext,&clu,&sz);
    if(attr<0) return -1;
    if(!(attr&ATTR_DIR)) return -2;
    f->cur_dir=clu;
    return 0;
}

int fat16_mkdir(FAT16 *f, char *name){
    char n[9],ext[4];
    to83(name,n,ext);
    unsigned int clu,sz;
    if(find(f,n,ext,&clu,&sz)>=0) return -2;
    if(alloc_clu(f,&clu)) return -1;
    unsigned char e[32];
    mkent(e,n,ext,ATTR_DIR,clu,0);
    int r=f->cur_dir==0?add_root(f,e):add_dir(f,f->cur_dir,e);
    if(r){ free_chain(f,clu); return -1; }
    for(int i=0;i<512;i++) f->buf[i]=0;
    mkent(f->buf, ".       ", "   ", ATTR_DIR, clu, 0);
    mkent(f->buf+32, "..      ", "   ", ATTR_DIR, f->cur_dir, 0);
    disk_write_sectors(f->data_start+(clu-2)*f->spc, 1, f->buf);
    return 0;
}

int fat16_rm(FAT16 *f, char *name){
    char n[9],ext[4];
    to83(name,n,ext);
    if(f->cur_dir==0){
        unsigned int rlba=f->reserved+f->fats*f->fat_sec;
        for(unsigned int i=0;i<f->root_sec;i++){
            if(disk_read_sectors(rlba+i,1,f->buf)) return -1;
            for(unsigned int off=0;off<f->bps;off+=32){
                if(f->buf[off]==0) return -1;
                if(f->buf[off]==0xE5) continue;
                if(match(f->buf+off,n,ext)){
                    unsigned int fc=*(unsigned short *)(f->buf+off+26);
                    if(fc>=2&&!(f->buf[off+11]&ATTR_DIR)) free_chain(f,fc);
                    if(disk_read_sectors(rlba+i,1,f->buf)) return -1;
                    f->buf[off]=0xE5;
                    return disk_write_sectors(rlba+i,1,f->buf);
                }
            }
        }
    } else {
        unsigned int clu=f->cur_dir;
        while(1){
            unsigned int clba=f->data_start+(clu-2)*f->spc;
            for(unsigned int s=0;s<f->spc;s++){
                if(disk_read_sectors(clba+s,1,f->buf)) return -1;
                for(unsigned int off=0;off<f->bps;off+=32){
                    if(f->buf[off]==0) return -1;
                    if(f->buf[off]==0xE5) continue;
                    if(match(f->buf+off,n,ext)){
                        unsigned int fc=*(unsigned short *)(f->buf+off+26);
                        if(fc>=2&&!(f->buf[off+11]&ATTR_DIR)) free_chain(f,fc);
                        if(disk_read_sectors(clba+s,1,f->buf)) return -1;
                        f->buf[off]=0xE5;
                        return disk_write_sectors(clba+s,1,f->buf);
                    }
                }
            }
            unsigned int n=read_fat(f,clu);
            if(n>=0xFFF8) break;
            clu=n;
        }
    }
    return -1;
}

int fat16_write(FAT16 *f, char *name, unsigned char *data, unsigned int sz){
    char n[9],ext[4];
    to83(name,n,ext);
    unsigned int oc=0,os;
    int ex=(find(f,n,ext,&oc,&os)>=0);
    if(ex&&oc>=2) free_chain(f,oc);
    unsigned int first=0,prev=0,clu;
    unsigned int left=sz;
    unsigned char *p=data;
    while(left>0){
        if(alloc_clu(f,&clu)){ if(first) free_chain(f,first); return -1; }
        if(prev) write_fat(f,prev,clu);
        for(unsigned int s=0;s<f->spc&&left>0;s++){
            unsigned int ch=f->bps;
            if(ch>left) ch=left;
            for(unsigned int i=0;i<ch;i++) f->buf[i]=p[i];
            for(unsigned int i=ch;i<f->bps;i++) f->buf[i]=0;
            if(disk_write_sectors(f->data_start+(clu-2)*f->spc+s,1,f->buf)){ if(first) free_chain(f,first); return -1; }
            p+=ch; left-=ch;
        }
        if(!first) first=clu;
        prev=clu;
    }
    if(!ex){
        unsigned char e[32];
        mkent(e,n,ext,0x20,first,sz);
        if(f->cur_dir==0) return add_root(f,e);
        return add_dir(f,f->cur_dir,e);
    }
    return 0;
}

int fat16_read(FAT16 *f, char *name, unsigned char *buf, unsigned int max){
    char n[9],ext[4];
    to83(name,n,ext);
    unsigned int clu,sz;
    int attr=find(f,n,ext,&clu,&sz);
    if(attr<0) return -1;
    if(attr&ATTR_DIR) return -2;
    if(sz>max) sz=max;
    unsigned int off=0;
    while(1){
        unsigned int lba=f->data_start+(clu-2)*f->spc;
        for(unsigned int s=0;s<f->spc;s++){
            if(disk_read_sectors(lba+s,1,f->buf)) return -1;
            unsigned int ch=f->bps;
            if(off+ch>sz) ch=sz-off;
            for(unsigned int i=0;i<ch;i++) buf[off+i]=f->buf[i];
            off+=ch;
            if(off>=sz) return off;
        }
        unsigned int n=read_fat(f,clu);
        if(n>=0xFFF8) return off;
        clu=n;
    }
}
