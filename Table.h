#ifndef TABLE_H
#define TABLE_H

#include <stdint.h>
#include <stdlib.h>
#include <stdint.h>

#include "Pager.h"
#include "Constants.h"

typedef struct {
    uint32_t numRows;
    PAGER *pager;
} TABLE;

void *rowSlot(TABLE *table, uint32_t rowNum);

#endif