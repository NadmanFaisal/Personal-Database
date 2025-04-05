#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

#include "Constants.h"
#include "Pager.h"
#include "Cursor.h"

// Forward declaration of cursor type to prevent circular dependency
struct CURSOR;
typedef struct CURSOR CURSOR;

#include "Row.h"

typedef enum {
    NODE_INTERNAL,
    NODE_LEAF
} NodeType;

void leafNodeSplitAndInsert(CURSOR *cursor, uint32_t key, ROW *value);
uint32_t *leafNodeNumCells(void *node);
void *leafNodeCell(void *node, uint32_t cellNum);
uint32_t *leafNodeKey(void *node, uint32_t cellNum);
void *leafNodeValue(void *node, uint32_t cellNum);
void leafNodeInsert(CURSOR *cursor, uint32_t key, ROW *value);
CURSOR *findLeafNode(TABLE *table, uint32_t pageNum, uint32_t key);
NodeType getNodeType(void *node);
void setNodeType(void *node, NodeType type);
void initializeLeafNode(void *node);
void createNewRoot(TABLE *table, uint32_t rightChildPageNum);
uint32_t *internalNodeNumKeys(void *node);
uint32_t* internalNodeRightChild(void* node);
uint32_t *internalNodeCell(void *node, uint32_t cellNum);
uint32_t *internalNodeChild(void *node, uint32_t childNum);
uint32_t *internalNodeKey(void *node, uint32_t keyNum);
uint32_t getNodeMaxKey(void *node);
bool isNodeRoot(void *root);
bool isNodeRoot(void *node);
void setNodeRoot(void *node, bool isRoot);
void initializeInternalNode(void *node);
uint32_t *leafNodeNextLeaf(void *node);

#endif