#pragma once

#include <stddef.h>
#include <inttypes.h>
#include <stdbool.h>
#include "vector_xy_t.h"
#include "braitenberg.h"

polygon_t make_polygon(vector_xy_t robot);

double cross_product(line_t line, point_t point);

bool check_collision(polygon_t polygon_a, polygon_t polygon_b);

void print_result(bool COLLISION);

//bool resolve_collision(vector_xy_t *lamp_centroid, vector_xy_t *polygon_centroid, polygon_t *lamp_vector, polygon_t *robot_polygon, vector_xy_t *robot_vector);
