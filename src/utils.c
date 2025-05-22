#include "includes/utils.h"

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