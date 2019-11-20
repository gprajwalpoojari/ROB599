#include "collision.h"
#include "vector_xy_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


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

bool check_collision(polygon_t *polygon_a, polygon_t *polygon_b)
{
  bool collision = false;
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
        if(prod[0] * prod[1] <= 0){
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
          collision = true;
          printf("Collision!\n");
          break;
        }
      }
    }
  }
  return collision;
}

void overlap(polygon_t *polygon_a, polygon_t *polygon_b)
{
  double temp_1[polygon_a->n_line];
  double temp_2[polygon_b->n_line];
  bool overlap_check_1 = false;
  bool overlap_check_2 = false;
  for (int polygon_n = 0; polygon_n < 2; polygon_n++){
    if (polygon_n == 0){
      for (int i = 0; i < polygon_a->n_line; i++){
        temp_1[i] = cross_product(polygon_a->line[i], polygon_b->line[0], 0);
      }
      int positive = 0;
      int negative = 0;
      for (int i = 0; i < polygon_a->n_line; i++){
        if (temp_1[i] >=0){
          positive++;
        }
        else if(temp_1[i] <= 0){
          negative++;
        }
      }
      if (positive == polygon_a->n_line || negative == polygon_a->n_line){
        overlap_check_1 = true;
      }
    }
    else{
      for (int i = 0; i < polygon_b->n_line; i++){
        temp_2[i] = cross_product(polygon_b->line[i], polygon_a->line[0], 0);
      }
      int positive = 0;
      int negative = 0;
      for (int i = 0; i < polygon_b->n_line; i++){
        if (temp_2[i] >=0){
          positive++;
        }
        else if(temp_2[i] <= 0){
          negative++;
        }
      }
      if (positive == polygon_b->n_line || negative == polygon_b->n_line){
        overlap_check_2 = true;
      }
    }
  }
  if (overlap_check_1 && overlap_check_2 == true){
    printf("Collision!");
  }
}
