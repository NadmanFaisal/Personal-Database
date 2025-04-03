#ifndef STATEMENT_H
#define STATEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "InputBuffer.h"
#include "Table.h"
#include "Row.h"
#include "Statement.h"
#include "Cursor.h"
#include "Node.h"

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
} StatementType;

typedef struct {
    StatementType type;
    ROW rowToInsert;
} STATEMENT;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNRECOGNIZED_STATEMENT,
    PREPARE_STRING_TOO_LONG,
    PREPARE_NEGATIVE_ID,
} PrepareResults;

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL
} ExecuteResult;

PrepareResults prepareStatements(INPUTBUFFER *node, STATEMENT *statement);
ExecuteResult executeInsert(STATEMENT *statement, TABLE *table);
ExecuteResult executeSelect(STATEMENT *statement, TABLE *table);
ExecuteResult executeStatement(STATEMENT *statement, TABLE *table);

#endif