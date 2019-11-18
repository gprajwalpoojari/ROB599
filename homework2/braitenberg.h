#pragma once

#include <stddef.h>
#include <inttypes.h>
#include "graphics.h"
#include "bmp.h"

typedef struct centroid{
    float x;
    float y;
}centroid_t;


void create_border(bitmap_t *bmp, color_bgr_t color);

vector_xy_t *initialize_lamp(bitmap_t *bmp, double x, double y, color_bgr_t color);

void initialize_robot(bitmap_t *bmp, color_bgr_t color);

vector_xy_t *initialize_sensor(double theta);

double dot(vector_xy_t a, vector_xy_t b);

color_bgr_t create_color(uint8_t b, uint8_t g, uint8_t r);

vector_xy_t *create_point(double x, double y);

void free_data(vector_xy_t *vector);
