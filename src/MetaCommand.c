#include "MetaCommand.h"

void printConstants() {
    printf("ROW_SIZE: %d\n", ROW_SIZE);
    printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}

void indent(uint32_t level) {
    for(uint32_t i = 0; i < level; i++) {
        printf("    ");
    }
}
void printTree(PAGER *pager, uint32_t pageNum, uint32_t indentationLevel) {
    void *node = getPage(pager, pageNum);
    uint32_t numKeys, child;

    switch(getNodeType(node)) {
        case(NODE_LEAF):
            numKeys = *leafNodeNumCells(node);
            indent(indentationLevel);
            printf("- leaf (size %d)\n", numKeys);
            
            for(uint32_t i = 0; i < numKeys; i++) {
                child = *internalNodeChild(node, i);
                printTree(pager, child, indentationLevel + 1);
                
                indent(indentationLevel + 1);
                printf("- key %d\n", *internalNodeKey(node, i));
            }

            child = *internalNodeRightChild(node);
            printTree(pager, child, indentationLevel + 1);
            break;
    }
}

MetaCommandResult doMetaCommand(INPUTBUFFER *node, TABLE *table) {
    if(strcmp(node->buffer, ".exit") == 0) {
        closeDB(table);
        exit(0);
    } else if(strcmp(node->buffer, ".constants") == 0) {
        printf("Constants:\n");
        printConstants();
        return META_COMMAND_SUCCESS;
    } else if(strcmp(node->buffer, ".btree") == 0) {
        printf("Tree:\n");
        printTree(table->pager, 0, 0);
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}