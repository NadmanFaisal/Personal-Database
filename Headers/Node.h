#ifndef NODE_H
#define NODE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

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

uint32_t *leafNodeNumCells(void *node);
void *leafNodeCell(void *node, uint32_t cellNum);
uint32_t *leafNodeKey(void *node, uint32_t cellNum);
void *leafNodeValue(void *node, uint32_t cellNum);
void leafNodeInsert(CURSOR *cursor, uint32_t key, ROW *value);
CURSOR *findLeafNode(TABLE *table, uint32_t pageNum, uint32_t key);
NodeType getNodeType(void *node);
void setNodeType(void *node, NodeType type);
void initializeLeafNode(void *node);

#endif