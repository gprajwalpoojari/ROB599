#pragma once

#include <stddef.h>
#include <inttypes.h>
#include "vector_xy_t.h"

typedef struct polygon{
  float *points[3];
  float lines[3][16];
}polygon_t;

polygon_t create_polygon(vector_xy_t *data);

int cross_product(polygon_t polygon_a, polygon_t polygon_b, int m, int n, vector_xy_t *polygon);

float overlap(polygon_t polygon_a, polygon_t polygon_b, int var);

void check_collision(polygon_t polygon_a, polygon_t polygon_b, vector_xy_t *data_1, vector_xy_t *data_2);
