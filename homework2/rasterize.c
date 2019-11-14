#include "bmp.h"
#include "vector_xy_i32_t.h"
#include "image_server.h"
#include "vector_xy_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <math.h>


void gx_clear(bitmap_t *bmp, color_bgr_t color){
  for (int i = 0; i < bmp->width * bmp->height;i++){
    bmp->data[i].b = color.b;
    bmp->data[i].g = color.g;
    bmp->data[i].r = color.r;
  }
}

vector_xy_i32_t *gx_rasterize_line(int x0,int y0, int x1, int y1){
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
    if(v->size == 0 /*&& v->size_y == 0*/){
      v->data_x[0] = x0;
      v->data_y[0] = y0;
      //v->size_x++;
      //v->size_y++;
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
        //v->size_x++;
      }
      else{
        v->data_x[v->size] = x0;
        //v->size_x++;
      }
      if (e2 <= dx){
        err += dx;
        y0 += sy;
        v->data_y[v->size] = y0;
        //v->size_y++;
      }
      else{
        v->data_y[v->size] = y0;
        //v->size_y++;
      }
    }
    v->size++;
  }
  return v;
}

void gx_draw_line(bitmap_t *bmp, vector_xy_i32_t *points, color_bgr_t color){
  for(int i = 0; i < points->size - 1;i++){
      bmp->data[(points->data_y[i]) * bmp->width + points->data_x[i]] = color;
  }
}

vector_xy_t *rectangle(int width, int height){
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

void gx_draw_polygon(bitmap_t *bmp,vector_xy_t *v, color_bgr_t color){
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
  }
  free(vector->data_x);
  free(vector->data_y);
}

int main(int argc, char **argv) {
  //image data
  bitmap_t bmp = { 0 }; // initialize to zeros
  bmp.width = 640;
  bmp.height = 480;
  bmp.data = calloc(bmp.width * bmp.height , sizeof(color_bgr_t));
  //background color data
  color_bgr_t background = {0};
  //line color data
  color_bgr_t color;
  color.b = 255;
  color.g = 255;
  color.r = 255;
  int n = atoi(argv[1]);
  switch (n){
    case 1 :
             ;
             //color the background black
             gx_clear(&bmp, background);
             break;
    case 2 :
             ;
             //color the background black
             gx_clear(&bmp, background);
             //create line vector
             vector_xy_i32_t *vector = gx_rasterize_line(10, 10, 200, 200);
             //draw line with colored points
             gx_draw_line(&bmp, vector, color);
             //serialize image data
            break;
    case 3 :
            ;
            //create vector of points for rectangle with a given width and height
            vector_xy_t *v = rectangle(950, 950);
            gx_draw_polygon(&bmp, v, color);
            free(v->data_x);
            free(v->data_y);
            free(v);
            break;
  }
  size_t bmp_size = bmp_calculate_size(&bmp);
  uint8_t *serialized_bmp = malloc(bmp_size);
  bmp_serialize(&bmp, serialized_bmp);
  //write data to file
  FILE *f = fopen("my_image.bmp", "wb");
  fwrite(serialized_bmp, bmp_size, 1, f);
  fclose(f);
  //get data on server
  image_server_set_data(bmp_size, serialized_bmp);
  image_server_start("8000"); // you could change the port number, but animation.html wants 8000
  sleep(1);
  free(serialized_bmp);
  free(bmp.data);
  return 0;
}
