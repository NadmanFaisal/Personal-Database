#ifndef ROW_H
#define ROW_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "Constants.h"

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE + 1];
    char email[COLUMN_EMAIL_SIZE + 1];
} ROW;

void serialize_row(ROW *row, void *destination);
void deserialize_row(void *source, ROW *destination);
void printRow(ROW *row);

#endif