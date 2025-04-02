#include "Cursor.h"

CURSOR *tableStart(TABLE *table) {
    CURSOR *cursor = malloc(sizeof(CURSOR));
    cursor->table = table;
    cursor->rowNum = 0;
    cursor->endOfTable = (table->numRows == 0);

    return cursor;
}

CURSOR *tableEnd(TABLE *table) {
    CURSOR *cursor = malloc(sizeof(CURSOR));
    cursor->table = table;
    cursor->rowNum = table->numRows;
    cursor->endOfTable = true;

    return cursor;
}

void *cursorValue(CURSOR* cursor) {
    uint32_t rowNum = cursor->rowNum;
    uint32_t pageNum = rowNum / ROWS_PER_PAGE;
    void *page = getPage(cursor->table->pager, pageNum);
    uint32_t rowOffset = rowNum % ROWS_PER_PAGE;
    uint32_t byteOffset = rowOffset * ROW_SIZE;
    return page + byteOffset;
}