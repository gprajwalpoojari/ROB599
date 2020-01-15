#include "bmp.h"
#include "graphics.h"
#include "image_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <stdbool.h>

int main(int argc, char **argv) {
  if (*argv[1] < 1 && *argv[1] > 9){
    fprintf(stderr, "usage: %s <test case>", argv[0]);
    return 1;
  }
  else{
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
      line_i32_t line;
      line.point[0].x = 10;
      line.point[0].y = 10;
      line.point[1].x = 200;
      line.point[1].y = 200;
      vector_xy_i32_t v = gx_rasterize_line(line);
      //draw line with colored points
      gx_draw_line(&bmp, &v, color);
      break;
      case 3 :
      ;
      //create vector of points for rectangle with a given width and height
      vector_xy_t *rectangle_3 = rectangle(4, 4);
      vector_xy_i32_t *rectangle_3_outline = malloc(sizeof(vector_xy_i32_t));
      initialize_vector_i32(rectangle_3_outline);
      //draw polygon outline
      rectangle_3_outline = gx_draw_polygon_outline(&bmp, rectangle_3, color);
      free_vector(rectangle_3);
      free_vector_i32(rectangle_3_outline);
      break;
      case 4 :
      ;
      //create vector of points for rectangle with a given width and height
      vector_xy_t *rectangle_4 = rectangle(4, 4);
      //perform translation
      translate_vector(rectangle_4, 2, 2);
      vector_xy_i32_t *rectangle_4_outline = malloc(sizeof(vector_xy_i32_t));
      initialize_vector_i32(rectangle_4_outline);
      //draw polygon outline
      rectangle_4_outline = gx_draw_polygon_outline(&bmp, rectangle_4, color);
      free_vector(rectangle_4);
      free_vector_i32( rectangle_4_outline);
      break;
      case 5 :
      ;
      //create vector of points for rectangle with a given width and height
      vector_xy_t *rectangle_5 = rectangle(5, 5);
      //perform translation
      translate_vector(rectangle_5, 2, 2);
      vector_xy_i32_t *rectangle_5_outline = malloc(sizeof(vector_xy_i32_t));
      initialize_vector_i32(rectangle_5_outline);
      //draw polygon outline
      rectangle_5_outline = gx_draw_polygon_outline(&bmp, rectangle_5, color);
      free_vector(rectangle_5);
      free_vector_i32( rectangle_5_outline);
      break;
      case 6 :
      ;
      //create vector of points for rectangle with a given width and height
      vector_xy_t *rectangle_6 = rectangle(bmp.width - 40, bmp.height - 40);
      //perform translation
      translate_vector(rectangle_6, bmp.width / 2, bmp.height / 2);
      vector_xy_i32_t *rectangle_6_outline = malloc(sizeof(vector_xy_i32_t));
      initialize_vector_i32(rectangle_6_outline);
      //draw polygon outline
      rectangle_6_outline = gx_draw_polygon_outline(&bmp, rectangle_6, color);
      free_vector(rectangle_6);
      free_vector_i32( rectangle_6_outline);
      break;
      case 7 :
      ;
      //create vector of points for rectangle with a given width and height
      vector_xy_t *rectangle_7 = rectangle(bmp.width - 40, bmp.height - 40);
      //perform translation
      translate_vector(rectangle_7, bmp.width / 2, bmp.height / 2);
      vector_xy_i32_t *rectangle_7_outline = malloc(sizeof(vector_xy_i32_t));
      initialize_vector_i32(rectangle_7_outline);
      //draw polygon outline
      rectangle_7_outline = gx_draw_polygon_outline(&bmp, rectangle_7, color);
      //display filled polygon
      gx_fill_polygon(&bmp, rectangle_7, rectangle_7_outline, color);
      free_vector(rectangle_7);
      free_vector_i32( rectangle_7_outline);
      break;
      case 8 :
      ;
      //create a vector of points for triangle
      vector_xy_t *triangle_8 = triangle( 21 );
      //perform translation
      translate_vector(triangle_8, 400, 400);
      vector_xy_i32_t *triangle_8_outline = malloc(sizeof(vector_xy_i32_t));
      initialize_vector_i32(triangle_8_outline);
      //draw polygon outline
      triangle_8_outline = gx_draw_polygon_outline(&bmp, triangle_8, color);
      //display filled polygon
      gx_fill_polygon(&bmp,triangle_8, triangle_8_outline, color);
      free_vector(triangle_8);
      free_vector_i32( triangle_8_outline);
      break;
      case 9 :
      ;
      //create a vector of points for triangle
      vector_xy_t *triangle_9 = triangle( 21 );
      //perform rotation
      //anti - clockwise considered positive
      rotate_vector(triangle_9, 30);
      //perform translation
      translate_vector(triangle_9, 400, 400);
      vector_xy_i32_t *triangle_9_outline = malloc(sizeof(vector_xy_i32_t));
      initialize_vector_i32(triangle_9_outline);
      //draw polygon outline
      triangle_9_outline = gx_draw_polygon_outline(&bmp, triangle_9, color);
      gx_fill_polygon(&bmp,triangle_9, triangle_9_outline, color);
      free_vector(triangle_9);
      free_vector_i32( triangle_9_outline);
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
}
