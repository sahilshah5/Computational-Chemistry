#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "system.h"
#include "ran_uniform.h"

#define CycleMultiplication 1000

 // Npt Simulation Of Hard-Spheres 
int main(void)
{
  int i,j,k,m,n,Ipart;
  int NumberOfCycles,NumberOfInitializationCycles;
  int NumberOfDisplTrials,NumberOfDisplAccepted;
  int NumberOfVolumeTrials,NumberOfVolumeAccepted;
  int overlap;
  double MaximumDisplacement,MaximumVolumeChange;
  double Pressure,Beta,Vnew,Vold,Boxold;
  double VolumeSum,VolumeCount;
  double Box,r2;
  FILE *FilePtr;
  VECTOR dr,pos;
      
  // initialize the random number generator with the system time
  InitializeRandomNumberGenerator(time(0l));

  // read parameters
  FilePtr=fopen("input","r");
  fscanf(FilePtr,"%d %d %d %lf %lf %lf %lf\n",
      &NumberOfCycles,&NumberOfInitializationCycles,&NumberOfParticles,
      &MaximumDisplacement,&MaximumVolumeChange,
      &Beta,&Pressure);
  fclose(FilePtr);

  printf("Number of cycles (x %d) : %d\n",CycleMultiplication,NumberOfCycles);
  printf("Number of init cycles    : %d\n",NumberOfInitializationCycles);
  printf("NumberOfParticles        : %d\n",NumberOfParticles);
  printf("Maximum displacement     : %f\n",MaximumDisplacement);
  printf("Maximum volume change    : %f\n",MaximumVolumeChange);
  printf("Beta                     : %f\n",Beta);
  printf("Pressure                 : %f\n",Pressure);

  printf("\n");
  printf("\n");

  if(NumberOfParticles>MAX_NUMBER_OF_PARTICLES||NumberOfParticles<100)
  {
    printf("Error in the number of particles\n");
    exit(-1);
  }

  // initialize stuff
  NumberOfDisplTrials=0;
  NumberOfDisplAccepted=0;
  NumberOfVolumeTrials=0;
  NumberOfVolumeAccepted=0;
  VolumeSum=0.0;
  VolumeCount=0.0;
  Box=10.0;

  FilePtr=fopen("movie.pdb","w");

  // put particles on a lattice
  n=0;
  for(i=0;i<9;i++)
    for(j=0;j<9;j++)
      for(k=0;k<9;k++)
      {
        Positions[n].x=i*1.1;
        Positions[n].y=j*1.1;
        Positions[n].z=k*1.1;
        n++;
      }

  // start of the simulation
  WritePdb(FilePtr);

  for(m=0;m<NumberOfCycles;m++)
  {
    for(n=0;n<CycleMultiplication;n++)
    {
      // Select Particle And Move At Random 
      Ipart=RandomNumber()*(NumberOfParticles+1);

      if(Ipart>=NumberOfParticles)
      {
        // volume change
        // random walk in Ln(V)
        NumberOfVolumeTrials++;
        Vold=CUBE(Box);
        Boxold=Box;
        Vnew=exp(log(Vold)+(RandomNumber()-0.5)*MaximumVolumeChange);

        Box=pow(Vnew,1.0/3.0);

        // transform coordinates
        for(i=0;i<NumberOfParticles;i++)
        {
          OldPositions[i].x=Positions[i].x;
          OldPositions[i].y=Positions[i].y;
          OldPositions[i].z=Positions[i].z;

          Positions[i].x*=(Box/Boxold);
          Positions[i].y*=(Box/Boxold);
          Positions[i].z*=(Box/Boxold);
        }

        // check for overlaps
        overlap=FALSE;
        for(i=0;i<NumberOfParticles-1;i++)
        {
          for(j=i+1;j<NumberOfParticles;j++)
          {
            dr.x=Positions[i].x-Positions[j].x;
            dr.y=Positions[i].y-Positions[j].y;
            dr.z=Positions[i].z-Positions[j].z;

            // apply boundary conditions
            dr.x-=Box*rint(dr.x/Box);
            dr.y-=Box*rint(dr.y/Box);
            dr.z-=Box*rint(dr.z/Box);

            // Nearest Image
            r2=SQR(dr.x)+SQR(dr.y)+SQR(dr.z); 
            if(r2<1.0)
              overlap=TRUE;
          }
        }

        // no overlap... use acceptance rule
        if((!overlap)&&(RandomNumber()<exp(-Beta*Pressure*(Vnew-Vold)+log(Vnew/Vold)*(NumberOfParticles+1.0))))
        {
          NumberOfVolumeAccepted++;
        }
        else
        {
          for(i=0;i<NumberOfParticles;i++)
          {
            Positions[i].x=OldPositions[i].x;
            Positions[i].y=OldPositions[i].y;
            Positions[i].z=OldPositions[i].z;
          }
          Box=Boxold;
        }
      }
      else
      {
        // displacement
        NumberOfDisplTrials++;
        pos.x=Positions[Ipart].x+(RandomNumber()-0.5)*MaximumDisplacement;
        pos.y=Positions[Ipart].y+(RandomNumber()-0.5)*MaximumDisplacement;
        pos.z=Positions[Ipart].z+(RandomNumber()-0.5)*MaximumDisplacement;

        // put particle back in the box
        pos.x-=Box*rint(pos.x/Box);
        pos.y-=Box*rint(pos.y/Box);
        pos.z-=Box*rint(pos.z/Box);

        // see if there is an overlap with any other particle
        overlap=FALSE;
        for(i=0;i<NumberOfParticles;i++)
        {
          if(i!=Ipart)
          {
            dr.x=pos.x-Positions[i].x;
            dr.y=pos.y-Positions[i].y;
            dr.z=pos.z-Positions[i].z;

            // apply boundary conditions
            dr.x-=Box*rint(dr.x/Box);
            dr.y-=Box*rint(dr.y/Box);
            dr.z-=Box*rint(dr.z/Box);

            r2=SQR(dr.x)+SQR(dr.y)+SQR(dr.z);

            if(r2<1.0)
              overlap=TRUE;
          }
        }

        // no overlap, accept
        if(!overlap)
        {
          NumberOfDisplAccepted++;
          Positions[Ipart].x=pos.x;
          Positions[Ipart].y=pos.y;
          Positions[Ipart].z=pos.z;
        }
      }
    }

    // make a movie file
    if((m%10)==0)
    {
      WritePdb(FilePtr);
      printf("Volume : %f   Box : %f\n",CUBE(Box),Box);
    }

    // sample volume
    if(m>NumberOfInitializationCycles)
    {
      VolumeCount+=1.0;
      VolumeSum+=CUBE(Box);
    }
  }

  fclose(FilePtr);

  // print results
  printf("\n");
  printf("\n");
  printf("Fraction Succes (Displ.) : %f\n",(double)NumberOfDisplAccepted/NumberOfDisplTrials);
  printf("Fraction Succes (Volume) : %f\n",(double)NumberOfVolumeAccepted/NumberOfVolumeTrials);
  printf("Average Volume           : %f\n",VolumeSum/VolumeCount);
  printf("Average Density          : %f\n",NumberOfParticles/(VolumeSum/VolumeCount));

  return 0;
}
