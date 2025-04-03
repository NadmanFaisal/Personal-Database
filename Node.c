#include "Node.h"
#include "Cursor.h"

uint32_t *leafNodeNumCells(void *node) {
    return node + LEAF_NODE_NUM_CELLS_OFFSET;
}

void *leafNodeCell(void *node, uint32_t cellNum) {
    return node + LEAF_NODE_HEADER_SIZE + cellNum * LEAF_NODE_CELL_SIZE;
}

uint32_t *leafNodeKey(void *node, uint32_t cellNum) {
    return leafNodeCell(node, cellNum);
}

void *leafNodeValue(void *node, uint32_t cellNum) {
    return leafNodeCell(node, cellNum) + LEAF_NODE_KEY_SIZE;
}

void initializeLeafNode(void *node) {
    *leafNodeNumCells(node) = 0;
}

void leafNodeInsert(CURSOR *cursor, uint32_t key, ROW *value) {
    void *node = getPage(cursor->table->pager, cursor->pageNum);

    uint32_t cellNums = *leafNodeNumCells(node);
    if(cellNums >= LEAF_NODE_MAX_CELLS) {
        printf("Need to split a leaf node.\n");
        exit(EXIT_FAILURE);
    }

    if(cursor->cellNum < cellNums) {
        for(uint32_t i = cellNums; i < cursor->cellNum; i--) {
            memcpy(leafNodeCell(node, i), leafNodeCell(node, i - 1), LEAF_NODE_CELL_SIZE);
        }
    }

    *(leafNodeNumCells(node)) += 1;
    *(leafNodeKey(node, cursor->cellNum)) = key;
    serialize_row(value, leafNodeValue(node, cursor->cellNum));
}