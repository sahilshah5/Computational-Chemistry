#include <stdio.h> 
#include <stdlib.h> 
#include <math.h> 
#include "system.h"
#include "ran_uniform.h"
 
// Read In System Information
void ReadData(void)
{ 
  double U;
  FILE *FilePtr;

  FilePtr=fopen("input","r");
  fscanf(FilePtr,"%d %lf %lf %d\n",&NumberOfSteps,&Temperature,&Tstep,&NumberOfInitializationSteps);
  fscanf(FilePtr,"%d %lf %lf %d\n",&Choice,&Nu,&Freqnos,&NumberOfNoseHooverChains);

  Position=0.0;
  Velocity=RandomVelocity(Temperature);
  OldF=0.0;
  U=0.0;

  Force(Position,&U,&OldF);

  printf("Md Of A Particle Over A Energy Barrier\n");
  printf("\n");
  printf("Number of Steps (x 1000): %d\n",NumberOfSteps);
  printf("Number of Init Steps  : %d\n",NumberOfInitializationSteps);
  printf("Time step             : %f\n",Tstep);
  printf("Temperature           : %f\n",Temperature);

  switch(Choice)
  {
    case 2:
      printf("Collision Frequency   : %f\n\n",Nu);
      break;
    case 3:
      Velocity=sqrt(Temperature);
      InitializeNoseHoover();
      printf("Number of Nose-Hoover Chains   : %d\n\n",NumberOfNoseHooverChains);
      printf("Frequency Omega               : %f\n\n",Freqnos);
      break;
  } 
}
