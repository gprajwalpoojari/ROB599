#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

typedef struct vector_xy_i32{
  size_t size;
  //size_t size_y;
  size_t capacity;
  int32_t *data_x;
  int32_t *data_y;
}vector_xy_i32_t;

void create_vector_i32(vector_xy_i32_t *v);
