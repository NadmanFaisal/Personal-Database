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
        for(uint32_t i = cellNums; i > cursor->cellNum; i--) {
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
    setNodeRoot(node, false);
    *leafNodeNumCells(node) = 0;
    *leafNodeNextLeaf(node) = 0;
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
    *leafNodeNextLeaf(newNode) = *leafNodeNextLeaf(oldNode);
    *leafNodeNextLeaf(oldNode) = newPageNum;

    /*
    Divide evenly all existing keys and new key between 
    left and right (old and new nodes). Starting from the 
    right, move each key to correct position. 
    */

    for(int32_t i = LEAF_NODE_MAX_CELLS; i >= 0; i--) {
        void *destinationNode;
        if(i >= LEAF_NODE_LEFT_SPLIT_COUNT) {
            destinationNode = newNode;
        } else {
            destinationNode = oldNode;
        }

        uint32_t indexWithinNode = i % LEAF_NODE_LEFT_SPLIT_COUNT;
        void *destination = leafNodeCell(destinationNode, indexWithinNode);

        if(i == cursor->cellNum) {
            serialize_row(value, leafNodeValue(destinationNode, indexWithinNode));
            *leafNodeKey(destinationNode, indexWithinNode) = key;
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

void createNewRoot(TABLE *table, uint32_t rightChildPageNum) {
    void *root = getPage(table->pager, table->rootPageNum);
    void *rootChild = getPage(table->pager, rightChildPageNum);
    uint32_t leftChildPageNum = getUnusedPageNum(table->pager);
    void *leftChild = getPage(table->pager, leftChildPageNum);

    memcpy(leftChild, root, PAGE_SIZE);
    setNodeRoot(leftChild, false);

    initializeInternalNode(root);
    setNodeRoot(root, true);
    *internalNodeNumKeys(root) = 1;
    *internalNodeChild(root, 0) = leftChildPageNum;
    uint32_t leftChildMaxKey = getNodeMaxKey(leftChild);
    *internalNodeKey(root, 0) = leftChildMaxKey;
    *internalNodeRightChild(root) = rightChildPageNum;
}

uint32_t *internalNodeNumKeys(void *node) {
    return node + INTERNAL_NODE_NUM_KEYS_OFFSET;
}

uint32_t* internalNodeRightChild(void* node) {
    return node + INTERNAL_NODE_RIGHT_CHILD_OFFSET;
}

uint32_t *internalNodeCell(void *node, uint32_t cellNum) {
    return node + INTERNAL_NODE_HEADER_SIZE + cellNum * INTERNAL_NODE_CELL_SIZE;
}

uint32_t *internalNodeChild(void *node, uint32_t childNum) {
    uint32_t numKeys = *internalNodeNumKeys(node);
    if(childNum > numKeys) {
        printf("Tried to access childNum %d > numKeys %d\n", childNum, numKeys);
        exit(EXIT_FAILURE);
    } else if(childNum == numKeys) {
        return internalNodeRightChild(node);
    } else {
        return internalNodeCell(node, childNum);
    }
}

uint32_t *internalNodeKey(void *node, uint32_t keyNum) {
    return internalNodeCell(node, keyNum) + INTERNAL_NODE_CHILD_SIZE;
}

uint32_t getNodeMaxKey(void *node) {
    switch(getNodeType(node)) {
        case NODE_INTERNAL:
            return *internalNodeKey(node, *internalNodeNumKeys(node) - 1);
        case NODE_LEAF:
            return *leafNodeKey(node, *leafNodeNumCells(node) - 1);
    }
}

bool isNodeRoot(void *node) {
    uint8_t value = *((uint8_t *)(node + IS_ROOT_OFFSET));
    return (bool)value;
}

void setNodeRoot(void *node, bool isRoot) {
    uint8_t value = isRoot;
    *((uint8_t *)(node + IS_ROOT_OFFSET)) = value;
}

void initializeInternalNode(void *node) {
    setNodeType(node, NODE_INTERNAL);
    setNodeRoot(node, false);
    *internalNodeNumKeys(node) = 0;
}

uint32_t *leafNodeNextLeaf(void *node) {
    return node + LEAF_NODE_NEXT_LEAF_OFFSET;
}