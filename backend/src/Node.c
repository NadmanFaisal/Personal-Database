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
    uint32_t oldMax = getNodeMaxKey(cursor->table->pager, oldNode);
    uint32_t newPageNum = getUnusedPageNum(cursor->table->pager);
    void *newNode = getPage(cursor->table->pager, newPageNum);
    initializeLeafNode(newNode);
    *nodeParent(newNode) = *nodeParent(oldNode);
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
        uint32_t parentPageNum = *nodeParent(oldNode);
        uint32_t newMax = getNodeMaxKey(cursor->table->pager, oldNode);
        void *parent = getPage(cursor->table->pager, parentPageNum);

        updateInternalNodeKey(parent, oldMax, newMax);
        internalNodeInsert(cursor->table, parentPageNum, newPageNum);
        return;
    }
}

void createNewRoot(TABLE *table, uint32_t rightChildPageNum) {
    void *root = getPage(table->pager, table->rootPageNum);
    void *rightChild = getPage(table->pager, rightChildPageNum);
    uint32_t leftChildPageNum = getUnusedPageNum(table->pager);
    void *leftChild = getPage(table->pager, leftChildPageNum);

    if(getNodeType(root) == NODE_INTERNAL) {
        initializeInternalNode(rightChild);
        initializeInternalNode(leftChild);
    }

    memcpy(leftChild, root, PAGE_SIZE);
    setNodeRoot(leftChild, false);

    if(getNodeType(leftChild) == NODE_INTERNAL) {
        void *child;
        for(int i = 0; i < *internalNodeNumKeys(leftChild); i++) {
            child = getPage(table->pager, *internalNodeChild(leftChild, i));
            *nodeParent(child) = leftChildPageNum;
        }
        child = getPage(table->pager, *internalNodeRightChild(leftChild));
        *nodeParent(child) = leftChildPageNum;
    }

    initializeInternalNode(root);
    setNodeRoot(root, true);
    
    *internalNodeNumKeys(root) = 1;
    *internalNodeChild(root, 0) = leftChildPageNum;
    
    uint32_t leftChildMaxKey = getNodeMaxKey(table->pager, leftChild);
    
    *internalNodeKey(root, 0) = leftChildMaxKey;
    *internalNodeRightChild(root) = rightChildPageNum;
    
    *nodeParent(leftChild) = table->rootPageNum;
    *nodeParent(rightChild) = table->rootPageNum;
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
        uint32_t *rightChild = internalNodeRightChild(node);
        if(*rightChild == INVALID_PAGE_NUM) {
            printf("Tried to access right child of node, but it was invalid page.\n");
            exit(EXIT_FAILURE);
        }
        return rightChild;
    } else {
        uint32_t *child = internalNodeCell(node, childNum);
        if(*child == INVALID_PAGE_NUM) {
            printf("Tried to access child %d of node, but it was invalid page.\n", childNum);
            exit(EXIT_FAILURE);
        }
        return child;
    }
}

uint32_t *internalNodeKey(void *node, uint32_t keyNum) {
    return (void*)internalNodeCell(node, keyNum) + INTERNAL_NODE_CHILD_SIZE;
}

uint32_t getNodeMaxKey(PAGER *pager, void *node) {
    if(getNodeType(node) == NODE_LEAF) {
        return *leafNodeKey(node, *leafNodeNumCells(node) - 1);
    }
    void *rightChild = getPage(pager, *internalNodeRightChild(node));
    return getNodeMaxKey(pager, rightChild);
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

    *internalNodeRightChild(node) = INVALID_PAGE_NUM;
}

uint32_t *leafNodeNextLeaf(void *node) {
    return node + LEAF_NODE_NEXT_LEAF_OFFSET;
}

uint32_t *nodeParent(void *node) {
    return node + PARENT_POINTER_OFFSET;
}

void updateInternalNodeKey(void *node, uint32_t oldKey, uint32_t newKey) {
    uint32_t oldChildIndex = internalNodeFindChild(node, oldKey);
    *internalNodeKey(node, oldChildIndex) = newKey;
}

