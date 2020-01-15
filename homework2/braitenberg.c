#define _GNU_SOURCE // this must come before all the #include's in order to work
#include <time.h>
#define BASE 80
#define MAXIMUM 12
#define POWER 100000
#define PI 3.1415
#define CONSTANT PI/180
#define MIN(X, Y) (((Y) < (X)) ? (Y) : (X))
#define MAX(X, Y) (((Y) > (X)) ? (Y) : (X))
#include "braitenberg.h"
#include "bmp.h"
#include "graphics.h"
#include "image_server.h"
//#include "collision.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>


vector_xy_t *initialize_lamp(bitmap_t *bmp, double x, double y, color_bgr_t color)
{
  vector_xy_t *lamp_points = rectangle(12, 12);
  rotate_vector(lamp_points, 45);
  translate_vector(lamp_points, x, y);
  vector_xy_i32_t *lamp_outline = malloc(sizeof(vector_xy_i32_t));
  initialize_vector_i32(lamp_outline);
  lamp_outline = gx_draw_polygon_outline(bmp, lamp_points, color);
  gx_fill_polygon(bmp, lamp_points, lamp_outline, color);
  return lamp_points;
}

vector_xy_t *initialize_robot(bitmap_t *bmp, color_bgr_t color)
{
  vector_xy_t *robot_points = triangle(21);
  translate_vector(robot_points, bmp->width / 2, bmp->height / 2);
  vector_xy_i32_t *robot_outline = malloc(sizeof(vector_xy_i32_t));
  initialize_vector_i32(robot_outline);
  robot_outline = gx_draw_polygon_outline(bmp, robot_points, color);
  gx_fill_polygon(bmp, robot_points, robot_outline, color);
  return robot_points;

}

line_t initialize_sensor(double theta)
{
  line_t sensor;
  sensor.point[0].x = 0;
  sensor.point[0].y = 0;
  sensor.point[1].x = 1;
  sensor.point[1].y = 0;
  double x_rotation = sensor.point[1].x * cos(CONSTANT * (-theta)) - sensor.point[1].y * sin(CONSTANT * (-theta));
  double y_rotation = sensor.point[1].x * sin(CONSTANT * (-theta)) + sensor.point[1].y * cos(CONSTANT * (-theta));
  sensor.point[1].x = x_rotation;
  sensor.point[1].y = y_rotation;
  return sensor;
}

