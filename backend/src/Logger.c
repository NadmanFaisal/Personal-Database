#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

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

void clearLog(const char *fileName) {
    FILE *file = fopen(fileName, "w");
    if(file) {
        fclose(file);
    }
}