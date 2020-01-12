#include "vector_xy_t.h"
#include <stdlib.h>
#include <stdio.h>

void initialize_vector(vector_xy_t *v)
{
  v->size = 0;
  v->capacity = 32;
  v->point = calloc(v->capacity, sizeof(point_t));
}


void free_vector(vector_xy_t *v)
{
  free(v->point);
  free(v);
}
