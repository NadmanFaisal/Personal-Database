#include <stdlib.h>
#include <stdint.h>

#include "Row.h"
#include "Table.h"

void *rowSlot(TABLE *table, uint32_t rowNum) {
    uint32_t pageNum = rowNum / ROWS_PER_PAGE;
    void *page = table->pages[pageNum];
    if(page == NULL) {
        page = table->pages[pageNum];
    }

    uint32_t rowOffset = rowNum % ROWS_PER_PAGE;
    uint32_t byteOffset = rowOffset * ROW_SIZE;
    return page + byteOffset;
}