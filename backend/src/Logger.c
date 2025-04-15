#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

void logOutput(const char *format, ...) {
    FILE *file = fopen("output.txt", "a");
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

void clearLog() {
    FILE *file = fopen("output.txt", "w");
    if(file) {
        fclose(file);
    }
}