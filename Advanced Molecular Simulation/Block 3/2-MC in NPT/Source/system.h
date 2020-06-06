#include <stdio.h>

#define MAX_NUMBER_OF_PARTICLES 1000

#define FALSE 0
#define TRUE 1

#define SQR(x) ((x)*(x))
#define CUBE(x) ((x)*(x)*(x))

typedef struct
{
  double x;
  double y;
  double z;
} VECTOR;

extern VECTOR Positions[MAX_NUMBER_OF_PARTICLES];
extern VECTOR OldPositions[MAX_NUMBER_OF_PARTICLES];
extern int NumberOfParticles;

void WritePdb(FILE *FilePtr);
