#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MetaCommand.h"

MetaCommandResult doMetaCommand(INPUTBUFFER *node, TABLE *table) {
    if(strcmp(node->buffer, ".exit") == 0) {
        closeDB(table);
        exit(0);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}