#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "InputBuffer.h"

#define MAX_CHARS 256

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
} StatementType;

typedef struct {
    StatementType type;
} STATEMENT;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT,
} PrepareResults;

PrepareResults prepareStatements(INPUTBUFFER *node, STATEMENT *statement) {
    if(strncmp(node->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }

    if(strncmp(node->buffer, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

void executeStatement(STATEMENT *statement) {
    switch (statement->type) {
    case STATEMENT_INSERT:
        printf("INSERT HAS BEEN REQUESTED\n");
        break;
    case STATEMENT_SELECT:
        printf("SELECT HAS BEEN REQUESTED\n");
        break;
    }
}