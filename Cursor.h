#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "Table.h"

typedef struct {
    TABLE *table;
    u_int32_t rowNum;
    bool endOfTable;
} CURSOR;

