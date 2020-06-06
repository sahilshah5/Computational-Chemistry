#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "sample.h"

// Sample Rms Displacement
// Tmax  = Maximum Timesteps For The Correlation Time
// T0max = Maximum Number Of Time Origins

#define T_MAX 10000
#define T0_MAX 100000
#define MIN(x,y) (((x)<(y))?(x):(y))
#define SQR(x) ((x)*(x))

void Sample(int Switch, int CurrentPosition)
{
  int Ttel,i,Dt;
  static int Origin[T0_MAX],Tvacf,Tt0[T0_MAX];
  static double Msd[T_MAX],Count[T_MAX];
  FILE *FilePtr;      

  switch(Switch)
  { 
    // initialize everything
    case INITIALIZE:
      for(i=0;i<T_MAX;i++)
      {
        Msd[i]=0.0;
        Count[i]=0.0;
      }
      break;
    // initialize new random walk
    case START_NEW_WALK:
      Tvacf=0;
      break;
    case SAMPLE_WALK:
      Tvacf++;
      Ttel=(Tvacf-1)%T0_MAX;
      Tt0[Ttel]=Tvacf;
      Origin[Ttel]=CurrentPosition;
         
      // loop over all time origins
      for(i=0;i<MIN(Tvacf,T0_MAX);i++)
      {
         Dt=Tvacf-Tt0[i];

        // only if the time difference is shorter
        // than the maximum correlation time
        if(Dt<T_MAX)
        {
           Count[Dt]+= 1.0;
           Msd[Dt]+=SQR(CurrentPosition-Origin[i]);
        }
      }
      break;
    case WRITE_RESULTS:
      // write everything to disk
      FilePtr=fopen("rms.dat","w");
      for(i=0;i<T_MAX;i++)
      {
        if(Count[i]>=0.5)
          fprintf(FilePtr,"%f %f\n",(double)i,Msd[i]/Count[i]);
      }
      fclose(FilePtr);
  }
}
