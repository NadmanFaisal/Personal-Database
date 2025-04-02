#ifndef METACOMMAND_H
#define METACOMMAND_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "InputBuffer.h"
#include "Table.h"
#include "DB.h"

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNRECOGNIZED_COMMAND,
} MetaCommandResult;

MetaCommandResult doMetaCommand(INPUTBUFFER *node, TABLE *table);

#endif