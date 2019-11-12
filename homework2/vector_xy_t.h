#pragma once
#include <inttypes.h>
#include <stddef.h>
#include <stdint.h>

typedef struct vector_xy{
  size_t size_x;
  size_t size_y;
  size_t capacity;
  double *data_x;
  double *data_y;
}vector_xy_t;
