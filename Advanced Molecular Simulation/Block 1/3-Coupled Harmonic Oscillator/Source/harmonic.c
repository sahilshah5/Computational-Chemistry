#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ran_uniform.h"

#define MAX_NUMBER_OF_OSCILLATORS 10000
#define MAX_ENERGY 100000
#define CycleMultiplication 1000

#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))

enum{NVE=1,NVT=2};
enum{UP=1,DOWN=-1};

// sets of harmonic oscillators; E or T is constant
int main(void)
{
  int NumberOfOscillators;
  int NumberOfCycles,Ninit,TotalEnergy;
  int Utot,A,B,OscA,OscB;
  int i,j,k,Choice;
  int Oscillator[MAX_NUMBER_OF_OSCILLATORS];
  double Beta,Normalization,Sum,Count;
  double Distribution[MAX_ENERGY];
  FILE *FilePtr;

  // initialize the random number generator with the system time
  InitializeRandomNumberGenerator(time(0l));
 
  // read the input parameters
  TotalEnergy=0;
  Beta=0.0;

  printf("Number of Oscillators? ");
  fscanf(stdin,"%d",&NumberOfOscillators);

  printf("Number of Cycles (x %d)? ",CycleMultiplication);
  fscanf(stdin,"%d",&NumberOfCycles);

  if(NumberOfCycles<=3)
  {
    printf("Input parameter error, should be\n");
    printf("\tNumber of Cycles > 3\n");
    exit(0);

  }

  printf("1. NVE Ensemble\n");
  printf("2. NVT Ensemble\n");
  fscanf(stdin,"%d",&Choice);

  switch(Choice)
  {
    case NVE:
      printf("Total Energy           ? \n");
      fscanf(stdin,"%d",&TotalEnergy);
      if(NumberOfOscillators<=3||NumberOfOscillators>MAX_NUMBER_OF_OSCILLATORS||
         TotalEnergy<=3||TotalEnergy>MAX_ENERGY)
      {
        printf("Input parameter error, should be\n");
        printf("\tNumberOfOscillators (4-%d)\n",MAX_NUMBER_OF_OSCILLATORS);
        printf("\tTotalEnergy (4-%d)\n",MAX_ENERGY);
        exit(0);
      }
      break;
    case NVT:
      printf("Beta            ? \n");
      fscanf(stdin,"%lf",&Beta);
      if(Beta<=0.0||Beta>1e7)
      {
        printf("Input parameter error, should be\n");
        printf("\tBeta (0 - 1e7)\n");
        exit(0);
      }
      NumberOfOscillators=1;
      break;
  }

  // initialize
  for(i=0;i<MAX_NUMBER_OF_OSCILLATORS;i++)
    Oscillator[i]=0;

  for(i=0;i<MAX_ENERGY;i++)
    Distribution[i]=0.0;

  Ninit=NumberOfCycles/2;
  Sum=0.0;
  Count=0.0;
  Normalization=0.0;
  Utot=0;

  if(Choice==NVE)
  {
    i=0;
    do
    {
      if(i>=NumberOfOscillators) 
        i=0;
      Oscillator[i]++;
      i++;
      Utot++;
    }
    while(Utot!=TotalEnergy);
  }
      
  Utot=0;
  for(i=0;i<NumberOfOscillators;i++)
    Utot+=Oscillator[i];

  printf("\n");
  printf("Initial energy : %d\n",Utot);


  // loop over all cycles

  for(i=0;i<NumberOfCycles;i++)
  {
    for(j=0;j<CycleMultiplication;j++)
    {
      for(k=0;k<NumberOfOscillators;k++)
      {
        switch(Choice)
        {
          // NVE ensemble
          // choose 2 different levels at random
          // exchange energy at random
          case NVE:
            do
            {
              OscA=NumberOfOscillators*RandomNumber();
              OscB=NumberOfOscillators*RandomNumber();
            } while(OscA==OscB);

            if(RandomNumber()<0.5)
              A=UP,B=DOWN;
            else
              A=DOWN,B=UP;

            if(MIN(Oscillator[OscA]+A,Oscillator[OscB]+B)>=0)
            {
              Oscillator[OscA]+=A;
              Oscillator[OscB]+=B;
            }

            break;

            // NVT ensemble
          case NVT:
            if(RandomNumber()<0.5)
              A=UP;
            else
              A=DOWN;

            if((Oscillator[0]+A>=0)&&RandomNumber()<exp(-Beta*A))
              Oscillator[0]+=A;
            break;
        }
      }
      // sample
      if(i>Ninit)
      {
        Distribution[Oscillator[0]]+=1.0;
        Normalization+=1.0;
        Sum+=Oscillator[0];
        Count+=1.0;
      }
    }
  }

  // write results
  Utot=0;
  for(i=0;i<NumberOfOscillators;i++)
    Utot+=Oscillator[i];

  printf("\n");
  printf("Final Energy                        : %d\n",Utot);
  printf("Average Energy First Oscillator 1   : %f\n",Sum/Count);

  FilePtr=fopen("results.dat","w");
  for(i=0;i<MAX_ENERGY;i++)
  {
    if(Distribution[i]>0.5)
      fprintf(FilePtr,"%d %f\n",i,Distribution[i]/Normalization);
  }
  fclose(FilePtr);

  return 0;
}
