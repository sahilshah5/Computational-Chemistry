#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "system.h" 

 
// integrate the equations of motion and calculate the total impulse
void Integrate(double Step,VECTOR *Momentum)
{ 
  int i;
  double Scale;

  // set kinetic energy to zero 
  // verlet integrator
  /*UKinetic=0.0;
  for(i=0;i<NumberOfParticles;i++)
  {
    NewPositions[i].x=2.0*Positions[i].x-OldPositions[i].x+Forces[i].x*SQR(Deltat);
    NewPositions[i].y=2.0*Positions[i].y-OldPositions[i].y+Forces[i].y*SQR(Deltat);
    NewPositions[i].z=2.0*Positions[i].z-OldPositions[i].z+Forces[i].z*SQR(Deltat);
 
    Velocities[i].x=(NewPositions[i].x-OldPositions[i].x)/(2.0*Deltat);
    Velocities[i].y=(NewPositions[i].y-OldPositions[i].y)/(2.0*Deltat);
    Velocities[i].z=(NewPositions[i].z-OldPositions[i].z)/(2.0*Deltat);
 
    UKinetic+=0.5*(SQR(Velocities[i].x)+SQR(Velocities[i].y)+SQR(Velocities[i].z));
  }*/
  
  // Euler integrator
  /*UKinetic=0.0;
  for(i=0;i<NumberOfParticles;i++)
  {
    NewPositions[i].x=Positions[i].x+Velocities[i].x*Deltat;
    NewPositions[i].y=Positions[i].y+Velocities[i].y*Deltat;
    NewPositions[i].z=Positions[i].z+Velocities[i].z*Deltat;

    NewVelocities[i].x=Velocities[i].x+Forces[i].x*Deltat;
    NewVelocities[i].y=Velocities[i].y+Forces[i].y*Deltat;
    NewVelocities[i].z=Velocities[i].z+Forces[i].z*Deltat;

    Velocities[i].x=NewVelocities[i].x;
    Velocities[i].y=NewVelocities[i].y;
    Velocities[i].z=NewVelocities[i].z;

    UKinetic+=0.5*(SQR(Velocities[i].x)+SQR(Velocities[i].y)+SQR(Velocities[i].z));
  }*/

  // Velocity verlet integrator
  UKinetic=0.0;
  for(i=0;i<NumberOfParticles;i++)
  {
    NewPositions[i].x=Positions[i].x+Velocities[i].x*Deltat+(Forces[i].x*SQR(Deltat))/(2.0);
    NewPositions[i].y=Positions[i].y+Velocities[i].y*Deltat+(Forces[i].y*SQR(Deltat))/(2.0);
    NewPositions[i].z=Positions[i].z+Velocities[i].z*Deltat+(Forces[i].z*SQR(Deltat))/(2.0);
  }
  
  NewForce();

  for(i=0;i<NumberOfParticles;i++)
  {
    NewVelocities[i].x=Velocities[i].x+(((NewForces[i].x+Forces[i].x)*Deltat)/2.0);
    NewVelocities[i].y=Velocities[i].y+(((NewForces[i].y+Forces[i].y)*Deltat)/2.0);
    NewVelocities[i].z=Velocities[i].z+(((NewForces[i].z+Forces[i].z)*Deltat)/2.0);

    Velocities[i].x=NewVelocities[i].x;
    Velocities[i].y=NewVelocities[i].y;
    Velocities[i].z=NewVelocities[i].z;

    UKinetic+=0.5*(SQR(Velocities[i].x)+SQR(Velocities[i].y)+SQR(Velocities[i].z));
  }    

  // for NumberOfSteps < NumberOfInitializationSteps; use the velocity 
  // scaling to get the exact temperature 
  // otherwise: Scale=1.0 beware that the positions/velocities have to be
  // recalculated !!!!

  if(Step<NumberOfInitializationSteps)
    Scale=sqrt(Temperature*(3.0*NumberOfParticles-3.0)/(2.0*UKinetic));
  else
    Scale=1.0;
 
  UKinetic=0.0;
  (*Momentum).x=0.0;
  (*Momentum).y=0.0;
  (*Momentum).z=0.0;
 
  // scale velocities and put particles back in the box
  // beware: the old positions are also put back in the box 
  for(i=0;i<NumberOfParticles;i++)
  {
    Velocities[i].x*=Scale;
    Velocities[i].y*=Scale;
    Velocities[i].z*=Scale;
 
    (*Momentum).x+=Velocities[i].x;
    (*Momentum).y+=Velocities[i].y;
    (*Momentum).z+=Velocities[i].z;
 
    NewPositions[i].x=OldPositions[i].x+Velocities[i].x*2*Deltat;
    NewPositions[i].y=OldPositions[i].y+Velocities[i].y*2*Deltat;
    NewPositions[i].z=OldPositions[i].z+Velocities[i].z*2*Deltat;

    PositionsNONPDB[i].x+=NewPositions[i].x-Positions[i].x;
    PositionsNONPDB[i].y+=NewPositions[i].y-Positions[i].y;
    PositionsNONPDB[i].z+=NewPositions[i].z-Positions[i].z;
 
    UKinetic+=0.5*(SQR(Velocities[i].x)+SQR(Velocities[i].y)+SQR(Velocities[i].z));
 
    OldPositions[i].x=Positions[i].x;
    OldPositions[i].y=Positions[i].y;
    OldPositions[i].z=Positions[i].z;
 
    Positions[i].x=NewPositions[i].x;
    Positions[i].y=NewPositions[i].y;
    Positions[i].z=NewPositions[i].z;

    // put particles back in the box
    // the previous position has the same transformation (Why ?)
    if(Positions[i].x>=Box)
    {
      Positions[i].x-=Box;
      OldPositions[i].x-=Box;
    }
    else if (Positions[i].x<0.0)
    {
      Positions[i].x+=Box;
      OldPositions[i].x+=Box;
    }
 
    if(Positions[i].y>=Box)
    {
      Positions[i].y-=Box;
      OldPositions[i].y-=Box;
    }
    else if(Positions[i].y<0.0)
    {
      Positions[i].y+=Box;
      OldPositions[i].y+=Box;
    }
 
    if(Positions[i].z>=Box)
    {
      Positions[i].z-=Box;
      OldPositions[i].z-=Box;
    }
    else if(Positions[i].z<0.0)
    {
      Positions[i].z+=Box;
      OldPositions[i].z+=Box;
    }
  }
 
  // add the kinetic part of the pressure
  Pressure+=(2.0*UKinetic*NumberOfParticles)/(CUBE(Box)*((3.0*NumberOfParticles)-3.0));
}
