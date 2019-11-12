#pragma once

#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

typedef struct vector_xy_i32{
  size_t size_x;
  size_t size_y;
  size_t capacity;
  int32_t *data_x;
  int32_t *data_y;
}vector_xy_i32_t;

vector_xy_i32_t *create_vector(void);