void internalNodeInsert(TABLE *table, uint32_t parentPageNum, uint32_t childPageNum) {
    void *parent = getPage(table->pager, parentPageNum);
    void *child = getPage(table->pager, childPageNum);
    uint32_t childMaxKey = getNodeMaxKey(table->pager, child);
    uint32_t index = internalNodeFindChild(parent, childMaxKey);

    uint32_t originalNumKeys = *internalNodeNumKeys(parent);

    if(originalNumKeys >= INTERNAL_NODE_MAX_CELLS) {
        internalNodeSplitAndInsert(table, parentPageNum, childPageNum);
        return;
    }

    uint32_t rightChildPageNum = *internalNodeRightChild(parent);

    if(rightChildPageNum == INVALID_PAGE_NUM) {
        *internalNodeRightChild(parent) = childPageNum;
        return;
    }

    void *rightChild = getPage(table->pager, rightChildPageNum);
    *internalNodeNumKeys(parent) = originalNumKeys + 1;

    if(childMaxKey > getNodeMaxKey(table->pager, rightChild)) {
        *internalNodeChild(parent, originalNumKeys) = rightChildPageNum;
        *internalNodeKey(parent, originalNumKeys) = getNodeMaxKey(table->pager, rightChild);
        *internalNodeRightChild(parent) = childPageNum;
    } else {
        for(uint32_t i = originalNumKeys; i > index; i--) {
            void *destination = internalNodeCell(parent, i);
            void *source = internalNodeCell(parent, i - 1);
            memcpy(destination, source, INTERNAL_NODE_CELL_SIZE);
        }
        *internalNodeChild(parent, index) = childPageNum;
        *internalNodeKey(parent, index) = childMaxKey;
    }
}

void internalNodeSplitAndInsert(TABLE *table, uint32_t parentPageNum, uint32_t childPageNum) {
    uint32_t oldPageNum = parentPageNum;
    void *oldNode = getPage(table->pager, parentPageNum);
    uint32_t oldMax = getNodeMaxKey(table->pager, oldNode);

    void *child = getPage(table->pager, childPageNum);
    uint32_t childMax = getNodeMaxKey(table->pager, child);

    uint32_t newPageNum = getUnusedPageNum(table->pager);

    uint32_t splittingRoot = isNodeRoot(oldNode);

    void *parent, *newNode;
    
    if(splittingRoot) {
        createNewRoot(table, newPageNum);
        parent = getPage(table->pager, table->rootPageNum);

        oldPageNum = *internalNodeChild(parent, 0);
        oldNode = getPage(table->pager, oldPageNum);
    } else {
        parent = getPage(table->pager, *nodeParent(oldNode));
        newNode = getPage(table->pager, newPageNum);
        initializeInternalNode(newNode);
    }

    uint32_t *oldNumKeys = internalNodeNumKeys(oldNode);
    uint32_t currentPageNum = *internalNodeRightChild(oldNode);
    void *current = getPage(table->pager, currentPageNum);

    internalNodeInsert(table, newPageNum, currentPageNum);
    *nodeParent(current) = newPageNum;
    *internalNodeRightChild(oldNode) = INVALID_PAGE_NUM;

    for(int i = INTERNAL_NODE_MAX_CELLS - 1; i > INTERNAL_NODE_MAX_CELLS / 2; i--) {
        currentPageNum = *internalNodeChild(oldNode, i);
        current = getPage(table->pager, currentPageNum);

        internalNodeInsert(table, newPageNum, currentPageNum);
        *nodeParent(current) = newPageNum;
        (*oldNumKeys)--;
    }
    *internalNodeRightChild(oldNode) = *internalNodeChild(oldNode, *oldNumKeys - 1);
    (*oldNumKeys)--;

    uint32_t maxAfterSplit = getNodeMaxKey(table->pager, oldNode);
    uint32_t destinationPageNum = childMax < maxAfterSplit ? oldPageNum : newPageNum;

    internalNodeInsert(table, destinationPageNum, childPageNum);
    *nodeParent(child) = destinationPageNum;

    updateInternalNodeKey(parent, oldMax, getNodeMaxKey(table->pager, oldNode));

    if(!splittingRoot) {
        internalNodeInsert(table, *nodeParent(oldNode), newPageNum);
        *nodeParent(newNode) = *nodeParent(oldNode);
    }
}