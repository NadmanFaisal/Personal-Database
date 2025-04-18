#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>
#include "InputBuffer.h"

void logOutput(const char *fileName, const char *logMode, const char *format, ...);
void readInputFromFile(INPUTBUFFER *node, const char *filename);
void clearLog(const char *fileName);

#endif