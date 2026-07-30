#include "std.h"
#include "diskio.h"

static int wait_bsy(void){
    int t=1000000;
    while(t--){
        if(!(io_in8(0x1F7)&0x80)) return 0;
    }
    return -1;
}

static int wait_drq(void){
    int t=1000000;
    while(t--){
        int s=io_in8(0x1F7);
        if(s&0x80) continue;
        if(s&0x08) return 0;
        return -1;
    }
    return -1;
}

int disk_read_sectors(unsigned int lba, unsigned int count, void *buf){
    unsigned short *p=(unsigned short *)buf;
    while(count--){
        if(wait_bsy()) return -1;
        io_out8(0x1F6,0xE0|((lba>>24)&0x0F));
        io_out8(0x1F2,1);
        io_out8(0x1F3,lba&0xFF);
        io_out8(0x1F4,(lba>>8)&0xFF);
        io_out8(0x1F5,(lba>>16)&0xFF);
        io_out8(0x1F7,0x20);
        if(wait_drq()) return -1;
        for(int i=0;i<256;i++) p[i]=io_in16(0x1F0);
        p+=256; lba++;
    }
    return 0;
}

int disk_write_sectors(unsigned int lba, unsigned int count, void *buf){
    unsigned short *p=(unsigned short *)buf;
    while(count--){
        if(wait_bsy()) return -1;
        io_out8(0x1F6,0xE0|((lba>>24)&0x0F));
        io_out8(0x1F2,1);
        io_out8(0x1F3,lba&0xFF);
        io_out8(0x1F4,(lba>>8)&0xFF);
        io_out8(0x1F5,(lba>>16)&0xFF);
        io_out8(0x1F7,0x30);
        if(wait_drq()) return -1;
        for(int i=0;i<256;i++) io_out16(0x1F0,p[i]);
        if(wait_bsy()) return -1;
        p+=256; lba++;
    }
    return 0;
}
