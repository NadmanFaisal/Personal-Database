#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "Row.h"

void serialize_row(ROW *row, void *destination) {
    memcpy(destination + ID_OFFSET, &(row->id), ID_SIZE);
    strncpy(destination + USERNAME_OFFSET, row->username, USERNAME_SIZE);
    strncpy(destination + EMAIL_OFFSET, row->email, EMAIL_SIZE);
}

void deserialize_row(void *source, ROW *destination) {
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void printRow(ROW *row) {
    printf("(%d, %s, %s)\n", row->id, row->username, row->email);
}