#include "bmp.h"
#include "graphics.h"
#include "image_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>

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
             vector_xy_i32_t *v = gx_rasterize_line(10, 10, 200, 200);
             //draw line with colored points
             gx_draw_line(&bmp, v, color);
             //serialize image data
            break;
    case 3 :
            ;
            //create vector of points for rectangle with a given width and height
            vector_xy_t *rectangle_3 = rectangle(4, 4);
            gx_draw_polygon(&bmp, rectangle_3, color);
            free(rectangle_3->data_x);
            free(rectangle_3->data_y);
            free(rectangle_3);
            break;
    case 4 :
            ;
            //create vector of points for rectangle with a given width and height
            vector_xy_t *rectangle_4 = rectangle(4, 4);
            translate_vector(rectangle_4, 2, 2);
            gx_draw_polygon(&bmp, rectangle_4, color);
            free(rectangle_4->data_x);
            free(rectangle_4->data_y);
            free(rectangle_4);
            break;
    case 5 :
            ;
            //create vector of points for rectangle with a given width and height
            vector_xy_t *rectangle_5 = rectangle(5, 5);
            translate_vector(rectangle_5, 2, 2);
            gx_draw_polygon(&bmp, rectangle_5, color);
            free(rectangle_5->data_x);
            free(rectangle_5->data_y);
            free(rectangle_5);
            break;
    case 6 :
            ;
            //create vector of points for rectangle with a given width and height
            vector_xy_t *rectangle_6 = rectangle(bmp.width - 40, bmp.height - 40);
            translate_vector(rectangle_6, bmp.width / 2, bmp.height / 2);
            gx_draw_polygon(&bmp, rectangle_6, color);
            free(rectangle_6->data_x);
            free(rectangle_6->data_y);
            free(rectangle_6);
            break;
    case 7 :
            ;
            //create vector of points for rectangle with a given width and height
            vector_xy_t *rectangle_7 = rectangle(bmp.width - 40, bmp.height - 40);
            translate_vector(rectangle_7, bmp.width / 2, bmp.height / 2);
            gx_draw_polygon(&bmp, rectangle_7, color);
            gx_fill_polygon(&bmp, color);
            free(rectangle_7->data_x);
            free(rectangle_7->data_y);
            free(rectangle_7);
            break;
    case 8 :
            ;
            vector_xy_t *triangle_8 = triangle( 21 );
            translate_vector(triangle_8, 400, 400);
            gx_draw_polygon(&bmp, triangle_8, color);
            gx_fill_polygon(&bmp, color);
            free(triangle_8->data_x);
            free(triangle_8->data_y);
            free(triangle_8);
            break;
    case 9 :
            ;
            vector_xy_t *triangle_data = triangle( 21 );
            //clockwise positive; theta in degrees
            rotate_vector(triangle_data, -30);
            translate_vector(triangle_data, 400, 400);
            gx_draw_polygon(&bmp, triangle_data, color);
            gx_fill_polygon(&bmp, color);
            free(triangle_data->data_x);
            free(triangle_data->data_y);
            free(triangle_data);
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
