#pragma once

#include <libgeometry/parser.h>
#include <libgeometry/token.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIDES_IN_TRIANGLE 3
#define POINTS_IN_TRIANGLE 4

void calculate(shape** shapes, size_t* size);

int is_circle(token identifier);

int check_intersection_circle_triangle(circle circle, point** points);

int orientation(point p, point q, point r);

int do_intersect(point p1, point q1, point p2, point q2);

int check_triangle_intersection(point** points_one, point** points_two);

int check_circle_intersection(circle c1, circle c2);

double get_area(shape* shape);

double get_perimeter(shape* shape);

double get_distance_between_points(point p1, point p2);

double* get_sides(point* points);
