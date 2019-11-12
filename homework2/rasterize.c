#include <stdio.h>
#include "bmp.h"
#include "vector_xy_i32_t.h"
#include "image_server.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


void gx_clear(bitmap_t *bmp, color_bgr_t color){
  for (int i = 0; i < bmp->width * bmp->height;i++){
    bmp->data[i].b = color.b;
    bmp->data[i].g = color.g;
    bmp->data[i].r = color.r;
  }
}

vector_xy_i32_t *gx_rasterize_line(int x0,int y0, int x1, int y1, vector_xy_i32_t *v){
  int dx = abs(x1 - x0);
  int sx = x0 < x1 ? 1 : -1;
  int dy = -abs(y1-y0);
  int sy = y0 < y1 ? 1 : -1;
  int err = dx + dy;  /* error value e_xy */
  while (1){
    if(v->size_x == v->capacity || v->size_y == v->capacity){
      v->capacity = 2 * v->capacity;
      v->data_x = realloc(v->data_x, v->capacity * sizeof(int32_t));
      v->data_y = realloc(v->data_y, v->capacity * sizeof(int32_t));
    }
    if(v->size_x == 0 && v->size_y == 0){
      v->data_x[0] = x0;
      v->data_y[0] = y0;
      v->size_x++;
      v->size_y++;
    }
    if (x0 == x1 && y0 == y1){
      break;
    }
    int e2 = 2 * err;
    if (e2 >= dy){
      err += dy;
      x0 += sx;
      v->data_x[v->size_x] = x0;
      v->size_x++;
    }
    else{
      v->data_x[v->size_x] = x0;
      v->size_x++;
    }
    if (e2 <= dx){
      err += dx;
      y0 += sy;
      v->data_y[v->size_y] = y0;
      v->size_y++;
    }
    else{
      v->data_y[v->size_y] = y0;
      v->size_y++;
    }
  }
  return v;
}

void gx_draw_line(bitmap_t *bmp, vector_xy_i32_t *points, color_bgr_t color){
  for(int i = 0; i < points->size_x - 1;i++){
      bmp->data[(points->data_y[i]) * bmp->width + points->data_x[i]] = color;
  }
}

int main(int argc, char **argv) {
  vector_xy_i32_t *v = create_vector();
  //background color data
  color_bgr_t background = {0};
  //line color data
  color_bgr_t color;
  color.b = atoi(argv[1]);
  color.g = atoi(argv[2]);
  color.r = atoi(argv[3]);
  //image data
  bitmap_t bmp = { 0 }; // initialize to zeros
  bmp.width = 640;
  bmp.height = 480;
  bmp.data = calloc(bmp.width * bmp.height, sizeof(color_bgr_t));
  //color the background black
  gx_clear(&bmp, background);
  //create line vector
  vector_xy_i32_t *vector = gx_rasterize_line(10, 10, 200, 200, v);
  //draw line with colored points
  printf("%ld",vector->size_x);
  printf("%ld",vector->size_y);
  gx_draw_line(&bmp, vector, color);
  //serialize image data
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

  free(bmp.data);
  free(serialized_bmp);
  free(vector);
  free(v->data_x);
  free(v->data_y);
  free(v);

  return 0;
}
