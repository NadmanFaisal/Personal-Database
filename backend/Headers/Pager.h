#ifndef PAGER_H
#define PAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <stdint.h>

#include "Constants.h"

typedef struct {
    int fileDesc;
    uint32_t fileLen;
    uint32_t numPages;
    void *pages[TABLE_MAX_PAGES];
} PAGER;

PAGER *openPager(const char *fileName);
void *getPage(PAGER *pager, uint32_t pageNum);
void flushPager(PAGER *pager, uint32_t pageNum);
uint32_t getUnusedPageNum(PAGER *pager);

#endif