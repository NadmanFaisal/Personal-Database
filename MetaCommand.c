#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "InputBuffer.h"
#include "MetaCommand.h"

#define MAX_CHARS 256

MetaCommandResult doMetaCommand(INPUTBUFFER *node) {
    if(strcmp(node->buffer, ".exit") == 0) {
        exit(0);
    } else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}