#include "MetaCommand.h"

void printConstants() {
    printf("ROW_SIZE: %d\n", ROW_SIZE);
    printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}

void printLeafNode(void *node) {
    uint32_t cellNums = *leafNodeNumCells(node);
    printf("Leaf (size %d)\n", cellNums);

    for(uint32_t i = 0; i < cellNums; i++) {
        uint32_t key = *leafNodeKey(node, i);
        printf("    - %d : %d\n", i, key);
    }
}

MetaCommandResult doMetaCommand(INPUTBUFFER *node, TABLE *table) {
    if(strcmp(node->buffer, ".exit") == 0) {
        closeDB(table);
        exit(0);
    } else if(strcmo(node->buffer, ".constants") == 0) {
        printf("Constants:\n");
        printConstants();
        return META_COMMAND_SUCCESS;
    } else if(strcmp(node->buffer, ".btree") == 0) {
        printf("Tree:\n");
        printLeafNode(getPage(table->pager, 0));
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}