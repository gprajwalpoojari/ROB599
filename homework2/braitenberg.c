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
#include "collision.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>


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


int main(int argc, char **argv){
  if (argc != 3){
    fprintf(stderr,"usage: %s <time steps> <fast=0|1>", argv[0]);
  }
  else if (argc == 3){
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
    //double pointer for storing all lamp data at a single place
    vector_xy_t **lamp = malloc(sizeof(vector_xy_t));
    //initialize world and display
    create_border(&bmp, border);
    lamp[0] = initialize_lamp(&bmp, 124.1, 224.4, color_lamp);
    lamp[1] = initialize_lamp(&bmp, 349.1, 99.1, color_lamp);
    lamp[2] = initialize_lamp(&bmp, 449.1, 349.1, color_lamp);
    vector_xy_t *robot = triangle(21);
    translate_vector(robot, c_r->data_x[0], c_r->data_y[0]);
    gx_draw_polygon(&bmp, robot, color_robot);
    gx_fill_polygon(&bmp, color_robot);
    //serialize image and display
    size_t bmp_size = bmp_calculate_size(&bmp);
    uint8_t *serialized_bmp = malloc(bmp_size);
    bmp_size = bmp_calculate_size(&bmp);
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

    //vector of lamp direction from robot
    vector_xy_t *dir = create_vector();
    //vectors for left and light sensors
    vector_xy_t *eye_l = initialize_sensor(-60);
    vector_xy_t *eye_r = initialize_sensor(60);
    double theta = 0;

    for (int time_step = 0; time_step < atoi(argv[1]); time_step++){
      //track left and right wheel movement for each frame
      double move_l = 0;
      double move_r = 0;
      double forward_dist = 0;
      bitmap_t temp_bmp = { 0 }; // initialize to zeros
      temp_bmp.width = 640;
      temp_bmp.height = 480;
      temp_bmp.data = calloc(temp_bmp.width * temp_bmp.height , sizeof(color_bgr_t));
      //create robot at (0,0)
      robot = triangle(21);
      //create border
      create_border(&temp_bmp, border);
      //initialize lamps
      lamp[0] = initialize_lamp(&temp_bmp, 124.1, 224.4, color_lamp);
      lamp[1] = initialize_lamp(&temp_bmp, 349.1, 99.1, color_lamp);
      lamp[2] = initialize_lamp(&temp_bmp, 449.1, 349.1, color_lamp);
      //calculate net left and right wheel movement
      for (int i = 0; i < 3; i++){
        dir->data_x[1] = c_l[i]->data_x[0];
        dir->data_y[1] = c_l[i]->data_y[0];
        dir->data_x[0] = c_r->data_x[0];
        dir->data_y[0] = c_r->data_y[0];
        dir->size = 2;
        double dist_sq = pow((dir->data_x[1] - dir->data_x[0]), 2) + pow((dir->data_y[1] - dir->data_y[0]), 2);
        move_l += MAX(0.0, dot(*dir, *eye_r)) * POWER / dist_sq;
        move_r += MAX(0.0, dot(*dir, *eye_l)) * POWER / dist_sq;
      }
      //calculate actual movement
      move_l = MIN(MAXIMUM, move_l);
      move_r = MIN(MAXIMUM, move_r);
      //calculate angle of rotation from theta = 0
      theta += (move_r - move_l) / BASE;
      forward_dist = (move_r + move_l) / 2;
      //update centroid
      c_r->data_x[0] += forward_dist * cos(-theta);
      c_r->data_y[0] += forward_dist * sin(-theta);
      //update robot
      rotate_vector(robot, - theta * 180 / PI);
      translate_vector(robot, c_r->data_x[0], c_r->data_y[0]);
      //create polygons
      polygon_t *p_triangle = create_polygon(robot);
      polygon_t *p_lamp0 = create_polygon(lamp[0]);
      polygon_t *p_lamp1 = create_polygon(lamp[1]);
      polygon_t *p_lamp2 = create_polygon(lamp[2]);
      //check collision for each polygon
      bool c1 = check_collision(p_triangle, p_lamp0);
      overlap(p_triangle, p_lamp0);
      bool c2 = check_collision(p_triangle, p_lamp1);
      overlap(p_triangle, p_lamp1);
      bool c3 = check_collision(p_triangle, p_lamp2);
      overlap(p_triangle, p_lamp2);
      //update sensor vectors
      rotate_vector(eye_l, -180 * ((move_r - move_l) / BASE) / PI);
      rotate_vector(eye_r, -180 * ((move_r - move_l) / BASE) / PI);
      //draw and fill updated polygon
      gx_draw_polygon(&temp_bmp, robot, color_robot);
      gx_fill_polygon(&temp_bmp, color_robot);
      //display image
      bmp_size = bmp_calculate_size(&temp_bmp);
      bmp_serialize(&temp_bmp, serialized_bmp);
      //write data to file
      if (atoi(argv[2]) == 0){
        FILE *fp = fopen("my_image.bmp", "wb");
        fwrite(serialized_bmp, bmp_size, 1, fp);
        fclose(fp);
        //get data on server
        image_server_set_data(bmp_size, serialized_bmp);
        image_server_start("8000"); // you could change the port number, but animation.html wants 8000
        int seconds = 0;
        long nanoseconds = 40 * 1000 * 1000;
        struct timespec interval = { seconds, nanoseconds };
        nanosleep(&interval, NULL);
      }
      if (atoi(argv[2]) == 1){
        if (time_step == atoi(argv[1]) - 1){
          FILE *fp = fopen("my_image.bmp", "wb");
          fwrite(serialized_bmp, bmp_size, 1, fp);
          fclose(fp);
          //get data on server
          image_server_set_data(bmp_size, serialized_bmp);
          image_server_start("8000"); // you could change the port number, but animation.html wants 8000
          sleep(1);
        }
      }

      free_data(robot);
      free(temp_bmp.data);
      for (int i = 0; i < 4; i++){
        free_data(p_lamp0->line[i]);
        free_data(p_lamp1->line[i]);
        free_data(p_lamp2->line[i]);
      }
      for (int i = 0; i < 3; i++){
        free_data(p_triangle->line[i]);
      }
      free(p_triangle->line);
      free(p_triangle);
      free(p_lamp0->line);
      free(p_lamp0);
      free(p_lamp1->line);
      free(p_lamp1);
      free(p_lamp2->line);
      free(p_lamp2);
    }
    //free all allocated memory
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
    return 0;
  }
}
