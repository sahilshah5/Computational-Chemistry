#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "system.h"

// MAXT  = maximum timesteps for the correlation time
// MAXT0 = maximum number of time origins 
// FREQT0 = frequency with which a new time origin is selected

// time = Current time in units of Deltat
// t0time[TMAXT0] = Time of the stored time origin in units of Deltat
// t0Counter = Number of time origins stored so far
// t0index = The index of the current t0 in the array of stored time origins.
// CorrelTime = Time difference between the current time and the time origin
// SampleCounter[MAXT] = Number of samples taken at a given CorrelTime.

#define MAXT 7500
#define MAXT0 250
#define FREQT0 50

void SampleDiff(int Switch)
{
  int t0index,i,j,CorrelTime;
  double CumIntegration;
  static int time,t0time[MAXT0],t0Counter,SampleCounter[MAXT];
  static double Vacf[MAXT],R2[MAXT];
  static double Vxt0[MAXPART][MAXT0],Vyt0[MAXPART][MAXT0],Vzt0[MAXPART][MAXT0];
  static double Rx0[MAXPART][MAXT0],Ry0[MAXPART][MAXT0],Rz0[MAXPART][MAXT0];
  FILE *FilePtrMsd,*FilePtrVacf;

  switch(Switch)
  {
    // initialize everything
    case INITIALIZE:
      time=0;
      t0Counter=0;
      for(i=0;i<MAXT;i++)
      {
        R2[i]=0.0;
        SampleCounter[i]=0;
        Vacf[i]=0.0;
      }
      break;
    case SAMPLE:
      time++;
      if((time%FREQT0)==0)
      {
        // new time origin
        // store the positions/velocities; the current velocities
        // are Velocities[i] and the current positions are PositionsNONPDB[i].
        // question: why do you have to be careful with Pbc ?
        // make sure to study algorithm 8 (page 91) of Frenkel/Smit
        // before you start to make modifications
        // note that most variable names are different here then in
        // Frenkel/Smit. in this Way, you will have to think more... 

        // start modification
        t0index=t0Counter%MAXT0;
        t0Counter++;
        t0time[t0index]=time;
        for(i=0;i<NumberOfParticles;i++)
        {
           Vxt0[i][t0index]=Velocities[i].x;
           Vyt0[i][t0index]=Velocities[i].y;
           Vzt0[i][t0index]=Velocities[i].z;

           Rx0[i][t0index]=PositionsNONPDB[i].x;
           Ry0[i][t0index]=PositionsNONPDB[i].y;
           Rz0[i][t0index]=PositionsNONPDB[i].z;
        } 

        // end modification
      }

      // loop over all time origins that have been stored

      // start modification
      for(j=0;j<MIN(t0Counter,MAXT0);j++)
      {
          CorrelTime=time-t0time[j];
          if(CorrelTime<MAXT)
          {
              SampleCounter[CorrelTime]++;
              for(i=0;i<NumberOfParticles;i++)
              {
                  Vacf[CorrelTime]+=Velocities[i].x*Vxt0[i][j]+Velocities[i].y*Vyt0[i][j]+Velocities[i].z*Vzt0[i][j];
                  R2[CorrelTime]+=SQR(PositionsNONPDB[i].x-Rx0[i][j])+SQR(PositionsNONPDB[i].y-Ry0[i][j])+SQR(PositionsNONPDB[i].z-Rz0[i][j]);
              }
          }
      }
      // end modification
      break;
    case WRITE_RESULTS:
      // write everything to disk
      CumIntegration=0.0;
      FilePtrMsd=fopen("msd.dat","w");
      FilePtrVacf=fopen("vacf.dat","w");
      for(i=0;i<MAXT-1;i++)
      {
        if(SampleCounter[i]>0)
        {
          Vacf[i]/=(double)(NumberOfParticles*SampleCounter[i]);
          R2[i]/=(double)(NumberOfParticles*SampleCounter[i]);
        }
        else
        {
          Vacf[i]=0.0;
          R2[i]=0.0;
        }
        CumIntegration+=Deltat*Vacf[i]/3.0;
        fprintf(FilePtrVacf,"%lf %lf %lf\n",(i+1)*Deltat,Vacf[i],CumIntegration);

        if(i>=1)
          fprintf(FilePtrMsd,"%lf %lf %lf\n",(i+1)*Deltat,R2[i],R2[i]/(6.0*(i+1)*Deltat));
        else
          fprintf(FilePtrMsd,"%lf %lf %lf\n",(i+1)*Deltat,R2[i],0.0);
      }
      fclose(FilePtrVacf);
      fclose(FilePtrMsd);
  }
}
