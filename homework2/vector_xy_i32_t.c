#include "vector_xy_i32_t.h"
#include <stdlib.h>
#include <stdio.h>

void initialize_vector_i32(vector_xy_i32_t *v)
{
  v->size = 0;
  v->capacity = 32;
  v->point = calloc(v->capacity, sizeof(point_i32_t));
}

void free_vector_i32(vector_xy_i32_t *v)
{
  free(v->point);
  free(v);
}
