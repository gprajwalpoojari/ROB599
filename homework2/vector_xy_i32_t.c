#include "vector_xy_i32_t.h"
#include <stdlib.h>
#include <stdio.h>

void create_vector_i32(vector_xy_i32_t *v){
  v->size = 0;
  v->capacity = 32;
  v->data_x = calloc(v->capacity, sizeof(int32_t));
  v->data_y = calloc(v->capacity, sizeof(int32_t));
}
