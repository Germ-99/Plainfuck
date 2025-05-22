#include "includes/plainfuck.h"

int line_number = 1;
int token_position = 0;

void error_exit(const char *message, const char *token) {
    if (token) {
        fprintf(stderr, "Error at line %d, token %d ('%s'): %s\n", line_number, token_position, token, message);
    } else {
        fprintf(stderr, "Error at line %d, token %d: %s\n", line_number, token_position, message);
    }
    exit(1);
}

int is_valid_number(const char *str) {
    if (!str || *str == '\0') return 0;
    if (*str == '-') str++;
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

TokenArray tokenize(char *code) {
    TokenArray tokens = {0};
    tokens.capacity = 1000;
    tokens.tokens = malloc(tokens.capacity * sizeof(char*));
    if (!tokens.tokens) {
        error_exit("Memory allocation failed for tokens", NULL);
    }
    
    char *token = strtok(code, " \n\t\r");
    while (token != NULL) {
        if (tokens.count >= tokens.capacity) {
            tokens.capacity *= 2;
            tokens.tokens = realloc(tokens.tokens, tokens.capacity * sizeof(char*));
            if (!tokens.tokens) {
                error_exit("Memory reallocation failed for tokens", NULL);
            }
        }
        
        tokens.tokens[tokens.count] = malloc(strlen(token) + 1);
        if (!tokens.tokens[tokens.count]) {
            error_exit("Memory allocation failed for token", token);
        }
        strcpy(tokens.tokens[tokens.count], token);
        tokens.count++;
        
        token = strtok(NULL, " \n\t\r");
    }
    
    return tokens;
}

void validate_syntax(TokenArray tokens) {
    int loop_depth = 0;
    
    for (int i = 0; i < tokens.count; i++) {
        token_position = i + 1;
        char *token = tokens.tokens[i];
        
        if (strcmp(token, "inc") == 0) {
            if (i + 1 >= tokens.count || !is_valid_number(tokens.tokens[i + 1])) {
                error_exit("inc command requires a number parameter", token);
            }
            i++;
        }
        else if (strcmp(token, "dec") == 0) {
            if (i + 1 >= tokens.count || !is_valid_number(tokens.tokens[i + 1])) {
                error_exit("dec command requires a number parameter", token);
            }
            i++;
        }
        else if (strncmp(token, "move_right", 10) == 0) {
            if (strlen(token) > 10) {
                if (token[10] != '_' || !is_valid_number(token + 11)) {
                    error_exit("Invalid move_right syntax. Use 'move_right' or 'move_right_N'", token);
                }
            }
        }
        else if (strncmp(token, "move_left", 9) == 0) {
            if (strlen(token) > 9) {
                if (token[9] != '_' || !is_valid_number(token + 10)) {
                    error_exit("Invalid move_left syntax. Use 'move_left' or 'move_left_N'", token);
                }
            }
        }
        else if (strcmp(token, "print") == 0) {
        }
        else if (strcmp(token, "input") == 0) {
        }
        else if (strcmp(token, "loop_start") == 0) {
            loop_depth++;
        }
        else if (strcmp(token, "loop_end") == 0) {
            loop_depth--;
            if (loop_depth < 0) {
                error_exit("Unmatched loop_end without corresponding loop_start", token);
            }
        }
        else if (is_valid_number(token)) {
        }
        else {
            error_exit("Unknown command", token);
        }
    }
    
    if (loop_depth > 0) {
        error_exit("Unmatched loop_start without corresponding loop_end", NULL);
    }
}

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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename.pf>\n", argv[0]);
        return 1;
    }
    
    char *filename = argv[1];
    size_t len = strlen(filename);
    
    if (len < 3 || strcmp(filename + len - 3, ".pf") != 0) {
        fprintf(stderr, "Error: File must have .pf extension\n");
        return 1;
    }
    
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", filename);
        return 1;
    }
    
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    if (file_size < 0) {
        fprintf(stderr, "Error: Cannot determine file size\n");
        fclose(file);
        return 1;
    }
    
    fseek(file, 0, SEEK_SET);
    
    char *code = malloc(file_size + 1);
    if (!code) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        fclose(file);
        return 1;
    }
    
    size_t bytes_read = fread(code, 1, file_size, file);
    if (bytes_read != file_size) {
        fprintf(stderr, "Error: Failed to read complete file\n");
        free(code);
        fclose(file);
        return 1;
    }
    
    code[file_size] = '\0';
    fclose(file);
    
    if (file_size == 0) {
        fprintf(stderr, "Error: Empty file\n");
        free(code);
        return 1;
    }
    
    TokenArray tokens = tokenize(code);
    if (tokens.count == 0) {
        fprintf(stderr, "Error: No valid tokens found\n");
        free(code);
        return 1;
    }
    
    validate_syntax(tokens);
    execute_program(tokens);
    
    for (int i = 0; i < tokens.count; i++) {
        free(tokens.tokens[i]);
    }
    free(tokens.tokens);
    free(code);
    
    return 0;
}
