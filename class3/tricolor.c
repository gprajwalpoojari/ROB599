#include<stdio.h>
#include<string.h>
#include<stdint.h>
#include<stdlib.h>

int main(void){
  char *trit_encodings[243];
  for (int i = 0; i < 243; i++){
    trit_encodings[i] = calloc(sizeof(char), 6);
    int n = i;
    for (int j = 0; j < 5; j++){
      int temp_trit = n % 3;
      if (temp_trit == 0){
        trit_encodings[i][j] = ' ';
      }
      else if(temp_trit == 1){
        trit_encodings[i][j] = ':';
      }
      else{
        trit_encodings[i][j] = '@';
      }
      n=n/3;
    }

  }
  FILE *f = fopen("img.bin", "rb");
  for (int i = 0; i < 40; i++){
    for (int j = 0; j < 18; j++){
      char c = fgetc(f);
      printf("%s",trit_encodings[(uint8_t)c]);
    }
    printf("\n");
  }
}
