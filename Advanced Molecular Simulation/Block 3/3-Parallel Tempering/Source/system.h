#define SQR(x) ((x)*(x))
#define MIN(x,y) ((x)<(y)?(x):(y))
#define MAX(x,y) ((x)>(y)?(x):(y))

#define SWAP(x,y) {swap=x;x=y;y=swap;}
#define NUMBEROFPARTICLES 9
#define MAXNUMBEROFSYSTEMS 9

enum{INITIALIZE,SAMPLE,WRITE_RESULTS};

typedef struct
{
  double x;
  double y;
} VECTOR;

extern VECTOR Positions[NUMBEROFPARTICLES][MAXNUMBEROFSYSTEMS];
extern double Uold[MAXNUMBEROFSYSTEMS];

extern double Box;
extern int NumberOfSystems;

void Energy(double *Uconf,int k);
void SampleEnergy(int Switch);
void SampleXcoord(int Switch);
