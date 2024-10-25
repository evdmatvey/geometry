#include <ctest.h>
#include <libgeometry/token.h>

CTEST(token_suite, get_token_identifier)
{
    char* lexeme = "circle";

    token result = get_token(lexeme);

    token_type expected = IDENTIFIER;
    ASSERT_EQUAL(expected, result.type);
}

CTEST(token_suite, get_token_lpar)
{
    char* lexeme = "(";

    token result = get_token(lexeme);

    token_type expected = LPAR;
    ASSERT_EQUAL(expected, result.type);
}

CTEST(token_suite, get_token_number)
{
    char* lexeme = "12.4";

    token result = get_token(lexeme);

    token_type expected = NUMBER;
    ASSERT_EQUAL(expected, result.type);
}

CTEST(token_suite, get_token_comma)
{
    char* lexeme = ",";

    token result = get_token(lexeme);

    token_type expected = COMMA;
    ASSERT_EQUAL(expected, result.type);
}

CTEST(token_suite, is_float_number)
{
    const char* lexeme = "13.4";

    int result = is_float_number(lexeme);

    int expected = 1;
    ASSERT_EQUAL(expected, result);
}
