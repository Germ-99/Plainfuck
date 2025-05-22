#ifndef PARSER_H
#define PARSER_H

#include "plainfuck.h"

TokenArray tokenize(char *code);
void validate_syntax(TokenArray tokens);

#endif