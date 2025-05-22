#include "includes/interpreter.h"
#include "includes/utils.h"

void execute_program(TokenArray tokens) {
    unsigned char *memory = calloc(MEMORY_SIZE, sizeof(unsigned char));
    if (!memory) {
        error_exit("Memory allocation failed for program memory", NULL);
    }
    
    int memory_pointer = 0;
    int *loop_stack = malloc(STACK_SIZE * sizeof(int));
    if (!loop_stack) {
        error_exit("Memory allocation failed for loop stack", NULL);
    }
    
    int stack_pointer = 0;
    
    for (int i = 0; i < tokens.count; i++) {
        token_position = i + 1;
        char *token = tokens.tokens[i];
        
        if (strcmp(token, "inc") == 0) {
            int n = atoi(tokens.tokens[i + 1]);
            if (memory[memory_pointer] + n > 255) {
                error_exit("Cell overflow: value would exceed 255", token);
            }
            memory[memory_pointer] += n;
            i++;
        }
        else if (strcmp(token, "dec") == 0) {
            int n = atoi(tokens.tokens[i + 1]);
            if (memory[memory_pointer] < n) {
                error_exit("Cell underflow: value would go below 0", token);
            }
            memory[memory_pointer] -= n;
            i++;
        }
        else if (strncmp(token, "move_right", 10) == 0) {
            int n = 1;
            if (strlen(token) > 10) {
                n = atoi(token + 11);
            }
            if (memory_pointer + n >= MEMORY_SIZE) {
                error_exit("Memory pointer overflow: would exceed memory bounds", token);
            }
            memory_pointer += n;
        }
        else if (strncmp(token, "move_left", 9) == 0) {
            int n = 1;
            if (strlen(token) > 9) {
                n = atoi(token + 10);
            }
            if (memory_pointer - n < 0) {
                error_exit("Memory pointer underflow: would go below 0", token);
            }
            memory_pointer -= n;
        }
        else if (strcmp(token, "print") == 0) {
            if (putchar(memory[memory_pointer]) == EOF) {
                error_exit("Output error occurred", token);
            }
        }
        else if (strcmp(token, "input") == 0) {
            int ch = getchar();
            if (ch == EOF) {
                memory[memory_pointer] = 0;
            } else {
                memory[memory_pointer] = (unsigned char)ch;
            }
        }
        else if (strcmp(token, "loop_start") == 0) {
            if (stack_pointer >= STACK_SIZE) {
                error_exit("Loop nesting too deep: stack overflow", token);
            }
            if (memory[memory_pointer] == 0) {
                int bracket_count = 1;
                int j = i + 1;
                while (j < tokens.count && bracket_count > 0) {
                    if (strcmp(tokens.tokens[j], "loop_start") == 0) bracket_count++;
                    if (strcmp(tokens.tokens[j], "loop_end") == 0) bracket_count--;
                    j++;
                }
                i = j - 1;
            } else {
                loop_stack[stack_pointer] = i;
                stack_pointer++;
            }
        }
        else if (strcmp(token, "loop_end") == 0) {
            if (stack_pointer <= 0) {
                error_exit("Internal error: loop stack underflow", token);
            }
            if (memory[memory_pointer] != 0) {
                i = loop_stack[stack_pointer - 1];
            } else {
                stack_pointer--;
            }
        }
    }
    
    free(memory);
    free(loop_stack);
}