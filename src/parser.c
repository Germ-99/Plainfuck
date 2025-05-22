#include "includes/parser.h"
#include "includes/utils.h"

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