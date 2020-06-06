#define MAXIMUM_NOSEHOOVER_CHAINLENGTH 100

#define SQR(x) ((x)*(x))
#define MIN(x,y) ((x)<(y)?(x):(y))

extern int NumberOfSteps;
extern int NumberOfInitializationSteps;
extern int NumberOfNoseHooverChains;
extern int Choice;

extern double Tstep;
extern double Nu;
extern double Temperature;
extern double Position;
extern double Velocity;
extern double ConservedEnergy;
extern double Freqnos;
extern double OldF;

void Force(double x,double *U,double *F);
void MdLoop(void);
void IntegrateAndersen(void);
void IntegrateMonteCarlo(void);
void IntegrateNVE(void);
void IntegrateNoseHoover(void);
void IntegrateRescaling(void);
void ReadData(void);
void SampleDiff(int Switch);
void SampleProf(int Switch);
void InitializeNoseHoover(void);
void NoseHooverNVT(double dt);
