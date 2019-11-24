#pragma once

#include <stddef.h>
#include <inttypes.h>
#include <stdbool.h>
#include "vector_xy_t.h"
#include "braitenberg.h"

/*typedef struct polygon{
  float *points[3];
  float lines[3][16];
}polygon_t;*/

typedef struct polygon{
  vector_xy_t **line;
  int n_line;
}polygon_t;

polygon_t *create_polygon(vector_xy_t *data);

double cross_product(vector_xy_t *line_1, vector_xy_t *line_2, int point_number);

bool check_collision(polygon_t *polygon_a, polygon_t *polygon_b);//, vector_xy_t *data_1, vector_xy_t *data_2);

bool overlap(polygon_t *polygon_a, polygon_t *polygon_b, bool collision);

bool resolve_collision(vector_xy_t *lamp_centroid, vector_xy_t *polygon_centroid, polygon_t *lamp_vector, polygon_t *robot_polygon, vector_xy_t *robot_vector);
