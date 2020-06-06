#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ran_uniform.h"
#include "sample.h"

// random walk 1d

#define MAX_LATTICE 100000
#define CycleMultiplication 1000

#define SQR(x) ((x)*(x))

enum{RIGHT=1,LEFT=-1};

int main(void)
{
  int NumberOfJumps,NumberOfCycles;
  int i,j,k;
  int CurrentPosition;
  double Distribution[2*MAX_LATTICE+1],Normalization,P;
  FILE *FilePtr;

  // initialize the random number generator with the system time
  InitializeRandomNumberGenerator(time(0l));

  printf("Number of Cycles (x %d)? ",CycleMultiplication);
  fscanf(stdin,"%d",&NumberOfCycles);

  printf("Number of Jumps/Cycle ? ");
  fscanf(stdin,"%d",&NumberOfJumps);

  Sample(INITIALIZE,1);


  for(i=-MAX_LATTICE;i<=MAX_LATTICE;i++)
    Distribution[i+MAX_LATTICE]=0.0;

  // loop over cycles
  Normalization=0.0;
  for(i=0;i<NumberOfCycles;i++)
  {
    for(j=0;j<CycleMultiplication;j++)
    {
      Sample(START_NEW_WALK,1);

      CurrentPosition=0;

      // perform the random walk
      for(k=0;k<NumberOfJumps;k++)
      {
        if(RandomNumber()<0.8)  // Change to 0.8 and investigate
          CurrentPosition+=RIGHT;
        else
          CurrentPosition+=LEFT;

        Sample(SAMPLE_WALK,CurrentPosition);
      }
      Normalization+=1.0;

      if((CurrentPosition>=-MAX_LATTICE)||(CurrentPosition<=MAX_LATTICE))
        Distribution[CurrentPosition+MAX_LATTICE]+=1.0;
    }
  }

  // write results
  Sample(WRITE_RESULTS,1);

  FilePtr=fopen("results.dat","w");
  for(i=-MAX_LATTICE;i<=MAX_LATTICE;i++)
  {
    if(Distribution[i+MAX_LATTICE]>0.5)
    {
      P=0.5*log(2.0/(NumberOfJumps*M_PI))-(SQR(i)/(2.0*NumberOfJumps));
      fprintf(FilePtr,"%d %f %f\n",i,Distribution[i+MAX_LATTICE]/Normalization,exp(P));
    }
  }
  fclose(FilePtr);

  return 0;
}
