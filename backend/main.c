#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Headers/InputBuffer.h"
#include "Headers/MetaCommand.h"
#include "Headers/Row.h"
#include "Headers/Statement.h"
#include "Headers/Table.h"
#include "Headers/DB.h"
#include "Headers/Server.h"
#include "Headers/Logger.h"

pid_t server_pid = -1;

void printPrompt(void) {
    printf("db > ");
}

void readInputFromFile(INPUTBUFFER *node, const char *filename) {
    FILE *cmdFile = fopen(filename, "r");
    if (!cmdFile) {
        printf("Command file not found\n");
        return;
    }

    memset(node->buffer, 0, MAX_CHARS);
    node->inputLength = 0;

    size_t bytesRead = fread(node->buffer, 1, MAX_CHARS - 1, cmdFile);
    fclose(cmdFile);
    remove(filename);
    
    if (bytesRead == 0) {
        node->buffer[0] = '\0';
        node->inputLength = 0;
        return;
    }
    
    node->buffer[bytesRead] = '\0';
    node->inputLength = strlen(node->buffer);
    
    while (node->inputLength > 0 &&
          (node->buffer[node->inputLength - 1] == '\n' ||
           node->buffer[node->inputLength - 1] == '\r')) {
        node->buffer[--node->inputLength] = '\0';
    }
}

int main(int argc, char **argv) {

    if(argc < 2) {
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }

    
    pid_t pid = fork();
    
    if (pid == 0) {
        struct Server server = serverConstructor(AF_INET, 3001, SOCK_STREAM, 0, 10, INADDR_ANY, launch);
        server.launch(&server);
        exit(0);
    } else {
        server_pid = pid;
    }
    
    char *fileName = argv[1];
    TABLE *table = openDB(fileName);
    INPUTBUFFER *buffer = createBuffer();
    while(true) {
        printPrompt();
        
        while (access("command.txt", F_OK) == -1) {
            usleep(100000);
        }
        
        readInputFromFile(buffer, "command.txt");
        clearLog("output.txt");
        
        if(buffer->buffer[0] == '.') {
            switch (doMetaCommand(buffer, table)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command: '%s'\n", buffer->buffer);
                    logOutput("output.txt", "a", "Unrecognized command: '%s'\n", buffer->buffer);
                    continue;
            }
        }

        STATEMENT statement;
        switch (prepareStatements(buffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at the start of '%s'\n", buffer->buffer);
                logOutput("output.txt", "a", "Unrecognized keyword at the start of '%s'\n", buffer->buffer);
                continue;
            case PREPARE_SYNTAX_ERROR:
                printf("Syntax error. Could not parse statement '%s'\n", buffer->buffer);
                logOutput("output.txt", "a", "Syntax error. Could not parse statement '%s'\n", buffer->buffer);
                continue;
            case PREPARE_STRING_TOO_LONG:
                printf("String is too long.\n");
                logOutput("output.txt", "a", "String is too long.\n");
                continue;
            case PREPARE_NEGATIVE_ID:
                printf("ID must be a positive integer.\n");
                logOutput("output.txt", "a", "ID must be a positive integer.\n");
                continue;
        }

        switch(executeStatement(&statement, table)) {
            case EXECUTE_SUCCESS:
                printf("Executed!\n");
                logOutput("output.txt", "a", "Executed!\n");
                break;
            case EXECUTE_TABLE_FULL:
                printf("Error: Table is full.\n");
                logOutput("output.txt", "a", "Error: Table is full.\n");
                break;
            case EXECUTE_DUPLICATE_KEY:
                printf("Error: Duplicate keys.\n");
                logOutput("output.txt", "a", "Error: Duplicate keys.\n");
                break;
        }
    }
    
    return 0;
}