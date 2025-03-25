#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "InputBuffer.h"
#include "Statement.h"

#define MAX_CHARS 256
#define COLUMN_EMAIL_SIZE 255
#define COLUMN_USERNAME_SIZE 32

PrepareResults prepareStatements(INPUTBUFFER *node, STATEMENT *statement) {
    if(strncmp(node->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        int argsPreped = sscanf(node->buffer, "insert %d %s %s",
            &(statement->rowToInsert.id),
            statement->rowToInsert.username,
            statement->rowToInsert.email
        );

        if(argsPreped < 3) {
            return PREPARE_SYNTAX_ERROR;
        }
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