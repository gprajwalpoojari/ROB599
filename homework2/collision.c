#include "collision.h"
#include "vector_xy_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



int cross_product(polygon_t polygon_a, polygon_t polygon_b, int m, int n, vector_xy_t *polygon)
{
  int value = polygon->size;
  float v1x = polygon_b.points[0][n] - polygon_a.points[0][m];
  float v1y = polygon_b.points[1][n] - polygon_a.points[1][m];
  float cross_prod_1 = v1y * polygon_a.lines[0][m] - v1x * polygon_a.lines[1][m];
  int var;
  if(value == polygon->size - 1){
    var = 0;
  }
  else{
    var = n + 1;
  }
  float v2x = polygon_b.points[0][var] - polygon_a.points[0][m];
  float v2y = polygon_b.points[1][var] - polygon_a.points[1][m];
  float cross_prod_2 = v2y * polygon_a.lines[0][m] - v2x * polygon_a.lines[1][m];
  int tmp;
  if(cross_prod_1 * cross_prod_2 < 0){
    tmp = 1;
  }
  else if(cross_prod_1 == 0 || cross_prod_2 == 0 || cross_prod_1 * cross_prod_2 > 0){
    if((cross_prod_1 == 0 && cross_prod_2 == 0) || cross_prod_1 * cross_prod_2 >= 0){
      tmp = 0;
    }
    else{
      tmp = 2;
    }
  }
  return tmp;
}

float overlap(polygon_t polygon_a, polygon_t polygon_b, int var)
{
  float vx = polygon_b.points[0][0] - polygon_a.points[0][var];
  float vy = polygon_b.points[1][0] - polygon_a.points[1][var];
  float value = polygon_a.lines[0][var] * vy - vx * polygon_a.lines[1][var];
  return value;
}

polygon_t create_polygon(vector_xy_t *data)
{
  polygon_t polygon;
  //calculating new coordinates for 1st polygon
  for (int row = 0; row < 2; row++){
    polygon.points[row] = calloc(data->size, sizeof(float));
    for (int column = 0; column < data->size; column++){
      if(row == 0){
          polygon.points[row][column] = data->data_x[column];
      }
      else{
          polygon.points[row][column] = data->data_y[column];
      }
    }
  }
  //create line vectors for polygon 1
  for (int row = 0; row < 2; row++){
    for (int column = 0;column < data->size; column++){
      int x;
      if(column == data->size-1){
        x = 0;
      }
      else{
        x = column+1;
      }
      polygon.lines[row][column] = polygon.points[row][x] - polygon.points[row][column];
    }
  }
  return polygon;
}

void check_collision(polygon_t polygon_a,polygon_t polygon_b, vector_xy_t *data_1, vector_xy_t *data_2)
{
  //calculating cross product and detect collision
    bool collision = false;
    for(int a = 0; a < data_1->size; a++){
      for(int b = 0; b < data_2->size; b++){
        int product_1 = cross_product(polygon_a, polygon_b, a, b, data_2);
        int product_2 = cross_product(polygon_b, polygon_a, b, a, data_1);
        if((product_1 == 1 && product_2 == 1) || (product_1 == 2 && product_2 == 1) || (product_1 == 1 && product_2 == 1)){
          collision = true;
        }
      }
    }
    //check for complete overlapping
    if (collision == false){
      float contain_1[data_1->size];
      bool check_1 = true;
      bool check_2 = true;
      for (int j = 0; j < data_1->size; j++){
        contain_1[j] = overlap(polygon_a, polygon_b, j);
        if (contain_1[j] > 0){
          check_1 = false;
        }
      }
      for (int j = 0; j < data_2->size; j++){
        contain_1[j] = overlap(polygon_b, polygon_a, j);
        if (contain_1[j] > 0){
          check_2 = false;
        }
      }
      if (check_1 == false && check_2 == false){
        collision = false;
      }
      else{
        collision = true;
      }
    }
    if (collision == true){
      printf("collision!\n");
    }
    else{
      printf("no collision\n");
    }
}
