#include <stdio.h>
#include <time.h>
#include "imageLib.h"
#include "mrf.h"
#include "runalgs.h"

char *algs[] = {"ICM", "Expansion", "Swap", "TRW-S", "BP-S", "BP-M"};
// BP-S is derived from TRW-S; BP-M is Marshall's

clock_t algInitTimer;


FILE *createDebugFile(int writeParams, char *outstem, int verbose, int argc, char **argv) 
{
    FILE *debugfile = NULL;
    if (verbose)
	debugfile = stderr;
    if (writeParams) {
	char paramname[1000];
	sprintf(paramname, "%s.txt", outstem);
	debugfile = fopen(paramname, "w");
	if (debugfile == NULL) 
	    throw CError("Cannot write to %s", paramname);
	if (verbose)
	    fprintf(stderr, "writing parameters to %s\n", paramname);
	for (int i=0; i<argc; i++) {
	    fprintf(debugfile, "%s ", argv[i]);
	}
	fprintf(debugfile, "\n");
    }
    return debugfile;
}


FILE *createTimeFile(int writeTimings, char *outstem, char *alg, FILE* debugfile) 
{
    FILE *timefile = NULL;
    if (writeTimings) {
	char timename[1000];
	sprintf(timename, "%s-%s.csv", outstem, alg); // comma-separated, to be read by Excel
	timefile = fopen(timename, "w");
	if (timefile == NULL) 
	    throw CError("Cannot write to %s", timename);
	if (debugfile)
	    fprintf(debugfile, "writing runtimes to %s\n", timename);
    }
    return timefile;
}

void startAlgInitTimer()
{
    algInitTimer = clock();
}

float getAlgInitTime()
{
    clock_t finish = clock();
    return (float) (((double)(finish - algInitTimer)) / CLOCKS_PER_SEC);
}

void runAlg(MRF *mrf, int numAlg, FILE *debugfile, FILE *timefile, int outerIter, int innerIter, float initTime)
{
    float tot_t = initTime;
    MRF::EnergyVal E, Ed, Es, Eold;
    Ed = mrf->dataEnergy();
    Es = mrf->smoothnessEnergy();
    E = Ed + Es; // mrf->totalEnergy();
    Eold = E;
    if (debugfile)
	fprintf(debugfile, "Energy = %d (Ed=%d, Es=%d) at start (%.3f secs init)\n", E, Ed, Es, tot_t);
    if (timefile) {
	fprintf(timefile, "sec,%s,%s\n", algs[numAlg], numAlg == aTRWS ? "lowerBound," : "");
	fprintf(timefile, "%.3f,%d,%s\n", tot_t, E, numAlg == aTRWS ? "," : "");
    }

    double lowerBound = 0;
    int iter;
    int noChange = 0;
    float t;
    for (iter = 0; iter < outerIter; iter++) {
	mrf->optimize(innerIter, t);
	tot_t += t ;
    
	Ed = mrf->dataEnergy();
	Es = mrf->smoothnessEnergy();
	E = Ed + Es; // mrf->totalEnergy();
	if (numAlg == aTRWS)
	    lowerBound = mrf->lowerBound();
	if (debugfile)
	    fprintf(debugfile, "Energy = %d (Ed=%d, Es=%d)", E, Ed, Es);
	if (numAlg == aTRWS && (debugfile))
	    fprintf(debugfile, ", lower bound = %.1f", lowerBound);
	if (debugfile)
	    fprintf(debugfile, ", %.3f secs\n", tot_t);
		
	if (timefile) {
	    if (numAlg == aTRWS)
		fprintf(timefile, "%.3f,%d,%.1f,\n", tot_t, E, lowerBound);
	    else
		fprintf(timefile, "%.3f,%d,\n", tot_t, E);
	}

	if (E == Eold) {
	    if (numAlg <= aSwap) // ICM, Expansion and Swap converge
		break;
	    noChange++;
	    if (numAlg != aTRWS && noChange >= 100) // unless TRW, quit after energy hasn't changed for 100 iterations
		break;
	} else
	    noChange = 0;

	if (numAlg != aTRWS && E > Eold) {
	    fprintf(stderr, "Warning: energy is increasing!\n");
	    if (debugfile)
		fprintf(debugfile, "Warning: energy is increasing!\n");
	}

	Eold = E;
    }

    if (timefile) {
	if (iter < outerIter)
	    iter++; // adjust for breaking out of loop
	while (iter < MAXITER) {
	    fprintf(timefile, ",,%s\n", numAlg == aTRWS ? "," : "");
	    iter++;
	}
	fclose(timefile);
    }
}
