#define PI 3.1415
#define MIN(X, Y) (((Y) < (X)) ? (Y) : (X))
#define MAX(X, Y) (((Y) > (X)) ? (Y) : (X))
#include "bmp.h"
#include "image_server.h"
#include "vector_xy_t.h"
#include "vector_xy_i32_t.h"
#include "graphics.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

//creating background of any color
void gx_clear(bitmap_t *bmp, color_bgr_t color)
{
  for (int i = 0; i < bmp->width * bmp->height;i++){
    bmp->data[i].b = color.b;
    bmp->data[i].g = color.g;
    bmp->data[i].r = color.r;
  }
}

//create rectangle with center at (0,0) of a given width and height
vector_xy_t *rectangle(int width, int height)
{
  vector_xy_t *rectangle = malloc(sizeof(vector_xy_t));
  double var_x = width / 2;
  double var_y = height / 2;
  initialize_vector(rectangle);
  rectangle->point[0].x = width / 2;
  rectangle->point[0].y = height / 2;
  rectangle->point[1].x = width / 2;
  rectangle->point[1].y = -height / 2;
  rectangle->point[2].x = -width / 2;
  rectangle->point[2].y = -height / 2;
  rectangle->point[3].x = -width / 2;
  rectangle->point[3].y = height / 2;
  rectangle->size = 4;
  return rectangle;
}

//create triangle with given width centered at (0,0)
vector_xy_t *triangle(int width)
{
  vector_xy_t *triangle = malloc(sizeof(vector_xy_t));
  initialize_vector(triangle);
  triangle->point[0].x = 8 * width / 9;
  triangle->point[0].y = 0;
  triangle->point[1].x = - 4 * width / 9;
  triangle->point[1].y = width / 2;
  triangle->point[2].x = - 4 * width / 9;
  triangle->point[2].y = - width / 2;
  triangle->size = 3;
  return triangle;
}

//translate the vector of points by given x and y values
void translate_vector(vector_xy_t *polygon_points, double x, double y)
{
  for (int i = 0; i < polygon_points->size; i++){
    polygon_points->point[i].x += x;
    polygon_points->point[i].y += y;
  }
}

//rotate the vector of points by given angle theta
//anticlockwise rotation considered positive
void rotate_vector(vector_xy_t *polygon_points, double theta)
{
  double cosine = cos(PI * (-theta) / 180);
  double sine = sin(PI * (-theta) / 180);
  for (int i = 0; i < polygon_points->size; i++){
    double x = polygon_points->point[i].x;
    double y = polygon_points->point[i].y;
    double x_rotation = x * cosine - y * sine;
    double y_rotation = x * sine + y * cosine;
    polygon_points->point[i].x = x_rotation;
    polygon_points->point[i].y = y_rotation;
  }
}

//rasterizes line
vector_xy_i32_t *gx_rasterize_line(line_i32_t line)
{
  vector_xy_i32_t *v = malloc(sizeof(vector_xy_i32_t));
  initialize_vector_i32(v);
  int dx = abs(line.point[1].x - line.point[0].x);
  int sx = line.point[0].x < line.point[1].x ? 1 : -1;
  int dy = -abs(line.point[1].y - line.point[0].y);
  int sy = line.point[0].y < line.point[1].y ? 1 : -1;
  int err = dx + dy;  /* error value e_xy */
  while (1){
    if(v->size == v->capacity){
      v->capacity = 2 * v->capacity;
      v->point = realloc(v->point, v->capacity * sizeof(point_i32_t));
    }
    if(v->size == 0){
      v->point[0].x = line.point[0].x;
      v->point[0].y = line.point[0].y;
    }
    else{
      if (line.point[0].x == line.point[1].x && line.point[0].y == line.point[1].y){
        break;
      }
      int e2 = 2 * err;
      if (e2 >= dy){
        err += dy;
        line.point[0].x += sx;
        v->point[v->size].x = line.point[0].x;
      }
      else{
        v->point[v->size].x = line.point[0].x;
      }
      if (e2 <= dx){
        err += dx;
        line.point[0].y += sy;
        v->point[v->size].y = line.point[0].y;
      }
      else{
        v->point[v->size].y = line.point[0].y;
      }
    }
    v->size++;
  }
  return v;
}

