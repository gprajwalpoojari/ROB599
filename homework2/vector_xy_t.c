#include "vector_xy_t.h"
#include <stdlib.h>
#include <stdio.h>

vector_xy_t *create_vector(void){
  vector_xy_t *v = malloc(sizeof(vector_xy_t));
  v->size_x = 0;
  v->size_y = 0;
  v->capacity = 32;
  v->data_x = calloc(v->capacity, sizeof(double));
  v->data_y = calloc(v->capacity, sizeof(double));
  return v;
  free(v->data_x);
  free(v->data_y);
  free(v);
}