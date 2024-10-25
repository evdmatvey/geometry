#pragma once

#include <libgeometry/token.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    IDENTIFIER, //          circle | triangle
    LPAR,       //          (                       SEPARATOR
    RPAR,       //          )                       SEPARATOR
    NUMBER,     //          dobule
    COMMA,      //          ,                       SEPARATOR
    SPACE,      //                                  SEPARATOR
    END         //          ~                       ENDLINE
} token_type;

typedef struct {
    char* lexeme;
    token_type type;
} token;

token get_token(char* lexeme);

void print_token(token* tok);

char* get_token_type_name(token_type* type);

int is_float_number(const char* lexeme);
