#ifndef STATEMENT_H
#define STATEMENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "InputBuffer.h"

#define MAX_CHARS 256
#define COLUMN_EMAIL_SIZE 255
#define COLUMN_USERNAME_SIZE 32

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} ROW;

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
} PrepareResults;

PrepareResults prepareStatements(INPUTBUFFER *node, STATEMENT *statement);
void executeStatement(STATEMENT *statement);

#endif