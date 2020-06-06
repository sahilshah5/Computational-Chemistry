#include "system.h"

VECTOR Positions[MAXPART];        // positions
VECTOR OldPositions[MAXPART];    // old positions
VECTOR NewPositions[MAXPART];
VECTOR Velocities[MAXPART];        // velocities
VECTOR NewVelocities[MAXPART];
VECTOR Forces[MAXPART];           // forces
VECTOR NewForces[MAXPART];
VECTOR PositionsNONPDB[MAXPART]; // positions that are not put back in the box

double Deltat;         // Timestep

double Box;           // Boxlengths
double CutOff;        // Cut-Off Radius
double Ecut;          // Cut-Off Energy

double UKinetic;      // Kinetic Energy
double UPotential;    // Potential Energy
double UTotal;        // Total Energy

double Temperature;   // Temperature
double Pressure;      // Pressure

int NumberOfSteps;
int NumberOfInitializationSteps;              
int NumberOfParticles;  

