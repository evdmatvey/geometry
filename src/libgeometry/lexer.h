#pragma once

#include <libgeometry/error-handler.h>
#include <libgeometry/token.h>
#include <libgeometry/utilities.h>

void validate_identifiers(token** tokens, size_t* size, char* line, size_t* at);

void throw_identifier_error(token* tok, char* line, size_t* line_index);

void push_lexeme(
        char*** lexemes, size_t* size, size_t* capacity, const char* lexeme);

void push_tokens(token*** lines, size_t* size, size_t* capacity, token* tokens);

char* get_substring(const char* str, size_t start_index, size_t end_index);

char** get_lexemes_from_string(char* line, size_t* size);

int is_separate_symbol(const char symbol);

int is_correct_identifier(token* identifier);

size_t index_of(const char* str, const char* substr);

token* get_tokens_from_lexemes(char*** lexemes, size_t* size);

token** read_file(char* file_path, size_t* lines_size);
