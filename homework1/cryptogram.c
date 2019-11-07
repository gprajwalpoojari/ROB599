#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define CONSTANT 32

int main(char **argv){
  int num_p = 0;
  int num_w = 0;
  //calculate number of characters in password
  while(argv[2][num_p] != '\0'){
    num_p++;
  }
  //calculate number of characters in words
  while(argv[3][num_w] != '\0'){
    num_w++;
  }
  int pass = 0;
  if (num_p ==0){
    pass = 1;
  }
  else{
    for (int i =0; i < num_p; i++){
      if ((argv[2][i] >= 'a' && argv[2][i] <= 'z') || (argv[2][i] >= 'A' && argv[2][i] <= 'Z')){
        pass++;
      }
    }
  }
  //encrypt word and print
  char modify[num_w];
  int new_var = 0;
  if (!strcmp(argv[1],"encrypt")){
    for (int var = 0; var < num_w; var++){
      if (new_var == pass){
        new_var = new_var - pass;
      }
      if (argv[3][var] >= 'a' && argv[3][var] <= 'z'){
        modify[var] = argv[3][var] + new_var;
        new_var++;
        if (modify[var] > 'z'){
          modify[var] = modify[var] - CONSTANT;
        }
      }
      else if (argv[3][var] >= 'A' && argv[3][var] <= 'Z'){
        modify[var] = argv[3][var] + new_var;
        new_var++;
        if (modify[var] > 'Z'){
          modify[var] = modify[var] - CONSTANT;
        }
      }
      else{
        modify[var] = argv[3][var];
      }
      printf("%c",modify[var]);
    }
  }
  //decrypt word and print
  else if (!strcmp(argv[1],"decrypt")){
    for (int var = 0; var < num_w; var++){
      if (new_var == pass){
        new_var = new_var - pass;
      }
      if (argv[3][var] >= 'a' && argv[3][var] <= 'z'){
        modify[var] = argv[3][var] - new_var;
        new_var++;
        if (modify[var] < 'a'){
          modify[var] = modify[var] + CONSTANT;
        }
      }
      else if (argv[3][var] >= 'A' && argv[3][var] <= 'Z'){
        modify[var] = argv[3][var] - new_var;
        new_var++;
        if (modify[var] < 'A'){
          modify[var] = modify[var] + CONSTANT;
        }
      }
      else{
        modify[var] = argv[3][var];
      }
      printf("%c",modify[var]);
    }
  }
  else{
    printf("expected command encrypt or decrypt. got %s",argv[1]);
  }

}
