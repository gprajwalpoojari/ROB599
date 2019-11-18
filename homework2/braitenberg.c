#define _GNU_SOURCE // this must come before all the #include's in order to work
#include <time.h>
#define BASE 80
#define MAXIMUM 12
#define POWER 100000
#define PI 3.1415
#define MIN(X, Y) (((Y) < (X)) ? (Y) : (X))
#define MAX(X, Y) (((Y) > (X)) ? (Y) : (X))
#include "braitenberg.h"
#include "bmp.h"
#include "graphics.h"
#include "image_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>


vector_xy_t *initialize_lamp(bitmap_t *bmp, double x, double y, color_bgr_t color)
{
  vector_xy_t *lamp = rectangle(12, 12);
  rotate_vector(lamp, -45);
  translate_vector(lamp, x, y);
  gx_draw_polygon(bmp, lamp, color);
  gx_fill_polygon(bmp, color);
  return lamp;
}

void initialize_robot(bitmap_t *bmp, color_bgr_t color)
{
  vector_xy_t *robot = triangle(21);
  translate_vector(robot, bmp->width / 2, bmp->height / 2);
  gx_draw_polygon(bmp, robot, color);
  gx_fill_polygon(bmp, color);

}

vector_xy_t *initialize_sensor(double theta)
{
  vector_xy_t *sensor = create_vector();
  sensor->data_x[0] = 0;
  sensor->data_y[0] = 0;
  sensor->data_x[1] = 1;
  sensor->data_y[1] = 0;
  sensor->size = 2;
  rotate_vector(sensor, theta);
  return sensor;
}

double dot(vector_xy_t a, vector_xy_t b)
{
  double temp_a = sqrt(pow(a.data_x[1] -a.data_x[0], 2) + pow(a.data_y[1] - a.data_y[0], 2));
  double temp_b = sqrt(pow(b.data_x[1] -b.data_x[0], 2) + pow(b.data_y[1] - b.data_y[0], 2));
  double dot = (((a.data_x[1] - a.data_x[0]) * (b.data_x[1] - b.data_x[0])) + ((a.data_y[1] - a.data_y[0]) * (b.data_y[1] - b.data_y[0]))) / (temp_a * temp_b);
  return dot;
}

color_bgr_t create_color(uint8_t b, uint8_t g, uint8_t r)
{
  color_bgr_t color;
  color.b = b;
  color.g = g;
  color.r = r;
  return color;
}

vector_xy_t *create_point(double x, double y)
{
  vector_xy_t *point = malloc(sizeof(vector_xy_t));
  point->data_x = malloc(sizeof(double));
  point->data_y = malloc(sizeof(double));
  point->data_x[0] = x;
  point->data_y[0] = y;
  return point;
}

void create_border(bitmap_t *bmp, color_bgr_t color)
{
  vector_xy_t *border = rectangle(bmp->width - 40, bmp->height - 40);
  translate_vector(border, bmp->width / 2, bmp->height / 2);
  gx_draw_polygon(bmp, border, color);
  free(border->data_x);
  free(border->data_y);
  free(border);
}

