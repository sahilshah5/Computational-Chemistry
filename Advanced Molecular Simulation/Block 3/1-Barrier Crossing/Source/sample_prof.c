#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "system.h"

#define Maxx 500
void SampleProf(int Switch)
{
  int i;
  static double Dtot,Dens[Maxx],Delta;
  FILE *FilePtr;

  switch(Switch)
  {
    case 1:
      Delta=(Maxx-1.0)/4.0;
      Dtot=0.0;
      for(i=0;i<Maxx;i++)
        Dens[i]=0.0;
      break;
    case 2:
      Dtot+=1.0;
      i=(int)(Delta*(Position+2.0));
      if(i>=0&&i<Maxx)
      {
        Dens[i]+=1.0;
      }
      break;
    case 3:
      FilePtr=fopen("density.dat","w");
      for(i=0;i<Maxx;i++)
        if(Dens[i]>0)
          fprintf(FilePtr,"%f %f\n",i/Delta-2.0,Dens[i]/Dtot);
      fclose(FilePtr);
      break;
  }
}
