#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "system.h"

int M;
int nc;
double w[5];
double QMass[5];
double xlogs[5];
double vlogs[5];
double glogs[5];
double UNoseHoover;

void InitializeNoseHoover(void)
{
  M=NumberOfNoseHooverChains;
  nc=1;

  w[0]=1.0/(4.0-pow(4.0,1.0/3.0));
  w[1]=w[0];
  w[2]=1.0-4.0*w[0];
  w[3]=w[0];
  w[4]=w[0];

  QMass[0]=Temperature/SQR(Freqnos);
  QMass[1]=Temperature/SQR(Freqnos);
  QMass[2]=Temperature/SQR(Freqnos);
  QMass[3]=Temperature/SQR(Freqnos);
  QMass[4]=Temperature/SQR(Freqnos);

  xlogs[0]=0.0;
  xlogs[1]=0.0;
  xlogs[2]=0.0;
  xlogs[3]=0.0;
  xlogs[4]=0.0;

  vlogs[0]=1.0/QMass[0];
  vlogs[1]=1.0/QMass[1];
  vlogs[2]=1.0/QMass[2];
  vlogs[3]=1.0/QMass[3];
  vlogs[4]=1.0/QMass[4];

  glogs[0]=0.0;
  glogs[1]=(QMass[0]*SQR(vlogs[0])-Temperature)/QMass[1];
  glogs[2]=(QMass[1]*SQR(vlogs[1])-Temperature)/QMass[2];
  glogs[3]=(QMass[2]*SQR(vlogs[2])-Temperature)/QMass[3];
  glogs[4]=(QMass[3]*SQR(vlogs[3])-Temperature)/QMass[4];

}

// NoseHoover thermostat (NVT-Ensemble) by Glenn J. Martyna
// Ref: Molecular Physics 1996, Vol. 87, No. 5, 1117-1157
void NoseHooverNVT(double dt)
{
  int i,j,k;
  double AA,scale,UKinetic_pressure;

  scale=1.0;
  UKinetic_pressure=SQR(Velocity);
  glogs[0]=(UKinetic_pressure-Temperature)/QMass[0];
  for(i=0;i<nc;i++)
    for(j=1;j<=5;j++)
    {
      vlogs[M-1]+=glogs[M-1]*w[j-1]*dt/(4.0*nc);
      for(k=0;k<M-1;k++)
      {
        AA=exp(-(w[j-1]*dt/(8.0*nc))*vlogs[M-k-1]);
        vlogs[M-k-2]=vlogs[M-k-2]*SQR(AA)+glogs[M-k-2]*AA*w[j-1]*dt/(4.0*nc);
      }
      AA=exp(-(w[j-1]*dt/(2.0*nc))*vlogs[0]);
      scale=scale*AA;
      glogs[0]=(SQR(scale)*UKinetic_pressure-Temperature)/QMass[0];
      for(k=1;k<=M;k++)
        xlogs[k-1]+=vlogs[k-1]*w[j-1]*dt/(2.0*nc);
      for(k=0;k<M-1;k++)
      {
        AA=exp(-(w[j-1]*dt/(8.0*nc))*vlogs[k+1]);
        vlogs[k]=vlogs[k]*SQR(AA)+glogs[k]*AA*(w[j-1]*dt/(4.0*nc));
        glogs[k+1]=(QMass[k]*SQR(vlogs[k])-Temperature)/QMass[k+1];
      }
      vlogs[M-1]+=glogs[M-1]*w[j-1]*dt/(4.0*nc);
    }

  // scale velocity
  Velocity*=scale;

  // compute thermostat energy
  UNoseHoover=0.0;
  for(i=0;i<M;i++)
    UNoseHoover+=0.5*SQR(vlogs[i])*QMass[i]+Temperature*xlogs[i];

}


// integrate the equations of motion using an explicit Nose-Hoover chain
// see the work of Martyna/Tuckerman et al.
void IntegrateNoseHoover(void)
{
  double U,F;

  // thermostat 0.5xTstep
  NoseHooverNVT(Tstep);

  // normal integration
  Velocity+=0.5*Tstep*OldF;
  Position+=Tstep*Velocity;
  Force(Position,&U,&F);
  OldF=F;
  Velocity+=0.5*Tstep*OldF;

  // thermostat 0.5xTstep
  NoseHooverNVT(Tstep);

  ConservedEnergy=U+0.5*SQR(Velocity)+UNoseHoover;
}
