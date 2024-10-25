#include <libgeometry/lexer.h>

token** read_file(char* file_path, size_t* lines_size)
{
    token** lines_tokens = NULL;
    FILE* file = fopen(file_path, "r");

    size_t lines_capacity = 0;

    if (file == NULL) {
        throw_error(ERROR, "File not found! Please check your filepath!");
    }

    char line[100];
    while (fgets(line, sizeof(line), file)) {
        size_t size = 0;
        char** lexemes = get_lexemes_from_string(line, &size);
        token* tokens = get_tokens_from_lexemes(&lexemes, &size);

        validate_identifiers(&tokens, &size, line, lines_size);

        push_tokens(&lines_tokens, lines_size, &lines_capacity, tokens);
    }

    fclose(file);

    return lines_tokens;
}

token* get_tokens_from_lexemes(char*** lexemes, size_t* size)
{
    token* tokens = (token*)malloc(*size * sizeof(token));

    for (size_t i = 0; i < *size; i++) {
        token tok = get_token((*lexemes)[i]);
        tokens[i] = tok;
    }

    return tokens;
}

char** get_lexemes_from_string(char* line, size_t* size)
{
    char** lexemes = NULL;
    size_t capacity = 0, lexeme_start_index = 0, line_len = strlen(line);

    for (size_t i = 0; i < line_len; i++) {
        if (i == line_len - 1 && !is_separate_symbol(line[i])) {
            char* lexeme = get_substring(line, lexeme_start_index, i - 1);

            if (strcmp(lexeme, "") != 0) {
                push_lexeme(&lexemes, size, &capacity, lexeme);
            }
        }

        if (is_separate_symbol(line[i])) {
            char* lexeme = get_substring(line, lexeme_start_index, i - 1);

            if (strcmp(lexeme, "") != 0) {
                push_lexeme(&lexemes, size, &capacity, lexeme);
            }

            char* separator = (char*)malloc(2 * sizeof(char));
            separator[0] = line[i];
            separator[1] = '\0';

            push_lexeme(&lexemes, size, &capacity, separator);

            lexeme_start_index = i + 1;
        }
    }

    push_lexeme(&lexemes, size, &capacity, "~");

    return lexemes;
}

void validate_identifiers(token** tokens, size_t* size, char* line, size_t* at)
{
    for (size_t i = 0; i < *size; i++) {
        if ((*tokens)[i].type == IDENTIFIER
            && !is_correct_identifier(&((*tokens)[i]))) {
            throw_identifier_error(&((*tokens)[i]), line, at);
        }
    }
}

void throw_identifier_error(token* tok, char* line, size_t* line_index)
{
    size_t at = index_of(line, (*tok).lexeme);

    char message[100] = "Unexpected identifier <";
    strcat(message, (*tok).lexeme);
    strcat(message,
           ">! Please use only 'circle' "
           "or 'triangle'!");

    throw_line_error(LEXER, message, line, *line_index, at);
}

void push_lexeme(
        char*** lexemes, size_t* size, size_t* capacity, const char* lexeme)
{
    if (*capacity == *size) {
        *capacity += 2;
        char** tmp = (char**)realloc(*lexemes, *capacity * sizeof(char*));

        if (tmp != NULL) {
            *lexemes = tmp;
        } else {
            throw_error(
                    ERROR,
                    "Oops! Some issues with memory allocations in "
                    "'push_lexeme'!");
        }
    }

    int len = strlen(lexeme) + 1;

    (*lexemes)[*size] = (char*)malloc(len * sizeof(char));
    strcpy((*lexemes)[*size], lexeme);

    (*size)++;
}

void push_tokens(token*** lines, size_t* size, size_t* capacity, token* tokens)
{
    if (*capacity == *size) {
        *capacity += 2;
        token** tmp = (token**)realloc(*lines, *capacity * sizeof(token*));

        if (tmp != NULL) {
            *lines = tmp;
        } else {
            throw_error(
                    ERROR,
                    "Oops! Some issues with memory allocations in "
                    "'push_tokens'!");
        }
    }

    (*lines)[*size] = tokens;
    (*size)++;
}

char* get_substring(const char* str, size_t start_index, size_t end_index)
{
    int substring_length = end_index - start_index + 1;
    char* substring = (char*)malloc(substring_length + 1);

    if (substring == NULL) {
        throw_error(
                ERROR,
                "Oops! Some issues with memory allocations in "
                "'get_substring'!");
    }

    strncpy(substring, str + start_index, substring_length);
    substring[substring_length] = '\0';

    return substring;
}

int is_separate_symbol(const char symbol)
{
    return (symbol == '(' || symbol == ')' || symbol == ' ' || symbol == ',');
}

int is_correct_identifier(token* identifier)
{
    int is_circle = strcmp((*identifier).lexeme, "circle");
    int is_circle_upper = strcmp((*identifier).lexeme, "CIRCLE");
    int is_triangle = strcmp((*identifier).lexeme, "triangle");
    int is_triangle_upper = strcmp((*identifier).lexeme, "TRIANGLE");

    return is_circle == 0 || is_circle_upper == 0 || is_triangle == 0
            || is_triangle_upper == 0;
}
