#ifndef DISKIO_H
#define DISKIO_H

int disk_read_sectors(unsigned int lba, unsigned int count, void *buf);
int disk_write_sectors(unsigned int lba, unsigned int count, void *buf);

#endif
