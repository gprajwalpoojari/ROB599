#include "vector_xy_i32_t.h"
#include <stdlib.h>
#include <stdio.h>

vector_xy_i32_t *create_vector(void){
  vector_xy_i32_t *v = malloc(sizeof(vector_xy_i32_t));
  v->size_x = 0;
  v->size_y = 0;
  v->capacity = 32;
  v->data_x = calloc(v->capacity, sizeof(int32_t));
  v->data_y = calloc(v->capacity, sizeof(int32_t));
  return v;
  free(v->data_x);
  free(v->data_y);
  free(v);
}
