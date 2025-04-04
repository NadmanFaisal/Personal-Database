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

CURSOR* findLeafNode(TABLE* table, uint32_t page_num, uint32_t key) {
    void* node = getPage(table->pager, page_num);
    uint32_t num_cells = *leafNodeNumCells(node);

    CURSOR* cursor = malloc(sizeof(CURSOR));
    cursor->table = table;
    cursor->pageNum = page_num;

    // Binary search
    uint32_t min_index = 0;
    uint32_t one_past_max_index = num_cells;
    while (one_past_max_index != min_index) {
      uint32_t index = (min_index + one_past_max_index) / 2;
      uint32_t key_at_index = *leafNodeKey(node, index);

      if (key == key_at_index) {
        cursor->cellNum = index;
        return cursor;
      }

      if (key < key_at_index) {
        one_past_max_index = index;
      } else {
        min_index = index + 1;
      }
    }

    cursor->cellNum = min_index;
    return cursor;
}

NodeType getNodeType(void *node) {
    uint8_t value = *((uint8_t*)(node + NODE_TYPE_OFFSET));
    return (NodeType)value;
}

void setNodeType(void *node, NodeType type) {
    uint8_t value = type;
    *((uint8_t *)(node + NODE_TYPE_OFFSET)) = value;
}

void initializeLeafNode(void *node) {
    setNodeType(node, NODE_LEAF);
    *leafNodeNumCells(node) = 0;
}