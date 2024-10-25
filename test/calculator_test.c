#include <ctest.h>
#include <libgeometry/calculator.h>

CTEST(calculator_suite, get_distance_between_points)
{
    point point_one = {10, 0};
    point point_two = {32, 0};

    double result = get_distance_between_points(point_one, point_two);

    double expected = 22;
    ASSERT_EQUAL(expected, result);
}

CTEST(calculator_suite, get_area)
{
    token tok = {"circle", IDENTIFIER};
    circle circ = {10, 10, 2};
    shape shape = {tok, circ, NULL, 1};

    double result = get_area(&shape);

    double expected = 6.28;
    ASSERT_EQUAL(expected, result);
}
