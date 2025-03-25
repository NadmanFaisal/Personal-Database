#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "InputBuffer.h"
#include "Statement.h"
#include "MetaCommand.h"

#define MAX_CHARS 256

void printPrompt(void) {
    printf("db > ");
}

int main(int argc, char **argv) {
    INPUTBUFFER *buffer = createBuffer();
    while(true) {
        printPrompt();
        readInput(buffer);
        
        if(buffer->buffer[0] == '.') {
            switch (doMetaCommand(buffer)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command: '%s'\n", buffer->buffer);
                    continue;
            }
        }

        STATEMENT *statement = (STATEMENT *)malloc(sizeof(STATEMENT));
        switch (prepareStatements(buffer, statement)) {
        case PREPARE_SUCCESS:
            executeStatement(statement);
            break;
        case PREPARE_UNRECOGNIZED_STATEMENT:
            printf("Unrecognized keyword at the start of '%s'\n", buffer->buffer);
            break;
        case PREPARE_SYNTAX_ERROR:
            printf("Unrecognized schema '%s'\n", buffer->buffer);
        default:
            break;
        }
    }
    
    return 0;
}