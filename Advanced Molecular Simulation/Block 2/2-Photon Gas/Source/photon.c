#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ran_uniform.h"

#define CycleMultiplication 1000

int main(void)
{
  int  NumberOfCycles,NumberOfInitializationSteps,New,Old,i,j,Count;
  double Beta,Sum;

  // initialize the random number generator with the system time
  InitializeRandomNumberGenerator(time(0l));

  // read the input parameters
  printf("How many cycles (x %d)? ",CycleMultiplication);
  fscanf(stdin,"%d",&NumberOfCycles);

  printf("How many initialization cycles (x %d)? ",CycleMultiplication);
  fscanf(stdin,"%d",&NumberOfInitializationSteps);

  if(NumberOfInitializationSteps>=NumberOfCycles)
  {
    printf("Initialisation must be shorter than the run!\n");
  exit(0); 
  } 

  printf("Beta*epsilon ? (Example: 1.0");
  fscanf(stdin,"%lf",&Beta);

  FILE * occupancies;
  occupancies=fopen("occupancies.txt","w");

  New=1;
  Old=1;
  Sum=0.0;
  Count=0;
  int Populations[NumberOfCycles*CycleMultiplication];

  // Loop Over All Cycles
  for(i=0;i<NumberOfCycles;i++)
  {
    for(j=0;j<CycleMultiplication;j++)
    {
      // start modification
      if(RandomNumber()<0.5)
      {
        New=Old+1;
      }
      else
      {
        New=Old-1;
      }
      if(New<0)
      {
        New=0;
      }
      // end   modification

      // accept or reject
      if(RandomNumber()<exp(-Beta*(New-Old)))
      {  
        Old=New;
      }
      // calculate average occupancy result
      if(i>NumberOfInitializationSteps)
      {
        Sum+=Old;
        Populations[Count]=Old;
        fprintf(occupancies, "%d\n", Populations[Count]);
        Count+=1;
      }
    }
  }
  fclose(occupancies);
  printf("%d", Count);
  // write the final result
  printf( "\nResults:\n" );
  printf("Average Value     : %lf\n",Sum/Count);
  printf("Theoretical Value : %lf\n",1.0/(exp(Beta)-1.0));
  printf("Relative Error    : %lf\n",fabs((exp(Beta)-1.0)*((Sum/Count) - (1.0/(exp(Beta)-1.0)))));
  printf("Probability of nj : %lf\n",exp(Sum/Count*Beta)/(1.0/(1.0-exp(-Beta))));

  return 0;
}
