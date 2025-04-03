#include "Cursor.h"
#include "Node.h"

CURSOR *tableStart(TABLE *table) {
    CURSOR *cursor = malloc(sizeof(CURSOR));
    cursor->table = table;
    cursor->pageNum = table->rootPageNum;
    cursor->cellNum = 0;

    void *rootNode = getPage(table->pager, table->rootPageNum);
    uint32_t cellNums = *leafNodeNumCells(rootNode);
    cursor->endOfTable = (cellNums == 0);

    return cursor;
}

CURSOR *tableEnd(TABLE *table) {
    CURSOR *cursor = malloc(sizeof(CURSOR));
    cursor->table = table;
    cursor->pageNum = table->rootPageNum;

    void *rootNode = getPage(table->pager, table->rootPageNum);
    uint32_t cellNums = *leafNodeNumCells(rootNode);
    cursor->cellNum = cellNums;
    cursor->endOfTable = true;

    return cursor;
}

void *cursorValue(CURSOR* cursor) {
    uint32_t pageNum = cursor->pageNum;
    void *page = getPage(cursor->table->pager, pageNum);
    return leafNodeValue(page, cursor->cellNum);
}

void cursorAdvance(CURSOR *cursor) {
    uint32_t pageNum = cursor->pageNum;
    void *node = getPage(cursor->table->pager, pageNum);
    cursor->cellNum += 1;
    if(cursor->cellNum >= (*leafNodeNumCells(node))) {
        cursor->endOfTable = true;
    }
}
