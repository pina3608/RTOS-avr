#include <stdio.h>

void a(){
  printf("a\n");
}

void b(){
  printf("b\n");
}


void exec(void (*test) (void)){

  test();
}

int main(){
  exec(a);
  exec(b);
}
