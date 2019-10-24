#include<stdio.h>

int main(void){
  int n;
  printf("Enter a nunber to factorize:\n");
  scanf("%d",&n);
  printf("Prime factors:\n");
  for(int i=1; i<=n; i++){
    if(i>1){
      if(n%i==0){
        while(n%i==0){
          n=n/i;
          printf("%d\n",i);
        }

      }
    }
    else{
    printf("%d\n",i);
    }
  }
}
