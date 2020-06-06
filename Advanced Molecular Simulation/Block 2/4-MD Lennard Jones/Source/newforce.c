#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "system.h"
 
// Calculate The Forces And Potential Energy
void NewForce(void)
{
  int i,j;
  double Ff,r2i,r6i;
  VECTOR dr;
 
  // set forces, potential energy and pressure to zero
  for(i=0;i<NumberOfParticles;i++)
  {
    NewForces[i].x=0.0;
    NewForces[i].y=0.0;
    NewForces[i].z=0.0;
  }
 
  UPotential=0.0;
  Pressure=0.0;
 
  // loop over all particle pairs
  for(i=0;i<NumberOfParticles-1;i++)
  {
    for(j=i+1;j<NumberOfParticles;j++)
    {
      dr.x=NewPositions[i].x-NewPositions[j].x;
      dr.y=NewPositions[i].y-NewPositions[j].y;
      dr.z=NewPositions[i].z-NewPositions[j].z;

      // apply boundary conditions 
      dr.x-=Box*rint(dr.x/Box);
      dr.y-=Box*rint(dr.y/Box);
      dr.z-=Box*rint(dr.z/Box);

      r2i=SQR(dr.x)+SQR(dr.y)+SQR(dr.z);

      // check if the distance is within the cutoff radius
      if(r2i<SQR(CutOff))
      {
        r2i=1.0/r2i;
        r6i=CUBE(r2i);
 
        UPotential+=4.0*r6i*(r6i-1.0)-Ecut;
        Ff=48.0*r6i*(r6i-0.5);
        Pressure+=Ff;
        Ff=Ff*r2i;
 
        NewForces[i].x+=Ff*dr.x;
        NewForces[i].y+=Ff*dr.y;
        NewForces[i].z+=Ff*dr.z;
 
        NewForces[j].x-=Ff*dr.x;
        NewForces[j].y-=Ff*dr.y;
        NewForces[j].z-=Ff*dr.z;
      }
    }
  }
 
  // scale the pressure
  Pressure/=3.0*CUBE(Box);
} 