double dot(line_t a, line_t b)
{
  double temp_a = sqrt(pow(a.point[1].x -a.point[0].x, 2) + pow(a.point[1].y - a.point[0].y, 2));
  double temp_b = sqrt(pow(b.point[1].x -b.point[0].x, 2) + pow(b.point[1].y - b.point[0].y, 2));
  double dot = (((a.point[1].x - a.point[0].x) * (b.point[1].x - b.point[0].x)) + ((a.point[1].y - a.point[0].y) * (b.point[1].y - b.point[0].y))) / (temp_a * temp_b);
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

point_t create_point(double x, double y)
{
  point_t point;
  point.x = x;
  point.y = y;
  return point;
}

void create_border(bitmap_t *bmp, color_bgr_t color)
{
  vector_xy_t *border_points = rectangle(bmp->width - 40, bmp->height - 40);
  translate_vector(border_points, bmp->width / 2, bmp->height / 2);
  vector_xy_i32_t *border_outline = malloc(sizeof(vector_xy_i32_t));
  initialize_vector_i32(border_outline);
  border_outline = gx_draw_polygon_outline(bmp, border_points, color);
  free_vector(border_points);
  free_vector_i32(border_outline);
}


int main(int argc, char **argv){
  if (argc != 3){
    fprintf(stderr,"usage: %s <time steps> <fast=0|1>", argv[0]);
    return 1;
  }
  else{


    for (int time_step = 0; time_step <= atoi(argv[1]); time_step++){
      //image data
      bitmap_t bmp = { 0 }; // initialize to zeros
      bmp.width = 640;
      bmp.height = 480;
      bmp.data = calloc(bmp.width * bmp.height , sizeof(color_bgr_t));
      //border color
      color_bgr_t border = create_color(255, 255, 255);
      //lamp color
      color_bgr_t color_lamp = create_color(0, 255, 255);
      //robot color
      color_bgr_t color_robot = create_color(0, 255, 0);
      //locate centroids of all three lamps
      point_t *centroid_lamp = calloc(3, sizeof(point_t));
      centroid_lamp[0] = create_point(124.1,224.4);
      centroid_lamp[1] = create_point(349.1, 99.1);
      centroid_lamp[2] = create_point(449.1, 349.1);
      point_t centroid_robot;
      //pointer for storing all lamp data at a single place
      vector_xy_t *lamp = calloc(3, sizeof(vector_xy_t));
      //pointer for robot points
      vector_xy_t *robot = malloc(sizeof(robot));
      //initialize world and display
      create_border(&bmp, border);
      lamp[0] = *initialize_lamp(&bmp, 124.1, 224.4, color_lamp);
      lamp[1] = *initialize_lamp(&bmp, 349.1, 99.1, color_lamp);
      lamp[2] = *initialize_lamp(&bmp, 449.1, 349.1, color_lamp);
      if (time_step == 0){
        //locate initial centroid of robot
        centroid_robot = create_point(bmp.width / 2, bmp.height / 2);
        robot = initialize_robot(&bmp, color_robot);
      }
      else{
        //vector of lamp direction from robot
        line_t *dir = calloc(3, sizeof(line_t));
        //vectors for left and light sensors
        line_t eye_l = initialize_sensor(60);
        line_t eye_r = initialize_sensor(-60);
        //vector for robot direction
        line_t robot_head = initialize_sensor(0);
        double theta = 0;
        //track left and right wheel movement for each frame
        double move_l = 0;
        double move_r = 0;
        double forward_dist = 0;
        //create robot at (0,0)
        robot = triangle(21);
        //calculate net left and right wheel movement
        for (int i = 0; i < 3; i++){
          dir[i].point[1].x = centroid_lamp[i].x;
          dir[i].point[1].y = centroid_lamp[i].y;
          dir[i].point[0].x = centroid_robot.x;
          dir[i].point[0].y = centroid_robot.y;
          double dist_sq = pow((dir[i].point[1].x - dir[i].point[0].x), 2) + pow((dir[i].point[1].y - dir[i].point[0].y), 2);
          move_l += MAX(0.0, dot(dir[i], eye_r)) * POWER / dist_sq;
          move_r += MAX(0.0, dot(dir[i], eye_l)) * POWER / dist_sq;
        }
        //calculate actual movement
        move_l = MIN(MAXIMUM, move_l);
        move_r = MIN(MAXIMUM, move_r);
        //calculate angle of rotation from theta = 0
        theta += (move_r - move_l) / BASE;
        forward_dist = (move_r + move_l) / 2;
        //update centroid
        centroid_robot.x += forward_dist * cos(-theta);
        centroid_robot.y += forward_dist * sin(-theta);
        //update robot
        rotate_vector(robot, theta * 180 / PI);
        translate_vector(robot, centroid_robot.x, centroid_robot.y);
        vector_xy_i32_t *robot_outline = malloc(sizeof(vector_xy_i32_t));
        initialize_vector_i32(robot_outline);
        robot_outline = gx_draw_polygon_outline(&bmp, robot, color_robot);
        gx_fill_polygon(&bmp, robot, robot_outline, color_robot);
      }

      /*
      //create polygon data types for integrating collision code
      polygon_t *p_triangle = create_polygon(robot);
      polygon_t *p_lamp0 = create_polygon(lamp[0]);
      polygon_t *p_lamp1 = create_polygon(lamp[1]);
      polygon_t *p_lamp2 = create_polygon(lamp[2]);
      //check collision for each polygon
      bool c1 = check_collision(p_triangle, p_lamp0);
      overlap(p_triangle, p_lamp0, c1);
      while (c1 == true){
        c1 = resolve_collision(c_l[0], c_r, p_lamp0, p_triangle, robot);
      }
      bool c2 = check_collision(p_triangle, p_lamp1);
      overlap(p_triangle, p_lamp1, c2);
      while (c2 == true){
        c2 = resolve_collision(c_l[1], c_r, p_lamp1, p_triangle, robot);
      }
      bool c3 = check_collision(p_triangle, p_lamp2);
      overlap(p_triangle, p_lamp2, c3);
      while (c3 == true){
        c3 = resolve_collision(c_l[2], c_r, p_lamp2, p_triangle, robot);

      }
      //update sensor vectors
      rotate_vector(eye_l, -180 * ((move_r - move_l) / BASE) / PI);
      rotate_vector(eye_r, -180 * ((move_r - move_l) / BASE) / PI);
      //draw and fill updated polygon in temp_bmp
*/

      //serialize image and display
      size_t bmp_size = bmp_calculate_size(&bmp);
      uint8_t *serialized_bmp = malloc(bmp_size);
      bmp_size = bmp_calculate_size(&bmp);
      bmp_serialize(&bmp, serialized_bmp);

      bool condition = false;
      if (atoi(argv[2]) == 0){
        condition = true;
      }
      if (atoi(argv[2]) == 1){
        if (time_step == atoi(argv[1])){
          condition = true;
        }
      }
      if (condition == true){
        //write data to file
        FILE *f = fopen("my_image.bmp", "wb");
        fwrite(serialized_bmp, bmp_size, 1, f);
        fclose(f);
        //get data on server
        image_server_set_data(bmp_size, serialized_bmp);
        image_server_start("8000"); // you could change the port number, but animation.html wants 8000
        sleep(1);
      }
      if (atoi(argv[2]) == 0){
        int seconds = 0;
        long nanoseconds = 20 * 1000 * 1000;
        struct timespec interval = { seconds, nanoseconds };
        nanosleep(&interval, NULL);
      }

      /*free_data(robot);
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
    return 0;*/
    }
  }
}
