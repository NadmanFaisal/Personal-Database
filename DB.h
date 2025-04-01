#ifndef DB_H
#define DB__H

#include <unistd.h>

#include "Table.h"
#include "Pager.h"

TABLE *openDB(const char *fileName);
void closeDB(TABLE *table);

#endif