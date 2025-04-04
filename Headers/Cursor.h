#ifndef CURSOR_H
#define CURSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "Table.h"

typedef struct CURSOR {
    TABLE *table;
    uint32_t pageNum;
    uint32_t cellNum;
    bool endOfTable;
} CURSOR;

CURSOR *tableStart(TABLE *table);
CURSOR *tableFind(TABLE* table, uint32_t key);
void *cursorValue(CURSOR* cursor);
void cursorAdvance(CURSOR *cursor);
CURSOR *internalNodeFind(TABLE *table, uint32_t pageNum, uint32_t key);

#endif