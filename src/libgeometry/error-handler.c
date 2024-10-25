#include <libgeometry/error-handler.h>

void throw_line_error(
        error_type type,
        char* message,
        char* source_line,
        size_t line,
        size_t at)
{
    char* error_name = get_error_name_by_type(type);

    printf("\n\n%s\n", source_line);

    for (size_t i = 0; i < at; i++) {
        printf(" ");
    }

    printf("^\n\n%s at line %zu, position %zu | %s\n\n\n",
           error_name,
           line + 1,
           at + 1,
           message);

    exit(1);
}

void throw_error(error_type type, char* message)
{
    char* error_name = get_error_name_by_type(type);

    printf("\n\n%s | %s\n\n\n", error_name, message);

    exit(1);
}

char* get_error_name_by_type(error_type type)
{
    if (type == LEXER)
        return "LEXER ERROR";

    if (type == PARSER)
        return "PARSER ERROR";

    return "ERROR";
}
