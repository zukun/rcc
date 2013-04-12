

// max number of lines to print into timing file
#define MAXITER 500
//#define MAXITER 3000

// count energy truncations in energy.h
#ifdef COUNT_TRUNCATIONS
extern int truncCnt, totalCnt;
#endif


enum algtype {aICM, aExpansion, aSwap, aTRWS, aBPS, aBPM, aRunAll};
extern char *algs[];
extern clock_t algInitTimer;

FILE *createDebugFile(int writeParams, char *outstem, int verbose, int argc, char **argv);
FILE *createTimeFile(int writeTimings, char *outstem, char *alg, FILE* debugfile);
void startAlgInitTimer();
float getAlgInitTime();
void runAlg(MRF *mrf, int numAlg, FILE *debugfile, FILE *timefile, int outerIter, int innerIter, float initTime);
