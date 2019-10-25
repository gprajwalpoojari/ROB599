#include<stdio.h>
#include<stdbool.h>
#include<string.h>

int main(int argc, char **argv){
  if(argc > 2){
    printf("Too many arguments\nusage: ./parity [<text>]\n");
  }
  else{
    bool parity = 0;
    if (argc == 2){
      for (int i = 0; i < strlen(argv[1]); i++){
        char ch = argv[1][i];
        for (int j = 0; j < 8; j++){
          parity = parity ^ (ch & 0x01);
          ch = ch >> 1;
        }
      }
    }
    printf("The Parity is:%d\n",parity);
  }
}
