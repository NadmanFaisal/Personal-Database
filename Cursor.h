#ifndef CURSOR_H
#define CURSOR_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "Table.h"
#include "Node.h"

typedef struct {
    TABLE *table;
    uint32_t pageNum;
    uint32_t cellNum;
    bool endOfTable;
} CURSOR;

CURSOR *tableStart(TABLE *table);
CURSOR *tableEnd(TABLE *table);
void *cursorValue(CURSOR* cursor);
void cursorAdvance(CURSOR *cursor);

#endif