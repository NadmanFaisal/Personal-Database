#ifndef DB_H
#define DB__H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Table.h"
#include "Pager.h"
#include "Node.h"

TABLE *openDB(const char *fileName);
void closeDB(TABLE *table);

#endif