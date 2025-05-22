#ifndef PLAINFUCK_H
#define PLAINFUCK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MEMORY_SIZE 30000
#define STACK_SIZE 1000
#define MAX_TOKEN_LENGTH 256

typedef struct {
    char **tokens;
    int count;
    int capacity;
} TokenArray;

extern int line_number;
extern int token_position;

#endif
