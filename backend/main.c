#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
#include <windows.h>
#include <process.h>
#include <io.h>
#define access _access
#define F_OK 0
#define sleep_ms(ms) Sleep(ms)
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#define sleep_ms(ms) usleep((ms) * 1000)
#endif

#include "Headers/InputBuffer.h"
#include "Headers/MetaCommand.h"
#include "Headers/Row.h"
#include "Headers/Statement.h"
#include "Headers/Table.h"
#include "Headers/DB.h"
#include "Headers/Server.h"
#include "Headers/Logger.h"

#ifdef _WIN32
#define THREAD_RET unsigned __stdcall
#define THREAD_CALLCONV __stdcall
#else
#include <pthread.h>
#define THREAD_RET void*
#define THREAD_CALLCONV
#endif

#define COMMAND_FILE "CommsFiles/command.txt"
#define OUTPUT_FILE "CommsFiles/output.txt"

TABLE *table;
pid_t server_pid = -1;

void* THREAD_CALLCONV runServer(void* args) {
    struct Server server = serverConstructor(AF_INET, 3001, SOCK_STREAM, 0, 10, INADDR_ANY, launch);
    server.launch(&server);
    return 0;
}

void printPrompt(void) {
    printf("db > ");
}

int main(int argc, char **argv) {
    if(argc < 2) {
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }

#ifdef _WIN32
    _beginthread(runServer, 0, NULL);
#else
    pthread_t serverThread;
    pthread_create(&serverThread, NULL, runServer, NULL);
#endif

    char *fileName = argv[1];
    table = openDB(fileName);
    INPUTBUFFER *buffer = createBuffer();

    while(true) {
        printPrompt();

        while (access(COMMAND_FILE, F_OK) == -1) {
            sleep_ms(100);
        }

        readInputFromFile(buffer, COMMAND_FILE);
        clearLog(OUTPUT_FILE);

        if(buffer->buffer[0] == '.') {
            switch (doMetaCommand(buffer, table)) {
                case (META_COMMAND_SUCCESS):
                    continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                    printf("Unrecognized command: '%s'\n", buffer->buffer);
                    logOutput(OUTPUT_FILE, "a", "Unrecognized command: '%s'\n", buffer->buffer);
                    continue;
            }
        }

        STATEMENT statement;
        switch (prepareStatements(buffer, &statement)) {
            case PREPARE_SUCCESS:
                break;
            case PREPARE_UNRECOGNIZED_STATEMENT:
                printf("Unrecognized keyword at the start of '%s'\n", buffer->buffer);
                logOutput(OUTPUT_FILE, "a", "Unrecognized keyword at the start of '%s'\n", buffer->buffer);
                continue;
            case PREPARE_SYNTAX_ERROR:
                printf("Syntax error. Could not parse statement '%s'\n", buffer->buffer);
                logOutput(OUTPUT_FILE, "a", "Syntax error. Could not parse statement '%s'\n", buffer->buffer);
                continue;
            case PREPARE_STRING_TOO_LONG:
                printf("String is too long.\n");
                logOutput(OUTPUT_FILE, "a", "String is too long.\n");
                continue;
            case PREPARE_NEGATIVE_ID:
                printf("ID must be a positive integer.\n");
                logOutput(OUTPUT_FILE, "a", "ID must be a positive integer.\n");
                continue;
        }

        switch(executeStatement(&statement, table)) {
            case EXECUTE_SUCCESS:
                printf("Executed!\n");
                logOutput(OUTPUT_FILE, "a", "Executed!\n");
                break;
            case EXECUTE_TABLE_FULL:
                printf("Error: Table is full.\n");
                logOutput(OUTPUT_FILE, "a", "Error: Table is full.\n");
                break;
            case EXECUTE_DUPLICATE_KEY:
                printf("Error: Duplicate keys.\n");
                logOutput(OUTPUT_FILE, "a", "Error: Duplicate keys.\n");
                break;
        }
    }

    return 0;
}