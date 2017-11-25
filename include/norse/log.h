#ifndef LOG_NORSE_H
#define LOG_NORSE_H

#include <norse/types.h>

void log(char *msg);
void logf(char *msg, ...);

enum status init_log(char *name);

#endif
