#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "system.h"
#include "ran_uniform.h"

#define CycleMultiplication 1000

// parallel tempering for a simple system
int main(void)
{ 
  FILE *FilePtr;
  int i,j,system=0,SystemA=0,SystemB=0,NumberOfSteps,particle,k;
  double Rho,Del,U,Rxold,Ryold,swap=0.0;
  double DispSum[MAXNUMBEROFSYSTEMS],DispCount[MAXNUMBEROFSYSTEMS];
  double SwapSum[MAXNUMBEROFSYSTEMS],SwapCount[MAXNUMBEROFSYSTEMS];
  double EnergySum[MAXNUMBEROFSYSTEMS],EnergyCount[MAXNUMBEROFSYSTEMS];
  double XcoordSum[MAXNUMBEROFSYSTEMS],XcoordCount[MAXNUMBEROFSYSTEMS];
  double Ppt,Temp[MAXNUMBEROFSYSTEMS],Beta[MAXNUMBEROFSYSTEMS];
  int ParticlesAdded;

  // initialize the random number generator with the system time
  InitializeRandomNumberGenerator(time(0l));

  ParticlesAdded=0;

  FilePtr=fopen("input","r");
  fscanf(FilePtr,"%*s %*s %*s %*s\n");
  fscanf(FilePtr,"%lf %lf %d %d\n",&Rho,&Ppt,&NumberOfSteps,&NumberOfSystems);

  if((NumberOfSystems<1)||(NumberOfSystems>MAXNUMBEROFSYSTEMS))
    exit(-1);

  fscanf(FilePtr,"%*s\n");
  for(i=0;i<NumberOfSystems;i++)
  {
    fscanf(FilePtr,"%lf ",&Temp[i]);
    Beta[i]=1.0/Temp[i];
  }

  Box=sqrt(NUMBEROFPARTICLES/Rho);
  Del=Box/(sqrt(NUMBEROFPARTICLES)+1.0);

  // generate a lattice of N particles
  for(i=0;i<(sqrt(NUMBEROFPARTICLES)+1);i++)
    for(j=0;j<(sqrt(NUMBEROFPARTICLES)+1);j++)
    {
      ParticlesAdded++;
      if(ParticlesAdded<NUMBEROFPARTICLES)
      {
        for(system=0;system<NumberOfSystems;system++)
        {
          Positions[i][system].x=(i+1)*Del;
          Positions[j][system].y=(j+1)*Del;
        }
      }
    }

  for(i=0;i<MAXNUMBEROFSYSTEMS;i++)
  {
    Uold[i]=0.0;
    DispSum[i]=0.0;
    DispCount[i]=0.0;
    SwapSum[i]=0.0;
    SwapCount[i]=0.0;
    EnergySum[i]=0.0;
    EnergyCount[i]=0.0;
    XcoordSum[i]=0.0;
    XcoordCount[i]=0.0;
  }

  // calculate the initial energy
  for(i=0;i<NumberOfSystems;i++)
  {
    Energy(&U,i);
    Uold[i]=U;
  }

  SampleEnergy(INITIALIZE);
  SampleXcoord(INITIALIZE);

  printf("Density            : %lf\n",Rho);
  printf("Boxsize            : %lf\n",Box);
  printf("Fraction Exchanges : %lf\n",Ppt);
  printf("Number Of Steps    : %d\n",NumberOfSteps);
  printf("Number Of Temp.    : %d\n",NumberOfSystems);

  for(i=0;i<NumberOfSystems;i++)
    printf("Temp. (Energy)     : %f (%f)\n",Temp[i],Uold[i]);

  // loop over all cycles 

  for(i=0;i<NumberOfSteps;i++)
  {
    if(i%1000==0) printf("i: %d\n",i);
    for(k=0;k<CycleMultiplication*NumberOfSystems;k++)
    {
      // Sample Energy And X-Coordinate
      if(i>10)
      {
        SampleEnergy(SAMPLE);
        SampleXcoord(SAMPLE);

        for(system=0;system<NumberOfSystems;system++)
        {
          EnergySum[system]+=Uold[system];
          EnergyCount[system]+=1.0;
          XcoordSum[system]+=Positions[0][system].x/Box;
          XcoordCount[system]+=1.0;
        }
      }

      // select a trial move at random

      if((RandomNumber()<Ppt)&&(NumberOfSystems>1))
      {

        // exchange trial move
        // select temperatures at random 

        // start modification

        // end modification
      }
      else
      {
        // Particle Displacement
        // select system/particle at random
        system=(int)(NumberOfSystems*RandomNumber());
        particle=(int)(NUMBEROFPARTICLES*RandomNumber());

        Rxold=Positions[particle][system].x;
        Ryold=Positions[particle][system].y;
        DispCount[system]+=1.0;

        Positions[particle][system].x+=(RandomNumber()-0.5)*0.05;
        Positions[particle][system].y+=(RandomNumber()-0.5)*0.05;
               
        // check for acceptance
        // check boundaries first 

        if(MIN(Positions[particle][system].x,Positions[particle][system].y)>0.0&&
           MAX(Positions[particle][system].x,Positions[particle][system].y)<Box)
        {
          Energy(&U,system);
          if(RandomNumber()<exp(-Beta[system]*(U-Uold[system])))
          {
            Uold[system]=U;
            DispSum[system]+=1.0;
          }
          else
          {
            Positions[particle][system].x=Rxold;
            Positions[particle][system].y=Ryold;
          }
        }
        else
        {
          Positions[particle][system].x=Rxold;
          Positions[particle][system].y=Ryold;
        }
      }
    }
  }

  // write info to the screen
  SampleEnergy(WRITE_RESULTS);
  SampleXcoord(WRITE_RESULTS);

  printf("\n");
  printf("\n");
  printf("\n");
  printf("Average Energy\n");

  for(i=0;i<NumberOfSystems;i++)
    printf("Temp : %d %f\n",i,EnergySum[i]/EnergyCount[i]);

  printf("\n");
  printf("Average X Coordinate\n");

  for(i=0;i<NumberOfSystems;i++)
    printf("Temp : %d %f\n",i,XcoordSum[i]/XcoordCount[i]);

  printf("\n");
  printf("Fraction Accepted Displacements:\n");

  for(i=0;i<NumberOfSystems;i++)
    printf("Temp : %d %f\n",i,DispSum[i]/DispCount[i]);

  printf("\n");
  printf("Fraction Accepted Exchanges:\n");

  for(i=0;i<NumberOfSystems-1;i++)
    if(SwapCount[i]>0.5) 
      printf("%d %d %f\n",i,i+1,SwapSum[i]/SwapCount[i]);

  return 0;
}
