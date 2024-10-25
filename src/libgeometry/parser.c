#include <libgeometry/parser.h>

shape* parse_lines(token*** lines, size_t* size)
{
    shape* shapes = (shape*)malloc(*size * sizeof(shape));

    for (size_t i = 0; i < *size; i++) {
        token* line = (*lines)[i];
        shape shape;

        shape.identifier = line[0];
        shape.line_number = i;

        if (is_circle(line[0])) {
            check_circle(&line, i);
            shape.points = NULL;

            shape.circle = get_circle_from_tokens(&line, i);
        } else {
            check_triangle(&line, i);
            shape.points = get_points_from_tokens(&line, i);
        }

        shapes[i] = shape;
    }

    return shapes;
}

void check_circle(token** line, size_t line_index)
{
    token_type correct_circle[9] = {
            IDENTIFIER,
            LPAR,
            NUMBER,
            SPACE,
            NUMBER,
            COMMA,
            SPACE,
            NUMBER,
            RPAR,
    };

    size_t i = 1;

    while ((*line)[i].type != END) {
        if ((*line)[i].type != correct_circle[i]) {
            char* source_line = get_source_line_from_tokens(line);
            strcat(source_line, "~");

            if ((*line)[i + 1].type != END) {
                throw_unexpected_token_error(
                        (*line)[i].lexeme,
                        (*line)[i + 1].lexeme,
                        source_line,
                        line_index);
                break;
            } else {
                throw_unexpected_token_error(
                        (*line)[i].lexeme, "~", source_line, line_index);
                break;
            }
        }
        i++;
    }
}

void check_triangle(token** line, size_t line_index)
{
    token_type correct_triangle[23] = {
            IDENTIFIER, LPAR,  LPAR,   NUMBER, SPACE,  NUMBER, COMMA, SPACE,
            NUMBER,     SPACE, NUMBER, COMMA,  SPACE,  NUMBER, SPACE, NUMBER,
            COMMA,      SPACE, NUMBER, SPACE,  NUMBER, RPAR,   RPAR,
    };

    size_t i = 1;

    while ((*line)[i].type != END) {
        if ((*line)[i].type != correct_triangle[i]) {
            char* source_line = get_source_line_from_tokens(line);
            strcat(source_line, "~");

            if ((*line)[i + 1].type != END) {
                throw_unexpected_token_error(
                        (*line)[i].lexeme,
                        (*line)[i + 1].lexeme,
                        source_line,
                        line_index);
                break;
            } else {
                throw_unexpected_token_error(
                        (*line)[i].lexeme, "~", source_line, line_index);
                break;
            }
        }
        i++;
    }
}

char* get_source_line_from_tokens(token** line)
{
    char* source_line = (char*)malloc(100 * sizeof(char));
    size_t i = 0;

    source_line[0] = '\0';

    while ((*line)[i].type != END) {
        strcat(source_line, (*line)[i].lexeme);
        i++;
    }

    return source_line;
}

void throw_unexpected_token_error(
        char* current_lexeme, char* next_lexeme, char* line, size_t line_index)
{
    char token[100];
    token[0] = '\0';

    strcat(token, current_lexeme);
    strcat(token, next_lexeme);

    size_t at = index_of(line, token);
    size_t line_size = strlen(line);
    line[line_size - 1] = '\0';

    char message[100] = "Unexpected token <";
    strcat(message, current_lexeme);
    strcat(message, ">!");

    throw_line_error(PARSER, message, line, line_index, at);
}

int is_circle(token identifier)
{
    int is_lowercase = strcmp(identifier.lexeme, "circle");
    int is_uppercase = strcmp(identifier.lexeme, "CIRCLE");

    return is_lowercase == 0 || is_uppercase == 0;
}

circle get_circle_from_tokens(token** tokens, size_t line_index)
{
    size_t i = 0, number_count = 0;

    circle circle;

    while ((*tokens)[i].type != END) {
        if ((*tokens)[i].type == NUMBER) {
            if (number_count > 2) {
                throw_error(ERROR, "Unexpected amount of numbers in circle!");
            }

            if (number_count == 0) {
                circle.x = get_number((*tokens)[i].lexeme);
            }

            if (number_count == 1) {
                circle.y = get_number((*tokens)[i].lexeme);
            }

            if (number_count == 2) {
                double radius = get_number((*tokens)[i].lexeme);

                if (radius < 0) {
                    char* line = get_source_line_from_tokens(tokens);

                    throw_line_error(
                            PARSER,
                            "Invalid radius value! Please use only positive "
                            "values!",
                            line,
                            line_index,
                            0);
                }

                circle.r = radius;
            }

            number_count++;
        }
        i++;
    }

    return circle;
}

point* get_points_from_tokens(token** tokens, size_t line_index)
{
    point* points = (point*)malloc(POINTS_IN_TRIANGLE * sizeof(point));
    size_t i = 0, numbers_count = 0, points_count = 0;
    point current_point;

    while ((*tokens)[i].type != END) {
        if ((*tokens)[i].type == NUMBER) {
            double number = get_number((*tokens)[i].lexeme);

            if ((numbers_count % 2 == 0)) {
                current_point.x = number;
            } else {
                current_point.y = number;
                points[points_count] = current_point;
                points_count++;
            }
            numbers_count++;
        }
        i++;
    }

    if (points[0].x != points[3].x || points[0].y != points[3].y) {
        char* line = get_source_line_from_tokens(tokens);

        throw_line_error(
                PARSER,
                "Invalid radius value! Please use only positive "
                "values!",
                line,
                line_index,
                0);
    }

    if (is_valid_triangle(&points) == 0) {
        char* line = get_source_line_from_tokens(tokens);

        throw_line_error(
                PARSER,
                "Invalid triangle points! Please check their again!",
                line,
                line_index,
                0);
    }

    return points;
}

double get_number(char* number_lexeme)
{
    char* endptr;
    double number = strtod(number_lexeme, &endptr);

    if (number_lexeme == endptr) {
        throw_error(
                ERROR,
                "Error in casting string number to double in 'get_number'!");
    }

    return number;
}

int is_valid_triangle(point** points)
{
    double a
            = sqrt(pow((*points)[1].x - (*points)[0].x, 2)
                   + pow((*points)[1].y - (*points)[0].y, 2));

    double b
            = sqrt(pow((*points)[2].x - (*points)[1].x, 2)
                   + pow((*points)[2].y - (*points)[1].y, 2));

    double c
            = sqrt(pow((*points)[3].x - (*points)[2].x, 2)
                   + pow((*points)[3].y - (*points)[2].y, 2));

    if (a + b > c && b + c > a && c + a > b) {
        return 1;
    } else {
        return 0;
    }
}
