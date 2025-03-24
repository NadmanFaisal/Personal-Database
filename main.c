#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CHARS 256

typedef struct {
    char *buffer;
    size_t bufferLength;        // Guaranteed to be a non-negative value of buffer length
    size_t inputLength;         // Guaranteed to be a non-negative value of input length
} INPUTBUFFER;

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND,
} MetaCommandResult;

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNRECOGNIZED_STATEMENT,
} PrepareResults;

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
} StatementType;

typedef struct {
    StatementType type;
} STATEMENT;

INPUTBUFFER *createBuffer() {
    INPUTBUFFER *node = (INPUTBUFFER *) malloc(sizeof(INPUTBUFFER));
    node->buffer = (char *)malloc(MAX_CHARS * sizeof(char));
    node->bufferLength = 0;
    node->inputLength = 0;
    
    return node;
}

MetaCommandResult doMetaCommand(INPUTBUFFER *node) {
    if(strcmp(node->buffer, ".exit") == 0) {
        exit(0);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

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

void closeInputBuffer(INPUTBUFFER *node) {
    free(node->buffer);
    free(node);
}

void readInput(INPUTBUFFER *node) {
    char string[MAX_CHARS];
    fgets(string, sizeof(string), stdin);
    
    if(string == NULL) {
        printf("Error reading input\n");
        exit(1);
    }

    string[strlen(string) - 1] = '\0';
    strcpy(node->buffer, string);
    node->inputLength = strlen(string);

    printf("The string in readinput is: %s\n", node->buffer);

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

        STATEMENT *statement;
        switch (prepareStatements(buffer, statement)) {
        case PREPARE_SUCCESS:
            executeStatement(statement);
            break;
        case PREPARE_UNRECOGNIZED_STATEMENT:
            printf("Unrecognized keyword at the start of '%s'\n", buffer->buffer);
        default:
            break;
        }
    }
    
    return 0;
}