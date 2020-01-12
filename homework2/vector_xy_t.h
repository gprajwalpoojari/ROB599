#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

//defining a point
typedef struct point{
  double x;
  double y;
}point_t;

//defining a line
typedef struct line{
  point_t point[2];
}line_t;

//defining a polygon
typedef struct polygon{
  int n_points;
  line_t *line;
}polygon_t;

typedef struct vector_xy{
  size_t size;
  size_t capacity;
  point_t *point;
}vector_xy_t;

void initialize_vector(vector_xy_t *v);

void free_vector(vector_xy_t *v);
