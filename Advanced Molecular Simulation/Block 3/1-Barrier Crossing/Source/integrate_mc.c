#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "system.h"
#include "ran_uniform.h"

// Mc simulation
void IntegrateMonteCarlo(void)
{
  double Uold,Unew,F,NewPosition;
 
  Force(Position,&Uold,&F);
  NewPosition=Position+2.5*(RandomNumber()-0.5);
  Force(NewPosition,&Unew,&F);

  if(RandomNumber()<exp(-(Unew-Uold)/Temperature)) 
    Position=NewPosition;

  OldF=0.0;
  Velocity=0.0;
  ConservedEnergy=0.0;
}
