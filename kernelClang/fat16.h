#ifndef FAT16_H
#define FAT16_H

#define ATTR_DIR 0x10

typedef struct {
    unsigned int bps;
    unsigned int spc;
    unsigned int reserved;
    unsigned int fats;
    unsigned int root_ent;
    unsigned int fat_sec;
    unsigned int root_sec;
    unsigned int data_start;
    unsigned int cur_dir;
    unsigned char buf[2048];
} FAT16;

void fat16_init(FAT16 *f);
int  fat16_list(FAT16 *f);
int  fat16_cd(FAT16 *f, char *name);
int  fat16_mkdir(FAT16 *f, char *name);
int  fat16_rm(FAT16 *f, char *name);
int  fat16_write(FAT16 *f, char *name, unsigned char *data, unsigned int sz);
int  fat16_read(FAT16 *f, char *name, unsigned char *buf, unsigned int max);
void to83(char *in, char *name, char *ext);

#endif
