#include<stdio.h>
#include<string.h>
#include<stdint.h>

int main(int argc, char **argv){
  if (argc==1){
    printf("Error-Please provide an argument after running the program!\n");
    return 1;
  }
  else{
    for (int i = 1; i < argc; i++){
      char *arg = argv[i];
      int len =strlen(arg);
      for (int j=0; j<len; j++){
        if (arg[j]>=97 && arg[j]<=122){
          arg[j]=arg[j]-32;
        }
      }
      printf("%s ", arg);
    }
  }
  printf("\n");
}
