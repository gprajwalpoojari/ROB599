#include "collision.h"
#include "vector_xy_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/*float overlap(polygon_t polygon_a, polygon_t polygon_b, int var)
{
  float vx = polygon_b.points[0][0] - polygon_a.points[0][var];
  float vy = polygon_b.points[1][0] - polygon_a.points[1][var];
  float value = polygon_a.lines[0][var] * vy - vx * polygon_a.lines[1][var];
  return value;
}*/

polygon_t *create_polygon(vector_xy_t *data)
{
  polygon_t *polygon = malloc(sizeof(polygon_t));
  polygon->n_line = 0;
  polygon->line = malloc(sizeof(vector_xy_t));
  for (int i = 0; i < data->size; i++){
    int tmp;
    if (i == data->size - 1){
      tmp = 0;
    }
    else{
      tmp = i + 1;
    }
    polygon->line[i] = create_vector();
    polygon->line[i]->data_x[0] = data->data_x[i];
    polygon->line[i]->data_y[0] = data->data_y[i];
    polygon->line[i]->data_x[1] = data->data_x[tmp];
    polygon->line[i]->data_y[1] = data->data_y[tmp];
    polygon->line[i]->size++;
    polygon->n_line++;
  }
  return polygon;
}

double cross_product(vector_xy_t *line_1, vector_xy_t *line_2, int point_number){
  double v1x = line_1->data_x[1] - line_1->data_x[0];
  double v1y = line_1->data_y[1] - line_1->data_y[0];
  double v2x = line_2->data_x[point_number] - line_1->data_x[0];
  double v2y = line_2->data_y[point_number] - line_1->data_y[0];
  double cross_product = v1x * v2y - v2x * v1y;
  return cross_product;
}

bool check_collision(polygon_t *polygon_a, polygon_t *polygon_b){
  bool collision;
  int both_zero_check = 0;
  double prod[2];
  int check[2];
  for (int i = 0; i < polygon_a->n_line; i++){
    for (int j = 0; j < polygon_b->n_line; j++){
      both_zero_check = 0;
      for (int count = 0; count < 2; count++){
        for (int k = 0; k < 2; k++){
          if (count == 0){
            prod[k] = cross_product(polygon_a->line[i], polygon_b->line[j], k);

          }
          else{
            prod[k] = cross_product(polygon_b->line[j], polygon_a->line[i], k);

          }
        }
        if(prod[0] * prod[1] < 0 || (prod[0] == 0 || prod[1] == 0)){
          check[count] = 1;
        }
        else{
          check[count] = 0;
        }
        if (prod[0] * prod[1] == 0){
          both_zero_check++;
        }
      }
      if (check[0] == 1 && check[1] == 1){
        if (both_zero_check != 2){
          printf("collision!\n");
          break;
        }
      }
    }
  }
  return 0;
}
