#include "collision.h"
#include "vector_xy_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



//function for creating a polygon using points data
polygon_t make_polygon(vector_xy_t robot)
{
  polygon_t polygon;
  polygon.n_points = robot.size;
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
    polygon.line[j].point[0] = robot.point[j];
    polygon.line[j].point[1] = robot.point[k];
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


bool check_collision(polygon_t polygon_a, polygon_t polygon_b)
{
  //create polygons
  polygon_t *polygon = calloc(2, sizeof(polygon_t));
  polygon[0] = polygon_a;
  polygon[1] = polygon_b;

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
  return COLLISION;
}

void print_result(bool COLLISION)
{
  //print
  if (COLLISION == true){
    printf("collision!\n");
  }
  else{
    printf("no collision\n");
  }
}


/*bool resolve_collision(vector_xy_t *lamp_centroid, vector_xy_t *polygon_centroid, polygon_t *lamp_polygon, polygon_t *robot_polygon, vector_xy_t *robot_vector){
  vector_xy_t *new_dir =  create_vector();
  new_dir->data_x[1] = lamp_centroid->data_x[0];
  new_dir->data_y[1] = lamp_centroid->data_y[0];
  new_dir->data_x[0] = polygon_centroid->data_x[0];
  new_dir->data_y[0] = polygon_centroid->data_y[0];
  new_dir->size = 2;
  vector_xy_t *horizontal = create_vector();
  horizontal->data_x[1] = 1;
  horizontal->data_y[1] = 0;
  horizontal->data_x[0] = 0;
  horizontal->data_y[0] = 0;
  horizontal->size = 2;
  double cos_theta = dot(*new_dir, *horizontal);
  //double dist = sqrt(pow(new_dir->data_x[1] - new_dir->data_x[0], 2) + pow(new_dir->data_y[1] - new_dir->data_y[0], 2));
  double h_movement = 0.5 * (1 - cos_theta);
  double v_movement;
  if (new_dir->data_y[1] > new_dir->data_y[0]){
    v_movement = - 0.5 * cos_theta;
  }
  else{
    v_movement = 0.5 * cos_theta;
  }
  translate_vector(robot_vector, h_movement, v_movement);
  robot_polygon = create_polygon(robot_vector);
  bool c1 = check_collision(robot_polygon, lamp_polygon);
  if (c1 == true){
    printf("Collision!");
  }
  //free_data(new_dir);
  free_data(horizontal);
  return c1;
}*/
