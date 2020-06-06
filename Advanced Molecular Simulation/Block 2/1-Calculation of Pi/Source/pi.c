#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ran_uniform.h"

#define CycleMultiplication 1000

#define SQR(x) ((x)*(x))

// calculates Pi using the circle/square method
// in general the hit/miss methods are usefull for n-dimensionals
// integrals with n large.

#define NR_COUNTS 100

int main(void)
{
  int i,j,k,NumberOfCycles;
  double ratio,x,y,pi;
  double NumberOfHits,NumberOfTrials;

  InitializeRandomNumberGenerator(time(0l));

  printf("Number of cycles (x %d)? ",CycleMultiplication);
  fscanf(stdin,"%d",&NumberOfCycles);

  printf("Ratio l/d        ? (always >= 1 !) ");
  fscanf(stdin,"%lf",&ratio);

  if(ratio<1.0)
  {
    printf("Ratio must be At least 1 !!!");
    exit(0);
  }

  pi=0.0;
  NumberOfHits=0.0;
  NumberOfTrials=0.0;

  // loop over all cycles
  for(k=0;k<NR_COUNTS;k++)
  {
    pi=0.0;
    NumberOfHits=0.0;
    NumberOfTrials=0.0;
    for(i=0;i<NumberOfCycles;i++)
    {
      for(j=0;j<CycleMultiplication;j++)
      {
      // generate a random point
      // check if it is in the circle
      // NumberOfTrial = number of points
      // NumberOfHits  = number in the circle

      // Start Modifications
      x = (RandomNumber() - 0.5)* ratio;
      y = (RandomNumber() - 0.5)* ratio;
      if (sqrt(x*x + y*y) < 0.5) 
      {
        NumberOfHits++;
        NumberOfTrials++;
      }
      else
      {
        NumberOfTrials++;
      }      
      // End Modifications
       }
     // Remember that we are using r = 0.5d.
     pi=4*SQR(ratio)*NumberOfHits/NumberOfTrials;
   }
  }
  printf("Estimate of Pi : %f\n",pi);
  printf("Real Pi        : %f\n",M_PI);
  printf("Relative error : %g\n",fabs(pi-M_PI)/M_PI); 
  return 0;
}
