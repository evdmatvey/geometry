#include <libgeometry/token.h>

token get_token(char* lexeme)
{
    token tok;
    tok.lexeme = lexeme;
    tok.type = IDENTIFIER;

    if (strcmp(lexeme, "(") == 0)
        tok.type = LPAR;

    if (strcmp(lexeme, ")") == 0)
        tok.type = RPAR;

    if (strcmp(lexeme, ",") == 0)
        tok.type = COMMA;

    if (strcmp(lexeme, " ") == 0)
        tok.type = SPACE;

    if (is_float_number(lexeme))
        tok.type = NUMBER;
    if (strcmp(lexeme, "~") == 0) {
        tok.type = END;
    }

    return tok;
}

char* get_token_type_name(token_type* type)
{
    switch (*type) {
    case IDENTIFIER:
        return "IDENTIFIER";
    case LPAR:
        return "LPAR";
    case RPAR:
        return "RPAR";
    case NUMBER:
        return "NUMBER";
    case COMMA:
        return "COMMA";
    case SPACE:
        return "SPACE";
    case END:
        return "END LINE";

    default:
        return "UNEXPECTED";
    }
}

int is_float_number(const char* lexeme)
{
    int has_dot = 0;
    int has_minus = 0;

    for (size_t i = 0; i < strlen(lexeme); i++) {
        if (lexeme[i] == '.' && !has_dot) {
            has_dot++;
            continue;
        }

        if (lexeme[i] == '-' && !has_minus) {
            has_minus++;
            continue;
        }

        if (has_dot > 1 || has_minus > 1) {
            return 0;
        }

        if (lexeme[i] < '0' || lexeme[i] > '9') {
            return 0;
        }
    }

    return 1;
}

void print_token(token* tok)
{
    printf("Token <%s> with type %s\n",
           (*tok).lexeme,
           get_token_type_name(&(*tok).type));
}
