#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum { LEXER, ERROR, PARSER } error_type;

char* get_error_name_by_type(error_type type);

void throw_line_error(
        error_type type,
        char* message,
        char* source_line,
        size_t line,
        size_t at);

void throw_error(error_type type, char* message);
