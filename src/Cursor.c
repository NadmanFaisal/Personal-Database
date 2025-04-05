#include "Cursor.h"
#include "Node.h"

CURSOR *tableStart(TABLE *table) {
    CURSOR *cursor = tableFind(table, 0);
    void *node = getPage(table->pager, cursor->pageNum);
    uint32_t cellNums = *leafNodeNumCells(node);
    cursor->endOfTable = (cellNums = 0);

    return cursor;
}

CURSOR *tableFind(TABLE* table, uint32_t key) {
    uint32_t rootPageNum = table->rootPageNum;
    void *rootNode = getPage(table->pager, rootPageNum);

    if(getNodeType(rootNode) == NODE_LEAF) {
        return findLeafNode(table, rootPageNum, key);
    } else {
        return internalNodeFind(table, rootPageNum, key);
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
        // Advance cursor to the next leaf node
        uint32_t nextPageNum = *leafNodeNextLeaf(node);
        if(nextPageNum == 0) {
            cursor->endOfTable = true;
        } else {
            cursor->pageNum = nextPageNum;
            cursor->cellNum = 0;
        }
    }
}

uint32_t internalNodeFindChild(void* node, uint32_t key) {
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

    return lo;
}

CURSOR *internalNodeFind(TABLE *table, uint32_t pageNum, uint32_t key) {
    void *node = getPage(table->pager, pageNum);
    
    uint32_t childIndex = internalNodeFindChild(node, key);
    uint32_t childNum = *internalNodeChild(node, childIndex);
    void *child = getPage(table->pager, childNum);
    switch(getNodeType(child)) {
        case NODE_LEAF:
            return findLeafNode(table, childNum, key);
        case NODE_INTERNAL:
            return internalNodeFind(table, childNum, key);
    }
}