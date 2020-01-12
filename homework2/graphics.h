#pragma once

#define PI 3.1415
#define MIN(X, Y) (((Y) < (X)) ? (Y) : (X))
#define MAX(X, Y) (((Y) > (X)) ? (Y) : (X))
#include "bmp.h"
#include "vector_xy_i32_t.h"
#include "vector_xy_t.h"
#include "image_server.h"
#include <inttypes.h>
#include <stddef.h>

//creating background of any color
void gx_clear(bitmap_t *bmp, color_bgr_t color);

//create rectangle with center at (0,0) of a given width and height
vector_xy_t *rectangle(int width, int height);

//create triangle with given width centered at (0,0)
vector_xy_t *triangle(int width);

//translate the vector of points by given x and y values
void translate_vector(vector_xy_t *polygon_points, double x, double y);

//rotate the vector of points by given angle theta
void rotate_vector(vector_xy_t *polygon_points, double theta);

//rasterizes line
vector_xy_i32_t *gx_rasterize_line(line_i32_t line);

//color the pixels of rasterized line
void gx_draw_line(bitmap_t *bmp, vector_xy_i32_t *v, color_bgr_t color);

//round off, rasterize and color the outline pixels of polygon
vector_xy_i32_t *gx_draw_polygon_outline(bitmap_t *bmp,vector_xy_t *v, color_bgr_t color);

//fill the entire polygon with colours
void gx_fill_polygon(bitmap_t *bmp,vector_xy_t *v, vector_xy_i32_t *polygon_outline, color_bgr_t color);
