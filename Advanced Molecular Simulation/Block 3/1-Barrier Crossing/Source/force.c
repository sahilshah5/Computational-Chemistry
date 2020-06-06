#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "system.h"
 
// calculate the forces and potential energy
void Force(double x,double *U,double *F)
{
  if(x<=0.0)
  {
     *U=2.0*SQR(M_PI)*SQR(x);
     *F=-4.0*SQR(M_PI)*x;
  }
  else if(x>=1.0)
  {
    *U=2.0*SQR(M_PI)*SQR(x-1.0);
    *F=-4.0*SQR(M_PI)*(x-1.0);
  }
  else
  {
    *U=1.0-cos(2.0*M_PI*x);
    *F=-2.0*M_PI*sin(2.0*M_PI*x);
  }
}