//color the pixels of rasterized line
void gx_draw_line(bitmap_t *bmp, vector_xy_i32_t *v, color_bgr_t color)
{
  for(int i = 0; i < v->size - 1;i++){
      bmp->data[(v->point[i].y) * bmp->width + v->point[i].x] = color;
  }
}
//round off, rasterize and then color the pixels of rectangle
vector_xy_i32_t *gx_draw_polygon_outline(bitmap_t *bmp,vector_xy_t *v, color_bgr_t color)
{
  polygon_i32_t *polygon = malloc(sizeof(polygon_t));
  polygon->n_points = v->size;
  polygon->line = calloc(polygon->n_points,sizeof(line_i32_t));
  double epsilon = 1e-6;
  double min_x = v->point[0].x;
  double min_y = v->point[0].y;
  //find minimum values of x and y
  for(int i = 0; i < v->size; i++){
      if(v->point[i].x < min_x){
        min_x = v->point[i].x;
      }
      if(v->point[i].y < min_y){
        min_y = v->point[i].y;
      }
  }
  //rounding and storing in data type polygon_t
  for(int i = 0; i < polygon->n_points; i++){
    int k =  i;
    for (int j = 0; j < 2; j++){
      if (v->point[i].x == min_x){
        polygon->line[i].point[j].x = (int32_t)ceil(v->point[k].x);
      }
      else{
        polygon->line[i].point[j].x = (int32_t)floor(v->point[k].x - epsilon);
      }
      if (v->point[i].y == min_y){
        polygon->line[i].point[j].y = (int32_t)ceil(v->point[k].y);
      }
      else{
        polygon->line[i].point[j].y = (int32_t)floor(v->point[k].y - epsilon);
      }
      k++;
      if (k == polygon->n_points){
        k = 0;
      }
    }
  }
  //rasterize
  vector_xy_i32_t *polygon_outline = calloc(polygon->n_points, sizeof(vector_xy_i32_t));
  for(int i = 0; i < polygon->n_points; i++){
    polygon_outline[i] = *gx_rasterize_line(polygon->line[i]);
    //check if co ordinates are negative; if yes then set to zero
    for (int j = 0; j < polygon_outline[i].size; j++){
      if(polygon_outline[i].point[j].x < 0){
        polygon_outline[i].point[j].x = 0;
      }
      if(polygon_outline[i].point[j].y < 0){
        polygon_outline[i].point[j].y = 0;
      }
    }
    gx_draw_line(bmp, &polygon_outline[i], color);
  }
  return polygon_outline;
}

//fill the entire polygon with colours
void gx_fill_polygon(bitmap_t *bmp, vector_xy_t *v, vector_xy_i32_t *polygon_outline, color_bgr_t color)
{
  //set buffers for start and end to -1
  int x0[bmp->height];
  int x1[bmp->height];
  for (int i = 0; i < bmp->height; i++){
    x0[i] = -1;
    x1[i] = -1;
  }
  //iterate through each point of polygon_outline and find minimum and maximum value of x for each y
  for (int i = 0; i < v->size; i++){
    for (int j = 0; j < polygon_outline[i].size; j++){
      if (x0[polygon_outline[i].point[j].y] == -1){
        x0[polygon_outline[i].point[j].y] = polygon_outline[i].point[j].x;
        x1[polygon_outline[i].point[j].y] = polygon_outline[i].point[j].x;
      }
      else{
        x0[polygon_outline[i].point[j].y] = MIN(x0[polygon_outline[i].point[j].y], polygon_outline[i].point[j].x);
        x1[polygon_outline[i].point[j].y] = MAX(x1[polygon_outline[i].point[j].y], polygon_outline[i].point[j].x);
      }
    }
  }
  //fill colour
  int width = 0;
  for (int y = 0; y < bmp->height; y++){
    for (int i = 0; i < bmp->width; i++){
      if (x0[y] != -1){
        if (i > x0[y] && i < x1[y]){
          bmp->data[i + width].b = color.b;
          bmp->data[i + width].g = color.g;
          bmp->data[i + width].r = color.r;
        }
      }
    }
    width += bmp->width;
  }
}
