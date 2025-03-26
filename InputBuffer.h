#ifndef INPUTBUFFER_H
#define INPUTBUFFER_H

#include <stdlib.h>

#define MAX_CHARS 1024

typedef struct {
    char *buffer;
    size_t bufferLength;        // Guaranteed to be a non-negative value of buffer length
    size_t inputLength;         // Guaranteed to be a non-negative value of input length
} INPUTBUFFER;

INPUTBUFFER *createBuffer();
void closeInputBuffer(INPUTBUFFER *node);
void readInput(INPUTBUFFER *node);

#endif