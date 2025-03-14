#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <stdint.h>

// Constants for Row class

#define COLUMN_EMAIL_SIZE 255
#define COLUMN_USERNAME_SIZE 32

// // Accessing member variable without actually creating the variable from the struct
// #define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

static const uint32_t ID_SIZE = sizeof(uint32_t);
static const uint32_t USERNAME_SIZE = COLUMN_USERNAME_SIZE + 1;
static const uint32_t EMAIL_SIZE = COLUMN_EMAIL_SIZE + 1;

static const uint32_t ID_OFFSET = 0;
static const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
static const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
static const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

// Constants for table class

#define TABLE_MAX_PAGES 100

static const uint32_t PAGE_SIZE = 4096;
static const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
static const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

#endif
