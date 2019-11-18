#define PI 3.1415
#define MIN(X, Y) (((Y) < (X)) ? (Y) : (X))
#define MAX(X, Y) (((Y) > (X)) ? (Y) : (X))
#include "bmp.h"
#include "vector_xy_i32_t.h"
#include "image_server.h"
#include "vector_xy_t.h"
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
//rasterizes line
vector_xy_i32_t *gx_rasterize_line(int x0,int y0, int x1, int y1)
{
  vector_xy_i32_t *v = malloc(sizeof(vector_xy_i32_t));
  create_vector_i32(v);
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1-y0);
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;  /* error value e_xy */
  while (1){
    if(v->size == v->capacity){
      v->capacity = 2 * v->capacity;
      v->data_x = realloc(v->data_x, v->capacity * sizeof(int32_t));
      v->data_y = realloc(v->data_y, v->capacity * sizeof(int32_t));
    }
    if(v->size == 0){
      v->data_x[0] = x0;
      v->data_y[0] = y0;
    }
    else{
      if (x0 == x1 && y0 == y1){
        break;
      }
      int e2 = 2 * err;
      if (e2 >= dy){
        err += dy;
        x0 += sx;
        v->data_x[v->size] = x0;
      }
      else{
        v->data_x[v->size] = x0;
      }
      if (e2 <= dx){
        err += dx;
        y0 += sy;
        v->data_y[v->size] = y0;
      }
      else{
        v->data_y[v->size] = y0;
      }
    }
    v->size++;
  }
  return v;
}
//color the pixels of rasterized line
void gx_draw_line(bitmap_t *bmp, vector_xy_i32_t *points, color_bgr_t color)
{
  for(int i = 0; i < points->size - 1;i++){
      bmp->data[(points->data_y[i]) * bmp->width + points->data_x[i]] = color;
  }
}
//create rectangle with center at (0,0) of a given width and height
vector_xy_t *rectangle(int width, int height)
{
  double var_x = width / 2;
  double var_y = height / 2;
  vector_xy_t *v = create_vector();
  int size = 4;
  for (int i = 0;i < size; i++){
    int tmp_x = 1;
    int tmp_y = 1;
    if (i == 1 || i == 2){
      tmp_x = -1;
    }
    if (i == 2 || i == 3){
      tmp_y = -1;
    }
    v->data_x[i] = tmp_x * var_x;
    v->data_y[i] = tmp_y * var_y;
    v->size++;
  }
  return v;
}
//create triangle with given width centered at (0,0)
vector_xy_t *triangle(int width)
{
  vector_xy_t *v = create_vector();
  v->data_x[0] = 8 * width / 9;
  v->data_x[1] = - 4 * width / 9;
  v->data_x[2] = - 4 * width / 9;
  v->data_y[0] = 0;
  v->data_y[1] = width / 2;
  v->data_y[2] = - width / 2;
  v->size = 3;
  return v;
}
//translate the vector of points by given x and y values
void translate_vector(vector_xy_t *rectangle_data, double x, double y)
{
  for (int i = 0; i < rectangle_data->size; i++){
    rectangle_data->data_x[i] += x;
    rectangle_data->data_y[i] += y;
  }
}
//rotate the vector of points by given angle theta
void rotate_vector(vector_xy_t *triangle_data, double theta)
{
  double cosine = cos(PI * theta / 180);
  double sine = sin(PI * theta / 180);
  for (int i = 0; i < triangle_data->size; i++){
    double x = triangle_data->data_x[i];
    double y = triangle_data->data_y[i];
    triangle_data->data_x[i] = x * cosine - y * sine;
    triangle_data->data_y[i] = x * sine + y * cosine;
  }
}
//round off, rasterize and then color the pixels of rectangle
void gx_draw_polygon(bitmap_t *bmp,vector_xy_t *v, color_bgr_t color)
{
  vector_xy_i32_t *vector = malloc(sizeof(vector_xy_i32_t));
  create_vector_i32(vector);
  double epsilon = 1e-6;
  double min_x = v->data_x[0];
  double min_y = v->data_y[0];
  //find minimum values of x and y
  for(int i = 0; i < v->size; i++){
      if(v->data_x[i] < min_x){
        min_x = v->data_x[i];
      }
      if(v->data_y[i] < min_y){
        min_y = v->data_y[i];
      }
  }
  //rounding and storing in *vector
  for(int i = 0;i < v->size; i++){
    if (v->data_x[i] == min_x){
      vector->data_x[i] = (int32_t)ceil(v->data_x[i]);
    }
    else{
      vector->data_x[i] = (int32_t)floor(v->data_x[i] - epsilon);
    }
    if (v->data_y[i] == min_y){
      vector->data_y[i] = (int32_t)ceil(v->data_y[i]);
    }
    else{
      vector->data_y[i] = (int32_t)floor(v->data_y[i] - epsilon);
    }
    vector->size++;
  }
  //rasterize and draw lines
  vector_xy_i32_t *vector_rasterize[vector->size] ;
  for(int i = 0; i < vector->size; i++){
   int tmp = i;
    if (i == vector->size - 1){
      tmp = 0;
    }
    else{
      tmp = i + 1;
    }
    vector_rasterize[i] = gx_rasterize_line(vector->data_x[i], vector->data_y[i], vector->data_x[tmp], vector->data_y[tmp]);
    //check if co ordinates are negative; if yes then set to zero
    for (int j = 0; j < vector_rasterize[i]->size; j++){
      if(vector_rasterize[i]->data_x[j] < 0){
        vector_rasterize[i]->data_x[j] = 0;
      }
      if(vector_rasterize[i]->data_y[j] < 0){
        vector_rasterize[i]->data_y[j] = 0;
      }
    }
    gx_draw_line(bmp, vector_rasterize[i], color);
    free(vector_rasterize[i]->data_x);
    free(vector_rasterize[i]->data_y);
    free(vector_rasterize[i]);
  }
  free(vector->data_x);
  free(vector->data_y);
  free(vector);
}
//fill the entire polygon with colours
void gx_fill_polygon(bitmap_t *bmp, color_bgr_t color)
{
  int x0[bmp->height];
  int x1[bmp->height];
  for (int i = 0; i < bmp->height; i++){
    x0[i] = -1;
    x1[i] = -1;
  }
  //identify minimum and maximum values for filling colors
  int width = 0;
  bool val[bmp->height];
  for (int i = 0; i < bmp->height; i++){
    val[i] = 0;
  }
  //check if the number of points in a horizontal line is greater than 1
  for (int y = 0; y < bmp->height; y++){
    int count = 0;
    for (int i = 0; i < bmp->width; i++){
      if (bmp->data[i + width].b == color.b && bmp->data[i + width].g == color.g && bmp->data[i + width].r == color.r){
        count++;
      }
      if (count > 1){
        val[y] = 1;
      }
      else{
        val[y] = 0;
      }
    }
    width += bmp->width;
  }
  width = 0;
  for (int y = 0; y < bmp->height; y++){
    for (int i = 0; i < bmp->width; i++){
      if (val[y] == 1){
        int count_val = 0;
        if (bmp->data[i + width].b == color.b && bmp->data[i + width].g == color.g && bmp->data[i + width].r == color.r){
          count_val++;
        }
        if (count_val == 1){
          if (x0[y] == -1){
            x0[y] = i;
            x1[y] = i;
          }
          else{
            x0[y] = MIN(x0[y], i);
            x1[y] = MAX(x1[y], i);
          }
        }
        if (count_val == 2){
          break;
        }
      }
    }
    width += bmp->width;
  }
  //fill color between min and max values
  width = 0;
  for (int y = 0; y < bmp->height; y++){
    for (int i = 0; i < bmp->width; i++){
      if (x0[y] != -1){
        if (i > x0[y] && i <= x1[y]){
          bmp->data[i + width].b = color.b;
          bmp->data[i + width].g = color.g;
          bmp->data[i + width].r = color.r;
        }
      }

    }
    width += bmp->width;
  }
}
