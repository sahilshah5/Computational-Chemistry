#include<stdio.h>
#include<stdlib.h>

#include "system.h"
#include "ran_uniform.h"


int main(void){
  int i;

  InitializeRandomNumberGenerator(time(0l));
  for(i=0;i<100000;i++) printf(" %lf\n",RandomGaussianNumber());

  return 0;
}
