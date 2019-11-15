#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#define PI 3.1415

typedef struct data{
  float transform[3];
  int points;
  float *coordinates[3];
}data_t;
typedef struct polygon{
  float *points[3];
  float lines[3][16];
}polygon_t;

data_t extract_data(FILE *fptr);
int cross_product(polygon_t polygon_a, polygon_t polygon_b, int m, int n, data_t data_b);
float overlap(polygon_t polygon_a, polygon_t polygon_b, int var);

int main(void){
  float constant = PI / 180;
  data_t data_1;
  data_t data_2;
  FILE *fp = fopen("polygons.csv","r");
  char buff;
  while (buff != '\n'){
    fscanf(fp,"%c",&buff);
  }
  data_1 = extract_data(fp);
  data_2 = extract_data(fp);
  fclose(fp);
//transformation matrix for 1st polygon
  double transform_1[3][3];
  transform_1[0][0] = cos(constant * data_1.transform[2]);
  transform_1[0][1] = -sin(constant * data_1.transform[2]);
  transform_1[0][2] = data_1.transform[0];
  transform_1[1][0] = sin(constant * data_1.transform[2]);
  transform_1[1][1] = cos(constant * data_1.transform[2]);
  transform_1[1][2] = data_1.transform[1];
  transform_1[2][0] = 0;
  transform_1[2][1] = 0;
  transform_1[2][2] = 1;
//transformation matrix for 2nd polygon
  double transform_2[3][3];
  transform_2[0][0] = cos(constant * data_2.transform[2]);
  transform_2[0][1] = -sin(constant * data_2.transform[2]);
  transform_2[0][2] = data_2.transform[0];
  transform_2[1][0] = sin(constant * data_2.transform[2]);
  transform_2[1][1] = cos(constant * data_2.transform[2]);
  transform_2[1][2] = data_2.transform[1];
  transform_2[2][0] = 0;
  transform_2[2][1] = 0;
  transform_2[2][2] = 1;

  polygon_t polygon_1;
  //calculating new coordinates for 1st polygon
  for (int row = 0; row < 3; row++){
    polygon_1.points[row] = calloc(data_1.points, sizeof(float));
    for (int column = 0; column < data_1.points; column++){
      int sum = 0;
      for (int i = 0; i < 3; i++){
        sum = sum + transform_1 [row][i] * data_1.coordinates[i][column];
      }
      polygon_1.points[row][column] = sum;
    }
  }
  //create line vectors for polygon 1
  for (int row = 0; row < 3; row++){
    for (int column = 0;column < data_1.points; column++){
      int x;
      if(column == data_1.points-1){
        x = 0;
      }
      else{
        x = column+1;
      }
      polygon_1.lines[row][column] = polygon_1.points[row][x] - polygon_1.points[row][column];
    }
  }

  polygon_t polygon_2;
//calculating new coordinates for 2nd polygon
  for (int row = 0; row < 3; row++){
    polygon_2.points[row] = calloc(data_2.points, sizeof(float));
    for (int column = 0; column < data_2.points; column++){
      float sum = 0;
      for (int i = 0; i < 3; i++){
        sum = sum + (transform_2 [row][i]) * (data_2.coordinates[i][column]);
      }
      polygon_2.points[row][column] = sum;
    }
  }
  //create line vectors for polygon 2
  for (int row = 0; row < 3; row++){
    for (int column = 0;column < data_2.points; column++){
      int x;
      if(column == (data_2.points) - 1){
        x = 0;
      }
      else{
        x = column + 1;
      }
      polygon_2.lines[row][column] = polygon_2.points[row][x] - polygon_2.points[row][column];
    }
  }

//calculating cross product and detect collision
  bool collision = false;
  for(int a = 0; a < data_1.points; a++){
    for(int b = 0; b < data_2.points; b++){
      int product_1 = cross_product(polygon_1, polygon_2, a, b, data_2);
      int product_2 = cross_product(polygon_2, polygon_1, b, a, data_1);
      if((product_1 == 1 && product_2 == 1) || (product_1 == 2 && product_2 == 1) || (product_1 == 1 && product_2 == 1)){
        collision = true;
      }
    }
  }
  //check for complete overlapping
  if (collision == false){
    float contain_1[data_1.points];
    bool check_1 = true;
    bool check_2 = true;
    for (int j = 0; j < data_1.points; j++){
      contain_1[j] = overlap(polygon_1, polygon_2, j);
      if (contain_1[j] > 0){
        check_1 = false;
      }
    }
    for (int j = 0; j < data_2.points; j++){
      contain_1[j] = overlap(polygon_2, polygon_1, j);
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

data_t extract_data(FILE *fptr)
{
  data_t data_a;
  for (int i = 0; i < 3; i++){
    fscanf(fptr, "%f", &data_a.transform[i]);
  }
  fscanf(fptr, "%d", &data_a.points);
  for(int j = 0; j < 3; j++){
    data_a.coordinates[j] = calloc(data_a.points,sizeof(float));
    for(int k = 0; k < data_a.points; k++){
      if(j == 2){
        data_a.coordinates[j][k] = 1;
      }
      else{
        fscanf(fptr, "%f", &data_a.coordinates[j][k]);
      }
    }
  }
  return data_a;
}

int cross_product(polygon_t polygon_a, polygon_t polygon_b, int m, int n, data_t data_b)
{
  int value = data_b.points;
  float v1x = polygon_b.points[0][n] - polygon_a.points[0][m];
  float v1y = polygon_b.points[1][n] - polygon_a.points[1][m];
  float cross_prod_1 = v1y * polygon_a.lines[0][m] - v1x * polygon_a.lines[1][m];
  int var;
  if(value == data_b.points - 1){
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
