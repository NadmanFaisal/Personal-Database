#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "InputBuffer.h"
#include "MetaCommand.h"
#include "Row.h"
#include "Statement.h"
#include "Table.h"
#include "DB.h"


void printPrompt(void) {
    printf("db > ");
}

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }

    char *fileName = argv[1];
    TABLE *table = openDB(fileName);
    INPUTBUFFER *buffer = createBuffer();
    while(true) {
        printPrompt();
        readInput(buffer);
        
        if(buffer->buffer[0] == '.') {
            switch (doMetaCommand(buffer, table)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command: '%s'\n", buffer->buffer);
                    continue;
            }
        }

        STATEMENT statement;
        switch (prepareStatements(buffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at the start of '%s'\n", buffer->buffer);
                continue;
            case PREPARE_SYNTAX_ERROR:
                printf("Syntax error. Could not parse statement '%s'\n", buffer->buffer);
                continue;
            case PREPARE_STRING_TOO_LONG:
                printf("String is too long.\n");
                continue;
            case PREPARE_NEGATIVE_ID:
                printf("ID must be a positive integer.\n");
                continue;
        }

        switch(executeStatement(&statement, table)) {
            case EXECUTE_SUCCESS:
                printf("Executed!\n");
                break;
            case EXECUTE_TABLE_FULL:
                printf("Error: Table is full.\n");
                break;
        }
    }
    
    return 0;
}