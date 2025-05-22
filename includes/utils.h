#ifndef UTILS_H
#define UTILS_H

#include "plainfuck.h"

void error_exit(const char *message, const char *token);
int is_valid_number(const char *str);

#endif