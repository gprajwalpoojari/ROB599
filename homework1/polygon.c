#include<stdio.h>
#include<math.h>
#include<stdlib.h>

typedef struct data{
  float transform[3];
  int points;
  float *coordinates[2];
}data_t;

data_t extract_data(FILE *fptr);

int main(void){
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
}

data_t extract_data(FILE *fptr)
{
  data_t data_a;
  for (int i = 0; i < 3; i++){
    fscanf(fptr, "%f", &data_a.transform[i]);
  }
  fscanf(fptr, "%d", &data_a.points);
  for(int j = 0; j < 2; j++){
    data_a.coordinates[j] = calloc(data_a.points,sizeof(float));
    for(int k = 0; k < data_a.points; k++){
      fscanf(fptr, "%f", &data_a.coordinates[j][k]);
    }
  }
  return data_a;
}
