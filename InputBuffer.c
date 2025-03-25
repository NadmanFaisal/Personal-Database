#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "InputBuffer.h"

#define MAX_CHARS 256

INPUTBUFFER *createBuffer() {
    INPUTBUFFER *node = (INPUTBUFFER *) malloc(sizeof(INPUTBUFFER));
    node->buffer = (char *)malloc(MAX_CHARS * sizeof(char));
    node->bufferLength = 0;
    node->inputLength = 0;
    
    return node;
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
}