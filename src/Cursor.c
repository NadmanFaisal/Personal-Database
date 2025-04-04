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

CURSOR *tableFind(TABLE* table, uint32_t key) {
    uint32_t rootPageNum = table->rootPageNum;
    void *rootNode = getPage(table->pager, rootPageNum);

    if(getNodeType(rootNode) == NODE_LEAF) {
        return findLeafNode(table, rootPageNum, key);
    } else {
        printf("Need to implement searching an internal node.\n");
        exit(EXIT_FAILURE);
    }
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

CURSOR *internalNodeFind(TABLE *table, uint32_t pageNum, uint32_t key) {
    void *node = getPage(table->pager, pageNum);
    uint32_t numKeys = *internalNodeNumKeys(node);

    uint32_t lo = 0;
    uint32_t hi = numKeys;
    while(lo != hi) {
        uint32_t mid = (lo + hi) / 2;
        uint32_t keyToRight = *internalNodeKey(node, mid);

        if(keyToRight >= key) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }

    uint32_t childNum = *internalNodeCell(node, lo);
    void *child = getPage(table->pager, childNum);
    switch(getNodeType(child)) {
        case NODE_LEAF:
            return findLeafNode(table, childNum, key);
        case NODE_INTERNAL:
            return internalNodeFind(table, childNum, key);
    }
}