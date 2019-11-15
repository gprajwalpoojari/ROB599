#include "vector_xy_t.h"
#include <stdlib.h>
#include <stdio.h>

vector_xy_t *create_vector(void){
  vector_xy_t *v = malloc(sizeof(vector_xy_t));
  v->size = 0;
  //v->size_y = 0;
  v->capacity = 16;
  v->data_x = calloc(v->capacity, sizeof(double));
  v->data_y = calloc(v->capacity, sizeof(double));
  return v;
}
