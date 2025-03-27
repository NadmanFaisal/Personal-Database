#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "InputBuffer.h"
#include "Statement.h"
#include "Table.h"
#include "Row.h"

#define COLUMN_EMAIL_SIZE 255
#define COLUMN_USERNAME_SIZE 32

PrepareResults prepareInsert(INPUTBUFFER *buffer, STATEMENT *statement) {
    statement->type = STATEMENT_INSERT;

    char *keyword = strtok(buffer->buffer, " ");
    char *idString = strtok(NULL, " ");
    char *username = strtok(NULL, " ");
    char *email = strtok(NULL, " ");

    if(idString == NULL || username == NULL || email == NULL) {
        return PREPARE_SYNTAX_ERROR;
    }

    int id = atoi(idString);
    if(strlen(username) > COLUMN_USERNAME_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    if(strlen(email) > COLUMN_EMAIL_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    statement->rowToInsert.id = id;
    strcpy(statement->rowToInsert.username, username);
    strcpy(statement->rowToInsert.email, email);

    return PREPARE_SUCCESS;
}

PrepareResults prepareStatements(INPUTBUFFER *node, STATEMENT *statement) {
    if(strncmp(node->buffer, "insert", 6) == 0) {
        return prepareInsert(node, statement);
    }

    if(strncmp(node->buffer, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }

    return PREPARE_UNRECOGNIZED_STATEMENT;
}

ExecuteResult executeInsert(STATEMENT *statement, TABLE *table) {
    if(table->numRows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    ROW *rowToInsert = &(statement->rowToInsert);
    void *destination = rowSlot(table, table->numRows);
    serialize_row(rowToInsert, destination);
    table->numRows += 1;

    return EXECUTE_SUCCESS;
}

ExecuteResult executeSelect(STATEMENT *statement, TABLE *table) {
    ROW row;
    for(uint32_t i = 0; i < table->numRows; i++) {
        deserialize_row(rowSlot(table, i), &row);
        printRow(&row);
    }

    return EXECUTE_SUCCESS;
}

ExecuteResult executeStatement(STATEMENT *statement, TABLE *table) {
    switch (statement->type) {
        case STATEMENT_INSERT:
            return executeInsert(statement, table);
        case STATEMENT_SELECT:
            return executeSelect(statement, table);
    }
}