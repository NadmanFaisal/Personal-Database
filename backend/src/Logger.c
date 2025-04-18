#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "Logger.h"

void logOutput(const char *fileName, const char *logMode, const char *format, ...) {
    FILE *file = fopen(fileName, logMode);
    if(!file) {
        printf("File was not created\n");
        exit(EXIT_FAILURE);
    }

    va_list args;
    va_start(args, format);
    vfprintf(file, format, args);
    va_end(args);

    fclose(file);
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

void clearLog(const char *fileName) {
    FILE *file = fopen(fileName, "w");
    if(file) {
        fclose(file);
    }
}