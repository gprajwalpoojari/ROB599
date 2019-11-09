#include <stdio.h>
#include <stdlib.h>
char char1;
int int1;
double doubles[3];
int main(void){

  printf("%p\n",(void *)&char1);
  printf("%p\n\n",(void *)&int1);

  long distance = (long)&char1 - (long)&int1;

  printf("%ld\n\n",distance);

  printf("%p\n\n",&doubles);

  for ( int i = 0; i < 3; i++){
    printf("%p\n",(void *)&doubles[i]);
  }
  printf("\n");

  for (int i = 1; i < 3; i++){
    printf("%ld\n",&doubles[i + 1] - &doubles[i]);
  }
  printf("\n");

  for (int i = 1; i < 3; i++){
    printf("%ld\n",(long)&doubles[i + 1] - (long)&doubles[i]);
  }

  char char2;
  int int2;
  float floats[3];

  printf("%p\n",(void *)&char2);
  printf("%p\n\n",(void *)&int2);

  long distance2 = (long)&char2 - (long)&int2;

  printf("%ld\n\n",distance2);

  printf("%p\n\n",&floats);

  for ( int i = 0; i < 3; i++){
    printf("%p\n",(void *)&floats[i]);
  }
  printf("\n");

  for (int i = 1; i < 3; i++){
    printf("%ld\n",&floats[i + 1] - &floats[i]);
  }
  printf("\n");

  for (int i = 1; i < 3; i++){
    printf("%ld\n",(long)&doubles[i + 1] - (long)&doubles[i]);
  }

  void *mem = malloc(1); // ask for memory
  printf("%p\n", mem); // memory is already "given" to us as an address!
  free(mem); // give memory back
}
