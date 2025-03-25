#include <stdint.h>

#include "Row.h"

#define TABLE_MAX_PAGES 100
static const uint32_t PAGE_SIZE = 4096;
static const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;

typedef struct {
    uint32_t numRows;
    void *pages[TABLE_MAX_PAGES];
} TABLE;

void *rowSlot(TABLE *table, uint32_t rowNum);