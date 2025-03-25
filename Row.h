#ifndef ROW_H
#define ROW_H

#include <stdint.h>

#define MAX_CHARS 256
#define COLUMN_EMAIL_SIZE 255
#define COLUMN_USERNAME_SIZE 32

// Accessing member variable without actually creating the variable from the struct
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

typedef struct {
    uint32_t id;
    char username[COLUMN_USERNAME_SIZE];
    char email[COLUMN_EMAIL_SIZE];
} ROW;


static const uint32_t ID_SIZE = size_of_attribute(ROW, id);
static const uint32_t USERNAME_SIZE = size_of_attribute(ROW, username);
static const uint32_t EMAIL_SIZE = size_of_attribute(ROW, email);

static const uint32_t ID_OFFSET = 0;
static const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
static const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
static const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;


void serialize_row(ROW *row, void *destination);
void deserialize_row(void *source, ROW *destination);
void printRow(ROW *row);

#endif