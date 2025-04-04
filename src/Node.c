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
        leafNodeSplitAndInsert(cursor, key, value);
        return;
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

void leafNodeSplitAndInsert(CURSOR *cursor, uint32_t key, ROW *value) {
    /*
    Create new node and move half the cells over.
    Insert the new value in any of the following two nodes. 
    Update the parent node, or create a new parent node. 
    */
    void *oldNode = getPage(cursor->table->pager, cursor->pageNum);
    uint32_t newPageNum = getUnusedPageNum(cursor->table->pager);
    void *newNode = getPage(cursor->table->pager, newPageNum);
    initializeLeafNode(newNode);

    /*
    Divide evenly all existing keys and new key between 
    left and right (old and new nodes). Starting from the 
    right, move each key to correct position. 
    */

    for(int32_t i = LEAF_NODE_MAX_CELLS; i >= 0; i--) {
        void *destination;
        if(i >= LEAF_NODE_LEFT_SPLIT_COUNT) {
            destination = newNode;
        } else {
            destination = oldNode;
        }

        uint32_t indexWithinNode = i % LEAF_NODE_LEFT_SPLIT_COUNT;
        void *destination = leafNodeCell(destination, indexWithinNode);

        if(i == cursor->cellNum) {
            serialize_row(value, destination);
        } else if(i > cursor->cellNum) {
            memcpy(destination, leafNodeCell(oldNode, i - 1), LEAF_NODE_CELL_SIZE);
        } else {
            memcpy(destination, leafNodeCell(oldNode, i), LEAF_NODE_CELL_SIZE);
        }
    }

    *(leafNodeNumCells(oldNode)) = LEAF_NODE_LEFT_SPLIT_COUNT;
    *(leafNodeNumCells(newNode)) = LEAF_NODE_RIGHT_SPLIT_COUNT;

    if(isNodeRoot(oldNode)) {
        return createNewRoot(cursor->table, newPageNum);
    } else {
        printf("Need to implement updating parent after splitting.\n");
        exit(EXIT_FAILURE);
    }
}