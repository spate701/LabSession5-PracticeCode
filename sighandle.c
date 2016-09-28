#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

void sigint_handler(int sig){
  printf("Signal handled.  Deal with it.\n");
  exit(0);
}

int main(){
  signal(SIGINT, sigint_handler);

  int i = 99;
  while(1){
    printf("%d bottles of beer on the wall\n", i);
    printf("%d bottles of beer\n", i);
    printf("Take one down, pass it around\n");
    i = i - 1;
    printf("%d bottles of beer on the wall\n", i);
  }


}
