#include<stdio.h>
#include<stdint.h>
#include<string.h>

int main(int argc,char **argv){
  printf("This program was given %d arguments\n",argc);
  for (int i = 0 ; i < argc; i++){
    char *arg = argv[i];
    printf("The %d argument is %s\n",i, arg);
    int len = strlen(arg);
    for (int j =0;j < len; j++){
      printf("%d: character %c = %d\n",j, arg[j], arg[j]);
    }
  }
  return 0;
}
