#include <libgeometry/calculator.h>

void calculate(shape** shapes, size_t* size)
{
    for (size_t i = 0; i < *size; i++) {
        shape shape = (*shapes)[i];
        double perimeter = get_perimeter(&shape);
        double area = get_area(&shape);

        if (is_circle(shape.identifier)) {
            printf("%zu. %s(%g %g, %g)\n",
                   i + 1,
                   shape.identifier.lexeme,
                   shape.circle.x,
                   shape.circle.y,
                   shape.circle.r);
        } else {
            printf("%zu. %s((", i + 1, shape.identifier.lexeme);
            for (size_t i = 0; i < POINTS_IN_TRIANGLE; i++) {
                if (i == 0) {
                    printf("%g, %g", shape.points[i].x, shape.points[i].y);
                } else {
                    printf(" %g, %g", shape.points[i].x, shape.points[i].y);
                }
            }

            printf("))\n");
        }

        printf("\tperimeter = %g\n", perimeter);
        printf("\tarea = %g\n", area);
        printf("\tintersects:\n");

        for (size_t j = 0; j < *size; j++) {
            if (j != i) {
                if (is_circle(shape.identifier)) {
                    if (is_circle((*shapes)[j].identifier) == 0) {
                        if (check_intersection_circle_triangle(
                                    shape.circle, &((*shapes)[j].points))) {
                            printf("\t  %zu. %s\n",
                                   j + 1,
                                   (*shapes)[j].identifier.lexeme);
                        }
                    } else {
                        if (check_circle_intersection(
                                    shape.circle, (*shapes)[j].circle)) {
                            printf("\t  %zu. %s\n",
                                   j + 1,
                                   (*shapes)[j].identifier.lexeme);
                        }
                    }
                } else {
                    if (is_circle((*shapes)[j].identifier)) {
                        if (check_intersection_circle_triangle(
                                    (*shapes)[j].circle, &(shape.points))) {
                            printf("\t  %zu. %s\n",
                                   j + 1,
                                   (*shapes)[j].identifier.lexeme);
                        }
                    } else {
                        if (check_triangle_intersection(
                                    &((*shapes)[j].points), &(shape.points))) {
                            printf("\t  %zu. %s\n",
                                   j + 1,
                                   (*shapes)[j].identifier.lexeme);
                        }
                    }
                }
            }
        }

        printf("\n");
    }
}

double get_area(shape* shape)
{
    double area = 0;

    if (is_circle((*shape).identifier)) {
        area = 3.14 * (*shape).circle.r;
    } else {
        double perimeter = get_perimeter(shape);
        double* sides = get_sides((*shape).points);
        double p = perimeter / 2;
        area = sqrt(p * (p - sides[0]) * (p - sides[1]) * (p - sides[2]));
    }

    return area;
}

double get_perimeter(shape* shape)
{
    double perimeter = 0;

    if (is_circle((*shape).identifier)) {
        perimeter = 2 * 3.14 * (*shape).circle.r;
    } else {
        double* sides = get_sides((*shape).points);

        for (size_t i = 0; i < SIDES_IN_TRIANGLE; i++) {
            perimeter += sides[i];
        }
    }

    return perimeter;
}

double* get_sides(point* points)
{
    double* sides = (double*)malloc(SIDES_IN_TRIANGLE * sizeof(double));

    for (size_t i = 0; i < POINTS_IN_TRIANGLE - 1; i++) {
        sides[i] = get_distance_between_points(points[i], points[i + 1]);
    }

    return sides;
}

double get_distance_between_points(point p1, point p2)
{
    double distance = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));

    return distance;
}

int check_intersection_circle_triangle(circle circle, point** points)
{
    point triangle_a = (*points)[0];
    point triangle_b = (*points)[1];
    point triangle_c = (*points)[2];

    point circle_point = {circle.x, circle.y};

    double dist_a = get_distance_between_points(circle_point, triangle_a);
    double dist_b = get_distance_between_points(circle_point, triangle_b);
    double dist_c = get_distance_between_points(circle_point, triangle_c);

    if (dist_a <= circle.r || dist_b <= circle.r || dist_c <= circle.r) {
        return 1;
    }

    double s
            = fabs((triangle_a.x - triangle_c.x) * (triangle_b.y - triangle_c.y)
                   - (triangle_b.x - triangle_c.x)
                           * (triangle_a.y - triangle_c.y))
            / 2.0;

    double h_a = 2 * s / get_distance_between_points(triangle_b, triangle_c);
    double h_b = 2 * s / get_distance_between_points(triangle_a, triangle_c);
    double h_c = 2 * s / get_distance_between_points(triangle_a, triangle_b);

    if (h_a <= circle.r || h_b <= circle.r || h_c <= circle.r) {
        return 1;
    }

    return 0;
}

int orientation(point p, point q, point r)
{
    float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    if (val == 0)
        return 0;
    return (val > 0) ? 1 : 2;
}

int do_intersect(point p1, point q1, point p2, point q2)
{
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    if (o1 != o2 && o3 != o4)
        return 1;

    return 0;
}

int check_triangle_intersection(point** points_one, point** points_two)
{
    point A1 = (*points_one)[0];
    point B1 = (*points_one)[1];
    point C1 = (*points_one)[2];
    point A2 = (*points_two)[0];
    point B2 = (*points_two)[0];
    point C2 = (*points_two)[0];

    if (do_intersect(A1, B1, A2, B2) || do_intersect(A1, B1, B2, C2)
        || do_intersect(A1, B1, C2, A2) || do_intersect(B1, C1, A2, B2)
        || do_intersect(B1, C1, B2, C2) || do_intersect(B1, C1, C2, A2)
        || do_intersect(C1, A1, A2, B2) || do_intersect(C1, A1, B2, C2)
        || do_intersect(C1, A1, C2, A2)) {
        return 1;
    }

    if (orientation(A1, B1, A2) == 0 || orientation(A1, B1, B2) == 0
        || orientation(A1, B1, C2) == 0 || orientation(B1, C1, A2) == 0
        || orientation(B1, C1, B2) == 0 || orientation(B1, C1, C2) == 0
        || orientation(C1, A1, A2) == 0 || orientation(C1, A1, B2) == 0
        || orientation(C1, A1, C2) == 0) {
        return 1;
    }

    return 0;
}

int check_circle_intersection(circle c1, circle c2)
{
    float distance = sqrt(pow(c2.x - c1.x, 2) + pow(c2.y - c1.y, 2));

    if (distance <= c1.r + c2.r) {
        return 1;
    } else {
        return 0;
    }
}
