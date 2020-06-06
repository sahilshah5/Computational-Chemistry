#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ran_uniform.h"

#define MAX_COMPARTMENTS 1000
#define MAX_PARTICLES 1000
#define CycleMultiplication 1000

// returns the estimate of fac(n) for large nummers
double LnFactorial(int n)
{
  int j;
  double f;

  f=0.0;
  for(j=2;j<=n;j++)
    f+=log(j);
  return f;
}

// divide N particles among p compartments
int main(void)
{
  int i,j,k,index;
  int P,N,Ncycle;
  double Distribution[MAX_COMPARTMENTS][MAX_PARTICLES];
  int NumInComp[MAX_COMPARTMENTS];
  FILE *FilePtr;

  // initialize the random number generator with the system time
  InitializeRandomNumberGenerator(time(0l));

  // read the input parameters
  printf("Number of Particles N? ");
  fscanf(stdin,"%d",&N);
 
  printf("Number of Compartments p? ");
  fscanf(stdin,"%d",&P);

  printf("Number of Cycles (x %d)? ",CycleMultiplication);
  fscanf(stdin,"%d",&Ncycle);

  if(P<2||P>MAX_COMPARTMENTS||N<2||N>MAX_PARTICLES)
  {
    printf("Error in input parameters\n");
    exit(1);
  }

  // initialize 
  for(i=0;i<N;i++)
  {
    for(j=0;j<P;j++)
    {
      Distribution[j][i]=0.0;
      NumInComp[j]=0;
    }
  }

  // loop over all cycles
  for(i=0;i<Ncycle;i++)
    for(k=0;k<CycleMultiplication;k++)
    {

  // Distribute particles over the compartments:
  // Loop over all particles, pick a random compartment, and add 
  // a particle to it.
  // A random number in the interval [0,1> can be generated using 
  // RandomNumber().

  // NumInComp[index] = number of particles in compartment 'index'.

      // start modification
  for (i=0; i<N; i++){
      index=(int)(P*RandomNumber());
      NumInComp[index]++;
  }
      // end modification

      // make a histogram
      for(j=0;j<P;j++)
      {
        Distribution[j][NumInComp[j]]+=1.0;
        NumInComp[j]=0;
      }
    }

  // Write Results
  FilePtr=fopen("results.dat","w");
  for(j=0;j<P;j++)
  {
    for(i=0;i<N;i++)
    {

      if(Distribution[j][i]>0.0)
        Distribution[j][i]/=(double)Ncycle*CycleMultiplication;

      fprintf(FilePtr,"%d %f\n",i,Distribution[j][i]);
    }
    fprintf(FilePtr,"\n");
  }
  fclose(FilePtr);

  // write analytical distribution
    FilePtr=fopen("analytical.dat","w");

    for(j=0;j<N;j++)
      fprintf(FilePtr,"%d %f\n",j,exp(LnFactorial(N)-LnFactorial(j)-
                       LnFactorial(N-j)-j*log(P)-(N-j)*log(P/((double)(P-1)))));

    fclose(FilePtr);
  return 0;
}
