#include "Statement.h"

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
    if(id < 0) {
        return PREPARE_NEGATIVE_ID;
    }
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
    CURSOR *cursor = tableEnd(table);

    serialize_row(rowToInsert, cursorValue(cursor));
    table->numRows += 1;

    free(cursor);

    return EXECUTE_SUCCESS;
}

ExecuteResult executeSelect(STATEMENT *statement, TABLE *table) {
    CURSOR *cursor = tableStart(table);
    ROW row;
    while(!(cursor->endOfTable)) {
        deserialize_row(cursorValue(cursor), &row);
        printRow(&row);
        cursorAdvance(cursor);
    }

    free(cursor);

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