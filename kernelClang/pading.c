#include "stdint.h"

// 暂时

void load_pading(uint32_t* page_table);

void set_pading(void)
{
    uint32_t *pd = (uint32_t *)0x1000;
    uint32_t *pt = (uint32_t *)0x2000;

    // zero
    for (int i = 0; i < 1024; i++) {
        pt[i] = 0;
        pd[i] = 0;
    }

    // 页目录第一项指向页表，可写
    pd[0] = (uint32_t)pt | 0x03;

    // total 4 mb pading    
    uint32_t phys = 0;
    for (int i = 0; i < 1024; i++) {
        pt[i] = phys | 0x03;
        phys += 4096;
    }

    load_pading(pd);
}