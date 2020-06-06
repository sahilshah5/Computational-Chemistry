#include <math.h>
#include "system.h"

// calculate the total energy of a configuration
void Energy(double *Uconf,int k)
{
  int i,j;
  double r2;

  *Uconf=0.0;

  for(i=0;i<NUMBEROFPARTICLES;i++)
  {
    for(j=i+1;j<NUMBEROFPARTICLES;j++)
    {
      r2=SQR(Positions[i][k].x-Positions[j][k].x)+
         SQR(Positions[i][k].y-Positions[j][k].y);
      if(r2<1.0) 
        (*Uconf)+=SQR(sqrt(r2)-1.0);
    }
  }
}
