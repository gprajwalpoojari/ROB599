#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

//defining a point
typedef struct point_i32{
  int32_t x;
  int32_t y;
}point_i32_t;

//defining a line
typedef struct line_i32{
  point_i32_t point[2];
}line_i32_t;

//defining a polygon
typedef struct polygon_i32{
  int n_points;
  line_i32_t *line;
}polygon_i32_t;

typedef struct vector_xy_i32{
  size_t size;
  size_t capacity;
  point_i32_t *point;
}vector_xy_i32_t;

void initialize_vector_i32(vector_xy_i32_t *v);

void free_vector_i32(vector_xy_i32_t *v);