void free_data(vector_xy_t *vector)
{
  free(vector->data_x);
  free(vector->data_y);
  free(vector);
}
int main(void){
  //image data
  bitmap_t bmp = { 0 }; // initialize to zeros
  bmp.width = 640;
  bmp.height = 480;
  bmp.data = calloc(bmp.width * bmp.height , sizeof(color_bgr_t));
  //background color data
  color_bgr_t background = {0};
  //border color
  color_bgr_t border = create_color(255, 255, 255);
  //lamp color
  color_bgr_t color_lamp = create_color(0, 255, 255);
  //robot color
  color_bgr_t color_robot = create_color(0, 255, 0);
  //locate centroids of all three lamps
  vector_xy_t **c_l = malloc(sizeof(vector_xy_t));
  c_l[0] = create_point(124.1,224.4);
  c_l[1] = create_point(349.1, 99.1);
  c_l[2] = create_point(449.1, 349.1);
  //locate initial centroid of robot
  vector_xy_t *c_r = create_point(bmp.width / 2, bmp.height / 2);
  //create border for image
  create_border(&bmp, border);
  //double pointer for storing all lamp data at a single place
  vector_xy_t **lamp = malloc(sizeof(vector_xy_t));
  //store lamp data in lamp and display image
  lamp[0] = initialize_lamp(&bmp, 124.1, 224.4, color_lamp);
  lamp[1] = initialize_lamp(&bmp, 349.1, 99.1, color_lamp);
  lamp[2] = initialize_lamp(&bmp, 449.1, 349.1, color_lamp);
  //store robot dtaa in lamp and display image
  initialize_robot(&bmp, color_robot);
  //serialize image and display
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
  //movement

  double theta = 0;

  vector_xy_t *eye_l = initialize_sensor(-60);
  vector_xy_t *eye_r = initialize_sensor(60);
  vector_xy_t *dir = create_vector();


  for(int a = 0; a < 100; a++){
    double move_l = 0;
    double move_r = 0;
    double forward_dist = 0;
    bitmap_t temp_bmp = { 0 }; // initialize to zeros
    temp_bmp.width = 640;
    temp_bmp.height = 480;
    temp_bmp.data = calloc(temp_bmp.width * temp_bmp.height , sizeof(color_bgr_t));
    vector_xy_t *temp_robot = triangle(21);
    create_border(&temp_bmp, border);

    lamp[0] = initialize_lamp(&temp_bmp, 124.1, 224.4, color_lamp);
    lamp[1] = initialize_lamp(&temp_bmp, 349.1, 99.1, color_lamp);
    lamp[2] = initialize_lamp(&temp_bmp, 449.1, 349.1, color_lamp);

    for (int i = 0; i < 3; i++){
      dir->data_x[1] = c_l[i]->data_x[0];
      dir->data_y[1] = c_l[i]->data_y[0];
      dir->data_x[0] = c_r->data_x[0];
      dir->data_y[0] = c_r->data_y[0];
      dir->size = 2;
      double dist_sq = pow(c_l[i]->data_x[0] - c_r->data_x[0], 2) + pow(c_l[i]->data_y[0] - c_r->data_y[0], 2);
      move_l += MAX(0.0, dot(*dir, *eye_r)) * POWER / dist_sq;
      move_r += MAX(0.0, dot(*dir, *eye_l)) * POWER / dist_sq;

    }
    //printf("%f ",move_r);
    move_l = MIN(MAXIMUM, move_l);
    move_r = MIN(MAXIMUM, move_r);
    //printf("%f \n", move_l);
    theta += (move_r - move_l) / BASE;
    forward_dist = (move_l + move_r) / 2;
    rotate_vector(temp_robot, -180 * theta / PI);
    c_r->data_x[0] += forward_dist * cos(-theta);
    c_r->data_y[0] += forward_dist * sin(-theta);
    translate_vector(temp_robot, c_r->data_x[0], c_r->data_y[0]);
    rotate_vector(eye_l, -180 * theta / PI);
    rotate_vector(eye_r, -180 * theta / PI);
    printf("%f\n",180 * 2 *theta / PI);
    gx_draw_polygon(&temp_bmp, temp_robot, color_robot);
    gx_fill_polygon(&temp_bmp, color_robot);
    //printf("%f %f\n", c_r->data_x[0], c_r->data_y[0]);
    bmp_size = bmp_calculate_size(&temp_bmp);
    bmp_serialize(&temp_bmp, serialized_bmp);
    //write data to file
    f = fopen("my_image.bmp", "wb");
    fwrite(serialized_bmp, bmp_size, 1, f);
    fclose(f);

    //get data on server
    image_server_set_data(bmp_size, serialized_bmp);
    image_server_start("8000"); // you could change the port number, but animation.html wants 8000
    sleep(1);
    free_data(temp_robot);
  }







//  free(temp_bmp.data);
  for (int i = 0; i < 3; i++){
    free_data(lamp[i]);
    free_data(c_l[i]);
  }
  free(lamp);
  free(c_l);

  free_data(eye_l);
  free_data(eye_r);
  free_data(dir);
  free_data(c_r);
  free(serialized_bmp);
  free(bmp.data);

  //free_data(temp_robot);
  return 0;
}
