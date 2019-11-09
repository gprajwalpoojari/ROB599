#include <stdio.h>
#include <stdlib.h>

typedef struct vector {
  size_t size;
  size_t capacity;
  int *data;
}vector_t;

vector_t vector_create(void);
void vector_append(vector_t *vector, int value);

int main(int argc, char **argv){
  if (argc == 2){
    int max = atoi(argv[1]);
    vector_t golomb = vector_create();
    vector_append(&golomb, 1);
    vector_append(&golomb, 2);
    vector_append(&golomb, 2);
    //golomb sequence
    for (int i = 2; i < max; i++){
      int temp = golomb.data[i];
      for (int j = 0; j < temp; j++){
          vector_append(&golomb,i+1);
      }
    }
    //print sequence
    if (max == 0){
      return 0;
    }
    else if(max == 1){
      printf("%d\n",golomb.data[0]);
    }
    else{
      for (int i = golomb.size-1; i >= 0; i--){
        printf("%d\n",golomb.data[i]);
      }
    }
  }
  else{
    fprintf(stderr,"usage: ./golomb <max golomb number>\n");
  }
}
vector_t vector_create(void)
{
  vector_t vector1;
  vector1.size = 0;
  vector1.capacity = 64;
  vector1.data = malloc(vector1.capacity * sizeof(int));
  return vector1;
}

void vector_append(vector_t *vector, int value)
{
  if (vector->size == vector->capacity) {
    vector->capacity *= 2;
    vector->data = realloc(vector->data, sizeof(int) * vector->capacity);
    vector->data[vector->size] = value;
    vector->size++;
  }
  else {
    vector->data[vector->size] = value;
    vector->size++;
    }
  }
