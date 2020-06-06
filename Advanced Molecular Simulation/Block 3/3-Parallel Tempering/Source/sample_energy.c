#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "system.h"

#define MAXIMUM_ENERGY 10.0
#define MAXIMUM_NUMBER_OF_BINS 1000

// sample the total energy distribution
void SampleEnergy(int Switch)
{
  int i,j;
  static double Gg1[MAXIMUM_NUMBER_OF_BINS][MAXNUMBEROFSYSTEMS],Gg2,Deltax;
  FILE *FilePtr;

  switch(Switch)
  {
    case INITIALIZE:
      for(i=0;i<MAXIMUM_NUMBER_OF_BINS;i++)
        for(j=0;j<MAXNUMBEROFSYSTEMS;j++)
          Gg1[i][j]=0.0;
      Gg2=0.0;
      Deltax=MAXIMUM_NUMBER_OF_BINS/MAXIMUM_ENERGY;
      break;
    case SAMPLE:
      Gg2+=1.0;
      for(j=0;j<NumberOfSystems;j++)
      {
        i=(int)(Deltax*Uold[j]);
        if(i<MAXIMUM_NUMBER_OF_BINS) Gg1[i][j]+=1.0;
      }
      break;
    case WRITE_RESULTS:
      FilePtr=fopen("energy_distribution.dat","w");
      for(j=0;j<NumberOfSystems;j++)
      {
        for(i=0;i<MAXIMUM_NUMBER_OF_BINS;i++)
          if(Gg1[i][j]>0.5) 
            fprintf(FilePtr,"%f %f\n",i/Deltax,Gg1[i][j]/Gg2);
        fprintf(FilePtr,"\n");
      }
      fclose(FilePtr);
      break;
  }
}
