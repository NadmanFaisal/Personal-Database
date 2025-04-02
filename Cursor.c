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