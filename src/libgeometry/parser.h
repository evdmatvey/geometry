#pragma once

#include <libgeometry/error-handler.h>
#include <libgeometry/token.h>
#include <libgeometry/utilities.h>
#include <math.h>

#define POINTS_IN_TRIANGLE 4

typedef struct {
    double x;
    double y;
    double r;
} circle;

typedef struct {
    double x;
    double y;
} point;

typedef struct {
    token identifier;
    circle circle;
    point* points;
    size_t line_number;
} shape;

shape* parse_lines(token*** lines, size_t* size);

void check_circle(token** line, size_t line_index);

void check_triangle(token** line, size_t line_index);

void throw_unexpected_token_error(
        char* current_lexeme, char* next_lexeme, char* line, size_t line_index);

int is_circle(token identifier);

int is_valid_triangle(point** points);

double get_number(char* number_lexeme);

char* get_source_line_from_tokens(token** line);

circle get_circle_from_tokens(token** tokens, size_t line_index);

point* get_points_from_tokens(token** tokens, size_t line_index);
