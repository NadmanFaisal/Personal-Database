#ifndef PAGER_H
#define PAGER_H

#include <stdint.h>
#include "Constants.h"

typedef struct {
    int fileDesc;
    uint32_t fileLen;
    void *pages[TABLE_MAX_PAGES];
} PAGER;

PAGER *openPager(const char *fileName);
void *getPage(PAGER *pager, uint32_t pageNum);
void flushPager(PAGER *pager, uint32_t pageNum, uint32_t size);

#endif