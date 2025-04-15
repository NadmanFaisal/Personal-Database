#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <stdarg.h>

void logOutput(const char *format, ...);
void clearLog();

#endif