#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "system.h"

#define Tmax 1000
#define T0max 200

// Tmax  = Maximum Timesteps For The Correlation Time
// T0max = Maximum Number Of Time Origins 
void SampleDiff(int Switch)
{
  int Tcounter,i,T,Dt;
  static int Tvacf,Tt0[T0max],T0;
  double Dtime;
  static double R2t[Tmax],Rx0[T0max],Nvacf[Tmax];
  FILE *FilePtr;

  switch(Switch)
  {
    case 1:
      Tvacf=0;
      T0=0;

      for(i=1;i<=Tmax;i++)
      {
        R2t[i]=0.0;
        Nvacf[i]=0.0;
      }
      break;
    case 2:
      Tvacf++;
      if((Tvacf%50)==0)
      {
        T0++;
        Tcounter=(T0-1)%T0max;
        Tt0[Tcounter]=Tvacf;
        Rx0[Tcounter]=Position;
      }
      for(T=0;T<MIN(T0,T0max);T++)
      {
        Dt=Tvacf-Tt0[T];
        if(Dt<Tmax)
        {
          Nvacf[Dt]+=1.0;
          R2t[Dt]+=SQR(Position-Rx0[T]);
        }
      }
      break;       
    case 3:
      FilePtr=fopen("msd.dat","w");
      for(i=0;i<Tmax-1;i++)
      {
        Dtime=i*Tstep;
        if(Nvacf[i]>0.5)
          R2t[i]=R2t[i]/Nvacf[i];
        else
          R2t[i]=0.0;

        if(i!=0)
          fprintf(FilePtr,"%f %f %f\n",Dtime,R2t[i],R2t[i]/(2.0*Dtime));
        else
          fprintf(FilePtr,"%f %f 0.0\n",Dtime,R2t[i]);
      }
      fclose(FilePtr);
      break;
   }
}
