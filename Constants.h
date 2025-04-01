#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

#include "Row.h"

#define TABLE_MAX_PAGES 100

static const uint32_t PAGE_SIZE = 4096;
static const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
static const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

#endif
