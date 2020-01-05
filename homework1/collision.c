#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#define PI 3.1415
#define CONSTANT PI/180


//defining a point
typedef struct point{
  double x;
  double y;
}point_t;

//defining a line
typedef struct line{
  point_t point[2];
}line_t;

//defining a polygon
typedef struct polygon{
  int n_points;
  line_t *line;
}polygon_t;

//structure for storing extracted data from csv file
typedef struct data{
  int n_points;
  float transformation[3];
  float *coordinates[2];
}data_t;

//function for extracting data from csv file
data_t extract_data(FILE *fptr)
{
  data_t data;
  for (int i = 0; i < 3; i++){
    //store translation and rotation
    fscanf(fptr, "%f", &data.transformation[i]);
  }
  //store number of points
  fscanf(fptr, "%d", &data.n_points);

  //store x and y coordinates
  for(int j = 0; j < 2; j++){
    data.coordinates[j] = calloc(data.n_points,sizeof(float));
    for(int k = 0; k < data.n_points; k++){
      fscanf(fptr, "%f", &data.coordinates[j][k]);
    }
  }
  return data;
}

//function for creating a polygon using points data
polygon_t make_polygon(data_t data)
{
  polygon_t polygon;
  polygon.n_points = data.n_points;
  //store point data of a polygon in one place
  point_t *point = calloc(polygon.n_points, sizeof(point_t));
  for (int i = 0; i < polygon.n_points; i++){
    point[i].x = data.coordinates[0][i];
    point[i].y = data.coordinates[1][i];
    //perform rotation
    double x_rotation = point[i].x * cos(CONSTANT * data.transformation[2]) - point[i].y * sin(CONSTANT * data.transformation[2]);
    double y_rotation = point[i].x * sin(CONSTANT * data.transformation[2]) + point[i].y * cos(CONSTANT * data.transformation[2]);
    point[i].x = x_rotation;
    point[i].y = y_rotation;
    //perform translation
    point[i].x = point[i].x + data.transformation[0];
    point[i].y = point[i].y + data.transformation[1];
  }
  //create line vectors
  polygon.line = calloc(polygon.n_points, sizeof(line_t));
  for (int j = 0; j < polygon.n_points; j++){
    int k;
    if (j == polygon.n_points - 1){
      k = 0;
    }
    else{
      k = j + 1;
    }
    polygon.line[j].point[0] = point[j];
    polygon.line[j].point[1] = point[k];
  }
  return polygon;
}

//function for computing cross product between a line and a point
double cross_product(line_t line, point_t point)
{
  double x1, y1, x2, y2;
  x1 = line.point[1].x - line.point[0].x;
  y1 = line.point[1].y - line.point[0].y;
  x2 = point.x - line.point[0].x;
  y2 = point.y - line.point[0].y;
  //cross product formula
  double cross = x1 * y2 - x2 * y1;
  return cross;
}

int main(void){
  data_t data_1;
  data_t data_2;
  FILE *fp = fopen("polygons.csv","r");
  char buff;
  //read the first(key) line
  while (buff != '\n'){
    fscanf(fp,"%c",&buff);
  }
  data_1 = extract_data(fp);
  if (data_1.n_points > 16){
    fprintf(stderr,"points greater than 16\n");
    return 1;
  }
  data_2 = extract_data(fp);
  if (data_2.n_points > 16){
    fprintf(stderr,"points greater than 16\n");
    return 1;
  }

  fclose(fp);

  //create polygons
  polygon_t *polygon = calloc(2, sizeof(polygon_t));
  polygon[0] = make_polygon(data_1);
  polygon[1] = make_polygon(data_2);

  bool COLLISION = false;
  //polygon collision algorithm
  for (int i = 0; i < polygon[0].n_points; i++){
    for (int j = 0; j < polygon[1].n_points;j++){
      bool probability[2] = {false}, exception[2] = {false};
      line_t *line = calloc(2, sizeof(line_t));
      line[0] = polygon[0].line[i];
      line[1] = polygon[1].line[j];
      //for each line l in [l1, l2]
      for (int k = 0; k < 2; k++){
        int m;
        if (k == 0){
          m = 1;
        }
        else{
          m = 0;
        }
        //for each point p in the other line
        double cross[2];
        for (int l = 0; l < 2; l++){
          cross[l] = cross_product(line[k],line[m].point[l]);
        }
        //If the two cross products have opposite sign, or one of them is zero
        if (cross[0] * cross[1] <= 0){
          if (cross[0] != 0 || cross[1] != 0){
            probability[k] = true;
            //store true value if only one of them is zero
            if (cross[0] == 0 || cross[1] == 0){
              exception[k] = true;
            }
            else{
              exception[k] = false;
            }
          }
          else{
            probability[k] = false;
          }
        }
      }
      //If both cross product checks above indicated opposite signs,
      if (probability[0] && probability[1] == true){
        //check for atleast one zero cross product in both cases
        if (!(exception[0] && exception[1])){
          COLLISION = true;
          break;
        }
      }
    }
  }

  //check for complete overlap
  if (COLLISION == false){
    bool collision[2];
    int k;
    //for each polygon
    for (int i = 0; i < 2; i++){
      if (i == 0){
        k = 1;
      }
      else{
        k = 0;
      }
      double cross[polygon[i].n_points];
      //for each line in polygon[i]
      for (int j = 0; j < polygon[i].n_points; j++){
        cross[j] = cross_product(polygon[i].line[j], polygon[k].line[0].point[1]);
      }
      for (int l = 0; l < polygon[i].n_points; l++){
        if (cross[l] > 0){
          collision[i] = false;
          break;
        }
        else{
          collision[i] = true;
        }
      }
    }
    if (collision[0] && collision[1]){
      COLLISION = true;
    }
  }

  //print
  if (COLLISION == true){
    printf("collision!\n");
  }
  else{
    printf("no collision\n");
  }
}
