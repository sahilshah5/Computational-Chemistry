#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// maximum number of energy levels
#define MAX_ENERGY_LEVELS 10000

// calculate the Boltzmann distribution
int main(void)
{
  int i,NumberOfEnergyLevels;
  double Distribution[MAX_ENERGY_LEVELS];
  double Normalize,Beta,Temperature,tmp;
  FILE *FilePtr;

  // read parameters
  printf("Number Of Energy Levels (2-10000) ? ");
  fscanf(stdin,"%d",&NumberOfEnergyLevels);

  printf("Temperature ? ");
  fscanf(stdin,"%lf",&Temperature);

  // check input
  if(NumberOfEnergyLevels<2||NumberOfEnergyLevels>MAX_ENERGY_LEVELS||
     Temperature<1e-7||Temperature>=1e7)
  {
    printf("Input parameter error, should be\n");
    printf("\tNumberOfEnergyLevels (2-%d)\n",MAX_ENERGY_LEVELS);
    printf("\tTemperature (1e-7 - 1e7)\n");
    exit(0);
  }

  Beta=1.0/Temperature;

  // loop over all levels
  Normalize=0.0;

  for(i=0;i<NumberOfEnergyLevels;i++)
  {
     // Original tmp=exp(-Beta*i);
     // Degeneracy tmp=(i+1)*exp(-Beta*i);
     tmp=(2i+1)*exp(-Beta*((pow(i,2)+i)/2));  //Linear Rotor

     // start modification
     
     // end modification

     Distribution[i]=tmp;
     Normalize+=tmp;
  }

  // Write Results 
  FilePtr=fopen("results.dat","w");
  for(i=0;i<NumberOfEnergyLevels;i++)
    fprintf(FilePtr,"%d %f\n",i,Distribution[i]/Normalize);
  fclose(FilePtr);

  return 0;
}
