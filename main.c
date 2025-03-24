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

INPUTBUFFER *createBuffer() {
    INPUTBUFFER *node = (INPUTBUFFER *) malloc(sizeof(INPUTBUFFER));
    node->buffer = NULL;
    node->bufferLength = 0;
    node->inputLength = 0;
    
    return node;
}

void closeInputBuffer(INPUTBUFFER *node) {
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
    node->buffer = string;
    node->inputLength = strlen(string);

    if(strcmp(node->buffer, ".exit") == 0) {
        closeInputBuffer(node);
        exit(0);
    } else {
        printf("Unrecognized input command: %s\n", node->buffer);
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
    }
    printf("FUUUUUUCL!\n");
    return 0;
}