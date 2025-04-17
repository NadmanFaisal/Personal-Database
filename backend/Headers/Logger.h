#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

void logOutput(const char *fileName, const char *logMode, const char *format, ...);
void clearLog(const char *fileName);

#endif