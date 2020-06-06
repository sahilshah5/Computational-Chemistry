#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "system.h" 

#define CycleMultiplication 1000
#define PrintFrequency 200

void MdLoop(void)
{
  int i,j;
  double Cstart,Cc1,Cc2,Tt1,Tt2;
  FILE *FilePtr;

  Cc1=0.0;
  Cc2=0.0;
  Cstart=0.0;
  Tt1=0.0;
  Tt2=0.0;
 
  SampleDiff(1);
  SampleProf(1);

  FilePtr=fopen("pos_vel.dat","w");

  printf("\n");

  for(i=0;i<NumberOfSteps;i++)
  {
    for(j=0;j<CycleMultiplication;j++)
    {
      switch(Choice)
	{
	case 1:
	  IntegrateNVE();
          break;
	case 2:
          IntegrateAndersen();
          break;
	case 3:
          IntegrateNoseHoover();
          break;
	case 4:
          IntegrateMonteCarlo();
          break;
	case 5:
	  IntegrateBerendsen();
	  break;
      }


      if(i>NumberOfInitializationSteps)
      {
        SampleDiff(2);
        SampleProf(2);

        Tt1+=SQR(Velocity);
        Tt2+=1.0;

        if((j%PrintFrequency)==0) 
           fprintf(FilePtr,"%f %f\n",Position,Velocity);
      }

      if(i==NumberOfInitializationSteps) 
        Cstart=ConservedEnergy;

      else if(i>NumberOfInitializationSteps)
	{
	  if(Choice!=2&&Choice!=4) {
	    Cc1+=fabs((ConservedEnergy-Cstart)/Cstart);
	    Cc2+=1.0;
	  }
	}
    }
  }

  SampleDiff(3);
  SampleProf(3);

  fclose(FilePtr);

  printf("\n");
  printf("Energy Drift          : %f\n",Cc1/Cc2); 
  printf("Average Temp.         : %f\n",Tt1/Tt2);
}
